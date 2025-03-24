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
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QWidget>
#include "PortFolio/PortfolioParser.h"

//#define MUTEX_DEBUG_LOG

struct CustomFormatting {
    QColor color;
    QFont font;
};

Table_Portfolios_Model::Table_Portfolios_Model(QObject *parent) : QAbstractTableModel(parent)
{
    devicer = FO_DEVICER;
    decimal_precision = FO_DECIMAL_PRECISION;
   // loadSettings();
    current_editingRow = -1;

    slowDataPriceUpdateTimer.start();  // Start the timer initially

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
    QSettings settings(appDataPath + "/tradedHighlight_ExcludeList.dat", QSettings::IniFormat);
    TradedHighlight_ExcludeList = settings.value("tradedHighlight_ExcludeList").toStringList();

}

void Table_Portfolios_Model::loadSettings(){
   /* QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
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
    }*/

}

void Table_Portfolios_Model::onItemChanged(const QModelIndex &index)
{
    //qDebug()<<"..............Table_Portfolios_Model: onItemChanged"<<index.row();
    // if (index.column() == PortfolioData_Idx::_Status)
    // {
    //     setData(index, data(index, Qt::ItemIsUserCheckable).value<Qt::CheckState>(), Qt::ItemIsUserCheckable);
    // }
}

