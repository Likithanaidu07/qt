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
//class MainWindow;


struct net_pos_data_{
    // QString  token_number;
    //QString  Stock_Name;
    //QString  Expiry;
    double Buy_Total_Lot;
    double Sell_Total_Lot;
    // double Buy_Price;
    // double Sell_Price;
    double Buy_Avg_Price;
    double Sell_Avg_Price;
    //double Net_Qty;
    //QString MTM;
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
    double devicer;
    int decimal_precision;
    QString InstrumentTypeFilter;

    userInfo login( QString user_name,  QString password);
    void  getPortfoliosTableData(Table_Portfolios_Model* model,Combined_Tracker_Table_Model *comb_tracker_model,QHash<QString, PortfolioAvgPrice> &averagePriceList,QString user_id);
    QHash<QString, contract_table> getContractTable(QHash<QString, QStringList> &_m_ContractDetailsFiltered,QStringList &F2F_data_list_Sorted_Key,QStringList &BFLY_data_list_Sorted_Key);
    PortfolioType checkAlogTypeForTheData(QString instrument_type);

    QSqlQuery runQuery(QString qry_str);
    bool updateDB_Table(QString query_str);
    QString get_Algo_Name(int algo_type,int leg1_token_number,int leg2_token_number,int leg3_token_number,double devicer,int decimal_precision);
    void  getTradeTableData(Trade_Table_Model* model,QString user_id,QHash<QString, PortFolioData_Less> PortFolioTypeHash);
    void  getNetPosTableData(Net_Position_Table_Model* model,QString user_id);
    void getLinersTableData(QString user_id);
    QString fixDecimal(double num,int decimal_precision);
    algo_data_insert_status insertToAlgoTable(algo_data_to_insert data,int MaxPortfolioCount,QString &msg);
    bool deleteAlgo(QString PortfolioNumber,QString &msg);
    void logToDB(QString logMessage);

private:
    bool checkDBOpened( QString &mesg);
    void loadSettings();
    QMutex mutex;
//    MainWindow MainWindow;


signals:
};

#endif // mysql_conn_H
