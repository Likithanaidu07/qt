#ifndef ORDER_F1_F2_MODEL_H
#define ORDER_F1_F2_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "defines.h"
#include "QColor"
#include "qtableview.h"

class Order_F1_F2_Model : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit Order_F1_F2_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> alog_data_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> f1f2_order_data_list;
    bool containsOrderId(int orderId);
private:
    int col_count;


    QStringList header={  /*"Algo No",*/
                          "SNo",
                          "StockName",
                          "User Price",
                          "Exch Price",
                          "Traded Lot",
//                          "Remaining Lot",
                          "Time",
                          "Status"};

};

#endif // ORDER_F1_F2_MODEL_H


