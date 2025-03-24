#include "Convert_to_Algo/convert_to_algo_bfly_bid.h"
#include "custom_q_completer.h"
#include "contractdetail.h"
#include "QElapsedTimer"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>


add_algo_btfly_bid::add_algo_btfly_bid(QObject *parent)
    : QObject{parent}
{
    //  model_start_strike_BFLY = new QStandardItemModel;
    sharedData = &AddAlgoSharedVar::getInstance();
    model_end_strike = new QStandardItemModel;
    model_start_strike_BFLY_BID = new QStandardItemModel;
    BFLY_BID_Tokens = ContractDetail::getInstance().Get_Tokens_For_PortfolioType(PortfolioType::BFLY_BID);


}

void add_algo_btfly_bid::clearAllModel(){
    model_end_strike->clear();
    model_start_strike_BFLY_BID->clear();
}

void add_algo_btfly_bid::copyUIElement(QDialog *parentWidget_,QTableWidget *tableWidget_, QLineEdit *lineEdit_Start_strike_, QLineEdit *lineEdit_EndStrike_, QLineEdit *lineEdit_StrikeDifference_,QPushButton *addButton_){

    lineEdit_Start_strike = lineEdit_Start_strike_;
    lineEdit_EndStrike = lineEdit_EndStrike_;
    lineEdit_StrikeDifference = lineEdit_StrikeDifference_;
    tableWidget = tableWidget_;
    addButton = addButton_;

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // to make floating window
    startStrikeListView = new QListView(parentWidget_);
    connect(startStrikeListView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemSelectedStartStrike(QModelIndex)));
    startStrikeListView->setSizePolicy(sizePolicy);
    startStrikeListView->setFixedSize(230, 200);
    startStrikeListView->setEditTriggers(QAbstractItemView::NoEditTriggers);




     endStrikeListView = new QListView(parentWidget_);
     connect(endStrikeListView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemSelectedEndStrike(QModelIndex)));
     endStrikeListView->setSizePolicy(sizePolicy);
     endStrikeListView->setFixedSize(230, 200);
     endStrikeListView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    startStrikeListView->hide();
    endStrikeListView->hide();


    // Install event filter on the QLineEdit and QListView
    eventFilterStart = new EventFilter(lineEdit_Start_strike, startStrikeListView);
    connect(eventFilterStart, SIGNAL(signalItemSelected(QModelIndex)), this, SLOT(itemSelectedStartStrike(QModelIndex)));
    lineEdit_Start_strike->installEventFilter(eventFilterStart);
    startStrikeListView->installEventFilter(eventFilterStart);

    eventFilterEnd = new EventFilter(lineEdit_EndStrike, endStrikeListView);
    connect(eventFilterEnd, SIGNAL(signalItemSelected(QModelIndex)), this, SLOT(itemSelectedEndStrike(QModelIndex)));
    lineEdit_EndStrike->installEventFilter(eventFilterEnd);
    endStrikeListView->installEventFilter(eventFilterEnd);


    //model_start_strike_BFLY_BID = ContractDetail::getInstance().Get_model_start_strike_BFLY_BID();
    //create qcompleter and fill with abovie model
    CustomSearchWidget *startstrikeCustomWidget = new CustomSearchWidget(startStrikeListView,model_start_strike_BFLY_BID);
    startstrikeCustomWidget->name = "startStrike";
    connect(lineEdit_Start_strike, SIGNAL(textEdited(QString)),startstrikeCustomWidget, SLOT(filterItems(QString)));
    connect(lineEdit_Start_strike, SIGNAL(textChanged(QString)),this, SLOT(slotStartHide(QString)));
    connect(lineEdit_EndStrike, SIGNAL(textChanged(QString)),this, SLOT(slotEndHide(QString)));

    // create qcompleter and fill with above model
    CustomSearchWidget *endstrikeCustomWidget = new CustomSearchWidget(endStrikeListView,model_end_strike);
    endstrikeCustomWidget->name = "endStrike";
    connect(lineEdit_EndStrike, SIGNAL(textEdited(QString)),endstrikeCustomWidget, SLOT(filterItems(QString)));

}


