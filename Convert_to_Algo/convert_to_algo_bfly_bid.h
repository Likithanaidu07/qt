#ifndef CONVERT_TO_ALGO_BFLY_BID_H
#define CONVERT_TO_ALGO_BFLY_BID_H

#include <QObject>
#include "QLineEdit"
#include "QTableWidget"
#include "QStandardItemModel"
#include "Convert_to_Algo/convert_algo_shared_var.h"
#include <QListView>
#include "QMessageBox"
#include "QDateTime"
#include "eventfilterlistview.h"

class add_algo_btfly_bid : public QObject
{
    Q_OBJECT

public:
    explicit add_algo_btfly_bid(QObject *parent = nullptr);

    void copyUIElement(QTableWidget *tableWidget_, QLineEdit *lineEdit_Start_strike_, QLineEdit *lineEdit_EndStrike_, QLineEdit *lineEdit_StrikeDifference_, QListView *sView, QListView *eView);
    QStringList sorted_keys_BFLY_BID; // sorted keys for contract_table hash table
    void create_AutoFillModel_StartStrike();
    void selectedAction();
    QString foo_token_number_start_strike;// this for start strike input of btfly
    QString foo_token_number_end_strike;// this for start strike input of btfly
    QStandardItemModel *model_start_strike_BFLY_BID;
    QStandardItemModel *model_end_strike;
     QStandardItemModel *model_start_strike;
    void startStrikeEditFinishedAction();
    void generateAlgo();
    void endStrikeEditFinishedAction();
    void initializeUI() ;



private slots:
    void slotStartHide(QString);
    void slotEndHide(QString);

public slots:
    void itemSelected(QModelIndex);
    void itemSelectedEndStrike(QModelIndex index);
private:

    QLineEdit *lineEdit_Start_strike;
    QLineEdit *lineEdit_EndStrike;
    QLineEdit *lineEdit_StrikeDifference;

    QTableWidget *tableWidget;
    QListView *startStrikeListView;
    QListView *endStrikeListView;

    QStringList expiry_dateList;
    QStringList token_numebrList;
    AddAlgoSharedVar *sharedData;
    QStringList strike_priceList;

    EventFilter *eventFilterStart;
    EventFilter *eventFilterEnd;

};


#endif // CONVERT_TO_ALGO_BFLY_BID_H
