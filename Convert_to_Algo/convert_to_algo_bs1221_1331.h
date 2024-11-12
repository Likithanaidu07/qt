#ifndef CONVERT_TO_ALGO_BS1221_1331_H
#define CONVERT_TO_ALGO_BS1221_1331_H

#include <QObject>
#include "QLineEdit"
#include "QTableWidget"
#include "QStandardItemModel"
#include "Convert_to_Algo/convert_algo_shared_var.h"
#include <QListView>
#include "QMessageBox"
#include "QDateTime"
#include "eventfilterlistview.h"
#include "custom_q_completer.h"

    class convert_to_algo_bs1221_1331 : public QObject
    {
        Q_OBJECT

    public:
        explicit convert_to_algo_bs1221_1331(QObject *parent = nullptr);
        void copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_, QLineEdit *lineEdit_Start_strike_, QLineEdit *lineEdit_EndStrike_, QLineEdit *lineEdit_StrikeDifference_,QPushButton *addButton_);
        QStringList BS1XX1_Tokens; // filtered token for BS1221 or BS1331
        void selectedAction(PortfolioType _Portfolio_Type);
        QString foo_token_number_start_strike;// this for start strike input of btfly
        QString foo_token_number_end_strike;// this for start strike input of btfly
        QStandardItemModel *model_start_strike_BS1XX1;
        QStandardItemModel *model_end_strike;
        QStandardItemModel *model_start_strike;
        void createEndStrikeModelAndPopulateListView();
        void generateAlgo();
        void endStrikeEditFinishedAction();
        void initializeUI() ;
        void clearAllModel();


    signals:
      void progressSignal(bool, QString);

    public slots:
        void itemSelectedStartStrike(QModelIndex);
        void itemSelectedEndStrike(QModelIndex index);
        void slotStartHide(QString);
        void slotEndHide(QString);

    private:

        QLineEdit *lineEdit_Start_strike;
        QLineEdit *lineEdit_EndStrike;
        QLineEdit *lineEdit_StrikeDifference;

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
        PortfolioType Portfolio_Type;
        CustomSearchWidget *endstrikeCustomWidget;
    };


#endif // CONVERT_TO_ALGO_BS1221_1331_H
