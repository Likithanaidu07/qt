#ifndef CONTRACTDETAIL_H
#define CONTRACTDETAIL_H
#include <QDir>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "defines.h"
#include "QHash"
#include <QMutex>
#include "QSettings"
#include "QStandardItemModel"
#include <QStandardPaths>
#define STORE_CONTRACT_LOCALLY// Enabling this will store contract.bin locally by fetching it from the DB on each new day when logging in for the first time,
                              //and on subsequent logins, the contract will load from the local file.





class ContractDetail
{
private:

    explicit ContractDetail() noexcept;

    static ContractDetail* instance;
    static QMutex mutex;
    static double devicer_contract;
    static double decimal_precision_contract;
    static bool settingLoaded;
    userInfo userData;

public:


    static QHash<int , QStringList> m_ContractDetails_Grouped;
    static QHash<QString, contract_table> m_ContractDetails_Hash;

    static QHash<QString, QStringList> m_ContractDetailsFiltered;

    static QHash<QString, int> m_SettPrice;


//    static QStandardItemModel *model_searchInstrument_BOX_Leg1; // used in add algo window  for BOX startegy
   // static QStandardItemModel *model_start_strike_BFLY; // used in add algo window  for BFLY startegy
   // static QStandardItemModel* model_searchInstrument_F2F_Leg1;
  //  static QStandardItemModel* model_FUT_CON_REV;
  //  static QStandardItemModel *model_start_strike_BFLY_BID;
   // static QStandardItemModel *model_start_strike_BOX_BID;

  //  static QStandardItemModel* model_F1_F2;

    static ContractDetail& getInstance();
    void ReloadContractDetails(userInfo uData);
    qint64 GetExpiry(int token, int type);
    QHash<QString, contract_table>  GetContracts();
   // QStringList Get_F2F_data_list_Sorted_Key();
   // QStringList Get_BFLY_data_list_Sorted_Key();
   // QStringList Get_BFLY_BID_data_list_Sorted_Key();
   // QStringList Get_BOX_BID_data_list_Sorted_Key();
    QStringList Get_Tokens_For_PortfolioType(PortfolioType type);
    QStringList Get_Tokens_For_IntrumentType(InstrumentType type);

    QHash<QString, int> get_SettPrice();


   // QHash<QString, contract_table> GetFutureContracts();
    QHash<QString, contract_table> GetContracts(QString type);

    int GetLotSize(int token, int type);
    contract_table GetDetail(int token);
    bool checkTokenExist(int token);

    QString GetStockName(int token, int type);
    QString GetInstrumentName(int token, int type);
    QString GetInstrumentType (int token, int type);
    int GetStrikePriceOrg(int token, int type);
    QString GetStrikePrice(int token, int type);
    double GetVolumeFreezeQty(int token, int type);
    QString GetOptionType(int token, int type);
    QString GetOptionType(QString token);

    QString GetExpiry(int token, QString format, int type);

    QString GetExpiry(QString format, long long Expiry) const;
    QDateTime GetExpiryDate(int token, int type);
   // void create_inputFiledAutoFillModel_For_AddAlgoWindow();
  //  QStandardItemModel *Get_model_searchInstrument_F2F_Leg1();
   // QStandardItemModel *Get_model_FUT_CON_REV();
   // QStandardItemModel *Get_model_start_strike_BFLY();

  //  QStandardItemModel *Get_model_start_strike_BFLY_BID();
   // QStandardItemModel *Get_model_start_strike_BOX_BID();

  //  QStandardItemModel* Get_model_F1_F2();

#ifdef STORE_CONTRACT_LOCALLY
    PortfolioType checkAlogTypeForTheData(QString instrument_type);
   // void StoreContractToLocalFile();
    bool LoadContractLocal(QHash<QString, QStringList> &_m_ContractDetailsFiltered);
#endif
};



#endif // CONTRACTDETAIL_H
