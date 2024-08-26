#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QtConcurrent/QtConcurrent>
#include <QCoreApplication>
#include <QHeaderView>
#include <QShortcut>
#include <QFontDatabase>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLabel>
#include "contractdetail.h"
#include "OrderBook/table_orderbook_delegate.h"
#include "mysql_conn.h"
#include "watch_data_list_item.h"
#include "style_sheet.h"
#include "sortsettingpopup.h"
#include "Liners/liners_delegate.h"
#include "NetPosition/net_position_table_delegate.h"
#include "NetPosition/net_position_table_headerview.h"
#include "Cards/logs_cards.h"
#include "Cards/summary_cards.h"
#include "Cards/watch_cards.h"
#include "QMenu"

#define ENABLE_BACKEND_DEBUG_MSG

static const char stylesheetvis[]= "background: #596167;" "border-radius: 10px;";

extern MainWindow *MainWindowObj;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
     sortWin(nullptr), // Initialize the pointer to nullptr,
     convertalgo(nullptr)

{

    QFontDatabase::addApplicationFont(":/WorkSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/WorkSans-ExtraBold.ttf");

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //ui->sidePanel->setVisible(false);



    // Create the Card menu
    QMenu *cardMenu = new QMenu("Card", this);

    // Create actions
    QAction *summaryAction = new QAction("Summary", this);
    QAction *watchAction = new QAction("Watch", this);
    QAction *LogAction = new QAction("Logs", this);

    // Add actions to the Card menu
    cardMenu->addAction(summaryAction);
    cardMenu->addAction(watchAction);
     cardMenu->addAction(LogAction);


    // Create a ToolButton to act as the menu title on ui->card
    ui->toolButtonCards->setMenu(cardMenu);
    ui->toolButtonCards->setPopupMode(QToolButton::InstantPopup);

    ui->toolButtonCards->setStyleSheet(
        "QToolButton {"
        "    color: white;"
        "    font-family: 'Work Sans';"
        "    font-weight: bold;"
        "}"
        );
    QFont font("Work Sans", 10, QFont::Bold); // Set the font to Work Sans, size 10, bold
    ui->label_4->setFont(font);
    ui->label_4->setText("status :");
    ui->label_2->setFont(font);
    ui->label_2->setText("User Message :");

    // Connect the actions to their respective slots (if needed)
    connect(summaryAction, &QAction::triggered, this, &MainWindow::onSummaryActionTriggered);
    connect(watchAction, &QAction::triggered, this, &MainWindow::onWatchActionTriggered);
    connect(LogAction, &QAction::triggered, this, &MainWindow::onLogActionTriggered);



   // ui->sidePanel->setVisible(false);
    //ui->widget_3->setVisible(false);
      ui->widget_5->setVisible(true);
    //ui->widget_4->setVisible(false);
    //ui->toggle_Button_1->setVisible(false);
  //  connect(this,SIGNAL(data_summary_update_signal()),this,SLOT(updateSummaryLabels()));
    connect(this,SIGNAL(display_log_text_signal(QString)),this,SLOT(slotAddLogForAddAlgoRecord(QString)));
    loadingDataWinodw = new loadingdatawindow(this);
    connect(this,SIGNAL(signalHideProgressBar()),this,SLOT(slotHideProgressBar()));

    // db_conn =new mysql_conn(this,"main_db_conn");
    db_conn =new mysql_conn(0,"main_db_conn");
    connect(db_conn,SIGNAL(display_log_text_signal(QString)),this,SLOT(slotAddLogForAddAlgoRecord(QString)));

    connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));
    orderPopUpWin = new OrderDetail_Popup();
    tradePopUpWin = new TradeDetailsPopup();


    QDir dir;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    if (!dir.exists(path)) {
        if (dir.mkpath(path)) {
        }
        else{
            qDebug() << "Failed to create directory! "+path;
        }
    }
    createINIFileIfNotExist();
    loadSettings();

    contractDetailsLoaded.storeRelaxed(0);
    reloadSortSettFlg.storeRelaxed(1);
    deletingPortFolioFlg.storeRelaxed(0);
    quit_db_data_load_thread.storeRelaxed(0);
    portfolio_table_updating_db.storeRelaxed(0);
    portfolio_table_slected_idx_for_editing.store(-1);
    loggedInFlg.storeRelaxed(0);


    QPixmap pixmapclose(":/close_window_icon.png");
    QPixmap pixmapminimize(":/minimize_window_icon.png");
    QPixmap pixmapmaximize(":/maximize_window_icon.png");
    QFont headerfont("Work Sans");



    //CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    //CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    CDockManager::setConfigFlag(CDockManager::ActiveTabHasCloseButton, false);
    CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);



    /***********************Move Side Panel items to dock Widget**********************/

//    DockManagerSidePanel = new CDockManager(ui->sidePanel);
//    ui->sidePanel->layout()->addWidget(DockManagerSidePanel);
//    CDockWidget* watchWidgetDockWidget = new CDockWidget("Watch");
//    watchWidgetDockWidget->setWidget(ui->watchWidget);
//    watchWidgetDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
//    watchWidgetDockWidget->resize(250, 150);
//    watchWidgetDockWidget->setMinimumSize(200,150);
//    DockManagerSidePanel->addDockWidget(DockWidgetArea::TopDockWidgetArea, watchWidgetDockWidget);

//    ui->sidePanel->layout()->addWidget(DockManagerSidePanel);
//    CDockWidget* summaryhWidgetDockWidget = new CDockWidget("Summary");
//    summaryhWidgetDockWidget->setWidget(ui->summary_widget);
//    summaryhWidgetDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
//    summaryhWidgetDockWidget->resize(250, 150);
//    summaryhWidgetDockWidget->setMinimumSize(200,150);
//    DockManagerSidePanel->addDockWidget(DockWidgetArea::CenterDockWidgetArea, summaryhWidgetDockWidget);

//    ui->sidePanel->layout()->addWidget(DockManagerSidePanel);
//    CDockWidget* loghWidgetDockWidget = new CDockWidget("Logs");
//    loghWidgetDockWidget->setWidget(ui->Logs_Widget);
//    loghWidgetDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
//    loghWidgetDockWidget->resize(250, 150);
//    loghWidgetDockWidget->setMinimumSize(200,150);
//    DockManagerSidePanel->addDockWidget(DockWidgetArea::BottomDockWidgetArea, loghWidgetDockWidget);



    /**********************************************************************************/


    QVBoxLayout* Layout = new QVBoxLayout(ui->mainPanel);
    Layout->setContentsMargins(QMargins(0, 0, 0, 0));

    DockManagerMainPanel = new CDockManager(ui->mainPanel);
    Layout->addWidget(DockManagerMainPanel);


    /***********Init portfolio table Window**************************/
    QPixmap pixmapdock_algorithms_close(":/dock_close.png");

    T_Portfolio_DockWin = new CDockWidget("Algorithms");// new QDockWidget(tr("Algorithms"), this);
    connect(T_Portfolio_DockWin, SIGNAL(visibilityChanged(bool)), this, SLOT(OnAlgorithmDockWidgetVisiblityChanged(bool)));
    T_Portfolio_DockWin->setStyleSheet(dock_style);
    T_Portfolio_DockWin->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);

    T_Portfolio_DockWin->setMinimumSize(200,150);
   // const auto autoHideContainerPortfolioTable = DockManager->addAutoHideDockWidget(SideBarLocation::SideBarLeft, T_Portfolio_DockWin);
   // autoHideContainerPortfolioTable->setSize(480);

    DockManagerMainPanel->addDockWidget(DockWidgetArea::RightDockWidgetArea, T_Portfolio_DockWin);

        //Create Titlebar
        QWidget* titlecontainer = new QWidget;
        titlecontainer->setStyleSheet(DockTitleBar_Style);
        QHBoxLayout *titleinternalLayout = new QHBoxLayout(titlecontainer);
        titleinternalLayout->setSpacing(10);
        titleinternalLayout->setContentsMargins(17,8,10,6);
        QLabel *label = new QLabel("Algorithms");
        QFont fontlabel=label->font();
        QSpacerItem* titlespc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
        QToolButton* dockclose=new QToolButton();
        connect(dockclose, &QToolButton::clicked, [=](){ T_Portfolio_DockWin->close();});
        dockclose->setIcon(pixmapdock_algorithms_close);
        dockclose->setIconSize(QSize(14, 14));
        fontlabel.setFamily("Work Sans");
        label->setFont(fontlabel);
        label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
        titleinternalLayout->addWidget(label);
        titleinternalLayout->addSpacerItem(titlespc);
        titleinternalLayout->addWidget(dockclose);
        //T_Portfolio_DockWin->setTitleBarWidget(titlecontainer);
        //Titlebar Created

            QWidget* container = new QWidget;
            T_Portfolio_DockWin->setWidget(container);
            QGridLayout *lay = new QGridLayout(container);
            container->setLayout(lay);
            lay->setContentsMargins(5, 0, 0, 0);
            lay->setSpacing(0);
            lay->setRowStretch(0, 0); //set minimum hieght for first row
            lay->setRowStretch(1, 1);//set maximum hieght for second row table

            QLineEdit* line_edit_trade_search = new QLineEdit;
            line_edit_trade_search->setMaximumWidth(360);
            line_edit_trade_search->setMaximumHeight(1500);
            line_edit_trade_search->setPlaceholderText("Search Algo");
            QAction* search_action = new QAction(QIcon(":/search.png"), "", line_edit_trade_search);

            // Add the action to the QLineEdit
            line_edit_trade_search->addAction(search_action, QLineEdit::LeadingPosition);
            line_edit_trade_search->setStyleSheet(lineedit_dock_SS);

            QToolButton *ConvertAlgo_button = new QToolButton();
            connect(ConvertAlgo_button, SIGNAL(clicked()), this, SLOT(on_ConvertAlgo_button_clicked()));
            //Frame 364.png
            ConvertAlgo_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

            ConvertAlgo_button->setText("Build Algo");
            ConvertAlgo_button->setStyleSheet("text-align: center;");

            const char convert_to_algo_button[]="QToolButton {"
                                                //  "border-radius: 4px;" "border: 1px solid #188CCA;"
                                                  "background: #495057;"
                                                  "color: #FFF;"
                                                  "font-size: 12px;"
                                                  "font-style: normal;"
                                                  "font-weight: 500;"
                                                  "line-height: normal;"
                                                  "}"
                                                  "QToolButton:hover{"
                                                  "border-radius: 4px;"
                                                  "border: 1px solid #106B9A;"
                                                  "background: #106B9A;"
                                                  "}";
            ConvertAlgo_button->setStyleSheet(convert_to_algo_button);
            ConvertAlgo_button->setContentsMargins(10,-1,-1,-1);
            ConvertAlgo_button->setFont(headerfont);


            QWidget *test=new QWidget;
            const char layss[]="background: #E9ECEF;";
            test->setStyleSheet(layss);
            QHBoxLayout *internalLayout = new QHBoxLayout(test);
            test->setLayout(internalLayout);
            internalLayout->setContentsMargins(10,-1,-1,-1);
            QSpacerItem* spc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
            QToolButton* button1 = new QToolButton();
            connect(button1, SIGNAL(clicked()), this, SLOT(on_startall_Button_clicked()));
            button1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button1->setIcon(QIcon(":/start_all.png"));
            button1->setToolTip("Start All");

            QToolButton* button2 = new QToolButton();
            connect(button2, SIGNAL(clicked()), this, SLOT(on_stopall_Button_clicked()));
            button2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button2->setIcon(QIcon(":/stop_all.png"));
            button2->setToolTip("Stop All");

            QToolButton* button3 = new QToolButton();
            button3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button3->setIcon(QIcon(":/import.png"));
            button3->setToolTip("Import");

            QToolButton* button4 = new QToolButton();
            button4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button4->setIcon(QIcon(":/reset.png"));
            button4->setToolTip("Sorting");
            connect(button4, SIGNAL(clicked()), this, SLOT(on_sorting_Button_clicked()));

            QToolButton* button5 = new QToolButton();
            button5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button5->setToolTip("Portfolio Colour");

            QToolButton* button6 = new QToolButton();
            button6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button6->setIcon(QIcon(":/export.png"));
            button6->setToolTip("Export");

            const char stylesheet_tb[] =
                "QToolButton {"
                "border-radius: 4px;"
                "border: 1px solid #4F5D75;"
                "background: #345D71;"
                "color: #4F5D75;"
                "font-style: normal;"
                "font-size: 12px;"
                "font-weight: 500;"
                "line-height: normal;"
                "padding: 2px 5px;"
                "}"
                "QToolButton:hover {"
                "border-radius: 4px;"
                "border: 1px solid #4F5D75;"
                "background: #4F5D75;"
                "color: #FFF;"
                "font-family: Work Sans;"
                "font-size: 12px;"
                "font-style: normal;"
                "font-weight: 500;"
                "line-height: normal;"
                "padding: 2px 5px;"
                "}";

            for (auto w : {button1, button2, button3, button4, button5, button6}) {
        w->setStyleSheet(stylesheet_tb);
        QFont font = w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
            }




                internalLayout->addWidget(ConvertAlgo_button);
                internalLayout->addSpacerItem(spc);
                internalLayout->addWidget(line_edit_trade_search);
                internalLayout->addSpacerItem(spc);
                internalLayout->addWidget(button1);
                internalLayout->addWidget(button2);
                internalLayout->addWidget(button3);
                internalLayout->addWidget(button6);
                internalLayout->addWidget(button4);
                //internalLayout->addWidget(button5);
              //  internalLayout->addSpacerItem(spc);
                //internalLayout->addWidget(line_edit_trade_search);
                lay->addWidget(test, 0, 0);

    T_Portfolio_Table = new table_portfolios_custom(T_Portfolio_DockWin);
    T_Portfolio_Table->setObjectName("algo_table");
    T_Portfolio_Table->setStyleSheet(scroll_bar_SS);
    // T_Portfolio_Table->viewport()->setFocusPolicy(Qt::NoFocus);
    T_Portfolio_Model = new Table_Portfolios_Model();
    T_Portfolio_Delegate =  new Table_Portfolios_Delegate();
    //connect(T_Portfolio_Model,SIGNAL(edit_Started(int,int)),this,SLOT(edit_Started_PortFolio_Table(int,int)));
    QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::editFinished, this, &MainWindow::profolioTableEditFinshedSlot);
    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::editCompleted, this, &MainWindow::profolioTableEditFinshedSlot);
     QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::tabKeyPressed, T_Portfolio_Table, &table_portfolios_custom::handleTabKeyPressFromEditableCell);
    QObject::connect(T_Portfolio_Table, &table_portfolios_custom::spaceKeySignal, this, &MainWindow::updatePortFolioStatus);
    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::resizePortFolioTableColWidth, this, &MainWindow::resizePortFolioTableColWidthSlot);
   // QObject::connect(T_Portfolio_Table, &table_portfolios_custom::clicked, T_Portfolio_Model, &Table_Portfolios_Model::onItemChanged);
    QObject::connect(T_Portfolio_Table, &table_portfolios_custom::selectionChangedSignal, T_Portfolio_Model, &Table_Portfolios_Model::selectionChangedSlot);

    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::updateDBOnDataChanged, this, &MainWindow::updatePortFolioStatus);

    T_Portfolio_Table->setModel(T_Portfolio_Model);
    T_Portfolio_Table->setItemDelegate(T_Portfolio_Delegate);

    connect(T_Portfolio_Table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this, SLOT(T_Portfolio_Table_cellClicked(const QItemSelection&, const QItemSelection&)));


    connect(T_Portfolio_Table, &QTableView::doubleClicked, this, &MainWindow::T_Portfolio_Table_cellDoubleClicked);



    PortfolioHeaderView* headerView = new PortfolioHeaderView(Qt::Horizontal, T_Portfolio_Table);
    headerView->setFixedHeight(32);
    headerView->setFont(headerfont);
    headerView->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
    connect(headerView, &QHeaderView::sectionMoved, this, &MainWindow::onPortFolioTableHeader_Rearranged);

    T_Portfolio_Table->setHorizontalHeader(headerView);
    T_Portfolio_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    T_Portfolio_Table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    T_Portfolio_Table->horizontalHeader()->setStretchLastSection(true);
    T_Portfolio_Table->setShowGrid(false);
    T_Portfolio_Table->verticalHeader()->setVisible(false);
    T_Portfolio_Table->setAlternatingRowColors(true);
    T_Portfolio_Table->verticalHeader()->setVisible(false);
    T_Portfolio_Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    T_Portfolio_Table->setSelectionMode(QAbstractItemView::ExtendedSelection);

    lay->addWidget(T_Portfolio_Table,1,0);
    T_Portfolio_Table->show();
    restoreTableViewColumnState(T_Portfolio_Table);
    /***********Init portfolio table Window**************************/



    /***********Init Order Book Window**************************/
    QPixmap pixmapdock_trade_close(":/dock_close.png");

    dock_win_trade =  new CDockWidget(tr("Trades"));

    connect(dock_win_trade, SIGNAL(visibilityChanged(bool)), this, SLOT(OnOrderBookDockWidgetVisiblityChanged(bool)));
   // subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_trade);
   // dock_win_trade->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock_win_trade->setStyleSheet(dock_style);
    dock_win_trade->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_trade->setMinimumSize(200,150);
    const auto autoHideContainerTrade = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_trade);
    autoHideContainerTrade->setSize(480);

    //create a titlebar
    QWidget *trade_titlebar=new QWidget;
    trade_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *trade_title_layout=new QHBoxLayout(trade_titlebar);
    trade_title_layout->setSpacing(10);
    trade_title_layout->setContentsMargins(17,8,10,6);
    QLabel *trade_label=new QLabel("Trades");
    QFont font_trade_label=trade_label->font();
    font_trade_label.setFamily("Work Sans");
    trade_label->setFont(font_trade_label);
    trade_label->setStyleSheet("color: #495057; font-size: 16px; font-style: normal; font-weight: bold; line-height: normal;");
    QSpacerItem* trade_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_trade = new QLineEdit;
    line_edit_trade->setMaximumWidth(160);
    line_edit_trade->setStyleSheet(lineedit_dock_SS);
    QToolButton* trade_close=new QToolButton();
    connect(trade_close, &QToolButton::clicked, [=](){ dock_win_trade->close();});
    trade_close->setIcon(pixmapdock_trade_close);
    trade_close->setIconSize(QSize(14, 14));
    trade_title_layout->addWidget(trade_label);
    trade_title_layout->addSpacerItem(trade_spacer);
    trade_title_layout->addWidget(line_edit_trade);
    trade_title_layout->addWidget(trade_close);
    //dock_win_trade->setTitleBarWidget(trade_titlebar);


    QWidget* container_trade = new QWidget;
    dock_win_trade->setWidget(container_trade);
    QGridLayout *lay_Trade_Window = new QGridLayout(container_trade);
    lay_Trade_Window->setContentsMargins(0, 0, 0, 0);
    lay_Trade_Window->setSpacing(3);
    lay_Trade_Window->setRowStretch(0, 0); //set minimum hieght for first row
    lay_Trade_Window->setRowStretch(1, 1);//set maximum hieght for second row table




