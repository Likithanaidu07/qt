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
#include "ContractDetail.h"
#include "OrderBook/table_orderbook_delegate.h"
#include "mysql_conn.h"
#include "watch_data_list_item.h"
static const char stylesheetvis[]= "background: #596167;" "border-radius: 10px;";

extern MainWindow *MainWindowObj;





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QFontDatabase::addApplicationFont(":/WorkSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/WorkSans-ExtraBold.ttf");

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(this,SIGNAL(display_log_text_signal(QString)),this,SLOT(slotAddLogForAddAlgoRecord(QString)));
    convertalgo=new ConvertAlgo_Win(this);

    // db_conn =new mysql_conn(this,"main_db_conn");
    db_conn =new mysql_conn(0,"main_db_conn");
    connect(db_conn,SIGNAL(display_log_text_signal(QString)),this,SLOT(slotAddLogForAddAlgoRecord(QString)));

    connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));


//    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(convertalgo);
//    effect->setBlurRadius(5);
//    convertalgo->setGraphicsEffect(effect);
//    convertalgo->update_contract_tableData(QString::number(userData.UserId),userData.MaxPortfolioCount);

    createINIFileIfNotExist();
    QDir dir;
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    if (!dir.exists(path)) {
        if (dir.mkpath(path)) {
        }
        else{
            qDebug() << "Failed to create directory! "+path;
        }
    }
    loadSettings();

    contractDetailsLoaded.storeRelaxed(0);
    quit_db_data_load_thread.storeRelaxed(0);
    portfolio_table_updating_db.storeRelaxed(0);
    portfolio_table_slected_idx_for_editing.store(-1);
    loggedInFlg.storeRelaxed(0);


    QPixmap pixmapclose(":/close_window_icon.png");
    QPixmap pixmapminimize(":/minimize_window_icon.png");
    QPixmap pixmapmaximize(":/maximize_window_icon.png");



    QFont headerfont("Work Sans");


    const char dock_style[] = "QDockWidget > QWidget{"
                              "border-bottom-right-radius: 12px;"
                              "border-bottom-left-radius: 12px;"
                              "border: 3px solid #F8F8F8;"
                              "box-shadow: 0px 1px 3px 0px #A8A8A8;"
                              "background: #FFF;"
                              "}"
                              "QDockWidget::title {"
                              "background-color: #F8F8F8;"
                              "border-top-left-radius: 12px;"
                              "border-top-right-radius: 12px;"
                              "padding: 4px;"
                              "}";
    const char DockTitleBar_Style[]="background: #FFF;"
                                    "border-top-left-radius :12px;"
                                    "border-top-right-radius : 12px;"
                                    "border-bottom-left-radius : 0px;"
                                    "border-bottom-right-radius : 0px;";
    const char lineedit_dock_SS[]="border-radius: 10px;"
                                  "border: 1px solid #ADB5BD;"
                                  "background: #FFF;";
    const char scroll_bar_SS[]="QTableView {"
                                 "border-bottom-left-radius : 12px;"
                                 "border-bottom-right-radius : 12px;"
                                 "border: 4px solid #DAE1E9;"
                                 "}"
                                "QScrollBar:vertical {"
                                 "    border: 1px solid #FFF;"
                                 "    background:transparent;"
                                 "    width:5px;    "
                                 "    margin: 0px 0px 0px 0px;"
                                 "}"
                                 "QScrollBar::handle:vertical {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    min-height: 0px;"
                                 "border-radius: 1px;"
                                 "}"
                                 "QScrollBar::add-line:vertical {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                                 "    height: 0px;"
                                 "    subcontrol-position: bottom;"
                                 "    subcontrol-origin: margin;"
                                 "border-radius: 1px;"
                                 "}"
                                 "QScrollBar::sub-line:vertical {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                 "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                                 "    height: 0 px;"
                                 "    subcontrol-position: top;"
                                 "    subcontrol-origin: margin;"
                                 "border-radius: 1px;"
                                 "}"
                                 "QScrollBar:horizontal {"
                                 "    border: 1px solid #FFF;"
                                 "    background: transparent;"
                                 "    height: 6px;    "
                                 "    margin: 0px 0px 0px 0px;"
                                 "}"
                                 "QScrollBar::handle:horizontal {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    min-width: 0px;"
                                 "    border-radius: 2px;"
                                 "}"
                                 "QScrollBar::add-line:horizontal {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    width: 0px;"
                                 "    subcontrol-position: right;"
                                 "    subcontrol-origin: margin;"
                                 "    border-radius: 2px;"
                                 "}"
                                 "QScrollBar::sub-line:horizontal {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                 "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    width: 0px;"
                                 "    subcontrol-position: left;"
                                 "    subcontrol-origin: margin;"
                                 "    border-radius: 2px;"
                                 "}";
    const char tableview_SS[]=  "QHeaderView::section {"
                                "   background: #495867;"
                                "   border: none;"
                                "   color: #FFFFFF;"
                                "   text-align: center; "
                                "   font-size: 12px; "
                                "   font-style: normal; "
                                "   font-weight: 600; "
                                "   line-height: normal;"
                                "}"
                                "QTableView {"
                                 "border-bottom-left-radius : 12px;"
                                 "border-bottom-right-radius : 12px;"
                                 "border: 4px solid #DAE1E9;"
                                 "}"
                                 "QScrollBar:vertical {"
                                 "    border: 1px solid #FFF;"
                                 "    background:transparent;"
                                 "    width:5px;    "
                                 "    margin: 0px 0px 0px 0px;"
                                 "}"
                                 "QScrollBar::handle:vertical {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    min-height: 0px;"
                                 "border-radius: 1px;"
                                 "}"
                                 "QScrollBar::add-line:vertical {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                                 "    height: 0px;"
                                 "    subcontrol-position: bottom;"
                                 "    subcontrol-origin: margin;"
                                 "border-radius: 1px;"
                                 "}"
                                 "QScrollBar::sub-line:vertical {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                 "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87),  stop:1 rgb(73, 80, 87));"
                                 "    height: 0 px;"
                                 "    subcontrol-position: top;"
                                 "    subcontrol-origin: margin;"
                                 "border-radius: 1px;"
                                 "}"
                                 "QScrollBar:horizontal {"
                                 "    border: 1px solid #FFF;"
                                 "    background: transparent;"
                                 "    height: 6px;    "
                                 "    margin: 0px 0px 0px 0px;"
                                 "}"
                                 "QScrollBar::handle:horizontal {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    min-width: 0px;"
                                 "    border-radius: 2px;"
                                 "}"
                                 "QScrollBar::add-line:horizontal {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                 "    stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    width: 0px;"
                                 "    subcontrol-position: right;"
                                 "    subcontrol-origin: margin;"
                                 "    border-radius: 2px;"
                                 "}"
                                 "QScrollBar::sub-line:horizontal {"
                                 "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
                                 "    stop: 0  rgb(73, 80, 87), stop: 0.5 rgb(73, 80, 87), stop:1 rgb(73, 80, 87));"
                                 "    width: 0px;"
                                 "    subcontrol-position: left;"
                                 "    subcontrol-origin: margin;"
                                 "    border-radius: 2px;"
                                 "}";


    QMainWindow *subWindow = new QMainWindow(ui->centralwidget);
    subWindow->setMinimumHeight(500);
    subWindow->setMinimumWidth(900);
    subWindow->setWindowFlags(Qt::Widget);
    ui->verticalLayout_11->addWidget(subWindow);
    subWindow->show();


