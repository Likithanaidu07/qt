#include "table_portfolios_model.h"
#include "QTime"
#include "QPushButton"
#include "SlowData/slowdata.h"
#include "SwitchButton/switchbutton.h"
#include "QDir"
#include "QSettings"
#include <QFontMetrics>
#include <QStandardPaths>
#include <QFontDatabase>
#include <QStandardItemModel>

struct CustomFormatting {
    QColor color;
    QFont font;
};

Table_Portfolios_Model::Table_Portfolios_Model(QObject *parent) : QAbstractTableModel(parent)
{
    devicer = FO_DEVICER;
    decimal_precision = FO_DECIMAL_PRECISION;
    loadSettings();
}

void Table_Portfolios_Model::loadSettings(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if (settings.contains("market_type"))
            market_type = settings.value("market_type").toString();
        settings.endGroup();
    }
    if(market_type=="fo"){
        devicer = FO_DEVICER;
        decimal_precision = FO_DECIMAL_PRECISION;
    }
    else{
        devicer = CDS_DEVICER;
        decimal_precision = CDS_DECIMAL_PRECISION;
    }

}

void Table_Portfolios_Model::onItemChanged(const QModelIndex &index)
{
    // if (index.column() == PortfolioData_Idx::_Status)
    // {
    //     setData(index, data(index, Qt::ItemIsUserCheckable).value<Qt::CheckState>(), Qt::ItemIsUserCheckable);
    // }
}

int Table_Portfolios_Model::rowCount(const QModelIndex & /*parent*/) const
{

    return portfolio_data_list.length();
}

int Table_Portfolios_Model::columnCount(const QModelIndex & /*parent*/) const
{
    return header.count();
    //  return col_count;
}

QString Table_Portfolios_Model::double_to_Human_Readable(double num, int precision) const{
    QString str = QString::number(num, 'f', precision);
    //str.remove( QRegularExpression("0+$") ); // Remove any number of trailing 0's
    // str.remove( QRegularExpression("\\.$") ); // If the last character is just a '.' then remove it
    return str;
}