void add_algo_btfly_bid::selectedAction(){

    model_start_strike_BFLY_BID->clear();
    model_end_strike->clear();


//    for(int i=0;i<BFLY_BID_Tokens.length();i++){

//        /**********Create model for BFLY BID*************************/
//        const auto& contract = sharedData->contract_table_hash[BFLY_BID_Tokens[i]];
//        unsigned int  unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        int targetYear = dt.date().year();
//        bool isLeapYear = QDate::isLeapYear(targetYear);

//        // If it is a leap year, and the date is after Feb 29, subtract one day
//        if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//            dt = dt.addDays(-1);
//        }
//        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
//        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" "+contract.OptionType;
//        QStandardItem *itemBFLY = new QStandardItem;
//        itemBFLY->setText(algo_combination);
//        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
//        QString strik_price = QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);

//        // Create custom data for sorting
//        QString compositeKey = contract.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
//        // Set the composite key as data for sorting
//        itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
//        model_start_strike_BFLY_BID->appendRow(itemBFLY);

//        /********************************************************************/

//    }

    // Create a progress dialog to show the progress




        // Create a lambda function for processing in the background
            QFuture<void> future = QtConcurrent::run([=]() {
            QElapsedTimer timer1;
            timer1.start();
            emit progressSignal(true,"Loading...");
            for (int i = 0; i < BFLY_BID_Tokens.length(); ++i) {

                /**********Create model for BFLY BID*************************/
                const auto &contract = sharedData->contract_table_hash[BFLY_BID_Tokens[i]];
                unsigned int unix_time = contract.Expiry;
                /*QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
                dt = dt.addYears(10);
                int targetYear = dt.date().year();
                bool isLeapYear = QDate::isLeapYear(targetYear);

                // If it is a leap year, and the date is after Feb 29, subtract one day
                if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
                    dt = dt.addDays(-1);
                }*/
                /*QDateTime dt;
                dt.setOffsetFromUtc(0);
                dt.setDate(QDate(1980, 1, 1));
                dt.setTime(QTime(0, 0, 0));
                QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_time);
                dt1 = dt1.addYears(10);
                QString Expiry = dt.addSecs(unix_time).toString("MMM dd ")+QString::number(dt1.date().year()).toUpper();*/
                //

                QDateTime dt;
                dt.setOffsetFromUtc(0);
                dt.setDate(QDate(1980, 1, 1));
                dt.setTime(QTime(0, 0, 0));
                dt = dt.addSecs(unix_time);
                QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_time);
                dt1 = dt1.addYears(10);
                QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
                dt.setDate(newDate);
                QString Expiry = dt.toString("MMM dd yyyy").toUpper();

                QString algo_combination = contract.InstrumentName + " " + Expiry + " " +
                                           QString::number(contract.StrikePrice / sharedData->strike_price_devider, 'f', sharedData->decimal_precision) + " " +
                                           contract.OptionType;
                QStandardItem *itemBFLY = new QStandardItem;
                itemBFLY->setText(algo_combination);
                itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
                QString strik_price = QString::number(contract.StrikePrice / sharedData->strike_price_devider, 'f', sharedData->decimal_precision);

                // Create custom data for sorting
                QString compositeKey = contract.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
                // Set the composite key as data for sorting
                itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
               // model_start_strike_BFLY_BID->appendRow(itemBFLY);
                QMetaObject::invokeMethod(this, [=]() {
                    model_start_strike_BFLY_BID->appendRow(itemBFLY);
                }, Qt::QueuedConnection);

                /********************************************************************/

                // Update progress bar in the main thread
            }
            emit progressSignal(false,"");


            qDebug()<<"model_start_strike_BFLY_BID Total data:"<<BFLY_BID_Tokens.count() << ", model_start_strike_BFLY_BID  Time:" << timer1.elapsed() << "milliseconds";

            // Close the progress dialog once done
        });



    foo_token_number_start_strike = "";
    foo_token_number_end_strike = "";

    lineEdit_Start_strike->clear();
    lineEdit_EndStrike->clear();
    lineEdit_StrikeDifference->clear();

    startStrikeListView->hide();
    endStrikeListView->hide();



    // Get the global position of lineEdit_Start_strike
    QPoint globalPos = lineEdit_Start_strike->mapToGlobal(lineEdit_Start_strike->geometry().bottomLeft());
    // Convert the global position to the dialog's coordinate system
    QPoint posSS = startStrikeListView->parentWidget()->mapFromGlobal(globalPos);
    // Move the startStrikeListView to the new position
    startStrikeListView->move(posSS.x(), posSS.y());

    globalPos = lineEdit_Start_strike->mapToGlobal(lineEdit_EndStrike->geometry().bottomLeft());
    QPoint posES = startStrikeListView->parentWidget()->mapFromGlobal(globalPos);
    endStrikeListView->move(posES.x(), posES.y());
}


