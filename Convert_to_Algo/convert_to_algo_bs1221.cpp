#include "convert_to_algo_bs1221.h"

#include "custom_q_completer.h"
#include "contractdetail.h"
#include "QElapsedTimer"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>

convert_to_algo_bs1221::convert_to_algo_bs1221(QObject *parent)
    : QObject{parent}
{
    //  model_start_strike_BFLY = new QStandardItemModel;
    sharedData = &AddAlgoSharedVar::getInstance();
    model_end_strike = new QStandardItemModel;
    model_start_strike_BX1221 = new QStandardItemModel;
    BX1221_Tokens = ContractDetail::getInstance().Get_Tokens_For_PortfolioType(PortfolioType::BX1221);
}

void convert_to_algo_bs1221::clearAllModel(){
    model_end_strike->clear();
    model_start_strike_BX1221->clear();
}

void convert_to_algo_bs1221::copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_, QLineEdit *lineEdit_Start_strike_, QLineEdit *lineEdit_EndStrike_, QLineEdit *lineEdit_StrikeDifference_){

    lineEdit_Start_strike = lineEdit_Start_strike_;
    lineEdit_EndStrike = lineEdit_EndStrike_;
    lineEdit_StrikeDifference = lineEdit_StrikeDifference_;
    tableWidget = tableWidget_;

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // to make floating window
    startStrikeListView = new QListView(parentWidget);
    connect(startStrikeListView, SIGNAL(clicked(QModelIndex)), this, SLOT(itemSelectedStartStrike(QModelIndex)));
    startStrikeListView->setSizePolicy(sizePolicy);
    startStrikeListView->setFixedSize(230, 200);
    startStrikeListView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    endStrikeListView = new QListView(parentWidget);
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


    //model_start_strike_BX1221 = ContractDetail::getInstance().Get_model_start_strike_BOX_BID();
    //create qcompleter and fill with abovie model
    CustomSearchWidget *startstrikeCustomWidget = new CustomSearchWidget(startStrikeListView,model_start_strike_BX1221);
    startstrikeCustomWidget->name = "startStrike";
    connect(lineEdit_Start_strike, SIGNAL(textEdited(QString)),startstrikeCustomWidget, SLOT(filterItems(QString)));
    connect(lineEdit_Start_strike, SIGNAL(textChanged(QString)),this, SLOT(slotStartHide(QString)));
    connect(lineEdit_EndStrike, SIGNAL(textChanged(QString)),this, SLOT(slotEndHide(QString)));





    // create qcompleter and fill with above model
    CustomSearchWidget *endstrikeCustomWidget = new CustomSearchWidget(endStrikeListView,model_end_strike);
    endstrikeCustomWidget->name = "endStrike";
    connect(lineEdit_EndStrike, SIGNAL(textEdited(QString)),endstrikeCustomWidget, SLOT(filterItems(QString)));

}


void convert_to_algo_bs1221::selectedAction(){

    foo_token_number_start_strike = "";
    foo_token_number_end_strike = "";

    lineEdit_Start_strike->clear();
    lineEdit_EndStrike->clear();
    lineEdit_StrikeDifference->clear();

    startStrikeListView->hide();
    endStrikeListView->hide();

    model_start_strike_BX1221->clear();
    // Create a lambda function for processing in the background
        QFuture<void> future = QtConcurrent::run([=]() {
        QElapsedTimer timer1;
        timer1.start();
        emit progressSignal(true,"Data Model is Loading, Please wait!");
        /**********Create model for BX_BID*************************/

            for(int i=0;i<BX1221_Tokens.length();i++){
                const auto& contract = sharedData->contract_table_hash[BX1221_Tokens[i]];
                //Only CE record need to avoaid duplicate
                if(contract.OptionType=="PE")
                    continue;
                unsigned int unix_time= contract.Expiry;
                QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
                dt = dt.addYears(10);
                // Check if the target year is a leap year
                int targetYear = dt.date().year();
                bool isLeapYear = QDate::isLeapYear(targetYear);

                // If it is a leap year, and the date is after Feb 29, subtract one day
                if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
                    dt = dt.addDays(-1);
                }

                QString Expiry = dt.toString("MMM dd yyyy").toUpper();

                QString instrument_name = contract.InstrumentName;

                QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
                QStandardItem *itemBOX_BID = new QStandardItem;
                itemBOX_BID->setText(algo_combination);
                itemBOX_BID->setData(contract.TokenNumber, Qt::UserRole + 1);

                QString strik_price = QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
                // Create custom data for sorting
                QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
                // Set the composite key as data for sorting
                itemBOX_BID->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
                QMetaObject::invokeMethod(this, [=]() {
                    model_start_strike_BX1221->appendRow(itemBOX_BID);
                 }, Qt::QueuedConnection);

           }
            /********************************************************************/
        emit progressSignal(false,"");


        qDebug() << "model_start_strike_BFLY_BID  Time:" << timer1.elapsed() << "milliseconds";

        // Close the progress dialog once done
    });



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


