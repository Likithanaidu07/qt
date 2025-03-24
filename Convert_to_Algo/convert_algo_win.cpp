#include "convert_algo_win.h"
#include "ContractDetail.h"
#include "qfuture.h"
#include "mainwindow.h"

extern MainWindow *MainWindowObj;

ConvertAlgo_Win::ConvertAlgo_Win(QWidget *parent, const QStringList &list) :
    QDialog(parent), ExFilterPF(list),
    ui(new Ui::ConvertAlgo_Win)
{
    ui->setupUi(this);
    MainWindowObj = (MainWindow*) parent;
    //setWindowModality(Qt::ApplicationModal);


  //  ui->tableWidget->setFocusPolicy(Qt::NoFocus);

    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Start_strike_Btfy);
    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Start_strike_BtfyBid);
    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Start_strike_f2f);
    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Fut_ConvRev);
    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Start_strike_BoxBid);
    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Start_strike_Bs1221_1331);
    setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Fut_CRJellyBid);


    setTabOrder(ui->lineEdit_Start_strike_Btfy,ui->lineEdit_EndStrike_Btfy);
    setTabOrder(ui->lineEdit_EndStrike_Btfy,ui->lineEdit_StrikeDifference_Btfy);
    setTabOrder(ui->lineEdit_StrikeDifference_Btfy,ui->pushButtonAdd);

    setTabOrder(ui->lineEdit_Start_strike_BtfyBid,ui->lineEdit_EndStrike_BtfyBid);
    setTabOrder(ui->lineEdit_EndStrike_BtfyBid,ui->lineEdit_StrikeDifference_BtfyBid);
    setTabOrder(ui->lineEdit_StrikeDifference_BtfyBid,ui->pushButtonAdd);

    setTabOrder(ui->lineEdit_Start_strike_f2f,ui->lineEdit_EndStrike_f2f);
    setTabOrder(ui->lineEdit_EndStrike_f2f,ui->pushButtonAdd);

    setTabOrder(ui->lineEdit_Fut_ConvRev,ui->lineEdit_Start_strike_ConvRev);
    setTabOrder(ui->lineEdit_Start_strike_ConvRev,ui->lineEdit_EndStrike_ConvRev);
    setTabOrder(ui->lineEdit_EndStrike_ConvRev,ui->pushButtonAdd);

    setTabOrder(ui->lineEdit_Fut_CRJellyBid,ui->lineEdit_Start_strike_CRJellyBid);
    setTabOrder(ui->lineEdit_Start_strike_CRJellyBid,ui->lineEdit_EndStrike_CRJellyBid);
    setTabOrder(ui->lineEdit_EndStrike_CRJellyBid,ui->pushButtonAdd);

    setTabOrder(ui->lineEdit_Start_strike_BoxBid,ui->lineEdit_EndStrike_BoxBid);
    setTabOrder(ui->lineEdit_EndStrike_BoxBid,ui->lineEdit_StrikeDifference_BoxBid);
    setTabOrder(ui->lineEdit_StrikeDifference_BoxBid,ui->pushButtonAdd);


    setTabOrder(ui->lineEdit_Start_strike_Bs1221_1331,ui->lineEdit_EndStrike_Bs1221_1331);
    setTabOrder(ui->lineEdit_EndStrike_Bs1221_1331,ui->lineEdit_StrikeDifference_Bs1221_1331);
    setTabOrder(ui->lineEdit_StrikeDifference_Bs1221_1331,ui->pushButtonAdd);

    setTabOrder(ui->lineEdit_Fut_CRJellyBid,ui->lineEdit_Start_strike_CRJellyBid);
    setTabOrder(ui->lineEdit_Start_strike_CRJellyBid,ui->lineEdit_EndStrike_CRJellyBid);
    setTabOrder(ui->lineEdit_EndStrike_CRJellyBid,ui->pushButtonAdd);


    setTabOrder(ui->pushButtonAdd, ui->pushButtonSelectAll);
    setTabOrder(ui->pushButtonSelectAll, ui->pushButton_Reset);
    setTabOrder(ui->pushButton_Reset, ui->pushButtonDelete);
    setTabOrder(ui->pushButtonDelete, ui->tableWidget);
    setTabOrder(ui->tableWidget, ui->pushButtonUpload);
    setTabOrder(ui->pushButtonUpload, ui->pushButton_Cancel);
    setTabOrder(ui->pushButton_Cancel, ui->comboBox_AlgoType);
    ui->comboBox_AlgoType->setStyleSheet("QComboBox { text-align: center; }");

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);

    QPixmap pixmapclose(":/dock_close.png");
    ui->Close->setIcon(pixmapclose);

    //const char LineEdit_SS[]="border-radius: 6px;""border: 0.5px solid #343A40;""background: #FFF;""color: #6C757D;""font-size: 12px;""font-style: normal;""font-weight: 400;""line-height: normal;" "padding: 4px 10px 4px 10px;";
    const char LineEdit_SS[] =
        "QLineEdit {"
        "    border-radius: 6px;"
        "    border: 0.5px solid #343A40;"
        "    background: #FFF;"
        "    color: #6C757D;"
        "    font-size: 12px;"
        "    font-style: normal;"
        "    font-weight: 400;"
        "    line-height: normal;"
        "    padding: 4px 10px 4px 10px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #EF7153;"
        "}";



    for(auto w: {ui->lineEdit_StrikeDifference_Btfy,
        ui->lineEdit_StrikeDifference_BtfyBid,
        ui->lineEdit_Start_strike_Btfy,
        ui->lineEdit_Start_strike_BtfyBid,
        ui->lineEdit_Start_strike_ConvRev,
        ui->lineEdit_Start_strike_f2f,
        ui->lineEdit_EndStrike_Btfy,
        ui->lineEdit_EndStrike_BtfyBid,
        ui->lineEdit_EndStrike_ConvRev,
        ui->lineEdit_EndStrike_f2f,
        ui->lineEdit_Fut_ConvRev,
        ui->lineEdit_Start_strike_BoxBid,
        ui->lineEdit_EndStrike_BoxBid,
        ui->lineEdit_StrikeDifference_BoxBid,
        ui->lineEdit_EndStrike_Bs1221_1331,
        ui->lineEdit_Start_strike_Bs1221_1331,
        ui->lineEdit_StrikeDifference_Bs1221_1331,
        ui->lineEdit_EndStrike_CRJellyBid,
        ui->lineEdit_Start_strike_CRJellyBid,
        ui->lineEdit_Fut_CRJellyBid
    }){
        w->setStyleSheet(LineEdit_SS);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
    }


    //const char pushButton_SS[]="border-radius: 8px;""border: 1px solid #485F6B;""background: #485F6B;""color: #FFF;""    text-align: center;""    font-size: 14px;""    font-style: normal;""    font-weight: 500;""    line-height: normal;";
    const char pushButton_SS[] =
        "QPushButton {"
        "    border-radius: 8px;"
        "    border: 1px solid #485F6B;"
        "    background: #485F6B;"
        "    color: #FFF;"
        "    text-align: center;"
        "    font-size: 14px;"
        "    font-style: normal;"
        "    font-weight: 500;"
        "    line-height: normal;"
        "}"
        "QPushButton:focus {"
        "    border: 2px solid #EF7153;"  // Customize focus border
        "}";


    ui->pushButtonAdd->setStyleSheet(pushButton_SS);
    QFont font=ui->pushButtonAdd->font();
    font.setFamily("Work Sans");
    ui->pushButtonAdd->setFont(font);

    const char label_SS[]= "color: #343A40; ""text-align: center; ""font-size: 14px; ""font-style: normal; ""font-weight: 600; ""line-height: normal;";
    ui->label->setStyleSheet(label_SS);
    QFont font_label=ui->label->font();
    font_label.setFamily("Work Sans");
    ui->label->setFont(font_label);

