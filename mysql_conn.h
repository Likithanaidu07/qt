#pragma once
//#include "mainwindow.h"
#ifndef mysql_conn_H
#define mysql_conn_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "QSqlQueryModel"
#include "QMessageBox"
#include "QDateTime"
#include "defines.h"
#include "QHashIterator"
#include "QHash"
#include <qmutex.h>
#include <QVBoxLayout>
#include "PortFolio/table_portfolios_model.h"
//#include "PortFolio/PortfolioParser.h"
#include <TradePosition/trade_table_model.h>
#include "NetPosition/net_position_table_model.h"
#include "CombinedTracker/combined_tracker_table_model.h"
#include "Liners/liners_model.h"
//class MainWindow;
#include "PortFolio/portfoliocustomsorting.h"
#include "MissedTrades/missed_trade_table_model.h"
#include "Order_F1_F2/order_f1_f2_model.h"
#include "OpenPosition/open_position_model.h"

struct net_pos_data_{
    QString  token_number;
    QString  Stock_Name;
    QString OptionType;
    QString InstrumentName;
    QString  Expiry;
    double Buy_Total_Lot;
    double Sell_Total_Lot;
    double Buy_Price;
    double Sell_Price;
    double Buy_Avg_Price;
    double Sell_Avg_Price;
    double BuyValue;
    double SellValue;
    double Net_Qty;
    int lotSize;
    double M2M;
    double MarginUsed;
    double exp;
    int Span;

    //QString MTM;
};

struct open_pos_data{
    QString  Stock_Name;
    double Buy_Total_Lot;
    double Sell_Total_Lot;
    double Buy_Avg_Price;
    double Sell_Avg_Price;
    double Net_Qty;
};
struct Liners_Data{
    QString algoID;
    QString algoName;
    QList<double> BuyAvgPrice;
    QList<int> BuyQtyLots;
    QList<double> SellAvgPrice;
    QList<int> SellQtyLots;
    QList<double> NetQty;
    QList<double> profit;
    QString Traded_Lot;
    QString StockName;


};

//class chlildofMainWindow: public MainWindow{
//public:

//};

class mysql_conn: public QObject
{
    Q_OBJECT
public:
    explicit mysql_conn(QObject *parent = 0,QString conne_name="main_db_conn");
    ~mysql_conn();
    void get_algo(QString str);
    QString HostName;
    int Port;
    QString DatabaseName;
    QString DB_UserName;

    QString Password;
    QSqlDatabase db;
    double devicer{100.0};
    int decimal_precision;
    QString InstrumentTypeFilter;
    static userInfo userDataStatic;

    userInfo login( QString user_name,  QString password);
    QHash<QString,PortFolioData_Less>  getPortfoliosTableData(QAtomicInt &reloadSortSettFlg,int &AlgoCount,Combined_Tracker_Table_Model *comb_tracker_model,QHash<QString, PortfolioAvgPrice> &averagePriceList,QString user_id, QStringList TradedPortFolioList, QStringList &PortFoliosToDelete );

    //QHash<QString, contract_table>  getContractTable( QHash<int , QStringList> &m_ContractDetails_Grouped_,userInfo userData);
    QHash<QString, contract_table>  getContractTable( QHash<int , QStringList> &m_ContractDetails_Grouped_,QHash<QString, QStringList> &_m_ContractDetailsFiltered,userInfo userData);
    QHash<QString, int>  get_SettPrice();

    QSqlQuery runQuery(QString qry_str);
    bool updateDB_Table(QString query_str,QString &msg);
    bool modifytype(QString value,QString user_id,QString &msg);
    bool resetPassword(const QString &new_password,QString user_id,QString &msg);
    QString get_Algo_Name(int algo_type,int leg1_token_number,int leg2_token_number,int leg3_token_number,double devicer,int decimal_precision);
    void  getTradeTableData(int &TraderCount,Trade_Table_Model *model,Order_F1_F2_Model * f1f2_order_table_model,Liners_Model *liners_model ,QString user_id,QHash<QString, PortFolioData_Less> PortFolioTypeHash,QStringList &algosToDisable,QStringList ExecutedTableHilightExcludeListint ,int &traderData_ID_OnAppStart,QStringList &newlyTradeAlgos);
    void  getNetPosTableData(double &BuyValue_summary,double &SellValue,double &Profit_summary,double &BuyQty_summary,double &SellQty_summary,double &NetQty_summary,Net_Position_Table_Model* model, open_position_model* openpos_model, QString user_id);
    void  getNetPosTableData_BackUp(double &BuyValue_summary,double &SellValue,double &Profit_summary,double &BuyQty_summary,double &SellQty_summary,double &NetQty_summary,Net_Position_Table_Model* model,QString user_id,QHash<QString,int> PortFoliosLotSizeHash);
    void getMissedTradeData(Missed_Trade_Table_Model* model,QString user_id);


    void getSummaryTableData(int &OrderCount,QString user_id);
    void getLinersTableData(Liners_Model *model,QString user_id,QHash<QString, PortFolioData_Less> PortFolioTypeHash);
    QString fixDecimal(double num,int decimal_precision);
    algo_data_insert_status insertToAlgoTable(algo_data_to_insert data,int MaxPortfolioCount,QString &msg);
    bool deleteAlgo(QString PortfolioNumber,QString &msg);
    bool deleteAlgos(QStringList PortfolioNumbers,QString &msg);
    bool deleteNonTradedAlgos(QStringList PortfolioNumbers,QString &msg);
    void logToDB(QString logMessage);

   // QList<QHash<QString,QString>> getOrderPopUPData(QString user_id, QString portfolioNumber,QString PortfolioType);
    QList<QHash<QString,QString>> getTradePopUPData(QString user_id, QString localOrderID, int lotSize );
    algo_data_insert_status place_F1F2_Order(QString userID,QString Leg1TokenNumber,QString sellprice,QString sellqty,QString buyprice,QString buyqty,int MaxPortfolioCount,QString orderQty,QString &msg,bool checkDuplicateExist);
    algo_data_insert_status retry_F1F2_Order( QString  OrderId,  QString ID,QString userID,QString Leg1TokenNumber,QString sellprice,QString sellqty,QString buyprice,QString buyqty,int MaxPortfolioCount,QString orderQty,QString &msg,bool checkDuplicateExist);

    ///
    /// \brief loadCurrentDayLogs to load logs of one day
    ///
    void loadCurrentDayLogs();
    QHash<QString, contract_table> prpareContractDataFromDB(QString queryStr, QSqlDatabase *db,QStringList &tokenData);
    QList<int> getPortfolioTypesForInstrumentType(const QString& data, const QMap<int, QStringList>& algoFilterMap);
    QHash<QString,QString>  getOrderDetailsData(QString  PortfolioNumberStr,QString user_id);
    bool clearAlogsForImport(QString user_id,QString &msg);
    bool insertAlogsForImport(QList<portfolioImportExportData> pData ,QString &msg);
    int getLastestTradetData_ID_FromTradeTable(QString user_id);

private:
    bool checkDBOpened( QString &mesg);
    void loadSettings();
    QMutex mutex;
    QString getAlgoTypeQuery(PortfolioType type, userInfo obj);
    userInfo userLoginInfo;
    double calculateAverage(const QList<double> &list);
    int calculateSum(const QList<int> &list);
    portfolioCustomSorting *portfolioCustomSort;


//    MainWindow MainWindow;


signals:
    void display_log_text_signal(QString);
    void updateModelDataListSignal(QList <PortfolioObject*> portfolio_data_list, bool clearTableFlg);

};



#endif // mysql_conn_H
