#ifndef LINERS_MODEL_H
#define LINERS_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QFont>
#include "defines.h"
#include "QColor"
#include "QMutex"



class Liners_Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Liners_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &index) const override;
    void setDataList(QList <QStringList> alog_data_list);
    QVariant headerData(int section, Qt::Orientation ,   int role) const override;
    QList <QStringList> liners_data_list;
    //QHash <QString,QStringList> liners_data_hash;
    QStringList getLinersDataForAlgoID(QString algoID);

private:
    int col_count;
    QStringList header={"Algo Id","Algo Name","BAvg","BLot","SAvg","SLot","Net Lot"/*,"Profit"*/};
    mutable  QMutex mutex;

signals:
    void  editCompleted(int row,int col,QString,QString,QString,bool);
    void  edit_Started(int row,int col) const;
};

#endif // LINERS_MODEL_H