//    const char toolButton_SS[]="border-radius: 4px; "
//                               "border: 0.5px solid #465460; "
//                               "background: #FFF; "
//                               "color: #465460; "
//                               "text-align: center; "
//                               "font-size: 10px; "
//                               "font-style: normal; "
//                               "font-weight: 500; "
//                               "line-height: normal;";
    const char toolButton_SS[] =
        "QPushButton {"
        "    border-radius: 4px;"
        "    border: 0.5px solid #465460;"
        "    background: #FFF;"
        "    color: #465460;"
        "    text-align: center;"
        "    font-size: 10px;"
        "    font-style: normal;"
        "    font-weight: 500;"
        "    line-height: normal;"
        "}"
        "QPushButton:focus {"
        "    border: 2px solid #EF7153;"  // Custom border on focus
        "}";

    ui->pushButtonAdd->setStyleSheet(pushButton_SS);

    for(auto w:{ui->pushButtonDelete,ui->pushButtonSelectAll,ui->pushButton_Reset}){
        w->setStyleSheet(toolButton_SS);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
    }


    const char pushButton_upload_cancel_ss[] =
        "QPushButton {"
        "    border-radius: 7px;"
        "    background: #1585C0;"
        "    color: #FFF;"
        "    font-family: Work Sans;"
        "    font-size: 14px;"
        "    font-style: normal;"
        "    font-weight: 600;"
        "    line-height: normal;"
        "}"
        "QPushButton:focus {"
        "    border: 2px solid #EF7153;"  // Custom border on focus
        "}";
    for(auto w:{ui->pushButtonUpload,ui->pushButton_Cancel}){
        w->setStyleSheet(pushButton_upload_cancel_ss);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
    }



    //initialize shared variable class
    sharedData = &AddAlgoSharedVar::getInstance();
    sharedData->ROW_LIMIT =  50;
    dataSorted.storeRelaxed(0);
   // db_conn =new mysql_conn(MainWindowObj,"add_algo_db_conn");
    db_conn =new mysql_conn(0,"add_algo_db_conn");

    connect(this,SIGNAL(update_ui_signal(int)),this,SLOT(update_ui_slot(int)));
    connect(this,SIGNAL(display_log_text_signal(QString)),MainWindowObj,SLOT(slotAddLogForAddAlgoRecord(QString)));

    ui->tableWidget->setNextFocusButton(ui->pushButtonUpload);

    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);

    /*******Class to generate F2F algos************/
   algoF2F = new add_algo_f2f();
   algoF2F->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_f2f,ui->lineEdit_EndStrike_f2f,ui->pushButtonAdd);
   connect(algoF2F,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    // /*******Class to generate F2F algos************/

    /*******Class to generate BtFly algos************/
    algoBtFly= new add_algo_btfly();
    algoBtFly->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_Btfy,ui->lineEdit_EndStrike_Btfy,ui->lineEdit_StrikeDifference_Btfy,ui->pushButtonAdd);
    connect(algoBtFly,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    /*******Class to generate BtFly algos************/

    /*******Class to generate con_rev algos************/
    algoConRev= new add_algo_con_rev();
    algoConRev->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_ConvRev,ui->lineEdit_EndStrike_ConvRev,ui->lineEdit_Fut_ConvRev,ui->pushButtonAdd);
    connect(algoConRev,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    /*******Class to generate con_rev algos************/

    /*******Class to generate BtFly-bid algos************/
    algoBtFlyBid= new add_algo_btfly_bid();
    algoBtFlyBid->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_BtfyBid,ui->lineEdit_EndStrike_BtfyBid,ui->lineEdit_StrikeDifference_BtfyBid,ui->pushButtonAdd);
    connect(algoBtFlyBid,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    /*******Class to generate BtFly-bid algos************/



    /*******Class to generate algoCRJellyBid algos************/
    algoCRJellyBid= new add_algo_cr_jelly_bid();
    algoCRJellyBid->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_CRJellyBid,ui->lineEdit_EndStrike_CRJellyBid,ui->lineEdit_Fut_CRJellyBid,ui->pushButtonAdd);
    connect(algoCRJellyBid,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    /*******Class to generate algoCRJellyBid algos************/

    /*******Class to generate BtFly-bid algos************/
    algoBoxBid= new convert_to_algo_box_bid();
    algoBoxBid->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_BoxBid,ui->lineEdit_EndStrike_BoxBid,ui->lineEdit_StrikeDifference_BoxBid,ui->pushButtonAdd);
    connect(algoBoxBid,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    /*******Class to generate BtFly-bid algos************/



    /*******Class to generate bs1221 algos************/
    algoBS_1221_1331= new convert_to_algo_bs1221_1331();
    algoBS_1221_1331->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_Bs1221_1331,ui->lineEdit_EndStrike_Bs1221_1331,ui->lineEdit_StrikeDifference_Bs1221_1331,ui->pushButtonAdd);
    connect(algoBS_1221_1331,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));
    /*******Class to generate BtFly-bid algos************/


    /*******Class to generate open_2L algos************/
    algoOpen2L= new convert_to_algo_open_2L();
    algoOpen2L->copyUIElement(this,ui->tableWidget,ui->lineEdit_Start_strike_Open_2L,ui->lineEdit_EndStrike_Open_2L,ui->lineEdit_StrikeDifference_Open_2L,ui->lineEdit_2LRatio_Open_2L,ui->pushButtonAdd);
    connect(algoOpen2L,SIGNAL(progressSignal(bool,QString)),this,SLOT(progressSlot(bool,QString)));

    /*******Class to generate BtFly-bid algos************/



    QDoubleValidator *val = new QDoubleValidator();
    val->setLocale(QLocale::C);
    val->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_StrikeDifference_Btfy->setValidator(val);
    ui->lineEdit_StrikeDifference_BtfyBid->setValidator(val);
    setCustomRatioValidator(ui->lineEdit_2LRatio_Open_2L,4);


    sharedData->unique_id = 0; // this used to identyfy table row and algo_data to upload

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
  //  QString market_type = "fo";
    QString bidTypeFromSettings{};
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
       // if (settings.contains("market_type"))
          //  market_type = settings.value("market_type").toString();
        if (settings.contains("enable_bidType"))
            bidTypeFromSettings = settings.value("enable_bidType").toString();
        settings.endGroup();
    }
    //currently hardcoded for FO
    sharedData->devicer = FO_DEVICER;
    sharedData->decimal_precision = FO_DECIMAL_PRECISION;
    /*if(market_type=="fo"){
        sharedData->devicer = FO_DEVICER;
        sharedData->decimal_precision = FO_DECIMAL_PRECISION;
    }
    else{
        sharedData->devicer = CDS_DEVICER;
        sharedData->decimal_precision = CDS_DECIMAL_PRECISION;
    }*/

   //  ui->comboBox_AlgoType->setItemData(1, QVariant(0), Qt::UserRole-1);
    ui->comboBox_AlgoType->clear();
    ui->comboBox_AlgoType->setEditable(true);  // Make the combo box editable to show the placeholder
    ui->comboBox_AlgoType->lineEdit()->setPlaceholderText("Build Algo");  // Set the placeholder text
    ui->comboBox_AlgoType->setEditable(false);  // Set it back to non-editable to function as a normal combo box

    QStringList comboxItems;
    comboxItems.append("BFLY-BID");
    comboxItems.append("CR-BID");
    comboxItems.append("CRJELLY-BID");
    comboxItems.append("BX-BID");
    comboxItems.append("F2F-BID");
    comboxItems.append("BS12-BID");
    comboxItems.append("BS13-BID");
    comboxItems.append("OPEN-2L");

    for (const QString& item : comboxItems) {
        if (!ExFilterPF.contains(item)) {  // Add only if it is NOT in ExFilterPF
            ui->comboBox_AlgoType->addItem(item);
        }
    }




