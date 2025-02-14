#include "order_f1_f2_model.h"
#include <qfont.h>
#include <QTableView>
#include <QHeaderView>


Order_F1_F2_Model::Order_F1_F2_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
}
int Order_F1_F2_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return f1f2_order_data_list.length();
}

int Order_F1_F2_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}
QVariant Order_F1_F2_Model::
    data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(f1f2_order_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::DisplayRole:
        return  f1f2_order_data_list[r][c];
    default:
        return QVariant();
    case Qt::TextAlignmentRole:{
        return Qt::AlignCenter;
    }

    case Qt::FontRole: {
        QFont font("Work Sans");
        if (c == F1F2_Order_Data_Idx::AlgoName_F1F2){
            font.setPointSize(8);
            font.setBold(true);
            return font;
        }
        else
          return font;


    }
    }

}

void Order_F1_F2_Model::setDataList(QList <QStringList> f1f2_order_data_listNew){
    if(f1f2_order_data_list.size()==0){
        Order_F1_F2_Model::beginResetModel();
        f1f2_order_data_list.clear();
        for(int i=0;i<f1f2_order_data_listNew.length();i++){
            f1f2_order_data_list.append(f1f2_order_data_listNew.at(i));
        }
        Order_F1_F2_Model::endResetModel();
    }
    else{
        for(int i=0;i<f1f2_order_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<f1f2_order_data_list.length();j++){
                if(f1f2_order_data_listNew[i][F1F2_Order_Data_Idx::TraderData_F1F2]==f1f2_order_data_list[j][F1F2_Order_Data_Idx::TraderData_F1F2]){ // TraderData_OB---this will be unique for row
                    newData = false;
                    for(int k=0;k<f1f2_order_data_listNew[i].size();k++){
                        if(f1f2_order_data_listNew[i][k]!=f1f2_order_data_list[j][k]){
                            f1f2_order_data_list[j][k] = f1f2_order_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }
            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                f1f2_order_data_list.insert(i,f1f2_order_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<f1f2_order_data_list.length();i++){
            bool deletRow = true;
            for(int j=0;j<f1f2_order_data_listNew.length();j++){
                if(f1f2_order_data_list[i][F1F2_Order_Data_Idx::TraderData_F1F2]==f1f2_order_data_listNew[j][F1F2_Order_Data_Idx::TraderData_F1F2]){ // TraderData_OB---this will be unique for row
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
            f1f2_order_data_list.removeAt(rowIndex); // use removeAt to remove the row by index
            endRemoveRows();

        }
    }

}


QVariant Order_F1_F2_Model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}
bool Order_F1_F2_Model::containsOrderId(int orderId) {
    for (int row = 0; row < f1f2_order_data_list.size(); ++row) {
        if (f1f2_order_data_list[row].at(OrderId).toInt() == orderId) {
            return true;
        }
    }
    return false;
}