//    connect(button_Trade, &QPushButton::clicked, this, &MainWindow::tradeTableSerachNext);
//    connect(line_edit_trade, &QLineEdit::textChanged, this, &MainWindow::tradeTableSerachTxtChanged);




    trade_table = new QTableView();
    connect(trade_table, &QTableView::doubleClicked, this, &MainWindow::trade_table_cellDoubleClicked);

  //  trade_table->setStyleSheet(tableview_SS);
    trade_table->horizontalHeader()->setFixedHeight(32);
    trade_table->horizontalHeader()->setFont(headerfont);
    trade_table->setShowGrid(false);
    trade_table->setAlternatingRowColors(true);

    tradetableheaderview* headerViews = new tradetableheaderview(Qt::Horizontal, trade_table);
    headerViews->setFixedHeight(32);
    headerViews->setFont(headerfont);
    headerViews->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
//    headerViews->setSectionsMovable(true);
//    headerViews->setHighlightSections(true);
     connect(headerViews, &QHeaderView::sectionMoved, this, &MainWindow::onTradeTableHeader_Rearranged);
    trade_table->setHorizontalHeader(headerViews);

    lay_Trade_Window->addWidget(trade_table, 1, 0, 1, 3);

    trade_model = new Trade_Table_Model();
    trade_table->setObjectName("trade_table");
    Table_OrderBook_Delegate* trade_delegate=new Table_OrderBook_Delegate;
    trade_table->setModel(trade_model);
    trade_table->setItemDelegate(trade_delegate);
    //T_order_Delegate =  new trade_table_delegate();
    //trade_table->setItemDelegate(T_order_Delegate);

    // Configure column resizing
    //trade_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Make all columns resizable by default
    trade_table->setColumnWidth(1,300);

    trade_table->horizontalHeader()->setStretchLastSection(true);
    trade_table->verticalHeader()->setVisible(false);
    // trade_table->setStyleSheet("QHeaderView { background-color: #111111;} QHeaderView::section { background-color:#555555;color:#eeeeee;font-weight: 400; }");
    trade_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    trade_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    trade_table->setSelectionMode(QAbstractItemView::SingleSelection);
    /*trade_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                            "selection-color: #4D4D4D;"
                                            "color:#3D3D3D;"
                                            "} "
                                            "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    trade_table->show();
    restoreTradeTableViewColumnState(trade_table);
    /***********Init Order Book Window**************************/

    /************Positions Window********************************/
    QPixmap pixmapdock_position_close(":/dock_close.png");

    dock_win_net_pos =  new CDockWidget(tr("Positions"));
    connect(dock_win_net_pos, SIGNAL(visibilityChanged(bool)), this, SLOT(OnPositionsDockWidgetVisiblityChanged(bool)));
    //dock_win_net_pos->setAllowedAreas(Qt::AllDockWidgetAreas );
   // subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_net_pos);
    dock_win_net_pos->setStyleSheet(dock_style);
    dock_win_net_pos->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_net_pos->setMinimumSize(200,150);
    const auto autoHideContainerPos = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_net_pos);
    autoHideContainerPos->setSize(480);

    //create a titlebar
    QWidget *position_titlebar=new QWidget;
    position_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *position_title_layout=new QHBoxLayout(position_titlebar);
    position_title_layout->setSpacing(10);
    position_title_layout->setContentsMargins(17,8,10,6);
    QLabel *position_label=new QLabel("Positions");
    QFont font_position_label=position_label->font();
    font_position_label.setFamily("Work Sans");
    position_label->setFont(font_position_label);
    position_label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
    QSpacerItem* position_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_position = new QLineEdit;
    line_edit_position->setMaximumWidth(160);
    line_edit_position->setStyleSheet(lineedit_dock_SS);
    QToolButton* position_close=new QToolButton();
    connect(position_close, &QToolButton::clicked, [=](){ dock_win_net_pos->close(); });
    position_close->setIcon(pixmapdock_position_close);
    position_close->setIconSize(QSize(14, 14));
    position_title_layout->addWidget(position_label);
    position_title_layout->addSpacerItem(position_spacer);
    position_title_layout->addWidget(line_edit_position);
    position_title_layout->addWidget(position_close);
    //dock_win_net_pos->setTitleBarWidget(position_titlebar);




    net_pos_table = new QTableView(dock_win_net_pos);
    net_pos_table->setStyleSheet(scroll_bar_SS);

    net_pos_table->horizontalHeader()->setFixedHeight(32);
    net_pos_table->horizontalHeader()->setFont(headerfont);
    net_pos_table->setStyleSheet(tableview_SS);
    net_pos_table->setShowGrid(false);
    net_pos_table->setAlternatingRowColors(true);

    net_position_table_headerview* headerView_netpos = new net_position_table_headerview(Qt::Horizontal, net_pos_table);
    headerView_netpos->setFixedHeight(32);
    headerView_netpos->setFont(headerfont);
    headerView_netpos->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
    connect(headerView_netpos, &QHeaderView::sectionMoved, this, &MainWindow::onNetposTableHeader_Rearranged);
    net_pos_table->setHorizontalHeader(headerView_netpos);

    net_pos_model = new Net_Position_Table_Model();
    net_pos_table->setObjectName("netpos_table");
    net_pos_table->setModel(net_pos_model);
    net_pos_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Make all columns resizable by default
    net_pos_table->setColumnWidth(1,250);

    net_pos_table->horizontalHeader()->setStretchLastSection(true);
    net_pos_table->verticalHeader()->setVisible(false);
    /*  net_pos_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                        "selection-color: #4D4D4D;"
                                        "color:#3D3D3D;"
                                        "} "
                                        "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    net_pos_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    net_pos_table->setSelectionMode(QAbstractItemView::SingleSelection);
    net_position_table_delegate* netpos_delegate=new net_position_table_delegate;
    net_pos_table->setItemDelegate(netpos_delegate);
   net_pos_table->horizontalHeader()->setStretchLastSection(true);
    dock_win_net_pos->setWidget(net_pos_table);
    net_pos_table->show();
    restoreNetposTableViewColumnState(net_pos_table);
    /************Net Position Window********************************/

    /************Liners Window********************************/
    QPixmap pixmapdock_Liners_close(":/dock_close.png");

    dock_win_liners =  new CDockWidget(tr("Liners"));
    connect(dock_win_liners, SIGNAL(visibilityChanged(bool)), this, SLOT(OnLinersDockWidgetVisiblityChanged(bool)));
    //dock_win_liners->setAllowedAreas(Qt::AllDockWidgetAreas );
   // subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_liners);
    dock_win_liners->setStyleSheet(dock_style);
    dock_win_liners->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_liners->setMinimumSize(200,150);
    const auto autoHideContainer_liners = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_liners);
    autoHideContainer_liners->setSize(480);

    //create a titlebar
    QWidget *liners_titlebar=new QWidget;
    liners_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *liners_title_layout=new QHBoxLayout(liners_titlebar);
    liners_title_layout->setSpacing(10);
    liners_title_layout->setContentsMargins(17,8,10,6);
    QLabel *liners_label=new QLabel("Liners");
    QFont font_liners_label=position_label->font();
    font_liners_label.setFamily("Work Sans");
    liners_label->setFont(font_liners_label);
    liners_label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
    QSpacerItem* liners_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_liners = new QLineEdit;
    line_edit_liners->setMaximumWidth(160);
    line_edit_liners->setStyleSheet(lineedit_dock_SS);
    QToolButton* liners_close=new QToolButton();
    connect(liners_close, &QToolButton::clicked, [=](){ dock_win_liners->close(); });
    liners_close->setIcon(pixmapdock_position_close);
    liners_close->setIconSize(QSize(14, 14));
    liners_title_layout->addWidget(liners_label);
    liners_title_layout->addSpacerItem(liners_spacer);
    liners_title_layout->addWidget(line_edit_liners);
    liners_title_layout->addWidget(liners_close);
    //dock_win_liners->setTitleBarWidget(liners_titlebar);

    liners_table = new QTableView(dock_win_liners);
    liners_table->setStyleSheet(scroll_bar_SS);
    liners_table->horizontalHeader()->setFixedHeight(32);
    liners_table->horizontalHeader()->setFont(headerfont);
    liners_table->setStyleSheet(tableview_SS);
    liners_table->setShowGrid(false);
    liners_table->setAlternatingRowColors(true);

    linersheaderview* headerViewrs = new linersheaderview(Qt::Horizontal, liners_table);
    headerViewrs->setFixedHeight(32);
    headerViewrs->setFont(headerfont);
    headerViewrs->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
    connect(headerViewrs, &QHeaderView::sectionMoved, this, &MainWindow::onLinersTableHeader_Rearranged);
    liners_table->setHorizontalHeader(headerViewrs);

    liners_model = new Liners_Model();
    liners_delegate* liner_delegate=new liners_delegate;
     liners_table->setObjectName("liners_table");
    liners_table->setModel(liners_model);
    liners_table->setItemDelegate(liner_delegate);

    liners_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Make all columns resizable by default
    liners_table->setColumnWidth(1,300);
    liners_table->horizontalHeader()->setStretchLastSection(true);
    liners_table->verticalHeader()->setVisible(false);
    /*  net_pos_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                        "selection-color: #4D4D4D;"
                                        "color:#3D3D3D;"
                                        "} "
                                        "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    liners_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    liners_table->setSelectionMode(QAbstractItemView::SingleSelection);

    dock_win_liners->setWidget(liners_table);
    liners_table->show();
     restoreLinersTableViewColumnState(liners_table);
    /************Liners Window********************************/





    /************Historical Positions Window********************************/
    QPixmap pixmapdock_hp_close(":/dock_close.png");

    dock_win_combined_tracker =  new CDockWidget(tr("Historical Positions"));
    connect(dock_win_combined_tracker, SIGNAL(visibilityChanged(bool)), this, SLOT(OnHPDockWidgetVisiblityChanged(bool)));
    //dock_win_combined_tracker->setAllowedAreas(Qt::AllDockWidgetAreas );
    dock_win_combined_tracker->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_combined_tracker->setMinimumSize(200,150);
    const auto autoHideContainer_tracker = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_combined_tracker);
    autoHideContainer_tracker->setSize(480);

    //create a titlebar
    QWidget *hp_titlebar=new QWidget;
    hp_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *position_hp_layout=new QHBoxLayout(hp_titlebar);
    position_hp_layout->setSpacing(10);
   position_hp_layout->setContentsMargins(17,8,10,6);
    QLabel *hp_label=new QLabel("Historical Positions");
    QFont font_hp_label=hp_label->font();
    font_hp_label.setFamily("Work Sans");
    hp_label->setFont(font_hp_label);
    hp_label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
    QSpacerItem* hp_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_hp = new QLineEdit;
   line_edit_hp->setMaximumWidth(160);
    line_edit_hp->setStyleSheet(lineedit_dock_SS);
    QToolButton* hp_close=new QToolButton();
    connect(hp_close, &QToolButton::clicked, [=](){ dock_win_combined_tracker->close(); });
    hp_close->setIcon(pixmapdock_hp_close);
    hp_close->setIconSize(QSize(14, 14));
    position_hp_layout->addWidget(hp_label);
    position_hp_layout->addSpacerItem(hp_spacer);
    position_hp_layout->addWidget(line_edit_hp);
    position_hp_layout->addWidget(hp_close);
    //dock_win_combined_tracker->setTitleBarWidget(hp_titlebar);



    //subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_combined_tracker);

    combined_tracker_table = new QTableView(dock_win_combined_tracker);
    combined_tracker_table->setStyleSheet(tableview_SS);
    combined_tracker_table->horizontalHeader()->setFixedHeight(32);
    combined_tracker_table->horizontalHeader()->setFont(headerfont);
    combined_tracker_table->setShowGrid(false);
    combined_tracker_table->setAlternatingRowColors(true);

    combined_tracker_model = new Combined_Tracker_Table_Model();
    combined_tracker_table->setModel(combined_tracker_model);
    combined_tracker_table->horizontalHeader()->setStretchLastSection(true);
    combined_tracker_table->verticalHeader()->setVisible(false);
    combined_tracker_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    combined_tracker_table->setSelectionMode(QAbstractItemView::SingleSelection);

    dock_win_combined_tracker->setWidget(combined_tracker_table);
    combined_tracker_table->show();
    /************Historical Positions********************************/



    /************Missed Trades Window********************************/
    QPixmap pixmapdock_mt_close(":/dock_close.png");

    dock_win_missed_trades =  new CDockWidget(tr("Missed Trades"));
    connect(dock_win_missed_trades, SIGNAL(visibilityChanged(bool)), this, SLOT(OnMTDockWidgetVisiblityChanged(bool)));
    dock_win_missed_trades->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_missed_trades->setMinimumSize(200,150);
    const auto autoHideContainer_missed_trade = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_missed_trades);
    autoHideContainer_missed_trade->setSize(480);

    //create a titlebar
    QWidget *mt_titlebar=new QWidget;
    mt_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *position_mt_layout=new QHBoxLayout(mt_titlebar);
    position_mt_layout->setSpacing(10);
   position_mt_layout->setContentsMargins(17,8,10,6);
    QLabel *mt_label=new QLabel("Missed Trades");
    QFont font_mt_label=mt_label->font();
    font_mt_label.setFamily("Work Sans");
    mt_label->setFont(font_mt_label);
    mt_label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
    QSpacerItem* mt_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_mt = new QLineEdit;
    line_edit_mt->setMaximumWidth(160);
    line_edit_mt->setStyleSheet(lineedit_dock_SS);
    QToolButton* mt_close=new QToolButton();
    connect(mt_close, &QToolButton::clicked, [=](){ dock_win_missed_trades->close(); });
    mt_close->setIcon(pixmapdock_mt_close);
    mt_close->setIconSize(QSize(14, 14));
    position_mt_layout->addWidget(mt_label);
    position_mt_layout->addSpacerItem(mt_spacer);
    position_mt_layout->addWidget(line_edit_mt);
    position_mt_layout->addWidget(mt_close);
    //dock_win_missed_trades->setTitleBarWidget(mt_titlebar);



    //subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_missed_trades);

    missed_trade_table = new QTableView(dock_win_missed_trades);
    missed_trade_table->setStyleSheet(tableview_SS);
    missed_trade_table->horizontalHeader()->setFixedHeight(32);
    missed_trade_table->horizontalHeader()->setFont(headerfont);
    missed_trade_table->setShowGrid(false);
    missed_trade_table->setAlternatingRowColors(true);

    missed_trade_model = new Missed_Trade_Table_Model();
    missed_trade_table->setModel(missed_trade_model);
    missed_trade_table->horizontalHeader()->setStretchLastSection(true);
    missed_trade_table->verticalHeader()->setVisible(false);
    missed_trade_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    missed_trade_table->setSelectionMode(QAbstractItemView::SingleSelection);

    dock_win_missed_trades->setWidget(missed_trade_table);
    missed_trade_table->show();
    /************Missed Trades ********************************/



    QFile styleFile(":/style/Style_Home.qss");
    styleFile.open(QFile::ReadOnly);
    QString mainstyleSheet = QLatin1String(styleFile.readAll());
    this->setStyleSheet(mainstyleSheet);

    //logo at the top left
    QPixmap pixmaplogo(":/Am Logomark - Standard.png");
    pixmaplogo = pixmaplogo.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmaplogo);



