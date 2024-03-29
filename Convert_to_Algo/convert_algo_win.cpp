#include "convert_algo_win.h"
#include "ContractDetail.h"
#include "qfuture.h"
#include "mainwindow.h"

extern MainWindow *MainWindowObj;

ConvertAlgo_Win::ConvertAlgo_Win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConvertAlgo_Win)
{
    ui->setupUi(this);
    MainWindowObj = (MainWindow*) parent;
    setWindowModality(Qt::ApplicationModal);


    ui->tableWidget->setFocusPolicy(Qt::NoFocus);

    // setTabOrder(ui->comboBox_AlgoType, ui->lineEdit_Start_strike);
    setTabOrder(ui->lineEdit_Start_strike, ui->lineEdit_EndStrike);
    setTabOrder(ui->lineEdit_EndStrike, ui->lineEdit_StrikeDifference);
    setTabOrder(ui->lineEdit_StrikeDifference, ui->pushButtonAdd);
    setTabOrder(ui->pushButtonAdd, ui->pushButtonSelectAll);
    setTabOrder(ui->pushButtonSelectAll, ui->pushButton_Reset);
    setTabOrder(ui->pushButton_Reset, ui->pushButtonDelete);
    // setTabOrder(ui->pushButtonDelete, ui->tableWidget);
    // setTabOrder(ui->tableWidget, ui->pushButtonUpload);
    setTabOrder(ui->pushButtonDelete, ui->pushButtonUpload);
    setTabOrder(ui->pushButtonUpload, ui->pushButtonUpload);
    setTabOrder(ui->pushButton_Cancel, ui->lineEdit_Start_strike);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // to make floating window
    listViewStartStrike = new QListView(this);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    listViewStartStrike->setSizePolicy(sizePolicy);
    listViewStartStrike->setFixedSize(230, 200);
    listViewStartStrike->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Get the geometry of lineEdit_EndStrike
    QPoint lineEditPosSS = ui->lineEdit_Start_strike->mapToGlobal(ui->lineEdit_Start_strike->geometry().bottomRight());
    // Set the position of the QListView just below lineEdit_EndStrike
    listViewStartStrike->move(lineEditPosSS.x()+20, lineEditPosSS.y()+55);

    // to make floating window
    listViewEndStrike = new QListView(this);
    QPoint lineEditPosES = ui->lineEdit_EndStrike->mapToGlobal(ui->lineEdit_EndStrike->geometry().bottomRight());
    listViewEndStrike->setSizePolicy(sizePolicy);
    listViewEndStrike->setFixedSize(230, 200);
    listViewEndStrike->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // Set the position of the QListView just below lineEdit_EndStrike
    listViewEndStrike->move(lineEditPosES.x()+270, lineEditPosSS.y()+55);
    // to make floating window

    QPixmap pixmapclose(":/close_window_icon.png");
    ui->Close->setIcon(pixmapclose);

    const char LineEdit_SS[]="border-radius: 6px;""border: 0.5px solid #343A40;""background: #FFF;""color: #6C757D;""font-size: 12px;""font-style: normal;""font-weight: 400;""line-height: normal;" "padding: 4px 10px 4px 10px;";
    for(auto w: {ui->lineEdit_StrikeDifference, ui->lineEdit_EndStrike, ui->lineEdit_Start_strike,ui->lineEdit_Fut}){
        w->setStyleSheet(LineEdit_SS);
        QFont font=w->font();
        font.setFamily("Work Sans");
        w->setFont(font);
    }
    const char pushButton_SS[]="border-radius: 8px;""border: 1px solid #485F6B;""background: #485F6B;""box-shadow: 5px 4px 4.2px 0px rgba(0, 0, 0, 0.10);""color: #FFF;""    text-align: center;""    font-size: 14px;""    font-style: normal;""    font-weight: 500;""    line-height: normal;";
    ui->pushButtonAdd->setStyleSheet(pushButton_SS);
    QFont font=ui->pushButtonAdd->font();
    font.setFamily("Work Sans");
    ui->pushButtonAdd->setFont(font);

    const char label_SS[]= "color: #343A40; ""text-align: center; ""font-size: 14px; ""font-style: normal; ""font-weight: 600; ""line-height: normal;";
    ui->label->setStyleSheet(label_SS);
    QFont font_label=ui->label->font();
    font_label.setFamily("Work Sans");
    ui->label->setFont(font_label);

    const char toolButton_SS[]="border-radius: 4px; "
                               "border: 0.5px solid #465460; "
                               "background: #FFF; "
                               "box-shadow: 0px 1px 2px 0px rgba(0, 0, 0, 0.25); "
                               "color: #465460; "
                               "text-align: center; "
                               "font-size: 8px; "
                               "font-style: normal; "
                               "font-weight: 500; "
                               "line-height: normal;";
    for(auto w:{ui->pushButtonDelete,ui->pushButtonSelectAll,ui->pushButton_Reset}){
        w->setStyleSheet(toolButton_SS);
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

    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->verticalHeader()->setVisible(false);

    /*******Class to generate F2F algos************/
    algoF2F = new add_algo_f2f();
    algoF2F->copyUIElement(ui->tableWidget,ui->lineEdit_Start_strike,ui->lineEdit_EndStrike);
    // /*******Class to generate F2F algos************/

    /*******Class to generate BtFly algos************/
    algoBtFly= new add_algo_btfly();
    algoBtFly->copyUIElement(ui->tableWidget,ui->lineEdit_Start_strike,ui->lineEdit_EndStrike,ui->lineEdit_StrikeDifference,listViewStartStrike,listViewEndStrike);
    /*******Class to generate BtFly algos************/

    /*******Class to generate con_rev algos************/
    algoConRev= new add_algo_con_rev();
    algoConRev->copyUIElement(ui->tableWidget,ui->lineEdit_Start_strike,ui->lineEdit_EndStrike,ui->lineEdit_Fut);
    /*******Class to generate con_rev algos************/

    /*******Class to generate BtFly-bid algos************/
    algoBtFlyBid= new add_algo_btfly_bid();
    algoBtFlyBid->copyUIElement(ui->tableWidget,ui->lineEdit_Start_strike,ui->lineEdit_EndStrike,ui->lineEdit_StrikeDifference,listViewStartStrike,listViewEndStrike);
    /*******Class to generate BtFly-bid algos************/

    QDoubleValidator *val = new QDoubleValidator();
    val->setLocale(QLocale::C);
    val->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit_StrikeDifference->setValidator(val);

    // ui->comboBox_AlgoType->clear();
    // ui->comboBox_AlgoType->addItem(BFLY_BID_TYPE);
    // ui->comboBox_AlgoType->addItem("F2F");
    // ui->comboBox_AlgoType->addItem("BFLY");
    // ui->comboBox_AlgoType->addItem("CON-REV");
    // ui->comboBox_AlgoType->addItem("BOX");
    // ui->comboBox_AlgoType->addItem("Open-BFLY");
    // ui->comboBox_AlgoType->addItem("Open-BOX");

    // // not implemented yet so disabled
    // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_F2F, QVariant(0), Qt::UserRole-1);
    // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_BFLY, QVariant(0), Qt::UserRole-1);
    // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_CON_REV, QVariant(0), Qt::UserRole-1);
    // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_Open_BFLY, QVariant(0), Qt::UserRole-1);
    // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_BOX, QVariant(0), Qt::UserRole-1);
    // ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_Open_BOX, QVariant(0), Qt::UserRole-1);
    // // not implemented yet so disabled

    sharedData->unique_id = 0; // this used to identyfy table row and algo_data to upload

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    QString bidTypeFromSettings{};
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if (settings.contains("market_type"))
            market_type = settings.value("market_type").toString();
        if (settings.contains("enable_bidType"))
            bidTypeFromSettings = settings.value("enable_bidType").toString();
        settings.endGroup();
    }
    if(market_type=="fo"){
        sharedData->devicer = FO_DEVICER;
        sharedData->decimal_precision = FO_DECIMAL_PRECISION;
    }
    else{
        sharedData->devicer = CDS_DEVICER;
        sharedData->decimal_precision = CDS_DECIMAL_PRECISION;
    }

     ui->comboBox_AlgoType->setItemData(1, QVariant(0), Qt::UserRole-1);

        ui->comboBox_AlgoType->clear();
        ui->comboBox_AlgoType->addItem(BFLY_BID_TYPE);
        ui->comboBox_AlgoType->addItem("F2F");
        ui->comboBox_AlgoType->addItem("BFLY");
        ui->comboBox_AlgoType->addItem("CON-REV");
        ui->comboBox_AlgoType->addItem("BOX");
        ui->comboBox_AlgoType->addItem("Open-BFLY");
        ui->comboBox_AlgoType->addItem("Open-BOX");

        // not implemented yet so disabled
        ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_F2F, QVariant(0), Qt::UserRole-1);
        ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_CON_REV, QVariant(0), Qt::UserRole-1);
        ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_Open_BFLY, QVariant(0), Qt::UserRole-1);
        ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_BOX, QVariant(0), Qt::UserRole-1);
        ui->comboBox_AlgoType->setItemData(BID_TYPE::INDEX_ALGO_Open_BOX, QVariant(0), Qt::UserRole-1);
        // not implemented yet so disabled



    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    enable_disable_UIElement(false);
    resetTableWidget();
    ui->comboBox_AlgoType->setFocus();
}

