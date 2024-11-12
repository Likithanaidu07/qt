#ifndef ADD_ALGO_F2F_H
#define ADD_ALGO_F2F_H

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

class add_algo_f2f : public QObject
{
    Q_OBJECT
public:
    explicit add_algo_f2f(QObject *parent = nullptr);
    void copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_,QLineEdit *lineEdit_searchInstrument_leg1_,QLineEdit *lineEdit_searchInstrument_leg2_,QPushButton *addButton_);
    QStringList F2F_Tokens;
    void create_AutoFillModel_SearchInstrument();
    void selectedAction();
    QString foo_token_number_start_strike;// this for start strike input of F2F
    QString foo_token_number_end_strike;// this for start strike input of F2F
    void instrumentEditFinishedAction();
    void generateAlgo();
    QStandardItemModel *model_searchInstrument_F2F_Leg1;
    void clearAllModel();
signals:
  void progressSignal(bool, QString);
private:
    QLineEdit *lineEdit_searchInstrument_leg1;
    QLineEdit *lineEdit_searchInstrument_leg2;
    QTableWidget *tableWidget;
    QListView *startStrikeListView;
    QListView *endStrikeListView;
    QPushButton *addButton;
    QStandardItemModel *model_searchInstrument_F2F_Leg2;

    QStringList expiry_dateList;
    QStringList token_numebrList;
    AddAlgoSharedVar *sharedData;
    QString   get_Algo_Name(PortfolioType algo_type,int leg1_token_number,int leg2_token_number,int leg3_token_number,double devicer,int decimal_precision);
    QString fixDecimal(double num,int decimal_precision);

    EventFilter *eventFilterStart;
    EventFilter *eventFilterEnd;

private slots:


public slots:
    void itemSelectedStartStrike(QModelIndex);
    void itemSelectedEndStrike(QModelIndex index);
    void slotStartHide(QString);
    void slotEndHide(QString);

};

#endif // ADD_ALGO_F2F_H