QVariant Table_Portfolios_Model::data(const QModelIndex &index, int role) const
{
    int r = index.row();
    int c = index.column();

    if(portfolio_data_list.length()==0)
        return QVariant();

    if (!index.isValid())
        return QVariant();

    PortfolioObject *portfolio = portfolio_data_list.at(index.row());
    switch (role) {

    case Qt::ItemIsUserCheckable:
    {
        if (index.column() == PortfolioData_Idx::_Status) {
            if (portfolio->StatusVal==QString::number(Qt::Checked))
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
    }

    case Qt::ForegroundRole: {
        if (c == PortfolioData_Idx::_BuyMarketRate){
            return QVariant::fromValue(QColor(0,122,0));
        }
        else if (c == PortfolioData_Idx::_SellMarketRate){
            return QVariant::fromValue(QColor(203,5,5));
        }
        else
            return QVariant();
    }
    case Qt::FontRole: {
        QFont font("Work Sans");
        if (c == PortfolioData_Idx::_AlgoName){
            font.setPointSize(8);
            font.setBold(true);
            return font;
        }
        else if(c == PortfolioData_Idx::_Status ||
                 c == PortfolioData_Idx::_PortfolioNumber ||
                 c == PortfolioData_Idx::_BuyMarketRate ||
                 c == PortfolioData_Idx::_BuyAveragePrice ||
                 c == PortfolioData_Idx::_BuyPriceDifference ||
                 c == PortfolioData_Idx::_BuyTotalQuantity ||
                 c == PortfolioData_Idx::_BuyTradedQuantity ||
                 c == PortfolioData_Idx::_BuyRemainingQuantity ||
                 c == PortfolioData_Idx::_SellMarketRate ||
                 c == PortfolioData_Idx::_SellAveragePrice ||
                 c == PortfolioData_Idx::_SellPriceDifference ||
                 c == PortfolioData_Idx::_SellTotalQuantity ||
                 c == PortfolioData_Idx::_SellTradedQuantity ||
                 c == PortfolioData_Idx::_SellRemainingQuantity ||
                 c == PortfolioData_Idx::_ExpiryDateTime ||
                 c == PortfolioData_Idx::_Leg1 ||
                 c == PortfolioData_Idx::_Cost ||
                 c == PortfolioData_Idx::_OrderQuantity ||
                 c == PortfolioData_Idx::_InstrumentName ||
                 c == PortfolioData_Idx::_Leg2 ||
                 c == PortfolioData_Idx::_Leg3 ||
                 c == PortfolioData_Idx::_AdditionalData1 ||
                 c == PortfolioData_Idx::_PortfolioType ||
                 c == PortfolioData_Idx::_Price ||
                 c == PortfolioData_Idx::_FuturePrice){

            return font;
        }
        if(portfolio->edting.loadRelaxed()==1){
            return font;
        }
        else
            return QVariant();
    }


    case Qt::TextAlignmentRole:{

        if(index.column()==PortfolioData_Idx::_PortfolioNumber){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_Status){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_AlgoName){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_BuyMarketRate || index.column()==PortfolioData_Idx::_BuyAveragePrice || index.column()==PortfolioData_Idx::_BuyPriceDifference || index.column()==PortfolioData_Idx::_BuyTotalQuantity|| index.column()==PortfolioData_Idx::_BuyTradedQuantity || index.column()==PortfolioData_Idx::_BuyRemainingQuantity){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_OrderQuantity || index.column()==PortfolioData_Idx::_SellMarketRate || index.column()==PortfolioData_Idx::_SellAveragePrice || index.column()==PortfolioData_Idx::_SellPriceDifference || index.column()==PortfolioData_Idx::_SellTotalQuantity || index.column()==PortfolioData_Idx::_SellTradedQuantity || index.column()==PortfolioData_Idx::_SellRemainingQuantity){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_ExpiryDateTime || index.column()==PortfolioData_Idx::_Leg1){
            return Qt::AlignCenter;
        }
    }


    case Qt::DisplayRole: {
        /* if (index.column() == PortfolioData_Idx::_Status) {
                     if(portfolio->StatusVal==QString::number(portfolio_status::Active))
                         return "Active";
                     else if(portfolio->StatusVal==QString::number(portfolio_status::Filled))
                         return "Filled";
                     else if(portfolio->StatusVal==QString::number(portfolio_status::DisabledByUser))
                         return "Stopped";
                     else
                         return "-NA-";

    }
        else */
            if (index.column() == PortfolioData_Idx::_PortfolioNumber) {
            return portfolio->PortfolioNumber;
        }
        else if(index.column() == PortfolioData_Idx::_AlgoName){
            return portfolio->AlgoName;
        }
        else if (index.column() == PortfolioData_Idx::_SellMarketRate) {
            return double_to_Human_Readable(portfolio->SellMarketRate,decimal_precision);
        }
        else if (index.column() == PortfolioData_Idx::_SellAveragePrice) {
            if(portfolio->SellAveragePrice==""){
                return "-";
            }
            return portfolio->SellAveragePrice;
        }
        else if (index.column() == PortfolioData_Idx::_SellPriceDifference) {
            return double_to_Human_Readable(portfolio->SellPriceDifference,decimal_precision);
        }
        else if (index.column() == PortfolioData_Idx::_SellTotalQuantity) {
            return portfolio->SellTotalQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_SellTradedQuantity) {
            return portfolio->SellTradedQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_SellRemainingQuantity) {
            return portfolio->SellRemainingQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_BuyMarketRate) {
            return double_to_Human_Readable(portfolio->BuyMarketRate,decimal_precision);
        }
        else if (index.column() == PortfolioData_Idx::_BuyAveragePrice) {
            if(portfolio->BuyAveragePrice=="")
                return "-";
            return portfolio->BuyAveragePrice;
        }
        else if (index.column() == PortfolioData_Idx::_BuyPriceDifference) {

            return double_to_Human_Readable(portfolio->BuyPriceDifference,decimal_precision);
        }
        else if (index.column() == PortfolioData_Idx::_BuyTotalQuantity) {
            return portfolio->BuyTotalQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_BuyTradedQuantity) {
            return portfolio->BuyTradedQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_BuyRemainingQuantity) {
            return portfolio->BuyRemainingQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_ExpiryDateTime) {
            return portfolio->ExpiryDateTime.toString("ddMMMyy");
        }
        else if (index.column() == PortfolioData_Idx::_Leg1) {
            return portfolio->Leg1;
        }
        else if (index.column() == PortfolioData_Idx::_Cost) {
            return portfolio->Cost;
        }
        else if (index.column() == PortfolioData_Idx::_OrderQuantity) {
            return portfolio->OrderQuantity;
        }
        /*else if (index.column() == PortfolioData_Idx::_InstrumentName) {
                     return portfolio->InstrumentName;
                 }*/

        /*else if (index.column() == PortfolioData_Idx::_Leg2) {
                     return portfolio->Leg2;
                 }
                 else if (index.column() == PortfolioData_Idx::_Leg3) {
                     return portfolio->Leg3;
                 }*/
        /* else if (index.column() == PortfolioData_Idx::_AdditionalData1) {
                     return portfolio->AdditionalData1;
                 }*/
        /*else if (index.column() == PortfolioData_Idx::_PortfolioType) {
                     return portfolio->PortfolioType;
                 }*/
        /*else if (index.column() == PortfolioData_Idx::_Price) {
                     return portfolio->Price();
                 }*/

        /*else if (index.column() == PortfolioData_Idx::_FuturePrice) {
                     return portfolio->FuturePrice;
                 }*/
        else
            return QVariant();
    }
case Qt::EditRole:{
    PortfolioObject *portfolio = portfolio_data_list.at(index.row());
    portfolio->edting.storeRelaxed(1);
    qDebug()<<"editStarted";
    if(c==PortfolioData_Idx::_SellPriceDifference){
        emit edit_Started(r,c);
        return double_to_Human_Readable(portfolio->SellPriceDifference,decimal_precision);
    }
    else if(c==PortfolioData_Idx::_BuyPriceDifference){
        emit edit_Started(r,c);
        return double_to_Human_Readable(portfolio->BuyPriceDifference,decimal_precision);
    }
    else if(c==PortfolioData_Idx::_SellTotalQuantity){
        emit edit_Started(r,c);
        return portfolio->SellTotalQuantity;
    }
    else if(c==PortfolioData_Idx::_BuyTotalQuantity){
        qDebug()<<"editStarted";
        emit edit_Started(r,c);
        return portfolio->BuyTotalQuantity;
    }
    else if(c==PortfolioData_Idx::_OrderQuantity){
        emit edit_Started(r,c);
        return portfolio->OrderQuantity;
    }
    else
        return QVariant();
}

default:
    return QVariant();
}


return QVariant();

}


bool Table_Portfolios_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QMutexLocker locker(&mutex); // Lock the mutex automatically

    int c=index.column();
    if (role == Qt::ItemIsUserCheckable
        && index.column() == PortfolioData_Idx::_Status)
    {
        portfolio_data_list[index.row()]->StatusVal = QString::number(value.toInt());
        emit dataChanged(index, index, {role});
        emit updateDBOnDataChanged(index);
        return true;
    }
    if(role== Qt::EditRole)
    {
        if (!checkIndex(index))
            return false;
        if(c==PortfolioData_Idx::_SellPriceDifference){
            portfolio_data_list[index.row()]->SellPriceDifference = value.toDouble();
            emit dataChanged(index, index);
            return true;
        }
        else if(c==PortfolioData_Idx::_BuyPriceDifference){
            portfolio_data_list[index.row()]->BuyPriceDifference = value.toDouble();
            emit dataChanged(index, index);
            return true;


        }
        else if(c==PortfolioData_Idx::_SellTotalQuantity){
            portfolio_data_list[index.row()]->SellTotalQuantity = value.toDouble();
            emit dataChanged(index, index);
            return true;


        }
        else if(c==PortfolioData_Idx::_BuyTotalQuantity){
            portfolio_data_list[index.row()]->BuyTotalQuantity = value.toDouble();
            emit dataChanged(index, index);
            return true;


        }
        else if(c==PortfolioData_Idx::_OrderQuantity){
            portfolio_data_list[index.row()]->OrderQuantity = value.toDouble();
            emit dataChanged(index, index);
            return true;

        }
        else
            return false;

    }
    return false;
}