ConvertAlgo_Win::~ConvertAlgo_Win()
{
    delete ui;
}

void ConvertAlgo_Win::on_Close_clicked()
{
    close();
}

void ConvertAlgo_Win::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        event->ignore(); // This line ignores the key press event
    } else {
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

    QMap<int, QHash<QString, contract_table>> contract_table_ = ContractDetail::getInstance().GetContracts();

    sharedData->foo_user_id = foo_user_id_;
    sharedData->MaxPortfolioCount = MaxPortfolioCount_;

    sharedData->FO_F2F_data_list_Sorted_Key = ContractDetail::getInstance().Get_F2F_data_list_Sorted_Key();
    sharedData->FO_BFLY_data_list_Sorted_Key = ContractDetail::getInstance().Get_BFLY_data_list_Sorted_Key();
    sharedData->FO_BFLY_BID_data_list_Sorted_Key = ContractDetail::getInstance().Get_BFLY_BID_data_list_Sorted_Key();

    // default oprion will be FO so use the contract table fo and it's sorted key
    algoF2F->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
    algoF2F->model_searchInstrument_F2F_Leg1 =  ContractDetail::getInstance().Get_model_searchInstrument_F2F_Leg1(); //get the model generated from contract class for leg1

    algoBtFly->sorted_keys_BFLY = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoBtFly->model_start_strike_BFLY = ContractDetail::getInstance().Get_model_start_strike_BFLY();  //get the model generated from contract class for start strike

    algoBtFlyBid->sorted_keys_BFLY_BID = sharedData->FO_BFLY_BID_data_list_Sorted_Key;
    algoBtFlyBid->model_start_strike_BFLY_BID = ContractDetail::getInstance().Get_model_start_strike_BFLY_BID();  //get the model generated from contract class for start strike

    algoConRev->sorted_keys_CON_REV = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoConRev->sorted_keys_F2F = sharedData->FO_F2F_data_list_Sorted_Key;
    algoConRev->model_FUT_CON_REV = ContractDetail::getInstance().Get_model_FUT_CON_REV();  //get the model generated from contract class

    /*algoBOX->sorted_keys_BOX = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoBOX->model_searchInstrument_BOX_Leg1 = ContractDetail::getInstance().Get_model_searchInstrument_BOX_Leg1(); //get the model generated from contract class for leg1


    algoOPEN_BOX->sorted_keys_BOX = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoOPEN_BOX->model_searchInstrument_BOX_Leg1 = ContractDetail::getInstance().Get_model_searchInstrument_BOX_Leg1(); //get the model generated from contract class for leg1



    algoOpen_BtFly->sorted_keys_BFLY = sharedData->FO_BFLY_data_list_Sorted_Key;
    algoOpen_BtFly->model_start_strike_BFLY = ContractDetail::getInstance().Get_model_start_strike_BFLY(); */ //get the model generated from contract class for start strike

    emit display_log_text_slot("Data Copied");
    QFuture<void> futureBackGroundDBThread = QtConcurrent::run(&ConvertAlgo_Win::sort_data_and_populate_model,this,contract_table_/*,cd_contract_table_hash_*/);
}