//    // Add the items to the dropdown
//        ui->comboBox_AlgoType->addItem(BFLY_BID_TYPE);
////        ui->comboBox_AlgoType->clear();
////        ui->comboBox_AlgoType->addItem(BFLY_BID_TYPE);

//      //  ui->comboBox_AlgoType->addItem("BFLY");
//        ui->comboBox_AlgoType->addItem("CR-BID");
////        ui->comboBox_AlgoType->addItem("BOX");
////        ui->comboBox_AlgoType->addItem("Open-BFLY");
////        ui->comboBox_AlgoType->addItem("Open-BOX");

//        ui->comboBox_AlgoType->addItem("CRJELLY-BID");
//        ui->comboBox_AlgoType->addItem("BX-BID");
//        ui->comboBox_AlgoType->addItem("F2F-BID");
      //  ui->comboBox_AlgoType->addItem("BS12-BID");
        ui->comboBox_AlgoType->setStyleSheet(
            "QComboBox {"
            "    font-weight: bold;"
            "    text-align: center;"
            "}"
            "QComboBox::item {"
            "    font-weight: bold;"
            "    text-align: center;"
            "}"
            "QComboBox::focus  {"
            "    border: 2px solid #EF7153;"
            "}"
//            "QComboBox::drop-down {"
//            "    subcontrol-origin: padding;"
//            "    subcontrol-position: top right;"
//            "    width: 15px;"
//            "    border-left-width: 1px;"
//            "    border-left-style: solid;"
//            "    border-top-right-radius: 3px;"
//            "    border-bottom-right-radius: 2px;"
//            "}"
            );

        // not implemented yet so disabled
        //ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_F2F, QVariant(0), Qt::UserRole-1);
        //ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_CON_REV, QVariant(0), Qt::UserRole-1);
       // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_Open_BFLY, QVariant(0), Qt::UserRole-1);
       // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_BOX, QVariant(0), Qt::UserRole-1);
      //  ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_Open_BOX, QVariant(0), Qt::UserRole-1);
        // not implemented yet so disabled



    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    enable_disable_UIElement(false);
    resetTableWidget();
    ui->comboBox_AlgoType->setFocus();

  //  ui->tableWidget->installEventFilter(this);

//    ui->tableWidget->setStyleSheet(
//        "QTableWidget::item:selected:focus {"
//        "    border: 5px solid #349800;"    // Border around the selected row
//        "    background-color: #e8f4fc;"    // Light background color for the selected row
//        "}"
//        "QTableWidget::item:selected {"
//        "    background-color: #d000f5;"    // Background color for selected row when table is not in focus
//        "    border: none;"                 // Remove border when table is not in focus
//        "}"
//    );



