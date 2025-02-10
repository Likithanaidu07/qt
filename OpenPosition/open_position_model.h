#ifndef OPEN_POSITION_MODEL_H
#define OPEN_POSITION_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFont>
#include "defines.h"
#include "QColor"
#include "QMutex"


class open_position_model: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit open_position_model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> alog_data_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> open_position_data_list;
    // void setColumnWidths(QTableView *tableView) const;
   // void updateM2M(const QHash<QString, MBP_Data_Struct>& MBP_Data_Hash);

private:
    int col_count;
    QStringList header={"StockName","Buy/Sell","Open Lot","Open Avg","LTP","Square Off"/*,"Profit"*/};
    mutable  QMutex mutex;
     double devicer;
};

#endif // OPEN_POSITION_MODEL_H
