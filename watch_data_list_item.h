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
    void setData(Indices_Data_Struct data,  QStringList* savedWatchItemsMainWinPtr_);
    Indices_Data_Struct data;

private slots:

    void on_checkBoxShowMainScreen_clicked(bool checked);

signals:
    void add_watch_item_to_main_win_signal(bool,Indices_Data_Struct);

private:
    Ui::watch_Data_List_Item *ui;
    QStringList* savedWatchItemsMainWinPtr; // Pointer to QStringList

};

#endif // WATCH_DATA_LISTI_TEM_H
