#include"ContractDetail.h"
#include "mysql_conn.h"
#include <iostream>
#include <string>
#include <sstream>

ContractDetail* ContractDetail::instance = nullptr;
QMutex ContractDetail::mutex;
QMap<int , QHash<QString, contract_table>> ContractDetail::m_ContractDetails;
QHash<QString, QStringList> ContractDetail::m_ContractDetailsFiltered;
QStringList ContractDetail::F2F_data_list_Sorted_Key;
QStringList ContractDetail::BFLY_data_list_Sorted_Key;
QStringList ContractDetail::BFLY_BID_data_list_Sorted_Key;
//QStandardItemModel* ContractDetail::model_searchInstrument_BOX_Leg1  = nullptr;
QStandardItemModel* ContractDetail::model_start_strike_BFLY = nullptr;
QStandardItemModel* ContractDetail::model_searchInstrument_F2F_Leg1 = nullptr;
QStandardItemModel* ContractDetail::model_FUT_CON_REV = nullptr;
QStandardItemModel* ContractDetail::model_start_strike_BFLY_BID = nullptr;
double ContractDetail::devicer_contract;
double ContractDetail::decimal_precision_contract;
bool ContractDetail::settingLoaded = false;


ContractDetail::ContractDetail() noexcept
{
    //        model_searchInstrument_BOX_Leg1 = new QStandardItemModel;
    model_start_strike_BFLY = new QStandardItemModel;
    model_searchInstrument_F2F_Leg1 = new QStandardItemModel;
    model_FUT_CON_REV = new QStandardItemModel;
    model_start_strike_BFLY_BID = new QStandardItemModel;
}

ContractDetail &ContractDetail::getInstance()
{
    if(!settingLoaded){
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
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
        }

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

    //check data stored in Local file and load from there
    // if (!LoadData(m_ContractDetailsFiltered))
    // {
    /*if (!LoadFromFTP(ContractFilePathFTP))
            {
                MySQL_Conn con(0,"contract_conn");
                m_ContractDetails = con.getContractTable(m_ContractDetailsFiltered);
            }*/

    userData = uData;
    mysql_conn con(0,"contract_conn");
    m_ContractDetails = con.getContractTable(m_ContractDetailsFiltered,
                                             F2F_data_list_Sorted_Key,
                                             BFLY_data_list_Sorted_Key,
                                             BFLY_BID_data_list_Sorted_Key, uData);
    //qDebug()<<"Loaded ContractDetails from DB size="<<m_ContractDetails.size();
    //StoreData();
    //}

    create_inputFiledAutoFillModel_For_AddAlgoWindow();

    // std::sort(m_ContractDetails.begin(), m_ContractDetails.end(), [](const ContractDetail& a, const ContractDetail& b) { return a.InstrumentName < b.InstrumentName; });
}

QMap<int, QHash<QString, contract_table>> ContractDetail::GetContracts()
{
    /*  if (m_ContractDetails.empty())
        {
            ReloadContractDetails();
        }*/

    return m_ContractDetails;
}

