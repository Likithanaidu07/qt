#include "mysql_conn.h"
#include "QSettings"
#include <QStandardPaths>
#include "QDir"
#include <QSqlError>
#include "QSqlRecord"
#include <QRegularExpression>
#include "QHash"
#include <QtCore/QElapsedTimer>
#include "QHostAddress"
#include "QNetworkInterface"
//#include "UiUtils.h"
#include "contractdetail.h"
#include "PortFolio/PortfolioParser.h"
#include "mainwindow.h"
#include "ui_convert_algo_win.h"

#define DISABLE_MULTI_USER_LOGIN_CHECK
extern MainWindow *MainWindowObj;

//#include "CombinedTracker/combined_tracker_table_model.h"
userInfo mysql_conn::userDataStatic;


mysql_conn::mysql_conn(QObject *parent,QString conne_name):
    QObject(parent)
{
   // MainWindowObj = (MainWindow*)parent;
    //connect(this,SIGNAL(display_log_text_signal(QString)),MainWindowObj,SLOT(slotAddLogForAddAlgoRecord(QString)));
    portfolioCustomSort = new portfolioCustomSorting();

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    settings.beginGroup("DB_Details");
    HostName = settings.value("HostName").toString();
    Port = settings.value("Port").toInt();
    DatabaseName = settings.value("DatabaseName").toString();
    DB_UserName = settings.value("UserName").toString();
    Password = settings.value("Password").toString();
    settings.endGroup();
    settings.beginGroup("GeneralSettings");
    InstrumentTypeFilter = settings.value("InstrumentTypeFilter").toString();
    settings.endGroup();

    if (QSqlDatabase::contains(conne_name))
        db = QSqlDatabase::database(conne_name);
    else
        db = QSqlDatabase::addDatabase("QMYSQL",conne_name);

    db.setHostName(HostName);
    db.setPort(Port);
    db.setDatabaseName(DatabaseName);
    db.setUserName(DB_UserName);
    db.setPassword(Password);

    devicer = FO_DEVICER;
    decimal_precision = FO_DECIMAL_PRECISION;

    loadSettings();
}
mysql_conn::~mysql_conn(){
    delete portfolioCustomSort;
    if (db.isOpen()) {
        db.close();
    }
}
void mysql_conn::loadSettings(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
//    QStringList groups = settings.childGroups();
//    QString market_type = "fo";
//    if(groups.contains("GeneralSettings")){
//        settings.beginGroup("GeneralSettings");
//        if (settings.contains("market_type"))
//            market_type = settings.value("market_type").toString();
//        settings.endGroup();
//    }
//    if(market_type=="fo"){
//        devicer = FO_DEVICER;
//        decimal_precision = FO_DECIMAL_PRECISION;
//    }
//    else{
//        devicer = CDS_DEVICER;
//        decimal_precision = CDS_DECIMAL_PRECISION;
//    }
}

bool mysql_conn::checkDBOpened( QString &msg){
    bool ok =true;
    if (!db.isOpen())
        try {
            if (db.open()) {
                // Connection successful
                ok = true;
                msg="Database connected.";
            } else {
                // Connection failed
                ok = false;
                qDebug() << "getContractTable(): Failed to connect to MySQL server.";
                msg="Failed to connect to MySQL server.";
            }
        } catch (const QSqlError& error) {
            ok = false;
            // Exception occurred while trying to connect
            qDebug() <<"getContractTable(): "<< error.text();
            msg="Failed to connect to MySQL server: "+error.text();
        }
    return ok;
}

userInfo mysql_conn::login(  QString UserName_,   QString password)
{
    QMutexLocker lock(&mutex);
    userLoginInfo.loginResponse= "NOK";
    userLoginInfo.loggedIn = false;
    userLoginInfo.dbError = false;
    userLoginInfo.errorCode = T_LoginErroCode::NOK;
    {
        QString msg;
        bool ok = checkDBOpened(msg);
        userLoginInfo.loginResponse = msg;
        if(ok){


            QString queryStr = "SELECT rt_usertable.*, COALESCE(rms_table.exp_mar, 0) AS exp_mar FROM rt_usertable LEFT JOIN rms_table ON rms_table.foo_user_id = rt_usertable.UserID WHERE rt_usertable.UserName = '"+UserName_+"'";

            QSqlQuery query(db);
            query.prepare(queryStr);


            if(!query.exec())
            {
                // Error Handling, check query.lastError(), probably return
                qDebug()<<"login: "<<query.lastError().text();
                userLoginInfo.dbError = true;
                userLoginInfo.loginResponse= query.lastError().text();
            }
            else if (query.next()) {
                QSqlRecord rec = query.record();
                QString storedPassword = query.value(rec.indexOf("Password")).toString();
                if (storedPassword == password) {
                    QString IsActive = query.value(rec.indexOf("IsActive")).toString();
                    if(IsActive=="1"){

#ifndef DISABLE_MULTI_USER_LOGIN_CHECK
                    // check user already logged in from different system
                    QString query_str = "SELECT CASE WHEN NOT EXISTS (SELECT 1 FROM Logs) THEN 'No Logs' WHEN EXISTS (SELECT 1 FROM Logs l WHERE l.Log_ID > (SELECT Log_ID FROM Logs WHERE UserName = '"+UserName_+"' AND LogMessage LIKE '%User:"+UserName_+" logged in from%' ORDER BY Log_ID DESC LIMIT 1) AND l.UserName = '"+UserName_+"' AND l.LogMessage LIKE '%Logged Out%') THEN 'Logged Out' ELSE 'Still Logged In' END AS LoginStatus;";
                 //   qDebug()<<query_str;

                    QSqlQuery query2(query_str, db);

                    if (query2.exec()) {
                        if (query2.next()) {
                            QString loginStatus = query2.value("LoginStatus").toString();
                            if(loginStatus=="Logged Out"||loginStatus=="No Logs"){

                            }
                            else{
                                userLoginInfo.dbError = true;
                                userLoginInfo.loginResponse=  "User already logged in";
                                userLoginInfo.errorCode = T_LoginErroCode::DB_ERROR;
                                return userLoginInfo;
                            }


                            qDebug() << "Login Status:" << loginStatus;
                        } else {
                            qDebug() << "No results found.";
                        }
                    } else {
                        qDebug() << "Query execution error:" << query2.lastError().text();
                    }
#endif


                    userLoginInfo.UserName = query.value(rec.indexOf("UserName")).toString();
                    userNameLogged=userLoginInfo.UserName;

                    QStringList InstrumentTypeAll;
                    InstrumentTypeAll << "FUTIDX" << "OPTIDX" << "FUTSTK" << "OPTSTK";

                    QStringList OPT_Instruments;
                    OPT_Instruments  << "OPTIDX" <<  "OPTSTK";
                    QStringList FUT_Instruments;
                    FUT_Instruments  << "FUTSTK" <<  "FUTIDX";


                    QString BFLY_BIDInFilter = query.value(rec.indexOf("BFLY_BIDInFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::BFLY_BID] =  (BFLY_BIDInFilter.isNull() || BFLY_BIDInFilter.trimmed().isEmpty())  ? InstrumentTypeAll  : BFLY_BIDInFilter.split(",");

                    userLoginInfo.algoFilterMap[PortfolioType::BX_BID] =  OPT_Instruments;//(BOX_BIDInFilter.isNull() || BOX_BIDInFilter.trimmed().isEmpty())  ? InstrumentTypeAll  : BOX_BIDInFilter.split(",");

                    userLoginInfo.algoFilterMap[PortfolioType::BS1221] =  OPT_Instruments;
                    userLoginInfo.algoFilterMap[PortfolioType::BS1331] =  OPT_Instruments;
                    userLoginInfo.algoFilterMap[PortfolioType::OPEN2L] =  OPT_Instruments;

                    userLoginInfo.algoFilterMap[InstrumentType::FUT_INSTRUMENT] =  FUT_Instruments;


                    QString BYInFilter = query.value(rec.indexOf("BYInFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::BY] =  (BYInFilter.isNull() || BYInFilter.trimmed().isEmpty())  ? InstrumentTypeAll  : BYInFilter.split(",");

                    QString F2FInFilter = query.value(rec.indexOf("F2FInFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::F2F] =  (F2FInFilter.isNull() || F2FInFilter.trimmed().isEmpty())  ? InstrumentTypeAll  : F2FInFilter.split(",");

                    QString CRInFilter = query.value(rec.indexOf("CRInFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::CR] =  (CRInFilter.isNull() || CRInFilter.trimmed().isEmpty())  ? InstrumentTypeAll  : CRInFilter.split(",");


                    QString F1_F2InFilter = query.value(rec.indexOf("F1_F2InFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::F1_F2] =  (F1_F2InFilter.isNull() || F1_F2InFilter.trimmed().isEmpty())  ? InstrumentTypeAll  : F1_F2InFilter.split(",");

                    userLoginInfo.UserId = query.value(rec.indexOf("UserId")).toInt();
                    userLoginInfo.MaxPortfolioCount = query.value(rec.indexOf("MaxPortfolioCount")).toInt();
                    userLoginInfo.MaxActiveCount = query.value(rec.indexOf("MaxActiveCount")).toInt();
                    QString ExFilterPF = query.value(rec.indexOf("ExFilterPF")).toString();
                    userLoginInfo.ExFilterPF.append(ExFilterPF.split(","));  // Split by comma and append
                    userLoginInfo.loggedIn = true;
                    userLoginInfo.loginResponse = "Login Sucess";
                    userLoginInfo.errorCode = T_LoginErroCode::OK;
                    userLoginInfo.IDXOpenLimit = query.value(rec.indexOf("IDXOpenLimit")).toInt();
                    userLoginInfo.STKOpenLimit = query.value(rec.indexOf("STKOpenLimit")).toInt();
                    userLoginInfo.exp_mar =  query.value(rec.indexOf("exp_mar")).toLongLong();

                    //make the entry to logs table

                    QString localIP = "";
                    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
                    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
                        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
                            //qDebug() << address.toString();
                            localIP =  address.toString();
                        }
                    }
                    QString timeQueryStr = "SELECT UNIX_TIMESTAMP(NOW())";
                    QSqlQuery timeQuery(db);
                    qint64 serverTime = timeQuery.value(0).toLongLong();
                    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(serverTime);
                    QString humanReadableTime = dateTime.toString("hh:mm:ss");
                    QString qryStr = "INSERT INTO Logs (LogMessage, UserName, Time) VALUES (:logMessage, :userName, :formattedTime)";
                    QSqlQuery query1(db);
                    query1.prepare(qryStr);
                    query1.bindValue(":logMessage", "User:" + UserName_ + " logged in from [" + localIP + "]");
                    query1.bindValue(":userName", UserName_);
                    query1.bindValue(":formattedTime", humanReadableTime);
                    if (!query1.exec())
                    {
                        qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
                        qDebug() << "Query Str: " << qryStr;
                    }
                    }
                    //User is not active
                    else{
                        userLoginInfo.loggedIn = false;
                        userLoginInfo.loginResponse = "UserId is not active.";
                        userLoginInfo.errorCode = T_LoginErroCode::INACTIVE;

                        QString localIP = "";
                        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
                        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
                            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
                                //qDebug() << address.toString();
                                localIP =  address.toString();
                            }
                        }

                        QTime currentTime = QTime::currentTime();
                        QString formattedTime = currentTime.toString(LOG_TIME_FORMAT);
                        QString qryStr = "INSERT INTO Logs (LogMessage, UserName, Time) VALUES (:logMessage, :userName, :formattedTime)";
                        QSqlQuery query1(db);
                        query1.prepare(qryStr);
                        query1.bindValue(":logMessage", "Inactve User:" + UserName_ + " try to login from from [" + localIP + "]");
                        query1.bindValue(":userName", UserName_);
                        query1.bindValue(":formattedTime", QDateTime::currentDateTime().toSecsSinceEpoch());
                        if (!query1.exec())
                        {
                            qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
                            qDebug() << "Query Str: " << qryStr;
                        }
                    }


                }

                else
                {
                    userLoginInfo.loginResponse = "Wrong password";
                    userLoginInfo.errorCode = T_LoginErroCode::PASSWORD_WRONG;

                    QString localIP = "";
                    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
                    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
                        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
                            //qDebug() << address.toString();
                            localIP =  address.toString();
                        }
                    }

                    QTime currentTime = QTime::currentTime();
                    QString formattedTime = currentTime.toString(LOG_TIME_FORMAT);
                    QString qryStr = "INSERT INTO Logs (LogMessage, UserName, Time) VALUES (:logMessage, :userName, :formattedTime)";
                    QString LogMessage = "Invalid login attempt from "+localIP+" for user ["+UserName_+"] with wrong password ["+password+"]";
                    QSqlQuery query1(db);
                    query1.prepare(qryStr);
                    query1.bindValue(":logMessage", LogMessage);
                    query1.bindValue(":userName", UserName_);
                    query1.bindValue(":formattedTime", QDateTime::currentDateTime().toSecsSinceEpoch());

                    if (!query1.exec()) {
                        qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
                        qDebug() << "Query Str: " << qryStr;
                    }
                }
            }
            else{
                userLoginInfo.loginResponse = "Username not exist";
                userLoginInfo.errorCode = T_LoginErroCode::USERNAME_WRONG;
                QString localIP = "";
                const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
                for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
                    if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
                        //qDebug() << address.toString();
                        localIP =  address.toString();
                    }
                }

                QTime currentTime = QTime::currentTime();
                QString formattedTime = currentTime.toString(LOG_TIME_FORMAT);
                QString qryStr = "INSERT INTO Logs (LogMessage, UserName, Time) VALUES (:logMessage, :userName, :formattedTime)";
                QString LogMessage = "Invalid login attempt from "+localIP+" with wrong username ["+UserName_+"]";
                QSqlQuery query1(db);
                query1.prepare(qryStr);
                query1.bindValue(":logMessage", LogMessage);
                query1.bindValue(":userName", UserName_);
                query1.bindValue(":formattedTime", QDateTime::currentDateTime().toSecsSinceEpoch());

                if (!query1.exec()) {
                    qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
                    qDebug() << "Query Str: " << qryStr;
                }
            }
            db.close();
        }
        else{
            userLoginInfo.dbError = true;
            userLoginInfo.loginResponse= "Unable to connect the Server "/*+db.lastError().text()*/;
            userLoginInfo.errorCode = T_LoginErroCode::DB_ERROR;

        }
    }
    // QSqlDatabase::removeDatabase("login_conn");

    userDataStatic = userLoginInfo;
    return userLoginInfo;
}
void  mysql_conn::getSummaryTableData(int &OrderCount,QString user_id)
{
    QMutexLocker lock(&mutex);
    QString msg;
    OrderCount=0;

    bool ok = checkDBOpened(msg);
    if(ok){
         QString sqlquery ="SELECT COUNT(*) FROM Order_Table_Bid WHERE Order_Table_Bid.Trader_ID='"+user_id+"'";
         QSqlQuery query(sqlquery, db);
        if(!query.exec())
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<"getSummaryTableData: "<<query.lastError().text();
        }
        else{
            if (!query.next()) {
                qDebug() << "getBidCount: No results found";
                return; // Handle empty result set gracefully (optional)
            }

             OrderCount = query.value(0).toInt(); // Assuming the count is in the first column

        }
    }
}

QHash<QString,QString>  mysql_conn::getOrderDetailsData(QString  PortfolioNumberStr,QString user_id)
{
    QMutexLocker lock(&mutex);
    QString msg;
    QHash<QString,QString> OrderDetailsData;

    int PortfolioNumber = PortfolioNumberStr.toInt();

    QString portfolioNum1;
    QString portfolioNum2;
    if(PortfolioNumber<1500000){
        portfolioNum1 = PortfolioNumberStr;
        portfolioNum2 = QString::number(PortfolioNumber+1500000);
    }
    else{
        portfolioNum1 = PortfolioNumberStr;
        portfolioNum2 = QString::number(1500000-PortfolioNumber);
    }



    bool ok = checkDBOpened(msg);
    if(ok){


        QString sqlquery = QString(
            "SELECT "
            "    FORMAT("
            "        SUM("
            "            CASE "
            "                WHEN Trades.BuySellIndicator = 1 THEN (Trades.TradedPrice * Trades.TotalVolume) "
            "                ELSE 0 "
            "            END"
            "        ) / 100, "
            "        2"
            "    ) AS BuyValue, "
            "    FORMAT("
            "        SUM("
            "            CASE "
            "                WHEN Trades.BuySellIndicator = 2 THEN (Trades.TradedPrice * Trades.TotalVolume) "
            "                ELSE 0 "
            "            END"
            "        ) / 100, "
            "        2"
            "    ) AS SellValue, "
            "    ("
            "        SELECT "
            "            COUNT(*) "
            "        FROM "
            "            Order_Table_Bid "
            "        WHERE "
            "            Trader_ID = '%1' "
            "            AND Order_Table_Bid.PortfolioNumber IN (%2, %3)"
            "    ) AS OrderCount, "
            "    ("
            "        SELECT "
            "            COUNT(*) "
            "        FROM "
            "            Order_Table_Bid "
            "        WHERE "
            "            Trader_ID = '%1' "
            "            AND Order_Table_Bid.PortfolioNumber IN (%2, %3) "
            "            AND ("
            "                Leg1_OrderState = 7 OR Leg2_OrderState = 7 OR "
            "                Leg3_OrderState = 7 OR Leg4_OrderState = 7 OR "
            "                Leg5_OrderState = 7 OR Leg6_OrderState = 7"
            "            )"
            "    ) AS TraderCount "
            "FROM "
            "    Trades "
            "JOIN "
            "    Contract ON Contract.Token = Trades.TokenNo "
            "WHERE "
            "    Trades.PortfolioNumber IN (%2, %3) "
            "    AND Trades.BuySellIndicator IN (1, 2);"
        ).arg(user_id, portfolioNum1, portfolioNum2);


      //  qDebug()<<"query : "<<sqlquery;

         QSqlQuery query(sqlquery, db);
        if(!query.exec())
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<"getOrderDetailsData: "<<query.lastError().text();
            qDebug()<<"query : "<<sqlquery;

        }
        else{
            QSqlRecord rec = query.record();
            while (query.next()) {
                QString BuyTotalVal =query.value(rec.indexOf("BuyValue")).toString();
                QString SellTotalVal =query.value(rec.indexOf("SellValue")).toString();
                QString OrderCount =query.value(rec.indexOf("OrderCount")).toString();
                QString TraderCount =query.value(rec.indexOf("TraderCount")).toString();
                OrderDetailsData["BuyTotalVal"]=BuyTotalVal;
                OrderDetailsData["SellTotalVal"]=SellTotalVal;
                OrderDetailsData["OrderCount"]=OrderCount;
                OrderDetailsData["TraderCount"]=TraderCount;

            }

        }
    }

    return OrderDetailsData;
}






