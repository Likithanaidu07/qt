#include "trade_table_model.h"
#include <qfont.h>

Trade_Table_Model::Trade_Table_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
}
int Trade_Table_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return trade_data_list.length();
}

int Trade_Table_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}



QVariant Trade_Table_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(trade_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {

    case Qt::ForegroundRole :
        //jackpot text color
//        if(c==OrderBook_Idx::Jackpot_OB){
//            int jackPot =  trade_data_list[r][c].toInt();
//            if(jackPot>0)
//                return QVariant::fromValue(QColor(Qt::green));
//            else
//                return QVariant::fromValue(QColor(Qt::red));

//        }
        if(c==OrderBook_Idx::BuyorSell_OB
          /* c==OrderBook_Idx::ExchPrice_OB ||
           c==OrderBook_Idx::UserPrice_OB ||
           c==OrderBook_Idx::Jackpot_OB ||
           c==OrderBook_Idx::TradedLot_OB ||
           c==OrderBook_Idx::RemainingLot_OB ||
           c==OrderBook_Idx::TradeTime_OB*/ ){
            QString buy_sell =  trade_data_list[r][c];
          //  QString Exch_Price =  trade_data_list[r][c];
            if(buy_sell=="Buy")
                return QVariant::fromValue(QColor(0,122,0));
            else
                return QVariant::fromValue(QColor(203,5,5));
        }
        //

 /*       else if(c==OrderBook_Idx::BidLegState_OB){
            int Leg2_OrderState_idx = trade_data_list[r].size()-2;
            int Leg2_OrderState = trade_data_list[r][Leg2_OrderState_idx].toInt();
            if(Leg2_OrderState==7 )
                return QVariant::fromValue(QColor(0,122,0));
            else if(Leg2_OrderState==13)
                return QVariant::fromValue(QColor(203,5,5));
            else if (Leg2_OrderState==8)
                return QVariant::fromValue(QColor(0,0,255));
            else
                return QVariant::fromValue(QColor(0,0,0));
        }
        else if(c==OrderBook_Idx::Leg1State_OB){
            int Leg1_OrderState_idx = trade_data_list[r].size()-4;
            int Leg1_OrderState = trade_data_list[r][Leg1_OrderState_idx].toInt();
            //            QString Leg2_OrderState =  trade_data_list[r][c];
            //            QString Leg3_OrderState =  trade_data_list[r][c];
            if(Leg1_OrderState==7 )
                return QVariant::fromValue(QColor(0,122,0));
            else if(Leg1_OrderState==13)
                return QVariant::fromValue(QColor(203,5,5));
            else if (Leg1_OrderState==8)
                return QVariant::fromValue(QColor(0,0,255));
            else
                return QVariant::fromValue(QColor(0,0,0));
        }
        else if(c==OrderBook_Idx::Leg3State_OB){
            int Leg3_OrderState_idx = trade_data_list[r].size()-3;
            int Leg3_OrderState = trade_data_list[r][Leg3_OrderState_idx].toInt();
            //            QString Leg2_OrderState =  trade_data_list[r][c];
            //            QString Leg3_OrderState =  trade_data_list[r][c];
            if(Leg3_OrderState==7 )
                return QVariant::fromValue(QColor(0,122,0));
            else if(Leg3_OrderState==13)
                return QVariant::fromValue(QColor(203,5,5));
            else if (Leg3_OrderState==8)
                return QVariant::fromValue(QColor(0,0,255));
            else
                return QVariant::fromValue(QColor(0,0,0));
        } */
        else
            return QVariant();

    case Qt::DisplayRole:
        return  trade_data_list[r][c];
    default:
        return QVariant();
    case Qt::TextAlignmentRole:{
        return Qt::AlignCenter;
    }

    case Qt::FontRole: {
        QFont font("Work Sans");
        if (c == OrderBook_Idx::AlgoName_OB){
            font.setPointSize(8);
            font.setBold(true);
            return font;
        }
        else if(c==OrderBook_Idx::AlgoNo_OB ||
                   c==OrderBook_Idx::ExchPrice_OB ||
                   c==OrderBook_Idx::UserPrice_OB ||
                   c==OrderBook_Idx::TradeTime_OB ||
                   c==OrderBook_Idx::Jackpot_OB ||
                   c==OrderBook_Idx::TradedLot_OB ||
                   c==OrderBook_Idx::RemainingLot_OB ||
                   c==OrderBook_Idx::BuyorSell_OB ||
                   c==OrderBook_Idx::BidLegState_OB||
                   c==OrderBook_Idx::Leg1State_OB||
                   c==OrderBook_Idx::Leg3State_OB){

            return font;
        }
        else
            return QVariant();
    }
    }
}

void Trade_Table_Model::setDataList(QList <QStringList> trade_data_listNew){
    if(trade_data_list.size()==0){
        Trade_Table_Model::beginResetModel();
        trade_data_list.clear();
        for(int i=0;i<trade_data_listNew.length();i++){
            trade_data_list.append(trade_data_listNew.at(i));
        }
        Trade_Table_Model::endResetModel();
    }
    else{
        for(int i=0;i<trade_data_listNew.length();i++){
            bool newData = true;
            for(int j=0;j<trade_data_list.length();j++){
                int trader_data_idx = trade_data_list[j].size()-1; // traded data will be inserted as last data
                if(trade_data_listNew[i][trader_data_idx]==trade_data_list[j][trader_data_idx]){ // trade_data_list---this will be unique for row
                    newData = false;
                    for(int k=0;k<trade_data_listNew[i].size();k++){
                        if(trade_data_listNew[i][k]!=trade_data_list[j][k]){
                            trade_data_list[j][k] = trade_data_listNew[i][k];
                            emit dataChanged(index(j, k), index(j, k));
                        }
                    }
                }
            }
            if(newData){
                beginInsertRows(QModelIndex(), i, i);
                trade_data_list.insert(i,trade_data_listNew[i]);
                endInsertRows();
            }
        }

        //check some row is removed
        QList<int> rowsToRemove;
        for(int i=0;i<trade_data_list.length();i++){
            bool deletRow = true;
            int trader_data_idx = trade_data_list[i].size()-1; // traded data will be inserted as last data

            for(int j=0;j<trade_data_listNew.length();j++){
                if(trade_data_list[i][trader_data_idx]==trade_data_listNew[j][trader_data_idx]){ // alog_data_list---this will be unique for row
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
            trade_data_list.removeAt(rowIndex); // use removeAt to remove the row by index
            endRemoveRows();

        }
    }

}

void Trade_Table_Model::search_action(QString txt){

}



QVariant Trade_Table_Model::headerData(int section, Qt::Orientation orientation,   int role) const
{
    if (role != Qt::DisplayRole){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);
    else
        return QString::number(section);

}
