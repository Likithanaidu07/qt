#include "Convert_to_Algo/convert_to_algo_bfly.h"
#include "custom_q_completer.h"

#include "QElapsedTimer"
#include "contractdetail.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>

add_algo_btfly::add_algo_btfly(QObject *parent)
    : QObject{parent}
{
    //  model_start_strike_BFLY = new QStandardItemModel;
    sharedData = &AddAlgoSharedVar::getInstance();
    model_end_strike = new QStandardItemModel;
    model_start_strike_BFLY = new QStandardItemModel;
    BFLY_Tokens = ContractDetail::getInstance().Get_Tokens_For_PortfolioType(PortfolioType::BY);


}
void add_algo_btfly::clearAllModel(){
    model_end_strike->clear();
    model_start_strike_BFLY->clear();
}

void add_algo_btfly::copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_, QLineEdit *lineEdit_Start_strike_, QLineEdit *lineEdit_EndStrike_, QLineEdit *lineEdit_StrikeDifference_,QPushButton *addButton_)
{
    lineEdit_Start_strike = lineEdit_Start_strike_;
    lineEdit_EndStrike = lineEdit_EndStrike_;
    lineEdit_StrikeDifference = lineEdit_StrikeDifference_;
    tableWidget = tableWidget_;
    addButton = addButton_;

    lineEdit_Start_strike->setCursorPosition(0);
    lineEdit_EndStrike->setCursorPosition(0);

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
     // Set the position of the QListView just below lineEdit_EndStrike

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


    //model_start_strike_BFLY = ContractDetail::getInstance().Get_model_start_strike_BFLY();
    CustomSearchWidget *start_strikeCustomWidget = new CustomSearchWidget(startStrikeListView,model_start_strike_BFLY);
    connect(lineEdit_Start_strike, SIGNAL(textEdited(QString)),start_strikeCustomWidget, SLOT(filterItems(QString)));
    connect(lineEdit_Start_strike, SIGNAL(textChanged(QString)),this, SLOT(slotStartHide(QString)));
    connect(lineEdit_EndStrike, SIGNAL(textChanged(QString)),this, SLOT(slotEndHide(QString)));

    // create qcompleter and fill with abovie model
    CustomSearchWidget *endstrikeCustomWidget = new CustomSearchWidget(endStrikeListView,model_end_strike);
    connect(lineEdit_EndStrike, SIGNAL(textEdited(QString)),endstrikeCustomWidget, SLOT(filterItems(QString)));


}
/*
void add_algo_btfly::create_AutoFillModel_StartStrike(){
     model_start_strike_BFLY->clear();
    // create model for BFLY_data_list to autfill

    QElapsedTimer timer;
     timer.start();
    for(int i=0;i<sorted_keys_BFLY.length();i++){
        const auto& contract = sharedData->contract_table_hash[sorted_keys_BFLY[i]];

        unsigned int unix_time= contract.Expiry;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" "+contract.OptionType;
        QStandardItem *itemBFLY = new QStandardItem;
        itemBFLY->setText(algo_combination);
        itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
        QString compositeKey = contract.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
                // Set the composite key as data for sorting
        itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
        model_start_strike_BFLY->appendRow(itemBFLY);
    }
    qDebug() << "btfly------" << timer.elapsed()/1000 << " seconds";

}*/
void add_algo_btfly::selectedAction(){
    foo_token_number_start_strike = "";
    foo_token_number_end_strike = "";

    lineEdit_Start_strike->clear();
    lineEdit_EndStrike->clear();
    lineEdit_StrikeDifference->clear();

    startStrikeListView->hide();
    endStrikeListView->hide();
    model_start_strike_BFLY->clear();

    // Create a lambda function for processing in the background
        QFuture<void> future = QtConcurrent::run([=]() {
        QElapsedTimer timer1;
        timer1.start();
        emit progressSignal(true,"Loading...");
        qDebug() << "BFLY_Tokens.length()  :" <<BFLY_Tokens.length() << "";

        for(int i=0;i<BFLY_Tokens.length();i++){
                const auto& contract = sharedData->contract_table_hash[BFLY_Tokens[i]];

                /**********Create model for BFLY *************************/
                unsigned int unix_time= contract.Expiry;
//                QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//                dt = dt.addYears(10);
//                int targetYear = dt.date().year();
//                bool isLeapYear = QDate::isLeapYear(targetYear);
//                QString instrument_name = contract.InstrumentName;

//                // If it is a leap year, and the date is after Feb 29, subtract one day
//                if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//                    dt = dt.addDays(-1);
//                }
                QDateTime dt;
                dt.setOffsetFromUtc(0);
                dt.setDate(QDate(1980, 1, 1));
                dt.setTime(QTime(0, 0, 0));
                dt = dt.addSecs(unix_time);
                QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_time);
                dt1 = dt1.addYears(10);
                QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
                dt.setDate(newDate);
                QString Expiry=dt.toString("MMM dd yyyy").toUpper();
                QString algo_combination = contract.InstrumentName+" "+Expiry+" "+QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" "+contract.OptionType;
                QStandardItem *itemBFLY = new QStandardItem;
                itemBFLY->setText(algo_combination);
                itemBFLY->setData(contract.TokenNumber, Qt::UserRole + 1);
                QString compositeKey = contract.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + QString::number(contract.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
                // Set the composite key as data for sorting
                itemBFLY->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);

                QMetaObject::invokeMethod(this, [=]() {
                                   model_start_strike_BFLY->appendRow(itemBFLY);
                }, Qt::QueuedConnection);

                /********************************************************************/

            }
        emit progressSignal(false,"");

        qDebug()<<"model_start_strike_BFLY Total data:"<<BFLY_Tokens.count() << ", model_start_strike_BFLY  Time:" << timer1.elapsed() << "milliseconds";


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

void add_algo_btfly::slotStartHide(QString)
{
    endStrikeListView->hide();

}

void add_algo_btfly::slotEndHide(QString)
{
    startStrikeListView->hide();

}


void add_algo_btfly::startStrikeEditFinishedAction()
{

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


    model_end_strike->clear();

    for(int i=0;i<BFLY_Tokens.length();i++) {
        contract_table tmp = sharedData->contract_table_hash[BFLY_Tokens[i]];

        float end_strike = tmp.StrikePrice;
        if(start_strike>end_strike)
            continue;
        if(tmp.InstrumentName==Instr_Name&&tmp.OptionType==Option_Type&&Expiry==tmp.Expiry){

            if(!strike_priceList.contains(QString::number(tmp.StrikePrice))){
                strike_priceList.append(QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)); // store in Rs
                token_numebrList.append(QString::number(tmp.TokenNumber));
            }

            // this will prevent adding same algo_combination  in end strike list view as in start strik list view
            if(start_strike==end_strike)
                continue;

            unsigned int unix_time= tmp.Expiry;
//            QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//            dt = dt.addYears(10);
//            int targetYear = dt.date().year();
//            bool isLeapYear = QDate::isLeapYear(targetYear);

//            // If it is a leap year, and the date is after Feb 29, subtract one day
//            if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//                dt = dt.addDays(-1);
//            }
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
            QString algo_combination = tmp.InstrumentName+" "+ExpiryTmp+" "+QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" "+tmp.OptionType;
            QStandardItem *item = new QStandardItem;
            item->setText(algo_combination);
            item->setData(tmp.TokenNumber, Qt::UserRole + 1);
            QString compositeKey = tmp.InstrumentName + "-" + dt.toString("yyyyMMdd") + "-" + QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);
            item->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
            model_end_strike->appendRow(item);





        }
    }



}

