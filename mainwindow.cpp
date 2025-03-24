#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QtConcurrent/QtConcurrent>
#include <QCoreApplication>
#include <QHeaderView>
#include <QRegularExpression>
#include <QShortcut>
#include <QFontDatabase>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLabel>
#include <QToolTip>
#include "contractdetail.h"
#include "OrderBook/table_orderbook_delegate.h"
#include "mysql_conn.h"
#include "style_sheet.h"
#include "sortsettingpopup.h"
#include "Liners/liners_delegate.h"
#include "NetPosition/net_position_table_delegate.h"
#include "NetPosition/net_position_table_headerview.h"
#include "Cards/logs_cards.h"
#include "Cards/summary_cards.h"
#include "Cards/watch_cards.h"
#include "QMenu"
#include "PortFolio/portfolio_searchfilterproxymodel.h"
#include "TradePosition/tradetable_searchfilterproxymodel.h"
#include <algorithm>
#include "MissedTrades/missed_trades_headerview.h"
#include "OpenPosition/open_position_model.h"
#include "OpenPosition/open_position_delegate.h"
#include "OpenPosition/open_position_headerview.h"
#include "MissedTrades/missed_trade_table_delegate.h"
#include "NetPosition/netpos_searchfilterproxymodel.h"

//#define ENABLE_BACKEND_DEBUG_MSG

static const char stylesheetvis[]= "background: #596167;" "border-radius: 10px;";

extern MainWindow *MainWindowObj;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
     sortWin(nullptr), // Initialize the pointer to nullptr,
     convertalgo(nullptr),
     summary(nullptr),
     watch(nullptr),
     logs(nullptr)

{
    //ui->lineEditSearch->hide();
   QFontDatabase::addApplicationFont(":/WorkSans-Bold.ttf");
   QFontDatabase::addApplicationFont(":/WorkSans-ExtraBold.ttf");

    ui->setupUi(this);

    this->setMinimumSize(800 ,500);

  //  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);-
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);


#ifdef BACKEND_LOG
    // Generate log file name with timestamp
       QString logFileName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs/"+QString("Backend_log_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
       // Open the log file
       backend_logFile.setFileName(logFileName);
       if (backend_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
           backend_logStream.setDevice(&backend_logFile);
           write_backend_Log("Log started");
       } else {
           qDebug() << "Failed to open backend log file.";
       }
#endif

    ui->Algorithms_Close->setVisible(false);
    newIndicesData = false;

    showMessagOnceFlg = true;
   // connect(this,SIGNAL(data_summary_update_signal()),this,SLOT(updateSummaryLabels()));
    connect(this,SIGNAL(showMessageSignal(QString)),this,SLOT(showMessageSlot(QString)));
    connect(this, SIGNAL(requestDeleteConfirmation(QStringList)), this, SLOT(onRequestDeleteConfirmation(QStringList)));


    /*************Initialize settings window*********/
    sett_win = new Settings_Window(this);
    connect(sett_win,SIGNAL(updateHotKeysSignal(QStringList)),this,SLOT(updateHotKeysSlot(QStringList)));


    /*************************************************/

    /***********************Main Menu Section**********************************/

    QString MenuStyle =  "QToolButton {"
                              "    color: white;"
                              "    font-family: 'Work Sans';"
                              "    font-weight: bold;"

                              // "    padding-right: 10px;"  // Add padding for spacing
                              // "   border: 0px solid #4F5D75;"
                              "}";
    QMenu *mainMenu = new QMenu("Settings", this);
    ui->menu->setMenu(mainMenu);
    ui->menu->setPopupMode(QToolButton::InstantPopup);
    ui->menu->setStyleSheet(MenuStyle);
    ui->menu->setText("Settings");

    // Create reset password actions
    QAction *changepasswordAction = new QAction("ChangePassword", this);
    mainMenu->addAction(changepasswordAction);// Add actions to the Card menu
    connect(changepasswordAction, &QAction::triggered, this, &MainWindow::onChangepasswordActionTriggered);// Create a ToolButton to act as the menu title on ui->card


    // Create reset password actions
    QAction *exportAction = new QAction("Export To CSV", this);
    mainMenu->addAction(exportAction);// Add actions to the Card menu
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportTableViewToCSV);// Create a ToolButton to act as the menu title on ui->card


    // Create setting window actions
    QAction *openSettWinAction = new QAction("Hot Keys", this);
    mainMenu->addAction(openSettWinAction);// Add actions to the Card menu
    connect(openSettWinAction, &QAction::triggered, this, &MainWindow::openSettingsWindow);// Create a ToolButton to act as the menu title on ui->card




    /********************************************************************************/

    /**********************Top Bar Button Widget Secton******************************/

    QString toolButtonStyle =  "QToolButton {"
                                      "    color: white;"
                                      "    font-family: 'Work Sans';"
                                      "    font-weight: bold;"

                                     // "    padding-right: 10px;"  // Add padding for spacing
                                     // "   border: 0px solid #4F5D75;"
                               "}";

    // Create the Card menu
    QMenu *cardMenu = new QMenu("Card", this);
    cardMenu->addAction("Summary", this, &MainWindow::onSummaryActionTriggered);
    cardMenu->addAction("Watch", this, &MainWindow::onWatchActionTriggered);
    cardMenu->addAction("Logs", this, &MainWindow::onLogActionTriggered);
    ui->toolButtonCards->setMenu(cardMenu);
    ui->toolButtonCards->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonCards->setStyleSheet(toolButtonStyle);



    QMenu *menuAlgorithm = new QMenu(this);
    menuAlgorithm->addAction("Algos", this, &MainWindow::on_Algorithms_Button_clicked);
    menuAlgorithm->addAction("Build Algo", this, &MainWindow::ConvertAlgo_button_clicked);
    menuAlgorithm->addAction("Define Algos", this, &MainWindow::on_Templates_Button_clicked);
    ui->toolButtonAlgorithm->setMenu(menuAlgorithm);
    ui->toolButtonAlgorithm->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonAlgorithm->setStyleSheet(toolButtonStyle);

    QMenu *menuPosition = new QMenu(this);
    menuPosition->addAction("Net Position", this, &MainWindow::on_Positions_Button_clicked);
    menuPosition->addAction("Algo Position", this, &MainWindow::on_Liners_Button_clicked);
    menuPosition->addAction("Open Position", this, &MainWindow::on_OpenPosition_Button_clicked);
    menuPosition->addAction("Historical Position", this, &MainWindow::on_HP_Button_clicked);
    ui->toolButtonPostion->setMenu(menuPosition);
    ui->toolButtonPostion->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonPostion->setStyleSheet(toolButtonStyle);



    QMenu *menuOrders = new QMenu(this);
    menuOrders->addAction("Executed Orders", this, &MainWindow::on_OrderBook_Button_clicked);
    menuOrders->addAction("Skipped Orders", this, &MainWindow::on_MissedTrade_Button_clicked);
    menuOrders->addAction("Manaual Orders", this, &MainWindow::on_F1F2Trade_Button_clicked);
    ui->toolButtonOrders->setMenu(menuOrders);
    ui->toolButtonOrders->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonOrders->setStyleSheet(toolButtonStyle);



//QFont font("Work Sans", 10, QFont::Bold); // Set the font to Work Sans, size 10, bold
//    ui->label_4->setFont(font);
//    ui->label_4->setText("status :");
//    ui->label_2->setFont(font);
//    ui->label_2->setText("User Message :");




//    ui->lineEditSearch->setPlaceholderText("Search Algo");
//    QAction* search_action = new QAction(QIcon(":/search.png"), "", ui->lineEditSearch);

//    // Add the action to the QLineEdit
//    ui->lineEditSearch->addAction(search_action, QLineEdit::LeadingPosition);
//    ui->lineEditSearch->setStyleSheet(
//        "QLineEdit {"
//        "    font-family: 'Work Sans';"
//        "    font-size: 10pt;"
//        "    border-radius: 15px;"      // Use 15px for a curved effect
//        "}"
//        );   --temporarily disabled for now it is not in use



//    ui->Templates_Widget->setToolTip("Coming Soon");
//    // Apply stylesheet for hover effect with sparkles
//    ui->Templates_Widget->setStyleSheet(
//        "QWidget#Templates_Widget {"
//      //  "    background-color: rgb(77, 77, 115);"  // Normal background
//        "    color: white;"
//        "    font-family: 'Work Sans';"
//        "    font-weight: bold;"
//        "}"
//        "QWidget#Templates_Widget:hover {"
//        "    background-color: rgb(100, 100, 150);"  // Change background on hover
//        "    color: white;"
//        "    border: 1px solid yellow;"  // Example effect
//     //   "    qproperty-toolTip: 'Coming Soon';"
//        "    /* Sparkle Effect: Add sparkles as a background image or custom animation */"
//        "}"
//        );

//      ui->lineEditSearch->setStyleSheet(
//        "QLineEdit {"
//        "    padding-right: 50px;"  // Add 20 pixels of space on the right
//        "}"
//        );

      ui->HP_Widget->setVisible(false);
      ui->Templates_Widget->setVisible(false);
      ui->Positions_Widget->setVisible(false);
      ui->OrderBook_Widget->setVisible(false);
      ui->MissedTrade_widget->setVisible(false);
      ui->Liners_Widget->setVisible(false);
      ui->F1F2Trade_widget->setVisible(false);
      ui->Algorithms_Widget->setVisible(false);
      ui->lineEditSearch->setVisible(false);

    //ui->widget_3->setVisible(false);
      ui->widget_5->setVisible(true);
    //ui->widget_4->setVisible(false);
    //ui->toggle_Button_1->setVisible(false);

      ui->Algorithms_Close->setVisible(false);
      ui->OrderBook_Close->setVisible(false);
      ui->Positions_Close->setVisible(false);
      ui->Liners_Close->setVisible(false);
      ui->HP_Close->setVisible(false);
      ui->Templates_Close->setVisible(false);

      /********************************************************************************/

  //  connect(this,SIGNAL(data_summary_update_signal()),this,SLOT(updateSummaryLabels()));
//      setTabOrder(ui->Algorithms_Button,ui->Liners_Button);
//    setTabOrder(ui->Liners_Button,ui->OrderBook_Button);
//    setTabOrder(ui->OrderBook_Button,ui->Positions_Button);
//    setTabOrder(ui->Positions_Button,ui->MissedTrade_Button);
//    setTabOrder(ui->MissedTrade_Button,ui->Templates_Button);
//    setTabOrder(ui->Templates_Button,ui->toolButtonCards);
//    setTabOrder(ui->toolButtonCards,ui->lineEditSearch);
//    setTabOrder(ui->lineEditSearch,ui->Algorithms_Button);





    connect(this,SIGNAL(display_log_text_signal(QString)),this,SLOT(slotAddLogForAddAlgoRecord(QString)));
    loadingDataWinodw = new loadingdatawindow(this);
    connect(this,SIGNAL(signalHideProgressBar()),this,SLOT(slotHideProgressBar()));

    // db_conn =new mysql_conn(this,"main_db_conn");
    db_conn =new mysql_conn(0,"main_db_conn");
    connect(db_conn,SIGNAL(display_log_text_signal(QString)),this,SLOT(slotAddLogForAddAlgoRecord(QString)));

    connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));
    orderPopUpWin = new OrderDetail_Popup(this);
    tradePopUpWin = new TradeDetailsPopup(this);


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
    refresh_entire_table.storeRelaxed(1);
    portfolio_table_updating_db.storeRelaxed(0);
    portfolio_table_slected_idx_for_editing.store(-1);
    loggedInFlg.storeRelaxed(0);


   // QPixmap pixmapclose(":/close_window_icon.png");
    //QPixmap pixmapminimize(":/minimize_window_icon.png");
    QPixmap pixmapclose = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QPixmap pixmapmaximize = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);
    QPixmap pixmapminimize = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    //QPixmap pixmapmaximize(":/maximize_window_icon.png");
    QFont headerfont("Work Sans");



    //CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    //CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    CDockManager::setConfigFlag(CDockManager::ActiveTabHasCloseButton, false);
    CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);



    QVBoxLayout* Layout = new QVBoxLayout(ui->mainPanel);
    Layout->setContentsMargins(QMargins(0, 0, 0, 0));

    DockManagerMainPanel = new CDockManager(ui->mainPanel);
    Layout->addWidget(DockManagerMainPanel);


    /***********Init portfolio table Window**************************/
   // QPixmap pixmapdock_algorithms_close(":/dock_close.png");

    T_Portfolio_DockWin = new CDockWidget("Algos");// new QDockWidget(tr("Algorithms"), this);
    connect(T_Portfolio_DockWin, SIGNAL(visibilityChanged(bool)), this, SLOT(OnAlgorithmDockWidgetVisiblityChanged(bool)));
    T_Portfolio_DockWin->setStyleSheet(dock_style);
    T_Portfolio_DockWin->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);

    T_Portfolio_DockWin->setMinimumSize(200,150);

    T_Portfolio_DockWin->setWindowModality(Qt::NonModal);

   // const auto autoHideContainerPortfolioTable = DockManager->addAutoHideDockWidget(SideBarLocation::SideBarLeft, T_Portfolio_DockWin);
   // autoHideContainerPortfolioTable->setSize(480);

    DockManagerMainPanel->addDockWidget(DockWidgetArea::RightDockWidgetArea, T_Portfolio_DockWin);

        //Create Titlebar
        QWidget* titlecontainer = new QWidget;
        titlecontainer->setStyleSheet(DockTitleBar_Style);
        QHBoxLayout *titleinternalLayout = new QHBoxLayout(titlecontainer);
        titleinternalLayout->setSpacing(10);
        titleinternalLayout->setContentsMargins(17,8,10,6);
        QLabel *label = new QLabel("Algos");
        QFont fontlabel=label->font();
        QSpacerItem* titlespc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
        QToolButton* dockclose=new QToolButton();
        connect(dockclose, &QToolButton::clicked, [=](){ T_Portfolio_DockWin->close();});
       // dockclose->setIcon(pixmapdock_algorithms_close);
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
            line_edit_trade_search->setFixedWidth(360);
            line_edit_trade_search->setFixedHeight(30);

            QFont font("Work Sans");
            font.setPointSize(9);
            font.setBold(true);
            line_edit_trade_search->setFont(font);

            line_edit_trade_search->setPlaceholderText("Search...");
            QAction* search_action = new QAction(QIcon(":/search.png"), "", line_edit_trade_search);

            // Add the action to the QLineEdit
            line_edit_trade_search->addAction(search_action, QLineEdit::LeadingPosition);
            line_edit_trade_search->setStyleSheet(lineedit_dock_SS);

            QToolButton *ConvertAlgo_button = new QToolButton();
            connect(ConvertAlgo_button, SIGNAL(clicked()), this, SLOT(ConvertAlgo_button_clicked()));
            //Frame 364.png
            ConvertAlgo_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
            //ConvertAlgo_button->setIcon(QIcon(":/BuildAlgo.png"));
            ConvertAlgo_button->setText("Build Algo");
            ConvertAlgo_button->setToolTip("Build Algo");

            const char convert_to_algo_button[]="QToolButton {"
                                               "border-radius: 4px;"
                                                  "background: #495057;"
                                                  "text-align: center;"
                                                  "color: #FFF;"
//                                                  "min-width: 22px;"   // Set minimum width
//                                                  "max-width: 22px;"   // Set maximum width
//                                                  "min-height: 22px;"  // Set minimum height
//                                                  "max-height: 22px;"
                                                  "}"
                                                  "QToolButton:hover{"
                                                  "border-radius: 4px;"
                                                  "border: 1px solid #106B9A;"
                                                  "background: #106B9A;"
                                                  "}";
           ConvertAlgo_button->setStyleSheet(convert_to_algo_button);
           // ConvertAlgo_button->setContentsMargins(10,-1,-1,-1);
            //ConvertAlgo_button->setFont(headerfont);
           ConvertAlgo_button->setObjectName("buildalgo");
           ConvertAlgo_button->setFocusPolicy(Qt::FocusPolicy::StrongFocus);


            QWidget *test=new QWidget;
            const char layss[]="background: #E9ECEF;";
            test->setStyleSheet(layss);
            QHBoxLayout *internalLayout = new QHBoxLayout(test);
            test->setLayout(internalLayout);
            internalLayout->setContentsMargins(10,-1,-1,-1);
            QSpacerItem* spc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
            QToolButton* button1 = new QToolButton();
            connect(button1, SIGNAL(clicked()), this, SLOT(startall_Button_clicked()));

           // button1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button1->setIcon(QIcon(":/start_all.png"));
            button1->setText("Start All");
            button1->setToolTip("Start All");
            button1->setObjectName("startallbutton");
            button1->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

            QToolButton* button2 = new QToolButton();
            connect(button2, SIGNAL(clicked()), this, SLOT(stopall_Button_clicked()));
           // button2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
           // button2->setIcon(QIcon(":/stop_all.png"));
            button2->setText("Stop All");
            button2->setToolTip("Stop All");
            button2->setObjectName("stopallbutton");
            button2->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

            QToolButton* button3 = new QToolButton();
           // button3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button3->setIcon(QIcon(":/import.png"));
            button3->setText("Import");
            button3->setToolTip("Import");
            button3->setObjectName("import");
            button3->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            connect(button3, SIGNAL(clicked()), this, SLOT(import_Action()));

            QToolButton* button4 = new QToolButton();
            //button4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button4->setIcon(QIcon(":/reset.png"));
            button4->setText("Sorting");
            button4->setToolTip("Sorting");
            connect(button4, SIGNAL(clicked()), this, SLOT(sorting_Button_clicked()));
            button4->setObjectName("sorting");
            button4->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

            QToolButton* button5 = new QToolButton();
            button5->setText("Refresh");
            //button5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button5->setToolTip("Refresh");
            connect(button5, SIGNAL(clicked()), this, SLOT(refresh_Button_clicked()));
            button5->setObjectName("refresh");
            button5->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

            QToolButton* button6 = new QToolButton();
          //  button6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
           // button6->setIcon(QIcon(":/export.png"));
            button6->setText("Export");
            button6->setToolTip("Export");
            button6->setObjectName("export");
            button6->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
            connect(button6, SIGNAL(clicked()), this, SLOT(export_Action()));

            QToolButton* button7 = new QToolButton();
            //  button7->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button7->setText("Duplicate");
            button7->setToolTip("Duplicate");
            connect(button7, SIGNAL(clicked()), this, SLOT(duplicate_Button_clicked()));
            button7->setObjectName("duplicate_bt_id");
            button7->setFocusPolicy(Qt::FocusPolicy::StrongFocus);


            // Create the QToolButton
            QToolButton* button8 = new QToolButton(this);
            button8->setText("Filter");
            button8->setPopupMode(QToolButton::InstantPopup); // Shows the menu immediately on click
            button8->setStyleSheet(
                "QToolButton {" // Corrected to QToolButton
                "   border-radius: 4px;"
                "   border: 1px solid #4F5D75;"
                "   background: #106B9A;"
                "   color: white;"
                "   font-family: 'Work Sans';"
                "   padding-right: 8px;"  // Add padding for spacing
                "   width: 50px;"      // Set button width
             "line-height: normal;"      // Optional: Set button height
                "}"
                "QToolButton:hover {" // Corrected to QToolButton:hover
                "   border-radius: 4px;"
                "   border: 1px solid #495057;"
                "   background: #495057;"
                "   color: white;"
                "   font-family: 'Work Sans';"
                "   width: 120px;"      // Ensure width remains consistent
                "}"
                );
            button8->setObjectName("filter");
            button8->setFocusPolicy(Qt::FocusPolicy::StrongFocus);









//            // Connect the button to the filter functionality
//            connect(button8, &QToolButton::clicked, this, &MainWindow::onFilterButtonClicked);

//            // Optionally, add the button to a layout
//            ui->someLayout->addWidget(button8); // Replace 'someLayout' with your actual layout object


            // Apply custom stylesheet
//            QCheckBox *switchBox = new QCheckBox(this);
//            switchBox->setText("OFF"); // Set the initial text to "OFF"
//            switchBox->setFixedSize(40, 25);

//            // Apply custom stylesheet
//            switchBox->setStyleSheet(
//                "QCheckBox {"
//                "    font-size: 14px;"     // Font size for the checkbox text
//                "    color: white;"        // Default text color
//                "    background-color: #444;" // Background color for the checkbox
//                "    border: 1px solid #888;" // Border to make the checkbox visible
//                "    padding: 5px;"        // Padding around the text
//                "}"
//                "QCheckBox::indicator {"
//                "    width: 0px;"          // Hide the default checkbox indicator
//                "    height: 0px;"         // Hide the default checkbox indicator
//                "}"
//                "QCheckBox:unchecked {"
//                "    color: red;"          // Text color for unchecked state
//                "}"
//                "QCheckBox:checked {"
//                "    color: green;"        // Text color for checked state
//                "}"
//                );

