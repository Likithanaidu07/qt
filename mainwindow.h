//#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QtConcurrent/QtConcurrent>
#include "QListWidgetItem"

#include <QTableView>
#include <QDockWidget>
#include "QAtomicInt"
#include <QLineEdit>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QBoxLayout>
#include <QStandardItemModel>
#include <QListView>

#include <mysql_conn.h>
#include "ContractDetail.h"
#include "SlowData/slowdata.h"
#include "BackendComm/backendcomm.h"
#include "custom_q_completer.h"
#include "defines.h"
#include "PortFolio/table_portfolios_delegate.h"
#include "PortFolio/table_portfolios_model.h"
#include "PortFolio/table_portfolios_custom.h"
#include "PortFolio/portfolioheaderview.h"
#include "TradePosition/trade_table_model.h"
#include "NetPosition/net_position_table_model.h"
#include "CombinedTracker/combined_tracker_table_model.h"
#include "SlowDataIndices/slowdataindices.h"
#include "Convert_to_Algo/convert_algo_win.h"
#include"ui_loginwindow.h"
#include "loadingdatawindow.h"
#include "order_detail_popup.h"

class mysql_conn;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

enum UI_UPDATE_TYPE{
    LOADED_MODEL=0
};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    userInfo userData;
    void add_logs(QString str);
    bool pressed;
    QPoint position;
    QStandardItemModel model;
    OrderDetail_Popup *orderWin;



//    QStandardItemModel *model_searchInstrument_BOX_Leg1;
    QString foo_token_number_start_strike;// this for start strike input of F2F
    QString foo_token_number_end_strike;// this for start strike input of F2F
    // QDockWidget *T_Algorithm_DockWin;
    // ContractDetail* contracts;
signals:
    void data_loded_signal(int table);
    void logoutRequested();
    void display_log_text_signal(QString);
    void update_ui_signal(int);
    void signalHideProgressBar();
private:
    Ui::MainWindow *ui;

    loadingdatawindow *loadingDataWinodw;

    void createINIFileIfNotExist();
    void loadSettings();
    mysql_conn * db_conn;
    QString  htmlLogsContent;

    QString base64_encode(QString str);
    QString base64_decode(QString str);
    void generateAlgo();
    void searchbar();
    SlowData* slowData;
    SlowDataIndices* slowDataIndices;

//    void stackViewSwithAnimation(int currentPageIndex,int nextPageIndex, QStackedWidget *stackedWidget);
    void loadContract();
    QFuture<void> futureBackGroundDBThread;
    QAtomicInt quit_db_data_load_thread;
    QAtomicInt data_loading_thread_running;
    QAtomicInt contractDetailsLoaded;
    QAtomicInt loggedInFlg;
    BackendComm *backend_comm;
    QThread *backend_comm_thread;


    void start_dataLoadingThread();
    void stop_dataLoadingThread();
    void refreshTableIn_Intervel();
    void loggedOut();

    Table_Portfolios_Model  *T_Portfolio_Model;
    table_portfolios_custom *T_Portfolio_Table;
    Table_Portfolios_Delegate *T_Portfolio_Delegate;

    QTableView *net_pos_table;
    Net_Position_Table_Model *net_pos_model;
    QDockWidget *dock_win_net_pos;

    QTableView *combined_tracker_table;
    Combined_Tracker_Table_Model *combined_tracker_model;
    QDockWidget *dock_win_combined_tracker;


    QTableView *trade_table;
    Trade_Table_Model *trade_model;
    QDockWidget *dock_win_trade;
//    QLineEdit *line_edit_trade_search;
//    QList<LiveDataWidget*> liveDataWidgetList;

    int currentIDxSearch = 0;
//    QModelIndexList table_search_result_list;

    QDockWidget *T_Portfolio_DockWin;
//    QHash<QString, MBP_Data_Struct> MBP_Data_Hash;
    void start_slowdata_worker();
    void stop_slowdata_worker();

    void start_slowdata_indices_worker();
    void stop_slowdata_indices_worker();
    QHash<QString, PortfolioAvgPrice> averagePriceList;

    void loadDataAndUpdateTable(int table);
    void searchTradeTable();

    double devicer = /*10000000*/100/*for bY bid bidding*/;
    double decimal_precision = 4;
    void start_backend_comm_socket_worker();
    void stop_backend_comm_socket_worker();

    class ConvertAlgo_Win *convertalgo;

    QHash<QString, Indices_Data_Struct> indicesDataList;//hash to store indices data for watch

    QMutex indicesDataMutex; //list to store indices data for watch
    QString watchItemSelectedindexName;
    QStringList savedWatchItems; // saved watch item to display on watch
    void showSaveWatchOnListView();
    void initWatchWindow();
    void updateSelecteWatch_UI( Indices_Data_Struct data);
    void addToSavedWatchItems(Indices_Data_Struct data);
    void removeFromSavedWatchItems(Indices_Data_Struct data);
    void saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList);
    void loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList);

public slots:
    void profolioTableEditFinshedSlot(QString val,QModelIndex);
//    void edit_Started_PortFolio_Table(int row,int col);
//    void tradeTableSerachNext();
//    void tradeTableSerachTxtChanged();
    void loggedIn();
    void loadCurrentDayLogs();
    void loggedInSucessful(userInfo userData);
    void Delete_clicked_slot();
    void indicesDataRecv_Slot(Indices_Data_Struct data);
    void backend_comm_Data_Slot(QString msg,SocketDataType msgType);
//    void T_Portfolio_Table_cellClicked(const QItemSelection&, const QItemSelection&);
    void updatePortFolioStatus();
    void resizePortFolioTableColWidthSlot(int width);
    void T_Portfolio_Table_cellClicked(const QItemSelection&, const QItemSelection&);
    void T_Portfolio_Table_cellDoubleClicked(const QModelIndex &index);
    void slotAddLogForAddAlgoRecord(QString str);
    void slotHideProgressBar();

private slots:
    void on_close_clicked();
    void on_minimize_clicked();
    void on_OrderBook_Button_clicked();
    void on_OrderBook_Close_clicked();
    void on_Positions_Button_clicked();
    void on_Positions_Close_clicked();
    void on_Liners_Button_clicked();
    void on_Liners_Close_clicked();
    void on_HP_Button_clicked();
    void on_HP_Close_clicked();
    void on_Templates_Button_clicked();
    void on_Templates_Close_clicked();
    void on_maximize_clicked();
    void on_toggle_Button_clicked();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void update_ui_slot(int type);

//    void OnDockWidgetClose(UiUtils::DockWidgetType p_Type);
    void OnAlgorithmDockWidgetVisiblityChanged(bool p_Visible);
    void OnOrderBookDockWidgetVisiblityChanged(bool p_Visible);
    void OnPositionsDockWidgetVisiblityChanged(bool p_Visible);
    void OnHPDockWidgetVisiblityChanged(bool p_Visible);
    void OnLinersDockWidgetVisiblityChanged(bool p_Visible);
    void on_Algorithms_Button_clicked();
    void on_Algorithms_Close_clicked();

    void on_ConvertAlgo_button_clicked();
    void on_lineEditWatchSearch_textChanged(const QString &arg1);
    void on_listWidgetWatch_itemClicked(QListWidgetItem *item);

    void on_listWidgetWatch_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidgetWatch_itemSelectionChanged();
};
#endif // MAINWINDOW_H
