#ifndef F1_F2_BUYSELL_H
#define F1_F2_BUYSELL_H

#include <QDialog>
#include "QStandardItemModel"
#include "QListView"
#include "Convert_to_Algo/eventfilterlistview.h"
#include "custom_q_completer.h"
#include "mysql_conn.h"
#include "SlowData/slowdata.h"


namespace Ui {
class F1_F2_BuySell;
}

class F1_F2_BuySell : public QDialog
{
    Q_OBJECT

public:
    explicit F1_F2_BuySell(QWidget *parent = nullptr, double devicer = 100.0, double decimal_precision = 4.0);
    ~F1_F2_BuySell();
    QString token_number;
    userInfo userData;
    bool buy_mode;
    void setBuyMode(bool buy_mode_);
    double priceSingleStepValue;
    bool updateLTPOnPriceInput;
    QMutex mutex;
    QHash<QString, MBP_Data_Struct> localMBP_Data;
    void refreshMarketDataTable();

      void highlightRowInF1F2Order(int orderId);
protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_pushButtonSubmit_clicked();
    void on_comboBoxBuySell_currentTextChanged(const QString &arg1);
     void keyPressEvent(QKeyEvent *event) override;



 public slots:
    void slotstockNameListViewHide(QString);
    void itemSelectedStockName(QModelIndex index);
    void adjustValue();
    void slowDataRecv_Slot(const QHash<QString, MBP_Data_Struct>& data);

signals:
    void portfolioAddedSignal();
private:
    Ui::F1_F2_BuySell *ui;
    QListView *stockNameListView;
    EventFilter *eventFilterstockName;
    QStandardItemModel *model_stock_name;
    double devicer;
    double decimal_precision;
    SlowData slowData;

};

#endif // F1_F2_BUYSELL_H
