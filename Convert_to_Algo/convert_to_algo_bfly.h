#ifndef CONVERT_TO_ALGO_BFLY_H
#define CONVERT_TO_ALGO_BFLY_H

#include <QObject>
#include "QLineEdit"
#include "QTableWidget"
#include "QStandardItemModel"
#include "Convert_to_Algo/convert_algo_shared_var.h"
#include "QListView"
#include "QMessageBox"
#include "QDateTime"
#include "eventfilterlistview.h"
#include "QSpinBox"

class add_algo_btfly : public QObject
{
    Q_OBJECT

public:
    explicit add_algo_btfly(QObject *parent = nullptr);
   void copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_,QLineEdit *lineEdit_Start_strike_,QLineEdit *lineEdit_EndStrike_,QLineEdit *lineEdit_StrikeDifference_,QPushButton *addButton_);
    QStringList BFLY_Tokens;
    //void create_AutoFillModel_StartStrike();
    void selectedAction();
    QString foo_token_number_start_strike;// this for start strike input of btfly
    QString foo_token_number_end_strike;// this for start strike input of btfly
    QStandardItemModel *model_start_strike_BFLY;
    QStandardItemModel *model_end_strike;


    void startStrikeEditFinishedAction();
    void generateAlgo();
    void clearAllModel();
signals:
  void progressSignal(bool, QString);

private slots:


public slots:
    void itemSelectedStartStrike(QModelIndex);
    void itemSelectedEndStrike(QModelIndex index);
    void slotStartHide(QString);
    void slotEndHide(QString);

private:
    QLineEdit *lineEdit_Start_strike;
    QLineEdit *lineEdit_EndStrike;
    QLineEdit *lineEdit_StrikeDifference;
    QSpinBox  *mkt_qty1;
    QSpinBox  *mkt_qty2;
    QSpinBox  *mkt_qty3;

    QTableWidget *tableWidget;
    QListView *startStrikeListView;
    QListView *endStrikeListView;
    QPushButton *addButton;

    QStringList expiry_dateList;
    QStringList token_numebrList;
    AddAlgoSharedVar *sharedData;
    QStringList strike_priceList;

    EventFilter *eventFilterStart;
    EventFilter *eventFilterEnd;

};


#endif // CONVERT_TO_ALGO_BFLY_H
