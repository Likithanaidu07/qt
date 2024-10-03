#include "net_position_table_model.h"
#include <QTableView>
#include <QHeaderView>


Net_Position_Table_Model::Net_Position_Table_Model(QObject *parent) : QAbstractTableModel(parent)
{
    col_count = header.count();
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
                int unique_id_idx = net_pos_data_listNew[i].length()-1; // token_numebr(last element in array)---this will be unique for row

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
                int unique_id_idx = net_pos_data_list[i].length()-1; // token_numebr(last element in array)---this will be unique for row
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
void Net_Position_Table_Model::setColumnWidths(QTableView *tableView) const {
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
        // Set the column width for AlgoName_OB to 300 (or any preferred width)
        tableView->resizeColumnsToContents();             // Adjust columns to content
        tableView->resizeRowsToContents();                // Adjust rows to content
       // tableView->horizontalHeader()->setStretchLastSection(true);  // Stretch the last column
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);  // Optional for dynamic resizing


    }
}