//    /***********Init portfolio table Window**************************/


//    QPixmap pixmapdock_algorithms_close(":/dock_close.png");


//    T_Portfolio_DockWin =  new QDockWidget(tr("Algorithms"), this);

//    //connect(T_Portfolio_DockWin, SIGNAL(WidgetClosed(UiUtils::DockWidgetType)), this, SLOT(OnDockWidgetClose(UiUtils::DockWidgetType)));
//    connect(T_Portfolio_DockWin, SIGNAL(visibilityChanged(bool)), this, SLOT(OnAlgorithmDockWidgetVisiblityChanged(bool)));

//    subWindow->addDockWidget(Qt::LeftDockWidgetArea, T_Portfolio_DockWin);
//    T_Portfolio_DockWin->setAllowedAreas(Qt::AllDockWidgetAreas);

//    T_Portfolio_DockWin->setStyleSheet(dock_style);

//    QWidget* titlecontainer = new QWidget;
//    titlecontainer->setStyleSheet(DockTitleBar_Style);
//    QHBoxLayout *titleinternalLayout = new QHBoxLayout(titlecontainer);
//    titleinternalLayout->setSpacing(10);
//    titleinternalLayout->setContentsMargins(10,8,10,6);

//    QLabel *label = new QLabel("Algorithms");
//    QFont fontlabel=label->font();
//    QSpacerItem* titlespc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

//    QToolButton* dockclose=new QToolButton();
//    connect(dockclose, &QToolButton::clicked, [=](){ T_Portfolio_DockWin->close(); });
//    dockclose->setIcon(pixmapdock_algorithms_close);
//    dockclose->setIconSize(QSize(14, 14));

//    fontlabel.setFamily("Work Sans");
//    label->setFont(fontlabel);
//    label->setStyleSheet("color: #495057;font-size: 14px;font-style: normal;font-weight: 700;line-height: normal;");
//    titleinternalLayout->addWidget(label);
//    titleinternalLayout->addSpacerItem(titlespc);
//    titleinternalLayout->addWidget(dockclose);

//    T_Portfolio_DockWin->setTitleBarWidget(titlecontainer);


//    QWidget* container = new QWidget;
//    T_Portfolio_DockWin->setWidget(container);
//    QGridLayout *lay = new QGridLayout(container);
//    container->setLayout(lay);
//    lay->setContentsMargins(0, 0, 0, 0);
//    lay->setSpacing(0);
//    lay->setRowStretch(0, 0); //set minimum hieght for first row
//    lay->setRowStretch(1, 1);//set maximum hieght for second row table


//    QLineEdit* line_edit_trade_search = new QLineEdit;
//    line_edit_trade_search->setMaximumWidth(160);

//    line_edit_trade_search->setStyleSheet(lineedit_dock_SS);

//    QToolButton *button = new QToolButton();
//    //Frame 364.png
//    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button->setIcon(QIcon(":/Convert to Algo.png"));
//    button->setText("Convert to Algo");
//    button->setLayoutDirection(Qt::LeftToRight);
//    const char convert_to_algo_button[]="border-radius: 4px;" "border: 1px solid #188CCA;""background: #1585C0;"
//                                        "color: #FFF;"
//                                        "font-family: Work Sans;"
//                                        "font-size: 12px;"
//                                        "font-style: normal;"
//                                        "font-weight: 500;"
//                                        "line-height: normal;";
//    button->setStyleSheet(convert_to_algo_button);
////    connect(button, &QPushButton::clicked, this, &MainWindow::tradeTableSerachNext);
////    connect(line_edit_trade_search, &QLineEdit::textChanged, this, &MainWindow::tradeTableSerachTxtChanged);

//    QWidget *test=new QWidget;
//    const char layss[]="background: #E9ECEF;";
//    test->setStyleSheet(layss);
//    QHBoxLayout *internalLayout = new QHBoxLayout(test);
//    internalLayout->setContentsMargins(10,-1,-1,-1);
//    QSpacerItem* spc = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
//    QToolButton* button1=new QToolButton();
//    button1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button1->setIcon(QIcon(a1));
//    button1->setText("Start All");
//    QToolButton* button2=new QToolButton();
//    button2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button2->setIcon(QIcon(a2));
//    button2->setText("Stop All");
//    QToolButton* button3=new QToolButton();
//    button3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button3->setIcon(QIcon(a3));
//    button3->setText("Import");
//    QToolButton* button6=new QToolButton();
//    button6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button6->setIcon(QIcon(a4));
//    button6->setText("Export");
//    QToolButton* button4=new QToolButton();
//    button4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button4->setIcon(QIcon(a5));
//    button4->setText("Reset Sorting");
//    QToolButton* button5=new QToolButton();
//    button5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    button5->setIcon(QIcon(a6));
//    button5->setText("Portfolio Colour");
//    const char stylesheet_tb[] = "border-radius: 4px;"
//                                 "border: 1px solid #ADB5BD;"
//                                 "background: #F3F4F5;"
//                                 "color: #6C757D;"
//                                 "font-style: normal;"
//                                 "font-size: 10px;"
//                                 "font-weight: 500;"
//                                 "line-height: normal;";
//    for (auto w : {button1,button2,button3,button4,button5,button6}){
//        w->setStyleSheet(stylesheet_tb);
//        QFont font=w->font();
//        font.setFamily("Work Sans");
//        w->setFont(font);
//    }


//    internalLayout->addWidget(button1);
//    internalLayout->addWidget(button2);
//    internalLayout->addWidget(button3);
//    internalLayout->addWidget(button6);
//    internalLayout->addWidget(button4);
//    internalLayout->addWidget(button5);
//    internalLayout->addSpacerItem(spc);
//    internalLayout->addWidget(line_edit_trade_search);
//    internalLayout->addWidget(button);

//    test->setLayout(internalLayout);
//    lay->addWidget(test, 0, 0);


