#include "order_detail_popup.h"
#include "ui_order_detail_popup.h"

OrderDetail_Popup::OrderDetail_Popup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderDetail_Popup)
{
    ui->setupUi(this);
    //setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    //Qt::WindowFlags flags = this->windowFlags();
  // this->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
    // Set window flags
      Qt::WindowFlags flags = this->windowFlags();
      this->setWindowFlags(flags | Qt::Dialog );

      // Ensure the window is non-modal
      this->setWindowModality(Qt::NonModal);

      connect(this, &OrderDetail_Popup::dataLoaded, this, &OrderDetail_Popup::updateUI);

      QString groupBoxStyle = "QGroupBox {"
                                "background-color: #E9ECEF;"
                              "}";
      ui->groupBox_2->setStyleSheet(groupBoxStyle);
      this->setStyleSheet(
          "QLabel { "
          "    border-bottom: 1px solid black; "  // Set border width and color
          "    border-radius: 1px; "    // Optional: Round the corners
          "    padding: 5px; "          // Optional: Add padding
          "}"
      );

      // Set horizontal header to resize columns to fit the available width
      ui->tableWidget_Buy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
      ui->tableWidget_Sell->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

      // Optionally, set vertical headers to resize to fit content
      ui->tableWidget_Buy->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
      ui->tableWidget_Sell->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);



      QString styleSheet = R"(
    QTableWidget {
        background-color: #E9ECEF;
        alternate-background-color: #D6FCF0; /* light green with transparency */
        gridline-color: transparent; /* Make gridlines invisible */
        font: bold;
    }
    QHeaderView::section {
        background-color: #505050; /* dark grey */
        color: white;
        padding: 1px;
        border: 1px solid #FFF;
    }
    QTableWidget::item {
        background-color: rgba(0, 255, 0, 0.1); /* light green with transparency */
        border-bottom: 1px solid black; /* Only a bottom border for horizontal lines */
        border-right: none; /* No vertical borders */
        border-left: none; /* No vertical borders */
    }
    QTableWidget::item:selected {
        background-color: #D6FCF0; /* selected color, slightly darker */
        color: black; /* black text color when selected */
        border-bottom: 1px solid black; /* Keep the bottom border for selected items */
        border-right: none; /* No vertical borders for selected */
        border-left: none; /* No vertical borders for selected */
    }
)";
      ui->tableWidget_Buy->setStyleSheet(styleSheet);


      QString styleSheet1 = R"(
    QTableWidget {
        background-color: #E9ECEF;
        alternate-background-color: #FED9D9;
         gridline-color: transparent;
        font: bold;
    }
    QHeaderView::section {
        background-color: #505050;
        color: white;
        padding: 1px;
        border: 1px solid #FFF;
    }
    QTableWidget::item {
        background-color: #FED9D9;
        border-bottom: 1px solid black; /* horizontal line only */
        border-right: none; /* No vertical borders */
        border-left: none; /* No vertical borders */
    }
    QTableWidget::item:selected {
        background-color: #FED9D9;
        color: black;
        border-bottom: 1px solid black; /* Keep the bottom




border for selected items */
        border-right: none; /* No vertical borders for selected */
        border-left: none; /* No vertical borders for selected */
    }
)";


    ui->tableWidget_Sell->setStyleSheet(styleSheet1);
}