//    QPixmap pixmapright(":/right_arrow.png");
//    ui->toggle_Button->setIcon(pixmapright);

//    QPixmap pixmapLeft(":/left_arrow.png");
//    ui->toggle_Button->setIcon(pixmapLeft);
   // connect(ui->toggle_Button, SIGNAL(clicked()), this, SLOT(on_toggle_Button_clicked()));

    //stylesheet for summary window title labels
//    const char stylesheet_userinfo[] = "color: #2F3339;" "text-align: center;""font-size: 12px;""font-style: normal;""font-weight: bold;""line-height: normal;";
//    for (auto w : {ui->label_7, ui->label_9,ui->label_10,ui->label_8,ui->label_2,ui->label_20,ui->label_6,ui->label_16,ui->label_15,ui->label_14,ui->m2mlabel,ui->label_13}){
//        w->setStyleSheet(stylesheet_userinfo);
//        QFont font=w->font();
//        font.setFamily("Work Sans");
//        w->setFont(font);
//        w->setWordWrap(true);
//    }

//    ui->label_3->setText(QString::number(AlgoCount));
//    ui->label_25->setText(QString::number(OrderCount));

    ui->Algorithms_Close->setVisible(false);
    ui->OrderBook_Close->setVisible(false);
    ui->Positions_Close->setVisible(false);
    ui->Liners_Close->setVisible(false);
    ui->HP_Close->setVisible(false);
    ui->Templates_Close->setVisible(false);

//    ui->lineEditWatchSearch->setPlaceholderText("Search...");
//    ui->lineEditWatchSearch->setStyleSheet("font-family: 'Work Sans';"
//                                           "font-size: 12px;"
//                                           "font-weight: 400;"
//                                           );
    QPixmap pixmapmenu(":/menu.png");
    ui->close->setIcon(pixmapclose);
    ui->minimize->setIcon(pixmapminimize);
    ui->maximize->setIcon(pixmapmaximize);
    ui->menu->setIcon(pixmapmenu);

    //setting icons for top window widget
    QPixmap pixmapbuttonclose(":/button_close.png");
    ui->Algorithms_Close->setIcon(pixmapbuttonclose);
    ui->OrderBook_Close->setIcon(pixmapbuttonclose);
    ui->Positions_Close->setIcon(pixmapbuttonclose);
    ui->Liners_Close->setIcon(pixmapbuttonclose);
    ui->HP_Close->setIcon(pixmapbuttonclose);
    ui->MissedTrade_Close->setIcon(pixmapbuttonclose);
    ui->Templates_Close->setIcon(pixmapbuttonclose);

    const char stylesheet_params[] = "color: #000000 ;""font-size: 12px;""font-style: normal;""font-weight: bold;""line-height: normal;";
//    for (auto w : {ui->Open_Label, ui->High_Label,ui->Low_Label,ui->Volume_Label}){
//        w->setStyleSheet(stylesheet_params);
//        QFont font=w->font();
//        font.setFamily("Work Sans");
//        w->setFont(font);
//    }

    const char stylesheet_params_nos[] = "color: #4f3c7f ;""font-family: Work Sans;" "font-size: 14px;" "font-style: normal;" "font-weight: bold;""line-height: normal;";
//    for (auto w : {ui->Open_Num, ui->High_Num,ui->Low_Num,ui->Volume_Num}){
//        w->setStyleSheet(stylesheet_params_nos);
//        QFont font=w->font();
//        font.setFamily("Work Sans");
//        w->setFont(font);
//    }

//    QFont fontbuy=ui->Buy_Watch_Button->font();
//    QFont fontsell=ui->Sell_Watch_Button->font();
//    fontbuy.setFamily("Work Sans");
//    fontsell.setFamily("Work Sans");
//    ui->Buy_Watch_Button->setFont(fontbuy);
//    ui->Sell_Watch_Button->setFont(fontsell);

//    ui->listWidgetWatch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Hides vertical scroll bar
//    ui->listWidgetWatch->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Hides horizontal scroll bar



    // Apply the shadow effect to the widget
//    QSettings settings;
//    settings.setValue("DOCK_LOCATIONS",this->saveState(1));

//    this->restoreState(settings.value("DOCK_LOCATIONS").toByteArray(),1);

    new QShortcut(QKeySequence(Qt::Key_Delete), this,  SLOT(Delete_clicked_slot()));
    new QShortcut(QKeySequence(Qt::Key_F1), this,  SLOT(F1_clicked_slot()));
    new QShortcut(QKeySequence(Qt::Key_F2), this,  SLOT(F2_clicked_slot()));

    // db_conn =new mysql_conn(this,"main_db_conn");
    // connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));
   //loadContract();
    //initWatchWindow();
    // this should be called after all dock widget are set
    restoreDockManagerState();
}


MainWindow::~MainWindow()
{
    delete ui;
    if (sortWin) {
        delete sortWin; // Ensure the window is deleted
    }
}