QHash<QString,PortFolioData_Less> mysql_conn::getPortfoliosTableData(QAtomicInt &reloadSortSettFlg,int &AlgoCount, Combined_Tracker_Table_Model *comb_tracker_model, QHash<QString, PortfolioAvgPrice> &averagePriceList, QString user_id, QStringList TradedPortFolioList,QStringList &PortFoliosToDelete )
{
    QMutexLocker lock(&mutex);
    // int editing_state = portfolio_table_editing.loadRelaxed();
    PortfolioParser p;
    QList<PortfolioObject*> PortfolioObjectList;
    QStringList algoNameList;

    QList<QStringList> Combined_TrackerDataList;
    QString msg;
    SlowData slowData;
    QHash<QString, MBP_Data_Struct>  MBP_Data_Hash = slowData.getMBP_Data_Hash();

    QHash<QString,PortFolioData_Less>  PortFolioDataHash;



    bool ok = checkDBOpened(msg);
    if(ok){
      // QString sqlquery = "SELECT PortfolioNumber, (sum(TradedPrice*TotalVolume) * 1.0 / sum(TotalVolume))/"+QString::number(devicer)+" as AvgPrice, TokenNo, BuySellIndicator from Trades WHERE TraderID='"+user_id+"' group by PortfolioNumber, TokenNo, BuySellIndicator ";


       /*QString sqlquery = "SELECT "
               "t.PortfolioNumber, "
               "(SUM(t.TradedPrice * (t.TotalVolume / c.LotSize)) * 1.0 / SUM(t.TotalVolume / c.LotSize)) / "+QString::number(devicer)+" AS AvgPrice, "
               "t.TokenNo, "
               "t.BuySellIndicator "
           "FROM "
              " Trades t "
           "JOIN "
              " Contract c ON t.TokenNo = c.Token "
           " WHERE "
             "  t.TraderID = '"+user_id+"' "
          " GROUP BY "
              " t.PortfolioNumber, "
               "t.TokenNo, "
               "t.BuySellIndicator; ";*/

     //  QString traderID = "2";  // Replace with actual value
      // double divisor = 100;    // Replace with actual value

       QString sqlquery = QString(
                              "SELECT "
                              "    t.PortfolioNumber, "
                              "    p.PortfolioType, "
                              "    (SUM(t.TradedPrice * t.TotalVolume) * 1.0 / SUM(t.TotalVolume)) / %1 AS AvgPrice, "
                              "    t.TokenNo, "
                              "    t.BuySellIndicator, "
                              "    o.Leg1_OrderState, "
                              "    o.Leg2_OrderState, "
                              "    o.Leg3_OrderState, "
                              "    o.Leg4_OrderState  "
                              "FROM Trades t "
                              "LEFT JOIN Portfolios p ON "
                              "    (CASE WHEN t.PortfolioNumber > 1500000 THEN t.PortfolioNumber - 1500000 ELSE t.PortfolioNumber END) = p.PortfolioNumber "
                              "LEFT JOIN Order_Table_Bid o ON t.LocalOrderNumber = o.Trader_Data "
                              "WHERE t.TraderID = '%2' "
                              "GROUP BY t.PortfolioNumber, p.PortfolioType, t.TokenNo, t.BuySellIndicator, o.Leg1_OrderState, o.Leg2_OrderState, o.Leg3_OrderState, o.Leg4_OrderState;"
                              ).arg(devicer).arg(user_id);


       QSqlQuery queryAvgPrice(sqlquery, db);
        if(!queryAvgPrice.exec())
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<"getContractTable->queryAvgPrice: "<<queryAvgPrice.lastError().text();
        }
        else{
            QSqlRecord rec = queryAvgPrice.record();
            while (queryAvgPrice.next()) {
                int portfolioNumber =queryAvgPrice.value(rec.indexOf("PortfolioNumber")).toInt();
                int token = queryAvgPrice.value(rec.indexOf("TokenNo")).toInt();
                int  portfoliotype = queryAvgPrice.value(rec.indexOf("PortfolioType")).toInt();
                int Leg1_OrderState = queryAvgPrice.value(rec.indexOf("Leg1_OrderState")).toInt();
                int Leg2_OrderState = queryAvgPrice.value(rec.indexOf("Leg2_OrderState")).toInt();
                int Leg3_OrderState = queryAvgPrice.value(rec.indexOf("Leg3_OrderState")).toInt();
                int Leg4_OrderState = queryAvgPrice.value(rec.indexOf("Leg4_OrderState")).toInt();
                bool traded = false;



                switch (portfoliotype) {
                case PortfolioType::F2F:{
                     if (Leg1_OrderState == 7 && Leg2_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }
                case PortfolioType::BY:{
                    if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }
                case PortfolioType::CR:{
                    if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }
                case PortfolioType::CR_JELLY:{
                   if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }
                case PortfolioType::BFLY_BID:{
                    if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }
                case PortfolioType::BX_BID: {
                    if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7 && Leg4_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }
                case PortfolioType::OPEN2L: {
                    if (Leg1_OrderState == 7 && Leg2_OrderState == 7 ) {
                        traded = true;
                    }

                    break;
                }
//                case PortfolioType::F1_F2: {

//                    int strikePriceLeg1 = ContractDetail::getInstance().GetStrikePrice(leg1_token_number, portfolio_type).toInt()* devicer;
//                    if (Leg1BuySellIndicator == 1) {
//                        Exch_Price_val = static_cast<double>(leg1Price ) / devicer;
//                    } else {
//                        Exch_Price_val = static_cast<double>(leg1Price) / devicer;

//                    }

//                    break;
              //  }

                case PortfolioType::BS1221: {

                if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7 && Leg4_OrderState == 7) {
                        traded = true;
                }
                    break;
                }
                case PortfolioType::BS1331: {
                   if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7 && Leg4_OrderState == 7) {
                        traded = true;
                    }

                    break;
                }

                default:
                    break;
                }

                if (!traded)
                    continue;

                QString key = QString::number(portfolioNumber)+"_"+QString::number(token);
                if(!averagePriceList.contains(key)){
                    PortfolioAvgPrice avgPrice;
                    avgPrice.PortfolioNumber = portfolioNumber;
                    avgPrice.TokenNo = token;
                    avgPrice.BuySellIndicator = queryAvgPrice.value(rec.indexOf("BuySellIndicator")).toString();
                    avgPrice.AvgPrice = queryAvgPrice.value(rec.indexOf("AvgPrice")).toDouble();
                    averagePriceList.insert(key,avgPrice);
                }
                else{
                    averagePriceList[key].PortfolioNumber = portfolioNumber;
                    averagePriceList[key].TokenNo = token;
                    averagePriceList[key].BuySellIndicator = queryAvgPrice.value(rec.indexOf("BuySellIndicator")).toString();
                    averagePriceList[key].AvgPrice = queryAvgPrice.value(rec.indexOf("AvgPrice")).toDouble();

                }
            }
        }
        // Print elapsed time

        QSqlQuery query("SELECT * from Portfolios WHERE TraderId='"+user_id+"'", db);

        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<"getPortfoliosTableData: "<<query.lastError().text();
        }
        else{
            QSqlRecord rec = query.record();
            //  QElapsedTimer timer;
            //  timer.start();
            // qDebug() << rec.count();
            //qDebug() <<"SELECT * from Portfolios WHERE TraderId='"+user_id+"'";

            while (query.next()) {
                // QString foo_token_number = query.value(rec.indexOf("foo_token_number")).toString();

                PortfolioType type = static_cast<PortfolioType>(query.value("PortfolioType").toInt());
                if(type==PortfolioType::F1_F2) // For F1_F2 do not show the result
                    continue;


                PortfolioObject *o = new PortfolioObject();
                if(!p.ToObject(query,*o,MBP_Data_Hash,averagePriceList,devicer,decimal_precision)){
                    //show warning an delete portfolio
                    PortFoliosToDelete.append(QString::number(o->PortfolioNumber));
                    delete o;

                    continue;
                }
                //o->BuyTotalQuantity = QDateTime::currentSecsSinceEpoch(); // this need to remove


                //check portfolio is trader
                if(TradedPortFolioList.contains(QString::number(o->PortfolioNumber)))
                    o->TradedHighlight = true;
                PortfolioObjectList.append(o);

                PortFolioData_Less P;
                P.PortfolioType = o->PortfolioType;
                P.Expiry =  o->Expiry;
                P.lotSize = o->GetLotSize();

                PortFolioDataHash.insert(QString::number(o->PortfolioNumber),P);


                QString sort_string = QString::number(o->Status)+"-"+o->AlgoNameForSorting;
                algoNameList.append(sort_string);

                //add to combined Tracker
                if(o->BuyTradedQuantity>0||o->SellTradedQuantity>0){
                    QStringList combineTrackerDataTmp(CombinedTrackerData_Length, "");
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Rule_No] = QString::number(o->PortfolioNumber);
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Symbol] = o->InstrumentName;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Strategy] =o->getPortFolioTypeString(o->PortfolioType);
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Expiry] = o->Expiry;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Type] = o->AdditionalData1;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Left_Leg] = o->Leg1;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Center_Leg] =  o->Leg2;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Right_Leg] =  o->Leg3;

                    int lotSize = o->GetLotSize();

                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Bid_Qty] = QString::number(o->BuyTradedQuantity * lotSize);
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Bid_Lots] = QString::number(o->BuyTradedQuantity);
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Bid_Avg_Price] = o->BuyAveragePrice;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Ask_Qty] = QString::number(o->SellTradedQuantity * lotSize);
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Ask_Lots] = QString::number(o->SellTradedQuantity);
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Ask_Avg_Price] = o->SellAveragePrice;

                    int profit = 1;
                    int BuyQuantity = o->BuyTradedQuantity * lotSize;
                    int SellQuantity = o->BuyTradedQuantity * lotSize;
                    int qty = SellQuantity > BuyQuantity ? BuyQuantity : SellQuantity;
                    if (qty > 0)
                        profit = (o->SellAveragePrice.toDouble() - o->BuyAveragePrice.toDouble())* qty;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Profit] = QString::number(profit);
                    Combined_TrackerDataList.append(combineTrackerDataTmp);
                }

            }
            // qDebug() << "Elapsed Time:" << timer.elapsed() << "milliseconds";

            AlgoCount = PortfolioObjectList.size();

            comb_tracker_model->setDataList(Combined_TrackerDataList);

            //if table is not editing, then only update data. Alos make sure table not edited while data is loading from sql
            //if(portfolio_table_editing.loadRelaxed()==0&&editing_state==portfolio_table_editing.loadRelaxed())

            //sort PortfolioObjectList based on the custom sort order
            bool clearTableFlg = false;
            if(portfolio_table_updating_db.loadRelaxed()==0){
                if(reloadSortSettFlg.loadRelaxed()==1){
                    reloadSortSettFlg.storeRelaxed(0);
                    portfolioCustomSort->loadSortConfig();
                    clearTableFlg=true;//model->clearTable();
                }
                QVector<int> sortRank = portfolioCustomSort->sortPortFolio(algoNameList);
                QList<PortfolioObject*> PortfolioObjectList_Sorted(PortfolioObjectList.size(), nullptr);

                   for (int i = 0; i < sortRank.size(); ++i) {
                       int newIndex = sortRank[i];
                       PortfolioObjectList_Sorted[newIndex] = PortfolioObjectList[i];
                   }
                PortfolioObjectList.clear();

                emit updateModelDataListSignal(PortfolioObjectList_Sorted,clearTableFlg);
                //model->setDataList(PortfolioObjectList_Sorted);
            }
        }
    }
    // qDebug()<<"getPortfoliosTableData ----------------mutex unlocked";
    return PortFolioDataHash;
}


QString mysql_conn::fixDecimal(double num,int decimal_precision){
    QString str = QString::number(num, 'f', decimal_precision);
    // str.remove( QRegularExpression("0+$") ); // Remove any number of trailing 0's
    // str.remove( QRegularExpression("\\.$") ); // If the last character is just a '.' then remove it
    return str;
}

void mysql_conn::logToDB(QString logMessage)
{
    QMutexLocker lock(&mutex);

    QString msg;
    bool ok = checkDBOpened(msg);
    if (ok) {
        // Query to get the database server's current time
        QString timeQueryStr = "SELECT UNIX_TIMESTAMP(NOW())";
        QSqlQuery timeQuery(db);
        if (!timeQuery.exec(timeQueryStr) || !timeQuery.next()) {
            qDebug() << "Failed to fetch server time: " << timeQuery.lastError().text();
            return;
        }
        // Fetch the server time
        qint64 serverTime = timeQuery.value(0).toLongLong();

        // Convert serverTime to human-readable format
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(serverTime);
        QString humanReadableTime = dateTime.toString("hh:mm:ss");
        QString qryStr = "INSERT INTO Logs (LogMessage, UserName, Time) VALUES (:logMessage, :userName, :formattedTime)";
        QSqlQuery query1(db);
        query1.prepare(qryStr);
        query1.bindValue(":logMessage", logMessage);
        query1.bindValue(":userName", userNameLogged);
        query1.bindValue(":formattedTime", humanReadableTime); // Use formatted time

        if (!query1.exec()) {
            qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
            qDebug() << "Query Str: " << qryStr;
        }
        QString serverTimeFormatted = dateTime.toString("hh:mm:ss");
        QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.0;'>"
                              "<span>" + serverTimeFormatted +
                              "&nbsp;</span><span style='font-weight:400;/*color: white;*/'>" + logMessage + "</span></p>";
        emit display_log_text_signal(htmlContent);

    } else {
        qDebug() << "logToDB : " << msg;
    }
}



void mysql_conn::loadCurrentDayLogs()
{
    QMutexLocker lock(&mutex);
    QString msg;
    bool ok = checkDBOpened(msg);
    if(ok)
    {
        QSqlQuery selQry(db);

        auto currentTime = std::chrono::system_clock::now();

        //to get midnight time
        auto midnight = std::chrono::time_point_cast<std::chrono::hours>(std::chrono::system_clock::from_time_t(std::chrono::system_clock::to_time_t(currentTime)));

        //to get duration of hours from midnight to now
        auto duration = currentTime - midnight;
        auto totalHours = std::chrono::duration_cast<std::chrono::hours>(duration).count();
        QString queryStr = "SELECT * FROM logs WHERE Time BETWEEN UNIX_TIMESTAMP(CURDATE()) AND UNIX_TIMESTAMP(NOW() - INTERVAL " +
                           QString::number(totalHours) + " HOUR)";

        selQry.prepare(queryStr);
        if (!selQry.exec())
        {
            qDebug() << "Select Query failed: " << selQry.lastError().text();
            qDebug() << "Query : " << queryStr;
        }
        QSqlRecord rec = selQry.record();
        while (selQry.next())
        {
            QString logMessage = selQry.value(rec.indexOf("LogMessage")).toString();
            QString timeString = selQry.value(rec.indexOf("Time")).toString();
            qint64 epochTime = timeString.toLongLong();
            QDateTime dateTime = QDateTime::fromSecsSinceEpoch(epochTime);
            QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.0'>"
                                  "<span>" + QTime::currentTime().toString("hh:mm:ss")
                                  + "&nbsp;</span><span style='font-weight:400;/*color: white;*/'>"+ logMessage.prepend("  ") + "</span></p>";
            emit display_log_text_signal(htmlContent);
        }
    }
}

QHash<QString, contract_table> mysql_conn::prpareContractDataFromDB(QString queryStr, QSqlDatabase *db, QStringList &tokenData)
{
    QHash<QString, contract_table> contract_table_hash;

    QSqlQuery query(queryStr, *db);
    if( !query.exec() )
    {
        // Error Handling, check query.lastError(), probably return
        qDebug()<<"getContractTable: "<<query.lastError().text();
    }
    else{
        QSqlRecord rec = query.record();
        while (query.next())
        {
            contract_table contractTableTmp;
            contractTableTmp.InstrumentType = query.value(rec.indexOf("InstrumentType")).toString();
            contractTableTmp.InstrumentName = query.value(rec.indexOf("InstrumentName")).toString();
            contractTableTmp.OptionType = query.value(rec.indexOf("OptionType")).toString();
            contractTableTmp.StrikePrice = query.value(rec.indexOf("StrikePrice")).toInt();
            contractTableTmp.LotSize = query.value(rec.indexOf("LotSize")).toInt();
            contractTableTmp.Expiry = query.value(rec.indexOf("ExpiryDate")).toLongLong();
            contractTableTmp.TokenNumber = query.value(rec.indexOf("Token")).toInt();
            contractTableTmp.StockName = query.value(rec.indexOf("StockName")).toString();
            contractTableTmp.MinimumSpread = query.value(rec.indexOf("MinSpread")).toInt();
            contractTableTmp.VolumeFreezeQty = query.value(rec.indexOf("VolumeFreezeQty")).toDouble();
            contractTableTmp.OperatingRangeslowPriceRange = query.value(rec.indexOf("OperatingRangeslowPriceRange")).toInt();
            contractTableTmp.OperatingRangeshighPriceRange = query.value(rec.indexOf("OperatingRangeshighPriceRange")).toInt();



            tokenData.append(QString::number(contractTableTmp.TokenNumber));
            contract_table_hash.insert( query.value(rec.indexOf("Token")).toString(), contractTableTmp);

        }
    }
    return contract_table_hash;
}

QSqlQuery mysql_conn::runQuery(QString qry_str){

    QMutexLocker lock(&mutex);

    QString msg;
    QSqlQuery retQ;
    bool ok = checkDBOpened(msg);
    if(ok){
        QSqlQuery query(qry_str, db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<"runQuery: "<<query.lastError().text();
        }

        return query;
    }
    return QSqlQuery();

}


bool mysql_conn::resetPassword(const QString &new_password, QString user_id,QString &msg)
{
   // QString reset_querystr;
   // QString reset_querystr = "UPDATE rt_usertable SET OldPassword03 = OldPassword02,OldPassword02 = OldPassword01,OldPassword01 = Password, Password = '"+new_password+"' WHERE UserId = '"+user_id+"'";
    QString reset_querystr = "UPDATE rt_usertable SET OldPassword03 = OldPassword02, OldPassword02 = OldPassword01, OldPassword01 = Password, Password = '"+new_password+"' WHERE UserId ='"+user_id+"';";
   // QSqlQuery query(reset_querystr, db);

    qDebug()<<"DB Update Query: "<<reset_querystr;
    QMutexLocker lock(&mutex);

    bool ret = false;


    bool ok = checkDBOpened(msg);
    if(ok){

        QSqlQuery query(reset_querystr, db);
        if( !query.exec() )
        {

            msg = /*reset_querystr+*/" Unable to connect the Server "/*+ query.lastError().text()*/;
            ret = false;
        }
        else{
            qDebug()<<"Updated Data base successfully.";
            ret = true;
        }

    }

    return ret;
}


bool mysql_conn::modifytype(QString value,QString user_id,QString &msg)
{
    // QString reset_querystr;
    // QString reset_querystr = "UPDATE rt_usertable SET OldPassword03 = OldPassword02,OldPassword02 = OldPassword01,OldPassword01 = Password, Password = '"+new_password+"' WHERE UserId = '"+user_id+"'";
    QString modify_querystr = "UPDATE Portfolios SET ModifyType = '" + value + "' WHERE TraderID = '" +user_id + "';";

    // QSqlQuery query(reset_querystr, db);

    qDebug()<<"DB Update Query: "<<modify_querystr;
    QMutexLocker lock(&mutex);

    bool ret = false;


    bool ok = checkDBOpened(msg);
    if(ok){

        QSqlQuery query(modify_querystr, db);
        if( !query.exec() )
        {

            msg = /*modify_querystr+*/" Unable to connect the Server "/*+ query.lastError().text()*/;
            ret = false;
        }
        else{
            qDebug()<<"Updated Data base successfully.";
            ret = true;
        }

    }

    return ret;
}

