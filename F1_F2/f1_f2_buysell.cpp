#include "f1_f2_buysell.h"
#include "ui_f1_f2_buysell.h"
#include "ContractDetail.h"
#include <QKeyEvent>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>
#include "QMessageBox"
F1_F2_BuySell::F1_F2_BuySell(QWidget *parent, double devicer, double decimal_precision) :
    QDialog(parent),
    ui(new Ui::F1_F2_BuySell),
    devicer(devicer),
    decimal_precision(decimal_precision)
{
    ui->setupUi(this);
    token_number = "";
    priceSingleStepValue = 0.01;
    updateLTPOnPriceInput = false;

    this->setStyleSheet("#F1_F2_BuySell { background-color: #D6FCF0; }");
    ui->tableviewBG->setStyleSheet("#tableviewBG { background-color: #FFFFFF; }");
    // Assuming 'ui->tableViewMarkerRate' is your QTableView
    setWindowFlags(Qt::Window);

    setTabOrder(ui->comboBoxBuySell,ui->lineEdit_Stockname);
    setTabOrder(ui->lineEdit_Stockname,ui->doubleSpinBox_price);
    setTabOrder(ui->doubleSpinBox_price,ui->spinBoxLot);
    setTabOrder(ui->spinBoxLot,ui->pushButtonSubmit);
    setTabOrder(ui->pushButtonSubmit,ui->comboBoxBuySell);




    const char pushButton_SS[]="border-radius: 8px;""border: 1px solid #485F6B;""background: #485F6B;""color: #FFF;""    text-align: center;""    font-size: 14px;""    font-style: normal;""    font-weight: 500;""    line-height: normal;";
    ui->pushButtonSubmit->setStyleSheet(pushButton_SS);
    QFont font=ui->pushButtonSubmit->font();
    font.setFamily("Work Sans");
    ui->pushButtonSubmit->setFont(font);

    ui->comboBoxBuySell->setStyleSheet(
                "QComboBox {"
                "    font-weight: bold;"
                "    text-align: center;"
                "}"
                "QComboBox::item {"
                "    font-weight: bold;"
                "    text-align: center;"
                "}");


    const char LineEdit_SS[]="border-radius: 6px;""border: 0.5px solid #343A40;""background: #FFF;""color: #6C757D;""font-size: 12px;""font-style: normal;""font-weight: 400;""line-height: normal;" "padding: 4px 10px 4px 10px;";
    for(auto w: {
            ui->lineEdit_Stockname}){
            w->setStyleSheet(LineEdit_SS);
            QFont font=w->font();
            font.setFamily("Work Sans");
            w->setFont(font);
    }

    ui->doubleSpinBox_price->setStyleSheet(LineEdit_SS);
    QFont font1=ui->doubleSpinBox_price->font();
    font.setFamily("Work Sans");
    ui->doubleSpinBox_price->setFont(font1);
    ui->doubleSpinBox_price->setDecimals(2);
    connect(ui->doubleSpinBox_price, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                   this, &F1_F2_BuySell::adjustValue);

    ui->spinBoxLot->setStyleSheet(LineEdit_SS);
    QFont font2=ui->spinBoxLot->font();
    font.setFamily("Work Sans");
    ui->spinBoxLot->setFont(font2);


    model_stock_name = new QStandardItemModel;

    //model_stock_name =  ContractDetail::getInstance().Get_model_F1_F2();

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // to make floating window
    stockNameListView = new QListView(this);


    //stockNameListView->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);

    connect(stockNameListView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemSelectedStockName(QModelIndex)));
    stockNameListView->setSizePolicy(sizePolicy);
    stockNameListView->setFixedSize(230, 200);
    stockNameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    stockNameListView->hide();

    // Install event filter on the QLineEdit and QListView
    eventFilterstockName = new EventFilter(ui->lineEdit_Stockname, stockNameListView);
    connect(eventFilterstockName, SIGNAL(signalItemSelected(QModelIndex)), this, SLOT(itemSelectedStockName(QModelIndex)));
    ui->lineEdit_Stockname->installEventFilter(eventFilterstockName);
    stockNameListView->installEventFilter(eventFilterstockName);

    CustomSearchWidget *startstrikeCustomWidget = new CustomSearchWidget(stockNameListView,model_stock_name);
    connect(ui->lineEdit_Stockname, SIGNAL(textEdited(QString)),startstrikeCustomWidget, SLOT(filterItems(QString)));
    connect(ui->lineEdit_Stockname, SIGNAL(textChanged(QString)),this, SLOT(slotstockNameListViewHide(QString)));



    //QPoint globalPos = this->mapToGlobal(QPoint(0, 0));

    // Calculate the new position for stockNameListView
   // int x = globalPos.x();
   // int y = globalPos.y() + ui->lineEdit_Stockname->height();



   // stockNameListView->move(x+100, y+65);


    ui->comboBoxBuySell->setEnabled(false);
    ui->lineEdit_Stockname->setEnabled(false);
    ui->doubleSpinBox_price->setEnabled(false);
    ui->spinBoxLot->setEnabled(false);
    ui->pushButtonSubmit->setEnabled(false);


    QFuture<void> future = QtConcurrent::run([=]() {
    QElapsedTimer timer1;
    timer1.start();
        QStringList F1_F2_Tokens = ContractDetail::getInstance().Get_Tokens_For_PortfolioType(PortfolioType::F1_F2);
        qDebug()<<"F1_F2_Tokens size: "<<F1_F2_Tokens.size();
        for(int i=0;i<F1_F2_Tokens.length();i++){

            /**********Create model for F1_F2*************************/
            contract_table contract = ContractDetail::getInstance().GetDetail(F1_F2_Tokens[i].toInt());

            unsigned int unix_time= contract.Expiry;
//            QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//            dt = dt.addYears(10);
//            int targetYear = dt.date().year();
//            bool isLeapYear = QDate::isLeapYear(targetYear);

//            // If it is a leap year, and the date is after Feb 29, subtract one day
//            if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//                dt = dt.addDays(-1);
//            }
            QDateTime dt;
            dt.setOffsetFromUtc(0);
            dt.setDate(QDate(1980, 1, 1));
            dt.setTime(QTime(0, 0, 0));
            dt = dt.addSecs(unix_time);
            QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_time);
            dt1 = dt1.addYears(10);
            QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
            dt.setDate(newDate);
            QString Expiry=dt.toString("MMM dd yyyy").toUpper();
            QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer,'f',decimal_precision)+" "+contract.OptionType;
            QStandardItem *itemF1_F2 = new QStandardItem;
            itemF1_F2->setText(algo_combination);
            itemF1_F2->setData(contract.TokenNumber, Qt::UserRole + 1);
            // Set the composite key as data for sorting
             QString compositeKey = contract.InstrumentName + "-" + dt.toString("yyyyMMdd");
            itemF1_F2->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
           // model_stock_name->appendRow(itemF1_F2);
            QMetaObject::invokeMethod(this, [=]() {
                model_stock_name->appendRow(itemF1_F2);
            }, Qt::QueuedConnection);


            /********************************************************************/
        }

        QMetaObject::invokeMethod(ui->progressBar, "setVisible", Qt::QueuedConnection, Q_ARG(bool, false));
        // Ensure each UI element update happens on the UI thread
        QMetaObject::invokeMethod(ui->comboBoxBuySell, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(ui->lineEdit_Stockname, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(ui->doubleSpinBox_price, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(ui->spinBoxLot, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
        QMetaObject::invokeMethod(ui->pushButtonSubmit, "setEnabled", Qt::QueuedConnection, Q_ARG(bool, true));

        qDebug() << "F1_F2 model creation took  Time:" << timer1.elapsed() << "milliseconds";

        // Close the progress dialog once done
    });




    // initlaize marker rate table view

    QStandardItemModel *model = new QStandardItemModel(this);
    ui->tableViewMarkerRate->setModel(model);

    // Set up the headers if needed
    model->setHorizontalHeaderLabels({"Price", "Qty","Price", "Qty"});
    ui->tableViewMarkerRate->horizontalHeader()->setDefaultAlignment(Qt::AlignRight);
    QHeaderView* header = ui->tableViewMarkerRate->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);  // Disable resizing for all columns
    ui->tableViewMarkerRate->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #FFFFFF;"  // Transparent background
        "    font-weight: bold;"              // Bold font
        "    border: none;"
        "}"
    );


    // Disable scrollbars (both horizontal and vertical)
       ui->tableViewMarkerRate->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       ui->tableViewMarkerRate->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

       // Set fixed width for each column
       int columnCount = ui->tableViewMarkerRate->model()->columnCount();
       for (int col = 0; col < columnCount; ++col) {
           ui->tableViewMarkerRate->setColumnWidth(col, 75);  // Set column width to 100 (adjust as necessary)
       }

       // Optionally resize to fit contents (use this if column widths should adapt to content)
       // ui->tableViewMarkerRate->resizeColumnsToContents();

       // Ensure the table view adjusts its size to the columns and rows
       ui->tableViewMarkerRate->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);


       ui->tableViewMarkerRate->setStyleSheet("#tableViewMarkerRate { background: #FFFFFF; border: none; }");
      // QPalette palette = ui->tableViewMarkerRate->palette();
       // Set custom colors for alternating rows
     //  palette.setColor(QPalette::Base, QColor(200, 200, 200));        // Normal row color (white)
     //  palette.setColor(QPalette::AlternateBase, QColor(230, 230, 230)); // Alternating row color (light gray)

       // Apply the palette to the table view
     //  ui->tableViewMarkerRate->setPalette(palette);

       // Enable alternating row colors
    //   ui->tableViewMarkerRate->setAlternatingRowColors(true);



}

