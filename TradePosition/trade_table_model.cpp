
#include "trade_table_model.h"
#include <qfont.h>
#include <QTableView>
#include <QHeaderView>
#include <QStandardPaths>
#include "QSettings"

Trade_Table_Model::Trade_Table_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
    QSettings settings(appDataPath + "/ExecutedTableHighlight_ExcludeList.dat", QSettings::IniFormat);
    ExecutedTableHighlight_ExcludeList = settings.value("ExecutedTableHighlight_ExcludeList").toStringList();
}
int Trade_Table_Model::rowCount(const QModelIndex & /*parent*/) const
{
    return trade_data_list.length();
}

int Trade_Table_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return col_count;
}
QVariant Trade_Table_Model::
    data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(trade_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    switch (role) {
        //    case Qt::BackgroundRole: {
        //        int Leg1_OrderState = trade_data_list[r][OrderBook_Idx::Leg1StateVal_OB].toInt();
        //        int Leg3_OrderState = trade_data_list[r][OrderBook_Idx::Leg3StateVal_OB].toInt();
        //        int Leg4_OrderState = trade_data_list[r][OrderBook_Idx::Leg4StateVal_OB].toInt();
        //        int Leg2_OrderState = trade_data_list[r][OrderBook_Idx::BidLegStateVal_OB].toInt();
        //        if (Leg1_OrderState == 6 || Leg3_OrderState == 6 || Leg4_OrderState == 6 || Leg2_OrderState == 6) {
        //            return QVariant::fromValue(QColor(215, 207, 232));  // Light purple background color
        //        } else {
        //            return QVariant();  // Default background color
        //        }
        //    }


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
        /* if(c==OrderBook_Idx::Leg1State_OB){
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
        else if(c==OrderBook_Idx::Leg4State_OB){
            int Leg4_OrderState = trade_data_list[r][OrderBook_Idx::Leg4StateVal_OB].toInt();
            if(Leg4_OrderState==5 || Leg4_OrderState==13 )
                return QVariant::fromValue(QColor(203,5,5));
            else if(Leg4_OrderState==6)
                return QVariant::fromValue(QColor(250, 42, 85));
            else if(Leg4_OrderState==8)
                return QVariant::fromValue(QColor(0,128,0));
            else if(Leg4_OrderState==10)
                return QVariant::fromValue(QColor(0,0,139));
            else
                return QVariant::fromValue(QColor(0,0,0));
        }
        else if(c==OrderBook_Idx::BidLegState_OB){
            int Leg2_OrderState = trade_data_list[r][OrderBook_Idx::BidLegStateVal_OB].toInt();
            if(Leg2_OrderState==5 || Leg2_OrderState==13 )
                return QVariant::fromValue(QColor(203,5,5));
            else if(Leg2_OrderState==6)
                return QVariant::fromValue(QColor(250, 42, 85));
            else if(Leg2_OrderState==8)
                return QVariant::fromValue(QColor(0,128,0));
            else if(Leg2_OrderState==10)
                return QVariant::fromValue(QColor(0,0,139));
            else
                return QVariant::fromValue(QColor(0,0,0));
        }

        else*/
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
                 c==OrderBook_Idx::OrderId_OB ||
                 c==OrderBook_Idx::Leg4State_OB
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
//QStringList Trade_Table_Model::getTradeTableHighlight_ExcludeList(){
//    return TradeTableHighlight_ExcludeList;
//}
QStringList Trade_Table_Model::getTradedPortFolioList(){
    QStringList tradedPortFolios;
    for(int j=0;j<trade_data_list.length();j++){
        tradedPortFolios.append(trade_data_list[j][OrderBook_Idx::AlgoNo_OB]);
    }
    return tradedPortFolios;
}


QList<QHash<QString,QString>> Trade_Table_Model::getTradedPortFolioListForAlgoID(QString algoID){

    QList<QHash<QString,QString>> tradedPortFolios;
    for(int j=0;j<trade_data_list.length();j++){
        if(algoID==trade_data_list[j][OrderBook_Idx::AlgoNo_OB]){
            QHash<QString,QString> tmp;
            tmp.insert("Algo_ID",trade_data_list[j][OrderBook_Idx::AlgoNo_OB]);
            tmp.insert("Algo_Name",trade_data_list[j][OrderBook_Idx::AlgoName_OB]);
            tmp.insert("Exch_Price",trade_data_list[j][OrderBook_Idx::ExchPrice_OB]);
            tmp.insert("User_Price",trade_data_list[j][OrderBook_Idx::UserPrice_OB]);
            tmp.insert("Jackpot",trade_data_list[j][OrderBook_Idx::Jackpot_OB]);
            tmp.insert("Traded_Lot",trade_data_list[j][OrderBook_Idx::TradedLot_OB]);
            tmp.insert("Remaining_Lot",trade_data_list[j][OrderBook_Idx::RemainingLot_OB]);
            tmp.insert("Buy_Sell",trade_data_list[j][OrderBook_Idx::BuyorSell_OB]);
            tmp.insert("Time",trade_data_list[j][OrderBook_Idx::TradeTime_OB]);
            tradedPortFolios.append(tmp);
        }
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
//void Trade_Table_Model::setColumnWidths(QTableView *tableView) const {
//    int maxWidth = 0;

//    // Calculate the maximum width across all columns and rows
//    for (int col = 0; col < columnCount(); ++col) {
//        // Consider header text width
//        QVariant headerText = this->headerData(col, Qt::Horizontal, Qt::DisplayRole);
//        int headerWidth = tableView->fontMetrics().horizontalAdvance(headerText.toString());
//        maxWidth = qMax(maxWidth, headerWidth);

//        // Check the width of each row's text for the current column
//        for (int row = 0; row < rowCount(); ++row) {
//            QModelIndex index = this->index(row, col);
//            QString text = data(index, Qt::DisplayRole).toString();
//            int textWidth = tableView->fontMetrics().horizontalAdvance(text);

//            // Update the maximum width if necessary
//            maxWidth = qMax(maxWidth, textWidth);
//        }


////    // Add some padding
////    maxWidth += 30;

//    tableView->setColumnWidth(col, maxWidth);

//    tableView->setColumnWidth(OrderId_OB,45);
//    tableView->setColumnWidth(AlgoName_OB,300);
//   // tableView->horizontalHeader()->setSectionResizeMode(_Status, QHeaderView::Fixed);
//}
//}

void Trade_Table_Model::selectionChangedSlot(const QModelIndex &index) {


    int currentIdx = index.row();
    if (currentIdx != -1) {
        if (trade_data_list[currentIdx][TradeTable_Hihglight_OB] == "1") {
            trade_data_list[currentIdx][TradeTable_Hihglight_OB] = "0";
            if (!ExecutedTableHighlight_ExcludeList.contains(trade_data_list[currentIdx][TraderData_OB])) {
                ExecutedTableHighlight_ExcludeList.append(trade_data_list[currentIdx][TraderData_OB]);
                QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
                QSettings settings(appDataPath + "/ExecutedTableHighlight_ExcludeList.dat", QSettings::IniFormat);
                settings.setValue("ExecutedTableHighlight_ExcludeList", ExecutedTableHighlight_ExcludeList);
            }
        }
    }
}
/*void Trade_Table_Model::selectionChangedSlot(int currentIdx) {
    if (currentIdx != -1) {
        if (trade_data_list[currentIdx][TradeTable_Hihglight_OB] == "1") {
            trade_data_list[currentIdx][TradeTable_Hihglight_OB] = "0";
            if (!ExecutedTableHighlight_ExcludeList.contains(trade_data_list[currentIdx][TradeTable_Hihglight_OB])) {
                ExecutedTableHighlight_ExcludeList.append(trade_data_list[currentIdx][TradeTable_Hihglight_OB]);
                QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
                QSettings settings(appDataPath + "/ExecutedTableHighlight_ExcludeList.dat", QSettings::IniFormat);
                settings.setValue("ExecutedTableHighlight_ExcludeList", ExecutedTableHighlight_ExcludeList);
            }
        }
    }
}*/
QStringList Trade_Table_Model::getExecutedTableHighlight_ExcludeList(){
    return ExecutedTableHighlight_ExcludeList;
}