void MainWindow::update_ui_slot(int type){

    switch (type) {
    case LOADED_MODEL:
        searchbar();
        break;

    default:
        break;
    }

}

void MainWindow::createINIFileIfNotExist(){

    //Create App Data folder if not exist
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    // Check if the directory already exists
    if (!dir.exists()) {
        // If it doesn't exist, create it
        if (dir.mkpath(".")) {
            // The directory was successfully created
            qDebug() << "App Data Directory created:" << appDataPath;
        } else {
            // There was an error creating the directory
            qDebug() << "Error creating directory:" << appDataPath;
        }
    }

    //create INI File if not exist
    if (!QFile(appDataPath + "/settings.ini").exists()){
        QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
        settings.beginGroup("DB_Details");
        settings.setValue("HostName", "localhost");
        settings.setValue("Port", "3306");
        settings.setValue("DatabaseName", "speedtrade");
        settings.setValue("UserName", "root");
        settings.setValue("Password", "Qwert@1234");
        settings.endGroup();
    }

    //if username already saved load it to form
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    if(!groups.contains("BackendServerDetails")){
        settings.beginGroup("BackendServerDetails");
        settings.setValue("ip","127.0.0.1");
        settings.setValue("port","6000");
        settings.endGroup();
    }
    else{
        // Check if keys "ip" and "port" exist, if not, create them
        settings.beginGroup("BackendServerDetails");
        if (!settings.contains("ip"))
            settings.setValue("ip", "127.0.0.1");
        if (!settings.contains("port"))
            settings.setValue("port", "6000");
        settings.endGroup();
    }
    if(!groups.contains("ExchangeServerDetails")){
        settings.beginGroup("ExchangeServerDetails");
        settings.setValue("fo_mult_ip","233.1.2.5");
        settings.setValue("fo_mult_port","34330");
        settings.setValue("cm_mult_ip","233.1.2.5");
        settings.setValue("cm_mult_port","34330");
        settings.endGroup();
    }
    else{
        settings.beginGroup("ExchangeServerDetails");
        if (!settings.contains("fo_mult_ip"))
            settings.setValue("fo_mult_ip", "233.1.2.5");
        if (!settings.contains("fo_mult_port"))
            settings.setValue("fo_mult_port", "34330");
        if (!settings.contains("cm_mult_ip"))
            settings.setValue("cm_mult_ip", "233.1.2.5");
        if (!settings.contains("cm_mult_port"))
            settings.setValue("cm_mult_port", "34330");
        settings.endGroup();
    }
    if(!groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        /*settings.setValue("devicer",10000000);
        settings.setValue("decimal_precision",4);
        settings.setValue("algo_table_price_diff_incrementer",0.0025);
        settings.setValue("algo_table_quantity_incrementer",1);*/
        settings.setValue("market_type","fo"); // or cds
        settings.setValue("InstrumentTypeFilter","");
        settings.setValue("Watch_InstrumentNames","");
        settings.endGroup();
    }
    else{
        settings.beginGroup("GeneralSettings");
        /*if (!settings.contains("devicer"))
            settings.setValue("devicer",10000000);
        if (!settings.contains("decimal_precision"))
            settings.setValue("decimal_precision",4);
        if (!settings.contains("algo_table_price_diff_incrementer"))
            settings.setValue("algo_table_price_diff_incrementer",0.0025);
        if (!settings.contains("algo_table_quantity_incrementer"))
            settings.setValue("algo_table_quantity_incrementer",1);*/

        if (!settings.contains("market_type"))
            settings.setValue("market_type","fo");
        if (!settings.contains("InstrumentTypeFilter"))
            settings.setValue("InstrumentTypeFilter","");
        if (!settings.contains("Watch_InstrumentNames"))
            settings.setValue("Watch_InstrumentNames","");
        settings.endGroup();
    }
    if(!groups.contains("BackendServerDetails")){
        settings.beginGroup("BackendServerDetails");
        settings.setValue("ip","192.168.15.63");
        settings.setValue("port",6000);
        settings.endGroup();
    }
    else{
        settings.beginGroup("BackendServerDetails");
        if (!settings.contains("ip"))
            settings.setValue("ip","192.168.15.63");
        if (!settings.contains("port"))
            settings.setValue("port",6000);
        settings.endGroup();
    }

    //Remeber me settings
    if(!groups.contains("AutoLoginSett")){
        settings.beginGroup("AutoLoginSett");
        settings.setValue("remember_me","0");
        settings.setValue("login_username","");
        settings.setValue("login_password","");
        settings.endGroup();
    }
    else{
        settings.beginGroup("AutoLoginSett");
        if (!settings.contains("remember_me"))
            settings.setValue("remember_me","0");
        if (!settings.contains("login_username"))
            settings.setValue("login_username","");
        if (!settings.contains("login_password"))
            settings.setValue("login_password","");
        settings.endGroup();
    }
}

void MainWindow::loadSettings(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if (settings.contains("market_type"))
            market_type = settings.value("market_type").toString();
//        if(settings.contains("Watch_InstrumentNames")){
//           QString watch=  settings.value("Watch_InstrumentNames").toString();
//            if(watch=="")
//               savedWatchItems.clear();
//            else
//                savedWatchItems = watch.split(";");
//        }
        settings.endGroup();
    }
    //currently hardcoded for FO
    devicer = FO_DEVICER;
    decimal_precision = FO_DECIMAL_PRECISION;
//    if(market_type=="fo"){
//        devicer = FO_DEVICER;
//        decimal_precision = FO_DECIMAL_PRECISION;
//    }
//    else{
//        devicer = CDS_DEVICER;
//        decimal_precision = CDS_DECIMAL_PRECISION;
//    }

}

void MainWindow::searchbar(){
//            QStandardItemModel* model = ContractDetail::getInstance().Get_model_searchInstrument_BOX_Leg1();
//            custom_q_completer *completerLeg1 = new custom_q_completer(this);
//            completerLeg1->setModel(model);
//            completerLeg1->setCaseSensitivity(Qt::CaseInsensitive);
//            completerLeg1->setCompletionMode(QCompleter::PopupCompletion);
//            completerLeg1->setModelSorting(QCompleter::CaseSensitivelySortedModel);
//            completerLeg1->setFilterMode(Qt::MatchContains);
//            QListView *viewLeg1 = (QListView *)completerLeg1->popup();
//            viewLeg1->setUniformItemSizes(true);
//            viewLeg1->setLayoutMode(QListView::Batched);
//            ui->lineEdit->setCompleter(completerLeg1);

}

void MainWindow::profolioTableEditFinshedSlot(QString valStr,QModelIndex index){

        if(portfolio_table_updating_db.loadRelaxed()==1){
            qDebug()<<"portfolio_table_updating_db----- in progress.";
                return;
        }

        portfolio_table_updating_db.storeRelaxed(1);
        QString PortfolioNumber = T_Portfolio_Model->index(index.row(),PortfolioData_Idx::_PortfolioNumber).data().toString();
        T_Portfolio_Model->portfolio_data_list[index.row()]->edting.storeRelaxed(0);

        //Update status data to DB
        if(index.column()==PortfolioData_Idx::_Status){
            if(valStr == "0"){
                QString Query = "UPDATE Portfolios SET Status='DisabledByUser' where PortfolioNumber="+PortfolioNumber;
                QString msg;
                bool success = db_conn->updateDB_Table(Query,msg);
                if(success){
                    db_conn->logToDB(QString("Disabled portfolio ["+PortfolioNumber+"]"));
                    //refresh sorting
                    reloadSortSettFlg.storeRelaxed(1);
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Update Status Failed");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(msg);
                    msgBox.exec();
                }
            }
            else{
                QString msg;
                QString Query = "UPDATE Portfolios SET Status='Active' where PortfolioNumber="+PortfolioNumber;
                bool success =  db_conn->updateDB_Table(Query,msg);

                if(success){
                    db_conn->logToDB(QString("Activated  portfolio ["+PortfolioNumber+"]"));
                    //refresh sorting
                    reloadSortSettFlg.storeRelaxed(1);
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Update Status Failed");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(msg);
                    msgBox.exec();
                }
            }
        }
        else if(index.column() == PortfolioData_Idx::_Alias) {
            // Skip checking for duplicate alias names

            QString Query = "UPDATE Portfolios SET Alias='" + valStr + "' WHERE PortfolioNumber=" + PortfolioNumber;
            QString msg;
            bool success = db_conn->updateDB_Table(Query, msg);

            if (success) {
                db_conn->logToDB(QString("Updated Alias to " + valStr + " [" + PortfolioNumber + "]"));
                // Refresh sorting
                reloadSortSettFlg.storeRelaxed(1);
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Update Alias Failed");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(msg);
                msgBox.exec();
            }
        }

        //Get edited cell data from portfolio table and update DB
        else{
            QHash<int, QString> editedCellData(T_Portfolio_Model->editingDataHash);
            QHash<int, QString>::const_iterator i;
            QStringList updateQueryList;
            QString logMsg = "";
            for (i = editedCellData.constBegin(); i != editedCellData.constEnd(); ++i) {
                int columnIdx = i.key();
                QString valStr = i.value();
                switch (columnIdx) {
                case PortfolioData_Idx::_OrderQuantity:{
                    int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
                    double val = valStr.toDouble();
                    val=val*lotSize;
                    double vfq = T_Portfolio_Model->portfolio_data_list[index.row()]->VolumeFreezeQty;



                    if(val > vfq){
                        QMessageBox msgBox;
                        msgBox.setText("Cannot update OrderQuantity");
                        msgBox.setText("OrderQuantity cannot be greater than VolumeFreezeQty "+QString::number(vfq));
                        msgBox.setIcon(QMessageBox::Warning);
                        msgBox.exec();
                    }
                    else{
                        //write
                        int val1 = valStr.toInt();
                        int BuyTotalQuantity = T_Portfolio_Model->portfolio_data_list[index.row()]->BuyTotalQuantity;
                        int SellTotalQuantity = T_Portfolio_Model->portfolio_data_list[index.row()]->SellTotalQuantity;
                        int qty = BuyTotalQuantity;
                        if(BuyTotalQuantity==0)
                            qty = SellTotalQuantity;
                        else if(SellTotalQuantity==0)
                            qty = BuyTotalQuantity;

                        else if(BuyTotalQuantity>SellTotalQuantity)
                            qty = SellTotalQuantity;
                        else
                            qty = BuyTotalQuantity;
                        //check the condtion
                        if(val1 >qty){
                            QMessageBox msgBox;
                            msgBox.setText("Cannot update OrderQuantity");
                            msgBox.setText("OrderQuantity cannot be greater than totalQty "+QString::number(qty));
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.exec();
                        }
                        else{
                            updateQueryList.append("OrderQuantity="+QString::number(val));
                            logMsg = logMsg+"OrderQuantity ["+QString::number(val1)+"], ";
                        }
                    }
                }

                    break;

                    case PortfolioData_Idx::_BuyTotalQuantity:{
                    double TQ = valStr.toDouble();
                    int bTTQ= T_Portfolio_Model->portfolio_data_list[index.row()]->BuyTradedQuantity;

                    QString InstrumentType  = ContractDetail::getInstance().GetInstrumentType(T_Portfolio_Model->portfolio_data_list[index.row()]->Leg1TokenNo,T_Portfolio_Model->portfolio_data_list[index.row()]->PortfolioType.toInt());


                    if (InstrumentType == "OPTIDX" || InstrumentType == "FUTIDX") {

                        if (TQ > userData.IDXOpenLimit+bTTQ) {
                            QMessageBox msgBox;
                            msgBox.setText("TQ should not be greater than IDXOpenLimit " + QString::number(userData.IDXOpenLimit));
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.exec();
                            break;
                        }
                    }
                    else if (InstrumentType == "OPTSTK" || InstrumentType == "FUTSTK") {

                        if (TQ > userData.STKOpenLimit+bTTQ) {
                            QMessageBox msgBox;
                            msgBox.setText("TQ should not be greater than STXOpenLimit " + QString::number(userData.STKOpenLimit));
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.exec();
                            break;
                        }
                    }

                    double TTQ = T_Portfolio_Model->portfolio_data_list[index.row()]->BuyTradedQuantity;

                    if (TQ < TTQ) {
                            QMessageBox msgBox;
                            msgBox.setText("Total Quantity cannot be less than Traded Quantity is " + QString::number(TTQ));
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.exec();

                        }
                        else {

                            double OrderQty = T_Portfolio_Model->portfolio_data_list[index.row()]->OrderQuantity;
                            if(TQ<OrderQty){
                               // show the error mesg here
                                QMessageBox msgBox;
                                msgBox.setText("TQ Should not be less than OrderQuantity is"+ QString::number(OrderQty));
                                msgBox.setIcon(QMessageBox::Warning);
                                msgBox.exec();
                            }
                            else{
                                int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
                                double val = valStr.toDouble();
                                val=val*lotSize;
                                if(T_Portfolio_Model->portfolio_data_list[index.row()]->OrderQuantity==0){
                                    updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                                }
                                updateQueryList.append("BuyTotalQuantity="+QString::number(val));
                                logMsg = logMsg+"BuyTotalQuantity ["+QString::number(val)+"], ";
                            }

                        }


                    }
                    break;

                    case PortfolioData_Idx::_SellTotalQuantity:{
                        double TQ = valStr.toDouble();
                       int sTTQ= T_Portfolio_Model->portfolio_data_list[index.row()]->SellTradedQuantity;
                        QString InstrumentType  = ContractDetail::getInstance().GetInstrumentType(T_Portfolio_Model->portfolio_data_list[index.row()]->Leg1TokenNo,T_Portfolio_Model->portfolio_data_list[index.row()]->PortfolioType.toInt());


                        if (InstrumentType == "OPTIDX" || InstrumentType == "FUTIDX") {

                            if (TQ > userData.IDXOpenLimit + sTTQ) {
                                QMessageBox msgBox;
                                msgBox.setText("TQ should not be greater than IDXOpenLimit " + QString::number(userData.IDXOpenLimit));
                                msgBox.setIcon(QMessageBox::Warning);
                                msgBox.exec();
                                break;
                            }
                        }
                        else if (InstrumentType == "OPTSTK" || InstrumentType == "FUTSTK") {

                            if (TQ > userData.STKOpenLimit + sTTQ) {
                                QMessageBox msgBox;
                                msgBox.setText("TQ should not be greater than STXOpenLimit " + QString::number(userData.STKOpenLimit));
                                msgBox.setIcon(QMessageBox::Warning);
                                msgBox.exec();
                                break;
                            }
                        }
                        double TTQ = T_Portfolio_Model->portfolio_data_list[index.row()]->SellTradedQuantity;

                        if (TQ < TTQ) {
                            QMessageBox msgBox;
                            msgBox.setText("Total Quantity cannot be less than Traded Quantity is " + QString::number(TTQ));
                            msgBox.setIcon(QMessageBox::Warning);
                            msgBox.exec();
                        }
                        else {
                            double OrderQty = T_Portfolio_Model->portfolio_data_list[index.row()]->OrderQuantity;
                            if(TQ<OrderQty){
                                // show the error mesg here
                                QMessageBox msgBox;
                                msgBox.setText("TQ Should not be less than OrderQuantity is"+ QString::number(OrderQty));
                                msgBox.setIcon(QMessageBox::Warning);
                                msgBox.exec();
                            }
                            else{
                                int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
                                double val = valStr.toDouble();
                                val=val*lotSize;
                                if(T_Portfolio_Model->portfolio_data_list[index.row()]->OrderQuantity==0){
                                    updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                                }
                                updateQueryList.append("SellTotalQuantity="+QString::number(val));
                                logMsg = logMsg+"SellTotalQuantity ["+QString::number(val)+"], ";
                            }

                        }
                    }
                    break;

                    case PortfolioData_Idx::_BuyPriceDifference:{
                        QString BuyPriceDifference = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->BuyPriceDifference*devicer,'f',decimal_precision);
                        updateQueryList.append("BuyPriceDifference="+BuyPriceDifference);
                        logMsg = logMsg+"BuyPriceDifference ["+BuyPriceDifference+"], ";

                    }
                    break;

                    case PortfolioData_Idx::_SellPriceDifference:{
                        QString SellPriceDifference = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->SellPriceDifference*devicer,'f',decimal_precision);
                        updateQueryList.append("SellPriceDifference="+SellPriceDifference);
                        logMsg = logMsg+"SellPriceDifference ["+SellPriceDifference+"], ";
                    }
                    break;

                    default:{

                    }
                    break;

            }
            }

            T_Portfolio_Model->editingDataHash.clear();
            if(updateQueryList.size()>0){
                QString msg;
                QString Query = "UPDATE Portfolios SET " +
                                updateQueryList.join(", ") +
                                " WHERE PortfolioNumber=" + PortfolioNumber;
                bool success = db_conn->updateDB_Table(Query,msg);
                if(success){
                    db_conn->logToDB(logMsg+ " updated for PortfolioNumber="+PortfolioNumber);
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Update DB Failed");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(msg);
                    msgBox.exec();
                }
            }

        }

            //send notifcation to backend server
            quint16 command = NOTIFICATION_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
            const unsigned char dataBytes[] = { 0xFF, 0xFF };
            QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 2);
            QByteArray msg = backend_comm->createPacket(command, data);
            backend_comm->insertData(msg);

            portfolio_table_updating_db.storeRelaxed(0);

}
void MainWindow::loadContract(){

    auto loadContract_BackgroundTask = [this]() {

        QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.0;'>"
                              "<span>" + QTime::currentTime().toString("hh:mm:ss")
                              + "&nbsp;</span><span style='font-weight:400;color: black;'> file loading... </span></p>";

        emit display_log_text_signal(htmlContent);

        ContractDetail::getInstance().ReloadContractDetails(userData);
        contractDetailsLoaded.storeRelaxed(1);

        //do the UI thread function inside here
        //        QMetaObject::invokeMethod(this, [this]() {
        //            //ui->stackedWidgetMain->setCurrentIndex(1);
        //            stackViewSwithAnimation(0,1,ui->stackedWidgetMain);
        //        });
        // Rest of your background task code...
        //if not logged in the data loading thread start by login function

        htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.0;'>"
                      "<span>" + QTime::currentTime().toString("hh:mm:ss")
                      + "&nbsp;</span><span style='font-weight:400;color: black;'> file loaded </span></p>";


        emit display_log_text_signal(htmlContent);

        // to hide progress bar winodw
        emit signalHideProgressBar();

        foo_token_number_start_strike = "";

        if(loggedInFlg.loadRelaxed()==1){
            start_dataLoadingThread();
        }
    };
    emit update_ui_signal(LOADED_MODEL);
    QFuture<void> future = QtConcurrent::run(loadContract_BackgroundTask);
}

