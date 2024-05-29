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

extern MainWindow *MainWindowObj;

//#include "CombinedTracker/combined_tracker_table_model.h"


mysql_conn::mysql_conn(QObject *parent,QString conne_name):
    QObject(parent)
{
   // MainWindowObj = (MainWindow*)parent;
    //connect(this,SIGNAL(display_log_text_signal(QString)),MainWindowObj,SLOT(slotAddLogForAddAlgoRecord(QString)));


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
            QSqlQuery query("SELECT UserId, UserName,Password,STKOpenLimit,IDXOpenLimit,MaxPortfolioCount,BFLY_BIDInFilter,BYInFilter,F2FInFilter FROM rt_usertable WHERE UserName='"+UserName_+"'", db);
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
                    userLoginInfo.UserName = query.value(rec.indexOf("UserName")).toString();
                    userNameLogged=userLoginInfo.UserName;
                    userLoginInfo.algoFilterMap[PortfolioType::BFLY_BID] = query.value(rec.indexOf("BFLY_BIDInFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::BY] = query.value(rec.indexOf("BYInFilter")).toString();
                    userLoginInfo.algoFilterMap[PortfolioType::F2F] = query.value(rec.indexOf("F2FInFilter")).toString();

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
            userLoginInfo.loginResponse= "Cannot connect Database"+db.lastError().text();
            userLoginInfo.errorCode = T_LoginErroCode::DB_ERROR;

        }
    }
    // QSqlDatabase::removeDatabase("login_conn");
    return userLoginInfo;
}
void  mysql_conn::getSummaryTableData()
{
    QMutexLocker lock(&mutex);
    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok){
         QString sqlquery ="SELECT COUNT(*) FROM Order_Table_Bid WHERE Order_Table_Bid.Trader_ID=1";
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

            int bidCount = query.value(0).toInt(); // Assuming the count is in the first column


        }
    }
}
void mysql_conn::getPortfoliosTableData(Table_Portfolios_Model *model, Combined_Tracker_Table_Model *comb_tracker_model, QHash<QString, PortfolioAvgPrice> &averagePriceList, QString user_id)
{
    QMutexLocker lock(&mutex);
    // int editing_state = portfolio_table_editing.loadRelaxed();
    PortfolioParser p;
    QList<PortfolioObject*> PortfolioObjectList;
    QList<QStringList> Combined_TrackerDataList;
    QString msg;
    SlowData slowData;
    QHash<QString, MBP_Data_Struct>  MBP_Data_Hash = slowData.getMBP_Data_Hash();

    bool ok = checkDBOpened(msg);
    if(ok){
        QString sqlquery = "SELECT PortfolioNumber, (sum(TradedPrice*TotalVolume) * 1.0 / sum(TotalVolume))/"+QString::number(devicer)+" as AvgPrice, TokenNo, BuySellIndicator from Trades WHERE TraderID='"+user_id+"' group by PortfolioNumber, TokenNo, BuySellIndicator ";
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
                PortfolioObject *o = new PortfolioObject();
                if(!p.ToObject(query,*o,MBP_Data_Hash,averagePriceList,devicer,decimal_precision)){
                    //show warning an delete portfolio
                }
                //o->BuyTotalQuantity = QDateTime::currentSecsSinceEpoch(); // this need to remove


                PortfolioObjectList.append(o);

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
                    int OrderQuantity = o->OrderQuantity * lotSize;
                    int qty = SellQuantity > BuyQuantity ? BuyQuantity : SellQuantity;
                    if (qty > 0)
                        profit = (o->SellAveragePrice.toDouble() - o->BuyAveragePrice.toDouble())* qty;
                    combineTrackerDataTmp[CombinedTrackerData_Idx::CT_Profit] = QString::number(profit);
                    Combined_TrackerDataList.append(combineTrackerDataTmp);
                }

            }
            // qDebug() << "Elapsed Time:" << timer.elapsed() << "milliseconds";

            comb_tracker_model->setDataList(Combined_TrackerDataList);

            //if table is not editing, then only update data. Alos make sure table not edited while data is loading from sql
            //if(portfolio_table_editing.loadRelaxed()==0&&editing_state==portfolio_table_editing.loadRelaxed())
            if(portfolio_table_updating_db.loadRelaxed()==0)
                model->setDataList(PortfolioObjectList);
        }
    }
    // qDebug()<<"getPortfoliosTableData ----------------mutex unlocked";
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

        QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:0.4;'>"
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
            QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;'>"
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

