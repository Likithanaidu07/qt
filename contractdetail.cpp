#include"ContractDetail.h"
#include "mysql_conn.h"
#include <iostream>
#include <string>
#include <sstream>

ContractDetail* ContractDetail::instance = nullptr;
QMutex ContractDetail::mutex;
QHash<int , QStringList> ContractDetail::m_ContractDetails_Grouped; //contract tables gropuped based on algoType

QHash<QString, contract_table> ContractDetail::m_ContractDetails_Hash; //store the eniter contract table

QHash<QString, QStringList> ContractDetail::m_ContractDetailsFiltered;

QHash<QString, int> ContractDetail::m_SettPrice;

//QStandardItemModel* ContractDetail::model_searchInstrument_BOX_Leg1  = nullptr;
//QStandardItemModel* ContractDetail::model_start_strike_BFLY = nullptr;
//QStandardItemModel* ContractDetail::model_searchInstrument_F2F_Leg1 = nullptr;
//QStandardItemModel* ContractDetail::model_FUT_CON_REV = nullptr;
//QStandardItemModel* ContractDetail::model_start_strike_BFLY_BID = nullptr;
//QStandardItemModel* ContractDetail::model_start_strike_BOX_BID = nullptr;
//QStandardItemModel* ContractDetail::model_F1_F2 = nullptr;

double ContractDetail::devicer_contract;
double ContractDetail::decimal_precision_contract;
bool ContractDetail::settingLoaded = false;



double estimateModelMemoryUsageMB(QStandardItemModel* model) {
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    double totalSizeBytes = 0.0;

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < columnCount; ++col) {
            QStandardItem* item = model->item(row, col);
            if (item) {
                // Estimate the size of the data stored in each item
                QVariant data = item->data();
                QByteArray byteArray = data.toString().toUtf8(); // For strings or general data
                totalSizeBytes += static_cast<double>(byteArray.size());
            }
        }
    }

    // Convert bytes to megabytes (MB)
    double totalSizeMB = totalSizeBytes / (1024.0 * 1024.0);
    return totalSizeMB;
}
ContractDetail::ContractDetail() noexcept
{
    //        model_searchInstrument_BOX_Leg1 = new QStandardItemModel;
//    model_start_strike_BFLY = new QStandardItemModel;
//    model_searchInstrument_F2F_Leg1 = new QStandardItemModel;
//    model_FUT_CON_REV = new QStandardItemModel;
//    //model_start_strike_BFLY_BID = new QStandardItemModel;
//    model_start_strike_BOX_BID = new QStandardItemModel;

//    model_F1_F2 = new QStandardItemModel;
}

ContractDetail &ContractDetail::getInstance()
{
    if(!settingLoaded){
        //currently hardcoded for FO
        devicer_contract = FO_DEVICER;
        decimal_precision_contract = FO_DECIMAL_PRECISION;
        /*QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
        QStringList groups = settings.childGroups();
        QString market_type = "fo";
        if(groups.contains("GeneralSettings")){
            settings.beginGroup("GeneralSettings");
            if (settings.contains("market_type"))
                market_type = settings.value("market_type").toString();

            settings.endGroup();
        }

        if(market_type=="fo"){
            devicer_contract = FO_DEVICER;
            decimal_precision_contract = FO_DECIMAL_PRECISION;
        }
        else{
            devicer_contract = CDS_DEVICER;
            decimal_precision_contract = CDS_DECIMAL_PRECISION;
        }*/

        settingLoaded = true;
    }
    // Double-checked locking to avoid unnecessary locking
    /* if (instance)
        {
            QMutexLocker lock(&mutex);
            if (!instance)
            {

                instance = new ContractDetail();
            }
        }
        return *instance;*/

    // Check if the instance already exists without locking
    if (!instance)
    {
        // Acquire the lock
        QMutexLocker lock(&mutex);

        // Check again inside the lock to prevent race conditions
        if (!instance)
        {
            instance = new ContractDetail();
        }
    }
    return *instance;
}

