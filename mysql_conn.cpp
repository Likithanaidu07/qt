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
            QSqlQuery query("SELECT UserId, UserName,Password,STKOpenLimit,IDXOpenLimit,MaxPortfolioCount,BFLY_BIDInFilter,BYInFilter,F2FInFilter,CRInFilter,F1_F2InFilter FROM rt_usertable WHERE UserName='"+UserName_+"'", db);
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

                     userLoginInfo.algoFilterMap[PortfolioType::BX1221] =  OPT_Instruments;

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
                    userLoginInfo.loggedIn = true;
                    userLoginInfo.loginResponse = "Login Sucess";
                    userLoginInfo.errorCode = T_LoginErroCode::OK;
                    userLoginInfo.IDXOpenLimit = query.value(rec.indexOf("IDXOpenLimit")).toInt();
                    userLoginInfo.STKOpenLimit = query.value(rec.indexOf("STKOpenLimit")).toInt();

                    //make the entry to logs table

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
                    query1.bindValue(":logMessage", "User:" + UserName_ + " logged in from [" + localIP + "]");
                    query1.bindValue(":userName", UserName_);
                    query1.bindValue(":formattedTime", QDateTime::currentDateTime().toSecsSinceEpoch());
                    if (!query1.exec())
                    {
                        qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
                        qDebug() << "Query Str: " << qryStr;
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
       QString sqlquery = "SELECT PortfolioNumber, (sum(TradedPrice*TotalVolume) * 1.0 / sum(TotalVolume))/"+QString::number(devicer)+" as AvgPrice, TokenNo, BuySellIndicator from Trades WHERE TraderID='"+user_id+"' group by PortfolioNumber, TokenNo, BuySellIndicator ";


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
                QString key = QString::number(portfolioNumber)+"_"+QString::number(token);
                if(!averagePriceList.contains(key)){
                    PortfolioAvgPrice avgPrice;
                    avgPrice.PortfolioNumber = portfolioNumber;
                    avgPrice.TokenNo = token;
                    avgPrice.BuySellIndicator = queryAvgPrice.value(rec.indexOf("BuySellIndicator")).toString();
                    avgPrice.AvgPrice = queryAvgPrice.value(rec.indexOf("AvgPrice")).toDouble();
                    averagePriceList.insert(key,avgPrice);
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
    if(ok){
        QTime currentTime = QTime::currentTime();
        QString qryStr = "INSERT INTO Logs (LogMessage, UserName, Time) VALUES (:logMessage, :userName, :formattedTime)";
        QSqlQuery query1(db);
        query1.prepare(qryStr);
        query1.bindValue(":logMessage", logMessage);
        query1.bindValue(":userName", userNameLogged);
        query1.bindValue(":formattedTime", QDateTime::currentDateTime().toSecsSinceEpoch());

        if (!query1.exec()) {
            qDebug() << "Insert Into Logs failed: " << query1.lastError().text();
            qDebug() << "Query Str: " << qryStr;
        }

        QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.0;'>"
                              "<span>" + QTime::currentTime().toString("hh:mm:ss")
                              + "&nbsp;</span><span style='font-weight:400;color: black;'>"+ logMessage + "</span></p>";
        emit display_log_text_signal(htmlContent);

    }
    else{
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
                                  + "&nbsp;</span><span style='font-weight:400;color: black;'>"+ logMessage.prepend("  ") + "</span></p>";
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

    else if(algo_type==PortfolioType::BX1221){
        Algo_Name = "BX1221-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }

    return Algo_Name.toUpper();
}

  QList<QHash<QString,QString>>  mysql_conn::getOrderPopUPData(QString user_id, QString portfolioNumber,QString PortfolioType){

    QList<QHash<QString,QString>> orderData;
    QMutexLocker lock(&mutex);

    QList <QStringList> orderData_listTmp;
    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok)
    {
        QString query_str = "SELECT * FROM Order_Table_bid WHERE Trader_ID='"+user_id+"' and Leg2_OrderState=7  ORDER BY Trader_Data DESC";

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

                int portfolio_type = PortfolioType.toInt();
                QString Algo_Name = "";


                 Algo_Name =get_Algo_Name(portfolio_type,leg1_token_number,leg2_token_number,leg3_token_number,devicer,decimal_precision);


                int qty = query.value(rec.indexOf("Leg1_Total_Volume")).toInt();
                //int leg2qty = query.value(rec.indexOf("Leg2_Total_Volume")).toInt();
                int leg1Price = query.value(rec.indexOf("Leg1_Price")).toInt();
                int leg2Price = query.value(rec.indexOf("Leg2_Price")).toInt();
                int leg3Price = query.value(rec.indexOf("Leg3_Price")).toInt();
                int Leg1BuySellIndicator = query.value(rec.indexOf("Leg1_Buy/Sell")).toInt();

                QString Exch_Price = "0";
                double Exch_Price_val  = 0;

                switch (portfolio_type) {
                case PortfolioType::F2F:{
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((leg2Price - leg1Price) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg1Price - leg2Price) * 1.0) / devicer;
                    }

                    break;

                }
                case PortfolioType::BY:{
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
                    }

                    break;
                }
                case PortfolioType::CR:{
                    int strikePrice = ContractDetail::getInstance().GetStrikePrice(leg2_token_number,portfolio_type).toInt();
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
                    int strikePrice = ContractDetail::getInstance().GetStrikePrice(leg2_token_number,portfolio_type).toInt();
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
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
                    }

                    break;
                }

                case PortfolioType::BX_BID:{
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
                    }

                    break;
                }

                case PortfolioType::BX1221:{
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
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


                int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);

                if(qty>0&&lotSize>0) // to prevent crash
                    qty = qty / lotSize;
                QString Traded_Lot = QString::number(qty);

                QString Remaining_Lot = QString::number(static_cast<double>(query.value(rec.indexOf("RemainingQty")).toDouble()) / lotSize);
                long long Trade_Time = query.value(rec.indexOf("TimeOrderEnteredHost")).toLongLong();
                QDateTime dt = QDateTime::fromSecsSinceEpoch(Trade_Time);
                dt = dt.toUTC();

                QHash<QString,QString> tmp;
                tmp.insert("Algo_ID",Algo_ID);
                tmp.insert("Algo_Name",Algo_Name);
                tmp.insert("Exch_Price",Exch_Price);
                tmp.insert("User_Price",User_Price);
                tmp.insert("Jackpot",Jackpot);
                tmp.insert("Traded_Lot",Traded_Lot);
                tmp.insert("Remaining_Lot",Remaining_Lot);
                tmp.insert("Buy_Sell",Buy_Sell);
                tmp.insert("Time",dt.toString("hh:mm:ss"));

                orderData.append(tmp);
            }

        }
    }
    return orderData;
}



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
void mysql_conn::getTradeTableData(int &TraderCount,Trade_Table_Model *model,Liners_Model *liners_model ,QString user_id, QHash<QString, PortFolioData_Less> PortFolioTypeHash)
{
    QMutexLocker lock(&mutex);

    QList <QStringList> trade_data_listTmp;

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
        QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' AND (Leg1_OrderState=7 OR Leg2_OrderState=7 OR Leg3_OrderState=7 OR Leg4_OrderState=7) ORDER BY Trader_Data DESC";

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

                int portfolio_type = -1;

                if(PortFolioTypeHash.contains(Algo_ID)){
                    PortFolioData_Less P = PortFolioTypeHash[Algo_ID];
                    portfolio_type = P.PortfolioType.toInt();
                }
                else{
                    continue;
                }

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




                QString Volant_No = query.value(rec.indexOf("Trader_Data")).toString();
                int leg1_token_number = query.value(rec.indexOf("Leg1_Tok_No")).toInt();
                int leg2_token_number = query.value(rec.indexOf("Leg2_Tok_No")).toInt();
                int leg3_token_number = query.value(rec.indexOf("Leg3_Tok_No")).toInt();
                int leg4_token_number = query.value(rec.indexOf("Leg4_Tok_No")).toInt();



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

                if (portfolio_type == PortfolioType::BX_BID) {
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
                QString Stockname = ContractDetail::getInstance().GetInstrumentName(leg1_token_number,portfolio_type);
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
                   Leg1_OrderStateStr = Leg1_OrderStateStr+"["+(QString::number(Leg1_Total_Volume/lotSize))+"]";
                   // +" ("+(QString::number(Leg1_OrderState))+")"+" "
                }
                else
                {
                   Leg1_OrderStateStr = Leg1_OrderStateStr;
                                        //+" ("+(QString::number(Leg1_OrderState))+")";
                }
                if(Leg3_OrderState==8)
                {
                    Leg3_OrderStateStr = Leg3_OrderStateStr+"["+(QString::number(Leg3_Total_Volume/lotSize))+"]";
                  // +" ("+(QString::number(Leg3_OrderState))+")"+" "
                }
                else
                {
                    Leg3_OrderStateStr = Leg3_OrderStateStr;
                                         //+" ("+(QString::number(Leg3_OrderState))+")";
                }

                QString Exch_Price = "0";
                double Exch_Price_val  = 0;

                switch (portfolio_type) {
                case PortfolioType::F2F:{
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((leg2Price - leg1Price) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg1Price - leg2Price) * 1.0) / devicer;
                    }
                    break;
                }
                case PortfolioType::BY:{
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;
                    }

                    break;
                }
                case PortfolioType::CR:{
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
                    if(Leg1BuySellIndicator==1){
                        Exch_Price_val = static_cast<double>((2.0 * leg2Price - (leg1Price + leg3Price)) * 1.0) / devicer;
                    }
                    else{
                        Exch_Price_val = static_cast<double>((leg3Price + leg1Price) * 1.0 - (2.0 * leg2Price)) / devicer;

                    }

                    break;
                }
                case PortfolioType::BX_BID: {
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


                default:
                    break;
                }

                Exch_Price = QString::number(Exch_Price_val,'f',decimal_precision);
                QString User_Price = fixDecimal(((query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer),decimal_precision);//QString::number(static_cast<double>(query.value(rec.indexOf("DesiredRate")).toDouble()) / devicer, 'f', decimal_precision+1);

                double userPriceVal = query.value(rec.indexOf("DesiredRate")).toDouble() / devicer;
                double JackpotVal =(Exch_Price_val-userPriceVal);
                QString Jackpot = QString::number(JackpotVal,'f',decimal_precision);
 //               QString BidLeg = ContractDetail::getInstance().GetStockName(leg2_token_number,portfolio_type)+ " "+"["+(QString::number(Leg2_Total_Volume/lotSize))+"]";
                //int lotSize =  ContractDetail::getInstance().GetLotSize(leg1_token_number,portfolio_type);

                if(qty>0&&lotSize>0) // to prevent crash
                    qty = qty / lotSize;
                QString Traded_Lot = QString::number(qty);

                QString Remaining_Lot = QString::number(static_cast<double>(query.value(rec.indexOf("RemainingQty")).toDouble()) / lotSize);
                long long Trade_Time = query.value(rec.indexOf("Leg2_TimeOrderEnteredHost")).toLongLong();
                QDateTime dt = QDateTime::fromSecsSinceEpoch(Trade_Time);
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
                rowList.append(order_id);
                //                rowList.append(Volant_No);
                rowList.append(Algo_Name);
                rowList.append(User_Price);
                rowList.append(Exch_Price);
                rowList.append(Jackpot);
                rowList.append(Traded_Lot);
                rowList.append(Remaining_Lot);
                //  rowList.append(Buy_Sell);
                rowList.append(dt.toString("hh:mm:ss"));
                rowList.append(Leg1_OrderStateStr);
                rowList.append(Leg2_OrderStateStr);
                rowList.append(Leg3_OrderStateStr);
                rowList.append(Leg4_OrderStateStr);
                rowList.append(Algo_ID);
                rowList.append(QString::number(Leg1_OrderState));
                rowList.append(QString::number(Leg2_OrderState));
                rowList.append(QString::number(Leg3_OrderState));
                rowList.append(QString::number(Leg4_OrderState));
                rowList.append(Expiry);
                rowList.append(Buy_Sell);
                rowList.append(QString::number(lotSize));
                rowList.append(traderData);


//                rowList.append(Leg1_OrderState); // this should be the 4th last data inserted to the row
 //               rowList.append(Leg3_OrderState); // this should be the 3rd last data inserted to the row
  //              rowList.append(Leg2_OrderState); // this should be the 2nd last data inserted to the row
           //    rowList.append(traderData); // this should be the last data inserted to the row


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
            model->setDataList(trade_data_listTmp);

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
                                    Net_Position_Table_Model *model, QString user_id)
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

    bool ok = checkDBOpened(msg);
    if (ok) {
        // Define the SQL query to retrieve the desired data
        QString query_str = "SELECT Trades.TokenNo, Contract.StockName, Trades.BuySellIndicator, Contract.LotSize, ROUND((SUM(Trades.TradedPrice * Trades.TotalVolume) / SUM(Trades.TotalVolume) / " + QString::number(devicer) + "), 2) AS AvgPrice, SUM(TotalVolume) AS Qty, ROUND((SUM(Trades.TradedPrice * Trades.TotalVolume) / " + QString::number(devicer) + "), 2) AS buysellvalue FROM Trades JOIN Contract ON Contract.Token = Trades.TokenNo WHERE Trades.TraderId = '" + user_id + "' GROUP BY Trades.TokenNo, Trades.BuySellIndicator, Contract.StockName ORDER BY Contract.StockName ASC";
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
             int lotSize = query.value(rec.indexOf("LotSize")).toInt();
             double buysellvalue = query.value(rec.indexOf("buysellvalue")).toDouble();

             QString key = QString::number(TokenNo);
             if (net_pos_dataList.contains(key)) {
                    if (buy_sell == "1") {
                        net_pos_dataList[key].Buy_Avg_Price += AvgPrice;
                        net_pos_dataList[key].Buy_Total_Lot += Qty;
                        net_pos_dataList[key].Buy_Price += buysellvalue;
                    } else {
                        net_pos_dataList[key].Sell_Avg_Price += AvgPrice;
                        net_pos_dataList[key].Sell_Total_Lot += Qty;
                        net_pos_dataList[key].Sell_Price += buysellvalue;
                    }
             } else {
                    net_pos_data_ net_pos;
                    net_pos.Stock_Name = StockName;
                    net_pos.lotSize = lotSize;
                    net_pos.Buy_Avg_Price = (buy_sell == "1") ? AvgPrice : 0;
                    net_pos.Buy_Total_Lot = (buy_sell == "1") ? Qty : 0;
                    net_pos.Buy_Price = (buy_sell == "1") ? buysellvalue : 0;
                    net_pos.Sell_Avg_Price = (buy_sell == "2") ? AvgPrice : 0;
                    net_pos.Sell_Total_Lot = (buy_sell == "2") ? Qty : 0;
                    net_pos.Sell_Price = (buy_sell == "2") ? buysellvalue : 0;

                    net_pos_dataList.insert(key, net_pos);
             }
            }

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

             double Profit = net_pos_dataList[TokenNo].Sell_Price - net_pos_dataList[TokenNo].Buy_Price;
             Profit_summary += Profit;

             QStringList rowList;
             rowList.append(net_pos_dataList[TokenNo].Stock_Name);
             rowList.append(QString::number((net_pos_dataList[TokenNo].Buy_Total_Lot) / net_pos_dataList[TokenNo].lotSize));
             rowList.append(QString::number((net_pos_dataList[TokenNo].Sell_Total_Lot) / net_pos_dataList[TokenNo].lotSize));
             rowList.append(fixDecimal(net_pos_dataList[TokenNo].Buy_Price, decimal_precision));
             rowList.append(fixDecimal(net_pos_dataList[TokenNo].Sell_Price, decimal_precision));
             rowList.append(fixDecimal((net_pos_dataList[TokenNo].Buy_Avg_Price), decimal_precision));
             rowList.append(fixDecimal((net_pos_dataList[TokenNo].Sell_Avg_Price), decimal_precision));
             rowList.append(QString::number(net_pos_dataList[TokenNo].Net_Qty / net_pos_dataList[TokenNo].lotSize));
             rowList.append(fixDecimal(Profit, decimal_precision));
             rowList.append("-");
             rowList.append(TokenNo); // TokenNo should be the last one

             netPos_data_listTmp.append(rowList);
            }

            // Sort the data based on StockName (first column in rowList)
            std::sort(netPos_data_listTmp.begin(), netPos_data_listTmp.end(), [](const QStringList &a, const QStringList &b) {
                return a[0] < b[0]; // Sort by StockName
            });

            // Set sorted data to the model
            model->setDataList(netPos_data_listTmp);
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
        QString query_str = "SELECT * FROM MissedTrades WHERE TraderID='"+user_id+"'";
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
                double Price = query.value(rec.indexOf("Price")).toDouble();
                Price = Price/devicer;

                QString PriceStr = fixDecimal(Price,decimal_precision);
                QString QuantityStr = QString::number(Quantity/lotsize);
                QString PortfolioStr = QString::number(Portfolio);



                QStringList rowList;
                rowList.append(OrderId);
                rowList.append(BuySellIndicator);
                rowList.append(Type);
                rowList.append(QuantityStr);
                rowList.append(PortfolioStr);
                rowList.append(StockName);
                rowList.append(Message);
                rowList.append(PriceStr);
                rowList.append(DateTimeStr);


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
    QHash<QString, net_pos_data_> net_pos_dataList;

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


                double Profit = net_pos_dataList[TokenNo].Sell_Price-net_pos_dataList[TokenNo].Buy_Price;
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
                c++;

            }


            model->setDataList(netPos_data_listTmp);
        }
    }
}