void add_algo_btfly_bid::startStrikeEditFinishedAction(){


    model_end_strike->clear();
    lineEdit_EndStrike->clear();
    foo_token_number_end_strike = "";
    expiry_dateList.clear();
    QString key = foo_token_number_start_strike;
    QString Instr_Name = sharedData->contract_table_hash[key].InstrumentName;
    QString Option_Type =sharedData->contract_table_hash[key].OptionType;
    long long Expiry = sharedData->contract_table_hash[key].Expiry;
    float start_strike = sharedData->contract_table_hash[key].StrikePrice;


    for(int i=0;i<BFLY_BID_Tokens.length();i++) {

        contract_table tmp = sharedData->contract_table_hash[BFLY_BID_Tokens[i]];

        float end_strike = tmp.StrikePrice;
        if(start_strike>end_strike)
            continue;

        if(tmp.InstrumentName==Instr_Name&&tmp.OptionType==Option_Type&&Expiry==tmp.Expiry){


            // this will prevent adding same algo_combination  in end strike list view as in start strik list view
            if(start_strike==end_strike)
                continue;

            unsigned int unix_time= tmp.Expiry;
            /*QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
            dt = dt.addYears(10);
            int targetYear = dt.date().year();
            bool isLeapYear = QDate::isLeapYear(targetYear);

            // If it is a leap year, and the date is after Feb 29, subtract one day
            if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
                dt = dt.addDays(-1);
            }
            QString ExpiryTmp=dt.toString("MMM dd yyyy").toUpper();*/
            //QString ExpiryTmp = dt.addSecs(unix_time).toString("MMM DD ")+QString::number(dt1.date().year());

            QDateTime dt;
            dt.setOffsetFromUtc(0);
            dt.setDate(QDate(1980, 1, 1));
            dt.setTime(QTime(0, 0, 0));
            dt = dt.addSecs(unix_time);
            QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_time);
            dt1 = dt1.addYears(10);
            QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
            dt.setDate(newDate);

            QString ExpiryTmp=dt.toString("MMM dd yyyy").toUpper();

            QString strik_price = QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
            QString algo_combination =
                tmp.InstrumentName+" "+
                ExpiryTmp.toUpper()+" "
                +strik_price+" "+
                tmp.OptionType;
            QStandardItem *item = new QStandardItem;
            item->setText(algo_combination);
            item->setData(tmp.TokenNumber, Qt::UserRole + 1);
            QString compositeKey = tmp.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
            item->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
            model_end_strike->appendRow(item);




        }
    }
}