void ContractDetail::ReloadContractDetails(userInfo uData)
{
    // MySQL_Conn con(0,"contract_conn");
    //m_ContractDetails = con.getContractTable(m_ContractDetailsFiltered);
    // return; // below  need to read write using Qt

    userData = uData;

#ifdef STORE_CONTRACT_LOCALLY
    //check data stored in Local file and load from there
     if (!LoadContractLocal(m_ContractDetailsFiltered))
     {
#endif

    mysql_conn con(0,"contract_conn");
    m_ContractDetails_Hash = con.getContractTable(m_ContractDetails_Grouped,m_ContractDetailsFiltered,uData);


    qDebug()<<"Loaded ContractDetails from DB size="<<m_ContractDetails_Hash.size();

#ifdef STORE_CONTRACT_LOCALLY
       // StoreContractToLocalFile();
    }
#endif

  // create_inputFiledAutoFillModel_For_AddAlgoWindow();
    //std::sort(m_ContractDetails.begin(), m_ContractDetails.end(), [](const ContractDetail& a, const ContractDetail& b) { return a.InstrumentName < b.InstrumentName; });
}

QHash<QString, contract_table> ContractDetail::GetContracts()
{
    /*  if (m_ContractDetails.empty())
        {
            ReloadContractDetails();
        }*/

    return m_ContractDetails_Hash;
}

//This function create autfill model for first input field in add algo window(frist leg)
//void ContractDetail::create_inputFiledAutoFillModel_For_AddAlgoWindow()
//{
//    //        model_searchInstrument_BOX_Leg1->clear();
//    model_start_strike_BFLY->clear();
//    model_searchInstrument_F2F_Leg1->clear();
//    model_FUT_CON_REV->clear();
//   // model_start_strike_BFLY_BID->clear();
//    model_start_strike_BOX_BID->clear();
//    model_F1_F2->clear();

//    QSet<QString> model_data_name_set;
//    QStringList BFLY_Tokens  = m_ContractDetails_Grouped[PortfolioType::BY];

//    QElapsedTimer timer;
//    QElapsedTimer timer1;
//    timer1.start();

//    timer.start();
//    for(int i=0;i<BFLY_Tokens.length();i++){
//        const auto& contract = m_ContractDetails_Hash[BFLY_Tokens[i]];

//        /**********Create model for BFLY *************************/
//        unsigned int unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        int targetYear = dt.date().year();
//        bool isLeapYear = QDate::isLeapYear(targetYear);
//        QString instrument_name = contract.InstrumentName;

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }
//        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
//        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
//        QStandardItem *itemBFLY = new QStandardItem;
//        itemBFLY->setText(algo_combination);
//        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
//        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd") + "-" + QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);
//        // Set the composite key as data for sorting
//        itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_start_strike_BFLY->appendRow(itemBFLY);

//        /********************************************************************/

//    }
//    // After the loop, calculate the elapsed time
//    qint64 elapsedTime = timer.elapsed();  // Time in milliseconds
//    qDebug() << "Time model_start_strike_BFLY:" << elapsedTime << "milliseconds";
//    qDebug() << "Time BFLY_Tokens Size:" << BFLY_Tokens.length();

//    //doubt
//    QSet<QString> model_data_name_set_bfly_bid;
//    QStringList BFLY_BID_Tokens  = m_ContractDetails_Grouped[PortfolioType::BFLY_BID];

//    for(int i=0;i<BFLY_BID_Tokens.length();i++){
//        /**********Create model for Mainwindow searchbar*************************/
//        const auto& contract = m_ContractDetails_Hash[BFLY_BID_Tokens[i]];
//        QString stock_name=contract.StockName;

//        QString name = stock_name;
//        if(!model_data_name_set_bfly_bid.contains(name)){
//            model_data_name_set_bfly_bid.insert(name);
//            QStandardItem *itemBOXL1 = new QStandardItem;
//            itemBOXL1->setText(name);
//            itemBOXL1->setData(contract.StockName, Qt::UserRole + 1);
//            //                model_searchInstrument_BOX_Leg1->appendRow(itemBOXL1);
//        }
//        /********************************************************************/
//    }

