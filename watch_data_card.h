#ifndef WATCH_DATA_CARD_H
#define WATCH_DATA_CARD_H

#include <QWidget>
#include "defines.h"

namespace Ui {
class watch_data_card;
}

class watch_data_card : public QWidget
{
    Q_OBJECT

public:
    explicit watch_data_card(QWidget *parent = nullptr);
    ~watch_data_card();
    void setData(Indices_Data_Struct data);
    QString getIndexName();

    Indices_Data_Struct data;

private:
    Ui::watch_data_card *ui;
};
#endif // WATCH_DATA_CARD_H
