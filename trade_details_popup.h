#ifndef TRADE_DETAILS_POPUP_H
#define TRADE_DETAILS_POPUP_H

#include <QDialog>
#include <QtConcurrent/QtConcurrent>
#include "mysql_conn.h"

namespace Ui {
class TradeDetailsPopup;
}

class TradeDetailsPopup : public QDialog
{
    Q_OBJECT

public:
    explicit TradeDetailsPopup(QWidget *parent = nullptr);
    ~TradeDetailsPopup();
    void getTradeDataFromDB(QString user_id, QString localOrderID,int lotSize );
    void setData();

signals:
    void dataLoaded(const QList<QHash<QString, QString>>& data);

public slots:
    void updateUI(const QList<QHash<QString, QString>>& data);
private:
    Ui::TradeDetailsPopup *ui;
};

#endif // TRADE_DETAILS_POPUP_H