bool mysql_conn::updateDB_Table(QString query_str, QString &msg)
{
    qDebug()<<"DB Update Query: "<<query_str;
    QMutexLocker lock(&mutex);

    bool ret = false;


    bool ok = checkDBOpened(msg);
    if(ok){

        QSqlQuery query(query_str, db);
        if( !query.exec() )
        {

            msg =/* query_str+*/" Unable to connect the Server "/*+ query.lastError().text()*/;
            ret = false;
        }
        else{
            qDebug()<<"Updated Data base successfully.";
            ret = true;
        }

    }

    return ret;
}

QString mysql_conn::get_Algo_Name(int algo_type, int leg1_token_number, int leg2_token_number, int leg3_token_number, double devicer, int decimal_precision)
{
    QString Algo_Name="-";
    if(algo_type==PortfolioType::F2F){
        Algo_Name = "F2F-";//-Nifty";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"MMM",algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"MMM",algo_type);

    }
    else if(algo_type==PortfolioType::BY){
        Algo_Name = "Bfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::OPEN_BY){
        Algo_Name = "OpenBfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::CR){
        Algo_Name = "CR-";//Nifty-17000";
        //            QString StockName = ContractDetail::getInstance().GetStockName(leg2_token_number);
        //            StockName.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::CR_JELLY){
        Algo_Name = "CRJELLY-";//Nifty-17000";
        //            QString StockName = ContractDetail::getInstance().GetStockName(leg2_token_number);
        //            StockName.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::BOX){
        Algo_Name = "BOX-";//18100-18200";
        //            QString StockNameLeg1 = ContractDetail::getInstance().GetStockName(leg1_token_number);
        //            StockNameLeg1.chop(2);
        //            QString StockNameLeg3 = ContractDetail::getInstance().GetStockName(leg3_token_number);
        //            StockNameLeg3.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::OPEN_BOX){
        Algo_Name = "OpenBOX-";//18100-18200";
        //            QString StockNameLeg1 = ContractDetail::getInstance().GetStockName(leg1_token_number);
        //            StockNameLeg1.chop(2);
        //            QString StockNameLeg3 = ContractDetail::getInstance().GetStockName(leg3_token_number);
        //            StockNameLeg3.chop(2);
        Algo_Name = Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::BFLY_BID){
        Algo_Name = "Bfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }

    else if(algo_type==PortfolioType::BX_BID){
        Algo_Name = "BX-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-"+QString::number(diff)/*+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type)*/;
    }

    else if(algo_type==PortfolioType::BS1221){
        Algo_Name = "BS12-";//Nifty-18000-CE-200";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+
                    ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+
                    ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+
                    ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type) +"-"+
                    ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);    }
    else if(algo_type==PortfolioType::BS1331){
        Algo_Name = "BS13-";//Nifty-18000-CE-200";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+
                    ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+
                    ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+
                    ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type) +"-"+
                    ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::F1_F2){
        Algo_Name = "F1_F2-";//Nifty-18000-CE-200";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"MMM",algo_type);
    }

    else if(algo_type==PortfolioType::OPEN2L){
        Algo_Name = "Open2L-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }


    return Algo_Name.toUpper();
}

//  QList<QHash<QString,QString>>  mysql_conn::getOrderPopUPData(QString user_id, QString PortfolioNumberStr,QString PortfolioType){

//    QList<QHash<QString,QString>> orderData;
//    QMutexLocker lock(&mutex);

//    QList <QStringList> orderData_listTmp;
//    QString msg;

//    int PortfolioNumber = PortfolioNumberStr.toInt();

//    QString portfolioNum1;
//    QString portfolioNum2;
//    if(PortfolioNumber<1500000){
//        portfolioNum1 = PortfolioNumberStr;
//        portfolioNum2 = QString::number(PortfolioNumber+1500000);
//    }
//    else{
//        portfolioNum1 = PortfolioNumberStr;
//        portfolioNum2 = QString::number(1500000-PortfolioNumber);
//    }

//    bool ok = checkDBOpened(msg);
//    if(ok)
//    {

//        QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' and Leg2_OrderState=7 and PortfolioNumber IN ("+portfolioNum1+", "+portfolioNum2+")  ORDER BY Trader_Data DESC";
//        qDebug()<<query_str;

//        QSqlQuery query(query_str,db);
//        if( !query.exec() )
//        {
//            // Error Handling, check query.lastError(), probably return
//            qDebug()<<query.lastError().text();
//        }
//        else{
//            QSqlRecord rec = query.record();
//            while (query.next()) {

//                QString Algo_ID = ""; // same as PortfolioNumber
//                QString Buy_Sell = "";


//                int Algo_ID_Int = query.value(rec.indexOf("PortfolioNumber")).toInt();

//                if(Algo_ID_Int>1500000){
//                    Buy_Sell = "Buy";
//                    Algo_ID = QString::number(Algo_ID_Int - 1500000);
//                }
//                else{
//                    Buy_Sell = "Sell";
//                    Algo_ID = QString::number(Algo_ID_Int);
//                }

//                QString Volant_No = query.value(rec.indexOf("Trader_Data")).toString();
//                int leg1_token_number = query.value(rec.indexOf("Leg1_Tok_No")).toInt();
//                int leg2_token_number = query.value(rec.indexOf("Leg2_Tok_No")).toInt();
//                int leg3_token_number = query.value(rec.indexOf("Leg3_Tok_No")).toInt();

//                int portfolio_type = PortfolioType.toInt();
//                QString Algo_Name = "";


//                 Algo_Name =get_Algo_Name(portfolio_type,leg1_token_number,leg2_token_number,leg3_token_number,devicer,decimal_precision);


//                int qty = query.value(rec.indexOf("Leg1_Total_Volume")).toInt();
//                //int leg2qty = query.value(rec.indexOf("Leg2_Total_Volume")).toInt();
//                int leg1Price = query.value(rec.indexOf("Leg1_Price")).toInt();
//                int leg2Price = query.value(rec.indexOf("Leg2_Price")).toInt();
//                int leg3Price = query.value(rec.indexOf("Leg3_Price")).toInt();
//                int Leg1BuySellIndicator = query.value(rec.indexOf("Leg1_Buy/Sell")).toInt();

//                QString Exch_Price = "0";
//                double Exch_Price_val  = 0;

//                switch (portfolio_type) {
//                case PortfolioType::F2F:{
//                    if(Leg1BuySellIndicator==1){
//                        Exch_Price_val = static_cast<double>((leg2Price - leg1Price) * 1.0) / devicer;
//                    }
//                    else{
//                        Exch_Price_val = static_cast<double>((leg1Price - leg2Price) * 1.0) / devicer;
//                    }

//                    break;

//                }
//                case PortfolioType::BY:{
//                    if(Leg1BuySellIndicator==1){
//                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
//                    }
//                    else{
//                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
//                    }

//                    break;
//                }
//                case PortfolioType::CR:{
//                    int strikePrice = ContractDetail::getInstance().GetStrikePrice(leg2_token_number,portfolio_type).toInt();
//                    if(Leg1BuySellIndicator==1){
//                        float diff = -leg1Price - leg3Price + leg2Price + strikePrice;
//                        Exch_Price_val = static_cast<double>(diff) / devicer;
//                    }
//                    else{
//                        float diff = -leg2Price - strikePrice + leg3Price + leg1Price;
//                        Exch_Price_val = static_cast<double>(diff) / devicer;
//                    }


//                    break;
//                }
//                case PortfolioType::CR_JELLY:{
//                    int strikePrice = ContractDetail::getInstance().GetStrikePrice(leg2_token_number,portfolio_type).toInt();
//                    if(Leg1BuySellIndicator==1){
//                        float diff = -leg1Price - leg3Price + leg2Price + strikePrice;
//                        Exch_Price_val = static_cast<double>(diff) / devicer;
//                    }
//                    else{
//                        float diff = -leg2Price - strikePrice + leg3Price + leg1Price;
//                        Exch_Price_val = static_cast<double>(diff) / devicer;
//                    }


//                    break;
//                }
//                case PortfolioType::BFLY_BID:{
//                    if(Leg1BuySellIndicator==1){
//                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
//                    }
//                    else{
//                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
//                    }

//                    break;
//                }

//                case PortfolioType::BX_BID:{
//                    if(Leg1BuySellIndicator==1){
//                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
//                    }
//                    else{
//                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
//                    }

//                    break;
//                }

//                case PortfolioType::BS1221:{
//                    if(Leg1BuySellIndicator==1){
//                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
//                    }
//                    else{
//                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
//                    }

//                    break;
//                }

//                default:
//                    break;
//                }

//                Exch_Price = QString::number(Exch_Price_val,'f',decimal_precision);
//                QString User_Price = fixDecimal(((query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer),decimal_precision);//QString::number(static_cast<double>(query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer, 'f', decimal_precision+1);

//                double userPriceVal = query.value(rec.indexOf("DesiredRate")).toDouble() / devicer;
//                double JackpotVal =(Exch_Price_val-userPriceVal);
//                QString Jackpot = QString::number(JackpotVal,'f',decimal_precision);


//                int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);

//                if(qty>0&&lotSize>0) // to prevent crash
//                    qty = qty / lotSize;
//                QString Traded_Lot = QString::number(qty);

//                QString Remaining_Lot = QString::number(static_cast<double>(query.value(rec.indexOf("RemainingQty")).toDouble()) / lotSize);
//                long long Trade_Time = query.value(rec.indexOf("TimeOrderEnteredHost")).toLongLong();
//                QDateTime dt = QDateTime::fromSecsSinceEpoch(Trade_Time);
//                dt = dt.toUTC();

//                QHash<QString,QString> tmp;
//                tmp.insert("Algo_ID",Algo_ID);
//                tmp.insert("Algo_Name",Algo_Name);
//                tmp.insert("Exch_Price",Exch_Price);
//                tmp.insert("User_Price",User_Price);
//                tmp.insert("Jackpot",Jackpot);
//                tmp.insert("Traded_Lot",Traded_Lot);
//                tmp.insert("Remaining_Lot",Remaining_Lot);
//                tmp.insert("Buy_Sell",Buy_Sell);
//                tmp.insert("Time",dt.toString("hh:mm:ss"));

//                orderData.append(tmp);
//            }

//        }
//    }
//    return orderData;
//}