QList<PortfolioType> mysql_conn::getPortfolioTypesForInstrumentType(const QString& data, const QMap<PortfolioType, QStringList>& algoFilterMap) {
    QList<PortfolioType> matchingPortfolios;

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

QHash<QString, contract_table>  mysql_conn::getContractTable( QHash<int , QStringList> &m_ContractDetails_Grouped_,userInfo userData)
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

                QStringList PortfolioAll;
                QList<PortfolioType>  portfolio_types = getPortfolioTypesForInstrumentType(contractTableTmp.InstrumentType,userData.algoFilterMap);
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
                            << contractTableTmp.TokenNumber
                            << contractTableTmp.StockName
                            << contractTableTmp.MinimumSpread
                            << contractTableTmp.VolumeFreezeQty;
                    }
                    else{
                        qDebug()<<"Waring: Cannot find portfolio type(InstrumentType) for contract table token = "<<contractTableTmp.TokenNumber;
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

algo_data_insert_status mysql_conn::insertToAlgoTable(algo_data_to_insert data,int MaxPortfolioCount,QString &msg){
    QMutexLocker lock(&mutex);
    algo_data_insert_status ret = algo_data_insert_status::FAILED;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);
            int PortfoliosCount = -1;
            query.prepare("SELECT * FROM Portfolios WHERE TraderID=" + (data.user_id));

            if( !query.exec() )
            {
                msg = query.lastError().text();
                qDebug()<<query.lastError().text();
            }
            else{
                PortfoliosCount = query.size();
                //if portfolio limit reaches do not insert
                if(PortfoliosCount>=MaxPortfolioCount){
                    ret = algo_data_insert_status::LIMIT_REACHED;
                    msg = "Maximum Portfolio Limit Reached";
                }
                else{
                    QString str = "";
                    //F2F
                    if(data.algo_type==QString::number(PortfolioType::F2F))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number;
                    //BFLY
                    else if(data.algo_type==QString::number(PortfolioType::BY))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number;
                    //CON-REV
                    else if(data.algo_type==QString::number(PortfolioType::CR))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number;

                    //CRJELLY
                    else if(data.algo_type==QString::number(PortfolioType::CR_JELLY))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number;

                    //BOX
                    else if(data.algo_type==QString::number(PortfolioType::BOX))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number+
                              " and Leg4TokenNo="+data.Leg4_token_number+
                              " and Leg5TokenNo="+data.Leg5_token_number;


                    //Open-BFLY
                    else if(data.algo_type==QString::number(PortfolioType::OPEN_BY))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number+
                              " and Leg1Ratio="+data.Leg1Ratio+
                              " and Leg2Ratio="+data.Leg2Ratio+
                              " and Leg3Ratio="+data.Leg3Ratio;
                    //Open-BOX
                    else if(data.algo_type==QString::number(PortfolioType::OPEN_BOX))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number+
                              " and Leg4TokenNo="+data.Leg4_token_number+
                              " and Leg5TokenNo="+data.Leg5_token_number+
                              " and Leg1Ratio="+data.Leg1Ratio+
                              " and Leg2Ratio="+data.Leg2Ratio+
                              " and Leg3Ratio="+data.Leg3Ratio+
                              " and Leg4Ratio="+data.Leg4Ratio;

                    //BFLY-BID
                    else if(data.algo_type==QString::number(PortfolioType::BFLY_BID))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number;

                    //BOX-BID
                    else if(data.algo_type==QString::number(PortfolioType::BX_BID))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number+
                              " and Leg4TokenNo="+data.Leg4_token_number;

                    //BX1221
                    else if(data.algo_type==QString::number(PortfolioType::BX1221))
                        str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                              " and PortfolioType="+ data.algo_type+
                              " and Leg1TokenNo="+data.Leg1_token_number+
                              " and Leg2TokenNo="+data.Leg2_token_number+
                              " and Leg3TokenNo="+data.Leg3_token_number+
                              " and Leg4TokenNo="+data.Leg4_token_number;

                    //CON-REV
                    //2L Straddle
                    /* else if(data.algo_type==_2L_Straddle)
                            str = "select  TraderID from Portfolios where TraderID="+data.user_id+
                                                    " and PortfolioType="+ data.algo_type+
                                                    " and Leg1TokenNo="+data.Leg1_token_number+
                                                    " and Leg2TokenNo="+data.Leg2_token_number;*/



                    query.prepare(str);

                    if( !query.exec() )
                    {
                        msg = query.lastError().text();
                        qDebug()<<query.lastError().text();
                    }
                    else{
                        if(query.size()>0){
                            ret = algo_data_insert_status::EXIST;
                            msg="Record already exist in DB";
                        }
                        else{
                            //F2F
                            if(data.algo_type==QString::number(PortfolioType::F2F)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,ModifyType) "
                                              "VALUES (:PortfolioType, :TraderID, :Status, "
                                              ":Leg1TokenNo, :Leg2TokenNo,"
                                              ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity,:ModifyType)");
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


                            }
                            //BFLY
                            else if(data.algo_type==QString::number(PortfolioType::BY)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity)");
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

                            }
                            //CON-REV
                            else if(data.algo_type==QString::number(PortfolioType::CR)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity)");
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

                            }

                            //CRJELLY
                            else if(data.algo_type==QString::number(PortfolioType::CR_JELLY)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,ModifyType) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity,:ModifyType)");
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

                            }
                            //BOX
                            else if(data.algo_type==QString::number(PortfolioType::BOX)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo, Leg3TokenNo, Leg4TokenNo, Leg5TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,AdditionalData1,AdditionalData2,AdditionalData3) "
                                              "VALUES (:PortfolioType, :TraderID, :Status, "
                                              ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo,"
                                              ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity, :AdditionalData1, :AdditionalData2, :AdditionalData3)");
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


                            }
                            //OPEN_BY
                            else if(data.algo_type==QString::number(PortfolioType::OPEN_BY)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,Leg1Ratio,Leg2Ratio,Leg3Ratio) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity,:Leg1Ratio,:Leg2Ratio,:Leg3Ratio)");
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

                            }
                            //OPEN BOX
                            else if(data.algo_type==QString::number(PortfolioType::OPEN_BOX)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo, Leg3TokenNo, Leg4TokenNo, Leg5TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,AdditionalData1,AdditionalData2,AdditionalData3,Leg1Ratio,Leg2Ratio,Leg3Ratio,Leg4Ratio) "
                                              "VALUES (:PortfolioType, :TraderID, :Status, "
                                              ":Leg1TokenNo, :Leg2TokenNo, :Leg3TokenNo, :Leg4TokenNo, :Leg5TokenNo,"
                                              ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity, :AdditionalData1, :AdditionalData2, :AdditionalData3,:Leg1Ratio,:Leg2Ratio,:Leg3Ratio,:Leg4Ratio)");
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

                            }
                            //BFLY_BID
                            else if(data.algo_type==QString::number(PortfolioType::BFLY_BID)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,ModifyType) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity,:ModifyType)");
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
                                query.bindValue(":ModifyType", 2);

                            }

                            //BFLY_BID
                            else if(data.algo_type==QString::number(PortfolioType::BX_BID)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo,Leg4TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity,ModifyType) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo,:Leg4TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity,:ModifyType)");
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

                            }

                            //BX1221l
                            else if(data.algo_type==QString::number(PortfolioType::BX1221)){
                                query.prepare("INSERT INTO Portfolios (PortfolioType, TraderID, Status, "
                                              "Leg1TokenNo, Leg2TokenNo,Leg3TokenNo,Leg4TokenNo"
                                              ",BuyPriceDifference,BuyTotalQuantity,BuyTradedQuantity,"
                                              "SellPriceDifference,SellTotalQuantity,SellTradedQuantity,"
                                              "OrderQuantity) "
                                              "VALUES (:PortfolioType, :TraderID, :Status,"
                                              ":Leg1TokenNo, :Leg2TokenNo,:Leg3TokenNo,:Leg4TokenNo"
                                              ",:BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity)");
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


algo_data_insert_status mysql_conn::place_F1F2_Order(QString userID,QString Leg1TokenNumber,QString sellprice,QString sellqty,QString buyprice,QString buyqty,int MaxPortfolioCount,QString &msg,bool checkDuplicateExist){
    QMutexLocker lock(&mutex);

    QString ClientID = "0";
    QString IsBroker = "1";
    algo_data_insert_status ret = algo_data_insert_status::FAILED;
    {
        bool ok = checkDBOpened(msg);
        if(ok){
            QSqlQuery query(db);
            int PortfoliosCount = -1;
            query.prepare("SELECT * FROM Portfolios");
            if( !query.exec() )
            {
                msg = query.lastError().text();
                qDebug()<<query.lastError().text();
                return ret;
            }

            PortfoliosCount = query.size();
            //if portfolio limit reaches do not insert
            if(PortfoliosCount>=MaxPortfolioCount){
                    ret = algo_data_insert_status::LIMIT_REACHED;
                    msg = "Maximum Portfolio Limit Reached";
                    return ret;
            }

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
                            query.bindValue(":OrderQuantity", 0);



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

