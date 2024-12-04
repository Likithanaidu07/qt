#include "net_position_table_model.h"
#include <QTableView>
#include <QHeaderView>
#include "defines.h"

Net_Position_Table_Model::Net_Position_Table_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
    devicer = FO_DEVICER;
}
int Net_Position_Table_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return net_pos_data_list.length();
}

int Net_Position_Table_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}

QVariant Net_Position_Table_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(net_pos_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::DisplayRole:
        return  net_pos_data_list[r][c];
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


void Net_Position_Table_Model::setDataList(QList <QStringList> net_pos_data_listNew){
    if(net_pos_data_list.size()==0){
        Net_Position_Table_Model::beginResetModel();
        net_pos_data_list.clear();
        for(int i=0;i<net_pos_data_listNew.length();i++){
            net_pos_data_list.append(net_pos_data_listNew.at(i));
        }
        Net_Position_Table_Model::endResetModel();
    }
    else{
        for(int i=0;i<net_pos_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<net_pos_data_list.length();j++){
                int unique_id_idx = NET_POS::TokenNo_NP;//net_pos_data_listNew[i].length()-1; // token_numebr(last element in array)---this will be unique for row

                if(net_pos_data_listNew[i][unique_id_idx]==net_pos_data_list[j][unique_id_idx]){
                    newData = false;
                    for(int k=0;k<net_pos_data_listNew[i].size();k++){
                        if(net_pos_data_listNew[i][k]!=net_pos_data_list[j][k]){
                            net_pos_data_list[j][k] = net_pos_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }

            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                net_pos_data_list.insert(i,net_pos_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<net_pos_data_list.length();i++){
            bool deletRow = true;
            for(int j=0;j<net_pos_data_listNew.length();j++){
                int unique_id_idx = NET_POS::TokenNo_NP;//net_pos_data_list[i].length()-1; // token_numebr(last element in array)---this will be unique for row
                if(net_pos_data_list[i][unique_id_idx]==net_pos_data_listNew[j][unique_id_idx]){ // token_numebr(last element in array)---this will be unique for row
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
            net_pos_data_list.removeAt(rowIndex);
            endRemoveRows();

        }
    }

}
void Net_Position_Table_Model::updateM2M(const QHash<QString, MBP_Data_Struct>& MBP_Data_Hash){

    for(int i=0;i<net_pos_data_list.length();i++){


        QString TokenNo = net_pos_data_list[i][NET_POS::TokenNo_NP];
        /***************M2M calcualtion**********************/
        if(MBP_Data_Hash.contains(TokenNo))
        {
            MBP_Data_Struct mbpData = MBP_Data_Hash[TokenNo];
            double Net_Qty = net_pos_data_list[i][NET_POS::NetQty_NP].toDouble();
            if ( Net_Qty> 0) {
                double Buy_Avg_Price = net_pos_data_list[i][NET_POS::BuyAvgPrice_NP].toDouble();
                double M2M = ((((mbpData.lastTradedPrice.toDouble()  / devicer) - Buy_Avg_Price) * Net_Qty));
                net_pos_data_list[i][NET_POS::MTM_NP] = QString::number(M2M);
                emit dataChanged(index(i, NET_POS::MTM_NP), index(i, NET_POS::MTM_NP));

            }
            else if (Net_Qty < 0) {
               double Sell_Avg_Price = net_pos_data_list[i][NET_POS::SellAvgPrice_NP].toDouble();
               double M2M = ((((mbpData.lastTradedPrice.toDouble() / devicer) - Sell_Avg_Price) * Net_Qty));
               net_pos_data_list[i][NET_POS::MTM_NP] = QString::number(M2M);
               emit dataChanged(index(i, NET_POS::MTM_NP), index(i, NET_POS::MTM_NP));
            }
        }
        /******************************************************************/
    }
}




QVariant Net_Position_Table_Model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}