////    QObject::connect(T_Portfolio_Table, &table_portfolios_custom::spaceKeySignal, this, &MainWindow::updatePortFolioStatus);
////    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::resizePortFolioTableColWidth, this, &MainWindow::resizePortFolioTableColWidthSlot);

//    //
//    for (int i = 0; i <= 19; i++) {
//        QList<QStandardItem *> items;
//        for (int j = 0; j <= 19; j++) {
//            if(i==19 && j==19){
//                items.append(new QStandardItem(QString("item(20,20)").arg(18).arg(18)));
////                continue;
//            }
//            items.append(new QStandardItem(QString("item(%1,%2)").arg(i).arg(j)));
//        }
//        model.appendRow(items);
//    }
//    QWidget *w=new QWidget;
//    lay->addWidget(w,1,0);
//    QVBoxLayout *testLayout = new QVBoxLayout(w);
//    w->setLayout(testLayout);

//    testLayout->setContentsMargins(0,0,0,0);
//    gridtableview.setStyleSheet(
//        "QHeaderView::section {"
//        "   background: #CED4DA;"
//        "   border: none; "
//        "   color: #6C757D; "
//        "   text-align: center; "
//        "   font-family: Work Sans; "
//        "   font-size: 12px; "
//        "   font-style: normal; "
//        "   font-weight: 600; "
//        "   line-height: normal;"
//        "}"
//        "QTableView::item {"
//        "   background-color: white;"
//        "background: transparent;"
//        "   color: black;"
//        "}"
//        );
//    "QTableView::item {"
//    "background-color: #FFF;"
//    "border: none; "
//    "color: #212529;"
//    "font-family: Work Sans;"
//    "font-size: 8px;"
//    "font-style: normal;"
//    "font-weight: 600;"
//    "line-height: normal;"
//    "}"
//    "QTableView::item:alternate {"
//    "border: none; "
//    "background-color: #F1F1F1; /* Set background color for alternate rows */"
//    "}"


//    gridtableview.setModel(&model);
//    gridtableview.setGridHeaderview(Qt::Horizontal, 2);
//    gridtableview.verticalHeader()->setVisible(false);
//    GridTableHeaderView *hHeader = gridtableview.gridHeaderview(Qt::Horizontal);

//    hHeader->setSpan(0, 0, 1, 3);
//    hHeader->setSpan(0, 3, 1, 6);
//    hHeader->setSpan(0, 9, 1, 6);
//    hHeader->setSpan(0, 15, 1, 5);
//    hHeader->setSpan(0, 20);

//    hHeader->setCellLabel(0, 0, "Algo");
//    hHeader->setCellLabel(1, 0, "Status");
//    hHeader->setCellLabel(1, 1, "Algo ID");
//    hHeader->setCellLabel(1, 2, "Algo Name");
//    hHeader->setCellLabel(0, 3, "Buy");
//    hHeader->setCellLabel(1, 3, "Rate");
//    hHeader->setCellLabel(1, 4, "Avg");
//    hHeader->setCellLabel(1, 5, "Diff");
//    hHeader->setCellLabel(1, 6, "TQ");
//    hHeader->setCellLabel(1, 7, "TTQ");
//    hHeader->setCellLabel(1, 8, "RQ");
//    hHeader->setCellLabel(0, 9, "Sell");
//    hHeader->setCellLabel(1, 9, "Rate");
//    hHeader->setCellLabel(1, 10, "Avg");
//    hHeader->setCellLabel(1, 11, "Diff");
//    hHeader->setCellLabel(1, 12, "TQ");
//    hHeader->setCellLabel(1, 13, "TTQ");
//    hHeader->setCellLabel(1, 14, "RQ");
//    hHeader->setCellLabel(0, 15, "Details");
//    hHeader->setCellLabel(1, 15, "Qty Ratio");
//    hHeader->setCellLabel(1, 16, "Skip/Market Strike");
//    hHeader->setCellLabel(1, 17, "Bid Leg");
//    hHeader->setCellLabel(1, 18, "Cost");
//    hHeader->setCellLabel(1, 19, "Fut Price");

//    testLayout->addWidget(&gridtableview);

//    gridtableview.show();


//    //

////    QTableView *trade_table = new QTableView();
////    trade_table->setShowGrid(false);
////    trade_table->setAlternatingRowColors(true);

////    lay->addWidget(trade_table, 1, 0, 1, 3);

////    T_Portfolio_Model = new Table_Portfolios_Model();

////    trade_table->setModel(T_Portfolio_Model);
////    trade_table->horizontalHeader()->setStretchLastSection(true);
////    trade_table->verticalHeader()->setVisible(false);
//////    // trade_table->setStyleSheet("QHeaderView { background-color: #111111;} QHeaderView::section { background-color:#555555;color:#eeeeee;font-weight: 400; }");
////    trade_table->setSelectionBehavior(QAbstractItemView::SelectRows);
////    trade_table->setSelectionMode(QAbstractItemView::SingleSelection);
//////    /*trade_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
//////                                            "selection-color: #4D4D4D;"
//////                                            "color:#3D3D3D;"
//////                                            "} "
//////                                            "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

