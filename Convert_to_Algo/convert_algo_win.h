#ifndef CONVERT_ALGO_WIN_H
#define CONVERT_ALGO_WIN_H

#include <QDialog>
#include <QMouseEvent>
#include <QtConcurrent/QtConcurrent>

#include "ui_convert_algo_win.h"
#include "convert_algo_shared_var.h"
#include "convert_to_algo_bfly.h"
#include "add_algo_f2f.h"
#include "add_algo_con_rev.h"
#include "mysql_conn.h"

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
    explicit ConvertAlgo_Win(QWidget *parent = nullptr);
    bool pressed;
    QPoint position;
    void update_contract_tableData(QString foo_user_id_,int MaxPortfolioCount_);
    ~ConvertAlgo_Win();


private slots:
    void on_Close_clicked();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void display_log_text_slot(QString msg);
    void update_ui_slot(int);

    void on_lineEdit_Start_strike_editingFinished();

    void on_pushButtonAdd_clicked();

    void on_comboBox_AlgoType_currentTextChanged(const QString arg1);


    void on_pushButton_Cancel_clicked();


    void on_pushButtonUpload_clicked();
    void on_pushButtonSelectAll_clicked();
    void on_pushButton_Reset_clicked();
    void on_pushButtonDelete_clicked();

    void on_lineEdit_Fut_editingFinished();

private:
    Ui::ConvertAlgo_Win *ui;
    AddAlgoSharedVar *sharedData;
    QAtomicInt dataSorted;
    mysql_conn *db_conn;
    add_algo_btfly *algoBtFly;
    add_algo_f2f *algoF2F;
    add_algo_con_rev *algoConRev;
    void sort_data_and_populate_model(QHash<QString, contract_table> contract_table_/*,QHash<QString, contract_table> cd_contract_table_*/);
    void resetTableWidget();
    void enable_disable_UIElement(bool enable);


signals:
    void update_ui_signal(int);
    void display_log_text_signal(QString);

};

#endif // CONVERT_ALGO_WIN_H

