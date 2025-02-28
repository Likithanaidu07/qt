//#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QtConcurrent/QtConcurrent>
#include "QListWidgetItem"
#include <QWaitCondition>

#include <QTableView>
#include <QDockWidget>
#include "QAtomicInt"
#include <QLineEdit>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QBoxLayout>
#include <QStandardItemModel>
#include <QListView>
#include "settings_window.h"
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
#include "TradePosition/tradetableheaderview.h"
#include "TradePosition/trade_table_model.h"
#include "NetPosition/net_position_table_model.h"
#include "CombinedTracker/combined_tracker_table_model.h"
#include "SlowDataIndices/slowdataindices.h"
#include "Convert_to_Algo/convert_algo_win.h"
#include"ui_loginwindow.h"
#include "loadingdatawindow.h"
#include "order_detail_popup.h"
#include "trade_details_popup.h"
#include "Liners/liners_model.h"
#include "Liners/linersheaderview.h"
#include "NetPosition/netpos_searchfilterproxymodel.h"


#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"
#include "AutoHideDockContainer.h"
#include "DockAreaWidget.h"
#include "DockAreaTitleBar.h"
#include "sortsettingpopup.h"
#include "F1_F2/f1_f2_buysell.h"

#include "MissedTrades/missed_trade_table_model.h"
#include "Cards/logs_cards.h"
#include "Cards/summary_cards.h"
#include "Cards/watch_cards.h"
#include "PortFolio/portfolio_searchfilterproxymodel.h"
#include "TradePosition/tradetable_searchfilterproxymodel.h"
#include "watch_data_card.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "Order_F1_F2/order_f1_f2_delegate.h"
#include "Order_F1_F2/order_f1_f2_model.h"
#include "Order_F1_F2/order_f1_f2_tableheaderview.h"
#include "OpenPosition/open_position_model.h"
#include "cache_file_io.h"
#include <QApplication>
using namespace ads;


#define BACKEND_LOG

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
    OrderDetail_Popup *orderPopUpWin;
    TradeDetailsPopup *tradePopUpWin;
 QSqlDatabase db;
    //void updateLabels();
 void resetPassword(const QString &newPassword, const QString &confirmPassword);
   //void resetPassword(const QString &newPassword);

//    QStandardItemModel *model_searchInstrument_BOX_Leg1;
    QString foo_token_number_start_strike;// this for start strike input of F2F
    QString foo_token_number_end_strike;// this for start strike input of F2F
    // QDockWidget *T_Algorithm_DockWin;
    // ContractDetail* contracts;
    bool backendConn_Status = false;
    QString fixDecimal(double num,int decimal_precision);
signals:
    void data_loded_signal(int table);
    void logoutRequested();
    void display_log_text_signal(QString);
    void update_ui_signal(int);
    void signalHideProgressBar();
    void data_summary_update_signal(QStringList);
    void indicesDataRecv_Signal_watch_card(Indices_Data_Struct);
    void logDataSignal(QStringList);
    void showMessageSignal(QString);
    void requestDeleteConfirmation(const QStringList &portfoliosToDelete);

private:
    Ui::MainWindow *ui;
    QStringList logsdata;
    Settings_Window *sett_win= nullptr;
    QStringList savedWatchItems;
    QList<watch_data_card*> watchCardWidgetList;
  //  QTimer* TableRefreshTimer;
    QStringList summarydatList;
    bool newIndicesData;


#ifdef BACKEND_LOG
    QFile backend_logFile;
    QTextStream backend_logStream;

    void write_backend_Log(const QString &message);
#endif

   // QTimer* Trade_TableRefreshTimer;

    loadingdatawindow *loadingDataWinodw;
    SortSettingPopUp *sortWin; // Add this line