void OrderDetail_Popup::setData(PortfolioObject *p,QStringList linersData,QList<QHash<QString,QString>>  tradeData){
   ui->algo_id->setText(QString::number(p->PortfolioNumber));
   ui->algo_name->setText(p->AlgoName);
   ui->bid_leg->setText(p->BidLeg);
   ui->buy_qty->setText(QString::number(p->BuyTradedQuantity));
   ui->sell_qty->setText(QString::number(p->SellTradedQuantity));
   ui->buy_avg->setText(p->BuyAveragePrice);
   ui->sell_avg->setText(p->SellAveragePrice);
   ui->lo_size->setText(QString::number(p->Leg1LotSizeOrg));

   if(linersData.size()>0){
    ui->net_qty->setText(linersData[Liners_Idx::NetQty]);
    //ui->p_l->setText(linersData[Liners_Idx::Profit]);
   }
   else{
       ui->net_qty->setText("-");
       ui->p_l->setText("-");
   }


   for(int i=0;i<tradeData.length();i++){
       if(tradeData[i]["Buy_Sell"]=="Buy"){
           ui->tableWidget_Buy->insertRow( ui->tableWidget_Buy->rowCount());
           QTableWidgetItem *c0 = new QTableWidgetItem();
           //c0->setData(Qt::UserRole + 1,data[i]["Exch_Price"]);
           c0->setData(Qt::DisplayRole,tradeData[i]["Exch_Price"]);
           c0->setTextAlignment(Qt::AlignCenter);
           c0->setFlags(c0->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 0, c0);

           QTableWidgetItem *c1 = new QTableWidgetItem();
           c1->setData(Qt::DisplayRole,tradeData[i]["User_Price"]);
           c1->setTextAlignment(Qt::AlignCenter);
           c1->setFlags(c1->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 1, c1);

           QTableWidgetItem *c2 = new QTableWidgetItem();
           c2->setData(Qt::DisplayRole,tradeData[i]["Jackpot"]);
           c2->setTextAlignment(Qt::AlignCenter);
           c2->setFlags(c2->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 2, c2);

           QTableWidgetItem *c3 = new QTableWidgetItem();
           c3->setData(Qt::DisplayRole,tradeData[i]["Traded_Lot"]);
           c3->setTextAlignment(Qt::AlignCenter);
           c3->setFlags(c3->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 3, c3);

           QTableWidgetItem *c4 = new QTableWidgetItem();
           c4->setData(Qt::DisplayRole,tradeData[i]["Remaining_Lot"]);
           c4->setTextAlignment(Qt::AlignCenter);
           c4->setFlags(c4->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 4, c4);

           QTableWidgetItem *c5 = new QTableWidgetItem();
           c5->setData(Qt::DisplayRole,tradeData[i]["Time"]);
           c5->setTextAlignment(Qt::AlignCenter);
           c5->setFlags(c5->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 5, c5);
       }
       else{
           ui->tableWidget_Sell->insertRow( ui->tableWidget_Sell->rowCount());
           QTableWidgetItem *c0 = new QTableWidgetItem();
           //c0->setData(Qt::UserRole + 1,data[i]["Exch_Price"]);
           c0->setData(Qt::DisplayRole,tradeData[i]["Exch_Price"]);
           c0->setTextAlignment(Qt::AlignCenter);
           c0->setFlags(c0->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 0, c0);

           QTableWidgetItem *c1 = new QTableWidgetItem();
           c1->setData(Qt::DisplayRole,tradeData[i]["User_Price"]);
           c1->setTextAlignment(Qt::AlignCenter);
           c1->setFlags(c1->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 1, c1);

           QTableWidgetItem *c2 = new QTableWidgetItem();
           c2->setData(Qt::DisplayRole,tradeData[i]["Jackpot"]);
           c2->setTextAlignment(Qt::AlignCenter);
           c2->setFlags(c2->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 2, c2);

           QTableWidgetItem *c3 = new QTableWidgetItem();
           c3->setData(Qt::DisplayRole,tradeData[i]["Traded_Lot"]);
           c3->setTextAlignment(Qt::AlignCenter);
           c3->setFlags(c3->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 3, c3);

           QTableWidgetItem *c4 = new QTableWidgetItem();
           c4->setData(Qt::DisplayRole,tradeData[i]["Remaining_Lot"]);
           c4->setTextAlignment(Qt::AlignCenter);
           c4->setFlags(c4->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 4, c4);

           QTableWidgetItem *c5 = new QTableWidgetItem();
           c5->setData(Qt::DisplayRole,tradeData[i]["Time"]);
           c5->setTextAlignment(Qt::AlignCenter);
           c5->setFlags(c5->flags() & ~Qt::ItemIsEditable);
           ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 5, c5);
       }
   }


}

void OrderDetail_Popup::getTradeDataFromDB(QString user_id, QString portfolioNumber,QString PortfolioType){
    ui->tableWidget_Buy->setRowCount(0);
    ui->tableWidget_Sell->setRowCount(0);


    auto loadDataFromDB_BackgroundTask = [this, user_id, portfolioNumber, PortfolioType]() {
        // Capture captured variables by value to ensure thread safety
        mysql_conn *db_conn = new mysql_conn(0, "get_popup_data_conn");
        //QList<QHash<QString, QString>> data = db_conn->getOrderPopUPData(user_id, portfolioNumber, PortfolioType);

        QHash<QString, QString> OrderDetailsData = db_conn->getOrderDetailsData(portfolioNumber,user_id);


        delete db_conn; // Release memory after use
        emit dataLoaded(OrderDetailsData);

    };

    QFuture<void> future = QtConcurrent::run(loadDataFromDB_BackgroundTask);

}


void OrderDetail_Popup::updateUI(QHash<QString, QString>OrderDetailsData){




    ui->buy_value->setText("-");
    ui->sell_value->setText("-");
    ui->total_order->setText("-");
    ui->executed_order->setText("-");

    if(OrderDetailsData.contains("BuyTotalVal")){
      ui->buy_value->setText(OrderDetailsData["BuyTotalVal"]);
    }
    if(OrderDetailsData.contains("SellTotalVal")){
      ui->sell_value->setText(OrderDetailsData["SellTotalVal"]);
    }
    if(OrderDetailsData.contains("OrderCount")){
      ui->total_order->setText(OrderDetailsData["OrderCount"]);
    }
    if(OrderDetailsData.contains("TraderCount")){
      ui->executed_order->setText(OrderDetailsData["TraderCount"]);
    }




}

OrderDetail_Popup::~OrderDetail_Popup()
{

    delete ui;
}