bool mysql_conn::updateDB_Table(QString query_str)
{
    qDebug()<<"DB Update Query: "<<query_str;
    QMutexLocker lock(&mutex);

    bool ret = false;

    QString msg;
    bool ok = checkDBOpened(msg);
    if(ok){

        QSqlQuery query(query_str, db);
        if( !query.exec() )
        {

            msg = query_str+" failed, "+ query.lastError().text();
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
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::OPEN_BY){
        Algo_Name = "OpenBfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::CR){
        Algo_Name = "CR-";//Nifty-17000";
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
        Algo_Name = "BflyBid-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }return Algo_Name.toUpper();
}

  QList<QHash<QString,QString>>  mysql_conn::getTradePopUPData(QString user_id, QString portfolioNumber,QString PortfolioType){

    QList<QHash<QString,QString>> tradeData;
    QMutexLocker lock(&mutex);

    QList <QStringList> trade_data_listTmp;
    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok)
    {
       // QString query_str = "SELECT * FROM Order_Table_Bid WHERE Trader_ID='"+user_id+"' and Leg2_OrderState=7 and PortfolioNumber="+portfolioNumber+" ORDER BY Trader_Data DESC";
        QString query_str = QString("SELECT * FROM Order_Table_Bid WHERE Trader_ID='%1' "
                                    "AND Leg2_OrderState=7 AND (PortfolioNumber=%2 OR PortfolioNumber=%3) "
                                    "ORDER BY Trader_Data DESC")
                                .arg(user_id)
                                .arg(portfolioNumber)
                               .arg(portfolioNumber.toInt() + 1500000);

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
                case PortfolioType::BFLY_BID:{
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
                tradeData.append(tmp);
            }

        }
    }
    return tradeData;
}

void mysql_conn::getTradeTableData(Trade_Table_Model *model, QString user_id, QHash<QString, PortFolioData_Less> PortFolioTypeHash)
{
    QMutexLocker lock(&mutex);

    QList <QStringList> trade_data_listTmp;
    // QList <QStringList> algo_pos_data_listTmp;

    // QHash<QString, alog_pos_data_> alog_pos_data;
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

                int qty = query.value(rec.indexOf("Leg1_Total_Volume")).toInt();
                //int leg2qty = query.value(rec.indexOf("Leg2_Total_Volume")).toInt();
                int leg1Price = query.value(rec.indexOf("Leg1_Price")).toInt();
                int leg2Price = query.value(rec.indexOf("Leg2_Price")).toInt();
                int leg3Price = query.value(rec.indexOf("Leg3_Price")).toInt();
                int Leg1BuySellIndicator = query.value(rec.indexOf("Leg1_Buy/Sell")).toInt();
                int Leg1_OrderState = query.value(rec.indexOf("Leg1_OrderState")).toInt();
                int Leg3_OrderState = query.value(rec.indexOf("Leg3_OrderState")).toInt();
                QString Leg1_OrderStateStr = "-";
                QString Leg3_OrderStateStr = "-";

                if(Leg1_OrderState==1){
                    Leg1_OrderStateStr = "Sent to Exachange";
                }
                else if(Leg1_OrderState==5){
                    Leg1_OrderStateStr = "Cancelled";
                }
                else if(Leg1_OrderState==6){
                    Leg1_OrderStateStr = "Rejected";
                }
                else if(Leg1_OrderState==7){
                    Leg1_OrderStateStr = "Traded";
                }
                else if(Leg1_OrderState==9){
                    Leg1_OrderStateStr = "Open";
                }
                else if(Leg1_OrderState==10){
                    Leg1_OrderStateStr = "ModifyPending";
                }
                else if(Leg1_OrderState==12){
                    Leg1_OrderStateStr = "CancelPending";
                }

                if(Leg3_OrderState==1){
                    Leg3_OrderStateStr = "Sent to Exachange";
                }
                else if(Leg3_OrderState==5){
                    Leg3_OrderStateStr = "Cancelled";
                }
                else if(Leg3_OrderState==6){
                    Leg3_OrderStateStr = "Rejected";
                }
                else if(Leg3_OrderState==7){
                    Leg3_OrderStateStr = "Traded";
                }
                else if(Leg3_OrderState==9){
                    Leg3_OrderStateStr = "Open";
                }
                else if(Leg3_OrderState==10){
                    Leg3_OrderStateStr = "ModifyPending";
                }
                else if(Leg3_OrderState==12){
                    Leg3_OrderStateStr = "CancelPending";
                }





                Leg1_OrderStateStr = Leg1_OrderStateStr+" ("+(QString::number(Leg1_OrderState))+")";
                Leg3_OrderStateStr = Leg3_OrderStateStr+" ("+(QString::number(Leg3_OrderState))+")";


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
                case PortfolioType::BFLY_BID:{
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



                QStringList rowList;
                rowList.append(Algo_ID);
//                rowList.append(Volant_No);
                rowList.append(Algo_Name);
                rowList.append(Exch_Price);
                rowList.append(User_Price);
                rowList.append(Jackpot);
                rowList.append(Traded_Lot);
                rowList.append(Remaining_Lot);
                rowList.append(Buy_Sell);
                rowList.append(dt.toString("hh:mm:ss"));
                rowList.append(Leg1_OrderStateStr);
                rowList.append(Leg3_OrderStateStr);
                rowList.append(Expiry);


                trade_data_listTmp.append(rowList);

            }
            model->setDataList(trade_data_listTmp);

        }
    }
}