QList<QHash<QString,QString>>  mysql_conn::getTradePopUPData(QString user_id, QString localOrderID,int lotSize){

    QList<QHash<QString,QString>> tradeData;
    QMutexLocker lock(&mutex);

    QList <QStringList> trade_data_listTmp;
    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok)
    {
        //QString query_str = "SELECT StockName,LocalOrderNumber,PortfolioNumber,BuySellIndicator,TradedPrice,TotalVolume,TimeOrderEnteredHost FROM Trades WHERE TraderId='"+user_id+"' and LocalOrderNumber="+localOrderID ;
        QString query_str = "SELECT StockName, LocalOrderNumber, PortfolioNumber, BuySellIndicator, TradedPrice, TotalVolume, TimeOrderEnteredHost, TradeNumber FROM Trades WHERE TraderId = '" + user_id + "' AND LocalOrderNumber = " + localOrderID + " ORDER BY TradeNumber ASC;";

        QSqlQuery query(query_str,db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<query.lastError().text();
        }
        else{
            QSqlRecord rec = query.record();
            while (query.next()) {




                QString Buy_Sell = query.value(rec.indexOf("BuySellIndicator")).toString();
                if(Buy_Sell=="1")
                    Buy_Sell = "Buy";
                else
                    Buy_Sell = "Sell";

                QString StockName = query.value(rec.indexOf("StockName")).toString();
                QString OrderId = query.value(rec.indexOf("LocalOrderNumber")).toString();
                int  AlgoNo = query.value(rec.indexOf("PortfolioNumber")).toInt();
                QString AlgoNoStr = "";
                if(AlgoNo>1500000){
                    AlgoNoStr = QString::number(AlgoNo - 1500000);
                }
                else
                    AlgoNoStr = QString::number(AlgoNo);
                QString TradedPrice = fixDecimal(((query.value(rec.indexOf("TradedPrice")).toDouble()) / devicer),decimal_precision);//QString::number(static_cast<double>(query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer, 'f', decimal_precision+1);

                int qty = query.value(rec.indexOf("TotalVolume")).toInt();


                if(qty>0&&lotSize>0) // to prevent crash
                    qty = qty / lotSize;
                QString Lots = QString::number(qty);
                long long Trade_Time = query.value(rec.indexOf("TimeOrderEnteredHost")).toLongLong();
                QDateTime dt = QDateTime::fromSecsSinceEpoch(Trade_Time);
                dt = dt.toUTC();
                QString Time = dt.toString("hh:mm:ss");

                QHash<QString,QString> tmp;
                tmp.insert("Buy_Sell",Buy_Sell);
                tmp.insert("OrderId",OrderId);
                tmp.insert("AlgoNo",AlgoNoStr);
                tmp.insert("StockName",StockName);
                tmp.insert("Price",TradedPrice);
                tmp.insert("Lots",Lots);
                tmp.insert("Time",Time);
                tradeData.append(tmp);
            }

        }
    }
    return tradeData;

}
void mysql_conn::getTradeTableData(int &TraderCount,Trade_Table_Model *trade_table,
                                   Order_F1_F2_Model *f1f2_order_table_model,Liners_Model *liners_model ,QString user_id,
                                   QHash<QString, PortFolioData_Less> PortFolioTypeHash,QStringList &algosToDisable,
                                   QStringList ExecutedTableHilightExcludeList,int &traderData_ID_OnAppStart,
                                   QStringList &newlyTradeAlgos)
{
    QMutexLocker lock(&mutex);
    QList <QStringList> trade_data_listTmp;
    QList <QStringList> f1f2_trade_data_listTmp;

    // QList <QStringList> algo_pos_data_listTmp;
    QList <QStringList> liners_listTmp;
    QHash<QString, Liners_Data> Liners_Data_Hash;
    QString msg;
    TraderCount = 0;
    bool ok = checkDBOpened(msg);
    if(ok)
    {

       // QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' and (Leg1_OrderState=7 and Leg2_OrderState=7 and Leg3_OrderState=7) ORDER BY Trader_Data DESC";
       // QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' and  Leg1_OrderState=7 or Leg2_OrderState=7 or Leg3_OrderState=7 or Leg4_OrderState = 7  ORDER BY Trader_Data DESC";
        //QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' AND (Leg1_OrderState=7 OR Leg2_OrderState=7 OR Leg3_OrderState=7 OR Leg4_OrderState=7) ORDER BY Trader_Data DESC";
//        QString query_str =
//            "SELECT O.*, P.PortfolioType "
//            "FROM Order_Table_Bid O "
//            "INNER JOIN Portfolios P "
//            "ON CASE "
//            "    WHEN O.PortfolioNumber > 1500000 THEN O.PortfolioNumber - 1500000 "
//            "    ELSE O.PortfolioNumber "
//            "END = P.PortfolioNumber "
//            "WHERE O.Trader_ID='" + user_id + "' "
//            "AND (O.Leg1_OrderState=7 OR O.Leg2_OrderState=7 OR O.Leg3_OrderState=7 OR O.Leg4_OrderState=7) "
//            "ORDER BY O.Trader_Data DESC";
 //       QString query_str =
//            "SELECT O.*, P.PortfolioType "
//            "FROM Order_Table_Bid O "
//            "INNER JOIN Portfolios P "
//            "ON CASE "
//            "    WHEN O.PortfolioNumber > 1500000 THEN O.PortfolioNumber - 1500000 "
//            "    ELSE O.PortfolioNumber "
//            "END = P.PortfolioNumber "
//            "WHERE O.Trader_ID='" + user_id + "' "
//                        "AND (O.Leg1_OrderState IN (7, 8) OR "
//                        "     O.Leg2_OrderState IN (7, 8) OR "
//                        "     O.Leg3_OrderState IN (7, 8) OR "
//                        "     O.Leg4_OrderState IN (7, 8)) "
//                        "ORDER BY O.Trader_Data DESC";

        /*QString query_str =
            "SELECT O.*, "
            "P.PortfolioType, "
            "P.MaxLoss, "
            "P.Leg1TokenNo, "
            "P.Leg2TokenNo, "
            "P.Leg3TokenNo, "
            "P.Leg4TokenNo, "
            "P.Leg5TokenNo, "
            "P.Leg6TokenNo "
            "FROM Order_Table_Bid O "
            "INNER JOIN Portfolios P "
            "ON CASE "
            "    WHEN O.PortfolioNumber > 1500000 THEN O.PortfolioNumber - 1500000 "
            "    ELSE O.PortfolioNumber "
            "END = P.PortfolioNumber "
            "WHERE O.Trader_ID='" + user_id + "' "
            "AND (O.Leg1_OrderState IN (7, 8) OR "
            "     O.Leg2_OrderState IN (7, 8) OR "
            "     O.Leg3_OrderState IN (7, 8) OR "
            "     O.Leg4_OrderState IN (7, 8)) "
            "ORDER BY O.Trader_Data DESC;" ;*/

        QString query_str =
            "SELECT O.*, "
            "       P.PortfolioType, "
            "       P.MaxLoss, "
            "       P.Leg1TokenNo, "
            "       P.Leg2TokenNo, "
            "       P.Leg3TokenNo, "
            "       P.Leg4TokenNo, "
            "       P.Leg5TokenNo, "
            "       P.Leg6TokenNo "
            "FROM Order_Table_Bid O "
            "INNER JOIN Portfolios P "
            "ON CASE "
            "    WHEN O.PortfolioNumber > 1500000 THEN O.PortfolioNumber - 1500000 "
            "    ELSE O.PortfolioNumber "
            "END = P.PortfolioNumber "
            "WHERE O.Trader_ID = '" + user_id + "' "
            "AND ("
            "    (P.PortfolioType IN (208, 207) AND O.Leg1_OrderState IN (5, 6, 7, 8, 13)) "
            "    OR "
            "    (P.PortfolioType NOT IN (208, 207) AND O.Leg1_OrderState IN (7, 8)) "
            "    OR "
            "    O.Leg2_OrderState IN (7, 8) "
            "    OR "
            "    O.Leg3_OrderState IN (7, 8) "
            "    OR "
            "    O.Leg4_OrderState IN (7, 8)"
            ") "
            "ORDER BY O.Trader_Data DESC;";



        //QString sqlquery ="SELECT COUNT(*) FROM Order_Table_Bid WHERE Order_Table_Bid.Trader_ID='"+user_id+"'";
        //QString query_str = "SELECT COUNT(*) FROM (SELECT * FROM Order_Table_Bid WHERE Trader_ID='" + user_id + "' AND Leg2_OrderState=7 ORDER BY Trader_Data DESC) AS SubQuery";

        QSqlQuery query(query_str,db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<query.lastError().text();
        }
        else{
            QSqlRecord rec = query.record();
            while (query.next()) {

                QString Algo_ID = ""; // same as PortfolioNumber
                QString Buy_Sell = "";

                int Algo_ID_Int = query.value(rec.indexOf("PortfolioNumber")).toInt();

                QString traderData =  query.value(rec.indexOf("Trader_Data")).toString();
                double maxLossThr = query.value(rec.indexOf("MaxLoss")).toDouble();

                QString TradeTable_highlight = "1";
                if (ExecutedTableHilightExcludeList.contains(traderData)) {
                    TradeTable_highlight = "0";
                }

                if(Algo_ID_Int>1500000){
                    Buy_Sell = "Buy";
                    Algo_ID = QString::number(Algo_ID_Int - 1500000);
                }
                else{
                    Buy_Sell = "Sell";
                    Algo_ID = QString::number(Algo_ID_Int);
                }

                //int portfolio_type = -1;
                int portfolio_type = query.value(rec.indexOf("PortfolioType")).toInt();

                /*if(PortFolioTypeHash.contains(Algo_ID)){
                    PortFolioData_Less P = PortFolioTypeHash[Algo_ID];
                    portfolio_type = P.PortfolioType.toInt();
                }
                else{
                    continue;
                }*/

                //qDebug()<<"portfolio_type: "<<portfolio_type <<" Algo_ID: "<<Algo_ID;

                int Leg1_OrderState = query.value(rec.indexOf("Leg1_OrderState")).toInt();
                int Leg3_OrderState = query.value(rec.indexOf("Leg3_OrderState")).toInt();
                int Leg2_OrderState = query.value(rec.indexOf("Leg2_OrderState")).toInt();
                int Leg4_OrderState = query.value(rec.indexOf("Leg4_OrderState")).toInt();

//                bool skipRecord = true;

//                if((portfolio_type==PortfolioType::BX_BID)&&(Leg1_OrderState==7||Leg2_OrderState==7||Leg3_OrderState==7||Leg4_OrderState==7))
//                        skipRecord = false;
//                if(portfolio_type!=PortfolioType::BX_BID&&Leg2_OrderState==7)
//                        skipRecord = false;

//                if(skipRecord==true)
//                  continue;

                QString Trader_Data = query.value(rec.indexOf("Trader_Data")).toString();
                int leg1_token_number = query.value(rec.indexOf("Leg1TokenNo")).toInt();
                int leg2_token_number = query.value(rec.indexOf("Leg2TokenNo")).toInt();
                int leg3_token_number = query.value(rec.indexOf("Leg3TokenNo")).toInt();
                int leg4_token_number = query.value(rec.indexOf("Leg4TokenNo")).toInt();

                QString Algo_Name = "";
                QString Expiry = "";



                PortFolioData_Less P = PortFolioTypeHash[Algo_ID];
                Expiry = P.Expiry;
                Algo_Name =get_Algo_Name(portfolio_type,leg1_token_number,leg2_token_number,leg3_token_number,devicer,decimal_precision);


                int qty = query.value(rec.indexOf("Leg1_Total_Volume")).toInt();
                //int leg2qty = query.value(rec.indexOf("Leg2_Total_Volume")).toInt();
                int leg1Price = query.value(rec.indexOf("Leg1_Price")).toInt();
                int leg2Price = query.value(rec.indexOf("Leg2_Price")).toInt();
                int leg3Price = query.value(rec.indexOf("Leg3_Price")).toInt();
                int leg4Price = query.value(rec.indexOf("Leg4_Price")).toInt();
                int Leg1BuySellIndicator = query.value(rec.indexOf("Leg1_Buy/Sell")).toInt();

                int Leg1_Total_Volume = query.value(rec.indexOf("Leg1_Total_Volume")).toInt();
                int Leg2_Total_Volume = query.value(rec.indexOf("Leg2_Total_Volume")).toInt();
                int Leg3_Total_Volume = query.value(rec.indexOf("Leg3_Total_Volume")).toInt();
                int Leg4_Total_Volume = query.value(rec.indexOf("Leg4_Total_Volume")).toInt();
                QString order_id = query.value(rec.indexOf("Trader_Data")).toString();
                QString Leg1_OrderStateStr ;
                QString Leg3_OrderStateStr ;
                QString Leg2_OrderStateStr ;
                QString Leg4_OrderStateStr  = "-";


                if(Leg1_OrderState==1){
                    Leg1_OrderStateStr = "Sent to Exachange";
                }
                else if(Leg1_OrderState==5){
                    Leg1_OrderStateStr = "Cancelled";
                    leg1Price = 0;
                }
                else if(Leg1_OrderState==6){
                    Leg1_OrderStateStr = "Rejected";
                    leg1Price = 0;
                }
                else if(Leg1_OrderState==7){
                    Leg1_OrderStateStr = "Traded";
                }
                else if(Leg1_OrderState==11){
                    Leg1_OrderStateStr = "Modified";
                }
                else if(Leg1_OrderState==9){
                    Leg1_OrderStateStr = "Open";
                }
                else if(Leg1_OrderState==3){
                    Leg1_OrderStateStr = "Open";
                }
                else if(Leg1_OrderState==10){
                    Leg1_OrderStateStr = "ModifyPending";
                }
                else if(Leg1_OrderState==12){
                    Leg1_OrderStateStr = "CancelPending";
                }
                else if(Leg1_OrderState==8){
                    Leg1_OrderStateStr = "PartialTrade";
                }
                else if(Leg1_OrderState==13){
                    Leg1_OrderStateStr = "Cancelled";
                    leg1Price = 0;
                }


                if(Leg3_OrderState==1){
                    Leg3_OrderStateStr = "Sent to Exachange";
                }
                else if(Leg3_OrderState==5){
                    Leg3_OrderStateStr = "Cancelled";
                    leg3Price=0;
                }
                else if(Leg3_OrderState==6){
                    Leg3_OrderStateStr = "Rejected";
                    leg3Price=0;
                }
                else if(Leg3_OrderState==7){
                    Leg3_OrderStateStr = "Traded";
                }
                else if(Leg3_OrderState==9){
                    Leg3_OrderStateStr = "Open";
                }
                else if(Leg3_OrderState==3){
                    Leg3_OrderStateStr = "Open";
                }
                else if(Leg3_OrderState==11){
                    Leg3_OrderStateStr = "Modified";
                }
                else if(Leg3_OrderState==10){
                    Leg3_OrderStateStr = "ModifyPending";
                }
                else if(Leg3_OrderState==12){
                    Leg3_OrderStateStr = "CancelPending";
                }
                else if(Leg3_OrderState==8){
                    Leg3_OrderStateStr = "PartialTrade";
                }
                else if(Leg3_OrderState==13){
                    Leg3_OrderStateStr = "Cancelled";
                    leg3Price=0;
                }


                if(Leg2_OrderState==1){
                    Leg2_OrderStateStr = "Sent to Exachange";
                }
                else if(Leg2_OrderState==5){
                    Leg2_OrderStateStr = "Cancelled";
                    leg2Price=0;
                }
                else if(Leg2_OrderState==6){
                    Leg2_OrderStateStr = "Rejected";
                     leg2Price=0;
                }
                else if(Leg2_OrderState==7){
                    Leg2_OrderStateStr = "Traded";
                }
                else if(Leg2_OrderState==9){
                    Leg2_OrderStateStr = "Open";
                }
                else if(Leg2_OrderState==3){
                    Leg2_OrderStateStr = "Open";
                }
                else if(Leg2_OrderState==10){
                    Leg2_OrderStateStr = "ModifyPending";
                }
                else if(Leg2_OrderState==12){
                    Leg2_OrderStateStr = "CancelPending";
                }
                else if(Leg2_OrderState==8){
                    Leg2_OrderStateStr = "PartialTrade";
                }
                else if(Leg2_OrderState==13){
                    Leg2_OrderStateStr = "Cancelled";
                     leg2Price=0;
                }
                else{
                    Leg2_OrderStateStr="-";
                }

                if (portfolio_type == PortfolioType::BX_BID || portfolio_type == PortfolioType::BS1221|| portfolio_type == PortfolioType::BS1331) {
                    if (Leg4_OrderState == 1) {
                        Leg4_OrderStateStr = "Sent to Exchange";
                    }
                    else if (Leg4_OrderState == 5 || Leg4_OrderState == 13) {
                        Leg4_OrderStateStr = "Cancelled";
                        leg4Price=0;
                    }
                    else if (Leg4_OrderState == 6) {
                        Leg4_OrderStateStr = "Rejected";
                        leg4Price=0;
                    }
                    else if (Leg4_OrderState == 7) {
                        Leg4_OrderStateStr = "Traded";
                    }
                    else if (Leg4_OrderState == 8) {
                        Leg4_OrderStateStr = "PartialTrade";
                    }
                    else if (Leg4_OrderState == 9) {
                        Leg4_OrderStateStr = "Open";
                    }
                    else if (Leg4_OrderState == 3) {
                        Leg4_OrderStateStr = "Open";
                    }
                    else if (Leg4_OrderState == 10) {
                        Leg4_OrderStateStr = "ModifyPending";
                    }
                    else if (Leg4_OrderState == 12) {
                        Leg4_OrderStateStr = "CancelPending";
                    }
                    else{
                        Leg4_OrderStateStr="-";
                    }

                }

//                 if (portfolio_type == PortfolioType::F2F||portfolio_type == PortfolioType::BFLY_BID||portfolio_type == PortfolioType::CR||portfolio_type == PortfolioType::CR_JELLY){
//                       Leg4_OrderStateStr = "-";
//                }
                QString Stockname = ContractDetail::getInstance().GetStockName(leg1_token_number,portfolio_type);
                int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);

//                Leg1_OrderStateStr = ContractDetail::getInstance().GetStockName(leg1_token_number,portfolio_type)+" "+"["+(QString::number(Leg1_Total_Volume/lotSize))+"]";
//                Leg3_OrderStateStr = ContractDetail::getInstance().GetStockName(leg3_token_number,portfolio_type)+" "+"["+(QString::number(Leg3_Total_Volume/lotSize))+"]";
                //Leg3_OrderStateStr+" ("+(QString::number(Leg3_OrderState))+")";
               // Leg1_OrderStateStr = Leg1_OrderStateStr+" ("+(QString::number(Leg1_OrderState))+")";
               // Leg3_OrderStateStr = Leg3_OrderStateStr+" ("+(QString::number(Leg3_OrderState))+")";
                Leg2_OrderStateStr = Leg2_OrderStateStr;
                              //       +" ("+(QString::number(Leg2_OrderState))+")";
                if(Leg1_OrderState==8)
                {
                   int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);
                   Leg1_OrderStateStr = Leg1_OrderStateStr+"["+(QString::number(Leg1_Total_Volume/lotSize))+"]";
                   Stockname = ContractDetail::getInstance().GetStockName(leg1_token_number,portfolio_type);
                   // +" ("+(QString::number(Leg1_OrderState))+")"+" "
                }
                else
                {
                   Leg1_OrderStateStr = Leg1_OrderStateStr;
                                        //+" ("+(QString::number(Leg1_OrderState))+")";
                }
                if(Leg3_OrderState==8)
                {
                   int lotSize =  ContractDetail::getInstance().GetLotSize(leg3_token_number,portfolio_type);
                    Leg3_OrderStateStr = Leg3_OrderStateStr+"["+(QString::number(Leg3_Total_Volume/lotSize))+"]";
                   Stockname = ContractDetail::getInstance().GetStockName(leg3_token_number,portfolio_type);
                  // +" ("+(QString::number(Leg3_OrderState))+")"+" "
                }
                else
                {
                    Leg3_OrderStateStr = Leg3_OrderStateStr;
                                         //+" ("+(QString::number(Leg3_OrderState))+")";
                }
                if(Leg2_OrderState==8)
                {
                     int lotSize =  ContractDetail::getInstance().GetLotSize(leg2_token_number,portfolio_type);
                    Leg2_OrderStateStr = Leg2_OrderStateStr+"["+(QString::number(Leg2_Total_Volume/lotSize))+"]";
                     Stockname = ContractDetail::getInstance().GetStockName(leg2_token_number,portfolio_type);
                    // +" ("+(QString::number(Leg3_OrderState))+")"+" "
                }
                else
                {
                    Leg2_OrderStateStr = Leg2_OrderStateStr;
                        //+" ("+(QString::number(Leg3_OrderState))+")";
                }
                if(Leg4_OrderState==8)
                {
                     int lotSize =  ContractDetail::getInstance().GetLotSize(leg4_token_number,portfolio_type);
                    Leg4_OrderStateStr = Leg4_OrderStateStr+"["+(QString::number(Leg4_Total_Volume/lotSize))+"]";
                     Stockname = ContractDetail::getInstance().GetStockName(leg4_token_number,portfolio_type);
                    // +" ("+(QString::number(Leg3_OrderState))+")"+" "
                }
                else
                {
                    Leg4_OrderStateStr = Leg4_OrderStateStr;
                        //+" ("+(QString::number(Leg3_OrderState))+")";
                }

                QString Exch_Price = "0";
                double Exch_Price_val  = 0;
                bool disableThisAlgo = false;
                bool traded = false;

                switch (portfolio_type) {
                case PortfolioType::F2F:{
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7) {
                        traded = true;
                    }
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((leg2Price - leg1Price) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg1Price - leg2Price) * 1.0) / devicer;
                    }
                    break;
                }
                case PortfolioType::BY:{
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
                    }

                    break;
                }
                case PortfolioType::CR:{
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }
                    int strikePrice = ContractDetail::getInstance().GetStrikePrice(leg2_token_number,portfolio_type).toInt();
                    strikePrice = strikePrice * devicer; // GetStrikePrice function will return strikePrice devided by devicer, and in below eqution again devide by devicer, to prevent it multipley it with debvicer
                    if(Leg1BuySellIndicator==1){
                        float diff = -leg1Price - leg3Price + leg2Price + strikePrice;
                        Exch_Price_val = static_cast<double>(diff) / devicer;
                    }
                    else{
                        float diff = -leg2Price - strikePrice + leg3Price + leg1Price;
                        Exch_Price_val = static_cast<double>(diff) / devicer;
                    }


                    break;
                }
                case PortfolioType::CR_JELLY:{
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }
                    int strikePrice = ContractDetail::getInstance().GetStrikePrice(leg2_token_number,portfolio_type).toInt();
                    strikePrice = strikePrice * devicer; // GetStrikePrice function will return strikePrice devided by devicer, and in below eqution again devide by devicer, to prevent it multipley it with debvicer
                    if(Leg1BuySellIndicator==1){
                        float diff = -leg1Price - leg3Price + leg2Price + strikePrice;
                        Exch_Price_val = static_cast<double>(diff) / devicer;
                    }
                    else{
                        float diff = -leg2Price - strikePrice + leg3Price + leg1Price;
                        Exch_Price_val = static_cast<double>(diff) / devicer;
                    }


                    break;
                }
                case PortfolioType::BFLY_BID:{
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7) {
                        traded = true;
                    }
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;

                    }

                    break;
                }
                case PortfolioType::BX_BID: {
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13) ||
                        (Leg4_OrderState == 6 || Leg4_OrderState == 5 || Leg4_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7 && Leg4_OrderState == 7) {
                        traded = true;
                    }
                    int strikePriceLeg3 = ContractDetail::getInstance().GetStrikePrice(leg3_token_number, portfolio_type).toInt() * devicer;

                    int strikePriceLeg1 = ContractDetail::getInstance().GetStrikePrice(leg1_token_number, portfolio_type).toInt()* devicer;

                    int strikeDiff = strikePriceLeg3 - strikePriceLeg1;
                    if (Leg1BuySellIndicator == 1) {
                        Exch_Price_val = static_cast<double>(leg3Price + leg2Price - leg4Price - leg1Price + strikeDiff) / devicer;
                    } else {
                        Exch_Price_val = static_cast<double>(-strikeDiff  - leg3Price - leg2Price + leg4Price + leg1Price) / devicer;

                    }

                    break;
                }
                case PortfolioType::F1_F2: {

                    int strikePriceLeg1 = ContractDetail::getInstance().GetStrikePrice(leg1_token_number, portfolio_type).toInt()* devicer;
                    if (Leg1BuySellIndicator == 1) {
                        Exch_Price_val = static_cast<double>(leg1Price ) / devicer;
                    } else {
                        Exch_Price_val = static_cast<double>(leg1Price) / devicer;

                    }

                    break;
                }

                case PortfolioType::BS1221: {
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13) ||
                        (Leg4_OrderState == 6 || Leg4_OrderState == 5 || Leg4_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7 && Leg4_OrderState == 7) {
                        traded = true;
                    }
                    if (Leg1BuySellIndicator == 1) {
                        Exch_Price_val = static_cast<double>(leg2Price * 2 + leg4Price - leg1Price - leg3Price * 2) / devicer;
                    } else {
                        Exch_Price_val = static_cast<double>(leg3Price * 2 + leg1Price - leg4Price - leg2Price*2) / devicer;

                    }

                    break;
                }
                case PortfolioType::BS1331: {
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13) ||
                        (Leg3_OrderState == 6 || Leg3_OrderState == 5 || Leg3_OrderState == 13) ||
                        (Leg4_OrderState == 6 || Leg4_OrderState == 5 || Leg4_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7 && Leg3_OrderState == 7 && Leg4_OrderState == 7) {
                        traded = true;
                    }
                    if (Leg1BuySellIndicator == 1) {
                        Exch_Price_val = static_cast<double>(leg2Price * 3 + leg4Price - leg1Price - leg3Price * 3) / devicer;
                    } else {
                        Exch_Price_val = static_cast<double>(leg3Price * 3 + leg1Price - leg4Price-leg2Price*3) / devicer;
                    }

                    break;
                }
                case PortfolioType::OPEN2L:{
                    if ((Leg1_OrderState == 6 || Leg1_OrderState == 5 || Leg1_OrderState == 13) ||
                        (Leg2_OrderState == 6 || Leg2_OrderState == 5 || Leg2_OrderState == 13)) {
                        disableThisAlgo = true;
                    }
                    else if (Leg1_OrderState == 7 && Leg2_OrderState == 7) {
                        traded = true;
                    }
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((leg2Price - leg1Price) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg1Price - leg2Price) * 1.0) / devicer;
                    }
                    break;
                }


                default:
                    break;
                }

                Exch_Price = QString::number(Exch_Price_val,'f',decimal_precision);
                QString User_Price = fixDecimal(((query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer),decimal_precision);//QString::number(static_cast<double>(query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer, 'f', decimal_precision+1);

                double userPriceVal = query.value(rec.indexOf("DesiredRate")).toDouble() / devicer;
                double JackpotVal =(Exch_Price_val-userPriceVal);
                QString Jackpot = QString::number(JackpotVal,'f',decimal_precision);

               // qDebug()<<"traderData_ID_OnAppStart: " <<traderData_ID_OnAppStart<<"  traderData: "<<traderData<<"  userPriceVal="<<userPriceVal<<" maxLossThr="<<maxLossThr<<" Exch_Price_val="<<Exch_Price_val;

                //disable only for the lastest trade,
                if(traderData.toInt()> traderData_ID_OnAppStart){
                    if(traded){
                        traderData_ID_OnAppStart = traderData.toInt();
                        //maxLossThr = maxLossThr/100.0;
                       // if(Exch_Price_val<userPriceVal*maxLossThr){
                        maxLossThr = maxLossThr/devicer;

                        double range = userPriceVal - maxLossThr;
                        if (Exch_Price_val<range){
                           if(!algosToDisable.contains(Algo_ID))
                                algosToDisable.append(Algo_ID);

                           qDebug()<<"Disablig porfolio: "<<Algo_ID<<"  userPriceVal="<<userPriceVal<<" maxLossThr="<<maxLossThr<<"  range="<<range<<" Exch_Price_val="<<Exch_Price_val;
                        }
                        newlyTradeAlgos.append(Algo_ID);
                    }
                    if(disableThisAlgo){
                        traderData_ID_OnAppStart = traderData.toInt();
                        if(!algosToDisable.contains(Algo_ID))
                            algosToDisable.append(Algo_ID);
                    }
                }


                //



 //               QString BidLeg = ContractDetail::getInstance().GetStockName(leg2_token_number,portfolio_type)+ " "+"["+(QString::number(Leg2_Total_Volume/lotSize))+"]";
                //int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);

                if(qty>0&&lotSize>0) // to prevent crash
                    qty = qty / lotSize;
                QString Traded_Lot = QString::number(qty);
                QString Remaining_Lot = QString::number(static_cast<double>(query.value(rec.indexOf("RemainingQty")).toDouble()) / lotSize);
                long long Trade_Time = query.value(rec.indexOf("Leg2_TimeOrderEnteredHost")).toLongLong();
                long long f1_f2Trade_Time = query.value(rec.indexOf("Leg1_TimeOrderEnteredHost")).toLongLong();
                QDateTime dt1 = QDateTime::fromSecsSinceEpoch(f1_f2Trade_Time, Qt::UTC);
                QDateTime dt = QDateTime::fromSecsSinceEpoch(Trade_Time);
                //QDateTime dt1 = QDateTime::fromSecsSinceEpoch(f1_f2Trade_Time);
                if (portfolio_type == PortfolioType::BX_BID) {
                    std::vector<int> legOrderStates = {Leg1_OrderState, Leg2_OrderState, Leg3_OrderState, Leg4_OrderState};
                    std::vector<QString> legTimeFields = {
                        "Leg1_TimeOrderEnteredHost",
                        "Leg2_TimeOrderEnteredHost",
                        "Leg3_TimeOrderEnteredHost",
                        "Leg4_TimeOrderEnteredHost"
                    };

                    // Loop through each leg
                    for (int i = 0; i < 4; ++i) {
                        if (legOrderStates[i] == 7) {
                            long long tradeTime = query.value(rec.indexOf(legTimeFields[i])).toLongLong();
                            QDateTime dt = QDateTime::fromSecsSinceEpoch(tradeTime);
                           // qDebug() << "Trade time for Leg" << i+1 << ":" << dt.toString();
                        }
                    }
                }

                dt = dt.toUTC();

                QStringList rowList;
                 if(portfolio_type != PortfolioType::F1_F2)
                rowList.append(Algo_ID);
                rowList.append(order_id);
                //                rowList.append(Volant_No);
                if (portfolio_type == PortfolioType::F1_F2) {
                rowList.append(Stockname);
                } else {
                rowList.append(Algo_Name);
                }
                rowList.append(User_Price);
                rowList.append(Exch_Price);
                if(portfolio_type != PortfolioType::F1_F2)
                    rowList.append(Jackpot); // Jackpot data not required or F1_F2
                rowList.append(Traded_Lot);
                if(portfolio_type != PortfolioType::F1_F2)
                rowList.append(Remaining_Lot);
                //  rowList.append(Buy_Sell);
                if (portfolio_type != PortfolioType::F1_F2) {
                rowList.append(dt.toString("hh:mm:ss"));
                }
                else{
                 rowList.append(dt1.toString("hh:mm:ss"));
                }
                rowList.append(Leg1_OrderStateStr);
                rowList.append(Leg2_OrderStateStr);
                rowList.append(Leg3_OrderStateStr);
                rowList.append(Leg4_OrderStateStr);
                rowList.append(QString::number(Leg1_OrderState));
                rowList.append(QString::number(Leg2_OrderState));
                rowList.append(QString::number(Leg3_OrderState));
                rowList.append(QString::number(Leg4_OrderState));
                rowList.append(Expiry);
                rowList.append(Buy_Sell);
                rowList.append(QString::number(lotSize));
                rowList.append(traderData);
                rowList.append(TradeTable_highlight);


//                rowList.append(Leg1_OrderState); // this should be the 4th last data inserted to the row
 //               rowList.append(Leg3_OrderState); // this should be the 3rd last data inserted to the row
  //              rowList.append(Leg2_OrderState); // this should be the 2nd last data inserted to the row
           //    rowList.append(traderData); // this should be the last data inserted to the row


               if(portfolio_type == PortfolioType::F1_F2){
                   f1f2_trade_data_listTmp.append(rowList);
                   continue;
               }
               else
                   trade_data_listTmp.append(rowList);


               TraderCount=trade_data_listTmp.size();

                QString linersDataKey = Algo_ID;
                //populate netpos data here
                if(Liners_Data_Hash.contains(linersDataKey)){
                    if(Buy_Sell=="Buy"){
                        Liners_Data_Hash[linersDataKey].BuyAvgPrice.append(Exch_Price_val);
                        Liners_Data_Hash[linersDataKey].BuyQtyLots.append(qty);
                    }
                    else{
                        Liners_Data_Hash[linersDataKey].SellAvgPrice.append(Exch_Price_val);
                        Liners_Data_Hash[linersDataKey].SellQtyLots.append(qty);
                    }
                }
                else{
                    Liners_Data liners_data;
                    liners_data.algoID = Algo_ID;
                    liners_data.algoName=Algo_Name;
                    if(Buy_Sell=="Buy"){
                        liners_data.BuyAvgPrice.append(Exch_Price_val);
                        liners_data.BuyQtyLots.append(qty);
                        //BuyValue = BuyValue + Exch_Price_val;
                    }
                    else{
                        liners_data.SellAvgPrice.append(Exch_Price_val);
                        liners_data.SellQtyLots.append(qty);
                        //SellValue = SellValue + Exch_Price_val;
                    }


                    Liners_Data_Hash.insert(linersDataKey,liners_data);
                }


            }