//    QAction *summaryAction;

    bool showMessagOnceFlg;
    int AlgoCount;
    int OrderCount;
    int TraderCount;
    double BuyValue_summary;
    double SellValue;
    double Profit;
    double BuyQty;
    double SellQty;
    double NetQty;
    int M2M;
    int AvlMargin;
    int UsedMargin;
    double BuyQty_summary;
    double SellQty_summary;
    QHash<QString,PortFolioData_Less> PortFolioHashLessHash;
    ads::CDockManager* DockManagerMainPanel;
 //  ads::CDockManager *DockManagerSidePanel;

 //   cache_file_io *CacheFileIO;
  //  QStringList tradedDataListDisabledOnMaxLoss_Thr_Reach; //This will contiain the Trader_Data from TradeTableBid if it dsiabled on MaxThr_Limit
    int traderData_ID_OnAppStart; // This will be used to disable alogo on  MaxThr_Limit, all the traderData greater than this will be considered, and this will be load when app start.


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
    QAtomicInt refresh_entire_table;

   // QAtomicInt data_loading_thread_running;
    QAtomicInt contractDetailsLoaded;
    QAtomicInt loggedInFlg;
    QAtomicInt reloadSortSettFlg; // this flag is used for reloading sorting config
    QAtomicInt deletingPortFolioFlg; // this flag is used to prevent portfolio table re-load while deleting.


    BackendComm *backend_comm;
    QThread *backend_comm_thread;

    QWaitCondition waitConditionDataLoadThread;
    QMutex DataLoadMutex;

    void start_dataLoadingThread();
    void stop_dataLoadingThread();
    void refreshTables();
    //void refreshTradeTable();
    void triggerImmediate_refreshTables();

    void loggedOut();
    void stopBG_Threads();

    Table_Portfolios_Model  *T_Portfolio_Model;
    Portfolio_SearchFilterProxyModel *T_Portfolio_ProxyModel;
    table_portfolios_custom *T_Portfolio_Table;
    Table_Portfolios_Delegate *T_Portfolio_Delegate;

    QTableView *net_pos_table;
    Net_Position_Table_Model *net_pos_model;
    CDockWidget *dock_win_net_pos;
     QSortFilterProxyModel *netposproxyModel;

    QTableView *liners_table;
    Liners_Model *liners_model;
    CDockWidget *dock_win_liners;

    QTableView *open_position;
    open_position_model *OpenPositionmodel;
    CDockWidget *dock_win_Open_position;

    QTableView *combined_tracker_table;
    Combined_Tracker_Table_Model *combined_tracker_model;
    CDockWidget *dock_win_combined_tracker;


    QTableView *trade_table;
    Trade_Table_Model *trade_model;
    CDockWidget *dock_win_trade;
     QSortFilterProxyModel *tradetableproxyModel;


    QTableView *missed_trade_table;
    Missed_Trade_Table_Model *missed_trade_model;
    CDockWidget *dock_win_missed_trades;
      QSortFilterProxyModel *SkippedproxyModel;



    QTableView *f1f2_order_table;
    Order_F1_F2_Model *f1f2_order_table_model;
    CDockWidget *dock_win_f1f2_order;


//    QLineEdit *line_edit_trade_search;
//    QList<LiveDataWidget*> liveDataWidgetList;

    int currentIDxSearch = 0;
//    QModelIndexList table_search_result_list;

    CDockWidget *T_Portfolio_DockWin;
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
    class Summary_cards *summary;
    class Watch_cards *watch;
    class Logs_cards *logs;

    QHash<QString, Indices_Data_Struct> indicesDataList;//hash to store indices data for watch

    QMutex indicesDataMutex; //list to store indices data for watch
    QString watchItemSelectedindexName;
   // QStringList savedWatchItems; // saved watch item to display on watch
    void showSaveWatchOnListView();
   // void initWatchWindow();
    void updateSelecteWatch_UI( Indices_Data_Struct data);
    void addToSavedWatchItems(Indices_Data_Struct data);
    void removeFromSavedWatchItems(Indices_Data_Struct data);
    void saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList);
 //   void loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList);

    void saveDockManagerState();
     void restoreDockManagerState();
     void restoreTableViewColumnState(QTableView *tableView);
     void saveTableViewColumnState(QTableView *tableView);
      void initializeGlobalHotKeys();
      QHash<QString, QAction*> HotKeyShortcutObjects;

      QModelIndexList  getSelectedPortFolioIndexs();
      void instilizeWatchUIOnTopBar();
      void updateWatchDataCard(Indices_Data_Struct data);
      int getWatchWindowChildWidth(QHBoxLayout* layout);
      void adjustTopBarLayout();
      void updateHotKeysAction(QString action,QAction *shortcutAction);
      void sendBackendCommandOnAddPortfolio();

public slots:

    void addWatchDataCard_TO_UI(Indices_Data_Struct data);
    void removeWatchDataCard_From_UI(Indices_Data_Struct data);
    void add_remove_watch_card_slot(bool add,Indices_Data_Struct data);

   // void startTableRefreshTimer();
    void openSettingsWindow();
    void onPortfolioAdded();
    void profolioTableEditFinshedSlot(QString val,QModelIndex);
    void onRequestDeleteConfirmation(const QStringList &PortFoliosToDelete);
