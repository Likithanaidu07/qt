#ifndef ORDER_DETAIL_POPUP_H
#define ORDER_DETAIL_POPUP_H

#include <QDialog>
#include <QtConcurrent/QtConcurrent>
#include "mysql_conn.h"

namespace Ui {
class OrderDetail_Popup;
}

class OrderDetail_Popup : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDetail_Popup(QWidget *parent = nullptr);
    ~OrderDetail_Popup();
    void getTradeDataFromDB(QString user_id, QString portfolioNumber,QString PortfolioType );
    void setData(PortfolioObject *p,QStringList linersData,QList<QHash<QString,QString>>  tradeData);

signals:
    void dataLoaded( QHash<QString, QString> OrderDetailsData);


public slots:
    void updateUI(QHash<QString, QString> OrderDetailsData);

private:
    Ui::OrderDetail_Popup *ui;
};

#endif // ORDER_DETAIL_POPUP_H