//
            trade_table->setDataList(trade_data_listTmp);
            f1f2_order_table_model->setDataList(f1f2_trade_data_listTmp);

            //iterate Liners_Data_Hash and create a QStringList;
            for (const QString &key : Liners_Data_Hash.keys()) {
                Liners_Data linersData = Liners_Data_Hash.value(key);

                double BuyAvgPrice = calculateAverage(linersData.BuyAvgPrice);
                int BuyQtyLots = calculateSum(linersData.BuyQtyLots);

                double SellAvgPrice = calculateAverage(linersData.SellAvgPrice);
                int SellQtyLots = calculateSum(linersData.SellQtyLots);

                double profit = SellAvgPrice - BuyAvgPrice;
                double NetQty = BuyQtyLots - SellQtyLots;

                QStringList rowList;
                rowList.append(linersData.algoID);
                rowList.append(linersData.algoName);
                rowList.append(fixDecimal(BuyAvgPrice, decimal_precision));
                rowList.append(QString::number(BuyQtyLots));
                rowList.append(fixDecimal(SellAvgPrice, decimal_precision));
                rowList.append(QString::number(SellQtyLots));
                rowList.append(QString::number(NetQty));
                rowList.append(fixDecimal(profit, decimal_precision));
               // rowList.append(linersData.StockName);

                liners_listTmp.append(rowList);
            }
//            std::sort(liners_listTmp.begin(), liners_listTmp.end(), [](const QStringList &a, const QStringList &b) {
//                return a[8] < b[8]; // Compare by algoName, which is the second element (index 1)
//            });

            liners_model->setDataList(liners_listTmp);


        }
    }
}

double mysql_conn::calculateAverage(const QList<double> &list) {
    if (list.isEmpty()) {
        return 0.0; // Return 0.0 if the list is empty to avoid division by zero
    }

    double sum = 0.0;
    for (double value : list) {
        sum += value;
    }

    double average = sum / list.size();
    return average;
}
int  mysql_conn::calculateSum(const QList<int> &list) {
    if (list.isEmpty()) {
        return 0; // Return 0.0 if the list is empty to avoid division by zero
    }

    int  sum = 0;
    for (int value : list) {
        sum += value;
    }

     return sum;
}


void mysql_conn::getLinersTableData(Liners_Model *model,QString user_id,QHash<QString, PortFolioData_Less> PortFolioTypeHash)
{
    QMutexLocker lock(&mutex);


    QList <QStringList> liners_list;
    //QList <QStringList> algo_pos_data_listTmp;

    //QHash<QString, alog_pos_data_> alog_pos_data;
    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok)
    {
        QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' and Leg2_OrderState=7  ORDER BY Trader_Data DESC";
        QSqlQuery query(query_str,db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<query.lastError().text();
        }
        else{
            QSqlRecord rec = query.record();
            while (query.next()) {

                QString Algo_ID = ""; // same as PortfolioNumber
                QString Buy_Sell = "";

                QString traderData =  query.value(rec.indexOf("Trader_Data")).toString();
                int Algo_ID_Int = query.value(rec.indexOf("PortfolioNumber")).toInt();

                if(Algo_ID_Int>1500000){
                    Buy_Sell = "Buy";
                    Algo_ID = QString::number(Algo_ID_Int - 1500000);
                }
                else{
                    Buy_Sell = "Sell";
                    Algo_ID = QString::number(Algo_ID_Int);
                }

                QString Volant_No = query.value(rec.indexOf("Trader_Data")).toString();
                int leg1_token_number = query.value(rec.indexOf("Leg1_Tok_No")).toInt();
                int leg2_token_number = query.value(rec.indexOf("Leg2_Tok_No")).toInt();
                int leg3_token_number = query.value(rec.indexOf("Leg3_Tok_No")).toInt();

                int portfolio_type = -1;
                QString Algo_Name = "";
                QString Expiry = "";

                if(PortFolioTypeHash.contains(Algo_ID)){
                    PortFolioData_Less P = PortFolioTypeHash[Algo_ID];
                    portfolio_type = P.PortfolioType.toInt();
                    Expiry = P.Expiry;
                    Algo_Name =get_Algo_Name(portfolio_type,leg1_token_number,leg2_token_number,leg3_token_number,devicer,decimal_precision);
                }


                int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);
                QStringList rowList;
                rowList.append(Algo_ID);
                //rowList.append(Volant_No);
                rowList.append(Algo_Name);
              /*  rowList.append(Exch_Price);
                rowList.append(User_Price);
                rowList.append(Jackpot);
                rowList.append(Traded_Lot);
                rowList.append(Remaining_Lot);
                rowList.append(Buy_Sell);
                rowList.append(dt.toString("hh:mm:ss"));
                rowList.append(Leg1_OrderStateStr);
                rowList.append(Leg3_OrderStateStr);
                rowList.append(Expiry);
                rowList.append(traderData); */
             liners_list.append(rowList);
            }
            model->setDataList(liners_list);

        }
    }
}