//This function create autfill model for first input field in add algo window(frist leg)
void ContractDetail::create_inputFiledAutoFillModel_For_AddAlgoWindow()
{
    //        model_searchInstrument_BOX_Leg1->clear();
    model_start_strike_BFLY->clear();
    model_searchInstrument_F2F_Leg1->clear();
    model_FUT_CON_REV->clear();
    model_start_strike_BFLY_BID->clear();

    QSet<QString> model_data_name_set;
    for(int i=0;i<BFLY_data_list_Sorted_Key.length();i++){

        /**********Create model for Mainwindow searchbar*************************/
        const auto& contract = m_ContractDetails[PortfolioType::BY][BFLY_data_list_Sorted_Key[i]];
        QString stock_name=contract.StockName;

        QString name = stock_name;
        if(!model_data_name_set.contains(name)){
            model_data_name_set.insert(name);
            QStandardItem *itemBOXL1 = new QStandardItem;
            itemBOXL1->setText(name);
            itemBOXL1->setData(contract.StockName, Qt::UserRole + 1);
            //                model_searchInstrument_BOX_Leg1->appendRow(itemBOXL1);
        }
        /********************************************************************/
    }

    for(int i=0;i<BFLY_data_list_Sorted_Key.length();i++){

        /**********Create model for BOX*************************/
        const auto& contract = m_ContractDetails[PortfolioType::BY][BFLY_data_list_Sorted_Key[i]];
        unsigned int unix_time= contract.Expiry;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString instrument_name = contract.InstrumentName;
        QString strik_price = QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);

        QString name = instrument_name+" "+Expiry+" "+strik_price;
        if(!model_data_name_set.contains(name)){
            model_data_name_set.insert(name);
            QStandardItem *itemBOXL1 = new QStandardItem;
            itemBOXL1->setText(name);
            itemBOXL1->setData(contract.TokenNumber, Qt::UserRole + 1);
            //                model_searchInstrument_BOX_Leg1->appendRow(itemBOXL1);
        }
        /********************************************************************/

        /**********Create model for BFLY*************************/

        unix_time= contract.Expiry;
        dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
        QStandardItem *itemBFLY = new QStandardItem;
        itemBFLY->setText(algo_combination);
        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
        model_start_strike_BFLY->appendRow(itemBFLY);

        /********************************************************************/

    }
    //doubt
    QSet<QString> model_data_name_set_bfly_bid;
    for(int i=0;i<BFLY_BID_data_list_Sorted_Key.length();i++){

        /**********Create model for Mainwindow searchbar*************************/
        const auto& contract = m_ContractDetails[PortfolioType::BFLY_BID][BFLY_BID_data_list_Sorted_Key[i]];
        QString stock_name=contract.StockName;

        QString name = stock_name;
        if(!model_data_name_set_bfly_bid.contains(name)){
            model_data_name_set_bfly_bid.insert(name);
            QStandardItem *itemBOXL1 = new QStandardItem;
            itemBOXL1->setText(name);
            itemBOXL1->setData(contract.StockName, Qt::UserRole + 1);
            //                model_searchInstrument_BOX_Leg1->appendRow(itemBOXL1);
        }
        /********************************************************************/
    }

    for(int i=0;i<BFLY_BID_data_list_Sorted_Key.length();i++){

        /**********Create model for BOX*************************/
        const auto& contract = m_ContractDetails[PortfolioType::BFLY_BID][BFLY_BID_data_list_Sorted_Key[i]];
        unsigned int unix_time= contract.Expiry;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString instrument_name = contract.InstrumentName;
        QString strik_price = QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);

        QString name = instrument_name+" "+Expiry+" "+strik_price;
        if(!model_data_name_set_bfly_bid.contains(name)){
            model_data_name_set_bfly_bid.insert(name);
            QStandardItem *itemBOXL1 = new QStandardItem;
            itemBOXL1->setText(name);
            itemBOXL1->setData(contract.TokenNumber, Qt::UserRole + 1);
            //                model_searchInstrument_BOX_Leg1->appendRow(itemBOXL1);
        }
        /********************************************************************/

        /**********Create model for BFLY*************************/

        unix_time= contract.Expiry;
        dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
        QStandardItem *itemBFLY = new QStandardItem;
        itemBFLY->setText(algo_combination);
        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);


        model_start_strike_BFLY_BID->appendRow(itemBFLY);

        /********************************************************************/

    }
    //doubt

    for(int i=0;i<F2F_data_list_Sorted_Key.length();i++){

        /**********Create model for _F2F*************************/
        const auto& contract = m_ContractDetails[PortfolioType::F2F][F2F_data_list_Sorted_Key[i]];

        unsigned int unix_time= contract.Expiry;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString instrument_name = contract.InstrumentName;


        QStandardItem *itemF2FL1 = new QStandardItem;
        itemF2FL1->setText(instrument_name+" "+Expiry);
        itemF2FL1->setData(contract.TokenNumber, Qt::UserRole + 1);
        model_searchInstrument_F2F_Leg1->appendRow(itemF2FL1);
        /********************************************************************/


        /**********Create model for CON_REV*************************/

        instrument_name = contract.InstrumentName;//" "+Expiry;
        unix_time= contract.Expiry;
        dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        Expiry=dt.toString("MMM dd yyyy").toUpper();

        QStandardItem *itemFut = new QStandardItem;
        itemFut->setText(instrument_name+" "+Expiry);
        itemFut->setData(contract.TokenNumber, Qt::UserRole + 1);
        model_FUT_CON_REV->appendRow(itemFut);

        /********************************************************************/


    }


}

QStandardItemModel* ContractDetail::Get_model_searchInstrument_F2F_Leg1()
{
    return model_searchInstrument_F2F_Leg1;
}


QStandardItemModel* ContractDetail::Get_model_FUT_CON_REV()
{
    return model_FUT_CON_REV;
}


QStandardItemModel* ContractDetail::Get_model_start_strike_BFLY()
{
    return model_start_strike_BFLY;
}

QStandardItemModel* ContractDetail::Get_model_start_strike_BFLY_BID()
{
    return model_start_strike_BFLY_BID;
}
QStringList ContractDetail::Get_F2F_data_list_Sorted_Key()
{
    return F2F_data_list_Sorted_Key;
}