//            // Connect the stateChanged signal to update the text dynamically
//            connect(switchBox, &QCheckBox::stateChanged, this, [this, switchBox](int state) {
//                if (state == Qt::Checked) {
//                     QString msg;
//                    bool ret = db_conn->modifytype("1", QString::number(userData.UserId), msg);
//                    switchBox->setText("ON");  // Set text to "ON" when checked
//                } else {
//                     QString msg;
//                    bool ret = db_conn->modifytype("2", QString::number(userData.UserId), msg);
//                    switchBox->setText("OFF"); // Set text to "OFF" when unchecked
//                }
//            });




            const char stylesheet_tb[] =
                "QToolButton {"
                "border-radius: 4px;"
                "border: 1px solid #4F5D75;"
                "background: #106B9A;"
                "color: white;"
//                "color: #4F5D75;"
//                "font-style: normal;"
//                "font-size: 12px;"
//                "font-weight: 500;"
//                "line-height: normal;"
//                "padding: 2px 5px;"
                "}"
                "QToolButton:hover {"
                "border-radius: 4px;"
                "border: 1px solid #495057;"
                "background: #495057;"
                "color: #FFF;"
                "font-family: Work Sans;"
                "color: white;"
//                "font-size: 12px;"
//                "font-style: normal;"
//                "font-weight: 500;"
//                "line-height: normal;"
//                "padding: 2px 5px;"
                "}";

            for (auto w : {button1, button2, button3, button4, button5, button6, button7}) {
        w->setStyleSheet(stylesheet_tb);
        QFont font = w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
            }




                internalLayout->addWidget(ConvertAlgo_button);

                //internalLayout->addSpacerItem(spc);

                internalLayout->addWidget(button1);
                internalLayout->addWidget(button2);
                internalLayout->addWidget(button7);
                internalLayout->addWidget(button8);
                internalLayout->addWidget(button4);
                internalLayout->addWidget(button3);
                internalLayout->addWidget(button6);
                internalLayout->addWidget(button5);
                internalLayout->addWidget(line_edit_trade_search);

                internalLayout->addSpacerItem(spc);
                //internalLayout->addWidget(button5);
                //internalLayout->addWidget(line_edit_trade_search);
                lay->addWidget(test, 0, 0);

    T_Portfolio_Table = new table_portfolios_custom(T_Portfolio_DockWin);
    T_Portfolio_Table->setObjectName("algo_table");
    T_Portfolio_Table->setStyleSheet(scroll_bar_SS);
    // T_Portfolio_Table->viewport()->setFocusPolicy(Qt::NoFocus);

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
    connect(headerView, &QHeaderView::sectionMoved, this, &MainWindow::onPortFolioTableHeader_Rearranged,Qt::UniqueConnection);

    // Set the width of the fixed column (adjust the width as needed)



    T_Portfolio_Table->setHorizontalHeader(headerView);
    T_Portfolio_Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    //T_Portfolio_Table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    T_Portfolio_Table->horizontalHeader()->setStretchLastSection(false);
    T_Portfolio_Table->setShowGrid(false);
    T_Portfolio_Table->verticalHeader()->setVisible(false);
    T_Portfolio_Table->setAlternatingRowColors(true);
    T_Portfolio_Table->setSelectionBehavior(QAbstractItemView::SelectRows);
    T_Portfolio_Table->setSelectionMode(QAbstractItemView::ExtendedSelection);



    T_Portfolio_Model = new Table_Portfolios_Model();
    T_Portfolio_Delegate =  new Table_Portfolios_Delegate();
    //connect(T_Portfolio_Model,SIGNAL(edit_Started(int,int)),this,SLOT(edit_Started_PortFolio_Table(int,int)));
    QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::editFinished, this, &MainWindow::profolioTableEditFinshedSlot);
    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::editCompleted, this, &MainWindow::profolioTableEditFinshedSlot);
    QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::tabKeyPressed, T_Portfolio_Table, &table_portfolios_custom::handleTabKeyPressFromEditableCell);
    QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::escapeKeyPressed, T_Portfolio_Model, &Table_Portfolios_Model::editCancelled);


    //QObject::connect(T_Portfolio_Table, &table_portfolios_custom::spaceKeySignal, this, &MainWindow::updatePortFolioStatus);
   // QObject::connect(T_Portfolio_Table, &table_portfolios_custom::portFolioDeleteKeyPressed, this, &MainWindow::Delete_clicked_slot);


    //QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::resizePortFolioTableColWidth, this, &MainWindow::resizePortFolioTableColWidthSlot);
   // QObject::connect(T_Portfolio_Table, &table_portfolios_custom::clicked, T_Portfolio_Model, &Table_Portfolios_Model::onItemChanged);
    QObject::connect(T_Portfolio_Table, &table_portfolios_custom::selectionChangedSignal, T_Portfolio_Model, &Table_Portfolios_Model::selectionChangedSlot,Qt::QueuedConnection);
    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::updateDBOnDataChanged, this, &MainWindow::updatePortFolioStatus, Qt::QueuedConnection);
    QObject::connect(db_conn, &mysql_conn::updateModelDataListSignal, T_Portfolio_Model, &Table_Portfolios_Model::updateModelDataList, Qt::QueuedConnection); //QueuedConnection for avoid deadlock when mutex used inside model class,



 //   T_Portfolio_Table->setModel(T_Portfolio_Model);
    T_Portfolio_Table->setItemDelegate(T_Portfolio_Delegate);




    connect(T_Portfolio_Table, &QTableView::doubleClicked, this, &MainWindow::T_Portfolio_Table_cellDoubleClicked);

    //QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    T_Portfolio_ProxyModel = new Portfolio_SearchFilterProxyModel(this);
    T_Portfolio_ProxyModel->setSourceModel(T_Portfolio_Model);
    T_Portfolio_ProxyModel->setFilterKeyColumn(-1);  // Search across all columns

//    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, [=](const QString &text) {
//        // Check if the relevant window or widget is open/visible
//       // if (T_Portfolio_Table->isVisible()) { // Replace 'this' with the specific window or widget if needed
//            T_Portfolio_ProxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
//            T_Portfolio_Delegate->setHighlightText(text);
//            T_Portfolio_Table->viewport()->update();  // Redraw the view to apply the highlight
//       // }
////        else{
////            T_Portfolio_Delegate->setHighlightText(text);
////        }
//    });
        connect(line_edit_trade_search, &QLineEdit::textChanged, this, [=](const QString &text) {
                T_Portfolio_ProxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
                T_Portfolio_Delegate->setHighlightText(text);
                T_Portfolio_Table->viewport()->update();  // Redraw the view to apply the highlight
              });


    T_Portfolio_Table->setModel(T_Portfolio_ProxyModel);

    connect(T_Portfolio_Table->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this, SLOT(T_Portfolio_Table_cellClicked(const QItemSelection&, const QItemSelection&)));



    lay->addWidget(T_Portfolio_Table,1,0);
    T_Portfolio_Table->show();
    restoreTableViewColumnState(T_Portfolio_Table);

    //This will make first column fixed size
    T_Portfolio_Table->horizontalHeader()->setSectionResizeMode(PortfolioData_Idx::_Status, QHeaderView::Fixed);
    T_Portfolio_Table->setColumnWidth(PortfolioData_Idx::_Status, 60);
    T_Portfolio_Table->setColumnWidth(PortfolioData_Idx::_AlgoName, 260);
   // T_Portfolio_Table->setColumnWidth(PortfolioData_Idx::_BidLeg, 180);

    int Portfolio_columnCount = T_Portfolio_Table->model()->columnCount();

    for (int col = 0; col < Portfolio_columnCount; ++col) {
        if (col != PortfolioData_Idx::_Status && col != PortfolioData_Idx::_AlgoName /*&& col != PortfolioData_Idx::_BidLeg*/) {
            // Set all other columns to 70 pixels
            T_Portfolio_Table->setColumnWidth(col, 70);
        }
        // Allow interactive resizing for all columns except _Status
        if (col != PortfolioData_Idx::_Status) {
            T_Portfolio_Table->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Interactive);
        }
    }


    //load the settings, get all the column index we checked already

    //create menu to show hide the table view columns
    QStringList headers = T_Portfolio_Model->getHeaders();
    // List of columns to exclude
    QSet<int> excludedColumns = {0,3, 7, 6,  12,13, 16, 17};

    // Create the filter menu
    QMenu* algoTableColumnShowHideMenu = new QMenu(this);
    for (int i = 0; i < headers.size(); ++i) {
        // Skip the excluded columns
        if (excludedColumns.contains(i)) {
            continue;
        }

        QAction* action = algoTableColumnShowHideMenu->addAction(headers[i]);
        action->setCheckable(true);
        action->setChecked(!T_Portfolio_Table->isColumnHidden(i));

        // Connect action's triggered signal
        connect(action, &QAction::triggered, this, [=](bool checked) {
            T_Portfolio_Table->setColumnHidden(i, !checked);
        });
    }

       button8->setMenu(algoTableColumnShowHideMenu);



    /***********Init portfolio table Window**************************/



    /***********Init Order Book Window**************************/
    QPixmap pixmapdock_trade_close(":/dock_close.png");

       // Create the dock widget
       dock_win_trade = new CDockWidget(tr("Executed Orders"));

       // Connect the visibilityChanged signal to the slot
connect(dock_win_trade, SIGNAL(visibilityChanged(bool)), this, SLOT(OnOrderBookDockWidgetVisiblityChanged(bool)));
       // Additional dock widget setup
       dock_win_trade->setStyleSheet(dock_style);
       dock_win_trade->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
       dock_win_trade->setMinimumSize(200, 150);

    const auto autoHideContainerTrade = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_trade);
    autoHideContainerTrade->setSize(480);

    //create a titlebar
    QWidget *trade_titlebar=new QWidget;
    trade_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *trade_title_layout=new QHBoxLayout(trade_titlebar);
    trade_title_layout->setSpacing(10);
    trade_title_layout->setContentsMargins(17,8,10,6);
    QLabel *trade_label=new QLabel("Executed Orders");
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


    QLineEdit* line_edit_Executed_search = new QLineEdit;
    line_edit_Executed_search->setFixedWidth(360);
    line_edit_Executed_search->setFixedHeight(30);

    QFont fontexe("Work Sans");
    fontexe.setPointSize(9);
    fontexe.setBold(true);
    line_edit_Executed_search->setFont(fontexe);

    line_edit_Executed_search->setPlaceholderText("Search...");

    QAction* search_actiontrades = new QAction(QIcon(":/search.png"), "", line_edit_Executed_search);



    // Add the action to the QLineEdit
    line_edit_Executed_search->addAction(search_actiontrades, QLineEdit::LeadingPosition);
    line_edit_Executed_search->setStyleSheet(lineedit_dock_SS);

    QWidget *test1=new QWidget;
    const char lays[]="background: #E9ECEF;";
    test1->setStyleSheet(lays);
    QHBoxLayout *internalLayouttrades = new QHBoxLayout(test1);
    test1->setLayout(internalLayouttrades);
    internalLayouttrades->setContentsMargins(10,-1,-1,-1);
    QSpacerItem* spcr = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    internalLayouttrades->addWidget(line_edit_Executed_search);
  lay_Trade_Window->addWidget(test1, 0, 0);
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
     connect(headerViews, &QHeaderView::sectionMoved, this, &MainWindow::onTradeTableHeader_Rearranged,Qt::UniqueConnection);

    //trade_table->setHorizontalHeader(headerViews);
    trade_table->setHorizontalHeader(headerViews);
    // Connect the horizontal header's sectionResized signal using a lambda function


    lay_Trade_Window->addWidget(trade_table, 1, 0, 1, 3);

    trade_model = new Trade_Table_Model();
    connect(trade_table, &QTableView::clicked, trade_model, &Trade_Table_Model::selectionChangedSlot);


    trade_table->setObjectName("trade_table");
    Table_OrderBook_Delegate* trade_delegate=new Table_OrderBook_Delegate;
    trade_table->setModel(trade_model);
   // QObject::connect(trade_table->selectionModel(), &QItemSelectionModel::selectionChanged,
    //                 trade_model, &Trade_Table_Model::selectionChangedSlot, Qt::QueuedConnection);

   // QObject::connect(trade_table->selectionModel(), &QItemSelectionModel::selectionChanged,
     //                trade_model, &Trade_Table_Model::selectionChangedSlot); // Qt::AutoConnection is sufficient here


    trade_table->setItemDelegate(trade_delegate);
    //T_order_Delegate =  new trade_table_delegate();
    //trade_table->setItemDelegate(T_order_Delegate);

    tradetableproxyModel = new QSortFilterProxyModel(this);
    tradetableproxyModel->setSourceModel(trade_model);
    tradetableproxyModel->setFilterKeyColumn(-1);  // Search across all columns

    connect(/*ui->lineEditSearch*/line_edit_Executed_search, &QLineEdit::textChanged, this, [=](const QString &text) {
        // proxyModel->setFilterRegularExpression(QRegExp(text, Qt::CaseInsensitive, QRegularExpression::FixedString));
        tradetableproxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
        trade_delegate->setHighlightText(text);
        trade_table->viewport()->update();  // Redraw the view to apply the highlight
    });
    trade_table->setModel(tradetableproxyModel);
    // Configure column resizing
    //trade_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Make all columns resizable by default
   // trade_table->setColumnWidth(1,300);

    trade_table->horizontalHeader()->setStretchLastSection(false);
    trade_table->verticalHeader()->setVisible(false);
    // trade_table->setStyleSheet("QHeaderView { background-color: #111111;} QHeaderView::section { background-color:#555555;color:#eeeeee;font-weight: 400; }");
    trade_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    trade_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    trade_table->setSelectionMode(QAbstractItemView::SingleSelection);
    /*trade_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                            2"selection-color: #4D4D4D;"
                                            "color:#3D3D3D;"
                                            "} "
                                            "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    trade_table->show();
    // Restore the previous state of the table view if any
    restoreTableViewColumnState(trade_table);
    trade_table->setColumnWidth(OrderBook_Idx::AlgoName_OB, 270);
    int Trade_columnCount = trade_table->model()->columnCount();
    for (int col = 0; col < Trade_columnCount; ++col) {
        if (col != OrderBook_Idx::AlgoName_OB) {
            // Set other columns to a width of 100
            trade_table->setColumnWidth(col, 70);
        }
    }


    //connect this signal only after every table init completed, or else it will overwrite table state with defult setting before restore previous state
   // connect(headerViews, &QHeaderView::sectionResized, this, &MainWindow::onTradeTableHeader_Rearranged,Qt::UniqueConnection);

    /***********Init Order Book Window**************************/


    /***********Init F1F2 Order  Window**************************/
    QPixmap pixmapdock_order_f1f2_close(":/dock_close.png");

    dock_win_f1f2_order =  new CDockWidget(tr("Manual Orders"));

    connect(dock_win_f1f2_order, SIGNAL(visibilityChanged(bool)), this, SLOT(OnF1F2OrderDockWidgetVisiblityChanged(bool)));
   // subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_f1f2_order);
   // dock_win_f1f2_order->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock_win_f1f2_order->setStyleSheet(dock_style);
    dock_win_f1f2_order->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_f1f2_order->setMinimumSize(200,150);
    const auto autoHideContainerTradeF1F2 = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_f1f2_order);
    autoHideContainerTradeF1F2->setSize(480);

    //create a titlebar
    QWidget *f1f2_order_titlebar=new QWidget;
    f1f2_order_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *f1f2_order_title_layout=new QHBoxLayout(f1f2_order_titlebar);
    f1f2_order_title_layout->setSpacing(10);
    f1f2_order_title_layout->setContentsMargins(17,8,10,6);
    QLabel *f1f2_order_label=new QLabel("Manual Orders");
    QFont font_f1f2_order_label=f1f2_order_label->font();
    font_f1f2_order_label.setFamily("Work Sans");
    f1f2_order_label->setFont(font_f1f2_order_label);
    f1f2_order_label->setStyleSheet("color: #495057; font-size: 16px; font-style: normal; font-weight: bold; line-height: normal;");
    QSpacerItem* f1f2_order_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_f1f2_order = new QLineEdit;
    line_edit_f1f2_order->setMaximumWidth(160);
    line_edit_f1f2_order->setStyleSheet(lineedit_dock_SS);
    QToolButton* f1f2_order_close=new QToolButton();
    connect(f1f2_order_close, &QToolButton::clicked, [=](){ dock_win_f1f2_order->close();});
    f1f2_order_close->setIcon(pixmapdock_order_f1f2_close);
    f1f2_order_close->setIconSize(QSize(14, 14));
    f1f2_order_title_layout->addWidget(f1f2_order_label);
    f1f2_order_title_layout->addSpacerItem(f1f2_order_spacer);
    f1f2_order_title_layout->addWidget(line_edit_f1f2_order);
    f1f2_order_title_layout->addWidget(f1f2_order_close);
    //dock_win_f1f2_order->setTitleBarWidget(f1f2_order_titlebar);


    QWidget* container_f1f2_order = new QWidget;
    dock_win_f1f2_order->setWidget(container_f1f2_order);
    QGridLayout *lay_F1F2_Order_Window = new QGridLayout(container_f1f2_order);
    lay_F1F2_Order_Window->setContentsMargins(0, 0, 0, 0);
    lay_F1F2_Order_Window->setSpacing(3);
    lay_F1F2_Order_Window->setRowStretch(0, 0); //set minimum hieght for first row
    lay_F1F2_Order_Window->setRowStretch(1, 1);//set maximum hieght for second row table




//    connect(button_Trade, &QPushButton::clicked, this, &MainWindow::tradeTableSerachNext);
//    connect(line_edit_trade, &QLineEdit::textChanged, this, &MainWindow::tradeTableSerachTxtChanged);




    f1f2_order_table = new QTableView();
    connect(f1f2_order_table, &QTableView::doubleClicked, this, &MainWindow::f1f2_order_table_cellDoubleClicked);

  //  f1f2_order_table->setStyleSheet(tableview_SS);
    f1f2_order_table->horizontalHeader()->setFixedHeight(32);
    f1f2_order_table->horizontalHeader()->setFont(headerfont);
    f1f2_order_table->setShowGrid(false);
    f1f2_order_table->setAlternatingRowColors(true);

    order_f1_f2_tableheaderview* f1f2_order_headerViews = new order_f1_f2_tableheaderview(Qt::Horizontal, f1f2_order_table);
    f1f2_order_headerViews->setFixedHeight(32);
    f1f2_order_headerViews->setFont(headerfont);
    f1f2_order_headerViews->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