void mysql_conn::getNetPosTableData(double &BuyValue_summary, double &SellValue, double &Profit_summary,
                                    double &BuyQty_summary, double &SellQty_summary, double &NetQty_summary,
                                    Net_Position_Table_Model *model,open_position_model*openpos_model, QString user_id)
{
    QMutexLocker lock(&mutex);
    BuyValue_summary = 0;
    SellValue = 0;
    Profit_summary = 0;
    BuyQty_summary = 0;
    SellQty_summary = 0;
    NetQty_summary = 0;

    QString msg;
    QList<QStringList> netPos_data_listTmp;
    QHash<QString, net_pos_data_> net_pos_dataList;
    QList <QStringList> openPos_data_listTmp;
    QHash<QString, open_pos_data> open_position_data_list;
    QHash<QString, int> spanHash;
    SlowData slowData;
    QHash<QString, MBP_Data_Struct>  MBP_Data_Hash = slowData.getMBP_Data_Hash();
    QStringList slowDataCurrentTokens = slowData.getMonitoringTokens();
    QHash<QString, int> sett_price = ContractDetail::getInstance().get_SettPrice();

    QStringList tokenInOrder;

    bool ok = checkDBOpened(msg);
    if (ok) {
        // Define the SQL query to retrieve the desired data
        QString query_str = "SELECT Trades.TokenNo, Contract.StockName,Contract.OptionType,Contract.InstrumentName, Trades.BuySellIndicator, Contract.LotSize, Trades.Expiry, "
                            "ROUND((SUM(Trades.TradedPrice * Trades.TotalVolume) / SUM(Trades.TotalVolume) / " + QString::number(devicer) + "), 2) AS AvgPrice, "
                             "SUM(TotalVolume) AS Qty, ROUND((SUM(Trades.TradedPrice * Trades.TotalVolume) / " + QString::number(devicer) + "), 2) AS buysellvalue "
                             "FROM Trades JOIN Contract ON Contract.Token = Trades.TokenNo WHERE Trades.TraderId = '" + user_id +
                             "' GROUP BY Trades.TokenNo, Trades.Expiry,Trades.BuySellIndicator, Contract.StockName";
        QSqlQuery query(query_str, db);
        if (!query.exec()) {
            // Error Handling, check query.lastError()
            qDebug() << "Query execution failed:" << query.lastError().text();
            return;  // Exit if the query fails
        } else {
            QSqlRecord rec = query.record();
            while (query.next()) {
             QString buy_sell = query.value(rec.indexOf("BuySellIndicator")).toString();
             int TokenNo = query.value(rec.indexOf("TokenNo")).toInt();
             double Qty = query.value(rec.indexOf("Qty")).toDouble();
             double AvgPrice = query.value(rec.indexOf("AvgPrice")).toDouble();
             QString StockName = query.value(rec.indexOf("StockName")).toString();
             QString OptionType = query.value(rec.indexOf("OptionType")).toString();
             QString InstrumentName = query.value(rec.indexOf("InstrumentName")).toString();
             QString Expiry = query.value(rec.indexOf("Expiry")).toString();

             int lotSize = query.value(rec.indexOf("LotSize")).toInt();
             double buysellvalue = query.value(rec.indexOf("buysellvalue")).toDouble();

             QString TokenStr = QString::number(TokenNo);

             if (net_pos_dataList.contains(TokenStr)) {
                    if (buy_sell == "1") {
                        net_pos_dataList[TokenStr].Buy_Avg_Price += AvgPrice;
                        net_pos_dataList[TokenStr].Buy_Total_Lot += Qty;
                        net_pos_dataList[TokenStr].Buy_Price += buysellvalue;
                    } else {
                        net_pos_dataList[TokenStr].Sell_Avg_Price += AvgPrice;
                        net_pos_dataList[TokenStr].Sell_Total_Lot += Qty;
                        net_pos_dataList[TokenStr].Sell_Price += buysellvalue;
                    }
             } else {
                    net_pos_data_ net_pos;
                    net_pos.Stock_Name = StockName;
                    net_pos.OptionType = OptionType;
                    net_pos.InstrumentName = InstrumentName;
                    net_pos.Expiry = Expiry;

                    net_pos.lotSize = lotSize;
                    net_pos.Buy_Avg_Price = (buy_sell == "1") ? AvgPrice : 0;
                    net_pos.Buy_Total_Lot = (buy_sell == "1") ? Qty : 0;
                    net_pos.Buy_Price = (buy_sell == "1") ? buysellvalue : 0;
                    net_pos.Sell_Avg_Price = (buy_sell == "2") ? AvgPrice : 0;
                    net_pos.Sell_Total_Lot = (buy_sell == "2") ? Qty : 0;
                    net_pos.Sell_Price = (buy_sell == "2") ? buysellvalue : 0;
                    net_pos.M2M = 0;
                    net_pos.MarginUsed = 0;
                    net_pos.exp = 0;
                    net_pos.token_number = TokenStr;
                    net_pos_dataList.insert(TokenStr, net_pos);

                    tokenInOrder.append(TokenStr);

                    spanHash[TokenStr] = 0;
                    if(!slowDataCurrentTokens.contains(TokenStr))
                        slowData.addLeg_n_token(TokenStr);

             }
            }

            /***************Get Span data for tokens***************************/
            QStringList distinct_tokens = net_pos_dataList.keys();
            if(distinct_tokens.size()>0){
                QString tokenListStr = "(" + distinct_tokens.join(",") + ")";
                QString sqlQuery = QString("SELECT Token, StockName, Min_Value, Max_Value FROM Span WHERE Token IN %1").arg(tokenListStr);
                QSqlQuery query2(sqlQuery,db);
                if( !query2.exec() )
                {
                    qDebug()<<sqlQuery;
                    qDebug()<<query2.lastError().text();
                }
                else{
                    QSqlRecord rec1 = query2.record();
                    while (query2.next())
                    {
                        QString TokenNo = query2.value(rec1.indexOf("Token")).toString();
                        if(spanHash.contains(TokenNo)){
                            int Min_Value = query2.value(rec1.indexOf("Min_Value")).toInt();
                            spanHash[TokenNo] = Min_Value;
                        }
                    }
                }
                query2.finish();
            }
            /******************************************************************/


            // Calculate the net quantities and summarize the data
            QHashIterator<QString, net_pos_data_> iter(net_pos_dataList);
            while (iter.hasNext()) {
             iter.next();
             QString TokenNo = iter.key();
             net_pos_dataList[TokenNo].Net_Qty = net_pos_dataList[TokenNo].Buy_Total_Lot - net_pos_dataList[TokenNo].Sell_Total_Lot;

             BuyValue_summary += net_pos_dataList[TokenNo].Buy_Price;
             SellValue += net_pos_dataList[TokenNo].Sell_Price;
             NetQty_summary += (net_pos_dataList[TokenNo].Net_Qty) / net_pos_dataList[TokenNo].lotSize;
             BuyQty_summary += (net_pos_dataList[TokenNo].Buy_Total_Lot) / net_pos_dataList[TokenNo].lotSize;
             SellQty_summary += (net_pos_dataList[TokenNo].Sell_Total_Lot) / net_pos_dataList[TokenNo].lotSize;
             double Profit = net_pos_dataList[TokenNo].Buy_Price - net_pos_dataList[TokenNo].Sell_Price;
             Profit_summary += Profit;

             /***************M2M calcualtion**********************/
             if(MBP_Data_Hash.contains(TokenNo))
             {
                 MBP_Data_Struct mbpData = MBP_Data_Hash[TokenNo];
                 if (net_pos_dataList[TokenNo].Net_Qty > 0) {
                    net_pos_dataList[TokenNo].M2M = ((((mbpData.lastTradedPrice.toDouble()  / devicer) - net_pos_dataList[TokenNo].Buy_Avg_Price) * net_pos_dataList[TokenNo].Net_Qty));
                 }
                 else if (net_pos_dataList[TokenNo].Net_Qty < 0) {
                    net_pos_dataList[TokenNo].M2M = ((((mbpData.lastTradedPrice.toDouble() / devicer) - net_pos_dataList[TokenNo].Sell_Avg_Price) * net_pos_dataList[TokenNo].Net_Qty));
                 }
             }
             /******************************************************************/




            }




            /***************MarginUsed calcualtion**********************/
              //group net_pos_dataList  based on InstrumentName
              QHash<QString, QStringList> net_pos_dataList_grouped_by_InstrumentName;
              for (auto it = net_pos_dataList.constBegin(); it != net_pos_dataList.constEnd(); ++it) {
                    const QString& token = it.key();
                    const net_pos_data_& netPos = it.value();

                    // Group tokens by InstrumentName
                    if (!net_pos_dataList_grouped_by_InstrumentName.contains(netPos.InstrumentName)) {
                        net_pos_dataList_grouped_by_InstrumentName[netPos.InstrumentName] = QStringList();
                    }
                    net_pos_dataList_grouped_by_InstrumentName[netPos.InstrumentName].append(token);
             }

             //iterate grouped net pos data and calculate    MarginUsed
             for (auto it = net_pos_dataList_grouped_by_InstrumentName.constBegin();
                     it != net_pos_dataList_grouped_by_InstrumentName.constEnd(); ++it) {

                    QString instrumentName = it.key();        // Get the InstrumentName
                    QStringList tokenList = it.value();       // Get the list of tokens
                    /*QStringList reorderedList;  // this should contin the token same as in the order like it retreved from DB, For Margin used calculation
                    //To rearrange the tokenList to match the order of tokenInOrder,
                    //while omitting tokens not in tokenList and ensuring the existing ones follow the order in tokenInOrder
                    for (const QString& token : tokenInOrder) {
                        if (tokenList.contains(token)) {
                           reorderedList.append(token);
                        }
                    }*/

                    double spanToSubtract = 0;
                  //  double buyTotalPrice = 0;
                    for (const QString& TokenNo : tokenList) {
                        net_pos_data_  net_pos_data = net_pos_dataList[TokenNo];
                        int span = 0;
                        if (spanHash.contains(TokenNo))
                            span = spanHash[TokenNo];

                        net_pos_data.Span = span;

                        //buy side
                        if (net_pos_data.Net_Qty > 0)
                        {
                            double value = 0;// net_pos_data.BuyAvgPrice;
                           // buyTotalPrice = buyTotalPrice+net_pos_data.Buy_Price;

                            // FUT option
                            if (net_pos_data.OptionType == "XX")
                            {
                                net_pos_dataList[TokenNo].MarginUsed = (double)value + (double)span + ((double)value * (double)userDataStatic.exp_mar) / devicer;
                                net_pos_dataList[TokenNo].MarginUsed = abs(net_pos_dataList[TokenNo].MarginUsed * net_pos_data.Net_Qty);
                            }
                            // CE/PE option
                            else
                            {
                                net_pos_dataList[TokenNo].MarginUsed = abs(value * net_pos_data.Net_Qty)+net_pos_data.Buy_Price;//buyTotalPrice;
                                spanToSubtract = spanToSubtract + abs(span * net_pos_data.Net_Qty);
                            }
                        }
                        //sell side
                        else if (net_pos_data.Net_Qty < 0)
                        {
                            double value = 0;// net_pos_data.SellAvgPrice;
                            net_pos_data.exp = ((double)value * (double)userDataStatic.exp_mar) / devicer;


                            // FUT option
                            if (net_pos_data.OptionType == "XX")
                            {
                                net_pos_dataList[TokenNo].MarginUsed = (double)span + ((double)value * (double)userDataStatic.exp_mar) / devicer;
                                net_pos_dataList[TokenNo].MarginUsed = abs(net_pos_dataList[TokenNo].MarginUsed * net_pos_data.Net_Qty);
                            }
                            // CE/PE option
                            else
                            {
                                //find the tokennumenr(instrumennam+expriy+XX) -> Find LTP --> value
                                /*int value_token = ContractDetail.GetTokenNumebr(net_pos_data.InstrumentName, net_pos_data.ExpiryDate, "XX");
                                INTERACTIVE_ONLY_MBP_DATA mbpData = null;
                                if (SlowDataReader.GetInstance().GetLeg(value_token, out mbpData))
                                {
                                    value = mbpData.LastTradedPrice;
                                }*/



                                // Define the base date: January 1, 1980, 00:00:00 UTC
                              //  QDateTime baseDate(QDate(1980, 1, 1), QTime(0, 0), Qt::UTC);

                                // Add seconds to the base date
                               // qint64 expirySeconds = net_pos_data.Expiry.toLongLong();
                               // QDateTime expiryDate = baseDate.addSecs(expirySeconds);

                                // Format the date to "MMM" (e.g., "Jan", "Feb")
                                //QString formattedDate = expiryDate.toString("MMM");
                                QString key_sett_price =  net_pos_data.Stock_Name;//net_pos_data.Stock_Name + "_" + formattedDate + "_XX";// + net_pos_data.OptionType;

                                if (sett_price.contains(key_sett_price)){
                                    value = sett_price[key_sett_price];
                                }

                                net_pos_dataList[TokenNo].MarginUsed = (double)span + ((double)value * (double)userDataStatic.exp_mar) / devicer;

                                net_pos_dataList[TokenNo].MarginUsed = abs(net_pos_dataList[TokenNo].MarginUsed * net_pos_data.Net_Qty);
                                //net_pos_data.exp = ((double)value * (double)userDataStatic.exp_mar) / devicer;


                            }
                        }



                    }
            }


            /********************************************************************************/



           QList<QStringList> netPos_data_listTmp; // Assuming this is declared as a list of QStringList
            double bLotSum=0.0;
            double sLotSum=0.0;
            double bvalueSum=0.0;
            double svalueSum=0.0;
            double bavgSum=0.0;
            double savgSum=0.0;
            double netlotSum=0.0;
            double profitSum=0.0;
           for (auto it = net_pos_dataList.constBegin(); it != net_pos_dataList.constEnd(); ++it) {
                 QString tokenNo = it.key();              // TokenNo (key in QHash)
                 const net_pos_data_& data = it.value();  // Access the net_pos_data_ object

                 QStringList rowList;
                 //rowList.append(data.Stock_Name+"("+data.token_number+")"); // Stock_Name
                 rowList.append(data.Stock_Name); // Stock_Name
                 rowList.append(QString::number(data.Buy_Total_Lot / data.lotSize)); // Buy Total Lot
                 rowList.append(QString::number(data.Sell_Total_Lot / data.lotSize)); // Sell Total Lot
                 rowList.append(fixDecimal(data.Buy_Price, decimal_precision)); // Buy Price
                 rowList.append(fixDecimal(data.Sell_Price, decimal_precision)); // Sell Price
                 rowList.append(fixDecimal(data.Buy_Avg_Price, decimal_precision)); // Buy Avg Price
                 rowList.append(fixDecimal(data.Sell_Avg_Price, decimal_precision)); // Sell Avg Price
                 rowList.append(QString::number(data.Net_Qty / data.lotSize)); // Net Qty
                 double Profit = (data.Buy_Price - data.Sell_Price); // Example profit calculation
                 rowList.append(fixDecimal(Profit, decimal_precision)); // Profit

                 rowList.append("-");
                 rowList.append("-");
                 //rowList.append(fixDecimal(data.M2M, decimal_precision)); // M2M temporarily disabled
                 //rowList.append(QString::number((data.MarginUsed))); // MarginUsed temporarily disabled
                 rowList.append(QString::number((data.lotSize))); //lotSize
                 rowList.append(tokenNo); // TokenNo as the last element
                 netPos_data_listTmp.append(rowList);

                 bLotSum =bLotSum+data.Buy_Total_Lot / data.lotSize;
                 sLotSum=sLotSum+data.Sell_Total_Lot/ data.lotSize;
                 bvalueSum=bvalueSum+data.Buy_Price;
                 svalueSum=svalueSum+data.Sell_Price;
                 bavgSum=bavgSum+data.Buy_Avg_Price;
                 savgSum=savgSum+data.Sell_Avg_Price;
                 netlotSum=netlotSum+data.Net_Qty / data.lotSize;
                 profitSum= profitSum + Profit;

                 if(data.Buy_Total_Lot-data.Sell_Total_Lot!=0){
                        QString buySell = "Buy";
                        QString openAvg = fixDecimal(data.Buy_Avg_Price, decimal_precision);
                        if(data.Buy_Total_Lot-data.Sell_Total_Lot>0){
                            buySell = "Sell";
                            openAvg = fixDecimal(data.Sell_Avg_Price, decimal_precision);
                        }
                        QString openLot = QString::number(qAbs((data.Buy_Total_Lot - data.Sell_Total_Lot) / data.lotSize));

                        QStringList rowListOpenPostion;
                        rowListOpenPostion.append(data.Stock_Name);
                        rowListOpenPostion.append(buySell);
                        rowListOpenPostion.append(openLot);
                        rowListOpenPostion.append(openAvg);
                        rowListOpenPostion.append("00:00:00");
                        rowListOpenPostion.append(" ");
                        rowListOpenPostion.append(tokenNo);




                        openPos_data_listTmp.append(rowListOpenPostion);


                 }
            }

            // Sort the data based on StockName (first column in rowList)
            std::sort(netPos_data_listTmp.begin(), netPos_data_listTmp.end(), [](const QStringList &a, const QStringList &b) {
                return a[0] < b[0]; // Sort by StockName
            });
            QStringList sumRowList;
            sumRowList.append("Total");
            sumRowList.append(QString::number(bLotSum));
            sumRowList.append(QString::number(sLotSum));
            sumRowList.append(fixDecimal(bvalueSum, decimal_precision));
            sumRowList.append(fixDecimal(svalueSum, decimal_precision));
            sumRowList.append(fixDecimal(bavgSum, decimal_precision));
            sumRowList.append(fixDecimal(savgSum, decimal_precision));
            sumRowList.append(QString::number(netlotSum));
            sumRowList.append(QString::number(profitSum));
            sumRowList.append("-");
            sumRowList.append("-");
            sumRowList.append("-"); // TokenNo as the last element
            sumRowList.append("dummy_token_no"); // dummy TokenNo as the last element, for comparing the row, and should be unique


            netPos_data_listTmp.append(sumRowList);
            // Set sorted data to the model
            model->setDataList(netPos_data_listTmp);
            openpos_model->setDataList(openPos_data_listTmp);
        }
    }
}


void mysql_conn::getMissedTradeData(Missed_Trade_Table_Model* model,QString user_id){

    QMutexLocker lock(&mutex);

    QList <QStringList> missed_trade_list;

    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok)
    {
       // QString query_str = "SELECT * FROM MissedTrades WHERE TraderID='"+user_id+"'";
//        QString query_str = "SELECT m.*, "
//                            "CASE WHEN p.AdditionalData1 IS NOT NULL AND p.AdditionalData2 IS NOT NULL THEN 1 ELSE 0 END AS retried "
//                            "FROM MissedTrades m "
//                            "LEFT JOIN Portfolios p ON m.localOrderID = p.AdditionalData1 "
//                            "AND m.Id = p.AdditionalData2 "
//                            "AND p.TraderID = '" + user_id + "' "
//                            "WHERE m.TraderID = '" + user_id + "'";


        QString query_str = QString(R"(
                    SELECT m.*,
                           CASE
                               WHEN p.AdditionalData1 IS NOT NULL AND p.AdditionalData2 IS NOT NULL
                               THEN 1
                               ELSE 0
                           END AS retried,
                           CASE
                               WHEN p.AdditionalData1 IS NOT NULL AND p.AdditionalData2 IS NOT NULL
                               THEN p.PortfolioNumber
                               ELSE 0
                           END AS retry_PortfolioNumber,
                           otb.Leg1_Price AS retry_Leg1_Price,
                           otb.Leg1_Total_Volume AS retry_Leg1_Total_Volume,
                           otb.Leg1_OrderState AS retry_Leg1_OrderState,
                           otb.Leg1_Tok_No AS retry_Leg1_Tok_No
                    FROM MissedTrades m
                    LEFT JOIN Portfolios p
                        ON m.localOrderID = p.AdditionalData1
                        AND m.Id = p.AdditionalData2
                        AND p.TraderID = '%1'
                    LEFT JOIN Order_Table_Bid otb
                        ON (p.PortfolioNumber = otb.PortfolioNumber OR p.PortfolioNumber + 1500000 = otb.PortfolioNumber)
                        AND p.PortfolioNumber > 0
                    WHERE m.TraderID = '%1';
                )").arg(user_id);


        QSqlQuery query(query_str,db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<query.lastError().text();
        }
        else{
            QSqlRecord rec = query.record();
            while (query.next()) {

                QString  OrderId =  query.value(rec.indexOf("localOrderID")).toString();
                long long DateTime = query.value(rec.indexOf("DateTime")).toLongLong();
                QDateTime dt = QDateTime::fromSecsSinceEpoch(DateTime);
                dt = dt.toUTC();

                // Add 5 hours (18000 seconds) and 30 minutes (1800 seconds)
                dt = dt.addSecs(19800);

                QString DateTimeStr = dt.toString("hh:mm:ss");

                QString BuySellIndicator = query.value(rec.indexOf("BuySellIndicator")).toString();
                if(BuySellIndicator=="1")
                    BuySellIndicator = "Buy";
                else
                    BuySellIndicator = "Sell";

                QString Type = query.value(rec.indexOf("Type")).toString();
                int token_number = query.value(rec.indexOf("Symbol")).toInt();

                QString StockName = ContractDetail::getInstance().GetStockName(token_number,0);
                QString Message = query.value(rec.indexOf("Message")).toString();

                int Quantity = query.value(rec.indexOf("Quantity")).toInt();
                int lotsize = ContractDetail::getInstance().GetLotSize(token_number,0);
                int Portfolio = query.value(rec.indexOf("Portfolio")).toInt();
                QString Id = query.value(rec.indexOf("Id")).toString();
                double Price = query.value(rec.indexOf("Price")).toDouble();


                if (Type == "RejectedByExchange") {
                    Type = "Rejected";
                }
                else if (Type == "CancelledByExchange") {
                    Type = "Cancelled";
                }


                Price = Price/devicer;

                QString PriceStr = fixDecimal(Price,decimal_precision);
                QString QuantityStr = QString::number(Quantity/lotsize);
                QString PortfolioStr = QString::number(Portfolio);

                QString retry_price = "-";
                QString retry_lot = "-";

                QString retried = query.value(rec.indexOf("retried")).toString();

               // int retry_Portfolio_number = query.value(rec.indexOf("retry_PortfolioNumber")).toInt();
                int retry_leg1_tkno = query.value(rec.indexOf("retry_Leg1_Tok_No")).toInt();

                if(retry_leg1_tkno>0){

                    int retry_Leg1_orderstate = query.value(rec.indexOf("retry_Leg1_OrderState")).toInt();
                    if(retry_Leg1_orderstate==7){
                            int lotsize_retry = ContractDetail::getInstance().GetLotSize(retry_leg1_tkno,0);
                            double retry_price_val = query.value(rec.indexOf("retry_Leg1_Price")).toDouble();
                            retry_price_val = retry_price_val/devicer;
                            double retry_lot_val = query.value(rec.indexOf("retry_Leg1_Total_Volume")).toDouble();
                            retry_lot_val = retry_lot_val/lotsize_retry;
                            retry_lot = QString::number(retry_lot_val);
                            retry_price = QString::number(retry_price_val);
                            retried = "Traded";
                    }
                    else if(retry_Leg1_orderstate==6){
                            retried = "Rejected";
                    }
                    else if(retry_Leg1_orderstate==8){
                            int lotsize_retry = ContractDetail::getInstance().GetLotSize(retry_leg1_tkno,0);
                            double retry_price_val = query.value(rec.indexOf("retry_Leg1_Price")).toDouble();
                            retry_price_val = retry_price_val/devicer;
                            double retry_lot_val = query.value(rec.indexOf("retry_Leg1_Total_Volume")).toDouble();
                            retry_lot_val = retry_lot_val/lotsize_retry;
                            retry_lot = QString::number(retry_lot_val);
                            retry_price = QString::number(retry_price_val);
                            retried = "Partial Trade";
                    }
                    else if(retry_Leg1_orderstate==5||retry_Leg1_orderstate==13){
                            retried = "Cancelled";
                    }

                }




                QStringList rowList;

                rowList.append(Id);
                rowList.append(OrderId);
                rowList.append(BuySellIndicator);
                rowList.append(Type);
                rowList.append(QuantityStr);
                rowList.append(PortfolioStr);
                rowList.append(StockName);
                rowList.append(Message);
                rowList.append(PriceStr);
                rowList.append(DateTimeStr);
                rowList.append(retried);
                rowList.append(retry_price);
                rowList.append(retry_lot);
                rowList.append(QString::number(token_number));

                missed_trade_list.append(rowList);
            }
            model->setDataList(missed_trade_list);
        }
    }
}