void mysql_conn::getLinersTableData(QString user_id)
{
    QMutexLocker lock(&mutex);

    QList <QStringList> liners_list;
    // QList <QStringList> algo_pos_data_listTmp;

    // QHash<QString, alog_pos_data_> alog_pos_data;
    QString msg;

    bool ok = checkDBOpened(msg);
    if(ok){
      //int  i=0;
    }
}


void mysql_conn::getNetPosTableData(Net_Position_Table_Model* model,QString user_id)
{
    QMutexLocker lock(&mutex);

    //query: SELECT (order_table.Leg2_Price - order_table.Leg1_Price)/10000000 AS Exch_price , order_table.DesiredRate/10000000 as user_price,order_table.Trader_Data FROM  order_table WHERE order_table.OrderState=7
    /*  QList <QStringList> netPos_data_listTmp;
        QHash<QString, net_pos_data_> net_pos_data;
        QString msg;

        bool ok = checkDBOpened(msg);
        if(ok){
            QString query_str = "SELECT * from Trades WHERE  TraderId='"+user_id+"'";
           // qDebug()<<query_str;
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

                                                      QString token_number = query.value(rec.indexOf("TokenNo")).toString();
                                                      QString executed_quantity = query.value(rec.indexOf("TotalVolume")).toString();
                                                      QString executed_price = query.value(rec.indexOf("TradedPrice")).toString();
                                                      QDateTime dt = ContractDetail::getInstance().GetExpiryDate(token_number.toInt());
                                                      dt = dt.addYears(10);
                                                      QString Expiry=dt.toString("dd MMM yy");
                                                      QString stock_name = query.value(rec.indexOf("StockName")).toString();
                                                      net_pos_data[token_number].token_number = token_number;
                                                      net_pos_data[token_number].Stock_Name = stock_name;
                                                      net_pos_data[token_number].Expiry = Expiry;
                                                      if(buy_sell=="1"){
                                                          net_pos_data[token_number].Buy_Total_Lot.append(executed_quantity.toFloat()/ContractDetail::getInstance().GetLotSize(token_number.toInt()));
                                                          net_pos_data[token_number].Buy_Price.append(executed_price.toFloat());
                                                          net_pos_data[token_number].Buy_Avg_Price.append(executed_price.toFloat());
                                                      }
                                                      else{
                                                          net_pos_data[token_number].Sell_Total_Lot.append(executed_quantity.toFloat()/ContractDetail::getInstance().GetLotSize(token_number.toInt()));
                                                          net_pos_data[token_number].Sell_Price.append(executed_price.toFloat());
                                                          net_pos_data[token_number].Sell_Avg_Price.append(executed_price.toFloat());
                                                      }

                           }
           }

                        QHashIterator<QString, net_pos_data_> iter(net_pos_data);
                            while(iter.hasNext())
                            {
                                iter.next();
                                double Buy_Total_Lot = 0;
                                double Sell_Total_Lot=0;

                                double Buy_Price = 0;
                                double Sell_Price = 0;

                                double Buy_Avg_Price = 0;
                                double Sell_Avg_Price = 0;


                                double Net_Qty = 0;

                                QString token_nmber =  iter.key();


                                for(int i=0;i<net_pos_data[token_nmber].Buy_Total_Lot.length();i++){
                                    Buy_Total_Lot = Buy_Total_Lot+net_pos_data[token_nmber].Buy_Total_Lot.at(i);
                                }
                                for(int i=0;i<net_pos_data[token_nmber].Sell_Total_Lot.length();i++){
                                    Sell_Total_Lot = Sell_Total_Lot+net_pos_data[token_nmber].Sell_Total_Lot.at(i);
                                }
                                for(int i=0;i<net_pos_data[token_nmber].Buy_Avg_Price.length();i++){
                                    Buy_Avg_Price = Buy_Avg_Price+net_pos_data[token_nmber].Buy_Avg_Price.at(i);
                                }
                                for(int i=0;i<net_pos_data[token_nmber].Sell_Avg_Price.length();i++){
                                    Sell_Avg_Price = Sell_Avg_Price+net_pos_data[token_nmber].Sell_Avg_Price.at(i);
                                }

                                if(Buy_Avg_Price>0&&net_pos_data[token_nmber].Buy_Avg_Price.length()>0)
                                    Buy_Avg_Price = Buy_Avg_Price/net_pos_data[token_nmber].Buy_Avg_Price.length();
                                if(Sell_Avg_Price>0&&net_pos_data[token_nmber].Sell_Avg_Price.length()>0)
                                    Sell_Avg_Price = Sell_Avg_Price/net_pos_data[token_nmber].Sell_Avg_Price.length();


                                Buy_Price = (Buy_Total_Lot*Buy_Avg_Price)/devicer;
                                Sell_Price = (Sell_Total_Lot*Sell_Avg_Price)/devicer;
                                Buy_Avg_Price = Buy_Avg_Price/devicer;
                                Sell_Avg_Price = Sell_Avg_Price/devicer;
                                Net_Qty = Buy_Total_Lot-Sell_Total_Lot;


                                QStringList rowList;
                                rowList.append(net_pos_data[token_nmber].Stock_Name);
                                rowList.append(net_pos_data[token_nmber].Expiry);
                                rowList.append(QString::number(Buy_Total_Lot));
                                rowList.append(QString::number(Sell_Total_Lot));
                                rowList.append(fixDecimal(Buy_Price,decimal_precision));
                                rowList.append(fixDecimal(Sell_Price,decimal_precision));
                                rowList.append(fixDecimal(Buy_Avg_Price,decimal_precision));
                                rowList.append(fixDecimal(Sell_Avg_Price,decimal_precision));
                                rowList.append(QString::number(Net_Qty));
                                rowList.append("NA");
                                rowList.append(token_nmber); // this should be the last one



                                netPos_data_listTmp.append(rowList);
                            }

                            model->setDataList(netPos_data_listTmp);

        }*/

    //Qury: SELECT TokenNo, (sum(TradedPrice * TotalVolume) / sum(TotalVolume)) /10000000  as AvgPrice, sum(TotalVolume) as Qty, BuySellIndicator FROM Trades where TraderId = 1 group by TokenNo, BuySellIndicator;
    QString msg;
    QList <QStringList> netPos_data_listTmp;
    QHash<QString, net_pos_data_> net_pos_dataList;

    bool ok = checkDBOpened(msg);
    if(ok){
        QString query_str ="SELECT TokenNo,StockName, (sum(TradedPrice * TotalVolume) / sum(TotalVolume)) /"+QString::number(devicer)+"  as AvgPrice, sum(TotalVolume) as Qty, BuySellIndicator FROM Trades where TraderId = '"+user_id+"'group by TokenNo, BuySellIndicator,StockName";
        //QString Buy_Avg_Price="SELECT Trades.TokenNo, FORMAT((SUM(Trades.TradedPrice * Trades.TotalVolume)/ SUM(Trades.TotalVolume)/100),2) AS avgpri, Trades.BuySellIndicator FROM Trades WHERE Trades.BuySellIndicator=1 GROUP BY Trades.TokenNo,Trades.BuySellIndicator";

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

                QString key = QString::number(TokenNo);
                if(net_pos_dataList.contains(key)){
                    if(buy_sell=="1"){
                        net_pos_dataList[key].Buy_Avg_Price = net_pos_dataList[key].Buy_Avg_Price+AvgPrice;
                        net_pos_dataList[key].Buy_Total_Lot = net_pos_dataList[key].Buy_Total_Lot + Qty;
                    }
                    else{
                        net_pos_dataList[key].Sell_Avg_Price =AvgPrice;
                        net_pos_dataList[key].Sell_Total_Lot = Qty;
                    }
                }
                else{
                    net_pos_data_ net_pos;
                    net_pos.Stock_Name = StockName;

                    if(buy_sell=="1"){
                        net_pos.Buy_Avg_Price =AvgPrice;
                        net_pos.Buy_Total_Lot = Qty;
                    }
                    else{
                        net_pos.Sell_Avg_Price =AvgPrice;
                        net_pos.Sell_Total_Lot = Qty;
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
           while(iter.hasNext())
            {
                iter.next();
                QString TokenNo =  iter.key();
                QDateTime dt;//need portfolio type later = ContractDetail::getInstance().GetExpiryDate(TokenNo.toInt());
                dt = dt.addYears(10);
               // QString stock_name=ContractDetail::getInstance().GetStockName(TokenNo.toInt(),portfolio_type);
                net_pos_dataList[TokenNo].Net_Qty = net_pos_dataList[TokenNo].Sell_Total_Lot-net_pos_dataList[TokenNo].Buy_Total_Lot;
                net_pos_dataList[TokenNo].Buy_Price =net_pos_dataList[TokenNo].Buy_Avg_Price*net_pos_dataList[TokenNo].Buy_Total_Lot;
                net_pos_dataList[TokenNo].Sell_Price = net_pos_dataList[TokenNo].Sell_Total_Lot*net_pos_dataList[TokenNo].Sell_Avg_Price;

                double Profit = net_pos_dataList[TokenNo].Sell_Price-net_pos_dataList[TokenNo].Buy_Price;
                QStringList rowList;
                rowList.append(net_pos_dataList[TokenNo].Stock_Name);
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Buy_Total_Lot,decimal_precision));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Sell_Total_Lot,decimal_precision));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Buy_Price,decimal_precision));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Sell_Price,decimal_precision));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Buy_Avg_Price,decimal_precision));
                rowList.append(fixDecimal(net_pos_dataList[TokenNo].Sell_Avg_Price,decimal_precision));
                rowList.append(QString::number(net_pos_dataList[TokenNo].Net_Qty));
                rowList.append(fixDecimal(Profit,decimal_precision));
                rowList.append("-");
                rowList.append(TokenNo); // this should be the last one
                netPos_data_listTmp.append(rowList);
            }
            model->setDataList(netPos_data_listTmp);
        }
    }
}