//    for(int i=0;i<BFLY_BID_Tokens.length();i++){
//        /**********Create model for BOX*************************/
//        const auto& contract = m_ContractDetails_Hash[BFLY_BID_Tokens[i]];
//        unsigned int unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
//        QString instrument_name = contract.InstrumentName;
//        QString strik_price = QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);

//        QString name = instrument_name+" "+Expiry+" "+strik_price;
//        if(!model_data_name_set_bfly_bid.contains(name)){
//            model_data_name_set_bfly_bid.insert(name);
//            QStandardItem *itemBOXL1 = new QStandardItem;
//            itemBOXL1->setText(name);
//            itemBOXL1->setData(contract.TokenNumber, Qt::UserRole + 1);
//            //                model_searchInstrument_BOX_Leg1->appendRow(itemBOXL1);
//        }
//        /********************************************************************/

//        /**********Create model for BFLY BID*************************/

//        unix_time= contract.Expiry;
//        dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        int targetYear = dt.date().year();
//        bool isLeapYear = QDate::isLeapYear(targetYear);

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }
//        Expiry=dt.toString("MMM dd yyyy").toUpper();
//        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
//        QStandardItem *itemBFLY = new QStandardItem;
//        itemBFLY->setText(algo_combination);
//        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);

//        // Create custom data for sorting
//        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
//        // Set the composite key as data for sorting
//        itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_start_strike_BFLY_BID->appendRow(itemBFLY);

//        /********************************************************************/

//    }
//    //doubt


//    QStringList F2F_Tokens  = m_ContractDetails_Grouped[PortfolioType::F2F];

//    for(int i=0;i<F2F_Tokens.length();i++){

//        /**********Create model for _F2F*************************/
//        const auto& contract = m_ContractDetails_Hash[F2F_Tokens[i]];
//        unsigned int unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        int targetYear = dt.date().year();
//        bool isLeapYear = QDate::isLeapYear(targetYear);

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }
//        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
//        QString instrument_name = contract.InstrumentName;


//        QStandardItem *itemF2FL1 = new QStandardItem;
//        itemF2FL1->setText(instrument_name+" "+Expiry);
//        itemF2FL1->setData(contract.TokenNumber, Qt::UserRole + 1);
//        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd");
//        // Set the composite key as data for sorting
//        itemF2FL1->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_searchInstrument_F2F_Leg1->appendRow(itemF2FL1);
//        /********************************************************************/


//        /**********Create model for CON_REV*************************/
//        instrument_name = contract.InstrumentName;//" "+Expiry;
//        unix_time= contract.Expiry;
//        dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        int targetYear_cr = dt.date().year();
//        bool isLeapYear_cr = QDate::isLeapYear(targetYear);

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }
//        Expiry=dt.toString("MMM dd yyyy").toUpper();

//        QStandardItem *itemFut = new QStandardItem;
//        itemFut->setText(instrument_name+" "+Expiry);
//        itemFut->setData(contract.TokenNumber, Qt::UserRole + 1);
//        compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd");
//        // Set the composite key as data for sorting
//        itemFut->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_FUT_CON_REV->appendRow(itemFut);

//        /********************************************************************/

//    }


//    QStringList CR_Tokens  = m_ContractDetails_Grouped[PortfolioType::CR];

//    for(int i=0;i<CR_Tokens.length();i++){

//    /**********Create model for CON_REV*************************/
//        const auto& contract = m_ContractDetails_Hash[CR_Tokens[i]];

//        unsigned int unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        int targetYear = dt.date().year();
//        bool isLeapYear = QDate::isLeapYear(targetYear);

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }
//        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
//        QString instrument_name = contract.InstrumentName;

//      //  QStandardItem *itemFut = new QStandardItem;
//      //  itemFut->setText(instrument_name+" "+Expiry);
//      //  itemFut->setData(contract.TokenNumber, Qt::UserRole + 1);
//        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd");
//        // Set the composite key as data for sorting
//       // itemFut->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//       // model_FUT_CON_REV->appendRow(itemFut);

//    /********************************************************************/


