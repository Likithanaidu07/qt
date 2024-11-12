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
#include "eventfilterlistview.h"
class add_algo_con_rev : public QObject
{
    Q_OBJECT

public:
    explicit add_algo_con_rev(QObject *parent = nullptr);
    void copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_,QLineEdit *lineEdit_Start_strike_,QLineEdit *lineEdit_EndStrike_,QLineEdit *lineEdit_Fut_,QPushButton *addButton_);
    QStringList CON_REV_Tokens; // sorted keys for contract_table hash table     // this used for start strike and endstrike
    QStringList Fut_Tokens;
    void create_AutoFillModel_StartStrike();

    void selectedAction();
    QString foo_token_number_start_strike;// this for start strike input of btfly
    QString foo_token_number_end_strike;// this for start strike input of btfly
    QString foo_token_number_fut; // this for Fut input of CON_REV algo type

    void startStrikeEditFinishedAction();
    void generateAlgo();
    QStandardItemModel *model_start_strike_CR;
    QStandardItemModel *model_end_strike_CR;
    QStandardItemModel *model_Fut_CR;
    void clearAllModel();

signals:
  void progressSignal(bool, QString);
private:
    QLineEdit *lineEdit_Start_strike;
    QLineEdit *lineEdit_EndStrike;
    QLineEdit *lineEdit_Fut;


    QTableWidget *tableWidget;
    QListView *startStrikeListView;
    QListView *endStrikeListView;
    QListView *futListView;
    QPushButton *addButton;

    QStringList expiry_dateList;
    QStringList token_numebrList;
    AddAlgoSharedVar *sharedData;
    QStringList strike_priceList;


    EventFilter *eventFilterStart;
    EventFilter *eventFilterEnd;
    EventFilter *eventFilterFUT;
    bool compareStrikePrice(const QString &key1, const QString &key2, const QHash<QString, contract_table> &contractTableHash);
    void sortFilteredKeys(QStringList &filteredKeys, const QHash<QString, contract_table> &contractTableHash);

public slots:
    void itemSelectedStartStrike(QModelIndex);
    void itemSelectedEndStrike(QModelIndex index);
    void itemSelectedFut(QModelIndex);

    void slotStartHide(QString);
    void slotEndHide(QString);
    void slotFutHide(QString);

};


#endif // ADD_ALGO_CON_REV_H

