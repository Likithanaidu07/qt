#ifndef CONVERT_ALGO_WIN_H
#define CONVERT_ALGO_WIN_H

#include <QDialog>
#include <QMouseEvent>
#include <QtConcurrent/QtConcurrent>

#include "ui_convert_algo_win.h"
#include "convert_algo_shared_var.h"
#include "convert_to_algo_bfly.h"
#include "Convert_to_Algo/convert_to_algo_box_bid.h"
#include "Convert_to_Algo/convert_to_algo_open_2l.h"

#include "add_algo_con_rev.h"
#include "mysql_conn.h"
#include "convert_to_algo_bfly_bid.h"
#include "Common/macros.h"
#include "add_algo_cr_jelly_bid.h"
#include "add_algo_f2f.h"
#include "convert_to_algo_bs1221_1331.h"

#include <QTableWidget>
#include <QKeyEvent>



enum UI_UPDATE_TYPE{
    LOADED_MODEL=0
};



namespace Ui {
class ConvertAlgo_Win;
}

class ConvertAlgo_Win : public QDialog
{
    Q_OBJECT
public:
    explicit ConvertAlgo_Win(QWidget *parent = nullptr,const QStringList &list = QStringList());
    bool pressed;
    QPoint position;
     userInfo userData;
    void update_contract_tableData(QString foo_user_id_,int MaxPortfolioCount_);
    ~ConvertAlgo_Win();

    void hideAnyListViewVisible();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void portfolioAddedSignal();


private slots:
    void on_Close_clicked();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void display_log_text_slot(QString msg);
    void update_ui_slot(int);


    void on_pushButtonAdd_clicked();

    void on_comboBox_AlgoType_currentTextChanged(const QString arg1);


    void on_pushButton_Cancel_clicked();


    void on_pushButtonUpload_clicked();
    void on_pushButtonSelectAll_clicked();
    void on_pushButton_Reset_clicked();
    void on_pushButtonDelete_clicked();

private:


    Ui::ConvertAlgo_Win *ui;
    AddAlgoSharedVar *sharedData;
    QAtomicInt dataSorted;
    mysql_conn *db_conn;
    add_algo_btfly *algoBtFly;

    add_algo_f2f *algoF2F;
    add_algo_con_rev *algoConRev;
    add_algo_cr_jelly_bid *algoCRJellyBid;

    add_algo_btfly_bid *algoBtFlyBid;
    convert_to_algo_box_bid *algoBoxBid;
    convert_to_algo_open_2L *algoOpen2L;


    convert_to_algo_bs1221_1331 *algoBS_1221_1331;
    QStringList ExFilterPF;


    void sort_data_and_populate_model(QHash<QString, contract_table> contract_table_/*,QHash<QString, contract_table> cd_contract_table_*/);
    void resetTableWidget();
    void enable_disable_UIElement(bool enable);
    QWidget *floatingWidgetES;
    QWidget *floatingWidgetSS;
  //  QListView *listViewStartStrike;
  //  QListView *listViewEndStrike;
    QListView *listViewFut;
    void setCustomRatioValidator(QLineEdit *lineEdit, int maxDigits);


signals:
    void update_ui_signal(int);
    void display_log_text_signal(QString);
    void portFolioAdded_Signal();

public slots:
    void  progressSlot(bool show, QString msg);


};

#endif // CONVERT_ALGO_WIN_H