QStringList ContractDetail::Get_BFLY_data_list_Sorted_Key()
{
    return BFLY_data_list_Sorted_Key;
}

QStringList ContractDetail::Get_BFLY_BID_data_list_Sorted_Key()
{
    return BFLY_BID_data_list_Sorted_Key;
}

QHash<QString, contract_table> ContractDetail::GetFutureContracts()
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
}

QHash<QString, contract_table> ContractDetail::GetContracts(QString type)
{
    if ( m_ContractDetails.empty())
    {
        ReloadContractDetails(userData);
    }

    QHash<QString, contract_table> Contracts;
    QStringList Toks = m_ContractDetailsFiltered[type];
    for(int i=0;i<Toks.length();i++){
        Contracts.insert(Toks[i],m_ContractDetails[type.toInt()][Toks[i]]);
    }

    return Contracts;

}

contract_table ContractDetail::GetDetail(int token, int type)
{
    if (m_ContractDetails.empty())
    {
        ReloadContractDetails(userData);
    }

    contract_table tmp;
    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key))
        return m_ContractDetails[type][key];

    return contract_table();
}

int ContractDetail::GetLotSize(int token,int type)
{
    if (token == 0)
        return 0;

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key))
        return m_ContractDetails[type][key].LotSize;

    else
        return 0;
}

QString ContractDetail::GetStockName(int token,int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key)){
        return m_ContractDetails[type][key].StockName;}
    else
        return "-";
}

QString ContractDetail::GetInstrumentName(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key)){
        QRegularExpression regex("-");
        m_ContractDetails[type][key].InstrumentName.replace(regex, "");
        return m_ContractDetails[type][key].InstrumentName;}
    else
        return "-";

}

int ContractDetail::GetStrikePriceOrg(int token, int type)
{
    if (token == 0)
        return 0;
    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key))
        return m_ContractDetails[type][key].StrikePrice;
    else
        return 0;

}

QString ContractDetail::GetStrikePrice(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key)){
        double sp = m_ContractDetails[type][key].StrikePrice * 1.0 / devicer_contract;

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
    if(m_ContractDetails[type].contains(key)){
        return m_ContractDetails[type][key].OptionType;
    }
    else
        return "-";

}

/* QString GetStrikePrice(int StrikePrice)
     {
         double sp = StrikePrice * 1.0 / 100;

         return QString::number(sp);
     }*/

QString ContractDetail::GetExpiry(int token, QString format, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key)){
        QString dateTimeStr = GetExpiry(format,m_ContractDetails[type][key].Expiry);
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
    if(m_ContractDetails[type].contains(key)){
        QDateTime dt;
        dt.setOffsetFromUtc(0);
        dt.setDate(QDate(1980, 1, 1));
        dt.setTime(QTime(0, 0, 0));

        return dt.addSecs(m_ContractDetails[type][key].Expiry);
    }
    else
        return QDateTime();
}

qint64 ContractDetail::GetExpiry(int token,int type)
{
    if (token == 0)
        return 0;

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key)){
        return m_ContractDetails[type][key].Expiry;
    }
    else
        return 0;
}
// bool LoadData(QHash<QString, QStringList> &_m_ContractDetailsFiltered)
// {
//     try
//     {
//         QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//         QString directory = appDataPath + "/contracts";

//         QDir().mkpath(directory);

//         QString fileName = directory + "/contracts.bin";

//         if (QFile::exists(fileName))
//         {
//             QFileInfo fileInfo(fileName);
//             QDateTime lastModified = fileInfo.lastModified();
//             QDateTime currentDateTime = QDateTime::currentDateTime();
//             if (currentDateTime.date() != lastModified.date())
//             {
//                 if (!QFile::remove(fileName))
//                 {
//                     return false;
//                 }
//             }

//             QFile file(fileName);
//             if (file.open(QIODevice::ReadOnly))
//             {
//                 m_ContractDetails.clear();
//                 F2F_data_list_Sorted_Key.clear();
//                 BFLY_data_list_Sorted_Key.clear();
//                 BFLY_BID_data_list_Sorted_Key.clear();
//                 QList <contract_table> BFLY_data_list;
//                 QList <contract_table> F2F_data_list;
//                 QList <contract_table> BFLY_BID_data_list;

//                 long long actualMin = LLONG_MAX ;

//                 int size;
//                 QDataStream in(&file);
//                 in >> size;



//                 for (int i = 0; i < size; i++) {
//                     QString key;
//                     contract_table value;
//                     in >> key >> value.InstrumentType >> value.InstrumentName >> value.OptionType >> value.StrikePrice >> value.LotSize >> value.Expiry >> value.TokenNumber >> value.StockName >> value.MinimumSpread;