bool Table_Portfolios_Model::storeToken(PortfolioObject* p){
    bool newTokenFound = false;
    //insert tokens
    if(!portfolio_tokens.contains(p->Leg1TokenNo)){
        portfolio_tokens.append(p->Leg1TokenNo);
        newTokenFound = true;
    }
    if(!portfolio_tokens.contains(p->Leg2TokenNo)){
        portfolio_tokens.append(p->Leg2TokenNo);
        newTokenFound = true;
    }
    if(!portfolio_tokens.contains(p->Leg3TokenNo)){
        portfolio_tokens.append(p->Leg3TokenNo);
        newTokenFound = true;
    }
    if(!portfolio_tokens.contains(p->Leg4TokenNo)){
        portfolio_tokens.append(p->Leg4TokenNo);
        newTokenFound = true;
    }
    if(!portfolio_tokens.contains(p->Leg5TokenNo)){
        portfolio_tokens.append(p->Leg5TokenNo);
        newTokenFound = true;
    }
    if(!portfolio_tokens.contains(p->Leg6TokenNo)){
        portfolio_tokens.append(p->Leg6TokenNo);
        newTokenFound = true;
    }

    return newTokenFound;
}

/*
void Table_Portfolios_Model::setDataList(QList <PortfolioObject*> portfolio_data_list_new){

QMutexLocker locker(&mutex); // Lock the mutex automatically
//new data so insert all to the list
bool newTokenFound = false;
if(portfolio_data_list.length()==0){
    for(int i=0;i<portfolio_data_list_new.length();i++){
        beginInsertRows(QModelIndex(), i, i);
        portfolio_data_list.insert(i,portfolio_data_list_new[i]);
        endInsertRows();
        if(storeToken(portfolio_data_list_new[i]))
            newTokenFound= true;
    }
}
//check the data is new else skip
else{
    for(int row_new=0;row_new<portfolio_data_list_new.length();row_new++){
        bool newData = true;

        for(int row_existing=0;row_existing<portfolio_data_list.length();row_existing++){
            if(portfolio_data_list_new[row_new]->PortfolioNumber==portfolio_data_list[row_existing]->PortfolioNumber){
                newData = false;
                if(portfolio_data_list_new[row_new]==portfolio_data_list[row_existing]){
                    continue;
                }
                else{
                    // update only if the current row is not editing
                    if(portfolio_data_list[row_existing]->edting.loadRelaxed()==0){
                        portfolio_data_list[row_existing] = portfolio_data_list_new[row_new];
                        int col = 17;// this need to be updated according to the changed column, if needed in future
                        emit dataChanged(index(row_existing,0), index(row_existing, col));
                    }
                }
            }


        }
        if(newData){
            beginInsertRows(QModelIndex(), row_new, row_new);
            portfolio_data_list.insert(row_new,portfolio_data_list_new[row_new]);
            endInsertRows();
            if(storeToken(portfolio_data_list_new[row_new]))
                newTokenFound= true;
        }
    }
}

//save token to slow data object
if(newTokenFound){
    QStringList list;
    for(int i=0;i<portfolio_tokens.length();i++)
        list.append(QString::number(portfolio_tokens[i]));
    SlowData slowData;
    slowData.setLeg_n_token(list);
}


//check some row is removed
// QList <PortfolioObject*> portfolio_ids_ToRemove;
QList <int> portfolio_ids_ToRemove;
for(int i=0;i<portfolio_data_list.length();i++){
    bool deletRow = true;
    for(int j=0;j<portfolio_data_list_new.length();j++){
        if(portfolio_data_list[i]->PortfolioNumber==portfolio_data_list_new[j]->PortfolioNumber){ // PortfolioNumber---this will be unique for row
            deletRow=false; //the id is there in new and old data so do not remove this row.
        }
    }
    if(deletRow)
        portfolio_ids_ToRemove.append(i);
}

//remove rows
for(int i = 0;i<portfolio_ids_ToRemove.length();i++){
    beginRemoveRows(QModelIndex(), portfolio_ids_ToRemove[i]-i, portfolio_ids_ToRemove[i]-i); //substract i means each time one item removed the index need to be decrement by 1
    PortfolioObject* portfolio = portfolio_data_list[portfolio_ids_ToRemove[i]-i];
    portfolio_data_list.removeOne(portfolio);//substract i means each time one item removed the index need to be decrement by 1
    delete portfolio;
    endRemoveRows();
}

/* for (PortfolioObject* portfolio : portfolio_ids_ToRemove)
    {
        delete portfolio;
    }*/