//        /**********Create model for F1_F2*************************/
//        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
//        QStandardItem *itemF1_F2 = new QStandardItem;
//        itemF1_F2->setText(algo_combination);
//        itemF1_F2->setData(contract.TokenNumber, Qt::UserRole + 1);
//        // Set the composite key as data for sorting
//        itemF1_F2->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_F1_F2->appendRow(itemF1_F2);

//        /********************************************************************/
//    }

//    /**********Create model for BX_BID*************************/
//    QStringList BX_BID_Tokens  = m_ContractDetails_Grouped[PortfolioType::BX_BID];

//    for(int i=0;i<BX_BID_Tokens.length();i++){
//        const auto& contract = m_ContractDetails_Hash[BX_BID_Tokens[i]];
//        //Only CE record need to avoaid duplicate
//        if(contract.OptionType=="PE")
//            continue;
//        unsigned int unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        // Check if the target year is a leap year
//        int targetYear = dt.date().year();
//        bool isLeapYear = QDate::isLeapYear(targetYear);

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }

//        QString Expiry = dt.toString("MMM dd yyyy").toUpper();

//        QString instrument_name = contract.InstrumentName;

//        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);
//        QStandardItem *itemBOX_BID = new QStandardItem;
//        itemBOX_BID->setText(algo_combination);
//        itemBOX_BID->setData(contract.TokenNumber, Qt::UserRole + 1);

//        QString strik_price = QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);
//        // Create custom data for sorting
//        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
//        // Set the composite key as data for sorting
//        itemBOX_BID->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_start_strike_BOX_BID->appendRow(itemBOX_BID);
//   }
//    /********************************************************************/

//    qDebug() << "Time  Total:" << timer1.elapsed() << "milliseconds";

//       qDebug() << "model_searchInstrument_F2F_Leg1: " << estimateModelMemoryUsageMB(model_start_strike_BFLY) << " MB";
//       qDebug() << "model_searchInstrument_F2F_Leg1: " << estimateModelMemoryUsageMB(model_searchInstrument_F2F_Leg1) << " MB";
//       qDebug() << "model_FUT_CON_REV: " << estimateModelMemoryUsageMB(model_FUT_CON_REV) << " MB";
//      // qDebug() << "model_start_strike_BFLY_BID: " << estimateModelMemoryUsageMB(model_start_strike_BFLY_BID) << " MB";
//       qDebug() << "model_start_strike_BOX_BID: " << estimateModelMemoryUsageMB(model_start_strike_BOX_BID) << " MB";
//       qDebug() << "model_F1_F2: " << estimateModelMemoryUsageMB(model_F1_F2) << " MB";

//}

//QStandardItemModel* ContractDetail::Get_model_searchInstrument_F2F_Leg1()
//{
//    return model_searchInstrument_F2F_Leg1;
//}


//QStandardItemModel* ContractDetail::Get_model_FUT_CON_REV()
//{
//    return model_FUT_CON_REV;
//}


//QStandardItemModel* ContractDetail::Get_model_start_strike_BFLY()
//{
//    return model_start_strike_BFLY;
//}

//QStandardItemModel* ContractDetail::Get_model_start_strike_BFLY_BID()
//{
//    return model_start_strike_BFLY_BID;
//}
//QStandardItemModel* ContractDetail::Get_model_start_strike_BOX_BID()
//{
//    return model_start_strike_BOX_BID;
//}


//QStandardItemModel* ContractDetail::Get_model_F1_F2()
//{
//    return model_F1_F2;
//}
//QStringList ContractDetail::Get_F2F_data_list_Sorted_Key()
//{
//    return m_ContractDetails_Grouped[PortfolioType::F2F];
//}

//QStringList ContractDetail::Get_BFLY_data_list_Sorted_Key()
//{
//    return  m_ContractDetails_Grouped[PortfolioType::BY];
//}

//QStringList ContractDetail::Get_BFLY_BID_data_list_Sorted_Key()
//{
//    return m_ContractDetails_Grouped[PortfolioType::BFLY_BID];
//}

//QStringList ContractDetail::Get_BOX_BID_data_list_Sorted_Key()
//{
//    return m_ContractDetails_Grouped[PortfolioType::BX_BID];
//}