//    ui->tableWidget->setStyleSheet(
//        " QTableWidget:focus {"
//        "    border: 2px solid #EF7153;"
//        "    border-radius: 4px;"
//        "}"
//    );

    const char TableWidget_SS[] =
        "QTableWidget::item {"
        "    background-color: #FFF;"
        "    border: none;"
        "    color: #495057;"
        "    font-family: Work Sans;"
        "    font-size: 8px;"
        "    font-style: normal;"
        "    font-weight: 600;"
        "    line-height: normal;"
        "}"
        "QTableWidget::item:alternate {"
        "    border: none;"
        "    background-color: #F5FAFD;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #42A5F5;"
        "    color: white;"
        "}"
        "QTableWidget::focus {"
        "    border: 2px solid #EF7153;"  // Custom border on focus
        "}";

    ui->tableWidget->setStyleSheet(TableWidget_SS);


}


void ConvertAlgo_Win::setCustomRatioValidator(QLineEdit *lineEdit, int maxDigits) {
    // Create regex with dynamic max digits for both sides
    QString pattern = QString(R"(^(?:[1-9]\d{0,%1}):(?:[1-9]\d{0,%1})$)").arg(maxDigits - 1);
    QRegularExpression regex(pattern);
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, lineEdit);
    lineEdit->setValidator(validator);
}
bool ConvertAlgo_Win::eventFilter(QObject *watched, QEvent *event) {

    if (watched == ui->tableWidget && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            int row = ui->tableWidget->currentRow();
            int column = ui->tableWidget->currentColumn();
            if (row == ui->tableWidget->rowCount() - 1 && column == ui->tableWidget->columnCount() - 1) {
                // Move focus to button2 and reset the focus to the first cell
                ui->pushButtonUpload->setFocus();
                ui->tableWidget->setCurrentCell(0, 0);
                return true; // Event handled
            }
        }
    }
    return ConvertAlgo_Win::eventFilter(watched, event);
}
ConvertAlgo_Win::~ConvertAlgo_Win()
{
    delete ui;
}

void ConvertAlgo_Win::hideAnyListViewVisible(){
    algoF2F->slotStartHide("");
    algoF2F->slotEndHide("");

    algoBtFly->slotStartHide("");
    algoBtFly->slotEndHide("");

    algoBtFlyBid->slotStartHide("");
    algoBtFlyBid->slotEndHide("");

    algoConRev->slotStartHide("");
    algoConRev->slotEndHide("");
    algoConRev->slotFutHide("");

    algoCRJellyBid->slotStartHide("");
    algoCRJellyBid->slotEndHide("");
    algoCRJellyBid->slotFutHide("");

    algoBoxBid->slotStartHide("");
    algoBoxBid->slotEndHide("");

    algoBS_1221_1331->slotStartHide("");
    algoBS_1221_1331->slotStartHide("");

    algoOpen2L->slotStartHide("");
    algoOpen2L->slotEndHide("");


}


void ConvertAlgo_Win::on_Close_clicked()
{


    algoBS_1221_1331->clearAllModel();
    algoBoxBid->clearAllModel();
    algoOpen2L->clearAllModel();
    algoCRJellyBid->clearAllModel();
    algoBtFlyBid->clearAllModel();
    algoConRev->clearAllModel();
    algoBtFly->clearAllModel();
    algoF2F->clearAllModel();



    hideAnyListViewVisible();
    close();
}

void ConvertAlgo_Win::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {

        ui->lineEdit_StrikeDifference_Btfy->clear();
        ui->lineEdit_StrikeDifference_BtfyBid->clear();
        ui->lineEdit_Start_strike_Btfy->clear();
        ui->lineEdit_Start_strike_BtfyBid->clear();
        ui->lineEdit_Start_strike_ConvRev->clear();
        ui->lineEdit_Start_strike_f2f->clear();
        ui->lineEdit_Start_strike_BoxBid->clear();
        ui->lineEdit_Start_strike_Bs1221_1331->clear();

        ui->lineEdit_EndStrike_Btfy->clear();
        ui->lineEdit_EndStrike_BtfyBid->clear();
        ui->lineEdit_EndStrike_ConvRev->clear();
        ui->lineEdit_EndStrike_f2f->clear();
        ui->lineEdit_Fut_ConvRev->clear();
        ui->lineEdit_EndStrike_BoxBid->clear();
        ui->lineEdit_EndStrike_Bs1221_1331->clear();
        on_comboBox_AlgoType_currentTextChanged(ui->comboBox_AlgoType->currentText());


        event->accept();                    // Accept the event to indicate it was handled
    }
    else
    {
        QWidget::keyPressEvent(event); // Pass other key events to the base class

    }

}

void ConvertAlgo_Win::update_ui_slot(int type){

    switch (type) {
    case LOADED_MODEL:
        enable_disable_UIElement(true);
        on_comboBox_AlgoType_currentTextChanged(ui->comboBox_AlgoType->currentText());
        break;

    default:
        break;
    }

}