void F1_F2_BuySell::showEvent(QShowEvent *event)
{
   QDialog::showEvent(event);

   // Get the global position of the bottom-left corner of lineEdit_Stockname
      QPoint globalPos = ui->lineEdit_Stockname->mapToGlobal(QPoint(10, ui->lineEdit_Stockname->height()+4));

      // Convert the global position to the dialog's coordinate system
      QPoint posSS = this->mapFromGlobal(globalPos);

      // Move the stockNameListView to be directly below lineEdit_Stockname
      stockNameListView->move(posSS);
}

void F1_F2_BuySell::itemSelectedStockName(QModelIndex index)
{
    token_number ="";
    if(index.isValid())
    {

        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewMarkerRate->model());
        model->removeRows(0, model->rowCount());
        ui->label_lastTradedPrice->clear();


        updateLTPOnPriceInput = true;
        QVariant dData = index.data(Qt::DisplayRole);
        QVariant data = index.data( Qt::UserRole + 1);
        token_number = data.toString();

        slowData.addLeg_n_token(token_number);

        ui->lineEdit_Stockname->setText(index.data(Qt::DisplayRole).toString());
        contract_table c = ContractDetail::getInstance().GetDetail(token_number.toInt());
        ui->doubleSpinBox_price->setMinimum(c.OperatingRangeslowPriceRange/devicer);
        ui->doubleSpinBox_price->setMaximum(c.OperatingRangeshighPriceRange/devicer);
        ui->doubleSpinBox_price->setValue(ui->doubleSpinBox_price->minimum());

        ui->spinBoxLot->setMinimum(1);
        ui->spinBoxLot->setMaximum(c.VolumeFreezeQty/c.LotSize);
        ui->spinBoxLot->setValue(ui->spinBoxLot->minimum());

        if(c.MinimumSpread == 5)
            priceSingleStepValue = 0.05;
        else if(c.MinimumSpread == 1)
            priceSingleStepValue = 0.01;

        ui->doubleSpinBox_price->setSingleStep(priceSingleStepValue);   // Default increment step

        //qDebug()<<"F_F2--> token_number: "<<token_number<<"   OperatingRangeslowPriceRange: "<<c.OperatingRangeslowPriceRange/devicer<<"   OperatingRangeshighPriceRange:"<<c.OperatingRangeshighPriceRange/devicer;
        //qDebug()<<"F_F2--> token_number: "<<token_number<<"   VolumeFreezeQty: "<<c.VolumeFreezeQty<<"   LotSize:"<<c.LotSize << "  maxLot: "<<c.VolumeFreezeQty/c.LotSize << "  MinimumSpread: "<<c.MinimumSpread;
        refreshMarketDataTable();


    }
    else
    {
        qInfo() << "Not a start strike vaild index";

    }
    stockNameListView->hide();
}