void MainWindow::start_dataLoadingThread(){

    futureBackGroundDBThread = QtConcurrent::run(&MainWindow::refreshTableIn_Intervel,this);

}

void MainWindow::stop_dataLoadingThread(){

    quit_db_data_load_thread.storeRelaxed(1); // this will quit the data loding thread.
    futureBackGroundDBThread.waitForFinished();
}

void MainWindow::refreshTableIn_Intervel(){
    qDebug()<<"Started data loading thread.....";
    data_loading_thread_running.storeRelaxed(1);

    int data_load_intervell = 1000;// in m-sec
    while(1){
        QElapsedTimer timer;
        timer.start();

        //check algos are deleting if so do not load.
        if(deletingPortFolioFlg.loadRelaxed()==0){
            loadDataAndUpdateTable(T_Table::PORTFOLIO);
        }
        loadDataAndUpdateTable(T_Table::TRADE);
        loadDataAndUpdateTable(T_Table::NET_POS);
        loadDataAndUpdateTable(T_Table::SUMMARY);
        loadDataAndUpdateTable(T_Table::MISSED_TRADE);


        //loadMysQLData(Table::ORDER);*/

        auto elapsed = timer.elapsed();
        if( timer.elapsed()<data_load_intervell){
            //  qDebug()<<"sleeping... "<<data_load_intervell-elapsed;

            QThread::msleep(data_load_intervell-elapsed);
        }


        if(quit_db_data_load_thread.loadRelaxed()==1)
            break;
        //break;// this need to be removed
    }
    qDebug()<<"Exiting data loading thread.....";
    data_loading_thread_running.storeRelaxed(0);


    //
    //QTimer::singleShot(2000, this, &MainWindow::refreshTableIn_Intervel);
}

void MainWindow::start_slowdata_worker()
{
    // SlowData& slowData = SlowData::getInstance();
    //  slowData.startSlowDataSocket();
    slowData = new SlowData();
    slowData->startSlowDataSocket();
}

void MainWindow::stop_slowdata_worker(){
    slowData->stopSlowDataSocket();
}

void MainWindow::start_slowdata_indices_worker()
{
    slowDataIndices = new SlowDataIndices();
    QObject::connect(slowDataIndices, SIGNAL(indicesDataSignal(Indices_Data_Struct)), this, SLOT(indicesDataRecv_Slot(Indices_Data_Struct)));
    slowDataIndices->startSlowDataSocket();
}

void MainWindow::stop_slowdata_indices_worker(){
    slowDataIndices->stopSlowDataSocket();
}

void MainWindow::loadDataAndUpdateTable(int table){
    switch (table) {
    case T_Table::PORTFOLIO:{
        QStringList TradedPortFolioList =  trade_model->getTradedPortFolioList();
        QStringList TradedHighlight_ExcludeList = T_Portfolio_Model->getTradedHighlight_ExcludeList();
        for (const QString &item : TradedHighlight_ExcludeList) {
            TradedPortFolioList.removeAll(item);
        }

        db_conn->getPortfoliosTableData(reloadSortSettFlg,AlgoCount,T_Portfolio_Model,combined_tracker_model,averagePriceList,QString::number(userData.UserId),TradedPortFolioList);
        emit data_loded_signal(T_Table::PORTFOLIO);
        updateSummaryLabels();
        break;
       }

    case T_Table::SUMMARY:
        db_conn->getSummaryTableData(OrderCount,QString::number(userData.UserId) );  // Correctly get the count
        emit data_loded_signal(T_Table::SUMMARY);
        updateSummaryLabels();
        break;

    case T_Table::TRADE:{
//        OutputDebugStringA("Test1 \n");
        QHash<QString, PortFolioData_Less> PortFolioHash = T_Portfolio_Model->getPortFolioDataLess();
        db_conn->getTradeTableData(TraderCount,trade_model,liners_model,QString::number(userData.UserId),PortFolioHash);
        emit data_loded_signal(T_Table::TRADE);
        updateSummaryLabels();
        break;
    }
   /* case T_Table::Liners:{
         QHash<QString, PortFolioData_Less> PortFolioTypeHash = T_Portfolio_Model->getPortFolioDataLess();
        db_conn->getLinersTableData(liners_model,QString::number(userData.UserId), PortFolioTypeHash);
         db_conn->getTradeTableData(trade_model,QString::number(userData.UserId),PortFolioTypeHash);
         emit data_loded_signal(T_Table::Liners);
         break;
    }*/

    case T_Table::NET_POS:{
        QHash<QString,int> PortFoliosLotSizeHash = T_Portfolio_Model->getPortFoliosLotSize();
        db_conn->getNetPosTableData(BuyValue,SellValue,Profit,BuyQty_summary,SellQty_summary,NetQty,net_pos_model,QString::number(userData.UserId),PortFoliosLotSizeHash);
        emit data_loded_signal(T_Table::NET_POS);
        updateSummaryLabels();
        break;
    }

    case T_Table::MISSED_TRADE:{
        db_conn->getMissedTradeData(missed_trade_model,QString::number(userData.UserId));
        emit data_loded_signal(T_Table::NET_POS);
        break;
    }



    default:
        break;
    }
}
QString MainWindow::fixDecimal(double num,int decimal_precision){
    QString str = QString::number(num, 'f', decimal_precision+1);
    return str;
}
void MainWindow::updateSummaryLabels()
{
    QStringList summarydatList;
    summarydatList.append(QString::number(AlgoCount));
    summarydatList.append(QString::number(OrderCount));
    summarydatList.append(QString::number(TraderCount));
    summarydatList.append(QString::number(BuyValue));
    summarydatList.append(QString::number(SellValue));
    summarydatList.append(QString::number(Profit));
    summarydatList.append(QString::number(BuyQty_summary));
    summarydatList.append(QString::number(SellQty_summary));
    summarydatList.append(QString::number(NetQty));

    data_summary_update_signal(summarydatList); // Call the method directly




//    ui->label_3->setText(QString::number(AlgoCount));
//    ui->label_3->setAlignment(Qt::AlignCenter);

//    ui->label_25->setText(QString::number(OrderCount));
//    ui->label_25->setAlignment(Qt::AlignCenter);

//    ui->label_30->setText(QString::number(TraderCount));
//    ui->label_30->setAlignment(Qt::AlignCenter);

//    int decimal_precision = 1;
//    ui->label_23->setText(fixDecimal(BuyValue, decimal_precision));
//    ui->label_23->setAlignment(Qt::AlignCenter);

//    ui->label_26->setText(fixDecimal(SellValue, decimal_precision));
//    ui->label_26->setAlignment(Qt::AlignCenter);

//    ui->label_31->setText(fixDecimal(Profit, decimal_precision));
//    ui->label_31->setAlignment(Qt::AlignCenter);

//    ui->label_24->setText(QString::number(BuyQty_summary));
//    ui->label_24->setAlignment(Qt::AlignCenter);

//    ui->label_27->setText(QString::number(SellQty_summary));
//    ui->label_27->setAlignment(Qt::AlignCenter);

//    ui->label_32->setText(QString::number(NetQty));
//    ui->label_32->setAlignment(Qt::AlignCenter);

//    ui->label_33->setText("-");
//    ui->label_33->setAlignment(Qt::AlignCenter);

//    ui->label_28->setText("-");
//    ui->label_28->setAlignment(Qt::AlignCenter);

//    ui->label_34->setText("-");
//    ui->label_34->setAlignment(Qt::AlignCenter);
}

