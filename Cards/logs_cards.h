#ifndef LOGS_CARDS_H
#define LOGS_CARDS_H

#include <QDialog>
#include "QStandardItemModel"
#include "QListView"
#include "Convert_to_Algo/eventfilterlistview.h"
#include "custom_q_completer.h"
#include "mysql_conn.h"
namespace Ui {
class Logs_cards;
}

class Logs_cards : public QDialog
{
    Q_OBJECT

public:
    explicit Logs_cards(QWidget *parent = nullptr);
    ~Logs_cards();

public slots:
 //  void  logDataSignal_Slot(QString str);
    void logDataSignal_Slot(QStringList data);

private:
    Ui::Logs_cards *ui;

     QString  htmlLogsContent;

};

#endif // LOGS_CARDS_H