QStringList ContractDetail::Get_Tokens_For_PortfolioType(PortfolioType type)
{
    if (m_ContractDetails_Grouped.contains(type)) {
        return m_ContractDetails_Grouped[type];
    }
    else {
       qDebug()<<"PortfolioType: " <<type<< " not exist in m_ContractDetails_Grouped, Debug and fix.";
       return QStringList(); // or handle it as needed
   }

}

QStringList ContractDetail::Get_Tokens_For_IntrumentType(InstrumentType type)
{
    if (m_ContractDetails_Grouped.contains(type)) {
        return m_ContractDetails_Grouped[type];
    }
    else {
       qDebug()<<"PortfolioType: " <<type<< " not exist in m_ContractDetails_Grouped, Debug and fix.";
       return QStringList(); // or handle it as needed
   }

}


/*Hash<QString, contract_table> ContractDetail::GetFutureContracts()
{
    if (m_ContractDetails.empty())
    {
        ReloadContractDetails(userData);
    }

    QHash<QString, contract_table> futureContracts;
    QStringList futToks = m_ContractDetailsFiltered["FUT"];
    for(int i=0;i<futToks.length();i++){
        futureContracts.insert(futToks[i],m_ContractDetails[PortfolioType::F2F][futToks[i]]);
    }

    //  std::sort(futureContracts.begin(), futureContracts.end(),
    //  [](const ContractDetail& a, const ContractDetail& b) { return a.InstrumentName < b.InstrumentName; });

    return futureContracts;
}*/

QHash<QString, contract_table> ContractDetail::GetContracts(QString type)
{
    if ( m_ContractDetails_Hash.empty())
    {
        ReloadContractDetails(userData);
    }

    QHash<QString, contract_table> Contracts;

    //m_ContractDetailsFiltered will be like OPTIDX OPTSTK FUTIDX FUTSTK
    if(type=="FUT"){
        if(m_ContractDetailsFiltered.contains("FUTIDX")){
            QStringList Toks = m_ContractDetailsFiltered["FUTIDX"];
            for(int i=0;i<Toks.length();i++){
                Contracts.insert(Toks[i],m_ContractDetails_Hash[Toks[i]]);
            }
        }
        if(m_ContractDetailsFiltered.contains("FUTSTK")){
            QStringList Toks = m_ContractDetailsFiltered["FUTSTK"];
            for(int i=0;i<Toks.length();i++){
                Contracts.insert(Toks[i],m_ContractDetails_Hash[Toks[i]]);
            }
        }
    }
    else if(m_ContractDetailsFiltered.contains(type)){
        QStringList Toks = m_ContractDetailsFiltered[type];
        for(int i=0;i<Toks.length();i++){
            Contracts.insert(Toks[i],m_ContractDetails_Hash[Toks[i]]);
        }
    }


    return Contracts;

}

contract_table ContractDetail::GetDetail(int token)
{
    if (m_ContractDetails_Hash.empty())
    {
        ReloadContractDetails(userData);
    }

    QString key = QString::number(token);
    if(m_ContractDetails_Hash.contains(key))
       return m_ContractDetails_Hash[key];
    return contract_table();

}

bool ContractDetail::checkTokenExist(int token){
    QString key = QString::number(token);
    if(m_ContractDetails_Hash.contains(key))
       return true;
    else
      return false;
}

int ContractDetail::GetLotSize(int token,int type)
{
    if (token == 0){
        //qDebug()<<"Warning: GetLotSize--> token cannot be zero.";
        return 0;
    }

    QString key = QString::number(token);
    if(m_ContractDetails_Hash.contains(key)){
        return m_ContractDetails_Hash[key].LotSize;
    }

    else{
        qDebug()<<"Warning: GetLotSize--> token not exist in contract for token=."<<token;
        return 0;
    }
}

QString ContractDetail::GetStockName(int token,int type)
{
    if (token == 0)
        return "-";
    QString key = QString::number(token);


    if(m_ContractDetails_Hash.contains(key)){
        return m_ContractDetails_Hash[key].StockName;}
    else
        return "-";
}

