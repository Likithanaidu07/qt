#include "trade_table_model.h"
#include <qfont.h>
#include <QTableView>
#include <QHeaderView>

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
        //        //jackpot background color
        //        //    if(c==OrderBook_Idx::Jackpot_OB){
        //        //        int jackPot =  trade_data_list[r][c].toInt();
        //        //        if(jackPot>0)
        //        //            return QVariant::fromValue(QColor(Qt::green));
        //        //        else
        //        //            return QVariant::fromValue(QColor(Qt::red));
        //        //    }
        //        if(c==OrderBook_Idx::BuyorSell_OB ||
        //            c==OrderBook_Idx::ExchPrice_OB ||
        //            c==OrderBook_Idx::UserPrice_OB ||
        //            c==OrderBook_Idx::Jackpot_OB ||
        //            c==OrderBook_Idx::TradedLot_OB ||
        //            c==OrderBook_Idx::RemainingLot_OB ||
        //            c==OrderBook_Idx::TradeTime_OB ||
        //            c==OrderBook_Idx::AlgoName_OB ||
        //            c==OrderBook_Idx::AlgoNo_OB ||
        //            c==OrderBook_Idx::OrderId_OB ||
        //            c==OrderBook_Idx::BidLegState_OB ||
        //            c==OrderBook_Idx::Leg1State_OB ||
        //            c==OrderBook_Idx::Leg3State_OB) {

        //            QString buy_sell = trade_data_list[r][OrderBook_Idx::BuyorSell_OB];

        //            if(buy_sell == "Sell")
        //                return QVariant::fromValue(QColor("#FEE"));
        //            else
        //                return QVariant::fromValue(QColor("#F0FFFA"));
        //        }



        //        //

        // /*       else if(c==OrderBook_Idx::BidLegState_OB){
        //            int Leg2_OrderState_idx = trade_data_list[r].size()-2;
        //            int Leg2_OrderState = trade_data_list[r][Leg2_OrderState_idx].toInt();
        //            if(Leg2_OrderState==7 )
        //                return QVariant::fromValue(QColor(0,122,0));
        //            else if(Leg2_OrderState==13)
        //                return QVariant::fromValue(QColor(203,5,5));
        //            else if (Leg2_OrderState==8)
        //                return QVariant::fromValue(QColor(0,0,255));
        //            else
        //                return QVariant::fromValue(QColor(0,0,0));
        //        }
        if(c==OrderBook_Idx::Leg1State_OB){
            int Leg1_OrderState = trade_data_list[r][OrderBook_Idx::Leg1StateVal_OB].toInt();
            //            QString Leg2_OrderState =  trade_data_list[r][c];
            //            QString Leg3_OrderState =  trade_data_list[r][c];
            if(Leg1_OrderState==5 || Leg1_OrderState==13 )
                return QVariant::fromValue(QColor(203,5,5));
            else if(Leg1_OrderState==6)
                return QVariant::fromValue(QColor(250, 42, 85));
            else if(Leg1_OrderState==8)
                return QVariant::fromValue(QColor(0,128,0));
            else if(Leg1_OrderState==10)
                return QVariant::fromValue(QColor(0,0,139));
            else
                return QVariant::fromValue(QColor(0,0,0));
        }
        else if(c==OrderBook_Idx::Leg3State_OB){
            int Leg3_OrderState = trade_data_list[r][OrderBook_Idx::Leg3StateVal_OB].toInt();
            //            QString Leg2_OrderState =  trade_data_list[r][c];
            //            QString Leg3_OrderState =  trade_data_list[r][c];
            if(Leg3_OrderState==5 || Leg3_OrderState==13 )
                return QVariant::fromValue(QColor(203,5,5));
            else if(Leg3_OrderState==6)
                return QVariant::fromValue(QColor(250, 42, 85));
            else if(Leg3_OrderState==8)
                return QVariant::fromValue(QColor(0,128,0));
            else if(Leg3_OrderState==10)
                return QVariant::fromValue(QColor(0,0,139));
            else
                return QVariant::fromValue(QColor(0,0,0));
        }

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
            //|| c==OrderBook_Idx::AlgoNo_OB){
            font.setPointSize(8);
            font.setBold(true);
            return font;
        }
        else if( c==OrderBook_Idx::ExchPrice_OB ||
                 c==OrderBook_Idx::UserPrice_OB ||
                 c==OrderBook_Idx::TradeTime_OB ||
                 c==OrderBook_Idx::Jackpot_OB ||
                 c==OrderBook_Idx::TradedLot_OB ||
                 c==OrderBook_Idx::RemainingLot_OB ||
                 c==OrderBook_Idx::BuyorSell_OB ||
                 c==OrderBook_Idx::BidLegState_OB||
                 c==OrderBook_Idx::Leg1State_OB||
                 c==OrderBook_Idx::Leg3State_OB||
                 c==OrderBook_Idx::OrderId_OB
                   || c==OrderBook_Idx::AlgoNo_OB){

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
                if(trade_data_listNew[i][OrderBook_Idx::TraderData_OB]==trade_data_list[j][OrderBook_Idx::TraderData_OB]){ // TraderData_OB---this will be unique for row
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
            for(int j=0;j<trade_data_listNew.length();j++){
                if(trade_data_list[i][OrderBook_Idx::TraderData_OB]==trade_data_listNew[j][OrderBook_Idx::TraderData_OB]){ // TraderData_OB---this will be unique for row
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

QStringList Trade_Table_Model::getTradedDataForIdx(int idx){
    QStringList data;
    if (idx >= 0 && idx < trade_data_list.size()) {
        data = trade_data_list.at(idx);
    }
    return data;
}

QStringList Trade_Table_Model::getTradedPortFolioList(){
    QStringList tradedPortFolios;
    for(int j=0;j<trade_data_list.length();j++){
        tradedPortFolios.append(trade_data_list[j][OrderBook_Idx::AlgoNo_OB]);
    }
    return tradedPortFolios;
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
void Trade_Table_Model::setColumnWidths(QTableView *tableView) const {
    for (int col = 0; col < columnCount(); ++col) {
        int maxWidth = 0;

        // Consider header text width
        QVariant headerText = this->headerData(col, Qt::Horizontal, Qt::DisplayRole);
        int headerWidth = tableView->fontMetrics().horizontalAdvance(headerText.toString());
        maxWidth = qMax(maxWidth, headerWidth);

        for (int row = 0; row < rowCount(); ++row) {
            QModelIndex index = this->index(row, col);

            // Use the font metrics of the QTableView
            QString text = data(index, Qt::DisplayRole).toString();
            int textWidth = tableView->fontMetrics().horizontalAdvance(text);

            // Adjust the maximum width if necessary
            maxWidth = qMax(maxWidth, textWidth);
        }

        // Add some padding
        maxWidth += 30;
        // Set the column width directly in the view
        tableView->setColumnWidth(col, maxWidth);
        tableView->setColumnWidth(AlgoName_OB,300);
        //  tableView->horizontalHeader()->setSectionResizeMode(_Status, QHeaderView::Fixed);
    }
}
