#include "combined_tracker_table_model.h"

Combined_Tracker_Table_Model::Combined_Tracker_Table_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
}
int Combined_Tracker_Table_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return  combined_tracker_data_list.length();
}

int Combined_Tracker_Table_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}

QVariant Combined_Tracker_Table_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(combined_tracker_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::DisplayRole:
        return  combined_tracker_data_list[r][c];
    default:
        return QVariant();

    case Qt::FontRole: {
        QFont font("Work Sans");
//        if (c == OrderBook_Idx::AlgoName_OB){
//            font.setPointSize(8);
//            font.setBold(true);
//            return font;
//        }
//        else if(c==OrderBook_Idx::AlgoNo_OB ||
//                 c==OrderBook_Idx::ExchPrice_OB ||
//                 c==OrderBook_Idx::UserPrice_OB ||
//                 c==OrderBook_Idx::TradeTime_OB ||
//                 c==OrderBook_Idx::Jackpot_OB ||
//                 c==OrderBook_Idx::TradedLot_OB ||
//                 c==OrderBook_Idx::RemainingLot_OB ||
//                 c==OrderBook_Idx::BuyorSell_OB ){

//            return font;
//        }
//        else
            return font;
    }
    }



}

void Combined_Tracker_Table_Model::setDataList(QList <QStringList> combined_tracker_data_listNew){
    if(combined_tracker_data_list.size()==0){
        Combined_Tracker_Table_Model::beginResetModel();
        combined_tracker_data_list.clear();
        for(int i=0;i<combined_tracker_data_listNew.length();i++){
            combined_tracker_data_list.append(combined_tracker_data_listNew.at(i));
        }
        Combined_Tracker_Table_Model::endResetModel();
    }
    else{
        for(int i=0;i<combined_tracker_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<combined_tracker_data_list.length();j++){
                int unique_id_idx = 0;  // portfolio numebr will be unique

                if(combined_tracker_data_listNew[i][unique_id_idx]==combined_tracker_data_list[j][unique_id_idx]){
                    newData = false;
                    for(int k=0;k<combined_tracker_data_listNew[i].size();k++){
                        if(combined_tracker_data_listNew[i][k]!=combined_tracker_data_list[j][k]){
                            combined_tracker_data_list[j][k] = combined_tracker_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }
            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                combined_tracker_data_list.insert(i,combined_tracker_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<combined_tracker_data_list.length();i++){
            bool deletRow = true;
            for(int j=0;j<combined_tracker_data_listNew.length();j++){
                int unique_id_idx = 0; // portfolio numebr will be unique
                if(combined_tracker_data_list[i][unique_id_idx]==combined_tracker_data_listNew[j][unique_id_idx]){ // token_numebr(last element in array)---this will be unique for row
                    deletRow=false; //the id is there in new and old data so do not remove this row.
                }
            }
            if(deletRow)
                rowsToRemove.append(i); // store the index of the row to be removed
        }

        //remove rows
        /*for(int i=0;i<rowsToRemove.length();i++){
            int rowIndex = rowsToRemove[i];
            beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
            combined_tracker_data_list.removeAt(rowIndex); //
            endRemoveRows();

        }*/
        //remove rows
        for (int i = rowsToRemove.length() - 1; i >= 0; i--) {
            int rowIndex = rowsToRemove[i];
            beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
            combined_tracker_data_list.removeAt(rowIndex); // use removeAt to remove the row by index
            endRemoveRows();

        }
    }

}



QVariant Combined_Tracker_Table_Model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}