//    f1f2_order_headerViews->setSectionsMovable(true);
//    f1f2_order_headerViews->setHighlightSections(true);
     connect(f1f2_order_headerViews, &QHeaderView::sectionMoved, this, &MainWindow::onF1F2OrderTableHeader_Rearranged,Qt::UniqueConnection);

    //f1f2_order_table->setHorizontalHeader(f1f2_order_headerViews);
    f1f2_order_table->setHorizontalHeader(f1f2_order_headerViews);
    // Connect the horizontal header's sectionResized signal using a lambda function


    lay_F1F2_Order_Window->addWidget(f1f2_order_table, 1, 0, 1, 3);

    f1f2_order_table_model = new Order_F1_F2_Model();
    f1f2_order_table->setObjectName("f1f2_order_table");
    Order_F1_F2_Delegate* f1f2_order_delegate=new Order_F1_F2_Delegate;
    f1f2_order_table->setModel(f1f2_order_table_model);
    f1f2_order_table->setItemDelegate(f1f2_order_delegate);
    //T_order_Delegate =  new trade_table_delegate();
    //f1f2_order_table->setItemDelegate(T_order_Delegate);


    // Configure column resizing
    //f1f2_order_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Make all columns resizable by default
   // f1f2_order_table->setColumnWidth(1,300);

    f1f2_order_table->horizontalHeader()->setStretchLastSection(false);
    f1f2_order_table->verticalHeader()->setVisible(false);
    // f1f2_order_table->setStyleSheet("QHeaderView { background-color: #111111;} QHeaderView::section { background-color:#555555;color:#eeeeee;font-weight: 400; }");
    f1f2_order_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    f1f2_order_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    f1f2_order_table->setSelectionMode(QAbstractItemView::SingleSelection);
    /*f1f2_order_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                            "selection-color: #4D4D4D;"
                                            "color:#3D3D3D;"
                                            "} "
                                            "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    f1f2_order_table->show();
    restoreTableViewColumnState(f1f2_order_table);
    f1f2_order_table->setColumnWidth(F1F2_Order_Data_Idx::AlgoName_F1F2, 270);
    int f1f2_order_columnCount = f1f2_order_table->model()->columnCount();
    for (int col = 0; col < f1f2_order_columnCount; ++col) {
        if (col != F1F2_Order_Data_Idx::AlgoName_F1F2) {
            // Set other columns to a width of 100
            f1f2_order_table->setColumnWidth(col, 70);
        }
    }


    //connect this signal only after every table init completed, or else it will overwrite table state with defult setting before restore previous state
   // connect(f1f2_order_headerViews, &QHeaderView::sectionResized, this, &MainWindow::onTradeTableHeader_Rearranged,Qt::UniqueConnection);

    /***********Init F1F2 Order  Window**************************/

    /************Positions Window********************************/
    QPixmap pixmapdock_position_close(":/dock_close.png");

    dock_win_net_pos =  new CDockWidget(tr("Net Positions"));
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
    QLabel *position_label=new QLabel("Net Positions");
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


    QWidget* container_net_pos = new QWidget;
    dock_win_net_pos->setWidget(container_net_pos);
    QGridLayout *lay_netpos_Window = new QGridLayout(container_net_pos);
    lay_netpos_Window->setContentsMargins(0, 0, 0, 0);
    lay_netpos_Window->setSpacing(3);
    lay_netpos_Window->setRowStretch(0, 0); //set minimum hieght for first row
    lay_netpos_Window->setRowStretch(1, 1);//set maximum hieght for second row table

    QLineEdit* line_edit_net_pos_search = new QLineEdit;
    line_edit_net_pos_search->setFixedWidth(360);
    line_edit_net_pos_search->setFixedHeight(30);

    QFont fontnet("Work Sans");
    fontnet.setPointSize(9);
    fontnet.setBold(true);
    line_edit_net_pos_search->setFont(fontnet);

    line_edit_net_pos_search->setPlaceholderText("Search...");
    QAction* search_action_netpos = new QAction(QIcon(":/search.png"), "", line_edit_net_pos_search);

    // Add the action to the QLineEdit
    line_edit_net_pos_search->addAction(search_action_netpos, QLineEdit::LeadingPosition);
    line_edit_net_pos_search->setStyleSheet(lineedit_dock_SS);

    QWidget *testnetpos=new QWidget;
    const char laysnetpos[]="background: #E9ECEF;";
    testnetpos->setStyleSheet(laysnetpos);
    QHBoxLayout *internalLayoutnetpos = new QHBoxLayout(testnetpos);
    testnetpos->setLayout(internalLayoutnetpos);
    internalLayoutnetpos->setContentsMargins(10,-1,-1,-1);
    QSpacerItem* spcrr = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    internalLayoutnetpos->addWidget(line_edit_net_pos_search);
    lay_netpos_Window->addWidget(testnetpos, 0, 0);



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
     lay_netpos_Window->addWidget(net_pos_table, 1, 0, 1, 3);

    net_pos_model = new Net_Position_Table_Model();
    net_pos_table->setObjectName("netpos_table");
    net_pos_table->setModel(net_pos_model);
    net_pos_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Make all columns resizable by default
    //net_pos_table->setColumnWidth(1,250);

    net_pos_table->horizontalHeader()->setStretchLastSection(false);
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

    netposproxyModel = new QSortFilterProxyModel(this);
    netposproxyModel->setSourceModel(net_pos_model);
    netposproxyModel->setFilterKeyColumn(-1);  // Search across all columns

    connect(/*ui->lineEditSearch*/line_edit_net_pos_search, &QLineEdit::textChanged, this, [=](const QString &text) {
        // proxyModel->setFilterRegularExpression(QRegExp(text, Qt::CaseInsensitive, QRegularExpression::FixedString));
        netposproxyModel->setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));
        netpos_delegate->setHighlightText(text);
        net_pos_table->viewport()->update();  // Redraw the view to apply the highlight
    });
    net_pos_table->setModel(netposproxyModel);

   net_pos_table->horizontalHeader()->setStretchLastSection(false);
   // dock_win_net_pos->setWidget(net_pos_table);

    net_pos_table->show();
    restoreTableViewColumnState(net_pos_table);
    net_pos_table->setColumnWidth(NET_POS::StockName_NP, 190);
    int netpos_columnCount = net_pos_table->model()->columnCount();
    for (int col = 0; col < netpos_columnCount; ++col) {
        if (col !=NET_POS::StockName_NP) {
            // Set other columns to a width of 100
            net_pos_table->setColumnWidth(col, 70);
        }
    }

    /************Net Position Window********************************/


    /************Liners Window********************************/
    QPixmap pixmapdock_Liners_close(":/dock_close.png");

    dock_win_liners =  new CDockWidget(tr("Algo Position"));
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
    liners_delegate* liner_delegate = new liners_delegate;
    liners_table->setObjectName("liners_table");
    liners_table->setModel(liners_model);
    liners_table->setItemDelegate(liner_delegate);

    // Disable the global Interactive setting for now
    // liners_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // Comment this out

    liners_table->horizontalHeader()->setStretchLastSection(false);
    liners_table->verticalHeader()->setVisible(false);

    // Set up selection behavior
    liners_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    liners_table->setSelectionMode(QAbstractItemView::SingleSelection);

    dock_win_liners->setWidget(liners_table);
    liners_table->show();
    restoreTableViewColumnState(liners_table);
    liners_table->setColumnWidth(Liners_Idx::AlgoName, 270);

    int liners_columnCount = liners_table->model()->columnCount();
    for (int col = 0; col < liners_columnCount; ++col) {
        if (col !=Liners_Idx::AlgoName) {
            // Set other columns to a width of 100
            liners_table->setColumnWidth(col, 70);
        }
    }




    /************Liners Window********************************/





    /************Historical Positions Window********************************/
    QPixmap pixmapdock_hp_close(":/dock_close.png");

    dock_win_combined_tracker =  new CDockWidget(tr("Historical Positions"));

    connect(dock_win_combined_tracker, SIGNAL(visibilityChanged(bool)), this, SLOT(OnHPDockWidgetVisiblityChanged(bool)));
    //dock_win_combined_tracker->setAllowedAreas(Qt::AllDockWidgetAreas );
    dock_win_combined_tracker->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_combined_tracker->setMinimumSize(200,150);
//    const auto autoHideContainer_tracker = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_combined_tracker);
//    autoHideContainer_tracker->setSize(480);

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
    combined_tracker_table->setObjectName("combined_tracker_table");

    combined_tracker_table->setStyleSheet(tableview_SS);
    combined_tracker_table->horizontalHeader()->setFixedHeight(32);
    combined_tracker_table->horizontalHeader()->setFont(headerfont);
    combined_tracker_table->setShowGrid(false);
    combined_tracker_table->setAlternatingRowColors(true);

    combined_tracker_model = new Combined_Tracker_Table_Model();
    combined_tracker_table->setModel(combined_tracker_model);
    combined_tracker_table->horizontalHeader()->setStretchLastSection(false);
    combined_tracker_table->verticalHeader()->setVisible(false);
    combined_tracker_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    combined_tracker_table->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(combined_tracker_table->horizontalHeader(), &QHeaderView::sectionMoved, this, &MainWindow::onCombined_tracker_tableHeader_Rearranged,Qt::UniqueConnection);
    dock_win_combined_tracker->setWidget(combined_tracker_table);
    combined_tracker_table->show();
    restoreTableViewColumnState(combined_tracker_table);

    /************Historical Positions********************************/


    /************Open Positions Window********************************/
    //QPixmap pixmapdock_hp_close(":/dock_close.png");

    dock_win_Open_position =  new CDockWidget(tr("Open Positions"));

    connect(dock_win_Open_position, SIGNAL(visibilityChanged(bool)), this, SLOT(OnOpenDockWidgetVisiblityChanged(bool)));
    //dock_win_combined_tracker->setAllowedAreas(Qt::AllDockWidgetAreas );
    dock_win_Open_position->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    dock_win_Open_position->setMinimumSize(200,150);
    const auto autoHideContainer_tracker = DockManagerMainPanel->addAutoHideDockWidget(SideBarLocation::SideBarLeft, dock_win_Open_position);
    //    autoHideContainer_tracker->setSize(480);

    //create a titlebar
    QWidget *open_titlebar=new QWidget;
    open_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *position_open_layout=new QHBoxLayout(open_titlebar);
    position_open_layout->setSpacing(10);
    position_open_layout->setContentsMargins(17,8,10,6);
    QLabel *open_label=new QLabel("Open Positions");
    QFont font_open_label=open_label->font();
    font_open_label.setFamily("Work Sans");
    open_label->setFont(font_open_label);
    open_label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
    QSpacerItem* open_spacer=new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLineEdit* line_edit_open = new QLineEdit;
    line_edit_open->setMaximumWidth(160);
    line_edit_open->setStyleSheet(lineedit_dock_SS);
    QToolButton* open_close=new QToolButton();
    connect(open_close, &QToolButton::clicked, [=](){ dock_win_combined_tracker->close(); });
    open_close->setIcon(pixmapdock_hp_close);
    open_close->setIconSize(QSize(14, 14));
    position_open_layout->addWidget(open_label);
    position_open_layout->addSpacerItem(open_spacer);
    position_open_layout->addWidget(line_edit_open);
    position_open_layout->addWidget(open_close);
    //dock_win_combined_tracker->setTitleBarWidget(hp_titlebar);



    //subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_combined_tracker);

    open_position = new QTableView(dock_win_Open_position);
    open_position->setObjectName("Open_Position");

    open_position->setStyleSheet(tableview_SS);
    open_position->horizontalHeader()->setFixedHeight(32);
    open_position->horizontalHeader()->setFont(headerfont);
    open_position->setShowGrid(false);
    open_position->setAlternatingRowColors(true);


    open_position_headerview* open_headerViewrs = new open_position_headerview(Qt::Horizontal, liners_table);
    open_headerViewrs->setFixedHeight(32);
    open_headerViewrs->setFont(headerfont);
    open_headerViewrs->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
   // connect(headerViewrs, &QHeaderView::sectionMoved, this, &MainWindow::onLinersTableHeader_Rearranged);
    open_position->setHorizontalHeader(open_headerViewrs);


    OpenPositionmodel = new open_position_model();
    //open_position->setModel(OpenPositionmodel);

    open_position_delegate* open_delegate =new open_position_delegate;
    open_position->setModel(OpenPositionmodel);
    open_position->setItemDelegate(open_delegate);

    open_position->horizontalHeader()->setStretchLastSection(false);
    open_position->verticalHeader()->setVisible(false);
    open_position->setSelectionBehavior(QAbstractItemView::SelectRows);
    open_position->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(open_position->horizontalHeader(), &QHeaderView::sectionMoved, this, &MainWindow::onOpen_Position_tableHeader_Rearranged,Qt::UniqueConnection);
    dock_win_Open_position->setWidget(open_position);
    open_position->show();
    restoreTableViewColumnState(open_position);
    open_position->setColumnWidth(Open_Position_Idx::StockName, 190);

    /************Open Positions********************************/



    /************Missed Trades Window********************************/
    QPixmap pixmapdock_mt_close(":/dock_close.png");

    dock_win_missed_trades =  new CDockWidget(tr("Skipped Orders"));
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
    QLabel *mt_label=new QLabel("Skipped Orders");
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
    missed_trade_table->setObjectName("missed_trade_table");
    connect(missed_trade_table, &QTableView::doubleClicked, this, &MainWindow::Skipped_Trades_cellDoubleClicked);

    missed_trade_table_delegate* missed_delegate =new missed_trade_table_delegate;
    connect(missed_delegate, &missed_trade_table_delegate::retryButtonClicked, this, &MainWindow::onRetryButtonClickedMissedTrade);

    missed_trade_table->setItemDelegate(missed_delegate);


    missed_trade_table->setStyleSheet(tableview_SS);
    missed_trade_table->horizontalHeader()->setFixedHeight(32);
    missed_trade_table->horizontalHeader()->setFont(headerfont);
    missed_trade_table->setShowGrid(false);
    missed_trade_table->setAlternatingRowColors(true);


    missed_trades_headerview* headerView_missedtrades = new missed_trades_headerview(Qt::Horizontal, missed_trade_table);
    headerView_missedtrades->setFixedHeight(32);
    headerView_missedtrades->setFont(headerfont);
    headerView_missedtrades->setStyleSheet(
        "   background: #495867;"
        "   border: none;"
        "   color: #FFF; "
        "   text-align: center; "
        "   font-size: 12px; "
        "   font-style: normal; "
        "   font-weight: 600; "
        "   line-height: normal;"
        );
    connect(headerView_missedtrades, &QHeaderView::sectionMoved, this, &MainWindow::onMissed_trade_tableHeader_Rearranged);
    missed_trade_table->setHorizontalHeader(headerView_missedtrades);

    missed_trade_model = new Missed_Trade_Table_Model();
    missed_trade_table->setModel(missed_trade_model);
    missed_trade_table->horizontalHeader()->setStretchLastSection(false);
    missed_trade_table->verticalHeader()->setVisible(false);
    missed_trade_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    missed_trade_table->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(missed_trade_table->horizontalHeader(), &QHeaderView::sectionMoved, this, &MainWindow::onMissed_trade_tableHeader_Rearranged, Qt::UniqueConnection);

    dock_win_missed_trades->setWidget(missed_trade_table);
    missed_trade_table->show();
    restoreTableViewColumnState(missed_trade_table);
    missed_trade_table->setColumnWidth(Missed_Trades_Idx::Stock_Name, 190);

//    int missedtrades_columnCount = missed_trade_table->model()->columnCount();
//    for (int col = 0; col < missedtrades_columnCount; ++col) {
//        if (col !=Liners_Idx::AlgoName) {
//            // Set other columns to a width of 100
//            missed_trade_table->setColumnWidth(col, 70);
//        }
//    }

      /************Missed Trades ********************************/



    QFile styleFile(":/style/Style_Home.qss");
    styleFile.open(QFile::ReadOnly);
    QString mainstyleSheet = QLatin1String(styleFile.readAll());
    this->setStyleSheet(mainstyleSheet);

    //logo at the top left
    QPixmap pixmaplogo(":/Am Logomark - Standard.png");
    int labelWidth = ui->label->width();
    int labelHeight = ui->label->height();
    pixmaplogo = pixmaplogo.scaled(labelWidth * 0.9, labelHeight * 0.9, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmaplogo);
    ui->label->setAlignment(Qt::AlignCenter); // Ensure it's centered




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



//    ui->lineEditWatchSearch->setPlaceholderText("Search...");
//    ui->lineEditWatchSearch->setStyleSheet("font-family: 'Work Sans';"
//                                           "font-size: 12px;"
//                                           "font-weight: 400;"
//                                           );
    //QPixmap pixmapmenu(":/resized_image1.png");
    //QPixmap pixmapmenu = style()->standardPixmap(QStyle::SP_FileDialogDetailedView);

    ui->close->setIcon(pixmapclose);
    ui->minimize->setIcon(pixmapminimize);
    ui->maximize->setIcon(pixmapmaximize);
    //ui->menu->setIcon(pixmapmenu);

    //setting icons for top window widget
       //QPixmap pixmapbuttonclose = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QPixmap pixmapbuttonclose(":/dock_close.png");
   // ui->Algorithms_Close->setIcon(pixmapbuttonclose);
   // ui->OrderBook_Close->setIcon(pixmapbuttonclose);
  //  ui->Positions_Close->setIcon(pixmapbuttonclose);
  //  ui->Liners_Close->setIcon(pixmapbuttonclose);
  //  ui->HP_Close->setIcon(pixmapbuttonclose);
  //  ui->MissedTrade_Close->setIcon(pixmapbuttonclose);
  //  ui->Templates_Close->setIcon(pixmapbuttonclose);

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

   // new QShortcut(QKeySequence(Qt::Key_Delete), this,  SLOT(Delete_clicked_slot()));
   // new QShortcut(QKeySequence(Qt::Key_F1), this,  SLOT(F1_clicked_slot()));
   // new QShortcut(QKeySequence(Qt::Key_F2), this,  SLOT(F2_clicked_slot()));
    // db_conn =new mysql_conn(this,"main_db_conn");
    // connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));
   //loadContract();
    //initWatchWindow();
    /***********Initlaize Hotkey here*************/
    initializeGlobalHotKeys();
    /*********************************************/
    instilizeWatchUIOnTopBar();
  /*  TableRefreshTimer = new QTimer(this);
    TableRefreshTimer->setSingleShot(true);  // Make it behave like singleShot
    connect(TableRefreshTimer, &QTimer::timeout, this, &MainWindow::start_dataLoadingThread);*/
    /*Trade_TableRefreshTimer = new QTimer(this);
    connect(Trade_TableRefreshTimer, &QTimer::timeout, this, [this]() {
        QtConcurrent::run(&MainWindow::refreshTradeTable, this);
    });*/
    QObject::connect(qApp, &QApplication::focusChanged,
                     [this](QWidget* oldWidget, QWidget* newWidget) {
                     if(newWidget&&oldWidget){
//            qDebug()<<"newWidget: "<<newWidget->objectName();
//            qDebug()<<"oldWidget: "<<oldWidget->objectName();
                         if(oldWidget == T_Portfolio_Table &&  newWidget->objectName() == "duplicate_bt_id"){
                             //do nothing
                         }
                         else if(newWidget != T_Portfolio_Table){
                             if(!newWidget->objectName().contains("portfolioTableCell_"))
                             T_Portfolio_Table->clearSelection();
                         }
                         else{
                         }
                        /* if(newWidget == net_pos_table ||
                             newWidget == liners_table ||
                             newWidget == combined_tracker_table ||
                             newWidget == trade_table ||
                             newWidget == missed_trade_table ||
                             newWidget == f1f2_order_table ||
                             newWidget->objectName() == "startallbutton" ||
                             newWidget->objectName() == "stopallbutton" ||
                             newWidget->objectName() == "buildalgo" ||
                             newWidget->objectName() == "import" ||
                             newWidget->objectName() == "sorting" ||
                             newWidget->objectName() == "refresh" ||
                             newWidget->objectName() == "export" ||
                             newWidget->objectName() == "filter" //||
                            // newWidget->objectName() == "summarycard" ||
                           //  newWidget->objectName() == "logcard" ||
                            // newWidget->objectName() == "watchcard"
                             ){
                             //clear selected rows of portfolio table
                             T_Portfolio_Table->clearSelection();
                         }*/
                     }

                     });
    //Read
    //CacheFileIO = new cache_file_io();
    //tradedDataListDisabledOnMaxLoss_Thr_Reach = CacheFileIO->readTradedDataList_DisabledOn_MaxLoss_Thr_Reach(); // this list contain algos whcih will excluded while autmatic disable on  ExchangePriceLimit reach
}


MainWindow::~MainWindow()
{
    if (sortWin) {
        delete sortWin; // Ensure the window is deleted
    }
#ifdef BACKEND_LOG

    write_backend_Log("Log closed");
       if (backend_logFile.isOpen()) {
           backend_logFile.close();
       }
#endif
       delete ui;

}