void add_algo_btfly::generateAlgo()
{
    if(foo_token_number_start_strike=="" && foo_token_number_end_strike=="")
    {
        QMessageBox msgBox;
        msgBox.setText("Cannot Use current Algo combination.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }



    QString keyStart = foo_token_number_start_strike;
    QString keyEnd = foo_token_number_end_strike;
    QString Option_Type = sharedData->contract_table_hash[keyStart].OptionType;
    if(lineEdit_StrikeDifference->text()==""){
        QMessageBox msgBox;
        msgBox.setText("Please fill Strike Difference.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    float StrikeDifference = lineEdit_StrikeDifference->text().toFloat(); /// will be in Rs
    if(StrikeDifference<=0){
        QMessageBox msgBox;
        msgBox.setText("Please Enter Valid  Strike Difference.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    float startStrike =  sharedData->contract_table_hash[keyStart].StrikePrice/sharedData->strike_price_devider; // will be in paise so converted to Rs
    float endStrike = sharedData->contract_table_hash[keyEnd].StrikePrice/sharedData->strike_price_devider;// will be in paise so converted to Rs
    unsigned int unix_time= sharedData->contract_table_hash[keyStart].Expiry;
//    QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//    dt = dt.addYears(10);
//    int targetYear = dt.date().year();
//    bool isLeapYear = QDate::isLeapYear(targetYear);

//    // If it is a leap year, and the date is after Feb 29, subtract one day
//    if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//        dt = dt.addDays(-1);
//    }
    QDateTime dt;
    dt.setOffsetFromUtc(0);
    dt.setDate(QDate(1980, 1, 1));
    dt.setTime(QTime(0, 0, 0));
    dt = dt.addSecs(unix_time);
    QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_time);
    dt1 = dt1.addYears(10);
    QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
    dt.setDate(newDate);
    QString ExpiryTime=dt.toString("MMM dd yyyy").toUpper();



    QList<float> leg1_list;
    QList<float> leg2_list;
    QList<float> leg3_list;

    QList<QString> Leg1_token_number_list;
    QList<QString> Leg2_token_number_list;
    QList<QString> Leg3_token_number_list;
    QList<QString> Algo_Name_list;

    float currentStrick = startStrike;
    while(1){
        if(StrikeDifference>0){
            if(((endStrike-startStrike)/StrikeDifference)>1000000000){
                QMessageBox msgBox;
                msgBox.setText("Strike diffrence is too small, please chose higher value.");
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.exec();
                break;
            }
        }
        float leg1 =  currentStrick;
        float leg2 = leg1+StrikeDifference;
        float leg3 = leg2+StrikeDifference;

        QString Leg1_token_number="";
        QString Leg2_token_number="";
        QString Leg3_token_number="";
        //check generated stricke price is in the DB and get the token numeber, if not remove it
        for(int i=0;i<strike_priceList.length();i++){
            if(strike_priceList[i].toFloat()==leg1){
                Leg1_token_number = token_numebrList[i];
            }
            else if(strike_priceList[i].toFloat()==leg2){
                Leg2_token_number = token_numebrList[i];
            }
            else if(strike_priceList[i].toFloat()==leg3){
                Leg3_token_number = token_numebrList[i];
            }
        }

        if(Leg1_token_number!=""&&Leg2_token_number!=""&&Leg3_token_number!=""){
            leg1_list.append(leg1);
            leg2_list.append(leg2);
            leg3_list.append(leg3);
            Leg1_token_number_list.append(Leg1_token_number);
            Leg2_token_number_list.append(Leg2_token_number);
            Leg3_token_number_list.append(Leg3_token_number);
            //(Instrument name, expiry year and month, middle strike and strike difference     eg.  Bfly - EURUSD 24 JUN 1.005 CE - 0.005

            //            Algo_Name = "Bfly-";
            //            double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number).toDouble());
            //            Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM")+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number) +"-"+QString::number(diff)+ContractDetail::getInstance().GetOptionType(leg1_token_number);

            QString Instr_Name = "Bfly-" + sharedData->contract_table_hash[keyStart].InstrumentName+"-"+dt.toString("ddMMM").toUpper()+"-"+QString::number(leg2)+"-"+QString::number(StrikeDifference)+"-"+Option_Type;
            Algo_Name_list.append(Instr_Name);

        }

        currentStrick = currentStrick+StrikeDifference;
        if(currentStrick>endStrike)
            break;
        if(leg1_list.size()>50){
            QMessageBox msgBox;
            msgBox.setText("Cannot create more than 50 algos, showing the first 50 algos.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            break;
        }
    }

    if(leg1_list.size()==0){
        QMessageBox msgBox;
        msgBox.setText("Not able to generate algos for given input, please select different strike price.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QStringList duplicateList;
    for(int i=0;i<leg1_list.size();i++){
        QString Leg1_Strike=QString::number(leg1_list[i],'f', sharedData->decimal_precision);
        QString Leg2_Strike=QString::number(leg2_list[i],'f', sharedData->decimal_precision);
        QString Leg3_Strike=QString::number(leg3_list[i],'f', sharedData->decimal_precision);



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
            QString Option_Type_tmp = "";
            QString ExpiryTime_tmp = "";
            QString Leg1_Strike_tmp= "";
            QString Leg2_Strike_tmp= "";
            QString Leg3_Strike_tmp= "";

            QTableWidgetItem *item0 = tableWidget->item(row, 0);
            if (item0) {
                Instr_Name_Tmp = item0->text();
            }
            QTableWidgetItem *item1 = tableWidget->item(row, 1);
            if (item1) {
                Option_Type_tmp = item1->text();
            }
            QTableWidgetItem *item2 = tableWidget->item(row, 2);
            if (item2) {
                ExpiryTime_tmp = item2->text();
            }
            QTableWidgetItem *item3 = tableWidget->item(row, 3);
            if (item3) {
                Leg1_Strike_tmp = item3->text();
            }
            QTableWidgetItem *item4 = tableWidget->item(row, 4);
            if (item4) {
                Leg2_Strike_tmp = item4->text();
            }
            QTableWidgetItem *item5 = tableWidget->item(row, 5);
            if (item5) {
                Leg3_Strike_tmp = item5->text();
            }



            if(Algo_Name_list[i]==Instr_Name_Tmp&&
                Option_Type_tmp==Option_Type&&
                ExpiryTime_tmp==ExpiryTime&&
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


        QTableWidgetItem *c1 = new QTableWidgetItem(Option_Type);
        c1->setFlags(c1->flags() ^ Qt::ItemIsEditable);
        c1->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 1, c1);

        QTableWidgetItem *c2 = new QTableWidgetItem(ExpiryTime);
        c2->setFlags(c2->flags() ^ Qt::ItemIsEditable);
        c2->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 2, c2);

        QTableWidgetItem *c3 = new QTableWidgetItem(Leg1_Strike);
        c3->setFlags(c3->flags() ^ Qt::ItemIsEditable);
        c3->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 3, c3);

        QTableWidgetItem *c4 = new QTableWidgetItem(Leg2_Strike);
        c4->setFlags(c4->flags() ^ Qt::ItemIsEditable);
        c4->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 4, c4);

        QTableWidgetItem *c5 = new QTableWidgetItem(Leg3_Strike);
        c5->setFlags(c5->flags() ^ Qt::ItemIsEditable);
        c5->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 5, c5);

        QTableWidgetItem *c6 = new QTableWidgetItem("NA");
        c6->setFlags(c6->flags() ^ Qt::ItemIsEditable);
        c6->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 6, c6);

        QTableWidgetItem *c7 = new QTableWidgetItem("NA");
        c7->setFlags(c7->flags() ^ Qt::ItemIsEditable);
        c7->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 7, c7);


        QTableWidgetItem *c8 = new QTableWidgetItem("To be Uploaded");
        c8->setFlags(c8->flags() ^ Qt::ItemIsEditable);
        c8->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 8, c8);

        algo_data_to_insert data;
        data.Algo_Status = "DisabledByUser";
        data.algo_type = QString::number(PortfolioType::BY);
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

}

void add_algo_btfly::itemSelectedStartStrike(QModelIndex index)
{
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        if (dData.isValid())
        {
            for (int row = 0; row < model_start_strike_BFLY->rowCount(); ++row)
            {
                QModelIndex index = model_start_strike_BFLY->index(row, 0);
                // Check if the item's display role value matches
                QVariant displayData = model_start_strike_BFLY->data(index, Qt::DisplayRole);
                if (displayData.isValid() && displayData.toString() == dData)
                {
                    QVariant userData = model_start_strike_BFLY->data(index, Qt::UserRole + 1);
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

void add_algo_btfly::itemSelectedEndStrike(QModelIndex index)
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
                        addButton->setFocus();


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