void convert_to_algo_bs1221::createEndStrikeModelAndPopulateListView(){


    model_end_strike->clear();
    lineEdit_EndStrike->clear();
    foo_token_number_end_strike = "";
    strike_priceList.clear();
    token_numebrList.clear();
    expiry_dateList.clear();
    QString key = foo_token_number_start_strike;
    QString Instr_Name = sharedData->contract_table_hash[key].InstrumentName;
    QString Option_Type =sharedData->contract_table_hash[key].OptionType;
    long long Expiry = sharedData->contract_table_hash[key].Expiry;
    float start_strike = sharedData->contract_table_hash[key].StrikePrice;


    for(int i=0;i<BX1221_Tokens.length();i++) {
        contract_table tmp = sharedData->contract_table_hash[BX1221_Tokens[i]];
        float end_strike = tmp.StrikePrice;
        if(start_strike>end_strike)
            continue;


        if(tmp.InstrumentName==Instr_Name&&Expiry==tmp.Expiry&&tmp.OptionType=="CE"){

            if(!strike_priceList.contains(QString::number(tmp.StrikePrice))){
                strike_priceList.append(QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)); // store in Rs
                token_numebrList.append(QString::number(tmp.TokenNumber));
            }
            // this will prevent adding same algo_combination  in end strike list view as in start strik list view
            if(start_strike==end_strike)
                continue;

            unsigned int unix_time= tmp.Expiry;
            QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
            dt = dt.addYears(10);
            int targetYear = dt.date().year();
            bool isLeapYear = QDate::isLeapYear(targetYear);

            // If it is a leap year, and the date is after Feb 29, subtract one day
            if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
                dt = dt.addDays(-1);
            }
            QString ExpiryTmp=dt.toString("MMM dd yyyy").toUpper();
            QString strik_price = QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
            QString algo_combination =
                tmp.InstrumentName+" "+
                ExpiryTmp+" "
                +strik_price;
            QStandardItem *item = new QStandardItem;
            item->setText(algo_combination);
            item->setData(tmp.TokenNumber, Qt::UserRole + 1);
            QString compositeKey = tmp.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + strik_price;
            item->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
            model_end_strike->appendRow(item);




        }
    }




}
void convert_to_algo_bs1221::generateAlgo()
{


    if(foo_token_number_start_strike=="" || foo_token_number_end_strike==""){
        QMessageBox msgBox;
        msgBox.setText("Cannot Use current Algo combination.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QString startStrikeToken = foo_token_number_start_strike;
    QString endStrikeToken = foo_token_number_end_strike;
    QString Option_Type = sharedData->contract_table_hash[startStrikeToken].OptionType;
    if(lineEdit_StrikeDifference->text()==""){
        QMessageBox msgBox;
        msgBox.setText("Please fill Strike Difference.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    int StrikeDifference = lineEdit_StrikeDifference->text().toFloat()*sharedData->strike_price_devider; /// will be in Rs convert to paise since strike price in Paise
    if(StrikeDifference<=0){
        QMessageBox msgBox;
        msgBox.setText("Please Enter Valid  Strike Difference.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    ///-----------------Logic----------------------------
    //    Start Strike	End Strike	Strike Diff		Contract
    //        80             120          10              80
    //                                                    100
    //                                                    110
    //                                                    120

    //    Strike 1	Strike 2        Strike 3        Strike 4
    //    100 CE      100 PE          110 CE          110 PE
    //    110 CE      110 PE          120 CE          120 PE
    //    120 CE      120 PE          130 CE          130 PE

    QList<QString> Leg1_token_number_list;
    QList<QString> Leg2_token_number_list;
    QList<QString> Leg3_token_number_list;
    QList<QString> Leg4_token_number_list;
    QList<QString> Algo_Name_list;


    int startStrike =  sharedData->contract_table_hash[startStrikeToken].StrikePrice;///sharedData->strike_price_devider; // will be in paise so converted to Rs
    int endStrike = sharedData->contract_table_hash[endStrikeToken].StrikePrice;///sharedData->strike_price_devider;// will be in paise so converted to Rs

    long long Expiry = sharedData->contract_table_hash[startStrikeToken].Expiry; //both start and end expiry will be same chose any one of it
    // long long startExpiry2 = sharedData->contract_table_hash[endStrikeToken].Expiry;
    QString Instr_Name = sharedData->contract_table_hash[startStrikeToken].InstrumentName;// both start and end instrument will be same, so chose an of it
    QDateTime dt = QDateTime::fromSecsSinceEpoch(Expiry);
    dt = dt.addYears(10);
    int targetYear = dt.date().year();
    bool isLeapYear = QDate::isLeapYear(targetYear);

    // If it is a leap year, and the date is after Feb 29, subtract one day
    if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
        dt = dt.addDays(-1);
    }
    QString ExpiryTimeStr=dt.toString("ddMMM").toUpper();

    //QString Instr_Name2 = sharedData->contract_table_hash[endStrikeToken].InstrumentName;

    // filtered token betweeen start and end strike(endStrike+StrikeDifference) and generate hash with tokens
    QHash<QString, QString> strikePrice_TokenFiltered;
    QList<int> strikePriceListFiltered;
    for(int i=0;i<BX1221_Tokens.length();i++){
        contract_table c = sharedData->contract_table_hash[BX1221_Tokens[i]];
        if(Instr_Name== c.InstrumentName&&Expiry == c.Expiry&&c.StrikePrice >= startStrike && c.StrikePrice <= endStrike+StrikeDifference){
            QString key = QString::number(c.StrikePrice)+"_"+c.OptionType;
            strikePrice_TokenFiltered.insert(key,QString::number(c.TokenNumber));
            if(c.OptionType=="CE") //both PE and CE will have same stike so avoid duplicate.
                strikePriceListFiltered.append(c.StrikePrice);
        }
    }
    std::sort(strikePriceListFiltered.begin(), strikePriceListFiltered.end());


    for(int i=0;i<strikePriceListFiltered.length();i++){
        QString key1 = QString::number(strikePriceListFiltered[i])+"_CE";
        QString key2 = QString::number(strikePriceListFiltered[i])+"_PE";
        QString key3 = QString::number(strikePriceListFiltered[i]+StrikeDifference)+"_CE";
        QString key4 = QString::number(strikePriceListFiltered[i]+StrikeDifference)+"_PE";

        if(strikePrice_TokenFiltered.contains(key1)&&strikePrice_TokenFiltered.contains(key2)&&strikePrice_TokenFiltered.contains(key3)&&strikePrice_TokenFiltered.contains(key4)){
            Leg1_token_number_list.append(strikePrice_TokenFiltered[key1]);
            Leg2_token_number_list.append(strikePrice_TokenFiltered[key2]);
            Leg3_token_number_list.append(strikePrice_TokenFiltered[key3]);
            Leg4_token_number_list.append(strikePrice_TokenFiltered[key4]);
            QString Algo_Name = "BX1221-" + Instr_Name+"-"+ExpiryTimeStr+"-"+QString::number(strikePriceListFiltered[i]/sharedData->strike_price_devider)+"-"+QString::number(StrikeDifference/sharedData->strike_price_devider);

            Algo_Name_list.append(Algo_Name);

            if(Algo_Name_list.size()==50){
                QMessageBox msgBox;
                msgBox.setText("Maximum allowed entry at a time is "+QString::number(sharedData->ROW_LIMIT));
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
            }
        }


    }



    if(Leg1_token_number_list.size()==0){
        QMessageBox msgBox;
        msgBox.setText("Not able to generate algos for given input, Cannot find token number from contract");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QStringList duplicateList;
    for(int i=0;i<Leg1_token_number_list.size();i++){
        QString Leg1_Strike=QString::number(sharedData->contract_table_hash[Leg1_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision)+" "+sharedData->contract_table_hash[Leg1_token_number_list[i]].OptionType;
        QString Leg2_Strike=QString::number(sharedData->contract_table_hash[Leg2_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision)+" "+sharedData->contract_table_hash[Leg2_token_number_list[i]].OptionType;
        QString Leg3_Strike=QString::number(sharedData->contract_table_hash[Leg3_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision)+" "+sharedData->contract_table_hash[Leg3_token_number_list[i]].OptionType;
        QString Leg4_Strike=QString::number(sharedData->contract_table_hash[Leg4_token_number_list[i]].StrikePrice/sharedData->strike_price_devider,'f', sharedData->decimal_precision)+" "+sharedData->contract_table_hash[Leg4_token_number_list[i]].OptionType;



        bool exist=false;
        for(int k=0;k<sharedData->algo_data_list.length();k++){
            algo_data_to_insert tmp = sharedData->algo_data_list[k];
            if(tmp.Algo_Name == Algo_Name_list[i] && tmp.Leg1_token_number == Leg1_token_number_list[i] && tmp.Leg2_token_number == Leg2_token_number_list[i]&& tmp.Leg3_token_number == Leg3_token_number_list[i]&& tmp.Leg4_token_number == Leg4_token_number_list[i]){
                exist=true;
                break;
            }
        }

        //check for if item  is already added or upload in table and will not be in sharedData->algo_data_list
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QString Instr_Name_Tmp="";
            QString ExpiryTime_tmp = "";
            QString Leg1_Strike_tmp= "";
            QString Leg2_Strike_tmp= "";
            QString Leg3_Strike_tmp= "";
            QString Leg4_Strike_tmp= "";

            QTableWidgetItem *item0 = tableWidget->item(row, 0);
            if (item0) {
                Instr_Name_Tmp = item0->text();
            }
            QTableWidgetItem *item1 = tableWidget->item(row, 1);
            if (item1) {
                ExpiryTime_tmp = item1->text();
            }
            QTableWidgetItem *item2 = tableWidget->item(row, 2);
            if (item2) {
                Leg1_Strike_tmp = item2->text();
            }
            QTableWidgetItem *item3 = tableWidget->item(row, 3);
            if (item3) {
                Leg2_Strike_tmp= item3->text();
            }
            QTableWidgetItem *item4 = tableWidget->item(row, 4);
            if (item4) {
                Leg3_Strike_tmp = item4->text();
            }
            QTableWidgetItem *item5 = tableWidget->item(row, 5);
            if (item5) {
                Leg4_Strike_tmp = item5->text();
            }



            if(Algo_Name_list[i]==Instr_Name_Tmp&&
                Instr_Name_Tmp==Instr_Name&&
                ExpiryTime_tmp==ExpiryTimeStr&&
                Leg1_Strike_tmp==Leg1_Strike&&
                Leg2_Strike_tmp==Leg2_Strike&&
                Leg3_Strike_tmp==Leg3_Strike&&
                Leg4_Strike_tmp==Leg4_Strike
                ){
                exist=true;
                break;
            }

        }

        if(exist){
            duplicateList.append("Duplicate entry Skipping, Leg1="+Leg1_token_number_list[i]+", Leg2="+Leg2_token_number_list[i]+", Leg3="+Leg3_token_number_list[i]+", Leg4="+Leg4_token_number_list[i]);
            continue;
        }

        tableWidget->insertRow(tableWidget->rowCount() );

        sharedData->unique_id++;
        QTableWidgetItem *c0 = new QTableWidgetItem();
        c0->setData(Qt::UserRole + 1,sharedData->unique_id);
        c0->setData(Qt::DisplayRole,Algo_Name_list[i]);
        c0->setFlags(c0->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 0, c0);


        QTableWidgetItem *c2 = new QTableWidgetItem(ExpiryTimeStr);
        c2->setFlags(c2->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 1, c2);

        QTableWidgetItem *c3 = new QTableWidgetItem(Leg1_Strike);
        c3->setFlags(c3->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 2, c3);

        QTableWidgetItem *c4 = new QTableWidgetItem(Leg2_Strike);
        c4->setFlags(c4->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 3, c4);

        QTableWidgetItem *c5 = new QTableWidgetItem(Leg3_Strike);
        c5->setFlags(c5->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 4, c5);

        QTableWidgetItem *c51 = new QTableWidgetItem(Leg4_Strike);
        c51->setFlags(c51->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 5, c51);

        QTableWidgetItem *c6 = new QTableWidgetItem("NA");
        c6->setFlags(c6->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 6, c6);

        QTableWidgetItem *c7 = new QTableWidgetItem("NA");
        c7->setFlags(c7->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 7, c7);


        QTableWidgetItem *c8 = new QTableWidgetItem("To be Uploaded");
        c8->setFlags(c8->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 8, c8);

        algo_data_to_insert data;
        data.Algo_Status = "DisabledByUser";
        data.algo_type = QString::number(PortfolioType::BX1221);
        data.exchange = sharedData->exchange;
        data.table_row_unique_id = sharedData->unique_id;
        data.user_id = sharedData->foo_user_id;
        data.Algo_Name = Algo_Name_list[i];
        data.Leg1_token_number = Leg1_token_number_list[i];
        data.Leg2_token_number = Leg2_token_number_list[i];
        data.Leg3_token_number = Leg3_token_number_list[i];
        data.Leg4_token_number = Leg4_token_number_list[i];
        data.indicator = "1";
        data.Leg1_Strike = Leg1_Strike;
        data.Leg2_Strike = Leg2_Strike;
        data.Leg3_Strike = Leg3_Strike;
        data.Leg4_Strike = Leg4_Strike;
        data.option_type = Option_Type;
        data.uploaded=false; // this flag is used to id

        sharedData->algo_data_list.append(data);
        if(tableWidget->rowCount()>=sharedData->ROW_LIMIT){
            QMessageBox msgBox;
            msgBox.setText("Maximum allowed entry at a time is "+QString::number(sharedData->ROW_LIMIT));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            break;
        }
    }

    if(duplicateList.size()>0){
        QMessageBox msgBox;
        //msgBox.setText(QString::number(duplicateList.size())+ " Duplicate entry Skipped!");
        msgBox.setText("Duplication entry  is not allowed!!");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    tableWidget->resizeColumnsToContents();

    tableWidget->resizeColumnsToContents();


}

void convert_to_algo_bs1221::slotStartHide(QString)
{
    endStrikeListView->hide();
}

void convert_to_algo_bs1221::slotEndHide(QString)
{    startStrikeListView->hide();

}

void convert_to_algo_bs1221::itemSelectedStartStrike(QModelIndex index)
{
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        if (dData.isValid())
        {
            for (int row = 0; row < model_start_strike_BX1221->rowCount(); ++row)
            {
                QModelIndex index = model_start_strike_BX1221->index(row, 0);
                // Check if the item's display role value matches
                QVariant displayData = model_start_strike_BX1221->data(index, Qt::DisplayRole);
                if (displayData.isValid() && displayData.toString() == dData)
                {
                    QVariant userData = model_start_strike_BX1221->data(index, Qt::UserRole + 1);
                    if (userData.isValid())
                    {
                        foo_token_number_start_strike = userData.toString();
                        lineEdit_Start_strike->setText(index.data(Qt::DisplayRole).toString());
                        lineEdit_Start_strike->setCursorPosition(0);
                        createEndStrikeModelAndPopulateListView();
                        startStrikeListView->hide();
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

void convert_to_algo_bs1221::itemSelectedEndStrike(QModelIndex index)
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