void MainWindow::start_backend_comm_socket_worker()
{
    backend_comm = new BackendComm();
    backend_comm->UserId = userData.UserId;
    backend_comm_thread= new QThread();
    backend_comm->moveToThreadAndNotify(backend_comm_thread);
    connect(backend_comm,SIGNAL(socketData(QString,SocketDataType)),this,SLOT(backend_comm_Data_Slot(QString,SocketDataType)));
    backend_comm_thread->start();
}


void MainWindow::backend_comm_Data_Slot(QString msg,SocketDataType msgType){

   // ui->textEdit->append(msg);
    //log_to_file logF(0,logfileName);
    //logF.log(data);
#ifdef ENABLE_BACKEND_DEBUG_MSG
    qDebug()<<"Backend  Comm Data: "<<msg;
#endif

    if((msgType == SocketDataType::BACKEND_COMM_SOCKET_WARNING)||msgType == SocketDataType::BACKEND_COMM_SOCKET_ERROR){
#ifdef ENABLE_BACKEND_DEBUG_MSG
        qDebug()<<"Backend Data: Backend Socket Error"<<msg;
#endif
 ui->label_3->setText("Connection Error");
        ui->label_3->setStyleSheet("color: red;");
    }
    //ui->toolButton_BackendServer->setStyleSheet("background-color: rgb(255, 32, 36);border-radius:6px;color:#000;");
    else if(msgType == SocketDataType::BACKEND_COMM_SOCKET_CONNECTED){
#ifdef ENABLE_BACKEND_DEBUG_MSG
        qDebug()<<"Backend Data: Backend Socket Connected"<<msg;
#endif
 ui->label_3->setText("Connected");
          ui->label_3->setStyleSheet("color: green;");
    }
    // ui->toolButton_BackendServer->setStyleSheet("background-color: rgb(94, 255, 107);border-radius:6px;color:#000;");
}
void MainWindow::stop_backend_comm_socket_worker(){
    //delete socket_Conn;
    backend_comm->quitDataFetchThread();
    backend_comm_thread->deleteLater();
}

QString MainWindow::base64_encode(QString str){
    QByteArray ba;
    ba.append(str.toStdString().c_str());
    return ba.toBase64();
}

QString MainWindow::base64_decode(QString str){
    QByteArray ba;
    ba.append(str.toStdString().c_str());
    return QByteArray::fromBase64(ba);
}


void MainWindow::updatePortFolioStatus(){

    if(portfolio_table_updating_db.loadRelaxed()==1){
        qDebug()<<"portfolio_table_updating_db----- in progress.";
        return;
    }


    portfolio_table_updating_db.storeRelaxed(1);

    QModelIndexList selection = T_Portfolio_Table->selectionModel()->selectedRows();
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);


            QString PortfolioNumber = T_Portfolio_Model->index(index.row(),PortfolioData_Idx::_PortfolioNumber).data().toString();
            T_Portfolio_Model->portfolio_data_list[index.row()]->edting.storeRelaxed(1);
            portfolio_status status = static_cast<portfolio_status>(T_Portfolio_Model->portfolio_data_list[index.row()]->StatusVal.toInt());//T_Portfolio_Model->index(index.row(),PortfolioData_Idx::_Status).data().toString();
            //status is in uncheck state so make it toggle(checked)
            if(status == portfolio_status::Filled || status == portfolio_status::DisabledByUser){
                int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
                //check remaing qunatity is greater than 1 lot
                    T_Portfolio_Model->portfolio_data_list[index.row()]->StatusVal = QString::number(portfolio_status::Active);
                    T_Portfolio_Model->refreshTable();
                    QString Query = "UPDATE Portfolios SET Status='Active' where PortfolioNumber="+PortfolioNumber;
                    QString msg;
                    bool success = db_conn->updateDB_Table(Query,msg);
                    if(success){
                        //refresh sorting
                        reloadSortSettFlg.storeRelaxed(1);
                        db_conn->logToDB(QString("Activated portfolio ["+PortfolioNumber+"]"));
                        quint16 command = NOTIFICATION_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
                        const unsigned char dataBytes[] = { 0x01};
                        QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
                        QByteArray msg = backend_comm->createPacket(command, data);
                        backend_comm->insertData(msg);
                    }
                }


            //status is in check state so make it toggle(uncheck)
            else{
                T_Portfolio_Model->portfolio_data_list[index.row()]->StatusVal = QString::number(portfolio_status::DisabledByUser);
                T_Portfolio_Model->refreshTable();
                QString Query = "UPDATE Portfolios SET Status='DisabledByUser' where PortfolioNumber="+PortfolioNumber;
                QString msg;
                bool success = db_conn->updateDB_Table(Query,msg);
                if(success){
                    //refresh sorting
                    reloadSortSettFlg.storeRelaxed(1);
                    db_conn->logToDB(QString("Disabled portfolio ["+PortfolioNumber+"]"));

                    quint16 command = NOTIFICATION_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
                    const unsigned char dataBytes[] = { 0x00};
                    QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
                    QByteArray msg = backend_comm->createPacket(command, data);
                    backend_comm->insertData(msg);
                }
            }






        T_Portfolio_Model->portfolio_data_list[index.row()]->edting.storeRelaxed(0);

    }
    portfolio_table_updating_db.storeRelaxed(0);
}

void MainWindow::resizePortFolioTableColWidthSlot(int width)
{
    T_Portfolio_Table->setColumnWidth(PortfolioData_Idx::_AlgoName,width);
}

//void MainWindow::tradeTableSerachNext(){
//    currentIDxSearch++;
//    searchTradeTable();
//}

void MainWindow::loggedIn(){
    loggedInFlg.storeRelaxed(1);

    //make sure the start_slowdata_worker in main thread
    QMetaObject::invokeMethod(this, [this]() {
       // ui->label_17->setText(userData.UserName+ "  " +"Summary");
        QString formattedText =  userData.UserName + " <span style=\"font-weight:normal;\">Summary</span>";
      //  ui->label_17->setText(formattedText);

        start_slowdata_worker();
        start_slowdata_indices_worker();
        start_backend_comm_socket_worker();
        //start data loading thread only if all contract table loaded, else it will loaded from contract table loading thread
        if(contractDetailsLoaded.loadRelaxed()==1)
            start_dataLoadingThread();
    });

}

void MainWindow::loadCurrentDayLogs()
{
    db_conn->loadCurrentDayLogs();
}

void MainWindow::loggedInSucessful(userInfo userData)
{
    loadingDataWinodw->move((width() - loadingDataWinodw->width()) / 2 + pos().x(),
                  (height() - loadingDataWinodw->height()) / 2 + pos().y());

    loadingDataWinodw->show();

    //loading data
    loadCurrentDayLogs();
    loadContract();

}

void MainWindow::loggedOut(){



    loggedInFlg.storeRelaxed(0);
    stop_slowdata_worker();
    stop_slowdata_indices_worker();
    stop_dataLoadingThread();
    stop_backend_comm_socket_worker();
    emit logoutRequested();


}

void MainWindow::add_logs(QString str){
   // ui->textEdit->append(str);
    logsdata.append(str);
    emit logDataSignal(logsdata);
     ui->label_5->setText(str);

}

void MainWindow::on_OrderBook_Button_clicked()
{
    //8
    //dock_win_trade->show();
    dock_win_trade->toggleView(true);

    ui->OrderBook_Widget->setStyleSheet(stylesheetvis);
    ui->OrderBook_Close->setVisible(true);
}


void MainWindow::on_OrderBook_Close_clicked()
{
    //8
    dock_win_trade->toggleView(false);
    ui->OrderBook_Widget->setStyleSheet("");
    ui->OrderBook_Close->setVisible(false);
}


void MainWindow::on_Positions_Button_clicked()
{
    //9
    dock_win_net_pos->toggleView(true);

    ui->Positions_Widget->setStyleSheet(stylesheetvis);
    ui->Positions_Close->setVisible(true);
}


void MainWindow::on_Positions_Close_clicked()
{
    //9
    dock_win_net_pos->toggleView(false);
    ui->Positions_Widget->setStyleSheet("");
    ui->Positions_Close->setVisible(false);
}
void MainWindow::on_Liners_Button_clicked()
{
    //10
    dock_win_liners->toggleView(true);
    ui->Liners_Widget->setStyleSheet(stylesheetvis);
    ui->Liners_Close->setVisible(true);
}




void MainWindow::on_Liners_Close_clicked()
{
    //10
    dock_win_liners->toggleView(false);
    ui->Liners_Widget->setStyleSheet("");
    ui->Liners_Close->setVisible(false);
}


void MainWindow::on_HP_Button_clicked()
{
    //11
    dock_win_combined_tracker->toggleView(true);
    ui->HP_Widget->setStyleSheet(stylesheetvis);
    ui->HP_Close->setVisible(true);
}


void MainWindow::on_HP_Close_clicked()
{
    //11
    dock_win_combined_tracker->toggleView(false);
    ui->HP_Widget->setStyleSheet("");
    ui->HP_Close->setVisible(false);
}


void MainWindow::on_MissedTrade_Button_clicked()
{
    dock_win_missed_trades->toggleView(true);
    ui->MissedTrade_widget->setStyleSheet(stylesheetvis);
    ui->MissedTrade_Close->setVisible(true);
}

void MainWindow::on_MissedTrade_Close_clicked()
{
    dock_win_missed_trades->toggleView(false);
    ui->MissedTrade_widget->setStyleSheet("");
    ui->MissedTrade_Close->setVisible(false);
}

void MainWindow::on_Templates_Button_clicked()
{
    //12
    ui->Templates_Widget->setStyleSheet(stylesheetvis);
    ui->Templates_Close->setVisible(true);
}


void MainWindow::on_Templates_Close_clicked()
{
    //12
    ui->Templates_Widget->setStyleSheet("");
    ui->Templates_Close->setVisible(false);
}

void MainWindow::on_close_clicked()
{
    db_conn->logToDB("Logged Out");
    //loggedOut();
    //portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser;
    close();
}


void MainWindow::on_minimize_clicked()
{
    showMinimized();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child!=ui->widget_6)
    {
        return;
    }
    pressed = true;
    position = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if(pressed){
        if (event->buttons() == Qt::LeftButton) {
            if (position.x() != 0 || position.y() != 0) {
                move(event->globalPosition().x() - position.x(), event->globalPosition().y() - position.y());
            }
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child!=ui->widget_6)
    {
        return;
    }
    pressed = false;
}

void MainWindow::on_maximize_clicked()
{
        if(isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
        Q_UNUSED(event);
        if (!ui->widget_6->underMouse())
            return;

        if(isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
}


void MainWindow::on_startall_Button_clicked()
{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to activate all selected algos?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }

        QStringList portfolioNumbers;
        for(int i = 0; i < T_Portfolio_Model->portfolio_data_list.size(); i++){
            if (T_Portfolio_Model->portfolio_data_list[i]) {
            portfolioNumbers.append(QString::number(T_Portfolio_Model->portfolio_data_list[i]->PortfolioNumber));
            }
        }
        QString joinedPortfolioNumbers = portfolioNumbers.join(", ");
        QString Query = "UPDATE Portfolios SET Status='Active' WHERE PortfolioNumber IN (" + joinedPortfolioNumbers + ")";
        QString msg;

        bool success =  db_conn->updateDB_Table(Query,msg);
        if(success){
            db_conn->logToDB(QString("Enabled Algos [" + joinedPortfolioNumbers + "]"));
        }


}
void MainWindow::on_stopall_Button_clicked()
{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to Disable all selected algos?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }

        QStringList portfolioNumbers;
        for(int i=0;i<T_Portfolio_Model->portfolio_data_list.size();i++){
            if (T_Portfolio_Model->portfolio_data_list[i]) {
            portfolioNumbers.append(QString::number(T_Portfolio_Model->portfolio_data_list[i]->PortfolioNumber));
            }
        }
        QString joinedPortfolioNumbers = portfolioNumbers.join(", ");
        QString Query = "UPDATE Portfolios SET Status='DisabledByUser' WHERE PortfolioNumber IN (" + joinedPortfolioNumbers + ")";

        QString msg;
        bool success =  db_conn->updateDB_Table(Query,msg);
        if(success){
            db_conn->logToDB(QString("Disabled  Algos ["+joinedPortfolioNumbers+"]"));
        }
}


void MainWindow::OnAlgorithmDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->Algorithms_Widget->setStyleSheet(stylesheetvis);
    else
        ui->Algorithms_Widget->setStyleSheet("");
    ui->Algorithms_Close->setVisible(p_Visible);
}

void MainWindow::OnOrderBookDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->OrderBook_Widget->setStyleSheet(stylesheetvis);
    else
        ui->OrderBook_Widget->setStyleSheet("");
    ui->OrderBook_Close->setVisible(p_Visible);
}

void MainWindow::OnPositionsDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->Positions_Widget->setStyleSheet(stylesheetvis);
    else
        ui->Positions_Widget->setStyleSheet("");
    ui->Positions_Close->setVisible(p_Visible);
}
void MainWindow::OnLinersDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->Liners_Widget->setStyleSheet(stylesheetvis);
    else
        ui->Liners_Widget->setStyleSheet("");
    ui->Liners_Close->setVisible(p_Visible);
}

