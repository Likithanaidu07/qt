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
QStandardItemModel* ContractDetail::model_F1_F2 = nullptr;

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
    model_F1_F2 = new QStandardItemModel;
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

#ifdef STORE_CONTRACT_LOCALLY
    //check data stored in Local file and load from there
     if (!LoadContractLocal(m_ContractDetailsFiltered,
                           F2F_data_list_Sorted_Key,
                           BFLY_data_list_Sorted_Key,
                           BFLY_BID_data_list_Sorted_Key))
     {
#endif

    userData = uData;
    mysql_conn con(0,"contract_conn");
    m_ContractDetails = con.getContractTable(m_ContractDetailsFiltered,
                                             F2F_data_list_Sorted_Key,
                                             BFLY_data_list_Sorted_Key,
                                             BFLY_BID_data_list_Sorted_Key, uData);
    qDebug()<<"Loaded ContractDetails from DB size="<<m_ContractDetails.size();

#ifdef STORE_CONTRACT_LOCALLY
        StoreContractToLocalFile();
    }
#endif

    create_inputFiledAutoFillModel_For_AddAlgoWindow();
    //std::sort(m_ContractDetails.begin(), m_ContractDetails.end(), [](const ContractDetail& a, const ContractDetail& b) { return a.InstrumentName < b.InstrumentName; });
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
    model_F1_F2->clear();

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

        /**********Create model for BFLY *************************/

        unix_time= contract.Expiry;
        dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
        QStandardItem *itemBFLY = new QStandardItem;
        itemBFLY->setText(algo_combination);
        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd") + "-" + QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract);
        // Set the composite key as data for sorting
        itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
        model_start_strike_BFLY->appendRow(itemBFLY);

        /********************************************************************/

        /**********Create model for F1_F2*************************/

        QStandardItem *itemF1_F2 = new QStandardItem;
        itemF1_F2->setText(algo_combination);
        itemF1_F2->setData(contract.TokenNumber, Qt::UserRole + 1);
        // Set the composite key as data for sorting
        itemF1_F2->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
        model_F1_F2->appendRow(itemF1_F2);

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

        /**********Create model for BFLY BID*************************/

        unix_time= contract.Expiry;
        dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
        QStandardItem *itemBFLY = new QStandardItem;
        itemBFLY->setText(algo_combination);
        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);

        // Create custom data for sorting
        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
        // Set the composite key as data for sorting
        itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
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
        QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd");
        // Set the composite key as data for sorting
        itemF2FL1->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
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
        compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd");
        // Set the composite key as data for sorting
        itemFut->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
        model_FUT_CON_REV->appendRow(itemFut);

        /********************************************************************/


        /**********Create model for F1_F2*************************/
        QStandardItem *itemF1_F2 = new QStandardItem;
        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer_contract,'f',decimal_precision_contract)+" "+contract.OptionType;
        itemF1_F2->setText(algo_combination);
        itemF1_F2->setData(contract.TokenNumber, Qt::UserRole + 1);
        // Set the composite key as data for sorting
        itemF1_F2->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
        model_F1_F2->appendRow(itemF1_F2);
        /*********************************************************/


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

QStandardItemModel* ContractDetail::Get_model_F1_F2()
{
    return model_F1_F2;
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

    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        contract_table tmp;
        QString key = QString::number(token);
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key];
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key];

        return contract_table();
    }

    else{
        contract_table tmp;
        QString key = QString::number(token);
        if(m_ContractDetails[type].contains(key))
            return m_ContractDetails[type][key];
        return contract_table();
    }


}



int ContractDetail::GetLotSize(int token,int type)
{
    if (token == 0)
        return 0;

    QString key = QString::number(token);

    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].LotSize;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].LotSize;

         return 0;
    }


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

    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].StockName;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].StockName;
         return "-";
    }

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
    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].InstrumentName;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].InstrumentName;
         return "-";
    }

    if(m_ContractDetails[type].contains(key)){
        QRegularExpression regex("-");
        m_ContractDetails[type][key].InstrumentName.replace(regex, "");
        return m_ContractDetails[type][key].InstrumentName;}
    else
        return "-";

}

QString ContractDetail::GetInstrumentType(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    if(m_ContractDetails[type].contains(key)){
        QRegularExpression regex("-");
        m_ContractDetails[type][key].InstrumentType.replace(regex, "");
        return m_ContractDetails[type][key].InstrumentType;}
    else
        return "-";

}

int ContractDetail::GetStrikePriceOrg(int token, int type)
{
    if (token == 0)
        return 0;
    QString key = QString::number(token);
    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].StrikePrice;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].StrikePrice;
         return 0;
    }

    if(m_ContractDetails[type].contains(key))
        return m_ContractDetails[type][key].StrikePrice;
    else
        return 0;

}

double ContractDetail::GetVolumeFreezeQty(int token, int type)
{
    if (token == 0)
        return 0;
    QString key = QString::number(token);
    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].VolumeFreezeQty;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].VolumeFreezeQty;
         return 0;
    }

    if(m_ContractDetails[type].contains(key))
        return m_ContractDetails[type][key].VolumeFreezeQty;
    else
        return 0;

}