void ConvertAlgo_Win::update_contract_tableData(QString foo_user_id_,int MaxPortfolioCount_){

    QHash<QString, contract_table> contract_table_ = ContractDetail::getInstance().GetContracts();

    sharedData->foo_user_id = foo_user_id_;
    sharedData->MaxPortfolioCount = MaxPortfolioCount_;


   // sharedData->FO_F2F_data_list_Sorted_Key = ContractDetail::getInstance().Get_F2F_data_list_Sorted_Key();
   // sharedData->FO_BFLY_data_list_Sorted_Key = ContractDetail::getInstance().Get_BFLY_data_list_Sorted_Key();
   // sharedData->FO_BFLY_BID_data_list_Sorted_Key = ContractDetail::getInstance().Get_BFLY_BID_data_list_Sorted_Key();

    // default oprion will be FO so use the contract table fo and it's sorted key
    //algoF2F->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
   // algoF2F->model_searchInstrument_F2F_Leg1 =  ContractDetail::getInstance().Get_model_searchInstrument_F2F_Leg1(); //get the model generated from contract class for leg1

   // algoBtFly->sorted_keys_BFLY = sharedData->FO_BFLY_data_list_Sorted_Key;
   // algoBtFly->model_start_strike_BFLY = ContractDetail::getInstance().Get_model_start_strike_BFLY();  //get the model generated from contract class for start strike

    //algoBtFlyBid->sorted_keys_BFLY_BID = sharedData->FO_BFLY_BID_data_list_Sorted_Key;
   // algoBtFlyBid->model_start_strike_BFLY_BID = ContractDetail::getInstance().Get_model_start_strike_BFLY_BID();  //get the model generated from contract class for start strike

 //   algoBoxBid->filtered_tokens_BX_BID = ContractDetail::getInstance().Get_BOX_BID_data_list_Sorted_Key();//sharedData->FO_BFLY_BID_data_list_Sorted_Key;
  //  algoBoxBid->model_start_strike_BOX_BID = ContractDetail::getInstance().Get_model_start_strike_BOX_BID();  //get the model generated from contract class for start strike

   // algoBS_1221_1331->filtered_tokens_BS1221 = ContractDetail::getInstance().Get_BOX_BID_data_list_Sorted_Key();//sharedData->FO_BFLY_BID_data_list_Sorted_Key;
  //  algoBS_1221_1331->model_start_strike_BS1221 = ContractDetail::getInstance().Get_model_start_strike_BOX_BID();  //get the model generated from contract class for start strike

   // algoConRev->sorted_keys_CON_REV = sharedData->FO_BFLY_data_list_Sorted_Key;
   // algoConRev->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
   // algoConRev->model_Fut_CR = ContractDetail::getInstance().Get_model_FUT_CON_REV();  //get the model generated from contract class


    //algoCRJellyBid->sorted_keys_CON_REV_JELLY_BID = sharedData->FO_BFLY_data_list_Sorted_Key;
   // algoCRJellyBid->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
   // algoCRJellyBid->model_Fut_CR_JELLY_BID = ContractDetail::getInstance().Get_model_FUT_CON_REV();  //get the model generated from contract class

    /*algoBOX->sorted_keys_BOX = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoBOX->model_searchInstrument_BOX_Leg1 = ContractDetail::getInstance().Get_model_searchInstrument_BOX_Leg1(); //get the model generated from contract class for leg1


    algoOPEN_BOX->sorted_keys_BOX = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoOPEN_BOX->model_searchInstrument_BOX_Leg1 = ContractDetail::getInstance().Get_model_searchInstrument_BOX_Leg1(); //get the model generated from contract class for leg1



    algoOpen_BtFly->sorted_keys_BFLY = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoOpen_BtFly->model_start_strike_BFLY = ContractDetail::getInstance().Get_model_start_strike_BFLY(); */ //get the model generated from contract class for start strike

    emit display_log_text_slot("Data Copied");
    QFuture<void> futureBackGroundDBThread = QtConcurrent::run(&ConvertAlgo_Win::sort_data_and_populate_model,this,contract_table_/*,cd_contract_table_hash_*/);
}

void ConvertAlgo_Win::sort_data_and_populate_model(QHash<QString, contract_table> contract_table_/*,QHash<QString, contract_table> cd_contract_table_*/){
    // qDebug()<<"Populating data on inpute file started..";

    emit display_log_text_slot("Setting contract data for input field..");
    //filter data based on instrument_type
   /* QHashIterator<QString, contract_table> iter1(contract_table_[PortfolioType::BFLY_BID]);

    while (iter1.hasNext()){
        iter1.next();
        sharedData->fo_contract_table_hash[QString::number(iter1.value().TokenNumber)] = iter1.value();
    }

    QHashIterator<QString, contract_table> iter2(contract_table_[PortfolioType::BY]);

    while (iter2.hasNext()){
        iter2.next();
        sharedData->fo_contract_table_hash[QString::number(iter2.value().TokenNumber)] = iter2.value();
    }


    QHashIterator<QString, contract_table> iter3(contract_table_[PortfolioType::F2F]);

    while (iter3.hasNext()){
        iter3.next();
        sharedData->fo_contract_table_hash[QString::number(iter3.value().TokenNumber)] = iter3.value();
    }*/
    // default oprion will be FO so use the contract table  for fo option
    sharedData->contract_table_hash = ContractDetail::getInstance().GetContracts();//sharedData->fo_contract_table_hash;
    //algoF2F->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
    //algoBtFly->sorted_keys_BFLY = sharedData->FO_BFLY_data_list_Sorted_Key;
    //algoConRev->sorted_keys_CON_REV = sharedData->FO_BFLY_data_list_Sorted_Key;
    //algoConRev->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
    //algoBOX->sorted_keys_BOX = sharedData->FO_BFLY_data_list_Sorted_Key;

    //sharedData->exchange = _nse_fo;


    // emit display_log_text_slot("Creating autofill model for F2F with contract table length = "+QString::number(algoF2F->sorted_keys_F2F.length()));
    // algoF2F->create_AutoFillModel_SearchInstrument();

    // emit display_log_text_slot("Creating autofill model for BtFly with contract table length = "+QString::number(algoBtFly->sorted_keys_BFLY.length()));
    // algoBtFly->create_AutoFillModel_StartStrike();

    // emit display_log_text_slot("Creating autofill model for ConRev with contract table length = "+QString::number(algoConRev->sorted_keys_CON_REV.length()));
    //algoConRev->create_AutoFillModel_FutInstrument();

    //emit display_log_text_slot("Creating autofill model for BOX with contract table length = "+QString::number(algoBOX->sorted_keys_BOX.length()));
    // algoBOX->create_AutoFillModel_SearchInstrument();

    // algo2LStraddle->create_AutoFillModel_StartStrike();

    // emit display_log_text_slot("Creating autofill model for all algo finished");

    // update_input_field();
    dataSorted.storeRelaxed(1);
    emit update_ui_signal(LOADED_MODEL);
    // qDebug()<<"Populating data on inpute filed finished..";
    emit display_log_text_slot("Model initialized.");
}

void ConvertAlgo_Win::mousePressEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child!=ui->ConvertAlgo_TitleBar)
    {
        return;
    }
    pressed = true;
    position = event->pos();
}

void ConvertAlgo_Win::mouseMoveEvent(QMouseEvent *event) {
    if(pressed){
        if (event->buttons() == Qt::LeftButton) {
            if (position.x() != 0 || position.y() != 0) {
                move(event->globalPosition().x() - position.x(), event->globalPosition().y() - position.y());
            }
        }
    }
}

