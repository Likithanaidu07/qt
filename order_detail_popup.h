#ifndef ORDER_DETAIL_POPUP_H
#define ORDER_DETAIL_POPUP_H

#include <QDialog>

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

private:
    Ui::OrderDetail_Popup *ui;
};

#endif // ORDER_DETAIL_POPUP_H