////    trade_table->show();


    /***********Init portfolio table Window**************************/
    QPixmap pixmapdock_algorithms_close(":/dock_close.png");

    T_Portfolio_DockWin =  new QDockWidget(tr("Algorithms"), this);
    connect(T_Portfolio_DockWin, SIGNAL(visibilityChanged(bool)), this, SLOT(OnAlgorithmDockWidgetVisiblityChanged(bool)));
    T_Portfolio_DockWin->setStyleSheet(dock_style);
    subWindow->addDockWidget(Qt::TopDockWidgetArea, T_Portfolio_DockWin);
    T_Portfolio_DockWin->setAllowedAreas(Qt::AllDockWidgetAreas);


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
        T_Portfolio_DockWin->setTitleBarWidget(titlecontainer);
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
            line_edit_trade_search->setMaximumWidth(160);

            line_edit_trade_search->setStyleSheet(lineedit_dock_SS);

            QToolButton *ConvertAlgo_button = new QToolButton();
            connect(ConvertAlgo_button, SIGNAL(clicked()), this, SLOT(on_ConvertAlgo_button_clicked()));
            //Frame 364.png
            ConvertAlgo_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

            ConvertAlgo_button->setText("Build Algo");
            ConvertAlgo_button->setStyleSheet("text-align: center;");

            const char convert_to_algo_button[]="QToolButton {"
                                                  "border-radius: 4px;" "border: 1px solid #188CCA;"
                                                  "background: #1585C0;"
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
            QToolButton* button1=new QToolButton();
            button1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button1->setIcon(QIcon(a1));
            button1->setText("Start All");
            QToolButton* button2=new QToolButton();
            button2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button2->setIcon(QIcon(a2));
            button2->setText("Stop All");
            QToolButton* button3=new QToolButton();
            button3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button3->setIcon(QIcon(a3));
            button3->setText("Import");
            QToolButton* button6=new QToolButton();
            button6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button6->setIcon(QIcon(a4));
            button6->setText("Export");
            QToolButton* button4=new QToolButton();
            button4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button4->setIcon(QIcon(a5));
            button4->setText("Reset Sorting");
            QToolButton* button5=new QToolButton();
            button5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            //button5->setIcon(QIcon(a6));
           // button5->setText("Portfolio Colour");
            const char stylesheet_tb[] ="QToolButton {"
                                        "border-radius: 4px;"
                                        "border: 1px solid #4F5D75;"
                                        "background: #F7F7FF;"
                                        "color: #4F5D75;"
                                        "font-style: normal;"
                                        "font-size: 12px;"
                                        "font-weight: 500;"
                                        "line-height: normal;"
                                        "}"
                                        "QToolButton:hover {"
                                        "border-radius: 4px;"
                                        "border: 1px solid #4F5D75;"
                                        "background: #4F5D75;"
                                        "color: #FFF; "
                                        "font-family: Work Sans;"
                                        "font-size: 12px;"
                                        "font-style: normal;"
                                        "font-weight: 500;"
                                        "line-height: normal;"
                                        "}";
            for (auto w : {button1,button2,button3,button4,button5,button6}){
                w->setStyleSheet(stylesheet_tb);
                QFont font=w->font();
                font.setFamily("Work Sans");
                w->setFont(font);
            }

                internalLayout->addWidget(ConvertAlgo_button);
                internalLayout->addWidget(button1);
                internalLayout->addWidget(button2);
                internalLayout->addWidget(button3);
                internalLayout->addWidget(button6);
                internalLayout->addWidget(button4);
                //internalLayout->addWidget(button5);
                internalLayout->addSpacerItem(spc);
                internalLayout->addWidget(line_edit_trade_search);
                lay->addWidget(test, 0, 0);

    T_Portfolio_Table = new table_portfolios_custom(T_Portfolio_DockWin);
    T_Portfolio_Table->setStyleSheet(scroll_bar_SS);
    // T_Portfolio_Table->viewport()->setFocusPolicy(Qt::NoFocus);
    T_Portfolio_Model = new Table_Portfolios_Model();
    T_Portfolio_Delegate =  new Table_Portfolios_Delegate();
    connect(T_Portfolio_Model,SIGNAL(edit_Started(int,int)),this,SLOT(edit_Started_PortFolio_Table(int,int)));
    QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::editFinished, this, &MainWindow::profolioTableEditFinshedSlot);
    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::editCompleted, this, &MainWindow::profolioTableEditFinshedSlot);
    QObject::connect(T_Portfolio_Delegate, &Table_Portfolios_Delegate::tabKeyPressed, T_Portfolio_Table, &table_portfolios_custom::handleTabKeyPressFromEditableCell);
    QObject::connect(T_Portfolio_Table, &table_portfolios_custom::spaceKeySignal, this, &MainWindow::updatePortFolioStatus);
    QObject::connect(T_Portfolio_Model, &Table_Portfolios_Model::resizePortFolioTableColWidth, this, &MainWindow::resizePortFolioTableColWidthSlot);
    QObject::connect(T_Portfolio_Table, &table_portfolios_custom::clicked, T_Portfolio_Model, &Table_Portfolios_Model::onItemChanged);
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
    /***********Init portfolio table Window**************************/



    /***********Init Order Book Window**************************/
    QPixmap pixmapdock_trade_close(":/dock_close.png");

    dock_win_trade =  new QDockWidget(tr("Order Book"), this);

    connect(dock_win_trade, SIGNAL(visibilityChanged(bool)), this, SLOT(OnOrderBookDockWidgetVisiblityChanged(bool)));
    subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_trade);
    dock_win_trade->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock_win_trade->setStyleSheet(dock_style);

    //create a titlebar
    QWidget *trade_titlebar=new QWidget;
    trade_titlebar->setStyleSheet(DockTitleBar_Style);
    QHBoxLayout *trade_title_layout=new QHBoxLayout(trade_titlebar);
    trade_title_layout->setSpacing(10);
    trade_title_layout->setContentsMargins(17,8,10,6);
    QLabel *trade_label=new QLabel("Order Book");
    QFont font_trade_label=trade_label->font();
    font_trade_label.setFamily("Work Sans");
    trade_label->setFont(font_trade_label);
    trade_label->setStyleSheet("color: #495057;font-size: 16px;font-style: normal;font-weight: 700;line-height: normal;");
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
    dock_win_trade->setTitleBarWidget(trade_titlebar);




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
    trade_table->setStyleSheet(tableview_SS);
    trade_table->horizontalHeader()->setFixedHeight(32);
    trade_table->horizontalHeader()->setFont(headerfont);
    trade_table->setShowGrid(false);
    trade_table->setAlternatingRowColors(true);

    lay_Trade_Window->addWidget(trade_table, 1, 0, 1, 3);

    trade_model = new Trade_Table_Model();
    Table_OrderBook_Delegate* trade_delegate=new Table_OrderBook_Delegate;
    trade_table->setModel(trade_model);
    trade_table->setItemDelegate(trade_delegate);
    trade_table->horizontalHeader()->setStretchLastSection(true);
    trade_table->verticalHeader()->setVisible(false);
    // trade_table->setStyleSheet("QHeaderView { background-color: #111111;} QHeaderView::section { background-color:#555555;color:#eeeeee;font-weight: 400; }");
    trade_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    trade_table->setSelectionMode(QAbstractItemView::SingleSelection);
    /*trade_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                            "selection-color: #4D4D4D;"
                                            "color:#3D3D3D;"
                                            "} "
                                            "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    trade_table->show();
    /***********Init Order Book Window**************************/

    /************Positions Window********************************/
    QPixmap pixmapdock_position_close(":/dock_close.png");

    dock_win_net_pos =  new QDockWidget(tr("Positions"), this);
    connect(dock_win_net_pos, SIGNAL(visibilityChanged(bool)), this, SLOT(OnPositionsDockWidgetVisiblityChanged(bool)));
    dock_win_net_pos->setAllowedAreas(Qt::AllDockWidgetAreas );
    subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_net_pos);
    dock_win_net_pos->setStyleSheet(dock_style);

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
    dock_win_net_pos->setTitleBarWidget(position_titlebar);


    net_pos_table = new QTableView(dock_win_net_pos);
    net_pos_table->setStyleSheet(scroll_bar_SS);
    net_pos_table->horizontalHeader()->setFixedHeight(32);
    net_pos_table->horizontalHeader()->setFont(headerfont);
    net_pos_table->setStyleSheet(tableview_SS);
    net_pos_table->setShowGrid(false);
    net_pos_table->setAlternatingRowColors(true);
    net_pos_model = new Net_Position_Table_Model();
    net_pos_table->setModel(net_pos_model);
    net_pos_table->horizontalHeader()->setStretchLastSection(true);
    net_pos_table->verticalHeader()->setVisible(false);
    /*  net_pos_table->setStyleSheet("QTableView {selection-background-color: #EFB37F;"
                                        "selection-color: #4D4D4D;"
                                        "color:#3D3D3D;"
                                        "} "
                                        "QHeaderView { background-color: #C0AAE5;color:#3D3D3D;} QHeaderView::section { background-color:#C0AAE5;color:#3D3D3D;font-weight: 400; }");*/

    net_pos_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    net_pos_table->setSelectionMode(QAbstractItemView::SingleSelection);

    dock_win_net_pos->setWidget(net_pos_table);
    net_pos_table->show();
    /************Net Position Window********************************/


    /************Historical Positions Window********************************/
    QPixmap pixmapdock_hp_close(":/dock_close.png");

 dock_win_combined_tracker =  new QDockWidget(tr("Historical Positions"), this);
 connect(dock_win_combined_tracker, SIGNAL(visibilityChanged(bool)), this, SLOT(OnHPDockWidgetVisiblityChanged(bool)));
 dock_win_combined_tracker->setAllowedAreas(Qt::AllDockWidgetAreas );

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
    dock_win_combined_tracker->setTitleBarWidget(hp_titlebar);



    subWindow->addDockWidget(Qt::RightDockWidgetArea, dock_win_combined_tracker);

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



    QFile styleFile(":/style/Style_Home.qss");
    styleFile.open(QFile::ReadOnly);
    QString mainstyleSheet = QLatin1String(styleFile.readAll());
    this->setStyleSheet(mainstyleSheet);

    //logo at the top left
    QPixmap pixmaplogo(":/Am Logomark - Standard.png");
    pixmaplogo = pixmaplogo.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmaplogo);



    QPixmap pixmapright(":/right_arrow.png");
    ui->toggle_Button->setIcon(pixmapright);

    //stylesheet for summary window title labels
    const char stylesheet_userinfo[] = "color: #2F3339;" "text-align: center;""font-size: 12px;""font-style: normal;""font-weight: bold;""line-height: normal;";
    for (auto w : {ui->label_7, ui->label_9,ui->label_10,ui->label_8,ui->label_2,ui->label_20,ui->label_6,ui->label_16,ui->label_15,ui->label_14,ui->m2mlabel,ui->label_13}){
        w->setStyleSheet(stylesheet_userinfo);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
        w->setWordWrap(true);
    }

    ui->Algorithms_Close->setVisible(false);
    ui->OrderBook_Close->setVisible(false);
    ui->Positions_Close->setVisible(false);
    ui->HP_Close->setVisible(false);
    ui->Templates_Close->setVisible(false);

    ui->lineEditWatchSearch->setPlaceholderText("Search...");
    ui->lineEditWatchSearch->setStyleSheet("font-family: 'Work Sans';"
                                           "font-size: 12px;"
                                           "font-weight: 400;"
                                           );
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
    ui->HP_Close->setIcon(pixmapbuttonclose);
    ui->Templates_Close->setIcon(pixmapbuttonclose);

    const char stylesheet_params[] = "color: #000000 ;""font-size: 12px;""font-style: normal;""font-weight: bold;""line-height: normal;";
    for (auto w : {ui->Open_Label, ui->High_Label,ui->Low_Label,ui->Volume_Label}){
        w->setStyleSheet(stylesheet_params);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
    }

    const char stylesheet_params_nos[] = "color: #4f3c7f ;""font-family: Work Sans;" "font-size: 14px;" "font-style: normal;" "font-weight: bold;""line-height: normal;";
    for (auto w : {ui->Open_Num, ui->High_Num,ui->Low_Num,ui->Volume_Num}){
        w->setStyleSheet(stylesheet_params_nos);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
    }

    QFont fontbuy=ui->Buy_Watch_Button->font();
    QFont fontsell=ui->Sell_Watch_Button->font();
    fontbuy.setFamily("Work Sans");
    fontsell.setFamily("Work Sans");
    ui->Buy_Watch_Button->setFont(fontbuy);
    ui->Sell_Watch_Button->setFont(fontsell);

    ui->listWidgetWatch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Hides vertical scroll bar
    ui->listWidgetWatch->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Hides horizontal scroll bar



    // Apply the shadow effect to the widget
