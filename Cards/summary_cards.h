#ifndef SUMMARY_CARDS_H
#define SUMMARY_CARDS_H

#include <QDialog>
#include "QStandardItemModel"
#include "QListView"
#include "Convert_to_Algo/eventfilterlistview.h"
#include "custom_q_completer.h"
#include "mysql_conn.h"

namespace Ui {
class Summary_cards;
}

class Summary_cards : public QDialog
{
    Q_OBJECT

public:
    explicit Summary_cards(QWidget *parent = nullptr);
    ~Summary_cards();
     QString fixDecimal(double num,int decimal_precision);
public slots:
    void updateSummaryData(const QStringList &data);



private:
    Ui::Summary_cards *ui;


};

#endif // SUMMARY_CARDS_H