void ConvertAlgo_Win::sort_data_and_populate_model(QMap<int, QHash<QString, contract_table>> contract_table_/*,QHash<QString, contract_table> cd_contract_table_*/){
    // qDebug()<<"Populating data on inpute file started..";

    emit display_log_text_slot("Setting contract data for input field..");
    //filter data based on instrument_type
    QHashIterator<QString, contract_table> iter1(contract_table_[PortfolioType::BFLY_BID]);

    while (iter1.hasNext()){
        iter1.next();
        sharedData->fo_contract_table_hash[QString::number(iter1.value().TokenNumber)] = iter1.value();
    }

    QHashIterator<QString, contract_table> iter2(contract_table_[PortfolioType::BY]);

    while (iter1.hasNext()){
        iter1.next();
        sharedData->fo_contract_table_hash[QString::number(iter1.value().TokenNumber)] = iter1.value();
    }


    QHashIterator<QString, contract_table> iter3(contract_table_[PortfolioType::F2F]);

    while (iter1.hasNext()){
        iter1.next();
        sharedData->fo_contract_table_hash[QString::number(iter1.value().TokenNumber)] = iter1.value();
    }
    // default oprion will be FO so use the contract table  for fo option
    sharedData->contract_table_hash = sharedData->fo_contract_table_hash;
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

void ConvertAlgo_Win::resetTableWidget(){
    QStringList headers = {};
    sharedData->unique_id = 0;


    QString algoType = ui->comboBox_AlgoType->currentText();
    if(algoType=="F2F"){
        headers = {"Algo Name","Strike/Expiry1","Strike/Expiry2","Market/Skip Leg","Buy Diff","Sell Diff","Status"};
    }
    else if(algoType=="BFLY"){
        headers = {"Algo Name","Option Type ","Expiry","Leg1 Strike","Leg2 Strike","Leg3 Strike","Reserved","Reserved","Status"};
    }
    else if(algoType=="CON-REV"){
        headers = {"Algo Name","Strike/Expiry1","Strike/Expiry2","Strike/Expiry3","Buy Diff","Sell Diff","Status"};
    }
    else if(algoType=="2L Straddle"){
        headers = {"Algo Name","Leg1 Strike","Leg2 Strike","Reserved","Reserved","Status"};
    }
    else if(algoType=="BOX"){
        headers = {"Algo Name","Strike/Expiry1","Strike/Expiry2","Market/Skip Leg","Buy Diff","Sell Diff","Status"};
    }
    else if(algoType=="Open-BFLY"){
        headers = {"Algo Name","Market-Skip/Leg-Ratio","Buy Diff","Sell Diff","Status"};
    }
    else if(algoType=="Open-BOX"){
        headers = {"Algo Name","Strike/Expiry1","Strike/Expiry2","Market/Skip Leg","Market-Skip/Leg-Ratio","Buy Diff","Sell Diff","Status"};
    }
    else if(algoType==BFLY_BID_TYPE){
        headers = {"Algo Name","Option Type ","Expiry","Leg1 Strike","Leg2 Strike","Leg3 Strike","Reserved","Reserved","Status"};
    }

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    /*for(int i=0;i<headers.size()-1;i++){
            ui->tableWidget->setColumnWidth(i, ui->tableWidget->width()/headers.size());
        }*/
    /* ui->tableWidget->setColumnWidth(0, 100);
        ui->tableWidget->setColumnWidth(1, 100);
        ui->tableWidget->setColumnWidth(2, 100);
        ui->tableWidget->setColumnWidth(3, 100);
        ui->tableWidget->setColumnWidth(4, 60);
        ui->tableWidget->setColumnWidth(5, 60);*/

    ui->tableWidget->resizeColumnsToContents();


    ui->comboBox_AlgoType->setFocus();

}

void ConvertAlgo_Win::enable_disable_UIElement(bool enable)
{
    ui->comboBox_AlgoType->setEnabled(enable);
    ui->lineEdit_Start_strike->setEnabled(enable);
    ui->lineEdit_EndStrike->setEnabled(enable);
    ui->lineEdit_StrikeDifference->setEnabled(enable);
    ui->pushButtonAdd->setEnabled(enable);
    ui->pushButton_Reset->setEnabled(enable);
    ui->pushButtonDelete->setEnabled(enable);
    ui->pushButtonUpload->setEnabled(enable);
    ui->lineEdit_Fut->setEnabled(enable);
}


void ConvertAlgo_Win::display_log_text_slot(QString msg)
{
    msg = QDateTime::currentDateTime().toString()+": "+msg;

    //log_msgList.append(msg);
//    ui->label_log_message->setText(msg);
}



void ConvertAlgo_Win::on_lineEdit_Start_strike_editingFinished()
{
    QString algo_type  = ui->comboBox_AlgoType->currentText();

    if(algo_type=="F2F"){
        algoF2F->instrumentEditFinishedAction();
    }
    else if(algo_type=="BFLY"){
        algoBtFly->startStrikeEditFinishedAction();
    }
    else if(algo_type=="CON-REV"){
        algoConRev->startStrikeEditFinishedAction();
    }
    else if(algo_type==BFLY_BID_TYPE){
        algoBtFlyBid->startStrikeEditFinishedAction();
    }
//    else if(algo_type=="BOX"){
//        algoBOX->instrumentEditFinishedAction();
//    }
//    else if (algo_type=="Open-BOX"){
//        algoOPEN_BOX->instrumentEditFinishedAction();
//    }
//    else if(algo_type=="Open-BFLY"){
//        algoOpen_BtFly->startStrikeEditFinishedAction();
//    }
}


void ConvertAlgo_Win::on_pushButtonAdd_clicked()
{
    QString algo_type  = ui->comboBox_AlgoType->currentText();
    if(algo_type=="F2F"){
        algoF2F->generateAlgo();
    }

    /************BFLY*************************************/
    else if(algo_type=="BFLY"){
        algoBtFly->generateAlgo();
    }
    /************************BFLY*************************************/

    else if(algo_type=="CON-REV"){
        algoConRev->generateAlgo();

    }
    else if(algo_type==BFLY_BID_TYPE){
        algoBtFlyBid->generateAlgo();
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

    resetTableWidget();
    sharedData->algo_data_list.clear();

    ui->lineEdit_Start_strike->clear();
    ui->lineEdit_EndStrike->clear();
    ui->lineEdit_StrikeDifference->clear();
    ui->lineEdit_Start_strike->setPlaceholderText("Start Strike");
    ui->lineEdit_EndStrike->setPlaceholderText("End Strike");

    ui->lineEdit_Fut->clear();

//    ui->mkt_qty1->setValue(1);
//    ui->mkt_qty2->setValue(1);
//    ui->mkt_qty3->setValue(1);
//    ui->mkt_qty4->setValue(1);

    if(algoType=="F2F"){

        ui->lineEdit_StrikeDifference->setVisible(false);
        ui->lineEdit_Fut->setVisible(false);
        //  ui->lineEdit_EndStrike->setVisible(false);
//        ui->qty_widget_parent->setVisible(false);
//        ui->label_qty_widget_parent->setVisible(false);

        algoF2F->selectedAction();

    }
    if(algoType=="BFLY"){
        ui->lineEdit_StrikeDifference->setVisible(true);
        ui->lineEdit_EndStrike->setVisible(true);
        ui->lineEdit_Fut->setVisible(false);
//        ui->qty_widget_parent->setVisible(false);
//        ui->label_qty_widget_parent->setVisible(false);
        algoBtFly->selectedAction();
    }
    else if(algoType=="CON-REV"){
        ui->lineEdit_Fut->setVisible(true);
        ui->lineEdit_EndStrike->setVisible(true);
        ui->lineEdit_StrikeDifference->setVisible(false);
//        ui->qty_widget_parent->setVisible(false);
//        ui->label_qty_widget_parent->setVisible(false);
        algoConRev->selectedAction();
    }
    if(algoType==BFLY_BID_TYPE){
        ui->lineEdit_StrikeDifference->setVisible(true);
        ui->lineEdit_EndStrike->setVisible(true);
        ui->lineEdit_Fut->setVisible(false);
        //        ui->qty_widget_parent->setVisible(false);
        //        ui->label_qty_widget_parent->setVisible(false);
        algoBtFlyBid->selectedAction();
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
                db_conn->logToDB(QString("Portfolio Added"));
                sharedData->algo_data_list[i].uploaded=true;
                ui->tableWidget->item(tableRowIDx, ui->tableWidget->columnCount()-1)->setForeground(QBrush(QColor(0, 200, 0)));
                QString algo_type  = sharedData->algo_data_list[i].algo_type;
                if(algo_type==QString::number(PortfolioType::F2F))
                    algo_type="F2F";
                else if(algo_type==QString::number(PortfolioType::BY))
                    algo_type="BFLY";
                else if(algo_type==QString::number(PortfolioType::CR))
                    algo_type="CON-REV";
                else if(algo_type==QString::number(PortfolioType::BOX))
                    algo_type="BOX";
                else if(algo_type==QString::number(PortfolioType::OPEN_BY))
                    algo_type="Open-BFLY";
                else if(algo_type==QString::number(PortfolioType::OPEN_BOX))
                    algo_type="Open-BOX";
                else if(algo_type==QString::number(PortfolioType::BFLY_BID))
                    algo_type=BFLY_BID_TYPE;

                QString Algo_Name = algo_type+"-"+sharedData->algo_data_list[i].Algo_Name;
                QString htmlContent = "<p><span style='background-color:#B3C1DE;'>" + QTime::currentTime().toString(LOG_TIME_FORMAT) +"</span>"
                              + "<span style='color: black;'>"+sharedData->foo_user_id +
                                      " Inserted New Algo: "+ Algo_Name +"</span> </p>";

                emit display_log_text_signal(htmlContent);
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

    //ui->lineEdit_Instrument_Name->clear();
    // ui->lineEdit_algo_combination->clear();
    ui->lineEdit_Start_strike->clear();
    ui->lineEdit_EndStrike->clear();
    // ui->lineEdit_OptionType->clear();
    ui->lineEdit_StrikeDifference->clear();
    ui->lineEdit_Fut->clear();
    //ui->lineEdit_Instrument_Name->setCompleter(0);
    // ui->lineEdit_algo_combination->setCompleter(0);
    // ui->lineEdit_Start_strike->setCompleter(0);
    ui->lineEdit_EndStrike->setCompleter(0);
    ui->lineEdit_Fut->setCompleter(0);
    // ui->lineEdit_OptionType->setCompleter(0);

    //update_input_field();
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
        qDebug()<<item->data(Qt::UserRole + 1).toInt();

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


void ConvertAlgo_Win::on_lineEdit_Fut_editingFinished()
{
    algoConRev->create_AutoFillModel_StartStrike();
}