QString ContractDetail::GetInstrumentName(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);


    if(m_ContractDetails_Hash.contains(key)){
        QRegularExpression regex("-");
        m_ContractDetails_Hash[key].InstrumentName.replace(regex, "");
        return m_ContractDetails_Hash[key].InstrumentName;}
    else
        return "-";

}

QString ContractDetail::GetInstrumentType(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    if(m_ContractDetails_Hash.contains(key)){
        QRegularExpression regex("-");
        m_ContractDetails_Hash[key].InstrumentType.replace(regex, "");
        return m_ContractDetails_Hash[key].InstrumentType;}
    else
        return "-";

}

int ContractDetail::GetStrikePriceOrg(int token, int type)
{
    if (token == 0)
        return 0;
    QString key = QString::number(token);

    if(m_ContractDetails_Hash.contains(key))
        return m_ContractDetails_Hash[key].StrikePrice;
    else
        return 0;

}

double ContractDetail::GetVolumeFreezeQty(int token, int type)
{
    if (token == 0)
        return 0;
    QString key = QString::number(token);

    if(m_ContractDetails_Hash.contains(key))
        return m_ContractDetails_Hash[key].VolumeFreezeQty;
    else
        return 0;

}

QString ContractDetail::GetStrikePrice(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);


    if(m_ContractDetails_Hash.contains(key)){
        double sp = m_ContractDetails_Hash[key].StrikePrice * 1.0 / devicer_contract;

        std::ostringstream formattedNumber;

        if (sp - static_cast<int>(sp) != 0) {
            // If there are decimal places, store the number with decimal values
            formattedNumber << std::fixed << std::setprecision(2) << sp;
        } else {
            // If there are no decimal places or only zeros, store the number without decimal values
            formattedNumber << std::fixed << std::setprecision(0) << sp;
        }

        std::string result = formattedNumber.str(); // Get the formatted number as a string

        //            return QString::number(sp,'f',decimal_precision_contract);
        return QString::fromStdString(result);
    }
    else
        return "-";

}
QString ContractDetail::GetOptionType(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);


    if(m_ContractDetails_Hash.contains(key)){
        return m_ContractDetails_Hash[key].OptionType;
    }
    else
        return "-";

}
QString ContractDetail::GetOptionType(QString token)
{
    if (token == "0")
        return "-";
    if(m_ContractDetails_Hash.contains(token)){
        return m_ContractDetails_Hash[token].OptionType;
    }
    else
        return "-";

}


QString ContractDetail::GetExpiry(int token, QString format, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);



    if(m_ContractDetails_Hash.contains(key)){
        QString dateTimeStr = GetExpiry(format,m_ContractDetails_Hash[key].Expiry);
        return dateTimeStr;
    }
    else
        return "-";
}

QString ContractDetail::GetExpiry(QString format,long long Expiry) const
{
    QDateTime dt;
    dt.setOffsetFromUtc(0);
    dt.setDate(QDate(1980, 1, 1));
    dt.setTime(QTime(0, 0, 0));
    QString dateTimeStr = dt.addSecs(Expiry).toString(format);
    return dateTimeStr;
    //.
}

QDateTime ContractDetail::GetExpiryDate(int token,int type)
{
    QString key = QString::number(token);

    if(m_ContractDetails_Hash.contains(key)){
        QDateTime dt;
        dt.setOffsetFromUtc(0);
        dt.setDate(QDate(1980, 1, 1));
        dt.setTime(QTime(0, 0, 0));

        return dt.addSecs(m_ContractDetails_Hash[key].Expiry);
    }
    else
        return QDateTime();
}

qint64 ContractDetail::GetExpiry(int token,int type)
{
    if (token == 0)
        return 0;

    QString key = QString::number(token);



    if(m_ContractDetails_Hash.contains(key)){
        return m_ContractDetails_Hash[key].Expiry;
    }
    else
        return 0;
}


/* QString ContractDetail::toString() const
     {
         return InstrumentName;
     }*/



//    QStandardItemModel* Get_model_searchInstrument_BOX_Leg1()
//    {