//    QSettings settings;
//    settings.setValue("DOCK_LOCATIONS",this->saveState(1));

//    this->restoreState(settings.value("DOCK_LOCATIONS").toByteArray(),1);

    new QShortcut(QKeySequence(Qt::Key_Delete), this,  SLOT(Delete_clicked_slot()));

    // db_conn =new mysql_conn(this,"main_db_conn");
    // connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));
   //loadContract();
    initWatchWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
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
        if(settings.contains("Watch_InstrumentNames")){
           QString watch=  settings.value("Watch_InstrumentNames").toString();
            if(watch=="")
               savedWatchItems.clear();
            else
                savedWatchItems = watch.split(";");
        }
        settings.endGroup();
    }
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
            T_Portfolio_Model->portfolio_data_list[index.row()]->edting.storeRelaxed(0); // maked the row flg not editing, so the bg data will be updated.
            switch (index.column()) {
            case PortfolioData_Idx::_Status:{
        if(valStr == "0"){
            QString Query = "UPDATE Portfolios SET Status='DisabledByUser' where PortfolioNumber="+PortfolioNumber;
            bool success = db_conn->updateDB_Table(Query);
            if(success){
                db_conn->logToDB(QString("Disabled portfolio ["+PortfolioNumber+"]"));
            }
        }
        else{
            QString Query = "UPDATE Portfolios SET Status='Active' where PortfolioNumber="+PortfolioNumber;
            bool success =  db_conn->updateDB_Table(Query);
            if(success){
                db_conn->logToDB(QString("Activated  portfolio ["+PortfolioNumber+"]"));
            }
        }

    }
    break;
        //     case PortfolioData_Idx::_SellPriceDifference:{
        //       double val = valStr.toDouble();
        //       val=val*devicer;
        //       QString Query = "UPDATE Portfolios SET SellPriceDifference="+QString::number(val,'f',decimal_precision)+" where PortfolioNumber="+PortfolioNumber;
        //       db_conn->updateDB_Table(Query);
        //     }
        //     break;
        // case PortfolioData_Idx::_BuyPriceDifference:{
        //       double val = valStr.toDouble();
        //       val=val*devicer;
        //       QString Query = "UPDATE Portfolios SET BuyPriceDifference="+QString::number(val,'f',decimal_precision)+" where PortfolioNumber="+PortfolioNumber;
        //       db_conn->updateDB_Table(Query);
        //     }
        //     break;
        // case PortfolioData_Idx::_SellTotalQuantity:{
        //       int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
        //       double val = valStr.toDouble();
        //       val=val*lotSize;
        //       QString Query = "UPDATE Portfolios SET SellTotalQuantity="+QString::number(val)+" where PortfolioNumber="+PortfolioNumber;
        //       db_conn->updateDB_Table(Query);
        //     }
        //     break;
        // case PortfolioData_Idx::_BuyTotalQuantity:{
        //     int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
        //     double val = valStr.toDouble();
        //     val=val*lotSize;
        //       QString Query = "UPDATE Portfolios SET BuyTotalQuantity="+QString::number(val)+" where PortfolioNumber="+PortfolioNumber;
        //       db_conn->updateDB_Table(Query);
        //     }
        //     break;

        // case PortfolioData_Idx::_OrderQuantity:{
        //       QString Query = "UPDATE Portfolios SET OrderQuantity="+valStr+" where PortfolioNumber="+PortfolioNumber;
        //       db_conn->updateDB_Table(Query);
        //       bool success = db_conn->updateDB_Table(Query);
        //       if(success){
        //           db_conn->logToDB(QString("OrderQuantity ["+valStr+"]"));
        //       }
        //     }
        //     break;
    case PortfolioData_Idx::_OrderQuantity:{
              int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
              double val = valStr.toDouble();
              val=val*lotSize;
              QString Query = "UPDATE Portfolios SET OrderQuantity="+QString::number(val)+" where PortfolioNumber="+PortfolioNumber;

              db_conn->updateDB_Table(Query);
              bool success = db_conn->updateDB_Table(Query);
            if(success){
                db_conn->logToDB(QString("OrderQuantity ["+valStr+"]"));
             }
          }
           break;
    case PortfolioData_Idx::_BuyTotalQuantity:{
             int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
             double val = valStr.toDouble();
             val=val*lotSize;
             if(val< userData.IDXOpenLimit ) {
                QString Query = "UPDATE Portfolios SET BuyTotalQuantity="+QString::number(val)+" where PortfolioNumber="+PortfolioNumber;
                db_conn->updateDB_Table(Query);
                bool success = db_conn->updateDB_Table(Query);
                if(success){
                  db_conn->logToDB(QString("BuyTotalQuantity ["+valStr+"]"));
                }
            }
    }
             break;

    case PortfolioData_Idx::_SellTotalQuantity:{
            int lotSize = T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize();
            double val = valStr.toDouble();
            val=val*lotSize;
            if(val<  userData.IDXOpenLimit) {
                QString Query = "UPDATE Portfolios SET SellTotalQuantity="+QString::number(val)+" where PortfolioNumber="+PortfolioNumber;
                db_conn->updateDB_Table(Query);
                bool success = db_conn->updateDB_Table(Query);
                if(success){
                  db_conn->logToDB(QString("SellTotalQuantity ["+valStr+"]"));
                }
            }
    }
            break;



    default:{
        QString SellPriceDifference = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->SellPriceDifference*devicer,'f',decimal_precision);
        QString BuyPriceDifference = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->BuyPriceDifference*devicer,'f',decimal_precision);
        QString SellTotalQuantity = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->SellTotalQuantity*T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize());
        QString BuyTotalQuantity = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->BuyTotalQuantity*T_Portfolio_Model->portfolio_data_list[index.row()]->GetLotSize());
        QString OrderQuantity = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->OrderQuantity);

        QString Query = "UPDATE Portfolios SET"
                        " SellPriceDifference="+SellPriceDifference+
                        ", BuyPriceDifference="+BuyPriceDifference+
                        ", SellTotalQuantity="+SellTotalQuantity+
                        ", BuyTotalQuantity="+BuyTotalQuantity+
                        ", OrderQuantity="+OrderQuantity+
                        " WHERE PortfolioNumber="+PortfolioNumber;

        bool success = db_conn->updateDB_Table(Query);
        if(success){
            db_conn->logToDB(QString("BuyPriceDifference["+BuyPriceDifference+"] " +
                                     "SellPriceDifference["+SellPriceDifference+"] " +
                                     "BuyTotalQuantity["+BuyTotalQuantity+"] " +
                                     "SellTotalQuantity["+SellTotalQuantity+"] " +
                                     "OrderQuantity["+OrderQuantity+"]"+
                                     "PortfolioNumber["+PortfolioNumber+"]"));
        }


    }
    break;
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

        QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:0.4;'>"
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

        htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:0.4;'>"
                      "<span>" + QTime::currentTime().toString("hh:mm:ss")
                      + "&nbsp;</span><span style='font-weight:400;color: black;'> file loaded </span></p>";


        emit display_log_text_signal(htmlContent);

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

        //load alog table only when it's not editing.
        //if(portfolio_table_editing.loadRelaxed()==0){
        loadDataAndUpdateTable(T_Table::PORTFOLIO);