/*  portfolio_ids_ToRemove.clear();
}*/

void Table_Portfolios_Model::setDataList(QList <PortfolioObject*> portfolio_data_list_new){

    QMutexLocker locker(&mutex); // Lock the mutex automatically
    //new data so insert all to the list
    bool newTokenFound = false;
    QString largestText = "";
    if(portfolio_data_list.length()==0)
    {
        for(int i=0;i<portfolio_data_list_new.length();i++)
        {

            beginInsertRows(QModelIndex(), i, i);
            // portfolio_data_list.insert(i,portfolio_data_list_new[i]);
            PortfolioObject* clonedObject = new PortfolioObject(*portfolio_data_list_new[i]);
            portfolio_data_list.insert(i,clonedObject);
            if(clonedObject->AlgoName.size()>largestText.size())
                largestText = clonedObject->AlgoName;
            endInsertRows();
            if(storeToken(portfolio_data_list_new[i]))
                newTokenFound= true;
        }

        //emit resize column signal only once
        QFont font;
        font.setFamily("Kanit");
        font.setPointSize(12);
        QFontMetrics fontMetrics(font);

        if(largestText==""){
            largestText = "Algo Name";
        }

        // emit resizePortFolioTableColWidth(fontMetrics.horizontalAdvance(largestText));
    }
    //check the data is new else skip
    else{
        for(int row_new=0;row_new<portfolio_data_list_new.length();row_new++){
            bool newData = true;

            for(int row_existing=0;row_existing<portfolio_data_list.length();row_existing++){
                if(portfolio_data_list_new[row_new]->PortfolioNumber==portfolio_data_list[row_existing]->PortfolioNumber){
                    newData = false;
                    if(portfolio_data_list_new[row_new]==portfolio_data_list[row_existing]){
                        continue;
                    }
                    else{
                        // update only if the current row is not editing
                        if(portfolio_data_list[row_existing]->edting.loadRelaxed()==0){
                            PortfolioObject* clonedObject = new PortfolioObject(* portfolio_data_list_new[row_new]);
                            delete portfolio_data_list[row_existing]; // Delete the original PortfolioObject at the specified index in portfolio_data_list
                            portfolio_data_list[row_existing] = clonedObject;
                            int col = 17;// this need to be updated according to the changed column, if needed in future
                            emit dataChanged(index(row_existing,0), index(row_existing, col));
                        }
                    }
                }


            }
            if(newData){
                beginInsertRows(QModelIndex(), row_new, row_new);
                PortfolioObject* clonedObject = new PortfolioObject(*portfolio_data_list_new[row_new]);
                portfolio_data_list.insert(row_new,clonedObject);
                //  portfolio_data_list.insert(row_new,portfolio_data_list_new[row_new]);
                endInsertRows();
                if(storeToken(clonedObject))
                    newTokenFound= true;
            }
            else{
                //delete object to prevent memory leak
                //  PortfolioObject* portfolio = portfolio_data_list_new[row_new];
                //  delete portfolio;
            }
        }
    }

    //save token to slow data object
    if(newTokenFound){
        QStringList list;
        for(int i=0;i<portfolio_tokens.length();i++)
            list.append(QString::number(portfolio_tokens[i]));
        SlowData slowData;
        slowData.setLeg_n_token(list);
    }


    //check some row is removed
    // QList <PortfolioObject*> portfolio_ids_ToRemove;
    QList <int> portfolio_ids_ToRemove;
    for(int i=0;i<portfolio_data_list.length();i++){
        bool deletRow = true;
        for(int j=0;j<portfolio_data_list_new.length();j++){
            if(portfolio_data_list[i]->PortfolioNumber==portfolio_data_list_new[j]->PortfolioNumber){ // PortfolioNumber---this will be unique for row
                deletRow=false; //the id is there in new and old data so do not remove this row.
            }
        }
        if(deletRow)
            portfolio_ids_ToRemove.append(i);
    }

    //remove rows
    for(int i = 0;i<portfolio_ids_ToRemove.length();i++){
        beginRemoveRows(QModelIndex(), portfolio_ids_ToRemove[i]-i, portfolio_ids_ToRemove[i]-i); //substract i means each time one item removed the index need to be decrement by 1
        PortfolioObject* portfolio = portfolio_data_list[portfolio_ids_ToRemove[i]-i];
        portfolio_data_list.removeOne(portfolio);//substract i means each time one item removed the index need to be decrement by 1
        delete portfolio;
        endRemoveRows();
    }

    portfolio_ids_ToRemove.clear();
    for (PortfolioObject* portfolioDel : portfolio_data_list_new)
    {
        delete portfolioDel;
    }
    portfolio_data_list_new.clear();
}