//       return model_searchInstrument_BOX_Leg1;
//    }
//    QString GetExpiryMonth(int token, QString format)
//    {
//        if (token == 0)
//            return "-";

//        QString key = QString::number(token);
//        if(m_ContractDetails.contains(key)){
//            QString dateTimeStr = GetExpiry("MMM",m_ContractDetails[key].Expiry);
//            return dateTimeStr;
//        }
//        else
//            return "-";
//    }

#ifdef STORE_CONTRACT_LOCALLY

 PortfolioType ContractDetail::checkAlogTypeForTheData(QString instrument_type){
     if(instrument_type=="OPTSTK"||instrument_type=="OPTIDX"||instrument_type=="OPTCUR"||instrument_type=="OPTIRC")
         return PortfolioType::BY;
     if(instrument_type=="FUTIDX"||instrument_type=="FUTSTK"||instrument_type=="FUTCUR"||instrument_type=="FUTIRC"||instrument_type=="FUTIRT")
         return PortfolioType::F2F;


     return PortfolioType::INVALID;

 }


 bool ContractDetail::LoadContractLocal(QHash<QString, QStringList> &_m_ContractDetailsFiltered )

 {
     try
     {
         QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
         QString directory = appDataPath + "/Data";

         QString fileName = directory + "/contracts.bin";

        // QString fileName = directory + "/contracts_"+QString::number(userData.UserId)+".bin";

         if (QFile::exists(fileName))
         {
             QFileInfo fileInfo(fileName);
             QDateTime lastModified = fileInfo.lastModified();
             QDateTime currentTime = QDateTime::currentDateTime();
             if (currentTime.date() != lastModified.date())
             {
                 if (!QFile::remove(fileName))
                 {
                     return false;
                 }
             }

             QFile file(fileName);
             if (file.open(QIODevice::ReadOnly))
             {
                 m_ContractDetails_Hash.clear();

                 long long actualMin = LLONG_MAX ;
                 QDataStream in(&file);

                while (!in.atEnd()) {
                     QString PortfolioType;

                     contract_table contractT;
                     in >> PortfolioType>>
                             contractT.TokenNumber >>
                             contractT.InstrumentType >>
                             contractT.InstrumentName >>
                             contractT.OptionType >>
                             contractT.StrikePrice >>
                             contractT.LotSize >>
                             contractT.Expiry >>
                             contractT.StockName >>
                             contractT.MinimumSpread >>
                             contractT.VolumeFreezeQty>>
                             contractT.OperatingRangeslowPriceRange>>
                             contractT.OperatingRangeshighPriceRange;




                     if(contractT.Expiry>0){
                         if(actualMin>contractT.Expiry)
                             actualMin = contractT.Expiry;
                     }
                     _m_ContractDetailsFiltered[contractT.InstrumentType].append(QString::number(contractT.TokenNumber));
                     m_ContractDetails_Hash.insert( QString::number(contractT.TokenNumber), contractT);

                     QStringList PortfolioTypeList =  PortfolioType.split(",");
                     for(int i=0;i<PortfolioTypeList.size();i++){
                         int algo_type = PortfolioTypeList[i].toInt();
                         m_ContractDetails_Grouped[algo_type].append(QString::number(contractT.TokenNumber));
                     }



                 }

                 qDebug()<<"Loaded ContractDetails from "<<fileName<<"  size="<<m_ContractDetails_Hash.size();



                 try
                 {
                     mysql_conn con(0,"contract_conn");
                     QSqlQuery expiryQuery = con.runQuery("SELECT min(Contract.ExpiryDate) as MinExpiry FROM Contract where ExpiryDate > 0;");
                     //QSqlQuery expiryQuery("SELECT min(Contract.ExpiryDate) as MinExpiry FROM Contract where ExpiryDate > 0;", database);

                     if (expiryQuery.next())
                     {
                         long long minExpiry = expiryQuery.value("MinExpiry").toLongLong();

                         if (minExpiry != actualMin)
                         {
                             m_ContractDetails_Hash.clear();
                             return false;
                         }
                     }
                 }
                 catch (const std::exception& e)
                 {
                     qDebug() << "OnLoadData : " << e.what();
                 }


                 return true;
             }
         }
     }
     catch (const std::exception& e)
     {
         qDebug() << "LoadContractLocal : " << e.what();
     }

     return false;
 }


 QHash<QString,int> ContractDetail::get_SettPrice(){
     if (m_SettPrice.isEmpty()) {
         mysql_conn con(0,"SettPrice_conn");
         m_SettPrice = con.get_SettPrice();
      }

     return m_SettPrice;
 }

