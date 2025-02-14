#include "missed_trade_table_model.h"

Missed_Trade_Table_Model::Missed_Trade_Table_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
}
int Missed_Trade_Table_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return  missed_trade_data_list.length();
}

int Missed_Trade_Table_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}

QVariant Missed_Trade_Table_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(missed_trade_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::DisplayRole:
        return  missed_trade_data_list[r][c];

    case Qt::TextAlignmentRole:{
        return Qt::AlignCenter;
    }

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

    default:
        return QVariant();
    }



}

//void Missed_Trade_Table_Model::setDataList(QList <QStringList> missed_trade_data_listNew){
//    if(missed_trade_data_list.size()==0){
//        Missed_Trade_Table_Model::beginResetModel();
//        missed_trade_data_list.clear();
//        for(int i=0;i<missed_trade_data_listNew.length();i++){
//            missed_trade_data_list.append(missed_trade_data_listNew.at(i));
//        }
//        Missed_Trade_Table_Model::endResetModel();
//    }
//    else{
//        for(int i=0;i<missed_trade_data_listNew.length();i++){
//            bool newData = true;
//            for(int j=0;j<missed_trade_data_list.length();j++){
//                int unique_id_idx = Missed_Trades_Idx::Missed_Trade_Id;  // Id will be unique

//                if(missed_trade_data_listNew[i][unique_id_idx]==missed_trade_data_list[j][unique_id_idx]){
//                    newData = false;
//                    for(int k=0;k<missed_trade_data_listNew[i].size();k++){
//                        if(missed_trade_data_listNew[i][k]!=missed_trade_data_list[j][k]){
//                            missed_trade_data_list[j][k] = missed_trade_data_listNew[i][k];
//                            emit dataChanged(index(j, k), index(j, k));
//                        }
//                    }
//                }
//            }
//            if(newData){
//                beginInsertRows(QModelIndex(), i, i);
//                missed_trade_data_list.insert(i,missed_trade_data_listNew[i]);
//                endInsertRows();
//            }
//        }

//        //check some row is removed
//        QList<int> rowsToRemove;
//        for(int i=0;i<missed_trade_data_list.length();i++){
//            bool deletRow = true;
//            for(int j=0;j<missed_trade_data_listNew.length();j++){
//                int unique_id_idx = Missed_Trades_Idx::Missed_Trade_Id;  // Id will be unique
//                if(missed_trade_data_list[i][unique_id_idx]==missed_trade_data_listNew[j][unique_id_idx]){ // token_numebr(last element in array)---this will be unique for row
//                    deletRow=false; //the id is there in new and old data so do not remove this row.
//                }
//            }
//            if(deletRow)
//                rowsToRemove.append(i); // store the index of the row to be removed
//        }

//        //remove rows
//        for(int i=0;i<rowsToRemove.length();i++){
//            int rowIndex = rowsToRemove[i];
//            beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
//            missed_trade_data_list.removeAt(rowIndex); //
//            endRemoveRows();
//        }
//    }

//}


void Missed_Trade_Table_Model::setDataList(QList <QStringList> missed_trade_data_listNew){
    if(missed_trade_data_list.size()==0){
        Missed_Trade_Table_Model::beginResetModel();
        missed_trade_data_list.clear();
        for(int i=0;i<missed_trade_data_listNew.length();i++){
            missed_trade_data_list.append(missed_trade_data_listNew.at(i));
        }
        Missed_Trade_Table_Model::endResetModel();
    }
    else{
        for(int i=0;i<missed_trade_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<missed_trade_data_list.length();j++){
                if(missed_trade_data_listNew[i][Missed_Trades_Idx::Missed_Trade_Id]==missed_trade_data_list[j][Missed_Trades_Idx::Missed_Trade_Id]){ // TraderData_OB---this will be unique for row
                    newData = false;
                    for(int k=0;k<missed_trade_data_listNew[i].size();k++){
                        if(missed_trade_data_listNew[i][k]!=missed_trade_data_list[j][k]){
                            missed_trade_data_list[j][k] = missed_trade_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }
            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                missed_trade_data_list.insert(i,missed_trade_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<missed_trade_data_list.length();i++){
            bool deletRow = true;
            for(int j=0;j<missed_trade_data_listNew.length();j++){
                if(missed_trade_data_list[i][Missed_Trades_Idx::Missed_Trade_Id]==missed_trade_data_listNew[j][Missed_Trades_Idx::Missed_Trade_Id]){ // TraderData_OB---this will be unique for row
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
            missed_trade_data_list.removeAt(rowIndex); // use removeAt to remove the row by index
            endRemoveRows();
        }
    }

}

QStringList Missed_Trade_Table_Model::getDataForPlacingOrder(int idx) {
    if (idx >= 0 && idx < missed_trade_data_list.size()) {
        return QStringList(missed_trade_data_list[idx]);  // Explicit deep copy
    }
    return QStringList();  // Return an empty list for invalid index
}
int Missed_Trade_Table_Model::getSingleOrderId(int idx) {
    if (idx >= 0 && idx < missed_trade_data_list.size()) {
        return missed_trade_data_list[idx].at(OrderId).toInt();
    }
    return -1;  // Return -1 for invalid index
}




QVariant Missed_Trade_Table_Model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}

