#ifndef TRADE_TABLE_MODEL_H
#define TRADE_TABLE_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "defines.h"
#include "QColor"

class Trade_Table_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Trade_Table_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> alog_data_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> trade_data_list;
    void search_action(QString searchTxt);
    QStringList getTradedPortFolioList();
    QStringList getTradedDataForIdx(int idx);

private:
    int col_count;


    QStringList header={ "Order Id",
                          "Algo Name",
                          "User Price",
                          "Exch Price",
                          "Jackpot",
                          "Traded Lot",
                          "Remaining Lot",
                          //"Buy/Sell",
                          "Time",
                          "BidLeg State",
                          "Leg1 State",
                          "Leg3 State",
                          "Algo No"};


signals:
    void  editCompleted(int row,int col,QString,QString,QString,bool);
    void  edit_Started(int row,int col) const;



};

#endif // TRADE_TABLE_MODEL_H