void mysql_conn::getNetPosTableData_BackUp(double &BuyValue_summary,double &SellValue,double &Profit_summary,double &BuyQty_summary,double &SellQty_summary,double &NetQty_summary,Net_Position_Table_Model* model,QString user_id,QHash<QString,int> PortFoliosLotSizeHash)
{
    QMutexLocker lock(&mutex);
     BuyValue_summary=0;
     SellValue = 0;
     Profit_summary = 0 ;
     BuyQty_summary = 0;
     SellQty_summary = 0;
     NetQty_summary = 0;


    //Qury: SELECT TokenNo, (sum(TradedPrice * TotalVolume) / sum(TotalVolume)) /10000000  as AvgPrice, sum(TotalVolume) as Qty, BuySellIndicator FROM Trades where TraderId = 1 group by TokenNo, BuySellIndicator;
    QString msg;
    QList <QStringList> netPos_data_listTmp;
    QList <QStringList> openPos_data_listTmp;
    QHash<QString, net_pos_data_> net_pos_dataList;
    QHash<QString, net_pos_data_> openpos_dataList;

    bool ok = checkDBOpened(msg);
    if(ok){
        QString query_str ="SELECT TokenNo,StockName,PortfolioNumber, (sum(TradedPrice * TotalVolume) / sum(TotalVolume)) /"+QString::number(devicer)+"  as AvgPrice, sum(TotalVolume) as Qty, BuySellIndicator FROM Trades where TraderId = '"+user_id+"'group by TokenNo, BuySellIndicator,StockName,PortfolioNumber";


        //QString Buy_Avg_Price="SELECT Trades.TokenNo, FORMAT((SUM(Trades.TradedPrice * Trades.TotalVolume)/ SUM(Trades.TotalVolume)/100),2) AS avgpri, Trades.BuySellIndicator FROM Trades WHERE Trades.BuySellIndicator=1 GROUP BY Trades.TokenNo,Trades.BuySellIndicator";
         //QString query_str="SELECT StockName, (sum(TradedPrice * TotalVolume) / sum(TotalVolume)) /"+QString::number(devicer)+"  as AvgPrice, sum(TotalVolume) as Qty, BuySellIndicator FROM Trades where TraderId = '"+user_id+"' group by  StockName,BuySellIndicator";
        QSqlQuery query(query_str,db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<query.lastError().text();
        }
        else{
            QSqlRecord rec = query.record();
            while (query.next()) {
                QString buy_sell = query.value(rec.indexOf("BuySellIndicator")).toString();
                int TokenNo = query.value(rec.indexOf("TokenNo")).toInt();
                double Qty = query.value(rec.indexOf("Qty")).toDouble();
                double AvgPrice = query.value(rec.indexOf("AvgPrice")).toDouble();
                QString StockName = query.value(rec.indexOf("StockName")).toString();
                int PortfolioNumber = query.value(rec.indexOf("PortfolioNumber")).toInt();
                if(PortfolioNumber>1500000)
                    PortfolioNumber = PortfolioNumber-1500000;



                QString key = QString::number(TokenNo);
                if(net_pos_dataList.contains(key)){
                    if(buy_sell=="1"){
                        net_pos_dataList[key].Buy_Avg_Price = net_pos_dataList[key].Buy_Avg_Price+AvgPrice;
                        net_pos_dataList[key].Buy_Total_Lot = net_pos_dataList[key].Buy_Total_Lot + Qty;
                       // BuyQty_summary = BuyQty_summary + net_pos_dataList[key].Buy_Total_Lot;
                    }
                    else{
                        net_pos_dataList[key].Sell_Avg_Price = net_pos_dataList[key].Sell_Avg_Price+ AvgPrice;
                        net_pos_dataList[key].Sell_Total_Lot = net_pos_dataList[key].Sell_Total_Lot + Qty;
                       // SellQty_summary = SellQty_summary +  net_pos_dataList[key].Sell_Total_Lot;
                    }
                }
                else{
                    net_pos_data_ net_pos;
                    net_pos.Stock_Name = StockName;
         //           net_pos.PortfolioNumber = QString::number(PortfolioNumber);
                    net_pos.Buy_Avg_Price = 0;
                    net_pos.Buy_Total_Lot = 0;
                    net_pos.Buy_Price = 0;
                    net_pos.Sell_Avg_Price = 0;
                    net_pos.Sell_Price = 0;
                    net_pos.Sell_Total_Lot = 0;

                    if(buy_sell=="1"){
                        net_pos.Buy_Avg_Price = (AvgPrice);
                        net_pos.Buy_Total_Lot = (Qty);
                    }
                    else{
                        net_pos.Sell_Avg_Price = (AvgPrice);
                        net_pos.Sell_Total_Lot = (Qty);

                    }
                    net_pos_dataList.insert(key,net_pos);
                }
            }
           /*  int portfolio_type = -1;
             QString portfolioName = query.value(rec.indexOf("PortfolioNumber")).toString();
            if(PortFolioTypeHash.contains(portfolioName)){
                PortFolioData_Less P = PortFolioTypeHash[portfolioName];
                portfolio_type = P.PortfolioType.toInt();
            }*/
           QHashIterator<QString, net_pos_data_> iter(net_pos_dataList);
           int c=1;

           while(iter.hasNext())
            {
                iter.next();
                QString TokenNo =  iter.key();

           //     QString PortfolioNumber = net_pos_dataList[TokenNo].PortfolioNumber;
                int lotSize = 1;
//                if(PortFoliosLotSizeHash.contains(PortfolioNumber)){
//                    lotSize = PortFoliosLotSizeHash[PortfolioNumber];
//                }
                QDateTime dt;//need portfolio type later = ContractDetail::getInstance().GetExpiryDate(TokenNo.toInt());
                dt = dt.addYears(10);

               // QString stock_name=ContractDetail::getInstance().GetStockName(TokenNo.toInt(),portfolio_type);
                net_pos_dataList[TokenNo].Net_Qty = (net_pos_dataList[TokenNo].Buy_Total_Lot)-(net_pos_dataList[TokenNo].Sell_Total_Lot);
                net_pos_dataList[TokenNo].Buy_Price =(net_pos_dataList[TokenNo].Buy_Avg_Price)*(net_pos_dataList[TokenNo].Buy_Total_Lot);
                net_pos_dataList[TokenNo].Sell_Price = (net_pos_dataList[TokenNo].Sell_Total_Lot)*(net_pos_dataList[TokenNo].Sell_Avg_Price);


                BuyValue_summary = BuyValue_summary + net_pos_dataList[TokenNo].Buy_Price ;
                SellValue = SellValue +  net_pos_dataList[TokenNo].Sell_Price ;

                NetQty_summary = NetQty_summary + (net_pos_dataList[TokenNo].Net_Qty / lotSize);

                BuyQty_summary = BuyQty_summary + (net_pos_dataList[TokenNo].Buy_Total_Lot)/ lotSize;
                SellQty_summary = SellQty_summary + (net_pos_dataList[TokenNo].Sell_Total_Lot)/ lotSize;


                double Profit = net_pos_dataList[TokenNo].Buy_Price-net_pos_dataList[TokenNo].Sell_Price;
                Profit_summary = Profit_summary + Profit ;
                QStringList rowList;
                rowList.append(QString::number(c)+"-"+TokenNo);
                rowList.append(net_pos_dataList[TokenNo].Stock_Name);
                rowList.append(QString::number((net_pos_dataList[TokenNo].Buy_Total_Lot)/lotSize));
                rowList.append(QString::number((net_pos_dataList[TokenNo].Sell_Total_Lot)/lotSize));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Buy_Price,decimal_precision));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Sell_Price,decimal_precision));
                rowList.append(fixDecimal((net_pos_dataList[TokenNo].Buy_Avg_Price),decimal_precision));
                rowList.append(fixDecimal((net_pos_dataList[TokenNo].Sell_Avg_Price),decimal_precision));
                rowList.append(QString::number(net_pos_dataList[TokenNo].Net_Qty/lotSize));
                rowList.append(fixDecimal(Profit,decimal_precision));
                rowList.append("-");
                rowList.append(TokenNo); // this should be the last one
                netPos_data_listTmp.append(rowList);

                //get open postion data
               c++;

            }


            model->setDataList(netPos_data_listTmp);


            //
        }
    }
}

QList<int> mysql_conn::getPortfolioTypesForInstrumentType(const QString& data, const QMap<int, QStringList>& algoFilterMap) {
    QList<int> matchingPortfolios;

    // Iterate through each PortfolioType and QStringList in algoFilterMap
    for (auto it = algoFilterMap.begin(); it != algoFilterMap.end(); ++it) {
        // Check if the QStringList for this PortfolioType contains the data
        if (it.value().contains(data)) {
            // If found, add the corresponding PortfolioType to the list
            matchingPortfolios.append(it.key());
        }
    }

    return matchingPortfolios;
}

QHash<QString, contract_table>  mysql_conn::getContractTable( QHash<int , QStringList> &m_ContractDetails_Grouped_,QHash<QString, QStringList> &_m_ContractDetailsFiltered,userInfo userData)
{

    QMutexLocker lock(&mutex);
    QString  queryStr = "SELECT InstrumentType, InstrumentName, OptionType, StrikePrice, LotSize, ExpiryDate, Token, StockName, MinSpread,VolumeFreezeQty,OperatingRangeslowPriceRange,OperatingRangeshighPriceRange FROM Contract ORDER BY ExpiryDate,Token, InstrumentName, OptionType, StrikePrice";


    QHash<QString, contract_table> contractTableData;
    QString msg;
    bool ok = checkDBOpened(msg);
    if(ok)
    {
        QSqlQuery query(queryStr, db);
        if( !query.exec() )
        {
            // Error Handling, check query.lastError(), probably return
            qDebug()<<"getContractTable: "<<query.lastError().text();
        }
        else{

            bool writeContract = true;
            QString directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
            QDir().mkpath(directory);
           // QString filename = directory + "/contracts_"+QString::number(userData.UserId)+".bin";
            QString filename = directory + "/contracts.bin";

            QFile file(filename);
            if (!file.open(QIODevice::WriteOnly)) {
                qDebug() << "Waring: Failed to open file: " << filename <<" for contract writing.";
                writeContract = false;
            }

           // int size =  m_ContractDetails.size();
            QDataStream out;
            if(writeContract)
                out.setDevice(&file);

            QSqlRecord rec = query.record();
            while (query.next())
            {
                int token =  query.value(rec.indexOf("Token")).toInt();
                if(token==0)
                    continue;
                contract_table contractTableTmp;
                contractTableTmp.InstrumentType = query.value(rec.indexOf("InstrumentType")).toString();
                contractTableTmp.InstrumentName = query.value(rec.indexOf("InstrumentName")).toString();
                contractTableTmp.OptionType = query.value(rec.indexOf("OptionType")).toString();
                contractTableTmp.StrikePrice = query.value(rec.indexOf("StrikePrice")).toInt();
                contractTableTmp.LotSize = query.value(rec.indexOf("LotSize")).toInt();
                contractTableTmp.Expiry = query.value(rec.indexOf("ExpiryDate")).toLongLong();
                contractTableTmp.TokenNumber = token;
                contractTableTmp.StockName = query.value(rec.indexOf("StockName")).toString();
                contractTableTmp.MinimumSpread = query.value(rec.indexOf("MinSpread")).toInt();
                contractTableTmp.VolumeFreezeQty = query.value(rec.indexOf("VolumeFreezeQty")).toDouble();
                contractTableTmp.OperatingRangeslowPriceRange = query.value(rec.indexOf("OperatingRangeslowPriceRange")).toInt();
                contractTableTmp.OperatingRangeshighPriceRange = query.value(rec.indexOf("OperatingRangeshighPriceRange")).toInt();

                contractTableData.insert( query.value(rec.indexOf("Token")).toString(), contractTableTmp);

                _m_ContractDetailsFiltered[contractTableTmp.InstrumentType].append(QString::number(contractTableTmp.TokenNumber));

                QStringList PortfolioAll;
                QList<int>  portfolio_types = getPortfolioTypesForInstrumentType(contractTableTmp.InstrumentType,userData.algoFilterMap);
                for(int i=0;i<portfolio_types.length();i++){
                    m_ContractDetails_Grouped_[portfolio_types[i]].append(QString::number(contractTableTmp.TokenNumber));
                    PortfolioAll.append(QString::number(portfolio_types[i]));
                }


                //write contract to output file
                if(writeContract){
                    if(PortfolioAll.size()>0){
                        out << PortfolioAll.join(",")
                            << contractTableTmp.TokenNumber
                            << contractTableTmp.InstrumentType
                            << contractTableTmp.InstrumentName
                            << contractTableTmp.OptionType
                            << contractTableTmp.StrikePrice
                            << contractTableTmp.LotSize
                            << contractTableTmp.Expiry
                            << contractTableTmp.StockName
                            << contractTableTmp.MinimumSpread
                            << contractTableTmp.VolumeFreezeQty
                            << contractTableTmp.OperatingRangeslowPriceRange
                            << contractTableTmp.OperatingRangeshighPriceRange;
                    }
                    else{
                        //qDebug()<<"Waring: Cannot find portfolio type(InstrumentType) for contract table token = "<<contractTableTmp.TokenNumber;
                    }
                }

            }

            if(writeContract)
                file.close();  // Close the file after writing
        }
        //contractTableData[PortfolioType::BFLY_BID] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::BFLY_BID, userData),&db, BFLY_BID_data_list_Sorted_Key);
        //contractTableData[PortfolioType::BY] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::BY,userData),&db, BFLY_data_list_Sorted_Key);
        //contractTableData[PortfolioType::F2F] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::F2F,userData),&db,F2F_data_list_Sorted_Key);
        //contractTableData[PortfolioType::CR] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::CR,userData),&db,CR_data_list_Sorted_Key);
    }
    return contractTableData;
}


QHash<QString, int>  mysql_conn::get_SettPrice()
{

    QMutexLocker lock(&mutex);
    QString queryStr = "SELECT StockName, Close FROM BhavCopy";


    QHash<QString, int> SettPrice;
    QString msg;
    bool ok = checkDBOpened(msg);
    if(ok)
    {
        QSqlQuery query(queryStr, db);
        if( !query.exec() )
        {
            qDebug()<<"get_SettPrice: "<<query.lastError().text();
        }
        else{

            QSqlRecord rec = query.record();
            while (query.next())
            {
                QString StockName = query.value("StockName").toString();
              //  QString Expiry = query.value("Expiry").toString();
              //  QString Option_type = query.value("Option_type").toString();
                int Sett_price = query.value("Close").toInt();

//                QString monthAbbreviation;

//                // Parse the string into a QDate object
//                QDate date = QDate::fromString(Expiry, "dd-MMM-yyyy");
//                if (date.isValid()) {
//                    // Get the month abbreviation
//                    monthAbbreviation = date.toString("MMM");
//                }

//                QString key = Symbol + "_" + monthAbbreviation + "_" + Option_type;
                SettPrice.insert(StockName, Sett_price);

            }

        }

    }
    return SettPrice;
}
bool mysql_conn::deleteAlgo(QString PortfolioNumber,QString &msg)
{
    QMutexLocker lock(&mutex);

    bool ret = false;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);
            QString str = "SELECT Id FROM Trades WHERE PortfolioNumber='"+PortfolioNumber+"' OR PortfolioNumber='"+QString::number(PortfolioNumber.toInt() + 1500000)+"'";
            qDebug()<<"Delete Check records in Trades Query: " <<str;
            query.prepare(str);
            if(!query.exec())
            {
                msg = query.lastError().text();
                qDebug()<<query.lastError().text();
            }
            else{

                if(query.size()>0){
                    ret = false;  // PortfolioNumber exist in Trades so return
                    msg = "Executed Trades cannot be deleted.!";//"Delete skipped, Record exist in Trades table.";
                    /*QMessageBox msgBox;
                    msgBox.setText("Executed Trades cannot be deleted ");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.exec();*/
                }
                else {
                    str = "DELETE FROM Portfolios WHERE PortfolioNumber='"+PortfolioNumber+"'" +  " AND SellTradedQuantity = 0 AND BuyTradedQuantity = 0";
                    qDebug()<<"Delete Query: " <<str;
                    query.prepare(str);
                    if( !query.exec() )
                    {
                        msg = query.lastError().text();
                        qDebug()<<query.lastError().text();
                    }
                    else{
                        int rowsAffected = query.numRowsAffected();
                        if (rowsAffected > 0) {
                            ret = true;  //delet scuccess
                            msg = "Deleted successfully.";
                        }
                        else {
                            msg = "Deleted failed.";
                            ret = false;
                        }
                    }
                }
            }
        }
        else{
            qDebug()<<"Delete Portfolio  Cannot connect Database: "+db.lastError().text();
            msg="Delete Portfolio Cannot connect Database: "+db.lastError().text();
        }

        //db.close();
    }
    return ret;
}

bool mysql_conn::deleteAlgos(QStringList PortfolioNumbers,QString &msg)
{
    QMutexLocker lock(&mutex);

    bool ret = false;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);
            QString str = "DELETE FROM Portfolios WHERE PortfolioNumber IN ('" + PortfolioNumbers.join("','") + "')";


            query.prepare(str);
            if( !query.exec() )
            {
                msg = query.lastError().text();
                qDebug()<<query.lastError().text();
            }
            else{
                int rowsAffected = query.numRowsAffected();
                if (rowsAffected > 0) {
                    ret = true;  //delet scuccess
                    msg = "Portfolios "+PortfolioNumbers.join("','")+" Deleted successfully.";
                }
                else {
                    msg = "Deleted failed.";
                    ret = false;
                }
            }

        }
        else{
            qDebug()<<"Delete Portfolio  Cannot connect Database: "+db.lastError().text();
            msg="Delete Portfolio Cannot connect Database: "+db.lastError().text();
        }

        //db.close();
    }
    return ret;
}

bool mysql_conn::deleteNonTradedAlgos(QStringList PortfolioNumbers, QString &msg)
{
    QMutexLocker lock(&mutex);
    bool ret = true;  // Assume success unless we hit an issue

    if (!checkDBOpened(msg)) {
        msg = "Delete Portfolio cannot connect to Database: " + db.lastError().text();
        qDebug() << msg;
        return false;
    }

    // Step 1: Check which PortfolioNumbers exist in the Trades table
    QSqlQuery query(db);
    QStringList queryPortfolios;
    for (const QString &PortfolioNumber : PortfolioNumbers) {
        queryPortfolios << "'" + PortfolioNumber + "'";
        queryPortfolios << "'" + QString::number(PortfolioNumber.toInt() + 1500000) + "'";
    }

    QString checkQuery = "SELECT DISTINCT PortfolioNumber FROM Trades WHERE PortfolioNumber IN (" + queryPortfolios.join(",") + ")";
    qDebug() << "Check trades query: " << checkQuery;
    query.prepare(checkQuery);

    if (!query.exec()) {
        msg = "Error checking Trades: " + query.lastError().text();
        qDebug() << query.lastError().text();
        return false;  // Return if there's a problem executing the query
    }

    // Step 2: Collect PortfolioNumbers that exist in Trades
    QStringList existingInTrades;
    while (query.next()) {
        existingInTrades << query.value(0).toString();
    }

    // Step 3: Remove portfolios that exist in Trades from the deletion list
    QStringList portfoliosToDelete;
    for (const QString &PortfolioNumber : PortfolioNumbers) {
        if (!existingInTrades.contains(PortfolioNumber) && !existingInTrades.contains(QString::number(PortfolioNumber.toInt() + 1500000))) {
            portfoliosToDelete << PortfolioNumber;
        }
    }

    if (portfoliosToDelete.isEmpty()) {
        msg = "No portfolios available for deletion. All portfolios have existing trades.";
        return false;
    }

    // Step 4: Delete remaining portfolios in one query
    QSqlQuery deleteQuery(db);
    QString deleteStr = "DELETE FROM Portfolios WHERE PortfolioNumber IN ('" + portfoliosToDelete.join("','") + "')";
    qDebug() << "Delete query: " << deleteStr;
    deleteQuery.prepare(deleteStr);

    if (!deleteQuery.exec()) {
        msg = "Error deleting portfolios: " + deleteQuery.lastError().text();
        qDebug() << deleteQuery.lastError().text();
        ret = false;
    } else {
        int rowsAffected = deleteQuery.numRowsAffected();
        if (rowsAffected > 0) {
            msg = "Portfolios " + portfoliosToDelete.join("', '") + " deleted successfully.";
        } else {
            msg = "Deletion failed for some portfolios.";
            ret = false;
        }
    }

    // Step 5: Inform about the skipped portfolios due to existing trades
    if (!existingInTrades.isEmpty()) {
        msg += "\n Trades Portfolios were skipped: " + existingInTrades.join("', '");
    }

    return ret;
}
bool mysql_conn::clearAlogsForImport(QString user_id,QString &msg)
{
    QMutexLocker lock(&mutex);

    bool ret = false;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);
            QString str = "DELETE FROM Portfolios WHERE TraderID='" + user_id + "' AND SellTradedQuantity <= 0 AND BuyTradedQuantity <=0";

            query.prepare(str);
            if( !query.exec() )
            {
                msg = query.lastError().text();
                qDebug()<<query.lastError().text();
            }
            else{
                int rowsAffected = query.numRowsAffected();
                if (rowsAffected > 0) {
                    ret = true;  //delet scuccess
                    msg = "Portfolios  Deleted successfully.";
                }
                else {
                    msg = "No portfolio cleared from DB.";
                    ret = true;
                }
            }

        }
        else{
            qDebug()<<"clearAlogsForImport:  Cannot connect Database: "+db.lastError().text();
            msg="clearAlogsForImport: Cannot connect Database: "+db.lastError().text();
        }

        //db.close();
    }
    return ret;
}
bool mysql_conn::insertAlogsForImport(QList<portfolioImportExportData> pData ,QString &msg)
{
    QMutexLocker lock(&mutex);

    int insertCount = 0;
    bool ret = false;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            for(int i=0;i<pData.length();i++){
                QSqlQuery query(db);
                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID,ClientID,IsBroker, Status, "
                              "Leg1TokenNo,Leg2TokenNo,Leg3TokenNo,Leg4TokenNo,Leg5TokenNo,Leg6TokenNo"
                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,AdditionalData1,AdditionalData2,AdditionalData3,AdditionalData4,Alias,"
                              "OrderQuantity) "
                              "VALUES (:PortfolioType, :TraderID, :ClientID, :IsBroker, :Status, "
                              ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo, :Leg6TokenNo,"
                              ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,:AdditionalData1,:AdditionalData2,:AdditionalData3,:AdditionalData4,:Alias,"
                              ":OrderQuantity)");
                query.bindValue(":PortfolioType", pData[i].PortfolioType);
                query.bindValue(":TraderID", pData[i].TraderID);
                query.bindValue(":ClientID", pData[i].ClientID);
                query.bindValue(":IsBroker", pData[i].IsBroker);
                query.bindValue(":Status", pData[i].Status);
                query.bindValue(":Leg1TokenNo", pData[i].Leg1TokenNo);
                query.bindValue(":Leg2TokenNo", pData[i].Leg2TokenNo);
                query.bindValue(":Leg3TokenNo", pData[i].Leg3TokenNo);
                query.bindValue(":Leg4TokenNo", pData[i].Leg4TokenNo);
                query.bindValue(":Leg5TokenNo", pData[i].Leg5TokenNo);
                query.bindValue(":Leg6TokenNo", pData[i].Leg6TokenNo);

                query.bindValue(":BuyPriceDifference", pData[i].BuyPriceDifference);
                query.bindValue(":BuyTotalQuantity", pData[i].BuyTotalQuantity);
                query.bindValue(":BuyTradedQuantity", pData[i].BuyTradedQuantity);
                query.bindValue(":SellPriceDifference", pData[i].SellPriceDifference);
                query.bindValue(":SellTotalQuantity", pData[i].SellTotalQuantity);
                query.bindValue(":SellTradedQuantity", pData[i].SellTradedQuantity);
                query.bindValue(":AdditionalData1", pData[i].AdditionalData1);
                query.bindValue(":AdditionalData2", pData[i].AdditionalData2);
                query.bindValue(":AdditionalData3", pData[i].AdditionalData3);
                query.bindValue(":AdditionalData4", pData[i].AdditionalData4);
                query.bindValue(":Alias", pData[i].Alias);
                query.bindValue(":OrderQuantity", 0);

                if( !query.exec() )
                {
                    msg = query.lastError().text();
                    qDebug()<<query.lastError().text();
                }
                else{
                    insertCount ++;
                }
            }


        }
        else{
            qDebug()<<"insertAlogsForImport:  Cannot connect Database: "+db.lastError().text();
            msg="insertAlogsForImport: Cannot connect Database: "+db.lastError().text();
        }

        //db.close();
    }
    if(insertCount==pData.size())
        ret = true;
    else{
        msg = msg+"Some of the portfolio cannot insert to DB.";
    }
    return ret;
}

