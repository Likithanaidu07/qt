#ifndef ORDER_DETAIL_POPUP_H
#define ORDER_DETAIL_POPUP_H

#include <QDialog>
#include <QtConcurrent/QtConcurrent>

namespace Ui {
class OrderDetail_Popup;
}

class OrderDetail_Popup : public QDialog
{
    Q_OBJECT

public:
    explicit OrderDetail_Popup(QWidget *parent = nullptr);
    ~OrderDetail_Popup();
    void getData(QString user_id, QString portfolioNumber,QString PortfolioType );
signals:
    void dataLoaded(const QList<QHash<QString, QString>>& data);


public slots:
    void updateUI(const QList<QHash<QString, QString>>& data);

private:
    Ui::OrderDetail_Popup *ui;
};

#endif // ORDER_DETAIL_POPUP_H