void MainWindow::update_ui_slot(int type){

    // this should be called after all dock widget are set
    restoreDockManagerState();

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
        settings.setValue("mult_interface_ip","192.168.15.63");
        settings.setValue("fo_mult_ip","233.1.2.5");
        settings.setValue("fo_mult_port","34330");
        settings.setValue("cm_mult_ip","233.1.2.5");
        settings.setValue("cm_mult_port","34330");
        settings.endGroup();
    }
    else{
        settings.beginGroup("ExchangeServerDetails");
        if (!settings.contains("mult_interface_ip"))
            settings.setValue("mult_interface_ip", "192.168.15.63");
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
        settings.setValue("Watch_InstrumentNames_MainWin","");
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
        if (!settings.contains("Watch_InstrumentNames_MainWin"))
            settings.setValue("Watch_InstrumentNames_MainWin","");
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

    //create them sett file if not exist
    QDir dir1(appDataPath+"/Data");
    // Check if the directory already exists
    if (!dir1.exists()) {
        // If it doesn't exist, create it
        if (dir1.mkpath(".")) {
            qDebug() << "App Data Directory created:" << appDataPath+"/Data";
        } else {
            qDebug() << "Error creating directory:" << appDataPath+"/Data";
        }
    }
    QString theme_fileName = appDataPath+"/Data/theme_sett.ini";

    //create INI File if not exist
    if (!QFile(theme_fileName).exists()){
        QSettings settings(theme_fileName, QSettings::IniFormat);
        settings.beginGroup("font_sett");
        settings.setValue("Family", "Work Sans");
        settings.setValue("PointSize", 9);
        settings.setValue("Bold", true);
        settings.setValue("Italic", true);
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
//        int activatedCount = T_Portfolio_Model->getActivatedPortfolioCount(); // Correct type: int
//        if (activatedCount >= userData.MaxActiveCount) {
//                //show the message here
//                qDebug() << "profolioTableEditFinshedSlot----- no active portfolio found.";
//                return;
//        }
//        qDebug() << "Activated portfolio count:" << activatedCount;


        portfolio_table_updating_db.storeRelaxed(1);

        PortfolioObject *P =  T_Portfolio_Model->getPortFolioAt(index.row());//portfolioModal->portfolio_data_list.at(index.row());
        if (!P) {
            qDebug()<<"profolioTableEditFinshedSlot----- portfolio null, need to debug this code.";
            portfolio_table_updating_db.storeRelaxed(0);

            return;
        }

        QString PortfolioNumber = QString::number(P->PortfolioNumber);//T_Portfolio_Model->index(index.row(),PortfolioData_Idx::_PortfolioNumber).data().toString();
        T_Portfolio_Model->setEditingFlg(index.row(),0);


         /*if (index.column() == PortfolioData_Idx::_Status) {
            if (valStr == "0") {
            QString Query = "UPDATE Portfolios SET Status='DisabledByUser' WHERE PortfolioNumber=" + PortfolioNumber;
            QString msg;
            bool success = db_conn->updateDB_Table(Query, msg);
            if (success) {
                db_conn->logToDB(QString("Disabled portfolio [" + PortfolioNumber + "]"));
                reloadSortSettFlg.storeRelaxed(1);  // Refresh sorting
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Update Status Failed");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(msg);
                msgBox.exec();
            }
            } else {
            QString Query = "UPDATE Portfolios SET Status='Active' WHERE PortfolioNumber=" + PortfolioNumber;
            QString msg;
            bool success = db_conn->updateDB_Table(Query, msg);
            if (success) {
                db_conn->logToDB(QString("Activated portfolio [" + PortfolioNumber + "]"));
                reloadSortSettFlg.storeRelaxed(1);  // Refresh sorting
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Update Status Failed");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(msg);
                msgBox.exec();
            }
            }
        }*/



        /*else if(index.column() == PortfolioData_Idx::_Alias) {
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
        else if(index.column() == PortfolioData_Idx::_MaxLoss) {
            // Skip checking for duplicate alias names
            double val = valStr.toDouble();
            int valInt = val*devicer;
            QString Query = "UPDATE Portfolios SET MaxLoss='" + QString::number(valInt) + "' WHERE PortfolioNumber=" + PortfolioNumber;
            QString msg;
            bool success = db_conn->updateDB_Table(Query, msg);

            if (success) {
                db_conn->logToDB(QString("Updated MaxLoss to " + valStr + " [" + PortfolioNumber + "]"));
                // Refresh sorting
                reloadSortSettFlg.storeRelaxed(1);
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Update MaxLoss Failed");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(msg);
                msgBox.exec();
            }
        }*/

        //Get edited cell data from portfolio table and update DB
       // else{
            QHash<int, Cell_Cache> editedCellData(T_Portfolio_Model->editingDataHash);
            QHash<int, Cell_Cache>::const_iterator i;
            QStringList updateQueryList;
            QStringList oq_btq_stq_msg;
            QString logMsg = "";
            QString InstrumentType  = ContractDetail::getInstance().GetInstrumentType(P->Leg1TokenNo,P->PortfolioType.toInt());
            int OpenLimit = userData.IDXOpenLimit;
            QString openLimitType = "IDXOpenLimit";
            if (InstrumentType == "OPTSTK" || InstrumentType == "FUTSTK"){
                 OpenLimit =  userData.STKOpenLimit;
                 openLimitType = "STKOpenLimit";
            }

            int order_qty_limit = 10;
            int lotSize = P->GetLotSize();//T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
            int OpenLimitThrBuy = OpenLimit+P->BuyTradedQuantity;
            int OpenLimitThrSell = OpenLimit+P->SellTradedQuantity;
            double BTTQ = static_cast<double>(P->BuyTradedQuantity);
            double STTQ = static_cast<double>(P->SellTradedQuantity);

            ///----------------_OrderQuantity and _BuyTotalQuantity and _SellTotalQuantity edited-------------
            if(editedCellData.contains(PortfolioData_Idx::_OrderQuantity)&&
                    editedCellData.contains(PortfolioData_Idx::_BuyTotalQuantity)&&
                    editedCellData.contains(PortfolioData_Idx::_SellTotalQuantity)){

                double OQ = editedCellData[_OrderQuantity].newVal.toDouble();
                double BTQ = editedCellData[_BuyTotalQuantity].newVal.toDouble();
                double STQ = editedCellData[_SellTotalQuantity].newVal.toDouble();


                if(BTQ<BTTQ){
                    oq_btq_stq_msg.append("BTQ "+QString::number(BTQ)+" is less than  BTTQ " + QString::number(BTTQ));
                }
                else if(STQ<STTQ){
                    oq_btq_stq_msg.append("STQ "+QString::number(STQ)+" is less than  STTQ " + QString::number(STTQ));
                }

                //if BTQ and STQ greater than OpenLimitThr, do not update BTQ STQ and OQ
                else if(BTQ>OpenLimitThrBuy&&STQ>OpenLimitThrSell){
                    oq_btq_stq_msg.append("BTQ/STQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                }
                // BTQ do not satifsy condtion  STQ statisfy, so update STQ
                else if(BTQ>OpenLimitThrBuy&&STQ<=OpenLimitThrSell){
                    oq_btq_stq_msg.append("BTQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                    updateQueryList.append("SellTotalQuantity="+QString::number(STQ*lotSize));
                    logMsg = logMsg+"SellTotalQuantity ["+QString::number(STQ)+"], ";

                    if(OQ > order_qty_limit)
                       OQ = 1;
                    updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                    logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";


                }
                // STQ do not satifsy condtion  BTQ statisfy, so update STQ
                else if(BTQ<=OpenLimitThrBuy&&STQ>OpenLimitThrSell){
                    oq_btq_stq_msg.append("STQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                    updateQueryList.append("BuyTotalQuantity="+QString::number(BTQ*lotSize));
                    logMsg = logMsg+"BuyTotalQuantity ["+QString::number(BTQ)+"], ";
                    if(OQ > order_qty_limit)
                       OQ = 1;
                    updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                    logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";


                }
                else{
                    updateQueryList.append("BuyTotalQuantity="+QString::number(BTQ*lotSize));
                    updateQueryList.append("SellTotalQuantity="+QString::number(STQ*lotSize));
                    logMsg = logMsg+"BuyTotalQuantity ["+QString::number(BTQ)+"], ";
                    logMsg = logMsg+"SellTotalQuantity ["+QString::number(STQ)+"], ";

                    if(OQ > order_qty_limit)
                       OQ = 1;
                    updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                    logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";

                    //else{
                      //  oq_btq_stq_msg.append("OrderQuantity cannot be greater than "+ QString::number(order_qty_limit));
                  //  }
                }

            }
            ///----------------------------------------------------------------------

            ///------only _OrderQuantity and _BuyTotalQuantity edited-------------
            else if(editedCellData.contains(PortfolioData_Idx::_OrderQuantity)&&
                                  editedCellData.contains(PortfolioData_Idx::_BuyTotalQuantity)){
                double OQ = editedCellData[_OrderQuantity].newVal.toDouble();
                double BTQ = editedCellData[_BuyTotalQuantity].newVal.toDouble();


                if(BTQ<BTTQ){
                    oq_btq_stq_msg.append("BTQ "+QString::number(BTQ)+" is less than  BTTQ " + QString::number(BTTQ));
                }

                else if(BTQ>OpenLimitThrBuy){
                    oq_btq_stq_msg.append("BTQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                    if(OQ <= order_qty_limit){
                       updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                       logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";
                    }
                }
                else{
                    updateQueryList.append("BuyTotalQuantity="+QString::number(BTQ*lotSize));
                    logMsg = logMsg+"BuyTotalQuantity ["+QString::number(BTQ)+"], ";
                    if(OQ > order_qty_limit)
                       OQ = 1;
                    updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                    logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";

                }
            }
            ///----------------------------------------------------------------------

            ///------only _OrderQuantity and _SellTotalQuantity edited-------------
            else if(editedCellData.contains(PortfolioData_Idx::_OrderQuantity)&&
                                  editedCellData.contains(PortfolioData_Idx::_SellTotalQuantity)){
                double OQ = editedCellData[_OrderQuantity].newVal.toDouble();
                double STQ = editedCellData[_SellTotalQuantity].newVal.toDouble();

                if(STQ<STTQ){
                    oq_btq_stq_msg.append("STQ "+QString::number(STQ)+" is less than  STTQ " + QString::number(STTQ));
                }
                else if(STQ>OpenLimitThrSell){
                    oq_btq_stq_msg.append("STQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                    if(OQ <= order_qty_limit){
                       updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                       logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";
                    }
                }
                else{
                    updateQueryList.append("SellTotalQuantity="+QString::number(STQ*lotSize));
                    logMsg = logMsg+"SellTotalQuantity ["+QString::number(STQ)+"], ";
                    if(OQ > order_qty_limit)
                       OQ = 1;
                    updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                    logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";


                }
            }
            ///----------------------------------------------------------------------


            ///------only _BuyTotalQuantity and _SellTotalQuantity edited-------------
            else if(editedCellData.contains(PortfolioData_Idx::_BuyTotalQuantity)&&
                                  editedCellData.contains(PortfolioData_Idx::_SellTotalQuantity)){
                double BTQ = editedCellData[_BuyTotalQuantity].newVal.toDouble();
                double STQ = editedCellData[_SellTotalQuantity].newVal.toDouble();

                if(BTQ<BTTQ){
                    oq_btq_stq_msg.append("BTQ "+QString::number(BTQ)+" is less than  BTTQ " + QString::number(BTTQ));
                }
                else if(STQ<STTQ){
                    oq_btq_stq_msg.append("STQ "+QString::number(STQ)+" is less than  STTQ " + QString::number(STTQ));
                }
                else if(BTQ>OpenLimitThrBuy&&STQ>OpenLimitThrSell){
                    oq_btq_stq_msg.append("BTQ/STQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                }
                else if(BTQ>OpenLimitThrBuy&&STQ<=OpenLimitThrSell){
                    oq_btq_stq_msg.append("BTQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                    updateQueryList.append("SellTotalQuantity="+QString::number(STQ*lotSize));
                    logMsg = logMsg+"SellTotalQuantity ["+QString::number(STQ)+"], ";

                    if(P->OrderQuantity==0){
                        updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                        logMsg = logMsg+"OrderQuantity ["+QString::number(1)+"], ";
                    }
                }
                else if(BTQ<=OpenLimitThrBuy&&STQ>OpenLimitThrSell){
                    oq_btq_stq_msg.append("STQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                    updateQueryList.append("BuyTotalQuantity="+QString::number(BTQ*lotSize));
                    logMsg = logMsg+"BuyTotalQuantity ["+QString::number(BTQ)+"], ";
                    if(P->OrderQuantity==0){
                        updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                        logMsg = logMsg+"OrderQuantity ["+QString::number(1)+"], ";
                    }
                }
                //both BTQ and STQ statisy
                else{
                    updateQueryList.append("BuyTotalQuantity="+QString::number(BTQ*lotSize));
                    logMsg = logMsg+"BuyTotalQuantity ["+QString::number(BTQ)+"], ";
                    updateQueryList.append("SellTotalQuantity="+QString::number(STQ*lotSize));
                    logMsg = logMsg+"SellTotalQuantity ["+QString::number(STQ)+"], ";
                    if(P->OrderQuantity==0){
                        updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                        logMsg = logMsg+"OrderQuantity ["+QString::number(1)+"], ";
                    }
                }


            }
            ///----------------------------------------------------------------------

            ///----------------_OrderQuantity  only edited-------------

            else if(editedCellData.contains(PortfolioData_Idx::_OrderQuantity)){

                double OQ = editedCellData[_OrderQuantity].newVal.toDouble();
                double BTQ_ = static_cast<double>(P->BuyTotalQuantity);
                double STQ_ = static_cast<double>(P->SellTotalQuantity);

                if(BTQ_==0&&STQ_==0&&OQ>0){
                    oq_btq_stq_msg.append("OrderQuantity cannot be greater than 0 if both BTQ and STQ are zero.");
                }
                else if(OQ <= order_qty_limit){
                   if(OQ==0)
                       OQ = 1;
                   updateQueryList.append("OrderQuantity="+QString::number(OQ*lotSize));
                   logMsg = logMsg+"OrderQuantity ["+QString::number(OQ)+"], ";
                }
                else{
                    oq_btq_stq_msg.append("OrderQuantity cannot be greater than "+ QString::number(order_qty_limit));
                }
            }

            ///------only _BuyTotalQuantity edited-------------
            else if(editedCellData.contains(PortfolioData_Idx::_BuyTotalQuantity)){
                double BTQ = editedCellData[_BuyTotalQuantity].newVal.toDouble();

                if(BTQ<BTTQ){
                    oq_btq_stq_msg.append("BTQ "+QString::number(BTQ)+" is less than  BTTQ " + QString::number(BTTQ));
                }

                else if(BTQ>OpenLimitThrBuy){
                    oq_btq_stq_msg.append("BTQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                }
                else{
                    updateQueryList.append("BuyTotalQuantity="+QString::number(BTQ*lotSize));
                    logMsg = logMsg+"BuyTotalQuantity ["+QString::number(BTQ)+"], ";
                    if(P->OrderQuantity==0){
                        updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                        logMsg = logMsg+"OrderQuantity ["+QString::number(1)+"], ";
                    }
                }
            }
            ///----------------------------------------------------------------------

            ///------only _SellTotalQuantity edited-------------
            else if(editedCellData.contains(PortfolioData_Idx::_SellTotalQuantity)){
                double STQ = editedCellData[_SellTotalQuantity].newVal.toDouble();

                if(STQ<STTQ){
                    oq_btq_stq_msg.append("STQ "+QString::number(STQ)+" is less than  STTQ " + QString::number(STTQ));
                }
                else if(STQ>OpenLimitThrSell){
                    oq_btq_stq_msg.append("STQ should not be greater than "+openLimitType+" " + QString::number(OpenLimit));
                }
                else{
                    updateQueryList.append("SellTotalQuantity="+QString::number(STQ*lotSize));
                    logMsg = logMsg+"SellTotalQuantity ["+QString::number(STQ)+"], ";
                    if(P->OrderQuantity==0){
                        updateQueryList.append("OrderQuantity="+QString::number(lotSize));
                        logMsg = logMsg+"OrderQuantity ["+QString::number(1)+"], ";
                    }
                }


            }
            ///----------------------------------------------------------------------



            for (i = editedCellData.constBegin(); i != editedCellData.constEnd(); ++i) {
                int columnIdx = i.key();
                QString valStr = i.value().newVal;
                switch (columnIdx) {

                    case PortfolioData_Idx::_BuyPriceDifference:{
                        QString BuyPriceDifference = QString::number(P->BuyPriceDifference*devicer,'f',decimal_precision);
                        updateQueryList.append("BuyPriceDifference="+BuyPriceDifference);
                        logMsg = logMsg+"BuyPriceDifference ["+valStr+"], ";

                    }
                    break;

                    case PortfolioData_Idx::_SellPriceDifference:{
                        QString SellPriceDifference = QString::number(P->SellPriceDifference*devicer,'f',decimal_precision);
                        updateQueryList.append("SellPriceDifference="+SellPriceDifference);
                        logMsg = logMsg+"SellPriceDifference ["+valStr+"], ";
                    }
                    break;

                    case PortfolioData_Idx::_Alias:{
                        updateQueryList.append("Alias='"+valStr+"'");
                        logMsg = logMsg+"Alias ["+valStr+"], ";
                    }
                    break;

                    case PortfolioData_Idx::_Depth: {
                        double depth = editedCellData[_Depth].newVal.toDouble();
                        double oldDepth = editedCellData[_Depth].prevVal.toDouble(); // Retrieve old value

                        if (depth > 5) {
                       oq_btq_stq_msg.append("Depth  cannot be greater than 5");
                        depth = oldDepth; // Restore old value

                        }

                        updateQueryList.append("DepthConfigured=" + QString::number(depth));
                        logMsg = logMsg + "DepthConfigured [" + QString::number(depth) + "], ";
                    }
                    break;

                    case PortfolioData_Idx::_MaxLoss:{
                        double val = valStr.toDouble();
                        int valInt = val*devicer;
                        updateQueryList.append("MaxLoss="+QString::number(valInt));
                        logMsg = logMsg+"MaxLoss ["+valStr+"], ";
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
                    db_conn->logToDB( "Algo No="+PortfolioNumber+" "+"Updated With"+" "+logMsg);

                    //send notifcation to backend server
                    quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
                    const unsigned char dataBytes[] = { 0xFF, 0xFF };
                    QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 2);
                    QByteArray msg = backend_comm->createPacket(command, data);
                    backend_comm->insertData(msg);

                }
                else{
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Update DB Failed");
                    msgBox.setIcon(QMessageBox::Warning);
                    msgBox.setText(msg);
                    msgBox.exec();
                }
            }

      //  }

            if(oq_btq_stq_msg.size()){
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Cannot update database");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(oq_btq_stq_msg.join(", "));
                msgBox.exec();
            }
            portfolio_table_updating_db.storeRelaxed(0);
            triggerImmediate_refreshTables();


            delete P;

}
void MainWindow::loadContract(){



    auto loadContract_BackgroundTask = [this]() {

        QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.0;'>"
                              "<span>" + QTime::currentTime().toString("hh:mm:ss")
                              + "&nbsp;</span><span style='font-weight:400;/*color: black;*/'> file loading... </span></p>";

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
                      + "&nbsp;</span><span style='font-weight:400;/*color: black;*/'> file loaded </span></p>";


        emit display_log_text_signal(htmlContent);

        // to hide progress bar winodw
        emit signalHideProgressBar();

        foo_token_number_start_strike = "";

        if(loggedInFlg.loadRelaxed()==1){
            start_dataLoadingThread();
            QMetaObject::invokeMethod(this, [this]() { onSummaryActionTriggered();}, Qt::QueuedConnection);

            //QMetaObject::invokeMethod(summaryAction, "trigger", Qt::QueuedConnection);
        }
    };
    emit update_ui_signal(LOADED_MODEL);
    QFuture<void> future = QtConcurrent::run(loadContract_BackgroundTask);
}

void MainWindow::start_dataLoadingThread(){

    futureBackGroundDBThread = QtConcurrent::run(&MainWindow::refreshTables,this);

}

void MainWindow::stop_dataLoadingThread(){

    quit_db_data_load_thread.storeRelaxed(1); // this will quit the data loding thread.
    DataLoadMutex.lock();
       waitConditionDataLoadThread.wakeAll();
       DataLoadMutex.unlock();
    futureBackGroundDBThread.waitForFinished();
}
/*
void MainWindow::startTableRefreshTimer() {
    // This method runs in the main thread, and starts the QTimer safely
    int timeOut = 2000;
    if (!TableRefreshTimer->isActive()) {
        TableRefreshTimer->start(timeOut);  // Start the timer for 2 seconds
        qDebug() << "TableRefreshTimer started...";
    } else {
        qDebug() << "TableRefreshTimer is already running, skipping this call.";
    }
}*/

/*void MainWindow::refreshTradeTable(){
    qDebug()<<"refresh Trade-Table thread.....";

     int timeOut = 2000;

    if(data_loading_thread_running.loadRelaxed()==1){
        QMetaObject::invokeMethod(this, "startTableRefreshTimer", Qt::QueuedConnection);
        qDebug()<<"Trade data loading thread already running, skipping.....";
        return;
    }

    ///check algos are deleting if so do not load.
    if(deletingPortFolioFlg.loadRelaxed()==1){
        QMetaObject::invokeMethod(this, "startTableRefreshTimer", Qt::QueuedConnection);
        qDebug()<<" PortFolios delete is going in, table refresh will do in next "<<timeOut<<" ms.....";
        return;
    }

   //QElapsedTimer timer;
   // timer.start();

    data_loading_thread_running.storeRelaxed(1);
    loadDataAndUpdateTable(T_Table::TRADE);
    qDebug()<<"Exiting refresh Trade-Table loading thread.....";
    data_loading_thread_running.storeRelaxed(0);
    //qDebug() << "refreshTradeTable took :" << timer.elapsed() << "milliseconds";

}*/

void MainWindow::refreshTables(){
    qDebug()<<"------------Started data loading thread.....";

   // int timeOut = 2000;

    /*if(data_loading_thread_running.loadRelaxed()==1){
        QMetaObject::invokeMethod(this, "startTableRefreshTimer", Qt::QueuedConnection);
        qDebug()<<"------------data loading thread already running, skipping.....";
        return;
    }

    ///check algos are deleting if so do not load.
    if(deletingPortFolioFlg.loadRelaxed()==1){
        QMetaObject::invokeMethod(this, "startTableRefreshTimer", Qt::QueuedConnection);
        qDebug()<<" PortFolios delete is going in, table refresh will do in next "<<timeOut<<" ms.....";
        return;
    }*/

   // data_loading_thread_running.storeRelaxed(1);

    QElapsedTimer timer;

    int data_load_interval = 60000; // in m-sec
    while(1){


        timer.restart();
        if(refresh_entire_table.loadRelaxed()==1){
            qDebug()<<"Refreshing entire table.....";

            loadDataAndUpdateTable(T_Table::PORTFOLIO);
            loadDataAndUpdateTable(T_Table::TRADE);
            loadDataAndUpdateTable(T_Table::NET_POS);
            loadDataAndUpdateTable(T_Table::SUMMARY);
            loadDataAndUpdateTable(T_Table::MISSED_TRADE);
            refresh_entire_table.storeRelaxed(0);
        }
        else if(refresh_entire_table.loadRelaxed()==200) {
#ifdef BACKEND_LOG
            write_backend_Log("Refreshing Entire table...");
#endif
            qDebug()<<"Refreshing Entire table...";
            loadDataAndUpdateTable(T_Table::PORTFOLIO);
            loadDataAndUpdateTable(T_Table::TRADE);
            loadDataAndUpdateTable(T_Table::NET_POS);
            loadDataAndUpdateTable(T_Table::SUMMARY);
            loadDataAndUpdateTable(T_Table::MISSED_TRADE);
            refresh_entire_table.storeRelaxed(0);


        }
        else if(refresh_entire_table.loadRelaxed()==300) {
#ifdef BACKEND_LOG
            write_backend_Log("Refreshing MISSED_TRADE and TRADE table...");
#endif
            qDebug()<<"Refreshing MISSED_TRADE and TRADE table...";
            loadDataAndUpdateTable(T_Table::MISSED_TRADE);
            loadDataAndUpdateTable(T_Table::TRADE);
            refresh_entire_table.storeRelaxed(0);
        }

        // Calculate the remaining time to wait
        int elapsed = timer.elapsed();
        int remainingWaitTime = data_load_interval - elapsed;

        // Lock the mutex and wait for the remaining time or until woken up
        if (remainingWaitTime > 0) {
            DataLoadMutex.lock();  // Lock the mutex
            waitConditionDataLoadThread.wait(&DataLoadMutex, remainingWaitTime); // Wait with timeout
            DataLoadMutex.unlock();  // Unlock after waiting
        }



     if(quit_db_data_load_thread.loadRelaxed()==1)
           break;
   }
       // QThread::sleep(5);

    qDebug()<<"------------ Exiting data loading thread.....";
  //  data_loading_thread_running.storeRelaxed(0);
   // qDebug() << "refreshTables took :" << timer.elapsed() << "milliseconds";

    //
    //QTimer::singleShot(2000, this, &MainWindow::refreshTableIn_Intervel);
}

void MainWindow::triggerImmediate_refreshTables() {

    refresh_entire_table.storeRelaxed(1);
    // Lock the mutex, wake the condition, then unlock
    DataLoadMutex.lock();
    waitConditionDataLoadThread.wakeAll();
    DataLoadMutex.unlock();
}


// This data will received from exchange and used to update alog table rate column
void MainWindow::start_slowdata_worker()
{
    // SlowData& slowData = SlowData::getInstance();
    //  slowData.startSlowDataSocket();
    slowData = new SlowData();
    QObject::connect(slowData, SIGNAL(newSlowDataReceivedSignal(const QHash<QString, MBP_Data_Struct> &)), this, SLOT(slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct> &)));

    slowData->startSlowDataSocket();
}

void MainWindow::stop_slowdata_worker(){
    slowData->stopSlowDataSocket();
}

//This slot will be called when new slow date received
void MainWindow::slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct>& data){

    T_Portfolio_Model->updateMarketRate(data);
    net_pos_model->updateM2M(data);
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


        QStringList PortFoliosToDelete;
        PortFolioHashLessHash = db_conn->getPortfoliosTableData(reloadSortSettFlg,AlgoCount,combined_tracker_model,averagePriceList,QString::number(userData.UserId),TradedPortFolioList,PortFoliosToDelete);

        if(PortFoliosToDelete.size()){
         // QString msg = "Token not exist in contract table for  Portfolios ('" + PortFoliosToDelete.join("','") + "')!. Deleting it.";
            //If the contract has no data for the portflios delete it
            if(showMessagOnceFlg){
              emit requestDeleteConfirmation(PortFoliosToDelete);
             showMessagOnceFlg = false;
          }


          //  emit showMessageSignal(msg);


//          bool ret = db_conn->deleteAlgos(PortFoliosToDelete,msg);
//          if(!ret&&showMessagOnceFlg){
//            emit showMessageSignal(msg);
//          }
          //showMessagOnceFlg = false;
        }

        emit data_loded_signal(T_Table::PORTFOLIO);
        updateSummaryLabels();
        break;
       }

    case T_Table::SUMMARY:
        db_conn->getSummaryTableData(OrderCount,QString::number(userData.UserId) );  // Correctly get the count
        emit data_loded_signal(T_Table::SUMMARY);
        updateSummaryLabels();
        break;


    case T_Table::NET_POS:{
        //QHash<QString,int> PortFoliosLotSizeHash = T_Portfolio_Model->getPortFoliosLotSize();
        db_conn->getNetPosTableData(BuyValue_summary,SellValue,Profit,BuyQty_summary,SellQty_summary,NetQty,net_pos_model,OpenPositionmodel,QString::number(userData.UserId));
        emit data_loded_signal(T_Table::NET_POS);
        updateSummaryLabels();
        break;
    }

    case T_Table::MISSED_TRADE:{
        db_conn->getMissedTradeData(missed_trade_model,QString::number(userData.UserId));
        emit data_loded_signal(T_Table::MISSED_TRADE);
        break;
    }

    case T_Table::TRADE:{
//        OutputDebugStringA("Test1 \n");
        //QHash<QString, PortFolioData_Less> PortFolioHash = T_Portfolio_Model->getPortFolioDataLess();
       QStringList ExecutedTableHilightExcludeList = trade_model->getExecutedTableHighlight_ExcludeList();

       // algosToDisableOnExchangePriceLimit.clear();
       QStringList newlyTradeAlgos;
       QStringList algosToDisableOnMaxLossLimit; //will contin list of "algoNo:TraderData" to disable
        db_conn->getTradeTableData(TraderCount,trade_model,f1f2_order_table_model,liners_model,QString::number(userData.UserId),PortFolioHashLessHash,algosToDisableOnMaxLossLimit,ExecutedTableHilightExcludeList,traderData_ID_OnAppStart,newlyTradeAlgos);
        emit data_loded_signal(T_Table::TRADE);
        updateSummaryLabels();
        T_Portfolio_Model->updateNewleyTradeHilgiht(newlyTradeAlgos);


//#ifdef DISABLE_ALGO_ON_EXCHANGE_PRICE_LIMIT
        //check algosToDisableOnExchangePriceLimit included in algosExludeListFromExchangePriceDisable, if so remove
       // algosToDisableOnExchangePriceLimit will contin list of "algoNo:TraderData"
        /*for (auto it = algosToDisableOnExchangePriceLimit.begin(); it != algosToDisableOnExchangePriceLimit.end(); ) {
            if (algosToDisableOnExchangePriceLimitExludedList.contains(*it)) {
                it = algosToDisableOnExchangePriceLimit.erase(it);  // Remove element and update iterator
            }
            else {
                ++it;  // Move to the next element
            }
        }*/

        QStringList portfoliosToDisable;
        // Remove items from algosToDisable that are not in enabledPortfolios( this check is for, no need to disable already disabled one)
        QStringList enabledPortfolios = T_Portfolio_Model->getActivatedPortfolios();
       // QStringList traderDataList;
        for (const QString &algo : algosToDisableOnMaxLossLimit) {
            //QStringList list =  arry.split(":");
           //QString algo = list.at(0);
           // QString traderData = list.at(1);
           // if(!tradedDataListDisabledOnMaxLoss_Thr_Reach.contains(traderData)){ // check it's already on disabled  previously
           // traderDataList.append(traderData);
            if (enabledPortfolios.contains(algo)) {
                if(!portfoliosToDisable.contains(algo))
                  portfoliosToDisable.append(algo);
            }
            //}
        }

        if(portfoliosToDisable.length()>0){
          // Create a comma-separated string from the QStringList
          QString portfolioNumberStr = portfoliosToDisable.join(", ");

          // Construct the SQL query
          QString Query = QString("UPDATE Portfolios SET Status='DisabledByUser' WHERE PortfolioNumber IN (%1)").arg(portfolioNumberStr);
          QString msg;
          bool success = db_conn->updateDB_Table(Query, msg);
          if(success){
              // Send deactivation command to backend
              quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
              const unsigned char dataBytes[] = { 0x00 };
              QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
              QByteArray msg = backend_comm->createPacket(command, data);
              backend_comm->insertData(msg);
              db_conn->logToDB(QString("Disabled portfolios [" + portfolioNumberStr + "] on maxLossThr"));
              qDebug()<<QString("Disabled portfolios [" + portfolioNumberStr + "] on maxLossThr");
              //QStringList combinedList = tradedDataListDisabledOnMaxLoss_Thr_Reach + traderDataList; // Concatenate using + operator
              //CacheFileIO->over_writeTradedDataList_DisabledOn_MaxLoss_Thr_Reach(combinedList);
              //tradedDataListDisabledOnMaxLoss_Thr_Reach.clear();
              //tradedDataListDisabledOnMaxLoss_Thr_Reach = combinedList;
              //triggerImmediate_refreshTables();
              QTimer::singleShot(2000, this, &MainWindow::triggerImmediate_refreshTables);

          }
          else{
              QMetaObject::invokeMethod(this, "showError", Qt::QueuedConnection,
                                        Q_ARG(QString, "Disable failed for portfolios (%1), Exchange Price greater than 20% check Error: " + msg));
              qDebug()<<QString("Disable failed for portfolios (%1), Exchange Price  greater than 20% check. Error: "+msg).arg(portfolioNumberStr);
          }


        }

        /***************************************************************************************/
//        #endif

        break;
    }
   /* case T_Table::Liners:{
         QHash<QString, PortFolioData_Less> PortFolioTypeHash = T_Portfolio_Model->getPortFolioDataLess();
        db_conn->getLinersTableData(liners_model,QString::number(userData.UserId), PortFolioTypeHash);
         db_conn->getTradeTableData(trade_model,QString::number(userData.UserId),PortFolioTypeHash);
         emit data_loded_signal(T_Table::Liners);
         break;
    }*/




    default:
        break;
    }
}

void MainWindow::onRequestDeleteConfirmation(const QStringList &PortFoliosToDelete) {

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Confirmation",
                                  "Token not exist in contract table for Portfolios ('" + PortFoliosToDelete.join("','") + "')!. Delete those portfolios?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString msg; // You can define msg as per your requirement
        bool ret = db_conn->deleteAlgos(PortFoliosToDelete, msg);

        if (!ret) {
            QMessageBox::warning(this, "Error", "Failed to delete the portfolios: " );
        }
        else{

            //send notifcation to backend server
            quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
            const unsigned char dataBytes[] = { 0xFF, 0xFF };
            QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 2);
            QByteArray msg = backend_comm->createPacket(command, data);
            backend_comm->insertData(msg);
        }
    } else {
        this->on_close_clicked();
    }
}

QString MainWindow::fixDecimal(double num,int decimal_precision){
    QString str = QString::number(num, 'f', decimal_precision+1);
    return str;
}
void MainWindow::updateSummaryLabels()
{
    summarydatList.clear();
    summarydatList.append(QString::number(AlgoCount));
    summarydatList.append(QString::number(OrderCount));
    summarydatList.append(QString::number(TraderCount));
    summarydatList.append(QString::number(BuyValue_summary, 'f', 2));
    summarydatList.append(QString::number(SellValue, 'f', 2));
    summarydatList.append(QString::number(Profit, 'f', 2));
    summarydatList.append(QString::number(BuyQty_summary));
    summarydatList.append(QString::number(SellQty_summary));
    summarydatList.append(QString::number(NetQty));

    if (summary) {
        if (summary->isVisible()) {
            //summary->updateSummaryData(summarydatList);
            emit data_summary_update_signal(summarydatList);
        }
    }






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
        ui->label_3->setText("   Server Status :"  "Stopped");
        ui->label_3->setStyleSheet("color: red; font-weight: bold;");
        backendConn_Status = false;
    }
    //ui->toolButton_BackendServer->setStyleSheet("background-color: rgb(255, 32, 36);border-radius:6px;color:#000;");
    else if(msgType == SocketDataType::BACKEND_COMM_SOCKET_CONNECTED){
#ifdef ENABLE_BACKEND_DEBUG_MSG
        qDebug()<<"Backend Data: Backend Socket Connected"<<msg;
#endif
        ui->label_3->setText("  Server Status :" "Started");
        ui->label_3->setStyleSheet("color: #013220;font-weight: bold;");
        backendConn_Status = true;
    }
#ifdef BACKEND_LOG

    else if( msgType == BACKEND_COMM_SEND_SUCCESS){
        write_backend_Log(msg);
    }
    else if( msgType == BACKEND_COMM_SEND_FAILED){
        write_backend_Log(msg);
    }
#endif



    else if(msgType == SocketDataType::BACKEND_COMM_SOCKET_DATA){
        if(msg == "CMD_ID_TRADE_UPDATED_200"){
#ifdef BACKEND_LOG
            write_backend_Log("Backend CMD 200 Received...");
#endif
            //refresh all table from here.

            refresh_entire_table.storeRelaxed(200);

            // Lock the mutex, wake the condition, then unlock
            DataLoadMutex.lock();
            waitConditionDataLoadThread.wakeAll();
            DataLoadMutex.unlock();

            //trigger after 1 second as DB might not get updated fully when backend command receved.
            QTimer::singleShot(1000, this, [this]() {
                refresh_entire_table.storeRelaxed(200);
                DataLoadMutex.lock();
                waitConditionDataLoadThread.wakeAll();
                DataLoadMutex.unlock();
            });


           //log here
        }
        else if(msg == "CMD_ID_TRADE_UPDATED_CMD_300"){
#ifdef BACKEND_LOG
            write_backend_Log("Backend CMD 300 Received...");
#endif
           //refresh trade table and missed trade table from here.
            refresh_entire_table.storeRelaxed(300);

            // Lock the mutex, wake the condition, then unlock
            DataLoadMutex.lock();
            waitConditionDataLoadThread.wakeAll();
            DataLoadMutex.unlock();

            //trigger after 1 second as DB might not get updated fully when backend command receved.
            QTimer::singleShot(1000, this, [this]() {
                refresh_entire_table.storeRelaxed(300);
                DataLoadMutex.lock();
                waitConditionDataLoadThread.wakeAll();
                DataLoadMutex.unlock();
            });

            //log here
        }


    }


    // ui->toolButton_BackendServer->setStyleSheet("background-color: rgb(94, 255, 107);border-radius:6px;color:#000;");
}
void MainWindow::stop_backend_comm_socket_worker(){
//    backend_comm->quitDataFetchThread();

//    // Quit the thread's event loop and wait for it to finish
//    backend_comm_thread->quit();
//    backend_comm_thread->wait();
//    backend_comm_thread->deleteLater();

    if (backend_comm) {
            // Close the backend communicationd thread properly
            backend_comm->quitDataFetchThread();  // Ensure socket is closed

            // Quit the thread's event loop and wait for it to finish
            backend_comm_thread->quit();

            // Add a small timeout to ensure the thread does not block too long
            if (!backend_comm_thread->wait(500)) {  // Wait for 1 second
                backend_comm_thread->terminate();  // Force termination if necessary
            }

            backend_comm_thread->deleteLater();
        }
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


void MainWindow::updatePortFolioStatus(QModelIndex index) {
    if (portfolio_table_updating_db.loadRelaxed() == 1) {
            qDebug() << "Portfolio table updating is in progress.";
            return;
    }
    portfolio_table_updating_db.storeRelaxed(1);

    // Get the portfolio object at the given row index
    PortfolioObject *P = T_Portfolio_Model->getPortFolioAt(index.row());
    if (!P) {
            qDebug() << "updatePortFolioStatus: portfolio is null, need to debug.";
            portfolio_table_updating_db.storeRelaxed(0);
            return;
    }

    // Get the portfolio number and status
    QString PortfolioNumber = QString::number(P->PortfolioNumber);
    portfolio_status status = static_cast<portfolio_status>(P->StatusVal.toInt());

    // Set the editing flag for this portfolio row
    T_Portfolio_Model->setEditingFlg(index.row(), 1);

    if (status == portfolio_status::Filled || status == portfolio_status::DisabledByUser) {
            // Activate the portfolio if the active portfolio count is below the limit
            QStringList activatedPortFolios = T_Portfolio_Model->getActivatedPortfolios();
            if (activatedPortFolios.size() >= userData.MaxActiveCount) {
                // Show warning message


                qDebug() << "updatePortFolioStatus: Maximum active portfolio limit reached, ActivatedPortFolios ="
                         << activatedPortFolios.size() << ", Limit=" << userData.MaxActiveCount;

                T_Portfolio_Model->updatePortFolioStatusValue(index.row(), QString::number(portfolio_status::DisabledByUser));

                // Ensure the UI remains consistent by refreshing the table to restore the button's visibility
                T_Portfolio_Table->viewport()->update();

                // Prevent further database updates
                portfolio_table_updating_db.storeRelaxed(0);

                QMessageBox msgBox;
                msgBox.setText("Maximum Active Portfolio Limit Reached. Cannot activate more portfolios.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
                return;
            }

            // Proceed with activating the portfolio
            T_Portfolio_Model->getActivatedPortfolios(); // Example: Call your activation function here



            // Activate portfolio in UI and update the database
            QString Query = "UPDATE Portfolios SET Status='Active' WHERE PortfolioNumber=" + PortfolioNumber;
            QString msg;
            if (db_conn->updateDB_Table(Query, msg)) {
                T_Portfolio_Model->updatePortFolioStatusValue(index.row(), QString::number(portfolio_status::Active));

                // Log success and trigger backend communication
                triggerImmediate_refreshTables();
                reloadSortSettFlg.storeRelaxed(1);
                db_conn->logToDB(QString("Activated portfolio [" + PortfolioNumber + "]"));

                // Send activation command to backend
                quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
                const unsigned char dataBytes[] = { 0x01 };
                QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
                QByteArray msg = backend_comm->createPacket(command, data);
                backend_comm->insertData(msg);

            }
            else{
                T_Portfolio_Model->updatePortFolioStatusValue(index.row(), QString::number(portfolio_status::DisabledByUser));
                QMessageBox::warning(this, "Error", "Failed to disable portfolio, Error: "+msg);

            }

    } else {
            // Deactivate portfolio in UI and update the database
            QString Query = "UPDATE Portfolios SET Status='DisabledByUser' WHERE PortfolioNumber=" + PortfolioNumber;
            QString msg;
            if (db_conn->updateDB_Table(Query, msg)) {
                T_Portfolio_Model->updatePortFolioStatusValue(index.row(), QString::number(portfolio_status::DisabledByUser));
                // Log success and trigger backend communication
                triggerImmediate_refreshTables();
                reloadSortSettFlg.storeRelaxed(1);
                db_conn->logToDB(QString("Disabled portfolio [" + PortfolioNumber + "]"));

                // Send deactivation command to backend
                quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
                const unsigned char dataBytes[] = { 0x00 };
                QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
                QByteArray msg = backend_comm->createPacket(command, data);
                backend_comm->insertData(msg);
            }
            else{
                T_Portfolio_Model->updatePortFolioStatusValue(index.row(), QString::number(portfolio_status::Active));
                QMessageBox::warning(this, "Error", "Failed to enable portfolio, Error: "+msg);

            }
    }

    // Reset the editing flag and clean up
    T_Portfolio_Model->setEditingFlg(index.row(), 0);
    delete P;
    portfolio_table_updating_db.storeRelaxed(0);
}


//void MainWindow::resizePortFolioTableColWidthSlot(int width)
//{
//    T_Portfolio_Table->setColumnWidth(PortfolioData_Idx::_AlgoName,width);
//}

//void MainWindow::tradeTableSerachNext(){
//    currentIDxSearch++;
//    searchTradeTable();
//}

void MainWindow::loggedIn(){

    loggedInFlg.storeRelaxed(1);

    //make sure the start_slowdata_worker in main thread
    QMetaObject::invokeMethod(this, [this]() {
       // ui->label_17->setText(userData.UserName+ "  " +"Summary");
        QString formattedText =  userData.UserName /*+ " <span style=\"font-weight:normal;\">Summary</span>"*/;
        ui->label_6->setText("User Name :" + formattedText);
        QFont font = ui->label_6->font();
        font.setBold(true);
        ui->label_6->setFont(font);
      ui->label_6->setStyleSheet("color: black; font-size: 10pt;");

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

    traderData_ID_OnAppStart  = db_conn->getLastestTradetData_ID_FromTradeTable(QString::number(userData.UserId));

    //loading data
    loadCurrentDayLogs();
    loadContract();


}
/*
void MainWindow::stopBG_Threads(){
    stop_dataLoadingThread();
    stop_slowdata_worker();
    stop_slowdata_indices_worker();
    stop_backend_comm_socket_worker();
}
*/
void MainWindow::stopBG_Threads(){

   // Trade_TableRefreshTimer->stop();

    QElapsedTimer timer;

    // Measure stop_dataLoadingThread
    timer.start();
    stop_dataLoadingThread();
    qint64 time1 = timer.elapsed();
    qDebug() << "Time taken for stop_dataLoadingThread: " << time1 << "ms";

    // Measure stop_slowdata_worker
    timer.restart();
    stop_slowdata_worker();
    qint64 time2 = timer.elapsed();
    qDebug() << "Time taken for stop_slowdata_worker: " << time2 << "ms";

    // Measure stop_slowdata_indices_worker
    timer.restart();
    stop_slowdata_indices_worker();
    qint64 time3 = timer.elapsed();
    qDebug() << "Time taken for stop_slowdata_indices_worker: " << time3 << "ms";

    // Measure stop_backend_comm_socket_worker
    timer.restart();
    stop_backend_comm_socket_worker();
    qint64 time4 = timer.elapsed();
    qDebug() << "Time taken for stop_backend_comm_socket_worker: " << time4 << "ms";

}

void MainWindow::loggedOut(){
    loggedInFlg.storeRelaxed(0);
    stopBG_Threads();
    emit logoutRequested();

}

void MainWindow::add_logs(QString str){
   // ui->textEdit->append(str);
    logsdata.append(str);
    emit logDataSignal(logsdata);
    ui->label_5->setWordWrap(true);
    ui->label_5->setText(str);
    ui->label_5->setStyleSheet("font-family: 'Work Sans'; font-size: 8pt; /*color: white;*/");
     ui->label_5->setStyleSheet("color: black;");

}

void MainWindow::on_OrderBook_Button_clicked()
{
    //8
    //dock_win_trade->show();
    dock_win_trade->toggleView(true);

   // ui->OrderBook_Widget->setStyleSheet(stylesheetvis);
   // ui->OrderBook_Close->setVisible(true);
}

void MainWindow::on_F1F2Trade_Button_clicked()
{
    //8
    //dock_win_trade->show();
    dock_win_f1f2_order->toggleView(true);

   // ui->F1F2Trade_widget->setStyleSheet(stylesheetvis);
   // ui->OrderBook_Close->setVisible(true);
}

//void MainWindow::on_OrderBook_Close_clicked()
//{
//    //8
//    dock_win_trade->toggleView(false);
//    ui->OrderBook_Widget->setStyleSheet("");
//    ui->OrderBook_Close->setVisible(false);
//}


void MainWindow::on_Positions_Button_clicked()
{
    //9
    dock_win_net_pos->toggleView(true);

   // ui->Positions_Widget->setStyleSheet(stylesheetvis);
   // ui->Positions_Close->setVisible(true);
}


//void MainWindow::on_Positions_Close_clicked()
//{
//    //9
//    dock_win_net_pos->toggleView(false);
//    ui->Positions_Widget->setStyleSheet("");
//    ui->Positions_Close->setVisible(false);
//}
void MainWindow::on_Liners_Button_clicked()
{
    //10
    dock_win_liners->toggleView(true);
   // ui->Liners_Widget->setStyleSheet(stylesheetvis);
   // ui->Liners_Close->setVisible(true);
}

void MainWindow::on_OpenPosition_Button_clicked()
{
    //10
    dock_win_Open_position->toggleView(true);
    // ui->Liners_Widget->setStyleSheet(stylesheetvis);
    // ui->Liners_Close->setVisible(true);
}




//void MainWindow::on_Liners_Close_clicked()
//{
//    //10
//    dock_win_liners->toggleView(false);
//    ui->Liners_Widget->setStyleSheet("");
//    ui->Liners_Close->setVisible(false);
//}


void MainWindow::on_HP_Button_clicked()
{
    //11
    dock_win_combined_tracker->toggleView(true);
    //ui->HP_Widget->setStyleSheet(stylesheetvis);
   // ui->HP_Close->setVisible(true);
}


//void MainWindow::on_HP_Close_clicked()
//{
//    //11
//    dock_win_combined_tracker->toggleView(false);
//    ui->HP_Widget->setStyleSheet("");
//    ui->HP_Close->setVisible(false);
//}


void MainWindow::on_MissedTrade_Button_clicked()
{
    dock_win_missed_trades->toggleView(true);
  // ui->MissedTrade_widget->setStyleSheet(stylesheetvis);
   // ui->MissedTrade_Close->setVisible(true);
}

//void MainWindow::on_MissedTrade_Close_clicked()
//{
//    dock_win_missed_trades->toggleView(false);
//    ui->MissedTrade_widget->setStyleSheet("");
//    ui->MissedTrade_Close->setVisible(false);
//}

void MainWindow::on_Templates_Button_clicked()
{
    //12
   // ui->Templates_Widget->setStyleSheet(stylesheetvis);
   // ui->Templates_Close->setVisible(true);
}


//void MainWindow::on_Templates_Close_clicked()
//{
//    //12
//    ui->Templates_Widget->setStyleSheet("");
//    ui->Templates_Close->setVisible(false);
//}



void MainWindow::on_minimize_clicked()
{
    showMinimized();
}

//void MainWindow::on_menu_clicked()
//{

//}






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


void MainWindow::startall_Button_clicked()
{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to activate all selected algos?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }

        QStringList portfolioNumbers = T_Portfolio_Model->getAllPortfolioNumbers();
        QString joinedPortfolioNumbers = portfolioNumbers.join(", ");

        // First, disable all selected portfolios by setting their status to 'DisabledByUser'
        QString disableQuery = "UPDATE Portfolios SET Status='DisabledByUser' WHERE PortfolioNumber IN (" + joinedPortfolioNumbers + ")";
        QString msg;
        bool disableSuccess = db_conn->updateDB_Table(disableQuery, msg);

        if (!disableSuccess) {
            // Handle error if disabling portfolios failed
            QMessageBox::warning(this, "Error", "Failed to disable portfolios.");
            return;
        }

        // Limit portfolios to be activated to MaxActiveCount
        if (portfolioNumbers.size() > userData.MaxActiveCount) {
            portfolioNumbers = portfolioNumbers.mid(0, userData.MaxActiveCount); // Get first MaxActiveCount portfolios
        }

        joinedPortfolioNumbers = portfolioNumbers.join(", ");

        // Then, activate the selected portfolios
        QString activateQuery = "UPDATE Portfolios SET Status='Active' WHERE PortfolioNumber IN (" + joinedPortfolioNumbers + ")";
        bool success = db_conn->updateDB_Table(activateQuery, msg);

        if (success) {
            db_conn->logToDB(QString("Enabled Algos [" + joinedPortfolioNumbers + "]"));

            // Send backend command
            quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
            const unsigned char dataBytes[] = { 0x01 };
            QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
            QByteArray msg = backend_comm->createPacket(command, data);
            backend_comm->insertData(msg);

            triggerImmediate_refreshTables();
        } else {
            QMessageBox::warning(this, "Error", "Failed to activate portfolios.");
        }
}


void MainWindow::stopall_Button_clicked()
{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Are you sure you want to Disable all selected algos?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }

        QStringList portfolioNumbers = T_Portfolio_Model->getAllPortfolioNumbers();

//        for(int i=0;i<T_Portfolio_Model->portfolio_data_list.size();i++){
//            if (T_Portfolio_Model->portfolio_data_list[i]) {
//            portfolioNumbers.append(QString::number(T_Portfolio_Model->portfolio_data_list[i]->PortfolioNumber));
//            }
//        }
        QString joinedPortfolioNumbers = portfolioNumbers.join(", ");
        QString Query = "UPDATE Portfolios SET Status='DisabledByUser' WHERE PortfolioNumber IN (" + joinedPortfolioNumbers + ")";

        QString msg;
        bool success =  db_conn->updateDB_Table(Query,msg);
        if(success){
            db_conn->logToDB(QString("Disabled  Algos ["+joinedPortfolioNumbers+"]"));
            quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
            const unsigned char dataBytes[] = { 0x01};
            QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
            QByteArray msg = backend_comm->createPacket(command, data);
            backend_comm->insertData(msg);

            triggerImmediate_refreshTables();

        }
}

void MainWindow::refresh_Button_clicked(){
  triggerImmediate_refreshTables();
  //updateSummaryData(summarydatList);
}


void MainWindow::duplicate_Button_clicked() {
  algo_data_to_insert data;
  int MaxPortfolioCount = userData.MaxPortfolioCount;
  QString msg;

  QModelIndexList selected = getSelectedPortFolioIndexs();

  if (selected.isEmpty()) {
            QMessageBox::information(this, "Duplicate Portfolio", "Select a portfolio to duplicate.");
            return;
  }

  if (selected.size() > 1) {
            QMessageBox::information(this, "Duplicate Portfolio", "Select only one portfolio to duplicate.");
            return;
  }

  for (const QModelIndex& index : selected) {
            PortfolioObject* selectedPortfolio = T_Portfolio_Model->getPortFolioAt(index.row());
            if (!selectedPortfolio) {
            qDebug() << "Error: Portfolio object not found at index" << index.row();
            continue;
            }

            data.algo_type = selectedPortfolio->PortfolioType;
            data.Algo_Name = selectedPortfolio->AlgoName;
            data.Algo_Status = "DisabledByUser";
            data.Leg1_token_number = QString::number(selectedPortfolio->Leg1TokenNo);
            data.Leg2_token_number = QString::number(selectedPortfolio->Leg2TokenNo);
            data.Leg3_token_number = QString::number(selectedPortfolio->Leg3TokenNo);
            data.Leg4_token_number = QString::number(selectedPortfolio->Leg4TokenNo);
            data.user_id = QString::number(userData.UserId);
            data.Alias = selectedPortfolio->Alias;

            algo_data_insert_status status = db_conn->insertToAlgoTable(data, MaxPortfolioCount, msg);
            if (status == algo_data_insert_status::LIMIT_REACHED) {
            QMessageBox::warning(this, "Duplicate Portfolio", msg);
            break;
            }
            if (status == algo_data_insert_status::INSERTED) {
            // Log the creation of the duplicate
            QString logMessage = QString("Duplicate added for Portfolio Number: %1").arg(selectedPortfolio->PortfolioNumber);
            db_conn->logToDB(logMessage);

            // Send backend command
            quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
            const unsigned char dataBytes[] = { 0x01 };
            QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
            QByteArray backendMsg = backend_comm->createPacket(command, data);
            backend_comm->insertData(backendMsg);

            triggerImmediate_refreshTables();
            onPortfolioAdded(); // send backend command
            }
  }

  // Clear selection and focus
  T_Portfolio_Table->clearSelection();
  T_Portfolio_Table->clearFocus();
}




void MainWindow::OnAlgorithmDockWidgetVisiblityChanged(bool p_Visible)
{
   /* if(p_Visible)
        ui->Algorithms_Widget->setStyleSheet(stylesheetvis);
    else
        ui->Algorithms_Widget->setStyleSheet("");*/
  //  ui->Algorithms_Close->setVisible(p_Visible);
  if (!p_Visible) {
            // Dock is being hidden (likely closed), clear the highlight text
            //T_Portfolio_Delegate->setHighlightText("");
            //ui->lineEditSearch->clear();
           // emit ui->lineEditSearch->textChanged(""); // Emit the signal with an empty string

  }

}

void MainWindow::OnOrderBookDockWidgetVisiblityChanged(bool p_Visible)
{
   /* if(p_Visible)
        ui->OrderBook_Widget->setStyleSheet(stylesheetvis);
    else
        ui->OrderBook_Widget->setStyleSheet("");*/
  //  ui->OrderBook_Close->setVisible(p_Visible);
//  if (!p_Visible) {
//            // Dock is being hidden (likely closed), clear the highlight text
//            T_Portfolio_Delegate->setHighlightText("");
//  }
}
void MainWindow::OnF1F2OrderDockWidgetVisiblityChanged(bool p_Visible)
{
    /*if(p_Visible)
        ui->F1F2Trade_widget->setStyleSheet(stylesheetvis);
    else
        ui->F1F2Trade_widget->setStyleSheet("");*/
  //  ui->OrderBook_Close->setVisible(p_Visible);
}



void MainWindow::OnPositionsDockWidgetVisiblityChanged(bool p_Visible)
{
  /*  if(p_Visible)
        ui->Positions_Widget->setStyleSheet(stylesheetvis);
    else
        ui->Positions_Widget->setStyleSheet("");*/
   // ui->Positions_Close->setVisible(p_Visible);
}
void MainWindow::OnLinersDockWidgetVisiblityChanged(bool p_Visible)
{
  /*  if(p_Visible)
        ui->Liners_Widget->setStyleSheet(stylesheetvis);
    else
        ui->Liners_Widget->setStyleSheet("");*/
   // ui->Liners_Close->setVisible(p_Visible);
}

void MainWindow::OnHPDockWidgetVisiblityChanged(bool p_Visible)
{
  /*  if(p_Visible)
        ui->HP_Widget->setStyleSheet(stylesheetvis);
    else
        ui->HP_Widget->setStyleSheet("");*/
   // ui->HP_Close->setVisible(p_Visible);
}
void MainWindow::OnOpenDockWidgetVisiblityChanged(bool p_Visible)
{
  /*  if(p_Visible)
        ui->HP_Widget->setStyleSheet(stylesheetvis);
    else
        ui->HP_Widget->setStyleSheet("");*/
  // ui->HP_Close->setVisible(p_Visible);
}

void MainWindow::OnMTDockWidgetVisiblityChanged(bool p_Visible)
{
    /*if(p_Visible)
        ui->MissedTrade_widget->setStyleSheet(stylesheetvis);
    else
        ui->MissedTrade_widget->setStyleSheet("");*/
    ui->MissedTrade_Close->setVisible(p_Visible);
}


void MainWindow::on_Algorithms_Button_clicked()
{
    T_Portfolio_DockWin->toggleView(true);
    //7
  //  ui->Algorithms_Widget->setStyleSheet(stylesheetvis);
   // ui->Algorithms_Close->setVisible(true);
}


//void MainWindow::on_Algorithms_Close_clicked()
//{
//    T_Portfolio_DockWin->toggleView(false);
//    ui->Algorithms_Widget->setStyleSheet("");
//    ui->Algorithms_Close->setVisible(false);
//}
void MainWindow::ConvertAlgo_button_clicked(){

    if(!convertalgo){

        convertalgo=new ConvertAlgo_Win(this,userData.ExFilterPF);
        connect(convertalgo, &ConvertAlgo_Win::portfolioAddedSignal, this, &MainWindow::onPortfolioAdded);

        qDebug()<<"Creating convertalgo....";
    }


    convertalgo->update_contract_tableData(QString::number(userData.UserId),userData.MaxPortfolioCount);
    if(!convertalgo->isVisible())
    convertalgo->show();
}


void MainWindow::onPortfolioAdded(){

    sendBackendCommandOnAddPortfolio();
}

void MainWindow::sendBackendCommandOnAddPortfolio(){
    quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
    const unsigned char dataBytes[] = { 0x01};
    QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 1);
    QByteArray msg = backend_comm->createPacket(command, data);
    backend_comm->insertData(msg);
    triggerImmediate_refreshTables();
}

QModelIndexList  MainWindow::getSelectedPortFolioIndexs(){
    QModelIndexList selection = T_Portfolio_Table->selectionModel()->selectedRows();

    // Get the proxy model
    const Portfolio_SearchFilterProxyModel *proxyModel = qobject_cast<const Portfolio_SearchFilterProxyModel*>(T_Portfolio_Table->model());

    // Iterate over the selected rows and map them to the source model
    QModelIndexList sourceSelection;
    for (const QModelIndex &index : selection) {
        QModelIndex sourceIndex = proxyModel->mapToSource(index);
        sourceSelection.append(sourceIndex);
    }
   return sourceSelection;
}
void MainWindow::Delete_clicked_slot()
{
   // Get the selected portfolios first
   QModelIndexList selected = getSelectedPortFolioIndexs();

   // If no rows are selected, show the "Please select a portfolio" message and return
   if (selected.count() == 0)
   {
       QMessageBox msgBox;
       msgBox.setText("Please select a Algo.");
       msgBox.setIcon(QMessageBox::Warning);
       msgBox.exec();
       return;
   }


   // Multiple rows can be selected
   //QString logs;
   QStringList activeAlgoList;
   QStringList portFoliosToDelete;
   QList<int> portFolioIdxToDelete;

  // QString notDeletedMSg;

   for (int i = 0; i < selected.count(); i++)
   {
       QModelIndex index = selected.at(i);
       PortfolioObject *P =  T_Portfolio_Model->getPortFolioAt(index.row());

       if (!P) {
           qDebug() << "updatePortFolioStatus----- portfolio null, need to debug this code.";
           continue;
       }

       QString PortfolioNumber = QString::number(P->PortfolioNumber);

       if (P->Status == true)
       {
           //logs = "PortfolioNumber '" + PortfolioNumber + "' is Active, cannot delete.";
           activeAlgoList.append(PortfolioNumber);
          // db_conn->logToDB(logs);
           continue;
       }
       portFoliosToDelete.append(PortfolioNumber);
       portFolioIdxToDelete.append(index.row());


   }


   //if (reply == QMessageBox::Yes) {
   if (!activeAlgoList.empty()) {
       db_conn->logToDB("Portfolios [" + activeAlgoList.join(",") + "] are Active, cannot delete from DB");

       QMessageBox msgBox;
       msgBox.setText("Active algo cannot be deleted.\nDisable the algo No. " + activeAlgoList.join(",") + " and then try again.");
       msgBox.setIcon(QMessageBox::Information);
       msgBox.exec();

       // Remove active portfolios from deletion list
       for (const QString &activeAlgo : activeAlgoList) {
           portFoliosToDelete.removeAll(activeAlgo);
       }

       // If no portfolios remain for deletion, exit
       if (portFoliosToDelete.isEmpty()) {
           return;
       }
   }

   deletingPortFolioFlg.storeRelaxed(1);
   QString msg;
   bool ret = db_conn->deleteNonTradedAlgos(portFoliosToDelete, msg);

   if (!ret) {
       QMessageBox::warning(this, "Error", "Executed Trades cannot be deleted: " + portFoliosToDelete.join(","));
   } else {
       QMessageBox::StandardButton reply;
       reply = QMessageBox::question(this, "Delete Portfolio From Database?",
                                     "Delete portfolios [" + portFoliosToDelete.join(",") + "] from DB?",
                                     QMessageBox::Yes | QMessageBox::No);
       if (reply == QMessageBox::Yes) {
           // Delete from model and refresh the entire table
           T_Portfolio_Model->removeRowsByIndices(portFolioIdxToDelete);
           triggerImmediate_refreshTables();

           // Send notification to backend server
           quint16 command = BACKEND_CMD_TYPE::CMD_ID_PORTTFOLIO_NEW_1;
           const unsigned char dataBytes[] = { 0xFF, 0xFF };
           QByteArray data = QByteArray::fromRawData(reinterpret_cast<const char*>(dataBytes), 2);
           QByteArray bckend_msg = backend_comm->createPacket(command, data);
           backend_comm->insertData(bckend_msg);

           db_conn->logToDB("Deleted portfolios [" + portFoliosToDelete.join(",") + "] from DB");

           QMessageBox msgBox(this);
           msgBox.setWindowTitle("Algo Delete");
           msgBox.setText(msg);
           msgBox.setIcon(QMessageBox::Information);
           msgBox.exec();
       }
   }


       deletingPortFolioFlg.storeRelaxed(0);
   }
//}



void MainWindow::T_Portfolio_Table_cellDoubleClicked(const QModelIndex &index){

    QModelIndex mappedIdx = T_Portfolio_ProxyModel->mapToSource(index);

    int col = mappedIdx.column();
    if(col==PortfolioData_Idx::_AlgoName){
        if(orderPopUpWin->isHidden())
            orderPopUpWin->show();
        // QString Expiry = T_Portfolio_Model->portfolio_data_list[index.row()]->Expiry;
        PortfolioObject *P =  T_Portfolio_Model->getPortFolioAt(mappedIdx.row());
        if (!P) {
            qDebug()<<"updatePortFolioStatus----- portfolio null, need to debug this code.";

        }

        QString PortfolioType =P->PortfolioType;
        QString PortfolioNumber = QString::number(P->PortfolioNumber);
        QStringList linersData = liners_model->getLinersDataForAlgoID(PortfolioNumber); // This contain profit and netqty for portfolio
        orderPopUpWin->getTradeDataFromDB(QString::number(userData.UserId), PortfolioNumber,PortfolioType);

        QList<QHash<QString,QString>>  tradeData= trade_model->getTradedPortFolioListForAlgoID(PortfolioNumber);

        orderPopUpWin->setData(P,linersData,tradeData);
       // orderPopUpWin->activateWindow();
        delete P;
    }

}


void MainWindow::trade_table_cellDoubleClicked(const QModelIndex &index){
    //int row = index.row();
    QModelIndex mappedIdx = tradetableproxyModel->mapToSource(index);

    int col = index.column();
    if(col==OrderBook_Idx::AlgoName_OB){
        if(tradePopUpWin->isHidden())
            tradePopUpWin->show();
       // QString Expiry = T_Portfolio_Model->portfolio_data_list[index.row()]->Expiry;
        QStringList data = trade_model->getTradedDataForIdx(mappedIdx.row());
        QString localOrderID = data[OrderBook_Idx::TraderData_OB];
        int LotSize = data[OrderBook_Idx::LotSize_OB].toInt();

        tradePopUpWin->getTradeDataFromDB(QString::number(userData.UserId), localOrderID,LotSize);
        tradePopUpWin->activateWindow();
    }

}
void MainWindow::highlightRowInF1F2Order(int row) {
    QModelIndex rowIndex = f1f2_order_table->model()->index(row, 0);
    f1f2_order_table->setCurrentIndex(rowIndex);
    f1f2_order_table->selectionModel()->select(rowIndex,
    QItemSelectionModel::Select | QItemSelectionModel::Rows);

    // Scroll to the highlighted row for visibility
    f1f2_order_table->scrollTo(rowIndex);
}


void MainWindow::Skipped_Trades_cellDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) return; // Ensure valid index

    int col = index.column();
    if (col == Missed_Trades_Idx::Stock_Name) {
        dock_win_f1f2_order->toggleView(true);

        int row = index.row();  // Get the clicked row
        int orderId = missed_trade_model->getSingleOrderId(row);  // Fetch OrderId

        if (orderId != -1) {  // Ensure valid OrderId
            highlightRowInF1F2Order(orderId);
        }
    }
}



void MainWindow::f1f2_order_table_cellDoubleClicked(const QModelIndex &index){


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
            T_Portfolio_Model->setEditingFlg(portfolio_table_slected_idx_for_editing.load(),0);//T_Portfolio_Model->portfolio_data_list[portfolio_table_slected_idx_for_editing.load()]->edting.storeRelaxed(0); // maked the row flg not editing, so the bg data will be updated.
            portfolio_table_slected_idx_for_editing.store(-1);
        }
    }
    //multiple row selected or no row selected, so make last editing row uneditable
    else if(portfolio_table_slected_idx_for_editing.load()!=-1){
        T_Portfolio_Model->setEditingFlg(portfolio_table_slected_idx_for_editing.load(),0);//T_Portfolio_Model->portfolio_data_list[portfolio_table_slected_idx_for_editing.load()]->edting.storeRelaxed(0); // maked the row flg not editing, so the bg data will be updated.
        portfolio_table_slected_idx_for_editing.store(-1);
    }

}

void MainWindow::slotAddLogForAddAlgoRecord(QString str)
{
//    To show the latest log at top
//    htmlLogsContent.prepend(str);
//    ui->textEdit->setText(htmlLogsContent);
//    emit logDataSignal(str);
    logsdata.append(str);
    emit logDataSignal(logsdata);
    ui->label_5->setWordWrap(true);

    if(logsdata.size()>1){
        ui->label_5->setText("logs:"+logsdata[logsdata.size()-2]+logsdata[logsdata.size()-1]);
    }
    else
        ui->label_5->setText("logs:"+str);

    ui->label_5->setStyleSheet("color: black;font-weight: bold;");

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


void MainWindow::saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = appDataPath+"/Data/indices_data.bin";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing.");
        return;
    }
    QDataStream out(&file);
    out << indicesDataList;
    file.close();
}