algo_data_insert_status mysql_conn::insertToAlgoTable(algo_data_to_insert data,int MaxPortfolioCount,QString &msg){
    QMutexLocker lock(&mutex);
    algo_data_insert_status ret = algo_data_insert_status::FAILED;
    {
        bool ok = checkDBOpened(msg);
        if (ok) {
            QSqlQuery query(db);
            int PortfoliosCount = -1;

            query.prepare("SELECT COUNT(*) FROM Portfolios WHERE TraderID = :user_id AND PortfolioType != 208");
            query.bindValue(":user_id", data.user_id);

            if (!query.exec()) {
                msg = query.lastError().text();
                qDebug() << query.lastError().text();
            } else {
                if (query.next()) {
                    PortfoliosCount = query.value(0).toInt(); // Retrieve the count
                }

                // Check if the portfolio limit has been reached
                if (PortfoliosCount >= MaxPortfolioCount) {
                    ret = algo_data_insert_status::LIMIT_REACHED;
                    msg = "Maximum Portfolio Limit Reached";
                }
                else{
                    // QString str = "";
                    //F2F
                    if(data.algo_type==QString::number(PortfolioType::F2F)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,ModifyType,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status, "
                                      ":Leg1TokenNo, :Leg2TokenNo,"
                                      ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:ModifyType,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":ModifyType", 1);
                        query.bindValue(":Alias", data.Alias);


                    }
                    //BFLY
                    else if(data.algo_type==QString::number(PortfolioType::BY)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":Alias", data.Alias);


                    }
                    //CON-REV
                    else if(data.algo_type==QString::number(PortfolioType::CR)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":Alias", data.Alias);


                    }

                    //CRJELLY
                    else if(data.algo_type==QString::number(PortfolioType::CR_JELLY)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,ModifyType,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:ModifyType,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":ModifyType", 1);
                        query.bindValue(":Alias", data.Alias);


                    }
                    //BOX
                    else if(data.algo_type==QString::number(PortfolioType::BOX)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo, Leg3TokenNo, Leg4TokenNo, Leg5TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,AdditionalData1,AdditionalData2,AdditionalData3,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status, "
                                      ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo,"
                                      ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity, :AdditionalData1, :AdditionalData2, :AdditionalData3,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":Leg4TokenNo", data.Leg4_token_number);
                        query.bindValue(":Leg5TokenNo", data.Leg5_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":AdditionalData1", "CE");
                        query.bindValue(":AdditionalData2", "PE");
                        query.bindValue(":AdditionalData3", data.option_type);
                        query.bindValue(":Alias", data.Alias);

                    }
                    //OPEN_BY
                    else if(data.algo_type==QString::number(PortfolioType::OPEN_BY)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,Leg1Ratio,Leg2Ratio,Leg3Ratio,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:Leg1Ratio,:Leg2Ratio,:Leg3Ratio,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":Leg1Ratio", data.Leg1Ratio);
                        query.bindValue(":Leg2Ratio", data.Leg2Ratio);
                        query.bindValue(":Leg3Ratio", data.Leg3Ratio);
                        query.bindValue(":Alias", data.Alias);


                    }
                    //OPEN BOX
                    else if(data.algo_type==QString::number(PortfolioType::OPEN_BOX)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo, Leg3TokenNo, Leg4TokenNo, Leg5TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,AdditionalData1,AdditionalData2,AdditionalData3,Leg1Ratio,Leg2Ratio,Leg3Ratio,Leg4Ratio,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status, "
                                      ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo,"
                                      ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity, :AdditionalData1, :AdditionalData2, :AdditionalData3,:Leg1Ratio,:Leg2Ratio,:Leg3Ratio,:Leg4Ratio,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":Leg4TokenNo", data.Leg4_token_number);
                        query.bindValue(":Leg5TokenNo", data.Leg5_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":AdditionalData1", "CE");
                        query.bindValue(":AdditionalData2", "PE");
                        query.bindValue(":AdditionalData3", data.option_type);
                        query.bindValue(":Leg1Ratio", data.Leg1Ratio);
                        query.bindValue(":Leg2Ratio", data.Leg2Ratio);
                        query.bindValue(":Leg3Ratio", data.Leg3Ratio);
                        query.bindValue(":Leg4Ratio", data.Leg4Ratio);
                        query.bindValue(":Alias", data.Alias);
                    }
                    //BFLY_BID
                    else if(data.algo_type==QString::number(PortfolioType::BFLY_BID)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,ModifyType,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:ModifyType,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":ModifyType", 1);
                        query.bindValue(":Alias", data.Alias);

                    }

                    //BFLY_BID
                    else if(data.algo_type==QString::number(PortfolioType::BX_BID)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo,Leg4TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,ModifyType,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo,:Leg4TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:ModifyType,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":Leg4TokenNo", data.Leg4_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":ModifyType", 1);
                        query.bindValue(":Alias", data.Alias);

                    }

                    //BX1221l
                    else if(data.algo_type==QString::number(PortfolioType::BS1221)||data.algo_type==QString::number(PortfolioType::BS1331)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo,Leg4TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status,"
                                      ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo,:Leg4TokenNo"
                                      ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":Leg3TokenNo", data.Leg3_token_number);
                        query.bindValue(":Leg4TokenNo", data.Leg4_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":Alias", data.Alias);
                    }

                    //OPEN 2L
                    else if(data.algo_type==QString::number(PortfolioType::OPEN2L)){
                        query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                      "Leg1TokenNo, Leg2TokenNo"
                                      ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                      "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                      "OrderQuantity,Leg1Ratio,Leg2Ratio,Alias) "
                                      "VALUES (:PortfolioType, :TraderID, :Status, "
                                      ":Leg1TokenNo, :Leg2TokenNo,"
                                      ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                      ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                      ":OrderQuantity,:Leg1Ratio,:Leg2Ratio,:Alias)");
                        query.bindValue(":PortfolioType", data.algo_type);
                        query.bindValue(":TraderID", data.user_id);
                        query.bindValue(":Status", data.Algo_Status);
                        query.bindValue(":Leg1TokenNo", data.Leg1_token_number);
                        query.bindValue(":Leg2TokenNo", data.Leg2_token_number);
                        query.bindValue(":BuyPriceDifference", 0);
                        query.bindValue(":BuyTotalQuantity", 0);
                        query.bindValue(":BuyTradedQuantity", 0);
                        query.bindValue(":SellPriceDifference", 0);
                        query.bindValue(":SellTotalQuantity", 0);
                        query.bindValue(":SellTradedQuantity", 0);
                        query.bindValue(":OrderQuantity", 0);
                        query.bindValue(":Leg1Ratio", data.Leg1Ratio);
                        query.bindValue(":Leg2Ratio", data.Leg2Ratio);
                        query.bindValue(":Alias", data.Alias);
                    }

                    if(query.exec()){
                        msg="Add record to DB successfully";
                        ret = algo_data_insert_status::INSERTED;

                    }
                    else{
                        msg="Failed to insert record to DB";
                        ret = algo_data_insert_status::FAILED;
                        qDebug() << "Executed Query: " << query.lastQuery();
                        qDebug()<<"query.lastError: "+query.lastError().text();
                    }
                 }
                }

                }

        else{
            qDebug()<<"Cannot connect Database: "+db.lastError().text();
            msg="Cannot connect Database: "+db.lastError().text();
        }


        // db.close();
    }
    return ret;
}


QString mysql_conn::getAlgoTypeQuery(PortfolioType type, userInfo userLoginInfo)
{
    QString whereStr = " WHERE InstrumentType IN (";

    for(const auto &algoFilter: userLoginInfo.algoFilterMap[type])
    {
        whereStr += "'";
        whereStr += algoFilter;
        whereStr += "',";
    }
    if(whereStr.endsWith(","))
    {
        whereStr.chop(1);
    }
    whereStr += ")";

    return "SELECT InstrumentType, InstrumentName, OptionType, StrikePrice, LotSize, ExpiryDate, Token, StockName, MinSpread,VolumeFreezeQty,OperatingRangeslowPriceRange,OperatingRangeshighPriceRange FROM Contract" +
           whereStr +
           " ORDER BY ExpiryDate,Token, InstrumentName, OptionType, StrikePrice";
}


algo_data_insert_status mysql_conn::place_F1F2_Order(QString userID,QString Leg1TokenNumber,QString sellprice,QString sellqty,QString buyprice,QString buyqty,int MaxPortfolioCount,QString orderQty,QString &msg,bool checkDuplicateExist){
    QMutexLocker lock(&mutex);

    QString ClientID = "0";
    QString IsBroker = "0";
    algo_data_insert_status ret = algo_data_insert_status::FAILED;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);
//            int PortfoliosCount = -1;
//            query.prepare("SELECT * FROM Portfolios");
//            if( !query.exec() )
//            {
//                msg = query.lastError().text();
//                qDebug()<<query.lastError().text();
//                return ret;
//            }

            //ortfoliosCount = query.size();
            //if portfolio limit reaches do not insert
//            if(PortfoliosCount>=MaxPortfolioCount){
//                    ret = algo_data_insert_status::LIMIT_REACHED;
//                    msg = "Maximum Portfolio Limit Reached";
//                    return ret;
//            }

            if(checkDuplicateExist){
                QString str = "SELECT COUNT(*) FROM Portfolios WHERE Leg1TokenNo="+Leg1TokenNumber+" and TraderID="+userID+" and PortfolioType="+QString::number(PortfolioType::F1_F2);
                query.prepare(str);

                if( !query.exec() ){
                    msg = query.lastError().text();
                    qDebug()<<query.lastError().text();
                    return ret;
                }

                if (query.next() && query.value(0).toInt() > 0) {
                    ret = algo_data_insert_status::EXIST;
                    msg = "Record already exists in DB";
                    return ret;
                }
            }

            query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID,ClientID,IsBroker, Status, "
                                          "Leg1TokenNo, Leg2TokenNo, Leg3TokenNo, Leg4TokenNo, Leg5TokenNo, Leg6TokenNo"
                                          ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                          "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                          "OrderQuantity) "
                                          "VALUES (:PortfolioType, :TraderID,:ClientID,:IsBroker, :Status, "
                                          ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo, :Leg6TokenNo,"
                                          ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                          ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                          ":OrderQuantity)");
                            query.bindValue(":PortfolioType", QString::number(PortfolioType::F1_F2));
                            query.bindValue(":TraderID",userID);
                            query.bindValue(":ClientID",ClientID);
                            query.bindValue(":IsBroker",IsBroker);
                            query.bindValue(":Status", "Active");
                            query.bindValue(":Leg1TokenNo", Leg1TokenNumber);
                            query.bindValue(":Leg2TokenNo", 0);
                            query.bindValue(":Leg3TokenNo", 0);
                            query.bindValue(":Leg4TokenNo", 0);
                            query.bindValue(":Leg5TokenNo", 0);
                            query.bindValue(":Leg6TokenNo", 0);
                            query.bindValue(":BuyPriceDifference", buyprice);
                            query.bindValue(":BuyTotalQuantity", buyqty);
                            query.bindValue(":BuyTradedQuantity", 0);
                            query.bindValue(":SellPriceDifference", sellprice);
                            query.bindValue(":SellTotalQuantity", sellqty);
                            query.bindValue(":SellTradedQuantity", 0);
                            query.bindValue(":OrderQuantity", orderQty);



                            if(query.exec()){
                                msg="Add record to DB successfully";
                                ret = algo_data_insert_status::INSERTED;
                            }
                            else{
                                msg="Failed to insert record to DB";
                                ret = algo_data_insert_status::FAILED;
                                qDebug() << "Executed Query: " << query.lastQuery();

                                qDebug()<<"query.lastError: "+query.lastError().text();
                            }


        }
        else{
            qDebug()<<"Cannot connect Database: "+db.lastError().text();
            msg="Cannot connect Database: "+db.lastError().text();
        }

    }
    return ret;
}

algo_data_insert_status mysql_conn::retry_F1F2_Order( QString  OrderId, QString ID, QString userID,QString Leg1TokenNumber,QString sellprice,QString sellqty,QString buyprice,QString buyqty,int MaxPortfolioCount,QString orderQty,QString &msg,bool checkDuplicateExist){
    QMutexLocker lock(&mutex);

    QString ClientID = "0";
    QString IsBroker = "0";
    algo_data_insert_status ret = algo_data_insert_status::FAILED;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);


            if(checkDuplicateExist){
                QString str = "SELECT COUNT(*) FROM Portfolios WHERE Leg1TokenNo="+Leg1TokenNumber+" and TraderID="+userID+" and PortfolioType="+QString::number(PortfolioType::F1_F2)+" and AdditionalData1="+OrderId+" and AdditionalData2="+ID;
                query.prepare(str);

                if( !query.exec() ){
                    msg = query.lastError().text();
                    qDebug()<<query.lastError().text();
                    return ret;
                }

                if (query.next() && query.value(0).toInt() > 0) {
                    ret = algo_data_insert_status::EXIST;
                    msg = "Record already exists in DB";
                    return ret;
                }
            }

            query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID,ClientID,IsBroker, Status, "
                          "Leg1TokenNo, Leg2TokenNo, Leg3TokenNo, Leg4TokenNo, Leg5TokenNo, Leg6TokenNo"
                          ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                          "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                          "OrderQuantity,AdditionalData1,AdditionalData2) "
                          "VALUES (:PortfolioType, :TraderID,:ClientID,:IsBroker, :Status, "
                          ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo, :Leg6TokenNo,"
                          ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                          ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                          ":OrderQuantity,:AdditionalData1,:AdditionalData2)");
            query.bindValue(":PortfolioType", QString::number(PortfolioType::F1_F2));
            query.bindValue(":TraderID",userID);
            query.bindValue(":ClientID",ClientID);
            query.bindValue(":IsBroker",IsBroker);
            query.bindValue(":Status", "Active");
            query.bindValue(":Leg1TokenNo", Leg1TokenNumber);
            query.bindValue(":Leg2TokenNo", 0);
            query.bindValue(":Leg3TokenNo", 0);
            query.bindValue(":Leg4TokenNo", 0);
            query.bindValue(":Leg5TokenNo", 0);
            query.bindValue(":Leg6TokenNo", 0);
            query.bindValue(":BuyPriceDifference", buyprice);
            query.bindValue(":BuyTotalQuantity", buyqty);
            query.bindValue(":BuyTradedQuantity", 0);
            query.bindValue(":SellPriceDifference", sellprice);
            query.bindValue(":SellTotalQuantity", sellqty);
            query.bindValue(":SellTradedQuantity", 0);
            query.bindValue(":OrderQuantity", orderQty);
            query.bindValue(":AdditionalData1", OrderId);
            query.bindValue(":AdditionalData2", ID);



            if(query.exec()){
                msg="Add record to DB successfully";
                ret = algo_data_insert_status::INSERTED;
            }
            else{
                msg="Failed to insert record to DB";
                ret = algo_data_insert_status::FAILED;
                qDebug() << "Executed Query: " << query.lastQuery();

                qDebug()<<"query.lastError: "+query.lastError().text();
            }


        }
        else{
            qDebug()<<"Cannot connect Database: "+db.lastError().text();
            msg="Cannot connect Database: "+db.lastError().text();
        }

    }
    return ret;
}


int mysql_conn::getLastestTradetData_ID_FromTradeTable(QString user_id)
{
    QMutexLocker lock(&mutex);
    int traderDataID = -1;
    QString msg;

    if (checkDBOpened(msg))
    {
        QSqlQuery query(db);
        query.prepare("SELECT Trader_Data FROM Order_Table_Bid WHERE Trader_ID = ? ORDER BY Trader_Data DESC LIMIT 1;");
        query.addBindValue(user_id);

        if (!query.exec())
        {
            qDebug() << "Query Error:" << query.lastError().text();
        }
        else if (query.next())  // Only one row is expected
        {
            traderDataID = query.value(0).toInt(); // Use column index directly
        }
    }
    else{
        qDebug() << "Connection error:" << msg;
    }

    return traderDataID;
}

