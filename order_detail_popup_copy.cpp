#include "order_detail_popup.h"
#include "ui_order_detail_popup.h"
#include "mysql_conn.h"
OrderDetail_Popup::OrderDetail_Popup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OrderDetail_Popup)
{
    ui->setupUi(this);



}
void OrderDetail_Popup::getData(QString user_id, QString portfolioNumber,QString PortfolioType){
    mysql_conn *db_conn = new mysql_conn(0,"login_conn");

    QList<QHash<QString,QString>> data =  db_conn->getTradePopUPData(user_id, portfolioNumber,PortfolioType);
    for(int i=0;i<data.length();i++){
        if(data[i]["Buy_Sell"]=="Buy"){
            QColor color("#D6FCF0");

            ui->tableWidget_Buy->insertRow( ui->tableWidget_Buy->rowCount());
            QTableWidgetItem *c0 = new QTableWidgetItem();
            //c0->setData(Qt::UserRole + 1,data[i]["Exch_Price"]);
            c0->setData(Qt::DisplayRole,data[i]["Exch_Price"]);
            c0->setFlags(c0->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 0, c0);

            QTableWidgetItem *c1 = new QTableWidgetItem();
            c1->setData(Qt::DisplayRole,data[i]["User_Price"]);
            c1->setFlags(c1->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 1, c1);

            QTableWidgetItem *c2 = new QTableWidgetItem();
            c2->setData(Qt::DisplayRole,data[i]["Jackpot"]);
            c2->setFlags(c2->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 2, c2);

            QTableWidgetItem *c3 = new QTableWidgetItem();
            c3->setData(Qt::DisplayRole,data[i]["Traded_Lot"]);
            c3->setFlags(c3->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 3, c3);

            QTableWidgetItem *c4 = new QTableWidgetItem();
            c4->setData(Qt::DisplayRole,data[i]["Remaining_Lot"]);
            c4->setFlags(c4->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 4, c4);

            QTableWidgetItem *c5 = new QTableWidgetItem();
            c5->setData(Qt::DisplayRole,data[i]["Time"]);
            c5->setFlags(c5->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Buy->setItem(ui->tableWidget_Buy->rowCount()-1, 5, c5);
        }
        else{
            ui->tableWidget_Sell->insertRow( ui->tableWidget_Sell->rowCount());
            QTableWidgetItem *c0 = new QTableWidgetItem();
            //c0->setData(Qt::UserRole + 1,data[i]["Exch_Price"]);
            c0->setData(Qt::DisplayRole,data[i]["Exch_Price"]);
            c0->setFlags(c0->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 0, c0);

            QTableWidgetItem *c1 = new QTableWidgetItem();
            c1->setData(Qt::DisplayRole,data[i]["User_Price"]);
            c1->setFlags(c1->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 1, c1);

            QTableWidgetItem *c2 = new QTableWidgetItem();
            c2->setData(Qt::DisplayRole,data[i]["Jackpot"]);
            c2->setFlags(c2->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 2, c2);

            QTableWidgetItem *c3 = new QTableWidgetItem();
            c3->setData(Qt::DisplayRole,data[i]["Traded_Lot"]);
            c3->setFlags(c3->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 3, c3);

            QTableWidgetItem *c4 = new QTableWidgetItem();
            c4->setData(Qt::DisplayRole,data[i]["Remaining_Lot"]);
            c4->setFlags(c4->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 4, c4);

            QTableWidgetItem *c5 = new QTableWidgetItem();
            c5->setData(Qt::DisplayRole,data[i]["Time"]);
            c5->setFlags(c5->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_Sell->setItem(ui->tableWidget_Sell->rowCount()-1, 5, c5);
        }
    }

    delete db_conn;
}

OrderDetail_Popup::~OrderDetail_Popup()
{
    delete ui;
}
