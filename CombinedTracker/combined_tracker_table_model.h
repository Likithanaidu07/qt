#ifndef COMBINED_TRACKER_TABLE_MODEL_H
#define COMBINED_TRACKER_TABLE_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFont>
#include "defines.h"
#include "QColor"


class Combined_Tracker_Table_Model: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Combined_Tracker_Table_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> combined_tracker_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> combined_tracker_data_list;

private:
    int col_count;
    QStringList header={"Rule No", ///PortfolioNumber
                          "Symbol", ///InstrumentName
                          "Strategy", /// StrategyType
                          "Expiry",
                          "Type",
                          "Left Leg", /// Leg1
                          "Center Leg", /// Leg2
                          "Right Leg", /// Leg3
                          "Bid Qty", ///BuyQuantity
                          "Bid Lots", ///BuyQtyInLots
                          "Bid Avg Price", ///BuyAveragePrice
                          "Ask Qty", ///SellQuantity
                          "Ask Lots", ///SellQtyInLots
                          "Ask Avg Price", ///SellAveragePrice
                          "Profit"};


signals:
    void  editCompleted(int row,int col,QString,QString,QString,bool);
    void  edit_Started(int row,int col) const;


};

#endif // COMBINED_TRACKER_TABLE_MODEL_H
