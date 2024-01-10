#ifndef ADD_ALGO_CON_REV_H
#define ADD_ALGO_CON_REV_H

#include <QObject>


#include <QObject>
#include "QLineEdit"
#include "QTableWidget"
#include "QStandardItemModel"
#include "convert_algo_shared_var.h"
#include "custom_q_completer.h"
#include "QListView"
#include "QMessageBox"
#include "QDateTime"
class add_algo_con_rev : public QObject
{
    Q_OBJECT

public:
    explicit add_algo_con_rev(QObject *parent = nullptr);
    void copyUIElement(QTableWidget *tableWidget_,QLineEdit *lineEdit_Start_strike_,QLineEdit *lineEdit_EndStrike_,QLineEdit *lineEdit_StrikeDifference_);
    QStringList sorted_keys_CON_REV; // sorted keys for contract_table hash table     // this used for start strike and endstrike
    QStringList sorted_keys_F2F; // sorted keys for contract_table hash table  // this used to full fut instrument
    void create_AutoFillModel_StartStrike();
    void create_AutoFillModel_FutInstrument();

    void selectedAction();
    QString foo_token_number_start_strike;// this for start strike input of btfly
    QString foo_token_number_end_strike;// this for start strike input of btfly
    QString foo_token_number_fut; // this for Fut input of CON_REV algo type

    void startStrikeEditFinishedAction();
    void generateAlgo();
    QStandardItemModel *model_FUT_CON_REV;

signals:
private:
    QLineEdit *lineEdit_Start_strike;
    QLineEdit *lineEdit_EndStrike;
    QLineEdit *lineEdit_Fut;

    QTableWidget *tableWidget;


    QStringList expiry_dateList;
    QStringList token_numebrList;
    AddAlgoSharedVar *sharedData;
    QStringList strike_priceList;
};


#endif // ADD_ALGO_CON_REV_H

