#include "liners_model.h"

Liners_Model::Liners_Model(QObject *parent)
    : QAbstractTableModel{parent}
{
    col_count = header.count();
}
int Liners_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return liners_data_list.length();
}

int Liners_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}

QVariant Liners_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(liners_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::DisplayRole:
        return  liners_data_list[r][c];
    default:
        return QVariant();
    case Qt::TextAlignmentRole:{
        return Qt::AlignCenter;
    }

    case Qt::FontRole: {
        QFont font("Work Sans");
        return font;
    }
//    case Qt::BackgroundRole :

//        if(c==Liners_Idx::BuyAvgPrice ||
//            c==Liners_Idx::BuyQtyinLots) {
//                return QVariant::fromValue(QColor("#F0FFFA"));
//        }
//        else if(c==Liners_Idx::SellAvgPrice ||
//            c==Liners_Idx::SellQtyinLots) {
//                return QVariant::fromValue(QColor("#FEE"));
//        }
//            else
//              return QVariant();
        }

    }



void Liners_Model::setDataList(QList <QStringList> liners_data_listNew){
    QMutexLocker locker(&mutex);

    if(liners_data_list.size()==0){
        Liners_Model::beginResetModel();
        liners_data_list.clear();
        for(int i=0;i<liners_data_listNew.length();i++){
            liners_data_list.append(liners_data_listNew.at(i));
        }
        Liners_Model::endResetModel();
    }
    else{
        for(int i=0;i<liners_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<liners_data_list.length();j++){

                if(liners_data_listNew[i][Liners_Idx::AlgoId]==liners_data_list[j][Liners_Idx::AlgoId]){
                    newData = false;
                    for(int k=0;k<liners_data_listNew[i].size();k++){
                        if(liners_data_listNew[i][k]!=liners_data_list[j][k]){
                            liners_data_list[j][k] = liners_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }

            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                liners_data_list.insert(i,liners_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<liners_data_list.length();i++){
            bool deletRow = true;
            for(int j=0;j<liners_data_listNew.length();j++){
                if(liners_data_list[i][Liners_Idx::AlgoId]==liners_data_listNew[j][Liners_Idx::AlgoId]){ // AlgoId  will be unique for row
                    deletRow=false; //the id is there in new and old data so do not remove this row.
                }
            }
            if(deletRow)
                rowsToRemove.append(i); // store the index of the row to be removed
        }

        //remove rows

        for (int i = rowsToRemove.length() - 1; i >= 0; i--) {
            int rowIndex = rowsToRemove[i];
            beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
            liners_data_list.removeAt(rowIndex);
            endRemoveRows();

        }
    }

}

QStringList Liners_Model::getLinersDataForAlgoID(QString algoID){
    QMutexLocker locker(&mutex);
    QStringList data;
    for(int i=0;i<liners_data_list.size();i++){
       if(liners_data_list[i][0]==algoID){
           data = liners_data_list[i];
           break;
       }
    }
    return data;
}

QVariant Liners_Model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}