void ConvertAlgo_Win::mouseReleaseEvent(QMouseEvent *event) {
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (child!=ui->ConvertAlgo_TitleBar)
    {
        return;
    }
    pressed = false;
}

void ConvertAlgo_Win::resetTableWidget() {
    QStringList headers = {};
    sharedData->unique_id = 0;

    QString algoType = ui->comboBox_AlgoType->currentText();
    if (algoType == "F2F-BID") {
        headers = {"Algo Name", "Strike/Expiry1", "Strike/Expiry2", "Status"};
    } else if (algoType == "BFLY") {
        headers = {"Algo Name", "Option Type ", "Expiry", "Leg1 Strike", "Leg2 Strike", "Leg3 Strike", "Reserved", "Reserved", "Status"};
    } else if (algoType == "CR-BID") {
        headers = {"Algo Name", "Strike/Expiry1", "Strike/Expiry2", "Strike/Expiry3", "Status"};
    } else if (algoType == "2L Straddle") {
        headers = {"Algo Name", "Leg1 Strike", "Leg2 Strike", "Reserved", "Reserved", "Status"};
    } else if (algoType == "BOX") {
        headers = {"Algo Name", "Strike/Expiry1", "Strike/Expiry2", "Market/Skip Leg", "Buy Diff", "Sell Diff", "Status"};
    } else if (algoType == "Open-BFLY") {
        headers = {"Algo Name", "Market-Skip/Leg-Ratio", "Buy Diff", "Sell Diff", "Status"};
    } else if (algoType == "Open-BOX") {
        headers = {"Algo Name", "Strike/Expiry1", "Strike/Expiry2", "Market/Skip Leg", "Market-Skip/Leg-Ratio", "Buy Diff", "Sell Diff", "Status"};
    } else if (algoType == "BFLY-BID") {
        headers = {"Algo Name", "Strike1", "Strike2", "Strike3", "Status"};
    } else if (algoType == "CRJELLY-BID") {
        headers = {"Algo Name", "Strike/Expiry1", "Strike/Expiry2", "Strike/Expiry3", "Status"};
    } else if (algoType == "BX-BID") {
        headers = {"Algo Name", "Expiry", "Strike1", "Strike2", "Strike3", "Strike4", "Status"};
    } else if (algoType == "BS12-BID") {
        headers = {"Algo Name", "Expiry", "Strike1", "Strike2", "Strike3", "Strike4", "Status"};
    }else if (algoType == "BS13-BID") {
        headers = {"Algo Name", "Expiry", "Strike1", "Strike2", "Strike3", "Strike4", "Status"};
    }else if (algoType == "OPEN-2L") {
        headers = {"Algo Name", "Expiry", "Strike1", "Strike2","Status"};
    }

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QHeaderView *header = ui->tableWidget->horizontalHeader();
    header->setStyleSheet("QHeaderView::section {"
                          "border: 1px solid lightgrey;"  // Simple border style
                          "background-color: white;" // Optional: background color
                          "}");

    ui->tableWidget->resizeColumnsToContents();
    ui->comboBox_AlgoType->setFocus();
}


void ConvertAlgo_Win::enable_disable_UIElement(bool enable)
{
    ui->lineEdit_StrikeDifference_Btfy->setEnabled(enable);
    ui->lineEdit_StrikeDifference_BtfyBid->setEnabled(enable);
    ui->lineEdit_Start_strike_Btfy->setEnabled(enable);
    ui->lineEdit_Start_strike_BtfyBid->setEnabled(enable);

    ui->lineEdit_Start_strike_ConvRev->setEnabled(enable);
    ui->lineEdit_Start_strike_f2f->setEnabled(enable);
    ui->lineEdit_EndStrike_Btfy->setEnabled(enable);
    ui->lineEdit_EndStrike_BtfyBid->setEnabled(enable);
    ui->lineEdit_EndStrike_ConvRev->setEnabled(enable);
    ui->lineEdit_EndStrike_f2f->setEnabled(enable);
    ui->lineEdit_Fut_ConvRev->setEnabled(enable);

    ui->comboBox_AlgoType->setEnabled(enable);
    ui->pushButtonAdd->setEnabled(enable);
    ui->pushButton_Reset->setEnabled(enable);
    ui->pushButtonDelete->setEnabled(enable);
    ui->pushButtonUpload->setEnabled(enable);

    ui->lineEdit_Start_strike_BoxBid->setEnabled(enable);
    ui->lineEdit_EndStrike_BoxBid->setEnabled(enable);

}


void ConvertAlgo_Win::display_log_text_slot(QString msg)
{
    msg = QDateTime::currentDateTime().toString()+": "+msg;

    //log_msgList.append(msg);
//    ui->label_log_message->setText(msg);
}





void ConvertAlgo_Win::on_pushButtonAdd_clicked()
{
    QString algo_type  = ui->comboBox_AlgoType->currentText();
    if(algo_type=="F2F-BID"){
        algoF2F->generateAlgo();
    }

    /************BFLY*************************************/
    else if(algo_type=="BFLY"){
        algoBtFly->generateAlgo();
    }
    /************************BFLY*************************************/

    else if(algo_type=="CR-BID"){
        algoConRev->generateAlgo();

    }
    else if(algo_type=="BFLY-BID"){
        algoBtFlyBid->generateAlgo();
    }

    else if(algo_type=="CRJELLY-BID"){
        algoCRJellyBid->generateAlgo();
    }
    else if(algo_type=="BX-BID"){
        algoBoxBid->generateAlgo();
    }
    else if(algo_type=="BS12-BID"){
        algoBS_1221_1331->generateAlgo();
    }
    else if(algo_type=="BS13-BID"){
        algoBS_1221_1331->generateAlgo();
    }
    else if(algo_type=="OPEN-2L"){
        algoOpen2L->generateAlgo();
    }


//    if(algo_type=="BOX"){
//        algoBOX->generateAlgo();
//    }

//    if(algo_type=="Open-BFLY"){
//        algoOpen_BtFly->generateAlgo();
//    }
//    if(algo_type=="Open-BOX"){
//        algoOPEN_BOX->generateAlgo();
//    }

    /* else if(algo_type=="2L Straddle"){
        algo2LStraddle->generateAlgo();


    }*/
}