void F1_F2_BuySell::adjustValue(){
   // updateLTPOnPriceInput = false;
    // Ensure the value is a multiple of the current step (either 0.01 or 0.05)
           double step = ui->doubleSpinBox_price->singleStep();
           double value = ui->doubleSpinBox_price->value();
           double newValue = qRound(value / step) * step;

           if (newValue != value) {
               ui->doubleSpinBox_price->blockSignals(true);  // Prevent recursive signal emission
               ui->doubleSpinBox_price->setValue(newValue);
               ui->doubleSpinBox_price->blockSignals(false);
           }
}


F1_F2_BuySell::~F1_F2_BuySell()
{
    model_stock_name->clear();
    delete model_stock_name;
    stockNameListView->hide();
    delete stockNameListView;
    delete ui;
}

void F1_F2_BuySell::on_pushButtonSubmit_clicked()
{
    if(ui->lineEdit_Stockname->text()==""){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Cannot Add .");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please fill all the details.");
        msgBox.exec();
        return;
    }
    if(token_number==""){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Cannot Add .");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("The selected Stock name is not valid !.");
        msgBox.exec();
        return;
    }

    if(ui->doubleSpinBox_price->value()==0){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Cannot Add ");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Price value cannot be 0 !.");
        msgBox.exec();
        return;
    }


    QString sellprice="0";
    QString sellqty="0";
    QString buyprice="0";
    QString buyqty="0";
    QString msg="0";
    QString orderQunatity = "0";


    int lotSize=ContractDetail::getInstance().GetLotSize(token_number.toInt(),PortfolioType::F1_F2);

    QString confirmationMsg ="";
    if(buy_mode){
        buyprice = QString::number(ui->doubleSpinBox_price->value()* devicer);
        buyqty = QString::number(ui->spinBoxLot->value()* lotSize);
        orderQunatity = buyqty;

        confirmationMsg ="Buy "+ui->lineEdit_Stockname->text()+" @ "+QString::number(ui->doubleSpinBox_price->value())+" for "+QString::number(ui->spinBoxLot->value())+" lot. Would you like to proceed?";
    }
    else{
        sellprice = QString::number(ui->doubleSpinBox_price->value()* devicer);
        sellqty = QString::number(ui->spinBoxLot->value()* lotSize);
        orderQunatity = sellqty;
        confirmationMsg ="Sell "+ui->lineEdit_Stockname->text()+" @ "+QString::number(ui->doubleSpinBox_price->value())+" for "+QString::number(ui->spinBoxLot->value())+" lot. Would you like to proceed?";

    }


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Add Manual Order?", confirmationMsg,QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        mysql_conn *db_conn = new mysql_conn(0, "add_algo_db_conn");
        algo_data_insert_status status = db_conn->place_F1F2_Order(QString::number(userData.UserId),token_number, sellprice, sellqty, buyprice, buyqty,userData.MaxPortfolioCount,orderQunatity,msg,true);
        if(status == algo_data_insert_status::EXIST){
             QMessageBox::StandardButton reply;
             reply = QMessageBox::question(this, "Duplicate record!", "Record already exist for selected Stock Name!. Do you want to proceed?",QMessageBox::Yes | QMessageBox::No);
             if (reply == QMessageBox::Yes) {
                 algo_data_insert_status status1 = db_conn->place_F1F2_Order(QString::number(userData.UserId),token_number, sellprice, sellqty, buyprice, buyqty,userData.MaxPortfolioCount,orderQunatity,msg,false);
                 if(status1 == algo_data_insert_status::INSERTED){
                         emit portfolioAddedSignal();
                         QMessageBox msgBox;
                         msgBox.setWindowTitle("Success");
                         msgBox.setIcon(QMessageBox::Information);
                         msgBox.setText("Order placed successfully.");
                         msgBox.exec();
                         ui->lineEdit_Stockname->clear();
                         //ui->spinBoxLot->setValue(ui->spinBoxLot->minimum());
                         ui->spinBoxLot->clear();
                         //ui->doubleSpinBox_price->setValue(ui->doubleSpinBox_price->minimum());
                         ui->doubleSpinBox_price->clear();

                         QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewMarkerRate->model());
                         model->removeRows(0, model->rowCount());
                         ui->label_lastTradedPrice->clear();

                         token_number = "";
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
            emit portfolioAddedSignal();
            QMessageBox msgBox;
            msgBox.setWindowTitle("Success");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText("Order placed successfully.");
            msgBox.exec();
            ui->lineEdit_Stockname->clear();
            //ui->spinBoxLot->setValue(ui->spinBoxLot->minimum());
            //ui->doubleSpinBox_price->setValue(ui->doubleSpinBox_price->minimum());
            //ui->spinBoxLot->setValue(ui->spinBoxLot->minimum());
            ui->spinBoxLot->clear();
            //ui->doubleSpinBox_price->setValue(ui->doubleSpinBox_price->minimum());
            ui->doubleSpinBox_price->clear();

            QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewMarkerRate->model());
            model->removeRows(0, model->rowCount());

            token_number = "";
            ui->label_lastTradedPrice->clear();

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

void F1_F2_BuySell::slotstockNameListViewHide(QString)
{
    stockNameListView->hide();
}



void F1_F2_BuySell::on_comboBoxBuySell_currentTextChanged(const QString &arg1)
{
     if(arg1=="Buy"){
        buy_mode = true;
       // this->setWindowTitle("F1: BuyManual");
        this->setStyleSheet("#F1_F2_BuySell { background-color: #D6FCF0; }");
        ui->tableViewMarkerRate->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "    background-color: #FFFFFF;"  // Transparent background
            "    font-weight: bold;"              // Bold font
            "    border: none;"
            "}"
        );

     }
     else{
         buy_mode = false;
         // this->setWindowTitle("F2: SellManual");
         this->setStyleSheet("#F1_F2_BuySell { background-color: #FED9D9; }");
         ui->tableViewMarkerRate->horizontalHeader()->setStyleSheet(
             "QHeaderView::section {"
             "    background-color: #FFFFFF;"  // Transparent background
             "    font-weight: bold;"              // Bold font
             "    border: none;"                              // Remove header section borders

             "}"
         );

     }
}
void F1_F2_BuySell::setBuyMode(bool buy_mode_){
    if(buy_mode_){
        ui->comboBoxBuySell->setCurrentText("Buy");
        buy_mode = true;
        this->setStyleSheet("#F1_F2_BuySell { background-color: #D6FCF0; }");
        ui->tableViewMarkerRate->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "    background-color: #FFFFFF;"  // Transparent background
            "    font-weight: bold;"              // Bold font
            "    border: none;"
            "}"
        );

    }
    else{
        ui->comboBoxBuySell->setCurrentText("Sell");
        buy_mode = false;
        this->setStyleSheet("#F1_F2_BuySell { background-color: #FED9D9; }");
        ui->tableViewMarkerRate->horizontalHeader()->setStyleSheet(
            "QHeaderView::section {"
            "    background-color: #FFFFFF;"  // Transparent background
            "    font-weight: bold;"              // Bold font
            "    border: none;"
            "}"
        );

    }
}