//void MainWindow::loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList) {
//    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    QString fileName = appDataPath+"/Data/indices_data.bin";
//    QFile file(fileName);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qWarning("Could not open file for reading--> indices_data.bin");
//        return;
//    }
//    QDataStream in(&file);
//    in >> indicesDataList;
//    file.close();
//}


void MainWindow::indicesDataRecv_Slot(Indices_Data_Struct data){
    emit indicesDataRecv_Signal_watch_card(data);
    updateWatchDataCard(data);

    //remove below code
   // indicesDataMutex.lock();
    if(!indicesDataList.contains(data.indexName)){
        indicesDataList.insert(data.indexName,data);
        newIndicesData = true;
    }
    //check any of the value changed
    else if((indicesDataList[data.indexName].indexValue!=data.indexValue) || (indicesDataList[data.indexName].change!=data.change) || (indicesDataList[data.indexName].percentagechange!=data.percentagechange)){
        indicesDataList[data.indexName]=data;
        newIndicesData = true;
    }

  //  indicesDataMutex.unlock();
   // qDebug()<<"indicesDataRecv_Slot: " <<data.indexName;




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



}


void MainWindow::onPortFolioTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex){
     saveTableViewColumnState(T_Portfolio_Table);
}


void MainWindow::onTradeTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(trade_table);
}

