#ifndef MISSED_TRADE_TABLE_MODEL_H
#define MISSED_TRADE_TABLE_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFont>
#include "defines.h"
#include "QColor"


class Missed_Trade_Table_Model: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Missed_Trade_Table_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> missed_trade_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> missed_trade_data_list;
    QStringList getDataForPlacingOrder(int idx);
    int getSingleOrderId(int idx);

private:
    int col_count;
    QStringList header={  "SNo",
                          "Order Id",
                          "B/S",
                          "Type",
                          "Lot",
                          "Algo No",
                          "StockName",
                          "Message",
                          "Price",
                          "Time",
                          "Place Order",
                          "Traded Price",
                          "Traded Lot"};


signals:
    void  editCompleted(int row,int col,QString,QString,QString,bool);
    void  edit_Started(int row,int col) const;


};

#endif // MISSED_TRADE_TABLE_MODEL_H