void F1_F2_BuySell::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){
        ui->lineEdit_Stockname->clear();
        ui->spinBoxLot->clear();
        ui->doubleSpinBox_price->clear();
        event->accept();                    // Accept the event to indicate it was handled
    }
    else if(event->key() == Qt::Key_F1){
        ui->comboBoxBuySell->setCurrentIndex(0);
    }
    else if(event->key() == Qt::Key_F2){
        ui->comboBoxBuySell->setCurrentIndex(1);
    }

    else{
        QWidget::keyPressEvent(event); // Pass other key events to the base class
    }
}

void F1_F2_BuySell::slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct>& data){

    mutex.lock();
    localMBP_Data = data;             // Assign the received data to the class variable
    mutex.unlock();

    refreshMarketDataTable();

   /*QStringList CR_Tokens = ContractDetail::getInstance().Get_Tokens_For_PortfolioType(PortfolioType::CR);

    for (auto it = data.keyBegin(); it != data.keyEnd(); ++it) {
            bool ok;
            // Convert the key (QString) to an integer
            int tokenInt = it->toInt(&ok);  // Use toInt() with error checking

            if (ok) {
                // If conversion was successful, fetch the contract details
                if(ContractDetail::getInstance().checkTokenExist(tokenInt)){
                    if(CR_Tokens.contains(*it)){
                        qDebug()<<"token exist in CR_Tokens Too: "<<*it;
                        contract_table contract = ContractDetail::getInstance().GetDetail(tokenInt);

                        unsigned int unix_time= contract.Expiry;
                        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
                        dt = dt.addYears(10);
                        int targetYear = dt.date().year();
                        bool isLeapYear = QDate::isLeapYear(targetYear);

                        // If it is a leap year, and the date is after Feb 29, subtract one day
                        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
                            dt = dt.addDays(-1);
                        }
                        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
                        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/devicer,'f',decimal_precision)+" "+contract.OptionType;
                        qDebug()<<"algo_combination: "<<algo_combination;

                    }
                    else{
                        qDebug()<<"token not exist in CR_Tokens: "<<*it;
                    }
                }
                else{
                    qDebug()<<"-------slowDataRecv_Slot: token not exist in Contract: "<<*it;

                }

                // Print the token and any additional information you need
            } else {
                // Handle the case where the key is not a valid integer
                qDebug() << "Invalid token key, unable to convert to int:" << *it;
            }
        }*/
}

