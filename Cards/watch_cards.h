#ifndef WATCH_CARDS_H
#define WATCH_CARDS_H

#include <QDialog>
#include "QStandardItemModel"
#include "QListView"
#include "Convert_to_Algo/eventfilterlistview.h"
#include "custom_q_completer.h"
#include "mysql_conn.h"
#include "QListWidgetItem"

namespace Ui {
class Watch_cards;
}

class Watch_cards : public QDialog
{
    Q_OBJECT

public:
    explicit Watch_cards(QWidget *parent = nullptr);
    ~Watch_cards();

public slots:
      void indicesDataRecv_Slot(Indices_Data_Struct data);

private slots:
    void on_lineEditWatchSearch_textChanged(const QString &arg1);
    void on_listWidgetWatch_itemClicked(QListWidgetItem *item);

    void on_listWidgetWatch_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidgetWatch_itemSelectionChanged();

private:
    Ui::Watch_cards *ui;

    QHash<QString, Indices_Data_Struct> indicesDataList;//hash to store indices data for watch

    QMutex indicesDataMutex; //list to store indices data for watch
    QString watchItemSelectedindexName;
    QStringList savedWatchItems; // saved watch item to display on watch
    void showSaveWatchOnListView();
    void initWatchWindow();
    void loadWatch_InstrumentNames();
    void updateSelecteWatch_UI( Indices_Data_Struct data);
    void addToSavedWatchItems(Indices_Data_Struct data);
    void removeFromSavedWatchItems(Indices_Data_Struct data);
    void saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList);
    void loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList);


};

#endif // WATCH_CARDS_H