//    void edit_Started_PortFolio_Table(int row,int col);
//    void tradeTableSerachNext();
//    void tradeTableSerachTxtChanged();
    void loggedIn();
    void loadCurrentDayLogs();
    void loggedInSucessful(userInfo userData);
    void Delete_clicked_slot();
    void F1_clicked_slot();
    void F2_clicked_slot();

    void indicesDataRecv_Slot(Indices_Data_Struct data);
    void slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct>& data);
    void backend_comm_Data_Slot(QString msg,SocketDataType msgType);
//    void T_Portfolio_Table_cellClicked(const QItemSelection&, const QItemSelection&);
    void updatePortFolioStatus(QModelIndex index);
   // void resizePortFolioTableColWidthSlot(int width);
    void T_Portfolio_Table_cellClicked(const QItemSelection&, const QItemSelection&);
    void T_Portfolio_Table_cellDoubleClicked(const QModelIndex &index);
    void trade_table_cellDoubleClicked(const QModelIndex &index);
    void highlightRowInF1F2Order(int row);
     void Skipped_Trades_cellDoubleClicked(const QModelIndex &index);
    void f1f2_order_table_cellDoubleClicked(const QModelIndex &index);
    void slotAddLogForAddAlgoRecord(QString str);
    void slotHideProgressBar();
    void startall_Button_clicked();
    void stopall_Button_clicked();
    void refresh_Button_clicked();
    void duplicate_Button_clicked();
    void updateSummaryLabels();
    void showMessageSlot(QString);
    void sorting_Button_clicked();
    void onPortFolioTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onTradeTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onNetposTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onLinersTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onF1F2OrderTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);

    void onCombined_tracker_tableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onMissed_trade_tableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onOpen_Position_tableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void onSummaryActionTriggered();
    void onChangepasswordActionTriggered();
    void onWatchActionTriggered();
    void onLogActionTriggered();
    void updateHotKeysSlot(QStringList actions);

    void exportTableViewToCSV();


private slots:
    void on_close_clicked();
    void on_minimize_clicked();
 //   void  on_menu_clicked();
   // void showResetPasswordDialog();
    void on_OrderBook_Button_clicked();
 //   void on_OrderBook_Close_clicked();
    void on_Positions_Button_clicked();
  //  void on_Positions_Close_clicked();
    void on_Liners_Button_clicked();
    void on_OpenPosition_Button_clicked();
  //  void on_Liners_Close_clicked();
    void on_HP_Button_clicked();
 //   void on_HP_Close_clicked();
    void on_Templates_Button_clicked();
   // void on_Templates_Close_clicked();
    void on_maximize_clicked();

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void update_ui_slot(int type);

//    void OnDockWidgetClose(UiUtils::DockWidgetType p_Type);
    void OnAlgorithmDockWidgetVisiblityChanged(bool p_Visible);
    void OnOrderBookDockWidgetVisiblityChanged(bool p_Visible);
    void OnF1F2OrderDockWidgetVisiblityChanged(bool p_Visible);

    void OnPositionsDockWidgetVisiblityChanged(bool p_Visible);
    void OnHPDockWidgetVisiblityChanged(bool p_Visible);
    void OnOpenDockWidgetVisiblityChanged(bool p_Visible);
    void OnMTDockWidgetVisiblityChanged(bool p_Visible);
    void OnLinersDockWidgetVisiblityChanged(bool p_Visible);
    void on_Algorithms_Button_clicked();
   // void on_Algorithms_Close_clicked();


    void ConvertAlgo_button_clicked();
   // void on_lineEditWatchSearch_textChanged(const QString &arg1);
   // void on_listWidgetWatch_itemClicked(QListWidgetItem *item);

 //   void on_listWidgetWatch_itemDoubleClicked(QListWidgetItem *item);
   // void on_listWidgetWatch_itemSelectionChanged();

  //  void on_toggle_Button_1_clicked();


    void on_MissedTrade_Button_clicked();

  //  void on_MissedTrade_Close_clicked();

  //  void on_comboBox_currentTextChanged(const QString &arg1);

    void on_F1F2Trade_Button_clicked();

    void import_Action();
    void export_Action();

    void onRetryButtonClickedMissedTrade(int row);

protected:
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

};
#endif // MAINWINDOW_H