void add_algo_btfly_bid::generateAlgo()
{
    if(foo_token_number_start_strike=="" || foo_token_number_end_strike==""){
        QMessageBox msgBox;
        msgBox.setText("Cannot Use current Algo combination.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QString keyStart = foo_token_number_start_strike;
    QString keyEnd = foo_token_number_end_strike;
    QString Start_Strike_Option_Type = sharedData->contract_table_hash[keyStart].OptionType;
    QString Start_Strike_InstrumentName = sharedData->contract_table_hash[keyStart].InstrumentName;

    if(lineEdit_StrikeDifference->text()==""){
        QMessageBox msgBox;
        msgBox.setText("Please fill Strike Difference.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    float StrikeDifference = lineEdit_StrikeDifference->text().toFloat()*sharedData->strike_price_devider; /// will be in Rs
    if(StrikeDifference<=0){
        QMessageBox msgBox;
        msgBox.setText("Please Enter Valid  Strike Difference.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    float startStrike =  sharedData->contract_table_hash[keyStart].StrikePrice; // will be in paise so converted to Rs
    float endStrike = sharedData->contract_table_hash[keyEnd].StrikePrice;// will be in paise so converted to Rs
    long long Start_Strike_Expiry= sharedData->contract_table_hash[keyStart].Expiry;

    //    QDateTime dt = QDateTime::fromSecsSinceEpoch(Start_Strike_Expiry);
    //    dt = dt.addYears(10);
    //    int targetYear = dt.date().year();
    //    bool isLeapYear = QDate::isLeapYear(targetYear);

    //    // If it is a leap year, and the date is after Feb 29, subtract one day
    //    if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
    //        dt = dt.addDays(-1);
    //    }

    //unsigned int unix_time= tmp.Expiry;
    QDateTime dt;
    dt.setOffsetFromUtc(0);
    dt.setDate(QDate(1980, 1, 1));
    dt.setTime(QTime(0, 0, 0));
    dt = dt.addSecs(Start_Strike_Expiry);
    QDateTime dt1 = QDateTime::fromSecsSinceEpoch(Start_Strike_Expiry);
    dt1 = dt1.addYears(10);
    QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
    dt.setDate(newDate);
    QString ExpiryTime=dt.toString("MMM dd yyyy").toUpper();


    QHash<QString, QString> strikePrice_TokenFiltered;
    QList<int> strikePriceListFiltered;
    for(int i=0;i<BFLY_BID_Tokens.length();i++){
        contract_table c = sharedData->contract_table_hash[BFLY_BID_Tokens[i]];
        if(c.StrikePrice<startStrike||c.StrikePrice>endStrike+StrikeDifference+StrikeDifference)
            continue;

        if(c.InstrumentName==Start_Strike_InstrumentName&&c.OptionType==Start_Strike_Option_Type&&c.Expiry==Start_Strike_Expiry){
            QString key = QString::number(c.StrikePrice);
            strikePrice_TokenFiltered.insert(key,QString::number(c.TokenNumber));
            strikePriceListFiltered.append(c.StrikePrice);
        }
    }
    std::sort(strikePriceListFiltered.begin(), strikePriceListFiltered.end());

    QList<QString> Leg1_token_number_list;
    QList<QString> Leg2_token_number_list;
    QList<QString> Leg3_token_number_list;
    QList<QString> Algo_Name_list;

    for(int i=0;i<strikePriceListFiltered.length();i++){

        int leg1Strike = strikePriceListFiltered[i];
        int leg2Strike = strikePriceListFiltered[i]+StrikeDifference;
        int leg3Strike = strikePriceListFiltered[i]+StrikeDifference+StrikeDifference;

        QString key1 = QString::number(leg1Strike);
        QString key2 = QString::number(leg2Strike);
        QString key3 = QString::number(leg3Strike);

        if(strikePrice_TokenFiltered.contains(key1)&&strikePrice_TokenFiltered.contains(key2)&&strikePrice_TokenFiltered.contains(key3)){
            Leg1_token_number_list.append(strikePrice_TokenFiltered[key1]);
            Leg2_token_number_list.append(strikePrice_TokenFiltered[key2]);
            Leg3_token_number_list.append(strikePrice_TokenFiltered[key3]);
            QString Algo_Name = "Bfly-" + Start_Strike_InstrumentName+"-"+dt.toString("ddMMM").toUpper()+"-"+QString::number(leg2Strike/sharedData->strike_price_devider,'f', sharedData->decimal_precision)+"-"+QString::number(StrikeDifference/sharedData->strike_price_devider,'f', sharedData->decimal_precision)+"-"+Start_Strike_Option_Type;
            Algo_Name_list.append(Algo_Name);
            if(Algo_Name_list.size()==sharedData->ROW_LIMIT){
                QMessageBox msgBox;
                msgBox.setText("Maximum allowed entry at a time is "+QString::number(sharedData->ROW_LIMIT));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
                break;
            }
        }


    }



    if(Leg1_token_number_list.size()==0){
        QMessageBox msgBox;
        msgBox.setText("Not able to generate algos for given input, please select different strike price.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QStringList duplicateList;
    for(int i=0;i<Leg1_token_number_list.size();i++){
        QString Leg1_Strike=QString::number(sharedData->contract_table_hash[Leg1_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision);
        QString Leg2_Strike=QString::number(sharedData->contract_table_hash[Leg2_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision);
        QString Leg3_Strike=QString::number(sharedData->contract_table_hash[Leg3_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision);


        bool exist=false;
        for(int k=0;k<sharedData->algo_data_list.length();k++){
            algo_data_to_insert tmp = sharedData->algo_data_list[k];
            if(tmp.Algo_Name == Algo_Name_list[i] && tmp.Leg1_token_number == Leg1_token_number_list[i] && tmp.Leg2_token_number == Leg2_token_number_list[i]&& tmp.Leg3_token_number == Leg3_token_number_list[i]){
                exist=true;
                break;
            }
        }

        //check for if item  is already added or upload in table and will not be in sharedData->algo_data_list
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QString Instr_Name_Tmp="";
//            QString Option_Type_tmp = "";
//            QString ExpiryTime_tmp = "";
            QString Leg1_Strike_tmp= "";
            QString Leg2_Strike_tmp= "";
            QString Leg3_Strike_tmp= "";

            QTableWidgetItem *item0 = tableWidget->item(row, 0);
            if (item0) {
                Instr_Name_Tmp = item0->text();
            }
//            QTableWidgetItem *item1 = tableWidget->item(row, 1);
//            if (item1) {
//                Option_Type_tmp = item1->text();
//            }
//            QTableWidgetItem *item2 = tableWidget->item(row, 2);
//            if (item2) {
//                ExpiryTime_tmp = item2->text();
//            }
            QTableWidgetItem *item1 = tableWidget->item(row, 1);
            if (item1) {
                Leg1_Strike_tmp = item1->text();
            }
            QTableWidgetItem *item2 = tableWidget->item(row, 2);
            if (item2) {
                Leg2_Strike_tmp = item2->text();
            }
            QTableWidgetItem *item3 = tableWidget->item(row, 3);
            if (item3) {
                Leg3_Strike_tmp = item3->text();
            }



            if(Algo_Name_list[i]==Instr_Name_Tmp&&
               // Option_Type_tmp==Option_Type&&
               // ExpiryTime_tmp==ExpiryTime&&
                Leg1_Strike_tmp==Leg1_Strike&&
                Leg2_Strike_tmp==Leg2_Strike&&
                Leg3_Strike_tmp==Leg3_Strike
                ){
                exist=true;
                break;
            }

        }

        if(exist){
            duplicateList.append("Duplicate entry Skipping, Leg1="+Leg1_token_number_list[i]+", Leg2="+Leg2_token_number_list[i]+", Leg3="+Leg3_token_number_list[i]);
            continue;
        }

        tableWidget->insertRow(tableWidget->rowCount() );

        sharedData->unique_id++;
        QTableWidgetItem *c0 = new QTableWidgetItem();
        c0->setData(Qt::UserRole + 1,sharedData->unique_id);
        c0->setData(Qt::DisplayRole,Algo_Name_list[i]);
        c0->setFlags(c0->flags() ^ Qt::ItemIsEditable);
        c0->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 0, c0);


//        QTableWidgetItem *c1 = new QTableWidgetItem(Option_Type);
//        c1->setFlags(c1->flags() ^ Qt::ItemIsEditable);
//        tableWidget->setItem(tableWidget->rowCount()-1, 1, c1);

//        QTableWidgetItem *c2 = new QTableWidgetItem(ExpiryTime);
//        c2->setFlags(c2->flags() ^ Qt::ItemIsEditable);
//        tableWidget->setItem(tableWidget->rowCount()-1, 2, c2);

        QTableWidgetItem *c1 = new QTableWidgetItem(Leg1_Strike);
        c1->setFlags(c1->flags() ^ Qt::ItemIsEditable);
        c1->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 1, c1);

        QTableWidgetItem *c2 = new QTableWidgetItem(Leg2_Strike);
        c2->setFlags(c2->flags() ^ Qt::ItemIsEditable);
        c2->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 2, c2);

        QTableWidgetItem *c3 = new QTableWidgetItem(Leg3_Strike);
        c3->setFlags(c3->flags() ^ Qt::ItemIsEditable);
        c3->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 3, c3);

//        QTableWidgetItem *c6 = new QTableWidgetItem("NA");
//        c6->setFlags(c6->flags() ^ Qt::ItemIsEditable);
//        tableWidget->setItem(tableWidget->rowCount()-1, 6, c6);

//        QTableWidgetItem *c7 = new QTableWidgetItem("NA");
//        c7->setFlags(c7->flags() ^ Qt::ItemIsEditable);
//        tableWidget->setItem(tableWidget->rowCount()-1, 7, c7);


        QTableWidgetItem *c4 = new QTableWidgetItem("To be Uploaded");
        c4->setFlags(c4->flags() ^ Qt::ItemIsEditable);
        c4->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 4, c4);

        algo_data_to_insert data;
        data.Algo_Status = "DisabledByUser";
        data.algo_type = QString::number(PortfolioType::BFLY_BID);
        data.exchange = sharedData->exchange;
        data.table_row_unique_id = sharedData->unique_id;
        data.user_id = sharedData->foo_user_id;
        data.Algo_Name = Algo_Name_list[i];
        data.Leg1_token_number = Leg1_token_number_list[i];
        data.Leg2_token_number = Leg2_token_number_list[i];
        data.Leg3_token_number = Leg3_token_number_list[i];
        data.indicator = "1";
        data.Leg1_Strike = Leg1_Strike;
        data.Leg2_Strike = Leg2_Strike;
        data.Leg3_Strike = Leg3_Strike;
        data.option_type = Start_Strike_Option_Type;
        data.uploaded=false; // this flag is used to id

        sharedData->algo_data_list.append(data);
        /*if(tableWidget->rowCount()>=sharedData->ROW_LIMIT){
            QMessageBox msgBox;
            msgBox.setText("Maximum allowed entry at a time is "+QString::number(sharedData->ROW_LIMIT));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            break;
        }*/
    }

    if(duplicateList.size()>0){
        QMessageBox msgBox;
        //msgBox.setText(QString::number(duplicateList.size())+ " Duplicate entry Skipped!");
        msgBox.setText("Duplication entry  is not allowed!!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    tableWidget->resizeColumnsToContents();
}

void add_algo_btfly_bid::slotStartHide(QString)
{
    endStrikeListView->hide();
}

void add_algo_btfly_bid::slotEndHide(QString)
{    startStrikeListView->hide();

}

void add_algo_btfly_bid::itemSelectedStartStrike(QModelIndex index)
{
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        if (dData.isValid())
        {
            for (int row = 0; row < model_start_strike_BFLY_BID->rowCount(); ++row)
            {
                QModelIndex index = model_start_strike_BFLY_BID->index(row, 0);
                // Check if the item's display role value matches
                QVariant displayData = model_start_strike_BFLY_BID->data(index, Qt::DisplayRole);
                if (displayData.isValid() && displayData.toString() == dData)
                {
                    QVariant userData = model_start_strike_BFLY_BID->data(index, Qt::UserRole + 1);
                    if (userData.isValid())
                    {
                        foo_token_number_start_strike = userData.toString();
                        lineEdit_Start_strike->setText(index.data(Qt::DisplayRole).toString());
                        lineEdit_Start_strike->setCursorPosition(0);
                        startStrikeEditFinishedAction();
                        startStrikeListView->hide();
                        lineEdit_EndStrike->setFocus();

                        break;
                    }
                }
            }
        }
    }
    else
    {
        qInfo() << "Not a start strike vaild index";

    }
}

void add_algo_btfly_bid::itemSelectedEndStrike(QModelIndex index)
{
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        if (dData.isValid())
        {
            for (int row = 0; row < model_end_strike->rowCount(); ++row)
            {
                QModelIndex index = model_end_strike->index(row, 0);
                // Check if the item's display role value matches
                QVariant displayData = model_end_strike->data(index, Qt::DisplayRole);
                if (displayData.isValid() && displayData.toString() == dData)
                {
                    QVariant userData = model_end_strike->data(index, Qt::UserRole + 1);
                    if (userData.isValid())
                    {
                        foo_token_number_end_strike = userData.toString();
                        lineEdit_EndStrike->setText(index.data(Qt::DisplayRole).toString());
                        lineEdit_EndStrike->setCursorPosition(0);
                        startStrikeListView->hide();
                        endStrikeListView->hide();
                        lineEdit_StrikeDifference->setFocus();
                        break;
                    }
                }
            }

        }
    }
    else
    {
        qInfo() << "Not an end strike vaild index";
    }

}