//        //}
        loadDataAndUpdateTable(T_Table::TRADE);
        loadDataAndUpdateTable(T_Table::NET_POS);
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
    case T_Table::PORTFOLIO:
        db_conn->getPortfoliosTableData(T_Portfolio_Model,combined_tracker_model,averagePriceList,QString::number(userData.UserId));
        emit data_loded_signal(T_Table::PORTFOLIO);
        break;

    case T_Table::TRADE:{
//        OutputDebugStringA("Test1 \n");
        QHash<QString, PortFolioData_Less> PortFolioHash = T_Portfolio_Model->getPortFolioDataLess();
        db_conn->getTradeTableData(trade_model,QString::number(userData.UserId),PortFolioHash);
        emit data_loded_signal(T_Table::TRADE);
        break;
    }

    case T_Table::NET_POS:{
        db_conn->getNetPosTableData(net_pos_model,QString::number(userData.UserId));
        emit data_loded_signal(T_Table::NET_POS);
        break;
    }
    case T_Table::SUMMARY: {
        db_conn->getSummaryTableData();  // Correctly get the count
        emit data_loded_signal(T_Table::SUMMARY);
        break;
    }

    default:
        break;
    }
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
    qDebug()<<"Backend  Comm Data: "<<msg;

    if((msgType == SocketDataType::BACKEND_COMM_SOCKET_WARNING)||msgType == SocketDataType::BACKEND_COMM_SOCKET_ERROR){
        qDebug()<<"Backend Data: Backend Socket Error"<<msg;
    }
    //ui->toolButton_BackendServer->setStyleSheet("background-color: rgb(255, 32, 36);border-radius:6px;color:#000;");
    else if(msgType == SocketDataType::BACKEND_COMM_SOCKET_CONNECTED){
        qDebug()<<"Backend Data: Backend Socket Connected"<<msg;
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
            T_Portfolio_Model->portfolio_data_list[index.row()]->StatusVal = QString::number(portfolio_status::Active);
            T_Portfolio_Model->refreshTable();
            QString Query = "UPDATE Portfolios SET Status='Active' where PortfolioNumber="+PortfolioNumber;
            bool success = db_conn->updateDB_Table(Query);
            if(success){

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
            bool success = db_conn->updateDB_Table(Query);
            if(success){

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
//        ui->label_username->setText(userData.UserName);
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
    ui->textEdit->append(str);
}

void MainWindow::on_OrderBook_Button_clicked()
{
    //8
    dock_win_trade->show();
    ui->OrderBook_Widget->setStyleSheet(stylesheetvis);
    ui->OrderBook_Close->setVisible(true);
}


void MainWindow::on_OrderBook_Close_clicked()
{
    //8
    dock_win_trade->close();
    ui->OrderBook_Widget->setStyleSheet("");
    ui->OrderBook_Close->setVisible(false);
}


void MainWindow::on_Positions_Button_clicked()
{
    //9
    dock_win_net_pos->show();
    ui->Positions_Widget->setStyleSheet(stylesheetvis);
    ui->Positions_Close->setVisible(true);
}


void MainWindow::on_Positions_Close_clicked()
{
    //9
    dock_win_net_pos->close();
    ui->Positions_Widget->setStyleSheet("");
    ui->Positions_Close->setVisible(false);
}


void MainWindow::on_HP_Button_clicked()
{
    //11
    dock_win_combined_tracker->show();
    ui->HP_Widget->setStyleSheet(stylesheetvis);
    ui->HP_Close->setVisible(true);
}


void MainWindow::on_HP_Close_clicked()
{
    //11
    dock_win_combined_tracker->close();
    ui->HP_Widget->setStyleSheet("");
    ui->HP_Close->setVisible(false);
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

void MainWindow::on_toggle_Button_clicked()
{
        bool currentVisibility = ui->widget->isVisible();
        if(currentVisibility){
            ui->widget->setVisible(false);
            QPixmap pixmapright(":/left_arrow.png");
            ui->toggle_Button->setIcon(pixmapright);}
        else{
            ui->widget->setVisible(true);
            QPixmap pixmapright(":/right_arrow.png");
            ui->toggle_Button->setIcon(pixmapright);
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

void MainWindow::OnHPDockWidgetVisiblityChanged(bool p_Visible)
{
    if(p_Visible)
        ui->HP_Widget->setStyleSheet(stylesheetvis);
    else
        ui->HP_Widget->setStyleSheet("");
    ui->HP_Close->setVisible(p_Visible);
}


void MainWindow::on_Algorithms_Button_clicked()
{
    T_Portfolio_DockWin->show();
    //7
    ui->Algorithms_Widget->setStyleSheet(stylesheetvis);
    ui->Algorithms_Close->setVisible(true);
}


void MainWindow::on_Algorithms_Close_clicked()
{
    T_Portfolio_DockWin->close();
    ui->Algorithms_Widget->setStyleSheet("");
    ui->Algorithms_Close->setVisible(false);
}

void MainWindow::on_ConvertAlgo_button_clicked(){

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
        OrderDetail_Popup *orderWin = new OrderDetail_Popup();
        orderWin->show();
       // QString Expiry = T_Portfolio_Model->portfolio_data_list[index.row()]->Expiry;
        QString PortfolioType =T_Portfolio_Model->portfolio_data_list[index.row()]->PortfolioType;
        QString PortfolioNumber = QString::number(T_Portfolio_Model->portfolio_data_list[index.row()]->PortfolioNumber);
        orderWin->getData(QString::number(userData.UserId), PortfolioNumber,PortfolioType);
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
    htmlLogsContent.prepend(str);
    ui->textEdit->setText(htmlLogsContent);
}

/*********************Watch data section*************************/

QDataStream &operator<<(QDataStream &out, const Indices_Data_Struct &data) {
    out << data.indexName << data.indexValue << data.change << data.percentagechange
        << data.netChangeIndicator << data.openingIdx << data.closingIdx
        << data.highIndexValue << data.lowIndexValue << data.marketCapitialisation
        << data.display_widget_idx;
    return out;
}

QDataStream &operator>>(QDataStream &in, Indices_Data_Struct &data) {
    in >> data.indexName >> data.indexValue >> data.change >> data.percentagechange
        >> data.netChangeIndicator >> data.openingIdx >> data.closingIdx
        >> data.highIndexValue >> data.lowIndexValue >> data.marketCapitialisation
        >> data.display_widget_idx;
    return in;
}
void MainWindow::initWatchWindow(){
    loadIndicesDataListFromFile(indicesDataList);
    showSaveWatchOnListView();

}


void MainWindow::saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = appDataPath+"/Data/watch_cache.bin";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing.");
        return;
    }
    QDataStream out(&file);
    out << indicesDataList;
    file.close();
}

void MainWindow::loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = appDataPath+"/Data/watch_cache.bin";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for reading.");
        return;
    }
    QDataStream in(&file);
    in >> indicesDataList;
    file.close();
}


void MainWindow::indicesDataRecv_Slot(Indices_Data_Struct data){
    bool newIndicesData = false;
    indicesDataMutex.lock();
    if(!indicesDataList.contains(data.indexName)){
        indicesDataList.insert(data.indexName,data);
        newIndicesData = true;
    }
    //check any of the value changed
    else if((indicesDataList[data.indexName].indexValue!=data.indexValue) || (indicesDataList[data.indexName].change!=data.change) || (indicesDataList[data.indexName].percentagechange!=data.percentagechange)){
        indicesDataList[data.indexName]=data;
        newIndicesData = true;
    }

    indicesDataMutex.unlock();
   // qDebug()<<"indicesDataRecv_Slot: " <<data.indexName;

    if(newIndicesData==true&&ui->lineEditWatchSearch->text()=="")
        showSaveWatchOnListView();

    if(newIndicesData==true){

        QListWidgetItem *item = ui->listWidgetWatch->currentItem();
        if (item) {
            watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(item));
            if (widget) {
                updateSelecteWatch_UI( widget->data);
            }
        }

        saveIndicesDataListToFile(indicesDataList);

    }

}
void MainWindow::on_lineEditWatchSearch_textChanged(const QString &text)
{
    if(text==""){
        showSaveWatchOnListView();
        return;
    }

    indicesDataMutex.lock();

    QHash<QString, Indices_Data_Struct> indicesDataListTmp = indicesDataList;
    indicesDataListTmp.detach();
    indicesDataMutex.unlock();


    /*QStringList nameList;
    nameList.append("Nifty50");
    nameList.append("BankNifty");
    nameList.append("FinNifty");
    nameList.append("SmallCap");
    nameList.append("ABC");
    nameList.append("DDD");
    nameList.append("ee");
    nameList.append("dd");
    nameList.append("ABC");
    nameList.append("de");
    nameList.append("ddd");
    nameList.append("wsd");
    nameList.append("dfe");
    nameList.append("fg");
    nameList.append("gf");
    nameList.append("rt");
    nameList.append("fg");*/


    QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
    QStringList filteredList;

    for (auto it = indicesDataListTmp.constBegin(); it != indicesDataListTmp.constEnd(); ++it) {
        const QString &key = it.key();
        if (regex.match(key).hasMatch()) {
            filteredList.append(key);
        }
       // const Indices_Data_Struct &value = it.value();
    }

    /*for(const QString &name : nameList) {
        if (regex.match(name).hasMatch()) {
            filteredList.append(name);
        }
    }*/

    ui->listWidgetWatch->clear();
    int selecteIDx = -1;
    for(int i=0;i<filteredList.size();i++){
        auto item = new QListWidgetItem();
        auto widget1 = new watch_Data_List_Item(this);
        Indices_Data_Struct data = indicesDataListTmp[filteredList[i]];        
        widget1->setData(data);
        item->setSizeHint(widget1->sizeHint());

        ui->listWidgetWatch->addItem(item);
        ui->listWidgetWatch->setItemWidget(item, widget1);
        if(data.indexName==watchItemSelectedindexName)
            selecteIDx = ui->listWidgetWatch->count()-1;
    }
    if(selecteIDx>-1){
        ui->listWidgetWatch->setCurrentRow(selecteIDx);
    }


}
void MainWindow::showSaveWatchOnListView(){
    ui->listWidgetWatch->clear();
    if(savedWatchItems.size()==0){
        return;
    }
    indicesDataMutex.lock();
    QHash<QString, Indices_Data_Struct> indicesDataListTmp = indicesDataList;
    indicesDataListTmp.detach();
    indicesDataMutex.unlock();
    int selecteIDx = -1;
    for(int i=0;i<savedWatchItems.length();i++){
        if(indicesDataListTmp.contains(savedWatchItems[i])){
            auto item = new QListWidgetItem();
            auto widget1 = new watch_Data_List_Item(this);
            Indices_Data_Struct data = indicesDataListTmp[savedWatchItems[i]];
            widget1->setData(data);
            item->setSizeHint(widget1->sizeHint());
            ui->listWidgetWatch->addItem(item);
            ui->listWidgetWatch->setItemWidget(item, widget1);
            if(data.indexName==watchItemSelectedindexName)
                selecteIDx = ui->listWidgetWatch->count()-1;
        }
    }
    if(selecteIDx>-1){
        ui->listWidgetWatch->setCurrentRow(selecteIDx);
    }
}

