#include"ContractDetail.h"
ContractDetail* ContractDetail::instance = nullptr;
QMutex ContractDetail::mutex;
QHash<QString, contract_table> ContractDetail::m_ContractDetails;
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