QString ContractDetail::GetStrikePrice(int token, int type)
{
    if (token == 0)
        return "-";

    QString key = QString::number(token);
    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
       if(m_ContractDetails[PortfolioType::BY].contains(key)){
             double sp = m_ContractDetails[PortfolioType::BY][key].StrikePrice * 1.0 / devicer_contract;

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
         else if(m_ContractDetails[PortfolioType::F2F].contains(key)){
             double sp = m_ContractDetails[PortfolioType::F2F][key].StrikePrice * 1.0 / devicer_contract;

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

    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].OptionType;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].OptionType;
       return "-";
    }

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


    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return GetExpiry(format,m_ContractDetails[type][key].Expiry);
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return GetExpiry(format,m_ContractDetails[type][key].Expiry);
       return "-";
    }

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

    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key)){
            QDateTime dt;
            dt.setOffsetFromUtc(0);
            dt.setDate(QDate(1980, 1, 1));
            dt.setTime(QTime(0, 0, 0));
            return dt.addSecs(m_ContractDetails[PortfolioType::BY][key].Expiry);
        }
        if(m_ContractDetails[PortfolioType::F2F].contains(key)){
            QDateTime dt;
            dt.setOffsetFromUtc(0);
            dt.setDate(QDate(1980, 1, 1));
            dt.setTime(QTime(0, 0, 0));
            return dt.addSecs(m_ContractDetails[PortfolioType::F2F][key].Expiry);
        }
      return QDateTime();
    }

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

    //F1_F2 means Btfy and F2F
    if(type==PortfolioType::F1_F2){
        if(m_ContractDetails[PortfolioType::BY].contains(key))
            return m_ContractDetails[PortfolioType::BY][key].Expiry;
        if(m_ContractDetails[PortfolioType::F2F].contains(key))
            return m_ContractDetails[PortfolioType::F2F][key].Expiry;
       return 0;
    }

    if(m_ContractDetails[type].contains(key)){
        return m_ContractDetails[type][key].Expiry;
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


 bool ContractDetail::LoadContractLocal(QHash<QString, QStringList> &_m_ContractDetailsFiltered,
                                        QStringList &F2F_data_list_Sorted_Key,
                                        QStringList &BFLY_data_list_Sorted_Key,
                                        QStringList &BFLY_BID_data_list_Sorted_Key)

 {
     try
     {
         QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
         QString directory = appDataPath + "/Data";


         QString fileName = directory + "/contracts.bin";

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
                 m_ContractDetails.clear();
                 F2F_data_list_Sorted_Key.clear();
                 BFLY_data_list_Sorted_Key.clear();
                 BFLY_BID_data_list_Sorted_Key.clear();


                 QHash<QString, contract_table> BFLY_Contract_Hash;
                 QHash<QString, contract_table> F2F_Contract_Hash;
                 QHash<QString, contract_table> BFLY_BID_Contract_Hash;

                 long long actualMin = LLONG_MAX ;
                 QDataStream in(&file);

                while (!in.atEnd()) {
                     QString tokenNo;
                     QString PortfolioType;

                     contract_table contractT;
                     in >> PortfolioType>> tokenNo >> contractT.InstrumentType >> contractT.InstrumentName >> contractT.OptionType >> contractT.StrikePrice >> contractT.LotSize >> contractT.Expiry >> contractT.TokenNumber >> contractT.StockName >> contractT.MinimumSpread >> contractT.VolumeFreezeQty;
                     if(contractT.Expiry>0){
                         if(actualMin>contractT.Expiry)
                             actualMin = contractT.Expiry;
                     }
                     _m_ContractDetailsFiltered[contractT.InstrumentType].append(QString::number(contractT.TokenNumber));

                     int algo_type = PortfolioType.toInt();
                     // qDebug() << "---test--- algo type"<<algo_type << value.InstrumentType;
                     switch (algo_type) {
                     case PortfolioType::BY:{
                         BFLY_Contract_Hash.insert( tokenNo, contractT);
                         BFLY_data_list_Sorted_Key.append(tokenNo);
                         break;
                     }
                     case PortfolioType::F2F:{
                         F2F_Contract_Hash.insert( tokenNo, contractT);
                         F2F_data_list_Sorted_Key.append(tokenNo);
                         break;
                     }
                     case PortfolioType::BFLY_BID:{
                         BFLY_BID_Contract_Hash.insert( tokenNo, contractT);
                         BFLY_BID_data_list_Sorted_Key.append(tokenNo);
                         break;
                     }

                     default:
                         break;
                     }
                 }

                m_ContractDetails[PortfolioType::BFLY_BID] = BFLY_BID_Contract_Hash;
                m_ContractDetails[PortfolioType::BY] = BFLY_Contract_Hash;
                m_ContractDetails[PortfolioType::F2F] = F2F_Contract_Hash;



                 qDebug()<<"Loaded ContractDetails from "<<fileName<<"  size="<<m_ContractDetails.size();

                 if(BFLY_BID_Contract_Hash.size()==0&&BFLY_Contract_Hash.size()==0&&F2F_Contract_Hash.size()==0)
                     return false;


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
                             m_ContractDetails.clear();
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

     // Iterate over the QMap
     for (auto it = ContractDetail::m_ContractDetails.begin(); it != ContractDetail::m_ContractDetails.end(); ++it) {
         QString PortfolioType = QString::number(it.key());
         QHash<QString, contract_table>& hash = it.value();

         // Iterate over the QHash
         for (auto hashIt = hash.begin(); hashIt != hash.end(); ++hashIt) {
             QString toeknNo = hashIt.key();
             out << PortfolioType
                 << toeknNo
                 << hashIt.value().InstrumentType
                 << hashIt.value().InstrumentName
                 << hashIt.value().OptionType
                 << hashIt.value().StrikePrice
                 << hashIt.value().LotSize
                 << hashIt.value().Expiry
                 << hashIt.value().TokenNumber
                 << hashIt.value().StockName
                 << hashIt.value().MinimumSpread
                 << hashIt.value().VolumeFreezeQty;
         }
     }


 }
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