void MainWindow::OnHPDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->HP_Widget->setStyleSheet(stylesheetvis);
    else
        ui->HP_Widget->setStyleSheet("");
    ui->HP_Close->setVisible(p_Visible);
}

void MainWindow::OnMTDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->MissedTrade_widget->setStyleSheet(stylesheetvis);
    else
        ui->MissedTrade_widget->setStyleSheet("");
    ui->MissedTrade_Close->setVisible(p_Visible);
}


void MainWindow::on_Algorithms_Button_clicked()
{
    T_Portfolio_DockWin->toggleView(true);
    //7
    ui->Algorithms_Widget->setStyleSheet(stylesheetvis);
    ui->Algorithms_Close->setVisible(true);
}


void MainWindow::on_Algorithms_Close_clicked()
{
    T_Portfolio_DockWin->toggleView(false);
    ui->Algorithms_Widget->setStyleSheet("");
    ui->Algorithms_Close->setVisible(false);
}
void MainWindow::on_ConvertAlgo_button_clicked(){

    if(!convertalgo){
        convertalgo=new ConvertAlgo_Win(this);
        qDebug()<<"Creating convertalgo....";
    }

    convertalgo->update_contract_tableData(QString::number(userData.UserId),userData.MaxPortfolioCount);
    if(!convertalgo->isVisible())
    convertalgo->show();
}
void MainWindow::Delete_clicked_slot()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Portfolio From Database?", "Click Yes to Continue.",  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QModelIndexList selection = T_Portfolio_Table->selectionModel()->selectedRows();
        if(selection.count()==0)
        {
            QMessageBox msgBox;
            msgBox.setText("Please select a Portfolio from Portfolio Table.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;
        }

        // Multiple rows can be selected
        QString logs;
        QStringList activeAlgoList;
        deletingPortFolioFlg.storeRelaxed(1);
        for(int i=0; i< selection.count(); i++)
        {
            QModelIndex index = selection.at(i);
            QString PortfolioNumber = T_Portfolio_Model->index(index.row(),PortfolioData_Idx::_PortfolioNumber).data().toString();

            if( T_Portfolio_Model->portfolio_data_list[index.row()]->Status == true)
            {
                logs = "PortfolioNumber '"+PortfolioNumber+"' is Active, cannot delete.";
                activeAlgoList.append(PortfolioNumber);
                db_conn->logToDB(logs);
                continue;
            }

            QString msg;
            bool ret = db_conn->deleteAlgo(PortfolioNumber, msg);
            if(ret==true)
            {
                logs = "PortfolioNumber '"+PortfolioNumber+"' deleted successfully.";
                db_conn->logToDB(logs);
            }
            else
            {
                logs = "PortfolioNumber '"+PortfolioNumber+"' not Deleted, "+msg;
                db_conn->logToDB(logs);
                qDebug()<<"PortfolioNumber: "<<PortfolioNumber<<" not Deleted, Info:"<<msg;
            }

        }
        deletingPortFolioFlg.storeRelaxed(0);


        if(!activeAlgoList.empty())
        {
            QMessageBox msgBox;
            msgBox.setText("Active algo cannot be deleted,\n disable the algo No. " + activeAlgoList.join(",") +" and then try again.\n");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
    }
}

void MainWindow::T_Portfolio_Table_cellDoubleClicked(const QModelIndex &index){
    //int row = index.row();
    int col = index.column();
    if(col==PortfolioData_Idx::_AlgoName){
        if(orderPopUpWin->isHidden())
            orderPopUpWin->show();
        // QString Expiry = T_Portfolio_Model->portfolio_data_list[index.row()]->Expiry;
        QString PortfolioType =T_Portfolio_Model->portfolio_data_list[index.row()]->PortfolioType;
        QString PortfolioNumber = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->PortfolioNumber);
        orderPopUpWin->getTradeDataFromDB(QString::number(userData.UserId), PortfolioNumber,PortfolioType);
        orderPopUpWin->setData(T_Portfolio_Model->portfolio_data_list[index.row()]);
        orderPopUpWin->activateWindow();
    }

}


void MainWindow::trade_table_cellDoubleClicked(const QModelIndex &index){
    //int row = index.row();
    int col = index.column();
    if(col==OrderBook_Idx::AlgoName_OB){
        if(tradePopUpWin->isHidden())
            tradePopUpWin->show();
       // QString Expiry = T_Portfolio_Model->portfolio_data_list[index.row()]->Expiry;
        QStringList data = trade_model->getTradedDataForIdx(index.row());
        QString localOrderID = data[OrderBook_Idx::TraderData_OB];
        int LotSize = data[OrderBook_Idx::LotSize_OB].toInt();

        tradePopUpWin->getTradeDataFromDB(QString::number(userData.UserId), localOrderID,LotSize);
        tradePopUpWin->activateWindow();
    }

}



void MainWindow::T_Portfolio_Table_cellClicked(const QItemSelection &selected, const QItemSelection &deselected){

    QModelIndexList selectedIndexes = selected.indexes();
    QList<int> selectedRows;

    for (const QModelIndex &index : selectedIndexes)
    {
        if (!selectedRows.contains(index.row()))
        {
            selectedRows.append(index.row());
        }
    }
    if(selectedRows.size()==1){
        int row = selectedRows[0];
        //current editing row is not same as selected row  so make last editing row uneditable
        if(portfolio_table_slected_idx_for_editing.load()!=-1&&portfolio_table_slected_idx_for_editing.load()!=row){
            T_Portfolio_Model->portfolio_data_list[portfolio_table_slected_idx_for_editing.load()]->edting.storeRelaxed(0); // maked the row flg not editing, so the bg data will be updated.
            portfolio_table_slected_idx_for_editing.store(-1);
        }
    }
    //multiple row selected or no row selected, so make last editing row uneditable
    else if(portfolio_table_slected_idx_for_editing.load()!=-1){
        T_Portfolio_Model->portfolio_data_list[portfolio_table_slected_idx_for_editing.load()]->edting.storeRelaxed(0); // maked the row flg not editing, so the bg data will be updated.
        portfolio_table_slected_idx_for_editing.store(-1);
    }

}

void MainWindow::slotAddLogForAddAlgoRecord(QString str)
{
    // To show the latest log at top
//    htmlLogsContent.prepend(str);
//     ui->textEdit->setText(htmlLogsContent);
   //  emit logDataSignal(str);
    logsdata.append(str);
    emit logDataSignal(logsdata);
    ui->label_5->setText(str);
}

void MainWindow::slotHideProgressBar()
{
   loadingDataWinodw->hide();
}

/*********************Watch data section*************************/

//QDataStream &operator<<(QDataStream &out, const Indices_Data_Struct &data) {
//    out << data.indexName << data.indexValue << data.change << data.percentagechange
//        << data.netChangeIndicator << data.openingIdx << data.closingIdx
//        << data.highIndexValue << data.lowIndexValue << data.marketCapitialisation
//        << data.display_widget_idx;
//    return out;
//}

//QDataStream &operator>>(QDataStream &in, Indices_Data_Struct &data) {
//    in >> data.indexName >> data.indexValue >> data.change >> data.percentagechange
//        >> data.netChangeIndicator >> data.openingIdx >> data.closingIdx
//        >> data.highIndexValue >> data.lowIndexValue >> data.marketCapitialisation
//        >> data.display_widget_idx;
//    return in;
//}
//void MainWindow::initWatchWindow(){
//    loadIndicesDataListFromFile(indicesDataList);
//    showSaveWatchOnListView();

//}