void MainWindow::on_listWidgetWatch_itemClicked(QListWidgetItem *item)
{
    if (!item) {
        return; // Handle no item clicked scenario
    }

    watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(item));
    if (!widget) {
        return; // Handle case where no widget is associated with the item
    }

    watchItemSelectedindexName = widget->data.indexName;

    updateSelecteWatch_UI(widget->data);

}

void MainWindow::updateSelecteWatch_UI( Indices_Data_Struct data){
    bool redArrow = true;
    //if(data.netChangeIndicator=="+")
    // redArrow = false;
    if(data.change.toDouble()>=0)
        redArrow = false;

    if(redArrow){
        QPixmap pixmap(":/arrow_red.png");
        pixmap = pixmap.scaled(ui->watch_indicator->width(),ui->watch_indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->watch_indicator->setPixmap(pixmap);
        ui->watch_indicator->setMask(pixmap.mask());
        ui->watch_indicator->setStyleSheet("font-family: 'Work Sans';"
                                           "font-size: 10px;"
                                           "font-weight: 500;"
                                           "color:#CC3437"
                                           );
    }
    else{
        QPixmap pixmap(":/arrow_green.png");
        pixmap = pixmap.scaled(ui->watch_indicator->width(),ui->watch_indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->watch_indicator->setPixmap(pixmap);
        ui->watch_indicator->setMask(pixmap.mask());

        ui->watch_val1->setStyleSheet("font-family: 'Work Sans';"
                                      "font-size: 10px;"
                                      "font-weight: 500;"
                                      "color:#008000"
                                      );
    }

    QString percent = (data.change+"("+data.percentagechange+"%)");
    ui->Index_Name->setText(data.indexName);
    ui->watch_val1->setText(data.indexValue);
    ui->watch_val2->setText(percent);
    ui->Open_Num->setText(data.openingIdx);
    ui->High_Num->setText(data.highIndexValue);
    ui->Low_Num->setText(data.lowIndexValue);
    ui->Volume_Num->setText(data.marketCapitialisation);



}

void MainWindow::on_listWidgetWatch_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItem *selectedItem = ui->listWidgetWatch->currentItem();
    if (selectedItem) {
        watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(selectedItem));

        //if selected item already in saved item list remove from else or add it
        if(savedWatchItems.contains(widget->data.indexName)){
            removeFromSavedWatchItems(widget->data);
        }
        else{
            addToSavedWatchItems(widget->data);
        }
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Please select an item from Watch.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

}

void MainWindow::addToSavedWatchItems(Indices_Data_Struct data)
{
        if(!savedWatchItems.contains(data.indexName)){
            savedWatchItems.append(data.indexName);
            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
            settings.beginGroup("GeneralSettings");
            if(savedWatchItems.length()==1)
                settings.setValue("Watch_InstrumentNames",savedWatchItems[0]);
            else
                settings.setValue("Watch_InstrumentNames",savedWatchItems.join(";"));
            settings.endGroup();

            if(ui->lineEditWatchSearch->text()=="")
                showSaveWatchOnListView();

        }
        else{
            QMessageBox msgBox;
            msgBox.setText("IndexName already added.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
}

void MainWindow::removeFromSavedWatchItems( Indices_Data_Struct data)
{
        if(savedWatchItems.contains(data.indexName)){
            savedWatchItems.removeOne(data.indexName);
            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
            settings.beginGroup("GeneralSettings");
            if(savedWatchItems.length()==1)
                settings.setValue("Watch_InstrumentNames",savedWatchItems[0]);
            else
                settings.setValue("Watch_InstrumentNames",savedWatchItems.join(";"));
            settings.endGroup();
            if(ui->lineEditWatchSearch->text()=="")
                showSaveWatchOnListView();


        }
        else{
            QMessageBox msgBox;
            msgBox.setText("IndexName not exist in Saved List.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }

}


/****************************************************************/