/*
 void ContractDetail::StoreContractToLocalFile() {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
     QString directory = appDataPath + "/Data";
     QDir().mkpath(directory);

     QString filename = directory + "/contracts.bin";
     QFile file(filename);
     if (!file.open(QIODevice::WriteOnly)) {
         qDebug() << "Failed to open file: " << filename;
         return;
     }

    // int size =  m_ContractDetails.size();
     QDataStream out(&file);

     for (auto it = ContractDetail::m_ContractDetails_Hash.cbegin();
          it != ContractDetail::m_ContractDetails_Hash.cend(); ++it)
     {
         QString tokenNo = it.key();
         QString PortfolioType = "";// need to fill all the portfoilo type for the isntrument type, comma seperated.
         contract_table contract = it.value();
         out << PortfolioType
             << tokenNo
             << contract.InstrumentType
             << contract.InstrumentName
             << contract.OptionType
             << contract.StrikePrice
             << contract.LotSize
             << contract.Expiry
             << contract.TokenNumber
             << contract.StockName
             << contract.MinimumSpread
             << contract.VolumeFreezeQty;

         // Do something with key and value (contract_table)
     }




 }*/
#endif

// bool LoadFromFTP(const std::string& ftpLocation)
// {
//     if (QFile::exists(QString::fromStdString(ftpLocation)))
//     {
//         std::ifstream file(ftpLocation);
//         if (!file.is_open())
//         {
//             return false;
//         }

//         std::string line;
//         std::getline(file, line);

//         if (line.empty() || line.find("NEATFO") == std::string::npos)
//         {
//             return false;
//         }

//         m_ContractDetails.clear();

//         while (std::getline(file, line))
//         {
//             if (line.empty())
//                 continue;

//             std::istringstream iss(line);
//             std::string token, instrumentType, instrumentName, expiryStr, strikePriceStr, optionType, lotSizeStr, minimumSpreadStr, stockName;
//             if (!(std::getline(iss, token, '|') &&
//                   std::getline(iss, instrumentType, ',') &&
//                   std::getline(iss, instrumentName, ',') &&
//                   std::getline(iss, expiryStr, ',') &&
//                   std::getline(iss, strikePriceStr, ',') &&
//                   std::getline(iss, optionType, ',') &&
//                   std::getline(iss, lotSizeStr, ',') &&
//                   std::getline(iss, minimumSpreadStr, ',') &&
//                   std::getline(iss, stockName)))
//             {
//                 continue;
//             }

//             int tokenNum = std::stoi(token);
//             long long expiry = std::stoll(expiryStr);
//             int strikePrice = std::stoi(strikePriceStr);
//             int lotSize = std::stoi(lotSizeStr);
//             int minimumSpread = std::stoi(minimumSpreadStr);


//             contract_table contractTableTmp;
//             contractTableTmp.InstrumentType = QString::fromStdString(instrumentType);
//             contractTableTmp.InstrumentName = QString::fromStdString(instrumentName);
//             contractTableTmp.OptionType = QString::fromStdString(optionType);
//             contractTableTmp.StrikePrice = (strikePrice);
//             contractTableTmp.LotSize = (lotSize);
//             contractTableTmp.Expiry = (expiry);
//             contractTableTmp.TokenNumber = (tokenNum);
//             contractTableTmp.StockName = QString::fromStdString(stockName);
//             contractTableTmp.MinimumSpread = (minimumSpread);

//             m_ContractDetails.insert(contractTableTmp.StockName, contractTableTmp);

//         }

//         return true;
//     }

//     return false;
// }

