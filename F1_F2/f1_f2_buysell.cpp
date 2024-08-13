#include "f1_f2_buysell.h"
#include "ui_f1_f2_buysell.h"
#include "ContractDetail.h"

F1_F2_BuySell::F1_F2_BuySell(QWidget *parent, double devicer, double decimal_precision) :
    QDialog(parent),
    ui(new Ui::F1_F2_BuySell),
    devicer(devicer),
    decimal_precision(decimal_precision)
{
    ui->setupUi(this);
    token_number = "";

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

    ui->spinBoxLot->setStyleSheet(LineEdit_SS);
    QFont font2=ui->spinBoxLot->font();
    font.setFamily("Work Sans");
    ui->spinBoxLot->setFont(font2);




    model_stock_name =  ContractDetail::getInstance().Get_model_F1_F2();

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // to make floating window
    stockNameListView = new QListView(this);
    connect(stockNameListView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemSelectedStockName(QModelIndex)));
    stockNameListView->setSizePolicy(sizePolicy);
    stockNameListView->setFixedSize(230, 200);
    stockNameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stockNameListView->setWindowFlags(Qt::ToolTip);

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


}

void F1_F2_BuySell::showEvent(QShowEvent *event)
{
   QDialog::showEvent(event);
    ui->lineEdit_Stockname->updateGeometry();
    ui->lineEdit_Stockname->repaint();

    QPoint globalPos = ui->lineEdit_Stockname->mapToGlobal(QPoint(0, 0));
    QPoint parentPos = this->mapFromGlobal(globalPos);

    int x = globalPos.x();
    int y = globalPos.y() + ui->lineEdit_Stockname->height()+5;


    // Move stockNameListView to the new position
      stockNameListView->move(x, y);
   // stockNameListView->show();
}

void F1_F2_BuySell::itemSelectedStockName(QModelIndex index)
{
    token_number ="";
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        QVariant data = index.data( Qt::UserRole + 1);
        token_number = data.toString();
        ui->lineEdit_Stockname->setText(index.data(Qt::DisplayRole).toString());
        contract_table c = ContractDetail::getInstance().GetDetail(token_number.toInt(),PortfolioType::F1_F2);
        ui->doubleSpinBox_price->setMinimum(c.OperatingRangeslowPriceRange/devicer);
        ui->doubleSpinBox_price->setMaximum(c.OperatingRangeshighPriceRange/devicer);
        ui->spinBoxLot->setMinimum(0);
        ui->spinBoxLot->setMaximum(c.VolumeFreezeQty/c.LotSize);

        qDebug()<<"F_F2--> token_number: "<<token_number<<"   OperatingRangeslowPriceRange: "<<c.OperatingRangeslowPriceRange/devicer<<"   OperatingRangeshighPriceRange:"<<c.OperatingRangeshighPriceRange/devicer;
        qDebug()<<"F_F2--> token_number: "<<token_number<<"   VolumeFreezeQty: "<<c.VolumeFreezeQty<<"   LotSize:"<<c.LotSize << "  maxLot"<<c.VolumeFreezeQty/c.LotSize;


    }
    else
    {
        qInfo() << "Not a start strike vaild index";

    }
    stockNameListView->hide();
}



F1_F2_BuySell::~F1_F2_BuySell()
{
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
    QString sellprice="0";
    QString sellqty="0";
    QString buyprice="0";
    QString buyqty="0";
    QString msg="0";



    int lotSize=ContractDetail::getInstance().GetLotSize(token_number.toInt(),PortfolioType::F1_F2);
    if(buy_mode){
        buyprice = QString::number(ui->doubleSpinBox_price->value()* devicer);
        buyqty = QString::number(ui->spinBoxLot->value()* lotSize);
    }
    else{
        sellprice = QString::number(ui->doubleSpinBox_price->value()* devicer);
        sellqty = QString::number(ui->spinBoxLot->value()* lotSize);
    }

    mysql_conn *db_conn = new mysql_conn(0, "add_algo_db_conn");
    algo_data_insert_status status = db_conn->place_F1F2_Order(QString::number(userData.UserId),token_number, sellprice, sellqty, buyprice, buyqty,userData.MaxPortfolioCount,msg,true);
    if(status == algo_data_insert_status::EXIST){
         QMessageBox::StandardButton reply;
         reply = QMessageBox::question(this, "Duplicate record!", "Record already exist for selected Stock Name!. Do you want to proceed?",QMessageBox::Yes | QMessageBox::No);
         if (reply == QMessageBox::Yes) {
             algo_data_insert_status status1 = db_conn->place_F1F2_Order(QString::number(userData.UserId),token_number, sellprice, sellqty, buyprice, buyqty,userData.MaxPortfolioCount,msg,false);
             if(status1 == algo_data_insert_status::INSERTED){
                     QMessageBox msgBox;
                     msgBox.setWindowTitle("Success");
                     msgBox.setIcon(QMessageBox::Information);
                     msgBox.setText("Order placed successfully.");
                     msgBox.exec();
                     ui->lineEdit_Stockname->clear();
                     ui->spinBoxLot->setValue(0);
                     ui->doubleSpinBox_price->setValue(0.00);
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
        QMessageBox msgBox;
        msgBox.setWindowTitle("Success");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText("Order placed successfully.");
        msgBox.exec();
        ui->lineEdit_Stockname->clear();
        ui->spinBoxLot->setValue(0);
        ui->doubleSpinBox_price->setValue(0.00);
        token_number = "";
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

void F1_F2_BuySell::slotstockNameListViewHide(QString)
{
    stockNameListView->hide();
}



void F1_F2_BuySell::on_comboBoxBuySell_currentTextChanged(const QString &arg1)
{
     if(arg1=="Buy"){
        buy_mode = true;
       // this->setWindowTitle("F1: BuyManual");
     }
     else{
         buy_mode = false;
         // this->setWindowTitle("F2: SellManual");
     }
}
void F1_F2_BuySell::setBuyMode(bool buy_mode_){
    if(buy_mode_){
        ui->comboBoxBuySell->setCurrentText("Buy");
    }
    else{
        ui->comboBoxBuySell->setCurrentText("Sell");
    }
}