void ConvertAlgo_Win::on_comboBox_AlgoType_currentTextChanged(const QString algoType)
{
    if(dataSorted.loadRelaxed()==0)
        return;

    hideAnyListViewVisible();

    resetTableWidget();
    sharedData->algo_data_list.clear();

    if(algoType=="F2F-BID"){

        ui->stackedWidget->setCurrentWidget(ui->page_f2f);
        algoF2F->selectedAction();

    }
    if(algoType=="BFLY"){

        ui->stackedWidget->setCurrentWidget(ui->pageBtfy);
        algoBtFly->selectedAction();
    }
    else if(algoType=="CR-BID"){

        ui->stackedWidget->setCurrentWidget(ui->pageConvRev);
        algoConRev->selectedAction();
    }
    else if(algoType=="BFLY-BID"){

        ui->stackedWidget->setCurrentWidget(ui->pageBtfyBid);
        algoBtFlyBid->selectedAction();
    }

    else if(algoType=="CRJELLY-BID"){

        ui->stackedWidget->setCurrentWidget(ui->pageCRJellyBid);
        algoCRJellyBid->selectedAction();
    }

    else if(algoType=="BX-BID"){
        ui->stackedWidget->setCurrentWidget(ui->pagBoxBid);
        algoBoxBid->selectedAction();
    }
    else if(algoType=="BS12-BID"){
        ui->stackedWidget->setCurrentWidget(ui->pagBS1221_1331);
        algoBS_1221_1331->selectedAction(PortfolioType::BS1221);
    }
    else if(algoType=="BS13-BID"){
        ui->stackedWidget->setCurrentWidget(ui->pagBS1221_1331);
        algoBS_1221_1331->selectedAction(PortfolioType::BS1331);
    }
    else if(algoType=="OPEN-2L"){
        ui->stackedWidget->setCurrentWidget(ui->pageOpen_2L);
        algoOpen2L->selectedAction();
    }

//    else if(algoType=="2L Straddle"){

//        ui->lineEdit_Fut->setVisible(false);
//        ui->lineEdit_StrikeDifference->setVisible(false);
//        ui->lineEdit_EndStrike->setVisible(true);
//        ui->qty_widget_parent->setVisible(false);
//        ui->label_qty_widget_parent->setVisible(false);
//        //algo2LStraddle->selectedAction();

//    }

//    if(algoType=="BOX"){

//        ui->lineEdit_StrikeDifference->setVisible(false);
//        ui->lineEdit_Fut->setVisible(false);
//        ui->qty_widget_parent->setVisible(false);
//        ui->label_qty_widget_parent->setVisible(false);
//        //  ui->lineEdit_EndStrike->setVisible(false);

//        algoBOX->selectedAction();

//    }
//    else if (algoType=="Open-BOX"){
//        ui->lineEdit_StrikeDifference->setVisible(false);
//        ui->lineEdit_Fut->setVisible(false);
//        ui->qty_widget_parent->setVisible(true);
//        ui->label_qty_widget_parent->setVisible(true);
//        ui->mkt_qty4->setVisible(true);
//        //  ui->lineEdit_EndStrike->setVisible(false);
//        algoOPEN_BOX->selectedAction();

//    }

//    if(algoType == "Open-BFLY"){
//        ui->lineEdit_StrikeDifference->setVisible(true);
//        ui->lineEdit_EndStrike->setVisible(true);
//        ui->lineEdit_Fut->setVisible(false);
//        ui->qty_widget_parent->setVisible(true);
//        ui->label_qty_widget_parent->setVisible(true);
//        ui->mkt_qty4->setVisible(false);
//        algoOpen_BtFly->selectedAction();
//    }
}




void ConvertAlgo_Win::on_pushButton_Cancel_clicked()
{
    this->close();
}