QMap <int, QHash<QString, contract_table>> mysql_conn::getContractTable(
    QHash<QString, QStringList> &_m_ContractDetailsFiltered,
    QStringList &F2F_data_list_Sorted_Key,
    QStringList &BFLY_data_list_Sorted_Key,
    QStringList &BFLY_BID_data_list_Sorted_Key,
    userInfo userData)
{

    QMutexLocker lock(&mutex);

    QMap <int, QHash<QString, contract_table>> contractTableData;
    QString msg;
    bool ok = checkDBOpened(msg);
    if(ok)
    {
        contractTableData[PortfolioType::BFLY_BID] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::BFLY_BID, userData),&db, BFLY_BID_data_list_Sorted_Key);
        contractTableData[PortfolioType::BY] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::BY,userData),&db, BFLY_data_list_Sorted_Key);
        contractTableData[PortfolioType::F2F] = prpareContractDataFromDB(getAlgoTypeQuery(PortfolioType::F2F,userData),&db,F2F_data_list_Sorted_Key);
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
                    msg = "Delete skipped, Record exist in Trades table.";
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

algo_data_insert_status mysql_conn::insertToAlgoTable(algo_data_to_insert data,int MaxPortfolioCount,QString &msg){
    QMutexLocker lock(&mutex);
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
                                              "OrderQuantity) "
                                              "VALUES (:PortfolioType, :TraderID, :Status, "
                                              ":Leg1TokenNo, :Leg2TokenNo,"
                                              ":BuyPriceDifference,:BuyTotalQuantity,:BuyTradedQuantity,"
                                              ":SellPriceDifference,:SellTotalQuantity,:SellTradedQuantity,"
                                              ":OrderQuantity)");
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

    for(const auto &algoFilter: userLoginInfo.algoFilterMap[type].split(","))
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

    return "SELECT InstrumentType, InstrumentName, OptionType, StrikePrice, LotSize, ExpiryDate, Token, StockName, MinSpread FROM Contract" +
           whereStr +
           " ORDER BY ExpiryDate,Token, InstrumentName, OptionType, StrikePrice";
}