void MainWindow::onF1F2OrderTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(f1f2_order_table);
}


void MainWindow::onNetposTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(net_pos_table);
}

void MainWindow::onLinersTableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(liners_table);
}

void MainWindow::onCombined_tracker_tableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(combined_tracker_table);
}
void MainWindow::onOpen_Position_tableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(open_position);
}
void MainWindow::onMissed_trade_tableHeader_Rearranged(int logicalIndex, int oldVisualIndex, int newVisualIndex) {
     saveTableViewColumnState(missed_trade_table);
}


void MainWindow::saveTableViewColumnState(QTableView *tableView){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/table_view_sett.dat", QSettings::IniFormat);
    QString key =tableView->objectName()+ "_HeaderState";

    // Save the state of the table view headers
   settings.setValue(key,tableView->horizontalHeader()->saveState());
   qDebug()<<"Saving table state  for "<<tableView->objectName();
}

void MainWindow::restoreTableViewColumnState(QTableView *tableView){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/table_view_sett.dat", QSettings::IniFormat);

       // Restore the state of the table view headers if available
     QString key =tableView->objectName()+ "_HeaderState";
     if (settings.contains(key))
     {
       tableView->horizontalHeader()->restoreState(settings.value(key).toByteArray());
       qDebug()<<"Restoring table state  for "<<tableView->objectName();
     }
}