//void MainWindow::saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList) {
//    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    QString fileName = appDataPath+"/Data/watch_cache.bin";
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly)) {
//        qWarning("Could not open file for writing.");
//        return;
//    }
//    QDataStream out(&file);
//    out << indicesDataList;
//    file.close();
//}

//void MainWindow::loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList) {
//    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    QString fileName = appDataPath+"/Data/watch_cache.bin";
//    QFile file(fileName);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qWarning("Could not open file for reading--> watch_cache.bin");
//        return;
//    }
//    QDataStream in(&file);
//    in >> indicesDataList;
//    file.close();
//}


void MainWindow::indicesDataRecv_Slot(Indices_Data_Struct data){
    emit indicesDataRecv_Signal_watch_card(data);

    //remove below code
//    bool newIndicesData = false;
//    indicesDataMutex.lock();
//    if(!indicesDataList.contains(data.indexName)){
//        indicesDataList.insert(data.indexName,data);
//        newIndicesData = true;
//    }
//    //check any of the value changed
//    else if((indicesDataList[data.indexName].indexValue!=data.indexValue) || (indicesDataList[data.indexName].change!=data.change) || (indicesDataList[data.indexName].percentagechange!=data.percentagechange)){
//        indicesDataList[data.indexName]=data;
//        newIndicesData = true;
//    }

//    indicesDataMutex.unlock();
//   // qDebug()<<"indicesDataRecv_Slot: " <<data.indexName;

//    if(newIndicesData==true&&ui->lineEditWatchSearch->text()=="")
//        showSaveWatchOnListView();

//    if(newIndicesData==true){

//        QListWidgetItem *item = ui->listWidgetWatch->currentItem();
//        if (item) {
//            watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(item));
//            if (widget) {
//                updateSelecteWatch_UI( widget->data);
//            }
//        }

//        saveIndicesDataListToFile(indicesDataList);

//    }

}
//void MainWindow::on_lineEditWatchSearch_textChanged(const QString &text)
//{
//    if(text==""){
//        showSaveWatchOnListView();
//        return;
//    }

//    indicesDataMutex.lock();

//    QHash<QString, Indices_Data_Struct> indicesDataListTmp = indicesDataList;
//    indicesDataListTmp.detach();
//    indicesDataMutex.unlock();


//    /*QStringList nameList;
//    nameList.append("Nifty50");
//    nameList.append("BankNifty");
//    nameList.append("FinNifty");
//    nameList.append("SmallCap");
//    nameList.append("ABC");
//    nameList.append("DDD");
//    nameList.append("ee");
//    nameList.append("dd");
//    nameList.append("ABC");
//    nameList.append("de");
//    nameList.append("ddd");
//    nameList.append("wsd");
//    nameList.append("dfe");
//    nameList.append("fg");
//    nameList.append("gf");
//    nameList.append("rt");
//    nameList.append("fg");*/


//    QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
//    QStringList filteredList;

//    for (auto it = indicesDataListTmp.constBegin(); it != indicesDataListTmp.constEnd(); ++it) {
//        const QString &key = it.key();
//        if (regex.match(key).hasMatch()) {
//            filteredList.append(key);
//        }
//       // const Indices_Data_Struct &value = it.value();
//    }

//    /*for(const QString &name : nameList) {
//        if (regex.match(name).hasMatch()) {
//            filteredList.append(name);
//        }
//    }*/

//    ui->listWidgetWatch->clear();
//    int selecteIDx = -1;
//    for(int i=0;i<filteredList.size();i++){
//        auto item = new QListWidgetItem();
//        auto widget1 = new watch_Data_List_Item(this);
//        Indices_Data_Struct data = indicesDataListTmp[filteredList[i]];
//        widget1->setData(data);
//        item->setSizeHint(widget1->sizeHint());

//        ui->listWidgetWatch->addItem(item);
//        ui->listWidgetWatch->setItemWidget(item, widget1);
//        if(data.indexName==watchItemSelectedindexName)
//            selecteIDx = ui->listWidgetWatch->count()-1;
//    }
//    if(selecteIDx>-1){
//        ui->listWidgetWatch->setCurrentRow(selecteIDx);
//    }


//}
//void MainWindow::showSaveWatchOnListView(){

//    ui->listWidgetWatch->clear();
//    if(savedWatchItems.size()==0){
//        ui->watchListSelectedDetails_Parent->setVisible(false);
//        return;
//    }
//    indicesDataMutex.lock();
//    QHash<QString, Indices_Data_Struct> indicesDataListTmp = indicesDataList;
//    indicesDataListTmp.detach();
//    indicesDataMutex.unlock();
//    int selecteIDx = -1;
//    for(int i=0;i<savedWatchItems.length();i++){
//        if(indicesDataListTmp.contains(savedWatchItems[i])){
//            auto item = new QListWidgetItem();
//            auto widget1 = new watch_Data_List_Item(this);
//            Indices_Data_Struct data = indicesDataListTmp[savedWatchItems[i]];
//            widget1->setData(data);
//            item->setSizeHint(widget1->sizeHint());
//            ui->listWidgetWatch->addItem(item);
//            ui->listWidgetWatch->setItemWidget(item, widget1);
//            if(data.indexName==watchItemSelectedindexName)
//                selecteIDx = ui->listWidgetWatch->count()-1;
//        }
//    }
//    if(selecteIDx>-1){
//        ui->listWidgetWatch->setCurrentRow(selecteIDx);
//    }
//    if(ui->listWidgetWatch->selectedItems().count()==0){
//        ui->watchListSelectedDetails_Parent->setVisible(false);
//    }
//}

//void MainWindow::on_listWidgetWatch_itemClicked(QListWidgetItem *item)
//{
//    if (!item) {
//        ui->watchListSelectedDetails_Parent->setHidden(true);
//        return; // Handle no item clicked scenario
//    }

//    watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(item));
//    if (!widget) {
//        ui->watchListSelectedDetails_Parent->setHidden(true);
//        return; // Handle case where no widget is associated with the item
//    }

//    watchItemSelectedindexName = widget->data.indexName;

//    updateSelecteWatch_UI(widget->data);

//}

//void MainWindow::updateSelecteWatch_UI( Indices_Data_Struct data){
//    ui->watchListSelectedDetails_Parent->setVisible(true);

//    bool redArrow = true;
//    //if(data.netChangeIndicator=="+")
//    // redArrow = false;
//    if(data.change.toDouble()>=0)
//        redArrow = false;

//    if(redArrow){
//        QPixmap pixmap(":/arrow_red.png");
//        pixmap = pixmap.scaled(ui->watch_indicator->width(),ui->watch_indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        ui->watch_indicator->setPixmap(pixmap);
//        ui->watch_indicator->setMask(pixmap.mask());
//        ui->watch_indicator->setStyleSheet("font-family: 'Work Sans';"
//                                           "font-size: 10px;"
//                                           "font-weight: 500;"
//                                           "color:#CC3437"
//                                           );
//    }
//    else{
//        QPixmap pixmap(":/arrow_green.png");
//        pixmap = pixmap.scaled(ui->watch_indicator->width(),ui->watch_indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        ui->watch_indicator->setPixmap(pixmap);
//        ui->watch_indicator->setMask(pixmap.mask());

//        ui->watch_val1->setStyleSheet("font-family: 'Work Sans';"
//                                      "font-size: 10px;"
//                                      "font-weight: 500;"
//                                      "color:#008000"
//                                      );
//    }

//    QString percent = (data.change+"("+data.percentagechange+"%)");
//    ui->Index_Name->setText(data.indexName);
//    ui->watch_val1->setText(data.indexValue);
//    ui->watch_val2->setText(percent);
//    ui->Open_Num->setText(data.openingIdx);
//    ui->High_Num->setText(data.highIndexValue);
//    ui->Low_Num->setText(data.lowIndexValue);
//    ui->Volume_Num->setText(data.marketCapitialisation);



//}

//void MainWindow::on_listWidgetWatch_itemDoubleClicked(QListWidgetItem *item)
//{
//    QListWidgetItem *selectedItem = ui->listWidgetWatch->currentItem();
//    if (selectedItem) {
//        watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(selectedItem));

//        //if selected item already in saved item list remove from else or add it
//        if(savedWatchItems.contains(widget->data.indexName)){
//            removeFromSavedWatchItems(widget->data);
//        }
//        else{
//            addToSavedWatchItems(widget->data);
//        }

//        ui->lineEditWatchSearch->clear();
//    }
//    else {
//        QMessageBox msgBox;
//        msgBox.setText("Please select an item from Watch.");
//        msgBox.setIcon(QMessageBox::Warning);
//        msgBox.exec();
//    }

//}

//void MainWindow::addToSavedWatchItems(Indices_Data_Struct data)
//{
//        if(!savedWatchItems.contains(data.indexName)){
//            savedWatchItems.append(data.indexName);
//            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
//            settings.beginGroup("GeneralSettings");
//            if(savedWatchItems.length()==1)
//                settings.setValue("Watch_InstrumentNames",savedWatchItems[0]);
//            else
//                settings.setValue("Watch_InstrumentNames",savedWatchItems.join(";"));
//            settings.endGroup();

//            if(ui->lineEditWatchSearch->text()=="")
//                showSaveWatchOnListView();

//        }
//        else{
//            QMessageBox msgBox;
//            msgBox.setText("IndexName already added.");
//            msgBox.setIcon(QMessageBox::Warning);
//            msgBox.exec();
//        }
//}

//void MainWindow::removeFromSavedWatchItems( Indices_Data_Struct data)
//{
//        if(savedWatchItems.contains(data.indexName)){
//            savedWatchItems.removeOne(data.indexName);
//            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
//            settings.beginGroup("GeneralSettings");
//            if(savedWatchItems.length()==1)
//                settings.setValue("Watch_InstrumentNames",savedWatchItems[0]);
//            else
//                settings.setValue("Watch_InstrumentNames",savedWatchItems.join(";"));
//            settings.endGroup();
//            if(ui->lineEditWatchSearch->text()=="")
//                showSaveWatchOnListView();


//        }
//        else{
//            QMessageBox msgBox;
//            msgBox.setText("IndexName not exist in Saved List.");
//            msgBox.setIcon(QMessageBox::Warning);
//            msgBox.exec();
//        }

//}
//void MainWindow::on_listWidgetWatch_itemSelectionChanged()
//{
//    if(ui->listWidgetWatch->selectedItems().count()==0){
//        ui->watchListSelectedDetails_Parent->setVisible(false);
//    }
//}


/****************************************************************/



//void MainWindow::on_toggle_Button_1_clicked()
//{
//    bool currentVisibility = ui->sidePanel->isVisible();
//    if (currentVisibility) {
//        ui->sidePanel->setVisible(false);
//        QPixmap pixmapRight(":/right_arrow.png");
//        ui->toggle_Button_1->setIcon(pixmapRight);

//    } else {
//        ui->sidePanel->setVisible(true);
//        QPixmap pixmapLeft(":/left_arrow.png");
//        ui->toggle_Button_1->setIcon(pixmapLeft);
//    }
//}


void MainWindow::saveDockManagerState()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QByteArray state1 = DockManagerMainPanel->saveState();
    QFile file1(appDataPath+"/dock_state_main_panel.dat");
    if (file1.open(QIODevice::WriteOnly)) {
        QDataStream out1(&file1);
        out1 << state1;
        file1.close();
    }

    QByteArray state2 = DockManagerSidePanel->saveState();
    QFile file2(appDataPath+"/dock_state_side_panel.dat");
    if (file2.open(QIODevice::WriteOnly)) {
        QDataStream out2(&file2);
        out2 << state2;
        file2.close();
    }
}




void MainWindow::restoreDockManagerState()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QFile file1(appDataPath+"/dock_state_main_panel.dat");
    if (file1.open(QIODevice::ReadOnly)) {
        QByteArray state1;
        QDataStream in1(&file1);
        in1 >> state1;
        DockManagerMainPanel->restoreState(state1);
        file1.close();
    }


    QFile file2(appDataPath+"/dock_state_side_panel.dat");
    if (file2.open(QIODevice::ReadOnly)) {
        QByteArray state2;
        QDataStream in2(&file2);
        in2 >> state2;
        DockManagerSidePanel->restoreState(state2);
        file2.close();
    }
}


void MainWindow::onPortFolioTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex){
     saveTableViewColumnState(T_Portfolio_Table);
}

void MainWindow::saveTableViewColumnState(QTableView *tableView){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/table_view_sett.dat", QSettings::IniFormat);
    QString key =tableView->objectName()+ "_HeaderState";

    // Save the state of the table view headers
   settings.setValue(key,tableView->horizontalHeader()->saveState());

}

void MainWindow::restoreTableViewColumnState(QTableView *tableView){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/table_view_sett.dat", QSettings::IniFormat);

       // Restore the state of the table view headers if available
     QString key =tableView->objectName()+ "_HeaderState";
     if (settings.contains(key))
     {
       tableView->horizontalHeader()->restoreState(settings.value(key).toByteArray());
     }
}

void MainWindow::onTradeTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTradeTableViewColumnState(trade_table);

}

void MainWindow::saveTradeTableViewColumnState(QTableView *tableView) {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
     QSettings settings(appDataPath + "/table_view_sett.dat", QSettings::IniFormat);
     QString key = tableView->objectName() + "_HeaderState";

     // Save the state of the trade table headers
     settings.setValue(key, tableView->horizontalHeader()->saveState());
}

void MainWindow::restoreTradeTableViewColumnState(QTableView *tableView) {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
     QSettings settings(appDataPath + "/table_view_sett.dat", QSettings::IniFormat);

     // Restore the state of the trade table headers if available
     QString key = tableView->objectName() + "_HeaderState";
     if (settings.contains(key)) {
       tableView->horizontalHeader()->restoreState(settings.value(key).toByteArray());
     }
}
void MainWindow::onNetposTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveNetposTableViewColumnState(net_pos_table);

}

void MainWindow::saveNetposTableViewColumnState(QTableView *tableView) {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
     QSettings settings(appDataPath + "/table_view_sett.dat", QSettings::IniFormat);
     QString key = tableView->objectName() + "_HeaderState";

     // Save the state of the trade table headers
     settings.setValue(key, tableView->horizontalHeader()->saveState());
}

void MainWindow::restoreNetposTableViewColumnState(QTableView *tableView) {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
     QSettings settings(appDataPath + "/table_view_sett.dat", QSettings::IniFormat);

     // Restore the state of the trade table headers if available
     QString key = tableView->objectName() + "_HeaderState";
     if (settings.contains(key)) {
       tableView->horizontalHeader()->restoreState(settings.value(key).toByteArray());
     }
}
void MainWindow::onLinersTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveLinersTableViewColumnState(liners_table);

}

void MainWindow::saveLinersTableViewColumnState(QTableView *tableView) {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
     QSettings settings(appDataPath + "/table_view_sett.dat", QSettings::IniFormat);
     QString key = tableView->objectName() + "_HeaderState";

     // Save the state of the trade table headers
     settings.setValue(key, tableView->horizontalHeader()->saveState());
}

void MainWindow::restoreLinersTableViewColumnState(QTableView *tableView) {
     QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
     QSettings settings(appDataPath + "/table_view_sett.dat", QSettings::IniFormat);

     // Restore the state of the trade table headers if available
     QString key = tableView->objectName() + "_HeaderState";
     if (settings.contains(key)) {
       tableView->horizontalHeader()->restoreState(settings.value(key).toByteArray());
     }
}
void MainWindow::on_sorting_Button_clicked(){


      if (!sortWin) {
           sortWin = new SortSettingPopUp();
           connect(sortWin, &SortSettingPopUp::reloadSortSettingSignal, [this]() {
               reloadSortSettFlg.storeRelaxed(1);
           });
       }
      else{
          delete sortWin;
          sortWin = new SortSettingPopUp();
          connect(sortWin, &SortSettingPopUp::reloadSortSettingSignal, [this]() {
              reloadSortSettFlg.storeRelaxed(1);
          });
      }
       sortWin->show();
       sortWin->raise();  // Bring the window to the front
       sortWin->activateWindow(); // Give focus to the window

}
//For Buy
void MainWindow::F1_clicked_slot(){
   F1_F2_BuySell *F1F2 = new F1_F2_BuySell(nullptr,devicer,decimal_precision);
   F1F2->setAttribute(Qt::WA_DeleteOnClose);
   F1F2->userData = this->userData;
   F1F2->show();
   F1F2->setBuyMode(true);
}
//For Sell
void MainWindow::F2_clicked_slot(){
    F1_F2_BuySell *F1F2 = new F1_F2_BuySell(nullptr,devicer,decimal_precision);
    F1F2->setAttribute(Qt::WA_DeleteOnClose);
    F1F2->userData = this->userData;
    F1F2->show();
    F1F2->setBuyMode(false);

}




void MainWindow::closeEvent(QCloseEvent* event)
{
    // Delete dock manager here to delete all floating widgets. This ensures
    // that all top level windows of the dock manager are properly closed
    saveDockManagerState();

    DockManagerSidePanel->deleteLater();
    DockManagerMainPanel->deleteLater();
    QMainWindow::closeEvent(event);

    QCoreApplication::quit();

}




void MainWindow::onSummaryActionTriggered(){

    Summary_cards *SC = new Summary_cards(this);
    Qt::WindowFlags flags = SC->windowFlags();
    SC->setWindowFlags(flags | Qt::Tool);
    SC->setAttribute(Qt::WA_DeleteOnClose);
    connect(this, &MainWindow::data_summary_update_signal, SC, &Summary_cards::updateSummaryData);

    SC->show();
}

void MainWindow::onWatchActionTriggered(){
    Watch_cards *WC = new Watch_cards(this);
    Qt::WindowFlags flags = WC->windowFlags();
    WC->setWindowFlags(flags | Qt::Tool);

    WC->setAttribute(Qt::WA_DeleteOnClose);
    //connect(this, &MainWindow::indicesDataRecv_Signal_watch_card(Indices_Data_Struct), WC, &Watch_cards::indicesDataRecv_Slot(Indices_Data_Struct));
    connect(this, &MainWindow::indicesDataRecv_Signal_watch_card, WC, &Watch_cards::indicesDataRecv_Slot);
    WC->show();
}

void MainWindow::onLogActionTriggered(){
    Logs_cards *LC = new Logs_cards(this);
    Qt::WindowFlags flags = LC->windowFlags();
    LC->setWindowFlags(flags | Qt::Tool);
    LC->setAttribute(Qt::WA_DeleteOnClose);
    LC->logDataSignal_Slot(logsdata);
    connect(this, &MainWindow::logDataSignal, LC, &Logs_cards::logDataSignal_Slot);

    LC->show();
}


