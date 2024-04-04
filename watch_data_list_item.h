#ifndef WATCH_DATA_LISTI_TEM_H
#define WATCH_DATA_LISTI_TEM_H

#include <QWidget>
#include "defines.h"

namespace Ui {
class watch_Data_List_Item;
}

class watch_Data_List_Item : public QWidget
{
    Q_OBJECT

public:
    explicit watch_Data_List_Item(QWidget *parent = nullptr);
    ~watch_Data_List_Item();
    void setData(Indices_Data_Struct data);
    Indices_Data_Struct data;

private:
    Ui::watch_Data_List_Item *ui;
};

#endif // WATCH_DATA_LISTI_TEM_H