void F1_F2_BuySell::refreshMarketDataTable(){

    if(token_number!=""){
        QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableViewMarkerRate->model());
        mutex.lock();
        if(localMBP_Data.contains(token_number)){
            model->removeRows(0, model->rowCount());
            MBP_Data_Struct d = localMBP_Data[token_number];
            mutex.unlock();


//            for(int i=0;i<5;i++){
//                QList<QStandardItem *> rowData;
//                //buy
//                rowData.append(new QStandardItem(d.recordBuffer[i].price));
//                rowData.append(new QStandardItem(d.recordBuffer[i].quantity));
//                //sell
//                rowData.append(new QStandardItem(d.recordBuffer[i+5].price));
//                rowData.append(new QStandardItem(d.recordBuffer[i+5].quantity));
//                model->appendRow(rowData);  // Add new row
//            }

            double ltp = d.lastTradedPrice.toDouble()/devicer;
            ui->label_lastTradedPrice->setText(QString::number(ltp,'f',2));
            if(updateLTPOnPriceInput){
                ui->doubleSpinBox_price->setValue(ltp);
                qDebug()<<" setting doubleSpinBox_price:"<<ltp;
                if(ltp>ui->doubleSpinBox_price->maximum()){
                    QMessageBox::information(this, "Warning", "LTP: ",QString::number(ltp)+" is greater than HighPriceRange: "+QString::number(ui->doubleSpinBox_price->maximum())+" Toekn = "+token_number);

                }

                updateLTPOnPriceInput = false;
            }

            for (int i = 0; i < 5; i++) {
                QList<QStandardItem *> rowData;

                // Buy - Set text alignment to right
                QStandardItem *buyPriceItem = new QStandardItem(QString::number(d.recordBuffer[i].price.toDouble()/devicer,'f',2));
                buyPriceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                buyPriceItem->setForeground(QBrush(QColor("#4184f3")));
                QStandardItem *buyQuantityItem = new QStandardItem(QString::number(d.recordBuffer[i].quantity.toDouble(),'f',0));
                buyQuantityItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                buyQuantityItem->setForeground(QBrush(QColor("#4184f3")));


                // Sell - Set text alignment to right
                QStandardItem *sellPriceItem = new QStandardItem(QString::number(d.recordBuffer[i+5].price.toDouble()/devicer,'f',2));
                sellPriceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                sellPriceItem->setForeground(QBrush(QColor("#ff5722")));

                QStandardItem *sellQuantityItem = new QStandardItem(QString::number(d.recordBuffer[i+5].quantity.toDouble(),'f',0));
                sellQuantityItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
                sellQuantityItem->setForeground(QBrush(QColor("#ff5722")));

                // Append items to row
                rowData.append(buyPriceItem);
                rowData.append(buyQuantityItem);
                rowData.append(sellPriceItem);
                rowData.append(sellQuantityItem);

                // Add the row to the model
                model->appendRow(rowData);
            }


        }
        else{
            mutex.unlock();
        }
    }

}