void MainWindow::sorting_Button_clicked(){
      if (!sortWin) {
           sortWin = new SortSettingPopUp();
           connect(sortWin, &SortSettingPopUp::reloadSortSettingSignal, [this]() {
               reloadSortSettFlg.storeRelaxed(1);
               triggerImmediate_refreshTables();
           });
       }
      else{
          delete sortWin;
          sortWin = new SortSettingPopUp();
          connect(sortWin, &SortSettingPopUp::reloadSortSettingSignal, [this]() {
              reloadSortSettFlg.storeRelaxed(1);
              triggerImmediate_refreshTables();
          });
      }
       sortWin->show();
       sortWin->raise();  // Bring the window to the front
       sortWin->activateWindow(); // Give focus to the window

}
//For Buy
void MainWindow::F1_clicked_slot(){
   F1_F2_BuySell *F1F2 = new F1_F2_BuySell(this,devicer,decimal_precision);
   connect(F1F2, &F1_F2_BuySell::portfolioAddedSignal, this, &MainWindow::onPortfolioAdded);
   QObject::connect(slowData, SIGNAL(newSlowDataReceivedSignal(const QHash<QString, MBP_Data_Struct> &)), F1F2, SLOT(slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct> &)));
   F1F2->setAttribute(Qt::WA_DeleteOnClose);
   F1F2->userData = this->userData;
   F1F2->show();
   F1F2->setBuyMode(true);
}
//For Sell
void MainWindow::F2_clicked_slot(){
    F1_F2_BuySell *F1F2 = new F1_F2_BuySell(this,devicer,decimal_precision);
    connect(F1F2, &F1_F2_BuySell::portfolioAddedSignal, this, &MainWindow::onPortfolioAdded);
    QObject::connect(slowData, SIGNAL(newSlowDataReceivedSignal(const QHash<QString, MBP_Data_Struct> &)), F1F2, SLOT(slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct> &)));

    F1F2->setAttribute(Qt::WA_DeleteOnClose);
    F1F2->userData = this->userData;
    F1F2->show();
    F1F2->setBuyMode(false);

}

void MainWindow::on_close_clicked()
{
    db_conn->logToDB("Logged Out");

    //check summary windiow opened
    //if opende save some value in settings file

    //whne star the app check for that value and open it -- Do this step there

    //loggedOut();
    //portfolio->StatusVal.toInt()==portfolio_status::DisabledByUser;

    close();
}



void MainWindow::closeEvent(QCloseEvent* event)
{
    // Delete dock manager here to delete all floating widgets. This ensures
    // that all top level windows of the dock manager are properly closed
    saveTableViewColumnState(T_Portfolio_Table);
    saveTableViewColumnState(trade_table);
    saveTableViewColumnState(net_pos_table);
    saveTableViewColumnState(liners_table);
    saveTableViewColumnState(combined_tracker_table);
    saveTableViewColumnState(missed_trade_table);


    if(newIndicesData==true){
        saveIndicesDataListToFile(indicesDataList);
    }
    stopBG_Threads();

    saveDockManagerState();

    DockManagerMainPanel->deleteLater();
    QMainWindow::closeEvent(event);

    QCoreApplication::quit();

}

void MainWindow::showMessageSlot(QString msg){
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::onChangepasswordActionTriggered() {
    QDialog resetDialog(this);
    resetDialog.setWindowTitle("Reset Password");

    QVBoxLayout *layout = new QVBoxLayout(&resetDialog);

    // Create the new password input fields
    QLabel *newPasswordLabel = new QLabel("New password:", &resetDialog);
    QLineEdit *newPasswordLineEdit = new QLineEdit(&resetDialog);
    newPasswordLineEdit->setEchoMode(QLineEdit::Password); // Hide text for password input

    // Create the confirm password input fields
    QLabel *confirmPasswordLabel = new QLabel("Confirm password:", &resetDialog);
    QLineEdit *confirmPasswordLineEdit = new QLineEdit(&resetDialog);
    confirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    // Create reset button
    QPushButton *resetButton = new QPushButton("Reset Password", &resetDialog);

    // Add widgets to the layout
    layout->addWidget(newPasswordLabel);
    layout->addWidget(newPasswordLineEdit);
    layout->addWidget(confirmPasswordLabel);
    layout->addWidget(confirmPasswordLineEdit);
    layout->addWidget(resetButton);

    // Connect the reset button to handle password reset
    connect(resetButton, &QPushButton::clicked, [this, &resetDialog, newPasswordLineEdit, confirmPasswordLineEdit]() {
        QString newPassword = newPasswordLineEdit->text();
        QString confirmPassword = confirmPasswordLineEdit->text();
      //  QString user_id = "YourUserID"; // Replace with the actual user ID retrieval logic

        // Call the method to handle the password reset with three arguments
        resetPassword(newPassword, confirmPassword);

        // Close the dialog
        resetDialog.accept();
    });

    // Show the dialog as a modal window
    resetDialog.exec();
}

void MainWindow::resetPassword(const QString &newPassword, const QString &confirmPassword)
{
    // Check if the new password and confirm password match
    if (newPassword != confirmPassword) {
          QMessageBox::warning(this, "Error", "The new password and confirmation password do not match.");
          return;
    }

    QString msg;
    bool success = db_conn->resetPassword(newPassword, QString::number(userData.UserId),msg);

    // Implement further logic to reset the password
    QMessageBox::information(this, "Success", "Password has been reset successfully.");
}


//void MainWindow::onSummaryActionTriggered(){
//    Summary_cards *SC = new Summary_cards(this);
//    Qt::WindowFlags flags = SC->windowFlags();
//    SC->setWindowFlags(flags | Qt::Dialog );
//    SC->setAttribute(Qt::WA_DeleteOnClose);
//    //connect(this, &MainWindow::data_summary_update_signal, SC, &Summary_cards::updateSummaryData);
//    SC->updateSummaryData(summarydatList);
//    SC->show();
//}
void MainWindow::onSummaryActionTriggered() {

    if (!summary) {
          summary = new Summary_cards(this);
          connect(this, SIGNAL(data_summary_update_signal(const QStringList&)), summary, SLOT(updateSummaryData(const QStringList&)));

          Qt::WindowFlags flags = summary->windowFlags();
         // summary->setWindowFlags(flags | Qt::Dialog);
          summary->setWindowFlags(Qt::Window | Qt::Dialog);
          summary->setObjectName("summarycard");
          summary->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

         // summary->setAttribute(Qt::WA_DeleteOnClose);
    }

    if (!summary->isVisible()) {
          summary->show();
          //summary->updateSummaryData(summarydatList);
        //  emit data_summary_update_signal(summarydatList);
    }


}

//void MainWindow::onWatchActionTriggered(){
//    Watch_cards *WC = new Watch_cards(this);
//    Qt::WindowFlags flags = WC->windowFlags();
//    WC->setWindowFlags(flags | Qt::Dialog);
//    WC->setAttribute(Qt::WA_DeleteOnClose);
//    //connect(this, &MainWindow::indicesDataRecv_Signal_watch_card(Indices_Data_Struct), WC, &Watch_cards::indicesDataRecv_Slot(Indices_Data_Struct));
//    connect(this, &MainWindow::indicesDataRecv_Signal_watch_card, WC, &Watch_cards::indicesDataRecv_Slot);
//    connect(WC, &Watch_cards::add_remove_watch_card_signal,this , &MainWindow::add_remove_watch_card_slot);
//    WC->show();
//}
void MainWindow::onWatchActionTriggered() {

    if (!watch) {
          watch = new Watch_cards(this);
          Qt::WindowFlags flags = watch->windowFlags();
          //watch->setWindowFlags(flags | Qt::Dialog);
          watch->setWindowFlags(Qt::Window | Qt::Dialog);


          // Uncomment if you want the Watch_cards to be deleted when it's closed
          // watch->setAttribute(Qt::WA_DeleteOnClose);
          // Make sure the connections are made after the object is created
          connect(this, &MainWindow::indicesDataRecv_Signal_watch_card, watch, &Watch_cards::indicesDataRecv_Slot);
          connect(watch, &Watch_cards::add_remove_watch_card_signal, this, &MainWindow::add_remove_watch_card_slot);
          watch->setObjectName("watchcard");
          watch->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    }


    if (!watch->isVisible()) {
          watch->show();
    }
}


//void MainWindow::onLogActionTriggered(){
//    Logs_cards *LC = new Logs_cards(this);
//    Qt::WindowFlags flags = LC->windowFlags();
//    LC->setWindowFlags(flags | Qt::Dialog);
//    LC->setAttribute(Qt::WA_DeleteOnClose);
//    LC->logDataSignal_Slot(logsdata);
//    connect(this, &MainWindow::logDataSignal, LC, &Logs_cards::logDataSignal_Slot);
//    LC->show();
//}
void MainWindow::onLogActionTriggered() {
    // Check if Logs_cards instance already exists
    if (!logs) {
          logs = new Logs_cards(this);
          Qt::WindowFlags flags = logs->windowFlags();
          //logs->setWindowFlags(flags | Qt::Dialog);
          logs->setWindowFlags(Qt::Window | Qt::Dialog);
          logs->setObjectName("logcard");
          logs->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

          //logsCards->setAttribute(Qt::WA_DeleteOnClose);

    }
    if (!logs->isVisible()) {
          logs->show();
            logs->logDataSignal_Slot(logsdata);
          connect(this, &MainWindow::logDataSignal, logs, &Logs_cards::logDataSignal_Slot);
    }
}


/*********Setting Window Section***************/
void MainWindow::openSettingsWindow(){

    sett_win->loadHotkeyFromIniFileAndPopulateTreeView();
    sett_win->show();
}

/*********Setting Window Section***************/


void MainWindow::initializeGlobalHotKeys(){

    for (const QString &action : sett_win->HotKeysHash.keys()) {
       const HotKeyDetails &hotKey = sett_win->HotKeysHash.value(action);
       // If QShortcut already exists, update it
       if (HotKeyShortcutObjects.contains(action)) {
           QAction *shortcut = HotKeyShortcutObjects[action];
           shortcut->setShortcut(QKeySequence(hotKey.shortcut)); // Update shortcut key
           shortcut->setEnabled(hotKey.enabled); // Enable or disable based on the flag
       }
       else {
           // Store the QShortcut object in the hash for future updates
           QAction *shortcutAction = new QAction(this);
           shortcutAction->setShortcut(QKeySequence(hotKey.shortcut));
           shortcutAction->setEnabled(hotKey.enabled); // Set initial state
           if (action == "DELETE_SELECTED_ALGO"){
               shortcutAction->setShortcutContext(Qt::WidgetShortcut);
               updateHotKeysAction(action,shortcutAction);
               T_Portfolio_Table->addAction(shortcutAction);

           }
           else{
               shortcutAction->setShortcutContext(Qt::ApplicationShortcut);
               updateHotKeysAction(action,shortcutAction);
               this->addAction(shortcutAction);
           }


           HotKeyShortcutObjects.insert(action, shortcutAction);
       }


    }


}

void MainWindow::updateHotKeysAction(QString action,QAction* shortcutAction){
    /*************General Hotkey ****************************/

    // Connect the shortcut's activated signal to the corresponding action
    if (action == "OPEN_ALGO") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_Algorithms_Button_clicked);
    }
    else if (action == "OPEN_TRADES") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_OrderBook_Button_clicked);
    }
    else if (action == "OPEN_POSITION") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_Positions_Button_clicked);
    }
    else if (action == "OPEN_LINERS") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_Liners_Button_clicked);
    }
    else if (action == "OPEN_HISTORICAL_POSITION") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_HP_Button_clicked);
    }
    else if (action == "OPEN_MISSED_TRADES") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_MissedTrade_Button_clicked);
    }
    else if (action == "OPEN_TEMPLATES") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_Templates_Button_clicked);
    }
    else if (action == "OPEN_SUMMARY") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::onSummaryActionTriggered);
    }
    else if (action == "OPEN_WATCH") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::onWatchActionTriggered);
    }
    else if (action == "OPEN_LOGS") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::onLogActionTriggered);
    }
    else if (action == "OPEN_SETTINGS") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::openSettingsWindow);
    }
    else if (action == "OPEN_F1_WIN") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::F1_clicked_slot);
    }
    else if (action == "OPEN_F2_WIN") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::F2_clicked_slot);
    }
    /*************Algo Hotkey ****************************/
    else if (action == "OPEN_BUILD_ALOG") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::ConvertAlgo_button_clicked);
    }
    else if (action == "ENABLE_DISABLE_SELECTED_ALGO") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::on_Algorithms_Button_clicked);
        QObject::connect(shortcutAction, &QAction::triggered, this, [this]() {
              QModelIndexList selection = getSelectedPortFolioIndexs();
              if(selection.count()>0)
              {
                  updatePortFolioStatus(selection.first());
              }

        });
    }
    else if (action == "OPEN_ALGO_INFO_WIN") {
        QObject::connect(shortcutAction, &QAction::triggered, this, [this]() {
            QModelIndexList selection = T_Portfolio_Table->selectionModel()->selectedIndexes();
            if (selection.isEmpty()) {
                QMessageBox msgBox;
                msgBox.setText("Please select a Portfolio from Portfolio Table.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
                return;
            } else {
                // Get the first selected cell (QModelIndex)

                // Pass the first selected row to the double-click handler
                QModelIndex firstSelectedIndex = selection.first();

                // Get the row from the first selected index
                int row = firstSelectedIndex.row();

                // Specify the column you want to target (e.g., PortfolioData_Idx::_AlgoName)
                int col = PortfolioData_Idx::_AlgoName;

                // Create a new index for the same row but specific column
                QModelIndex targetIndex = T_Portfolio_Table->model()->index(row, col);

                // Call the double-click function with the new index
                T_Portfolio_Table_cellDoubleClicked(targetIndex);

            }
        });

    }
    else if (action == "IMPORT_ALGOS") {
        QObject::connect(shortcutAction, &QAction::triggered, this, [this]()
        {
        });
    }
    else if (action == "EXPORT_ALGOS") {
        QObject::connect(shortcutAction, &QAction::triggered, this, [this]() {
        });
    }
    else if (action == "OPEN_ALGO_SORT_WIN") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::sorting_Button_clicked);
    }
    else if (action == "ENABLE_ALL_ALGO") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::startall_Button_clicked);
    }
    else if (action == "DISABLE_ALL_ALGO") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::stopall_Button_clicked);
    }
    else if (action == "DELETE_SELECTED_ALGO") {
        connect(shortcutAction, &QAction::triggered, this, &MainWindow::Delete_clicked_slot);
    }
    /*******************************************************/
    /*************Trade Win Hotkey ****************************/
    else if (action == "OPEN_TRADE_DETAILS_WIN") {
        QObject::connect(shortcutAction, &QAction::triggered, this, [this]() {
            QModelIndexList selection = trade_table->selectionModel()->selectedIndexes();
            if (selection.isEmpty()) {
                QMessageBox msgBox;
                msgBox.setText("Please select a Portfolio from Trade Table.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
                return;
            } else {
                // Get the first selected cell (QModelIndex)
                QModelIndex firstSelectedCell = selection.at(OrderBook_Idx::AlgoName_OB);
                // Pass the first selected cell to the double-click handler
                trade_table_cellDoubleClicked(firstSelectedCell);
            }
        });
    }
    /*******************************************************/





}


void MainWindow::updateHotKeysSlot(QStringList actions) {
    for (const QString &action : actions) {
        if (!HotKeyShortcutObjects.contains(action)) {
            continue; // Skip if the shortcut is not found
        }

        HotKeyDetails hotKey = sett_win->HotKeysHash[action];
        QAction *shortcutAction = HotKeyShortcutObjects[action];

        // Update shortcut key and enable/disable state
        shortcutAction->setShortcut(QKeySequence(hotKey.shortcut));
        shortcutAction->setEnabled(hotKey.enabled);
        shortcutAction->setShortcutContext(Qt::ApplicationShortcut);

        // Remove and re-add the action to refresh the shortcut system
        removeAction(shortcutAction);
       // updateHotKeysAction(action,shortcutAction);
        addAction(shortcutAction);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        ui->lineEditSearch->clear();
    }
}




void MainWindow::instilizeWatchUIOnTopBar(){

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if(settings.contains("Watch_InstrumentNames_MainWin")){
            QString watch=  settings.value("Watch_InstrumentNames_MainWin").toString();
            if(watch=="")
                savedWatchItems.clear();
            else
                savedWatchItems = watch.split(";");
        }
        settings.endGroup();
    }


    QHash<QString, Indices_Data_Struct> indicesDataList;
    QString fileName = appDataPath+"/Data/indices_data.bin";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for reading--> indices_data.bin");
    }
    else{
        QDataStream in(&file);
        in >> indicesDataList;
        file.close();
    }



    ui->horizontalLayout_Watch->setSpacing(5);
    watchCardWidgetList.clear();

    for(int i=0;i<savedWatchItems.length();i++){
        if(indicesDataList.contains(savedWatchItems[i])){
           addWatchDataCard_TO_UI(indicesDataList[savedWatchItems[i]]);
        }
        else{
            Indices_Data_Struct tmp;
            tmp.indexName = savedWatchItems[i];
            tmp.indexValue = "-";
            tmp.change = "-";
            tmp.percentagechange = "-";
            tmp.netChangeIndicator = "-";
            tmp.openingIdx = "-";
            tmp.closingIdx = "-";
            tmp.highIndexValue = "-";
            tmp.lowIndexValue = "-";
            tmp.marketCapitialisation = "-";
            addWatchDataCard_TO_UI(tmp);
        }
    }


}

