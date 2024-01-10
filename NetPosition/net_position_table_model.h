#ifndef NET_POSITION_TABLE_MODEL_H
#define NET_POSITION_TABLE_MODEL_H

#include <QObject>
#include <QFont>
#include <QAbstractTableModel>
#include "defines.h"
#include "QColor"

class Net_Position_Table_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Net_Position_Table_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> alog_data_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> net_pos_data_list;

private:
    int col_count;
    QStringList header={"Stock Name","Expiry","Buy Total Lot","Sell Total Lot","Buy price","Sell price","Buy Avg Price","Sell Avg Price","Net Qty","MTM"};


signals:
    void  editCompleted(int row,int col,QString,QString,QString,bool);
    void  edit_Started(int row,int col) const;



};

#endif // NET_POSITION_TABLE_MODEL_H