Qt::ItemFlags Table_Portfolios_Model::flags(const QModelIndex &index) const
{
    int c=index.column();
    if(c==PortfolioData_Idx::_SellPriceDifference ||
        c==PortfolioData_Idx::_BuyPriceDifference ||
        c==PortfolioData_Idx::_SellTotalQuantity ||
        c==PortfolioData_Idx::_BuyTotalQuantity ||
        c==PortfolioData_Idx::_OrderQuantity)
    {
        return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    else if(c==PortfolioData_Idx::_Status){

        return Qt::ItemIsUserCheckable |QAbstractTableModel::flags(index);
    }

    return QAbstractTableModel::flags(index);
}

QVariant Table_Portfolios_Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole ){
        return QVariant();
    }
    if (orientation == Qt::Horizontal)
        return header.at(section);

    else
        return QString::number(section);

}

QHash<QString,PortFolioData_Less> Table_Portfolios_Model::getPortFolioDataLess(){

    QMutexLocker locker(&mutex); // Lock the mutex automatically
    QHash<QString,PortFolioData_Less>  PortFolioDataHash;
    for(int i=0;i<portfolio_data_list.length();i++){
        PortFolioData_Less P;
        P.PortfolioType = portfolio_data_list[i]->PortfolioType;
        P.Expiry =  portfolio_data_list[i]->Expiry;
        PortFolioDataHash.insert(QString::number(portfolio_data_list[i]->PortfolioNumber),P);

    }
    return PortFolioDataHash;
}

void Table_Portfolios_Model::refreshTable(){
    emit layoutChanged();
}



void Table_Portfolios_Model::setColumnWidths(QTableView *tableView) const {
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
    }
}
