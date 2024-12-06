#include "trade_details_popup.h"
#include "ui_trade_details_popup.h"

TradeDetailsPopup::TradeDetailsPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TradeDetailsPopup)
{
    ui->setupUi(this);    QObject::connect(this, SIGNAL(dataLoaded(QList<QHash<QString, QString>>)), this, SLOT(updateUI(QList<QHash<QString, QString>>)));

    QString styleSheet = R"(
       QTableWidget {
           background-color: transparent;
           alternate-background-color: #F0FFFA; /* light green with transparency */
           gridline-color: #FFF;
           font: bold;
       }
       QHeaderView::section {
           background-color: #505050; /* dark grey */
           color: white;
           padding: 4px;
           border: 1px solid #FFF;
       }

       QTableWidget::item:selected {
           background-color: #D6FCF0; /* selected color, slightly darker */
           color: black; /* black text color when selected */
       }

       )";

       ui->tableWidget->setStyleSheet(styleSheet);


   }



   void TradeDetailsPopup::getTradeDataFromDB(QString user_id, QString localOrderID,int lotSize  ){
       ui->tableWidget->setRowCount(0);

       auto loadDataFromDB_BackgroundTask = [this, user_id, localOrderID,lotSize]() {
           // Capture captured variables by value to ensure thread safety
           mysql_conn *db_conn = new mysql_conn(0, "get_popup_data_conn");
           QList<QHash<QString, QString>> data = db_conn->getTradePopUPData(user_id, localOrderID,lotSize);
           delete db_conn; // Release memory after use
           emit dataLoaded(data);

       };

       QFuture<void> future = QtConcurrent::run(loadDataFromDB_BackgroundTask);

   }


   void TradeDetailsPopup::updateUI(const QList<QHash<QString, QString>>& data){
       for(int i=0;i<data.length();i++){
               QString buySell = data[i]["Buy_Sell"];
               QColor backgroundColor = (buySell == "Buy") ? QColor(0, 255, 0, 25) : QColor(255, 0, 0, 25); // Light green for Buy, Light red for Sell

               ui->tableWidget->insertRow( ui->tableWidget->rowCount());
               QTableWidgetItem *c0 = new QTableWidgetItem();
               //c0->setData(Qt::UserRole + 1,data[i]["Exch_Price"]);
               c0->setData(Qt::DisplayRole,QString::number(i+1));
               c0->setFlags(c0->flags() & ~Qt::ItemIsEditable);
               c0->setBackground(backgroundColor);
               c0->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, c0);

               QTableWidgetItem *c1 = new QTableWidgetItem();
               c1->setData(Qt::DisplayRole,data[i]["OrderId"]);
               c1->setFlags(c1->flags() & ~Qt::ItemIsEditable);
               c1->setBackground(backgroundColor);
               c1->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, c1);

               QTableWidgetItem *c2 = new QTableWidgetItem();
               c2->setData(Qt::DisplayRole,data[i]["AlgoNo"]);
               c2->setFlags(c2->flags() & ~Qt::ItemIsEditable);
               c2->setBackground(backgroundColor);
               c2->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, c2);


               QTableWidgetItem *c3 = new QTableWidgetItem();
               c3->setData(Qt::DisplayRole,data[i]["StockName"]);
               c3->setFlags(c3->flags() & ~Qt::ItemIsEditable);
               c3->setBackground(backgroundColor);
               c3->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, c3);
               ui->tableWidget->resizeColumnToContents(3);

               QTableWidgetItem *c4 = new QTableWidgetItem();
               c4->setData(Qt::DisplayRole,data[i]["Price"]);
               c4->setFlags(c4->flags() & ~Qt::ItemIsEditable);
               c4->setBackground(backgroundColor);
               c4->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4, c4);

               QTableWidgetItem *c5 = new QTableWidgetItem();
               c5->setData(Qt::DisplayRole,data[i]["Lots"]);
               c5->setFlags(c5->flags() & ~Qt::ItemIsEditable);
               c5->setBackground(backgroundColor);
               c5->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 5, c5);

               QTableWidgetItem *c6 = new QTableWidgetItem();
               c6->setData(Qt::DisplayRole,data[i]["Time"]);
               c6->setFlags(c6->flags() & ~Qt::ItemIsEditable);
               c6->setBackground(backgroundColor);
               c6->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 6, c6);

       }
        ui->tableWidget->resizeColumnsToContents();
       ui->tableWidget->setColumnWidth(3, std::max(180, ui->tableWidget->columnWidth(3)));
       ui->tableWidget->setColumnWidth(4, std::max(70, ui->tableWidget->columnWidth(4)));
 }

TradeDetailsPopup::~TradeDetailsPopup()
{
    delete ui;
}