void Table_Portfolios_Model::selectionChangedSlot(int currentIdx){
#ifdef MUTEX_DEBUG_LOG
   // qDebug()<<"Entering---selectionChangedSlot";
#endif
    //QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
   // qDebug()<<"Setting Mutex---selectionChangedSlot";
#endif
    if(currentIdx!=-1){
        if(portfolio_data_list[currentIdx]->TradedHighlight == true){
            portfolio_data_list[currentIdx]->TradedHighlight = false; // make this flag false when click on portfolio row so it will un-highlight
            if(!TradedHighlight_ExcludeList.contains(QString::number(portfolio_data_list[currentIdx]->PortfolioNumber))){
                TradedHighlight_ExcludeList.append(QString::number(portfolio_data_list[currentIdx]->PortfolioNumber));
                QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
                QSettings settings(appDataPath + "/portfolio_tradedHighlight_ExcludeList.dat", QSettings::IniFormat);
                settings.setValue("tradedHighlight_ExcludeList", TradedHighlight_ExcludeList);
            }
        }
    }
    //editing row swithced, make previous row flg not editing
    if(current_editingRow!=currentIdx){
        if(current_editingRow!=-1&& current_editingRow < portfolio_data_list.size()){
           // portfolio_data_list[current_editingRow]->edting.storeRelaxed(0);
            editCancelled(current_editingRow);
        }
        else if(currentIdx == -1)
            current_editingRow = -1;
        editingDataHash.clear();

    }
#ifdef MUTEX_DEBUG_LOG
    //qDebug()<<"Exiting---selectionChangedSlot";
#endif

}
//This function retrun what portfolio clicked when it trade
QStringList Table_Portfolios_Model::getTradedHighlight_ExcludeList(){
    return TradedHighlight_ExcludeList;
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

    case Qt::CheckStateRole :
    {
        if (index.column() == PortfolioData_Idx::_Status) {
            if (portfolio->StatusVal==QString::number(Qt::Checked))
                return Qt::Checked;
            else
                return Qt::Unchecked;
        }
        else
         return QVariant();
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
        QFont font("Work Sans", 9);
        //font.setBold(false);
        if (c == PortfolioData_Idx::_AlgoName){
            font.setPointSize(8);
           // font.setBold(true);
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
                   c == PortfolioData_Idx::_OrderQuantity ||
                   c == PortfolioData_Idx::_ExpiryDateTime ||
                   c == PortfolioData_Idx::_Leg1 ||
                   c == PortfolioData_Idx::_Cost ||
                   c == PortfolioData_Idx::_InstrumentName ||
                   c == PortfolioData_Idx::_Leg2 ||
                   c == PortfolioData_Idx::_Leg3 ||
                   c == PortfolioData_Idx::_AdditionalData1 ||
                   c == PortfolioData_Idx::_PortfolioType ||
                   c == PortfolioData_Idx::_Price ||
                 c == PortfolioData_Idx::_FuturePrice||
                 c == PortfolioData_Idx::_QuantityRatio ||
             //    c == PortfolioData_Idx::_SkipMarketStrike||
               // c == PortfolioData_Idx::_BidLeg ||
                   c == PortfolioData_Idx::_Alias||
                c == PortfolioData_Idx::_MaxLoss||
                   c==PortfolioData_Idx::_Depth){

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
//        else if(index.column()==PortfolioData_Idx::_SkipMarketStrike){
//            return Qt::AlignCenter;
//        }
        else if(index.column()==PortfolioData_Idx::_FuturePrice){
            return Qt::AlignCenter;
        }
//        else if(index.column()==PortfolioData_Idx::_BidLeg){
//            return Qt::AlignCenter;
//        }
        else if(index.column()==PortfolioData_Idx::_QuantityRatio){
            return Qt::AlignCenter;

        }
        else if(index.column()==PortfolioData_Idx::_Cost){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_Alias){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_MaxLoss){
            return Qt::AlignCenter;
        }
        else if(index.column()==PortfolioData_Idx::_Depth){
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
        else if (index.column() == PortfolioData_Idx::_Depth) {
            return portfolio->Depth;
        }
        else if (index.column() == PortfolioData_Idx::_OrderQuantity) {
            return portfolio->OrderQuantity;
        }
        else if (index.column() == PortfolioData_Idx::_Alias) {
            return portfolio->Alias;
        }
        else if (index.column() == PortfolioData_Idx::_MaxLoss) {
             if(portfolio->MaxLoss<0)
                return "-";
            return double_to_Human_Readable(portfolio->MaxLoss,decimal_precision);
        }
        else if (index.column() == PortfolioData_Idx::_ExpiryDateTime) {
            QString ExpiryDateTimeStr = "-";
            if (!portfolio->ExpiryDateTime.isNull() && portfolio->ExpiryDateTime.isValid())
                ExpiryDateTimeStr =  portfolio->ExpiryDateTime.toString("dd-MMM-yy");
            if(ExpiryDateTimeStr=="")
                ExpiryDateTimeStr = "-";
            return ExpiryDateTimeStr;
        }
        else if (index.column() == PortfolioData_Idx::_Leg1) {
            return portfolio->Leg1;
        }
        else if (index.column() == PortfolioData_Idx::_Cost) {
            return portfolio->Cost;
        }

        else if (index.column() == PortfolioData_Idx::_QuantityRatio) {
            return portfolio->QuantityRatio;
        }
//        else if (index.column() == PortfolioData_Idx::_SkipMarketStrike) {
//            return portfolio->SkipMarketStrike;

//        }
//        else if (index.column() == PortfolioData_Idx::_BidLeg) {
//            return portfolio->BidLeg;

//        }
        else if (index.column() == PortfolioData_Idx::_FuturePrice) {
            return portfolio->FuturePrice;

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

    //editing row swithced, make previous row flg not editing
   if(current_editingRow!=index.row()){
      if(current_editingRow!=-1){
          portfolio_data_list[current_editingRow]->edting.storeRelaxed(0);
      }
    }
   // PortfolioObject *portfolio = portfolio_data_list.at(index.row());
    portfolio->edting.storeRelaxed(1);
   // qDebug()<<"editStarted";
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
    else if(c==PortfolioData_Idx::_Depth){
        emit edit_Started(r,c);
        return portfolio->Depth;
    }
    else if(c==PortfolioData_Idx::_BuyTotalQuantity){
      //  qDebug()<<"editStarted";
        emit edit_Started(r,c);
        return portfolio->BuyTotalQuantity;
    }
    else if(c==PortfolioData_Idx::_OrderQuantity){
        emit edit_Started(r,c);
        return portfolio->OrderQuantity;
    }
    else if(c==PortfolioData_Idx::_Alias){
        emit edit_Started(r,c);
        return portfolio->Alias;
    }
    else if(c==PortfolioData_Idx::_MaxLoss){
        emit edit_Started(r,c);
        return double_to_Human_Readable(portfolio->MaxLoss,decimal_precision);

    }
    else
        return QVariant();
}

default:
    return QVariant();
}


return QVariant();

}

// This called when  the user edits a cell and the delegate commits the new value to the model.
bool Table_Portfolios_Model::setData(const QModelIndex &index, const QVariant &value, int role)
{
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---setData";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
    PortfolioObject* P_Obj = portfolio_data_list.at(index.row());


#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---setData";
#endif
    int c=index.column();


    //editing row swithced, make previous row flg not editing
    if(current_editingRow!=index.row()){
        if(current_editingRow!=-1){
            portfolio_data_list[current_editingRow]->edting.storeRelaxed(0);
        }
        editingDataHash.clear();
    }

    current_editingRow = index.row();


    if (role == Qt::CheckStateRole
        && index.column() == PortfolioData_Idx::_Status)
    {
        P_Obj->StatusVal = QString::number(value.toInt());
        emit dataChanged(index, index, {role});
        emit updateDBOnDataChanged(index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

        return true;
    }
    if(role== Qt::EditRole)
    {
        if (!checkIndex(index)){
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return false;
        }
        if(c==PortfolioData_Idx::_SellPriceDifference){
            //store data to editing hash if it not equal to previous value

            if(P_Obj->SellPriceDifference != value.toDouble()){
                editingDataHash[c].prevVal = QString::number(P_Obj->SellPriceDifference);
                editingDataHash[c].newVal = value.toString();
            }
            portfolio_data_list[index.row()]->SellPriceDifference = value.toDouble();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;
        }
        else if(c==PortfolioData_Idx::_BuyPriceDifference){
            //store data to editing hash if it not equal to previous value
            if(P_Obj->BuyPriceDifference != value.toDouble()){
                editingDataHash[c].prevVal = QString::number(P_Obj->BuyPriceDifference);
                editingDataHash[c].newVal = value.toString();
            }

            portfolio_data_list[index.row()]->BuyPriceDifference = value.toDouble();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;


        }
        else if(c==PortfolioData_Idx::_SellTotalQuantity){
            //store data to editing hash if it not equal to previous value
            if(P_Obj->SellTotalQuantity != value.toDouble()){
                editingDataHash[c].prevVal = QString::number(P_Obj->SellTotalQuantity);{}
                editingDataHash[c].newVal = value.toString();
            }
            portfolio_data_list[index.row()]->SellTotalQuantity = value.toDouble();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;


        }
        else if(c==PortfolioData_Idx::_BuyTotalQuantity){
            //store data to editing hash if it not equal to previous value

            if(P_Obj->BuyTotalQuantity != value.toDouble()){
                editingDataHash[c].prevVal = QString::number(P_Obj->BuyTotalQuantity);
                editingDataHash[c].newVal = value.toString();
            }
            portfolio_data_list[index.row()]->BuyTotalQuantity = value.toDouble();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;


        }
        else if(c==PortfolioData_Idx::_OrderQuantity){
            //store data to editing hash if it not equal to previous value
            if(P_Obj->OrderQuantity != value.toDouble()){
                editingDataHash[c].prevVal = QString::number(P_Obj->OrderQuantity);
                editingDataHash[c].newVal = value.toString();
            }

            portfolio_data_list[index.row()]->OrderQuantity = value.toDouble();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;

        }
        else if(c==PortfolioData_Idx::_Alias){
            //store data to editing hash if it not equal to previous value


            if(P_Obj->Alias != value.toString()){
                editingDataHash[c].prevVal = P_Obj->Alias;
                editingDataHash[c].newVal = value.toString();
            }
            portfolio_data_list[index.row()]->Alias = value.toString();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;

        }
        else if(c==PortfolioData_Idx::_Depth){
            //store data to editing hash if it not equal to previous value


            if(P_Obj->Depth != value.toInt()){
                editingDataHash[c].prevVal = QString::number(P_Obj->Depth);
                editingDataHash[c].newVal = value.toString();
            }
            portfolio_data_list[index.row()]->Depth = value.toInt();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
            qDebug()<<"Exiting---setData";
#endif

            return true;

        }

        else if(c==PortfolioData_Idx::_MaxLoss){
            //store data to editing hash if it not equal to previous value
            if(P_Obj->MaxLoss != value.toDouble()){
                editingDataHash[c].prevVal = QString::number(P_Obj->MaxLoss);
                editingDataHash[c].newVal = value.toString();
            }

            portfolio_data_list[index.row()]->MaxLoss = value.toDouble();
            emit dataChanged(index, index);
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif

            return true;

        }


        else{
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif
            return false;
        }

    }
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Exiting---setData";
#endif
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
    if(!portfolio_tokens.contains(p->FutToken)){
        portfolio_tokens.append(p->FutToken);
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

void Table_Portfolios_Model::updateModelDataList(QList <PortfolioObject*> portfolio_data_list_new,bool clearTableFlg){
#ifdef MUTEX_DEBUG_LOG
        qDebug()<<"Entering---updateModelDataList";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---updateModelDataList";
#endif


    slowDataPriceUpdateTimer.restart(); // The price is already updated so do it on next slow date receive

    if(clearTableFlg == true){
//        QList <int> portfolio_ids_ToRemove;
//        for(int i=0;i<portfolio_data_list.length();i++){
//           portfolio_ids_ToRemove.append(i);
//        }
//        for(int i = 0;i<portfolio_ids_ToRemove.length();i++){
//            beginRemoveRows(QModelIndex(), portfolio_ids_ToRemove[i]-i, portfolio_ids_ToRemove[i]-i); //substract i means each time one item removed the index need to be decrement by 1
//            PortfolioObject* portfolio = portfolio_data_list[portfolio_ids_ToRemove[i]-i];
//            portfolio_data_list.removeOne(portfolio);//substract i means each time one item removed the index need to be decrement by 1
//            delete portfolio;
//            endRemoveRows();
//        }
            beginResetModel();  // Signals that a massive change is happening.
            qDeleteAll(portfolio_data_list);  // Deletes all portfolio objects.
            portfolio_data_list.clear();      // Clears the list.
            endResetModel();  // Signals that the model data has been reset.
    }

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
       // QFont font;
        //font.setFamily("Work Sans");
       // font.setPointSize(12);
       // QFontMetrics fontMetrics(font);

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
                            int col = columnCount()-1;// this need to be updated according to the changed column, if needed in future
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


    /// --- Note:Block Signals Temporarily:
    ///  beginRemoveRows() triggers internal model signals (like selectionChanged), which leads to a deadlock when the same mutex is locked inside both the updateModelDataList()
    ///  and selectionChangedSlot() functions.
    ///  The blockSignals(true) call disables all signals from the model during the row removal process.
    ///  This prevents the selectionChangedSlot from being triggered during the row removal.
  //  bool signalsBlocked = this->blockSignals(true);

  //remove rows
    for(int i = 0;i<portfolio_ids_ToRemove.length();i++){
        beginRemoveRows(QModelIndex(), portfolio_ids_ToRemove[i]-i, portfolio_ids_ToRemove[i]-i); //substract i means each time one item removed the index need to be decrement by 1
        PortfolioObject* portfolio = portfolio_data_list[portfolio_ids_ToRemove[i]-i];
        portfolio_data_list.removeOne(portfolio);//substract i means each time one item removed the index need to be decrement by 1
        delete portfolio;
        endRemoveRows();
    }

   // this->blockSignals(signalsBlocked);

    portfolio_ids_ToRemove.clear();
    for (PortfolioObject* portfolioDel : portfolio_data_list_new)
    {
        delete portfolioDel;
    }
    portfolio_data_list_new.clear();
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---updateModelDataList";
#endif
}
/*
void Table_Portfolios_Model::clearTable(){
    QMutexLocker locker(&mutex); // Lock the mutex automatically

    QList <int> portfolio_ids_ToRemove;
    for(int i=0;i<portfolio_data_list.length();i++){
       portfolio_ids_ToRemove.append(i);
    }
    for(int i = 0;i<portfolio_ids_ToRemove.length();i++){
        beginRemoveRows(QModelIndex(), portfolio_ids_ToRemove[i]-i, portfolio_ids_ToRemove[i]-i); //substract i means each time one item removed the index need to be decrement by 1
        PortfolioObject* portfolio = portfolio_data_list[portfolio_ids_ToRemove[i]-i];
        portfolio_data_list.removeOne(portfolio);//substract i means each time one item removed the index need to be decrement by 1
        delete portfolio;
        endRemoveRows();
    }
}
*/




Qt::ItemFlags Table_Portfolios_Model::flags(const QModelIndex &index) const
{
    int c=index.column();
    if(c==PortfolioData_Idx::_SellPriceDifference ||
        c==PortfolioData_Idx::_BuyPriceDifference ||
        c==PortfolioData_Idx::_SellTotalQuantity ||
        c==PortfolioData_Idx::_BuyTotalQuantity ||
        c==PortfolioData_Idx::_OrderQuantity ||
        c==PortfolioData_Idx::_Alias||
        c==PortfolioData_Idx::_MaxLoss||
        c==PortfolioData_Idx::_Depth)
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
/*
QHash<QString,PortFolioData_Less> Table_Portfolios_Model::getPortFolioDataLess(){
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---getPortFolioDataLess";
#endif
   // QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---getPortFolioDataLess";
#endif
    QHash<QString,PortFolioData_Less>  PortFolioDataHash;
    for(int i=0;i<portfolio_data_list.length();i++){
        PortFolioData_Less P;
        P.PortfolioType = portfolio_data_list[i]->PortfolioType;
        P.Expiry =  portfolio_data_list[i]->Expiry;
        PortFolioDataHash.insert(QString::number(portfolio_data_list[i]->PortfolioNumber),P);

    }
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---getPortFolioDataLess";
#endif
    return PortFolioDataHash;
}*/
/*
QHash<QString,int> Table_Portfolios_Model::getPortFoliosLotSize(){
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---getPortFoliosLotSize";
#endif
    //QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---getPortFoliosLotSize";
#endif
    QHash<QString,int>  PortFolioLotSizeHash;
    for(int i=0;i<portfolio_data_list.length();i++){
        PortFolioData_Less P;
        int lotSize = portfolio_data_list[i]->GetLotSize();
        PortFolioLotSizeHash.insert(QString::number(portfolio_data_list[i]->PortfolioNumber),lotSize);

    }
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---getPortFoliosLotSize";
#endif
    return PortFolioLotSizeHash;
}*/

void Table_Portfolios_Model::refreshTable(){
    emit layoutChanged();
}


/*
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

        tableView->setColumnWidth(_Status,45);
        tableView->setColumnWidth(_AlgoName,300);
        tableView->horizontalHeader()->setSectionResizeMode(_Status, QHeaderView::Fixed);

//        // For all other columns, allow them to resize to fit content
//        for (int col = 0; col < tableView->model()->columnCount(); ++col) {
//            if (col != _Status) {
//                tableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::ResizeToContents);  // Resize to fit content
//                tableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Interactive);       // Allow manual stretching
//            }
//        }

//        // Stretch the last column to fill remaining space, if needed
//        tableView->horizontalHeader()->setStretchLastSection(true);

//        // Adjust row height to fit content
//        tableView->resizeRowsToContents();


    }
}*/

// This function will be called when slow data receved
void Table_Portfolios_Model::updateMarketRate(const QHash<QString, MBP_Data_Struct>& MBP_Data_Hash){
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---updateMarketRate";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---updateMarketRate";
#endif
    if (slowDataPriceUpdateTimer.elapsed() >= 300) {
             PortfolioParser portfolioPareser;
            // qDebug()<<"updateMarketRate...";

             for(int i=0;i<portfolio_data_list.length();i++){
                 // update only if the current row is not editing
                 //if(portfolio_data_list[i]->edting.loadRelaxed()==0){
                     portfolioPareser.CalculatePriceDifference(*portfolio_data_list[i],MBP_Data_Hash,devicer,decimal_precision);
                    // portfolio_data_list[i]->BuyMarketRate = 1000;
                    // portfolio_data_list[i]->SellMarketRate = 1000;
                     //QModelIndex topLeft = index(i, PortfolioData_Idx::_BuyMarketRate);
                     //QModelIndex bottomRight = index(i, PortfolioData_Idx::_SellMarketRate);
                     //emit dataChanged(topLeft, bottomRight);
                     emit dataChanged(index(i, PortfolioData_Idx::_BuyMarketRate), index(i, PortfolioData_Idx::_BuyMarketRate));
                     emit dataChanged(index(i, PortfolioData_Idx::_SellMarketRate), index(i, PortfolioData_Idx::_SellMarketRate));
                     emit dataChanged(index(i, PortfolioData_Idx::_FuturePrice), index(i, PortfolioData_Idx::_FuturePrice));


                // }
             }
            // Reset the timer
            slowDataPriceUpdateTimer.restart();
        }
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---updateMarketRate";
#endif
}
PortfolioObject* Table_Portfolios_Model::getPortFolioAt(int idx) const {
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---getPortFolioAt";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---getPortFolioAt";
#endif


        // Check if the index is within the valid range
        if (idx < 0 || idx >= portfolio_data_list.size()) {
            return nullptr; // Return a null pointer if idx is out of range
        }

          // Get the portfolio at the specified index
           PortfolioObject* originalPortfolio = portfolio_data_list.at(idx);
           // Create a deep copy of the portfolio object
           PortfolioObject* copiedPortfolio = new PortfolioObject(*originalPortfolio);
#ifdef MUTEX_DEBUG_LOG
           qDebug()<<"Exiting---getPortFolioAt";
#endif
           return copiedPortfolio; // Return the deep copy
}

void Table_Portfolios_Model::updatePortFolioStatusValue(int row,QString statusVal) {
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---updatePortFolioStatusValue";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---updatePortFolioStatusValue";
#endif
    portfolio_data_list[row]->StatusVal = statusVal;
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---updatePortFolioStatusValue";
#endif
}

void Table_Portfolios_Model::updateNewleyTradeHilgiht(QStringList newleyTraderAlgos){

#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---updateNewleyTradeHilgiht";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---updateNewleyTradeHilgiht";
#endif




#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---updateNewleyTradeHilgiht";
#endif

    for(int i=0;i<portfolio_data_list.length();i++){
        for(int j=0;j<newleyTraderAlgos.length();j++){
            if(TradedHighlight_ExcludeList.contains(newleyTraderAlgos[j]))
                TradedHighlight_ExcludeList.removeOne(newleyTraderAlgos[j]);
            if(portfolio_data_list[i]->TradedHighlight == false&&portfolio_data_list[i]->PortfolioNumber == newleyTraderAlgos[j].toInt()){
                portfolio_data_list[i]->TradedHighlight = true; // make this flag true to hilgiht newly traded data
                emit dataChanged(index(i,0), index(i, header.count()));

            }
        }
    }
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Data";
    QSettings settings(appDataPath + "/tradedHighlight_ExcludeList.dat", QSettings::IniFormat);
    settings.setValue("tradedHighlight_ExcludeList", TradedHighlight_ExcludeList);

}

void Table_Portfolios_Model::setEditingFlg(int row,int val) {


#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---setEditingFlg";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---setEditingFlg";
#endif
    portfolio_data_list[row]->edting.storeRelaxed(val);
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---setEditingFlg";
#endif

}
QStringList Table_Portfolios_Model::getAllPortfolioNumbers() {
    QStringList portfolioNumbers;
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---getAllPortfolioNumbers";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---getAllPortfolioNumbers";
#endif
    for(int i = 0; i < portfolio_data_list.size(); i++){
        if (portfolio_data_list[i]) {
            portfolioNumbers.append(QString::number(portfolio_data_list[i]->PortfolioNumber));
        }
    }
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---getAllPortfolioNumbers";
#endif
   return portfolioNumbers;
}


// Implementation of removeRowsByIndices
bool Table_Portfolios_Model::removeRowsByIndices(const QList<int> &portFolioIdxToDelete)
{
    if (portFolioIdxToDelete.isEmpty())
        return false;

#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---removeRowsByIndices";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---removeRowsByIndices";
#endif

    if (portFolioIdxToDelete.isEmpty())
            return false;

        // Sort the indices in descending order to avoid index shifting
        QList<int> sortedIndices = portFolioIdxToDelete;
        std::sort(sortedIndices.begin(), sortedIndices.end(), std::greater<int>());

        // Start removing rows in reverse order
        for (int row : sortedIndices) {
            if (row < 0 || row >= portfolio_data_list.size()) {
                continue;  // Skip invalid rows
            }

            beginRemoveRows(QModelIndex(), row, row);

            // Remove the portfolio object at the current row and delete it if needed
            delete portfolio_data_list.takeAt(row);

            endRemoveRows();
        }

#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---removeRowsByIndices";
#endif

    return true;
}


QStringList Table_Portfolios_Model::getActivatedPortfolios() {
   QStringList portfolioNumbers;
  // int activatedCount = 0;
#ifdef MUTEX_DEBUG_LOG
   qDebug()<<"Entering---getActivatedPortfolioCount";
#endif
   QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
   qDebug()<<"Setting Mutex---getActivatedPortfolioCount";
#endif
   for(int i = 0; i < portfolio_data_list.size(); i++){
        if (portfolio_data_list[i]->Status) {
             portfolioNumbers.append(QString::number(portfolio_data_list[i]->PortfolioNumber));
            //activatedCount++;
        }
   }
#ifdef MUTEX_DEBUG_LOG
   qDebug()<<"Exiting---getActivatedPortfolioCount";
#endif
   return portfolioNumbers;
}


void Table_Portfolios_Model::editCancelled(int row) {
    qDebug()<<"editCancelled---"<<row;
  #ifdef MUTEX_DEBUG_LOG
     qDebug()<<"Entering---editCancelled";
  #endif
     QMutexLocker locker(&mutex); // Lock the mutex automatically
  #ifdef MUTEX_DEBUG_LOG
     qDebug()<<"Setting Mutex---editCancelled";
  #endif
     if (!editingDataHash.isEmpty()) {
         // Initialize min and max with the first key in the hash
         int minKey = editingDataHash.constBegin().key();
         int maxKey = minKey;
         QHash<int, Cell_Cache>::const_iterator i;
         for (i = editingDataHash.constBegin(); i != editingDataHash.constEnd(); ++i) {
             int columnIdx = i.key();
             QString valStr = i.value().newVal;
             // Update minKey and maxKey
             if (columnIdx < minKey) {
                minKey = columnIdx;
             }
             if (columnIdx > maxKey) {
                maxKey = columnIdx;
             }
             switch (columnIdx) {

                 case PortfolioData_Idx::_SellTotalQuantity:{
                    portfolio_data_list[row]->SellTotalQuantity = editingDataHash[PortfolioData_Idx::_SellTotalQuantity].prevVal.toInt();
                 }
                 break;
                 case PortfolioData_Idx::_BuyTotalQuantity:{
                    portfolio_data_list[row]->BuyTotalQuantity = editingDataHash[PortfolioData_Idx::_BuyTotalQuantity].prevVal.toInt();
                 }
                 break;
                 case PortfolioData_Idx::_OrderQuantity:{
                    portfolio_data_list[row]->OrderQuantity = editingDataHash[PortfolioData_Idx::_OrderQuantity].prevVal.toInt();
                 }
                 break;
                 case PortfolioData_Idx::_BuyPriceDifference:{
                    portfolio_data_list[row]->BuyPriceDifference = editingDataHash[PortfolioData_Idx::_BuyPriceDifference].prevVal.toDouble();
                 }
                 break;

                 case PortfolioData_Idx::_SellPriceDifference:{
                    portfolio_data_list[row]->SellPriceDifference = editingDataHash[PortfolioData_Idx::_SellPriceDifference].prevVal.toDouble();
                 }
                 break;

                 case PortfolioData_Idx::_Alias:{
                    portfolio_data_list[row]->Alias = editingDataHash[PortfolioData_Idx::_Alias].prevVal;
                 }
                 break;

                 case PortfolioData_Idx::_Depth:{
                    portfolio_data_list[row]->Depth = editingDataHash[PortfolioData_Idx::_Depth].prevVal.toInt();
                 }
                 break;

                 case PortfolioData_Idx::_MaxLoss:{
                    portfolio_data_list[row]->MaxLoss = editingDataHash[PortfolioData_Idx::_MaxLoss].prevVal.toDouble();
                 }
                 break;




                 default:{

                 }
                 break;

           }
       }

      emit dataChanged(index(row, minKey), index(row, maxKey));

     }


     portfolio_data_list[row]->edting.storeRelaxed(0);
     editingDataHash.clear();

  #ifdef MUTEX_DEBUG_LOG
     qDebug()<<"Exiting---editCancelled";
  #endif



}


QStringList Table_Portfolios_Model::getHeaders(){
     return header;
}

QList<portfolioImportExportData> Table_Portfolios_Model::getPortFolioToExport(){
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Entering---getPortFolioToExport";
#endif
    QMutexLocker locker(&mutex); // Lock the mutex automatically
#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Setting Mutex---getPortFolioToExport";
#endif
    QList <portfolioImportExportData> p_data_to_export;
    for(int i=0;i<portfolio_data_list.size();i++){
        if(portfolio_data_list[i]->BuyTradedQuantity > 0 || portfolio_data_list[i]->SellTradedQuantity  > 0 ){
            portfolioImportExportData tmp;
            tmp.PortfolioType = portfolio_data_list[i]->PortfolioType;
            tmp.TraderID = QString::number(portfolio_data_list[i]->TraderID);
            tmp.ClientID = QString::number(portfolio_data_list[i]->ClientID);
            tmp.Status  =  "DisabledByUser";//portfolio_data_list[i]->StatusVal;
            tmp.IsBroker = QString::number(portfolio_data_list[i]->IsBroker);
            tmp.Leg1TokenNo  = QString::number(portfolio_data_list[i]->Leg1TokenNo);
            tmp.Leg2TokenNo  = QString::number(portfolio_data_list[i]->Leg2TokenNo);
            tmp.Leg3TokenNo  = QString::number(portfolio_data_list[i]->Leg3TokenNo);
            tmp.Leg4TokenNo  = QString::number(portfolio_data_list[i]->Leg4TokenNo);
            tmp.Leg5TokenNo  = QString::number(portfolio_data_list[i]->Leg5TokenNo);
            tmp.Leg6TokenNo  = QString::number(portfolio_data_list[i]->Leg6TokenNo);
            tmp.SellPriceDifference  = "0";//QString::number(portfolio_data_list[i]->SellPriceDifference);
            tmp.SellTotalQuantity = "0";//QString::number(portfolio_data_list[i]->SellTotalQuantity*portfolio_data_list[i]->GetLotSize());
            tmp.SellTradedQuantity = "0";//QString::number(portfolio_data_list[i]->SellTradedQuantity*portfolio_data_list[i]->GetLotSize());
            tmp.BuyPriceDifference = "0";//QString::number(portfolio_data_list[i]->BuyPriceDifference);
            tmp.BuyTotalQuantity = "0";//QString::number(portfolio_data_list[i]->BuyTotalQuantity*portfolio_data_list[i]->GetLotSize());
            tmp.BuyTradedQuantity = "0";//QString::number(portfolio_data_list[i]->BuyTradedQuantity*portfolio_data_list[i]->GetLotSize());
            tmp.AdditionalData1  = portfolio_data_list[i]->AdditionalData1;
            tmp.AdditionalData2  = portfolio_data_list[i]->AdditionalData2;
            tmp.AdditionalData3  = portfolio_data_list[i]->AdditionalData3;
            tmp.AdditionalData4  = portfolio_data_list[i]->AdditionalData4;
            tmp.Alias  = portfolio_data_list[i]->Alias;
            p_data_to_export.append(tmp);
        }
    }

#ifdef MUTEX_DEBUG_LOG
    qDebug()<<"Exiting---getPortFolioToExport";
#endif
    return p_data_to_export; // Return the deep copy
}
