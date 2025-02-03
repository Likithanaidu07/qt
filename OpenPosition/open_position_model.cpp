#include "open_position_model.h"

open_position_model::open_position_model(QObject *parent)
    : QAbstractTableModel{parent}
{
    col_count = header.count();
     devicer = FO_DEVICER;
}


int open_position_model::rowCount(const QModelIndex & /*parent*/) const
{
    return open_position_data_list.length();;
}

int open_position_model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}


QVariant open_position_model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(open_position_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::DisplayRole:
        return  open_position_data_list[r][c];
    default:
        return QVariant();
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
        //    case Qt::BackgroundRole :
        //            if(c==NET_POS::SNo_NP ||
        //                c==NET_POS::BuyTotalLot_NP ||
        //                c==NET_POS::StockName_NP ||
        //                c==NET_POS::NetQty_NP ||
        //                c==NET_POS::Profit_NP ||
        //                c==NET_POS::MTM) {
        //                return QVariant::fromValue(QColor("#A4D1F5"));
        //            }
        //            else
        //                return QVariant();

    }
}


void open_position_model::setDataList(QList <QStringList> open_position_data_listNew){
    if(open_position_data_list.size()==0){
        open_position_model::beginResetModel();
        open_position_data_list.clear();
        for(int i=0;i<open_position_data_listNew.length();i++){
            open_position_data_list.append(open_position_data_listNew.at(i));
        }
        open_position_model::endResetModel();
    }
    else{
        for(int i=0;i<open_position_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<open_position_data_list.length();j++){
                int unique_id_idx = Open_Position_Idx::TokenNo;//open_position_data_listNew[i].length()-1; // token_numebr(last element in array)---this will be unique for row

                if(open_position_data_listNew[i][unique_id_idx]==open_position_data_list[j][unique_id_idx]){
                    newData = false;
                    for(int k=0;k<open_position_data_listNew[i].size();k++){
                        if(open_position_data_listNew[i][k]!=open_position_data_list[j][k]){
                            open_position_data_list[j][k] = open_position_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }

            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                open_position_data_list.insert(i,open_position_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<open_position_data_list.length();i++){
            bool deletRow = true;
            for(int j=0;j<open_position_data_listNew.length();j++){
                int unique_id_idx = Open_Position_Idx::TokenNo;//open_position_data_list[i].length()-1; // token_numebr(last element in array)---this will be unique for row
                if(open_position_data_list[i][unique_id_idx]==open_position_data_listNew[j][unique_id_idx]){ // token_numebr(last element in array)---this will be unique for row
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
            open_position_data_list.removeAt(rowIndex);
            endRemoveRows();
        }
    }
}





QVariant open_position_model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}