void ConvertAlgo_Win::on_pushButtonUpload_clicked()
{
    //QStringList addedIdx;
    // QList<algo_data_to_insert> algo_data_list_added;
    //get unique table row ids for selected rows in table to match it with data to be uploaded(algo_data_list)
    QList<int> selectedIds;
    QList<int> selectedrowsIDx;

    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows();
    for (int i = 0; i < selected.size(); i++){
        QTableWidgetItem *item=ui->tableWidget->item(selected[i].row(),0);
        selectedIds.append(item->data(Qt::UserRole + 1).toInt());
        selectedrowsIDx.append(selected[i].row());
        //qDebug()<<item->data(Qt::UserRole + 1).toInt();

    }
    if(selectedIds.size()==0){
        QMessageBox msgBox;
        msgBox.setText("No Data selected to insert.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    bool portfolioAdded =false;
    for(int k=0;k<selectedIds.length();k++){
        for(int i=0;i<sharedData->algo_data_list.size();i++){
            if(selectedIds[k]!=sharedData->algo_data_list[i].table_row_unique_id)
                continue;
            if(sharedData->algo_data_list[i].uploaded==true)
                continue;

            int tableRowIDx = selectedrowsIDx[k];
            QString msg = "";
            algo_data_insert_status status = db_conn->insertToAlgoTable(sharedData->algo_data_list[i],sharedData->MaxPortfolioCount,msg);
            //emit insert_algo_data_signal(algo_data_list);
            if(status==algo_data_insert_status::INSERTED){
                portfolioAdded = true;
                //  QString Algo_Name = algo_type+"-"+sharedData->algo_data_list[i].Algo_Name;
                //db_conn->logToDB(QString("Portfolio Added "));
                sharedData->algo_data_list[i].uploaded=true;
                ui->tableWidget->item(tableRowIDx, ui->tableWidget->columnCount()-1)->setForeground(QBrush(QColor(0, 200, 0)));
                QString algo_type  = sharedData->algo_data_list[i].algo_type;
                if(algo_type==QString::number(PortfolioType::F2F))
                    algo_type="F2F-BID";
                else if(algo_type==QString::number(PortfolioType::BY))
                    algo_type="BFLY";
                else if(algo_type==QString::number(PortfolioType::CR))
                    algo_type="CR-BID";
                else if(algo_type==QString::number(PortfolioType::BOX))
                    algo_type="BOX";
                else if(algo_type==QString::number(PortfolioType::OPEN_BY))
                    algo_type="Open-BFLY";
                else if(algo_type==QString::number(PortfolioType::OPEN_BOX))
                    algo_type="Open-BOX";
                else if(algo_type==QString::number(PortfolioType::BFLY_BID))
                    algo_type="BFLY_BID";
                else if(algo_type==QString::number(PortfolioType::CR_JELLY))
                    algo_type="CRJELLY";
                else if(algo_type==QString::number(PortfolioType::BX_BID))
                    algo_type="BX-BID";
                else if(algo_type==QString::number(PortfolioType::BS1221))
                    algo_type="BS12-BID";
                else if(algo_type==QString::number(PortfolioType::BS1331))
                    algo_type="BS13-BID";
                else if(algo_type==QString::number(PortfolioType::OPEN2L))
                    algo_type="OPEN-2L";




                QString Algo_Name = algo_type+"-"+sharedData->algo_data_list[i].Algo_Name;
                QString htmlContent = "<p style='font-family:\"Work Sans\"; font-weight:800; font-size:12px;line-height:1.2;'>"
                                      "<span>" + QTime::currentTime().toString("hh:mm:ss") +"&nbsp;</span>"
                              + "<span style='font-weight:400;/*color: black;*/'>"+/*sharedData->foo_user_id +*/
                                      " Inserted New Algo: "+ Algo_Name +"</span> </p>";

                emit display_log_text_signal(htmlContent);
                db_conn->logToDB(QString("Portfolio Added "+Algo_Name));
            }
            else if(status==algo_data_insert_status::FAILED){
                ui->tableWidget->item(tableRowIDx, ui->tableWidget->columnCount()-1)->setForeground(QBrush(QColor(200, 0, 0)));
            }
            else if(status==algo_data_insert_status::LIMIT_REACHED){
                ui->tableWidget->item(tableRowIDx, ui->tableWidget->columnCount()-1)->setForeground(QBrush(QColor(200, 0, 0)));
                db_conn->logToDB("Exceeds the  limit of maximum allowed Algo count "+QString::number(sharedData->MaxPortfolioCount));
            }
            else if(status==algo_data_insert_status::EXIST){
                sharedData->algo_data_list[i].uploaded=true;
                ui->tableWidget->item(tableRowIDx, ui->tableWidget->columnCount()-1)->setForeground(QBrush(QColor(255, 191, 0)));
            }
            ui->tableWidget->item(tableRowIDx, ui->tableWidget->columnCount()-1)->setText(msg);

        }
    }

    if(portfolioAdded){
        emit portfolioAddedSignal();
    }
    ///remove already added item from list.
    QMutableListIterator<algo_data_to_insert> i(sharedData->algo_data_list);
    while (i.hasNext()) {
        if (i.next().uploaded == true)
            i.remove();
    }


}


void ConvertAlgo_Win::on_pushButtonSelectAll_clicked()
{
    ui->tableWidget->selectAll();
}


void ConvertAlgo_Win::on_pushButton_Reset_clicked()
{
    resetTableWidget();
    sharedData->algo_data_list.clear();
    ui->lineEdit_StrikeDifference_Btfy->clear();
    ui->lineEdit_StrikeDifference_BtfyBid->clear();
    ui->lineEdit_Start_strike_Btfy->clear();
    ui->lineEdit_Start_strike_BtfyBid->clear();
    ui->lineEdit_Start_strike_ConvRev->clear();
    ui->lineEdit_Start_strike_f2f->clear();
    ui->lineEdit_EndStrike_Btfy->clear();
    ui->lineEdit_EndStrike_BtfyBid->clear();
    ui->lineEdit_EndStrike_ConvRev->clear();
    ui->lineEdit_EndStrike_f2f->clear();
    ui->lineEdit_Fut_ConvRev->clear();
    ui->lineEdit_EndStrike_CRJellyBid->clear();
    ui->lineEdit_Fut_CRJellyBid->clear();
    ui->lineEdit_Start_strike_CRJellyBid->clear();

    ui->lineEdit_Start_strike_BoxBid->clear();
    ui->lineEdit_EndStrike_BoxBid->clear();

    ui->lineEdit_Start_strike_Bs1221_1331->clear();
    ui->lineEdit_EndStrike_Bs1221_1331->clear();
}


void ConvertAlgo_Win::on_pushButtonDelete_clicked()
{
    QList<int> selectedIds;
    QList<int> selectedrowsIDx;
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows();
    for (int i = 0; i < selected.size(); i++){
        QTableWidgetItem *item=ui->tableWidget->item(selected[i].row(),0);
        selectedIds.append(item->data(Qt::UserRole + 1).toInt());
        selectedrowsIDx.append(selected[i].row());
       // qDebug()<<item->data(Qt::UserRole + 1).toInt();

    }

    if(selected.size()==0){
        QMessageBox msgBox;
        msgBox.setText("Please select an Item from table.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    //make the index in reverse order so table widget row index will not change while delete last
    std::sort(selectedrowsIDx.rbegin(), selectedrowsIDx.rend());
    // qDebug()<<selectedrowsIDx;

    for(int i=0;i<selectedrowsIDx.size();i++){
        ui->tableWidget->removeRow(selectedrowsIDx[i]);
    }

    for(int i=0;i<selectedIds.size();i++){
        for(int k=0;k<sharedData->algo_data_list.size();k++){
            if(selectedIds[i]==sharedData->algo_data_list[k].table_row_unique_id){
                sharedData->algo_data_list.removeAt(k);
                break;
            }
        }
    }

    //algo_data_list.removeAt(row);
}

void ConvertAlgo_Win::progressSlot(bool show, QString msg){

     ui->progressBar->setVisible(show);
     ui->label_ProgressText->setVisible(show);
     ui->label_ProgressText->setText(msg);

   //  ui->progressBar->setRange(0, 0);         // Indeterminate state (busy mode)
    // ui->progressBar->setTextVisible(true);   // Ensure text is visible
    // ui->progressBar->setFormat(msg);
}