void MainWindow::add_remove_watch_card_slot(bool add,Indices_Data_Struct data){
    if(add){
       addWatchDataCard_TO_UI(data);
    }
    else{
      removeWatchDataCard_From_UI(data);
    }
}

void MainWindow::addWatchDataCard_TO_UI(Indices_Data_Struct data){
    auto widget1 = new watch_data_card();
    widget1->setData(data);
    watchCardWidgetList.append(widget1);
    // Add the widget to the horizontal layout
    ui->horizontalLayout_Watch->addWidget(widget1);
   // ui->horizontalLayout_Watch->layout()->activate();

    QTimer::singleShot(0, this, [=]() {
      //  qDebug() << "Updated Watch width : " << ui->horizontalLayout_Watch->geometry().width();
        adjustTopBarLayout();

    });
}

void MainWindow::removeWatchDataCard_From_UI(Indices_Data_Struct data){
    for (int i = 0; i < watchCardWidgetList.size(); ++i) {
          auto widget = watchCardWidgetList[i];
          if (widget->getIndexName() == data.indexName) {  // Assuming getData() returns Indices_Data_Struct for comparison
              ui->horizontalLayout_Watch->removeWidget(widget);  // Remove from layout
              watchCardWidgetList.removeAt(i);                   // Remove from list
              widget->deleteLater();                             // Schedule widget for deletion
              break;
          }
      }
   // ui->horizontalLayout_Watch->layout()->activate();

    QTimer::singleShot(0, this, [=]() {
       // qDebug() << "Updated Watch width : " << ui->horizontalLayout_Watch->geometry().width();
        adjustTopBarLayout();
    });

}



void MainWindow::updateWatchDataCard(Indices_Data_Struct data){
   for(int i=0;i<watchCardWidgetList.size();i++){
       if(watchCardWidgetList.at(i)->data.indexName ==data.indexName){
           watchCardWidgetList[i]->setData(data);
       }
   }

}


void MainWindow::exportTableViewToCSV(){
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder to Save CSV");

    // If the user cancels, folderPath will be empty
    if (folderPath.isEmpty()) {

        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");

    QList<QAbstractItemModel*> modelsToExport;
    QStringList outfileNames;

    modelsToExport.append(trade_table->model());
    modelsToExport.append(net_pos_table->model());

    outfileNames.append(trade_table->objectName()+"_"+timestamp+ ".csv");
    outfileNames.append(net_pos_table->objectName()+"_"+timestamp+ ".csv");
    bool allFilesExported = true; // Flag to track export success

    for(int i=0;i<modelsToExport.length();i++){
        QString filePath = QDir(folderPath).filePath(outfileNames.at(i));

        QFile file(filePath);

            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);

                // Export headers
                QStringList headers;
                for (int col = 0; col < modelsToExport[i]->columnCount(); ++col) {
                    headers << modelsToExport[i]->headerData(col, Qt::Horizontal).toString();
                }
                out << headers.join(",") << "\n";

                // Export data
                for (int row = 0; row < modelsToExport[i]->rowCount(); ++row) {
                    QStringList rowValues;
                    for (int col = 0; col < modelsToExport[i]->columnCount(); ++col) {
                        rowValues << modelsToExport[i]->data(modelsToExport[i]->index(row, col)).toString();
                    }
                    out << rowValues.join(",") << "\n";
                }

                file.close();
                qDebug() << "File Exported  successfully!:" << filePath;
            } else {
                qDebug() << "Export Failed !, Could not open file for writing:" << file.errorString();
                allFilesExported = false; // Mark flag as false if any export fails

            }
    }
    // Show success message if all files were exported successfully
       if (allFilesExported) {
           QMessageBox::information(this, "Export Successful", "Table data have been exported successfully!");
       } else {
           QMessageBox::warning(this, "Export Failed", "Some Table could not be exported. Please check the logs.");
       }



}

//void MainWindow::resizeEvent(QResizeEvent *event) {
//    QMainWindow::resizeEvent(event);

//    int thresholdWidth = 1300;  // Set your width threshold

//    // Cast the layout to QGridLayout
//    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->widget_5->layout());
//   // qWarning() << "window width="<<event->size().width();

//    if (gridLayout) {  // Ensure the layout is a QGridLayout
//        if (event->size().width() < thresholdWidth) {

//            ui->widget_5->setMinimumHeight(90);
//            ui->widget_5->setMaximumHeight(90);

//            // Move widget to the second row if width is below threshold
//            gridLayout->removeWidget(ui->widgetButtonParent);
//            gridLayout->addWidget(ui->widgetButtonParent, 1, 0);
//        } else {
//            ui->widget_5->setMinimumHeight(50);
//            ui->widget_5->setMaximumHeight(50);

//            // Restore to the original position if width is above threshold
//            gridLayout->removeWidget(ui->widgetButtonParent);
//            gridLayout->addWidget(ui->widgetButtonParent, 0, 2);
//        }
//    } else {
//        qWarning() << "widget_5 layout is not a QGridLayout";
//    }
//}

int MainWindow::getWatchWindowChildWidth(QHBoxLayout* layout) {
    int layoutWidth = layout->geometry().width();  // Width of the layout
    int totalChildWidth = 0;

    // Iterate over each item in the layout
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem* item = layout->itemAt(i);
        if (item->widget()) {
            totalChildWidth += item->widget()->width();  // Sum the widths of each child widget
        }
    }
    return totalChildWidth;

//    // Check if child widgets are overlapping the layout width
//    if (totalChildWidth > layoutWidth) {
//      //  qDebug() << "Warning: Child widgets are overlapping in the layout.";
//        return true;
//    } else {
//        //qDebug() << "All child widgets fit within the layout.";
//        return false;
//    }
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    adjustTopBarLayout();
}
void MainWindow::adjustTopBarLayout(){
   // static int thresholdWidth = 0;  // Set your width threshold
   // static bool one_row = false;

    bool smallScreen = false;

    int parentWidth = ui->widget_5->geometry().width();
    int otherItemWidth = ui->widget->geometry().width()+getWatchWindowChildWidth(ui->horizontalLayout_Watch);
    int buttonParentWidth = ui->widgetButtonParent->geometry().width();
    int remainingWidth = parentWidth-otherItemWidth;
    if(remainingWidth <buttonParentWidth){
        smallScreen = true;
    }
//    qDebug() << "parentWidth       : "<<parentWidth;
//    qDebug() << "otherItemWidth    : "<<otherItemWidth<<"="<<ui->widget->geometry().width()<<"-"<<getWatchWindowChildWidth(ui->horizontalLayout_Watch);
//    qDebug() << "remainingWidth    : "<<remainingWidth;
//    qDebug() << "buttonParentWidth : "<<buttonParentWidth;

//    qDebug() << "watch Width      : "<<ui->horizontalLayout_Watch->geometry().width();
//    qDebug() << "watch ChildWidth : "<<getWatchWindowChildWidth(ui->horizontalLayout_Watch);



//    int currentWidth =this->geometry().width();
//    if(one_row){
//        if(checkChildOverlapLayout(ui->horizontalLayout_Watch)){
//           thresholdWidth = currentWidth;
//        }
//    }

//    if(currentWidth <= thresholdWidth)
//        smallScreen = true;

//    if(!one_row){
//        int parentWidth = ui->widget_5->geometry().width();
//        int otherItemWidth = ui->widget->geometry().width()+ui->horizontalLayout_Watch->geometry().width();
//        int buttonParentWidth = ui->widgetButtonParent->geometry().width();

//        qDebug() << "parentWidth       : "<<parentWidth;
//        qDebug() << "otherItemWidth    : "<<otherItemWidth;
//        qDebug() << "buttonParentWidth : "<<buttonParentWidth;

//        if((parentWidth-otherItemWidth)>buttonParentWidth){
//            smallScreen = false;
//            thresholdWidth =0;
//        }
//        else{
//            thresholdWidth = currentWidth;
//        }
//    }
//    //check

//    qDebug() << "currentWidth   : "<<currentWidth;
//    qDebug() << "thresholdWidth : "<<thresholdWidth;

//    qDebug() << "parent_Width       : "<<ui->widget_5->geometry().width();
//    qDebug() << "ch1+ch2+ch3        : "<<ui->widgetButtonParent->geometry().width()+ui->widget->geometry().width()+ui->horizontalLayout_Watch->geometry().width();
//    qDebug() << "widgetButtonParent : "<<ui->widgetButtonParent->geometry().width();
//    qDebug() << "widget5            : "<<ui->widget ->geometry().width();
//    qDebug() << "Watch              : "<<ui->horizontalLayout_Watch->geometry().width();

    // Cast the layout to QGridLayout
    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->widget_5->layout());
    if (gridLayout) {  // Ensure the layout is a QGridLayout
        if (smallScreen) {
            // Adjust widget_5 height
            ui->widget_5->setMinimumHeight(90);
            ui->widget_5->setMaximumHeight(90);

            // Clear existing layout items
            while (QLayoutItem *item = gridLayout->takeAt(0)) {
                gridLayout->removeItem(item);
            }

            // Add widgets for smaller width - two rows
            gridLayout->addLayout(ui->horizontalLayout_Watch, 0, 0);
            gridLayout->addItem(ui->horizontalSpacer_3, 0, 1);
            gridLayout->addWidget(ui->widget, 0, 2);
            gridLayout->addWidget(ui->widgetButtonParent, 1, 0, 1, 3); // Span across columns
          //  one_row = false;
        } else {
            // Adjust widget_5 height for larger width
            ui->widget_5->setMinimumHeight(50);
            ui->widget_5->setMaximumHeight(50);

            // Clear existing layout items
            while (QLayoutItem *item = gridLayout->takeAt(0)) {
                gridLayout->removeItem(item);
            }

            // Add widgets for larger width - single row
            gridLayout->addLayout(ui->horizontalLayout_Watch, 0, 0);
            gridLayout->addItem(ui->horizontalSpacer_3, 0, 1);
            gridLayout->addWidget(ui->widgetButtonParent, 0, 2);
            gridLayout->addWidget(ui->widget, 0, 3);
           // one_row = true;
           // thresholdWidth =0;

        }
    } else {
        qWarning() << "widget_5 layout is not a QGridLayout";
    }
}
// Serialize portfolioImportExportData to QDataStream
QDataStream &operator<<(QDataStream &out, const portfolioImportExportData &data) {
    out << data.PortfolioType
        << data.TraderID
        << data.ClientID
        << data.Status
        << data.IsBroker
        << data.Leg1TokenNo
        << data.Leg2TokenNo
        << data.Leg3TokenNo
        << data.Leg4TokenNo
        << data.Leg5TokenNo
        << data.Leg6TokenNo
        << data.SellPriceDifference
        << data.SellTotalQuantity
        << data.SellTradedQuantity
        << data.BuyPriceDifference
        << data.BuyTotalQuantity
        << data.BuyTradedQuantity
        << data.AdditionalData1
        << data.AdditionalData2
        << data.AdditionalData3
        << data.AdditionalData4
        << data.Alias;
    return out;
}

// Deserialize portfolioImportExportData from QDataStream
QDataStream &operator>>(QDataStream &in, portfolioImportExportData &data) {
      in  >> data.PortfolioType
          >> data.TraderID
          >> data.ClientID
          >> data.Status
          >> data.IsBroker
          >> data.Leg1TokenNo
          >> data.Leg2TokenNo
          >> data.Leg3TokenNo
          >> data.Leg4TokenNo
          >> data.Leg5TokenNo
          >> data.Leg6TokenNo
          >> data.SellPriceDifference
          >> data.SellTotalQuantity
          >> data.SellTradedQuantity
          >> data.BuyPriceDifference
          >> data.BuyTotalQuantity
          >> data.BuyTradedQuantity
          >> data.AdditionalData1
          >> data.AdditionalData2
          >> data.AdditionalData3
          >> data.AdditionalData4
          >> data.Alias;
    return in;
}
void MainWindow::import_Action(){

        QString fileName = QFileDialog::getOpenFileName(
            nullptr,
            "Import  Portfolios",
            "",
            "Portfolios Files (*.bin);;All Files (*)"
        );
        QString msg;

        if (!fileName.isEmpty()) {
            QList<portfolioImportExportData> pData ;
            QFile file(fileName);
              if (!file.open(QIODevice::ReadOnly)) {
                  QMessageBox::critical(nullptr, "Error", "Failed to open file for reading.");
                  return;
              }

            QDataStream in(&file);
            in >> pData;
            if(pData.size()>0){
                if(!db_conn->clearAlogsForImport(QString::number(userData.UserId),msg))
                    QMessageBox::warning(nullptr, "Warning: Clear DB", msg);

                if(db_conn->insertAlogsForImport(pData,msg)){
                    triggerImmediate_refreshTables();
                    db_conn->logToDB("Portfolios imported.");
                    QMessageBox::information(nullptr, "Success", "Portfolios imported successfully!");
                }
                else{
                    QMessageBox::warning(nullptr, "Warning: Portfolios import Failed", msg);
                }
            }
            else{
                QMessageBox::warning(nullptr, "Failed", "No portfolios in selected file. !");
            }
            file.close();

        }
        else{
            QMessageBox::warning(nullptr, "Failed", "Selected file is empty!.");
        }
}

void MainWindow::export_Action(){

    QList<portfolioImportExportData> pData = T_Portfolio_Model->getPortFolioToExport(); //get data to export
    if(pData.size()==0){
        QMessageBox::warning(nullptr, "Warning","No traded portfolios avaiable to export.");
        return;
    }

        QString fileName = QFileDialog::getSaveFileName(
            nullptr,
            "Export Portfolios",
            "",
            "Portfolios Files (*.bin);;All Files (*)"
        );

        if (!fileName.isEmpty()) {
             QFile file(fileName);
               if (!file.open(QIODevice::WriteOnly)) {
                   QMessageBox::critical(nullptr, "Error", "Failed to open file for writing.");
                   return;
               }
               QList<portfolioImportExportData> pData = T_Portfolio_Model->getPortFolioToExport(); //get data to export
               QDataStream out(&file);
               out << pData;
               db_conn->logToDB("Portfolios exported.");

               QMessageBox::information(nullptr, "Success", "Portfolios exported successfully!");
               file.close();

        }
        else{
            QMessageBox::warning(nullptr, "Failed","No File selected!.");
        }
}
void MainWindow::onRetryButtonClickedMissedTrade(int row){
       if (!missed_trade_table) {
           return;  // Safety check
       }

       if(backendConn_Status==false){
           QMessageBox msgBox;
           msgBox.setWindowTitle("Failed.");
           msgBox.setIcon(QMessageBox::Warning);
           msgBox.setText("Unable to connect the Server");
           msgBox.exec();
           return;
       }

       // Set the current index to the first column of the clicked row
       QModelIndex index = missed_trade_table->model()->index(row, 0);
       missed_trade_table->setCurrentIndex(index);

       // Select the entire row
       missed_trade_table->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);

      QStringList data =  missed_trade_model->getDataForPlacingOrder(row);
      if(data.size()==0){
          QMessageBox msgBox;
          msgBox.setWindowTitle("Failed.");
          msgBox.setIcon(QMessageBox::Warning);
          msgBox.setText("Cannot to get data for selected row.");
          msgBox.exec();
          return;
      }

      QString sellprice="0";
      QString sellqty="0";
      QString buyprice="0";
      QString buyqty="0";
      QString orderQunatity = "0";
      QString msg="0";
      QString confirmationMsg ="";
      QString Stockname = data[Missed_Trades_Idx::Stock_Name];
      QString BuySell = data[Missed_Trades_Idx::BuySell];
      QString token_number = data[Missed_Trades_Idx::Token_No];
      int lotSize=ContractDetail::getInstance().GetLotSize(token_number.toInt(),PortfolioType::F1_F2);
      QString orderID = data[Missed_Trades_Idx::OrderId];
      QString ID = data[Missed_Trades_Idx::Missed_Trade_Id];
      QHash<QString, MBP_Data_Struct>  MBP_Data_Hash = slowData->getMBP_Data_Hash();
      double lastTradedPrice = 0;
      if(MBP_Data_Hash.contains(token_number)){
          contract_table c = ContractDetail::getInstance().GetDetail(token_number.toInt());
          MBP_Data_Struct mbpData = MBP_Data_Hash[token_number];
          lastTradedPrice = mbpData.lastTradedPrice.toDouble();
          double offeset = 5*devicer;// in paise
          if(BuySell=="Buy"){
              lastTradedPrice = lastTradedPrice+offeset;
              if(lastTradedPrice<c.MinimumSpread)
                  lastTradedPrice = c.MinimumSpread;

              buyprice = QString::number(lastTradedPrice);
              buyqty = QString::number(data[Missed_Trades_Idx::Lot].toInt()* lotSize);
              orderQunatity = buyqty;
              confirmationMsg ="Buy "+Stockname+/*" @ "+data[Missed_Trades_Idx::Price]+*/" for "+data[Missed_Trades_Idx::Lot]+" lot. Would you like to proceed?";
              qDebug()<<"Retry order ---> LTP:"<<mbpData.lastTradedPrice.toDouble()<<" buyprice: "<<buyprice<<" MinimumSpread:"<<c.MinimumSpread;
          }
          else{
             // sellprice = QString::number(data[Missed_Trades_Idx::Price].toDouble()* devicer);
              lastTradedPrice = lastTradedPrice-offeset;
              if(lastTradedPrice<c.MinimumSpread)
                  lastTradedPrice = c.MinimumSpread;
              sellprice = QString::number(lastTradedPrice);
              sellqty = QString::number(data[Missed_Trades_Idx::Lot].toInt()* lotSize);
              orderQunatity = sellqty;
              confirmationMsg ="Sell "+Stockname/*+" @ "+data[Missed_Trades_Idx::Price]*/+" for "+data[Missed_Trades_Idx::Lot]+" lot. Would you like to proceed?";
              qDebug()<<"Retry order Sell ---> LTP:"<<mbpData.lastTradedPrice.toDouble()<<" sellprice: "<<sellprice<<" MinimumSpread:"<<c.MinimumSpread;

          }



           QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "Add Manual Order?", confirmationMsg,QMessageBox::Yes | QMessageBox::No);

           if (reply == QMessageBox::Yes) {
               mysql_conn *db_conn = new mysql_conn(0, "add_algo_db_conn");
               algo_data_insert_status status = db_conn->retry_F1F2_Order(orderID,ID,QString::number(userData.UserId),token_number, sellprice, sellqty, buyprice, buyqty,userData.MaxPortfolioCount,orderQunatity,msg,true);
               if(status == algo_data_insert_status::EXIST){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Duplicate record!", "Record already exist for selected Order Id!. Do you want to proceed?",QMessageBox::Yes | QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        algo_data_insert_status status1 = db_conn->retry_F1F2_Order(orderID,ID,QString::number(userData.UserId),token_number, sellprice, sellqty, buyprice, buyqty,userData.MaxPortfolioCount,orderQunatity,msg,false);
                        if(status1 == algo_data_insert_status::INSERTED){
                                sendBackendCommandOnAddPortfolio();
                                QMessageBox msgBox;
                                msgBox.setWindowTitle("Success");
                                msgBox.setIcon(QMessageBox::Information);
                                msgBox.setText("Order placed successfully.");
                                msgBox.exec();
                            }
                            else{
                                QMessageBox msgBox;
                                msgBox.setWindowTitle("Failed.");
                                msgBox.setIcon(QMessageBox::Warning);
                                msgBox.setText(msg);
                                msgBox.exec();
                            }
                    }
               }
               else if(status == algo_data_insert_status::INSERTED){
                   sendBackendCommandOnAddPortfolio();
                   QMessageBox msgBox;
                   msgBox.setWindowTitle("Success");
                   msgBox.setIcon(QMessageBox::Information);
                   msgBox.setText("Order placed successfully.");
                   msgBox.exec();
               }
               else{
                   QMessageBox msgBox;
                   msgBox.setWindowTitle("Failed");
                   msgBox.setIcon(QMessageBox::Warning);
                   msgBox.setText(msg);
                   msgBox.exec();
               }

               delete db_conn;
           }

      }
      else{
          qDebug()<<"LTP cannot find with the token number "<<token_number<<" , skipping retry!";
          QMessageBox msgBox;
          msgBox.setWindowTitle("Cannot Retry Order.");
          msgBox.setIcon(QMessageBox::Warning);
          msgBox.setText("LTP cannot find with the token number in Contract, please try manually");
          msgBox.exec();
      }


}



#ifdef BACKEND_LOG
void MainWindow::write_backend_Log(const QString &message) {
    if (backend_logFile.isOpen()) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        backend_logStream << timestamp << " - " << message << "\n";
        backend_logStream.flush();
    }
}
#endif