//                     m_ContractDetails[key] = value;
//                     if(value.Expiry>0){
//                         if(actualMin>value.Expiry)
//                             actualMin = value.Expiry;
//                     }
//                     _m_ContractDetailsFiltered[value.InstrumentType].append(QString::number(value.TokenNumber));

//                     PortfolioType algo_type = checkAlogTypeForTheData(value.InstrumentType);
//                     // qDebug() << "---test--- algo type"<<algo_type << value.InstrumentType;
//                     switch (algo_type) {
//                     case PortfolioType::BY:{
//                         BFLY_data_list.append(value);
//                         break;
//                     }
//                     case PortfolioType::F2F:{
//                         F2F_data_list.append(value);
//                         break;
//                     }
//                     case PortfolioType::BFLY_BID:{
//                         BFLY_BID_data_list.append(value);
//                         break;
//                     }

//                     default:
//                         break;
//                     }


//                 }
//                 //sort F2F_data_list based on date
//                 // std::sort(F2F_data_list.begin(), F2F_data_list.end(), [](const contract_table& a, const contract_table& b)->bool{
//                 //     return  a.Expiry < b.Expiry; // sort data based  expiry_date
//                 // });
//                 // //sort BFLY_data_list based on strike_price
//                 // std::sort(BFLY_data_list.begin(), BFLY_data_list.end(), [](const contract_table& a, const contract_table& b)->bool{
//                 //     return  a.StrikePrice < b.StrikePrice; // sort data based  strike price
//                 // });

//                 //sort BFLY_BID_data_list based on strike_price
//                 // std::sort(BFLY_BID_data_list.begin(), BFLY_BID_data_list.end(), [](const contract_table& a, const contract_table& b)->bool{
//                 //     return  a.StrikePrice < b.StrikePrice; // sort data based  strike price
//                 // });

//                 for(int i=0;i<F2F_data_list.length();i++){
//                     F2F_data_list_Sorted_Key.append(QString::number(F2F_data_list[i].TokenNumber));
//                 }
//                 for(int i=0;i<BFLY_data_list.length();i++){
//                     BFLY_data_list_Sorted_Key.append(QString::number(BFLY_data_list[i].TokenNumber));
//                 }
//                 for(int i=0;i<BFLY_BID_data_list.length();i++){
//                     BFLY_BID_data_list_Sorted_Key.append(QString::number(BFLY_BID_data_list[i].TokenNumber));
//                 }

//                 qDebug()<<"Loaded ContractDetails from "<<fileName<<"  size="<<m_ContractDetails.size();

//                 if(m_ContractDetails.size()==0)
//                     return false;



//                 try
//                 {
//                     mysql_conn con(0,"contract_conn");
//                     QSqlQuery expiryQuery = con.runQuery("SELECT min(Contract.ExpiryDate) as MinExpiry FROM Contract where ExpiryDate > 0;");
//                     //QSqlQuery expiryQuery("SELECT min(Contract.ExpiryDate) as MinExpiry FROM Contract where ExpiryDate > 0;", database);

//                     if (expiryQuery.next())
//                     {
//                         long long minExpiry = expiryQuery.value("MinExpiry").toLongLong();

//                         if (minExpiry != actualMin)
//                         {
//                             m_ContractDetails.clear();
//                             return false;
//                         }
//                     }
//                 }
//                 catch (const std::exception& e)
//                 {
//                     qDebug() << "OnLoadData : " << e.what();
//                 }


//                 return true;
//             }
//         }
//     }
//     catch (const std::exception& e)
//     {
//         qDebug() << "OnLoadData2 : " << e.what();
//     }

//     return false;
// }



/* QString ContractDetail::toString() const
     {
         return InstrumentName;
     }*/



//    QStandardItemModel* Get_model_searchInstrument_BOX_Leg1()
//    {

////        return model_searchInstrument_BOX_Leg1;
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


// void StoreData() {
//     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//     QString directory = appDataPath + "/contracts";
//     QDir().mkpath(directory);

//     QString filename = directory + "/contracts.bin";
//     QFile file(filename);
//     if (!file.open(QIODevice::WriteOnly)) {
//         qDebug() << "Failed to open file: " << filename;
//         return;
//     }

//     int size =  m_ContractDetails.size();
//     QDataStream out(&file);
//     out << size;

//     QHashIterator<QString, contract_table> it(m_ContractDetails);
//     while (it.hasNext()) {
//         it.next();
//         out << it.key() << it.value().InstrumentType << it.value().InstrumentName << it.value().OptionType << it.value().StrikePrice << it.value().LotSize << it.value().Expiry << it.value().TokenNumber << it.value().StockName << it.value().MinimumSpread;
//     }

// }


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

