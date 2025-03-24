#include "add_algo_f2f.h"
#include "ContractDetail.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QProgressDialog>

add_algo_f2f::add_algo_f2f(QObject *parent)
    : QObject{parent}
{
    //model_searchInstrument_F2F_Leg1 = new QStandardItemModel;
    model_searchInstrument_F2F_Leg2 = new QStandardItemModel;
    sharedData = &AddAlgoSharedVar::getInstance();
    model_searchInstrument_F2F_Leg1 = new QStandardItemModel;
    F2F_Tokens = ContractDetail::getInstance().Get_Tokens_For_PortfolioType(PortfolioType::F2F);

}
void add_algo_f2f::clearAllModel(){
    model_searchInstrument_F2F_Leg2->clear();
    model_searchInstrument_F2F_Leg1->clear();
}

void add_algo_f2f::copyUIElement(QDialog *parentWidget,QTableWidget *tableWidget_,QLineEdit *lineEdit_searchInstrument_leg1_,QLineEdit *lineEdit_searchInstrument_leg2_,QPushButton *addButton_){
    lineEdit_searchInstrument_leg1 = lineEdit_searchInstrument_leg1_;
    lineEdit_searchInstrument_leg2 = lineEdit_searchInstrument_leg2_;
    tableWidget = tableWidget_;
    addButton = addButton_;

    //startStrikeListView = sView;
   // endStrikeListView = eView;


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
     eventFilterStart = new EventFilter(lineEdit_searchInstrument_leg1, startStrikeListView);
     connect(eventFilterStart, SIGNAL(signalItemSelected(QModelIndex)), this, SLOT(itemSelectedStartStrike(QModelIndex)));
     lineEdit_searchInstrument_leg1->installEventFilter(eventFilterStart);
     startStrikeListView->installEventFilter(eventFilterStart);

     eventFilterEnd = new EventFilter(lineEdit_searchInstrument_leg2, endStrikeListView);
     connect(eventFilterEnd, SIGNAL(signalItemSelected(QModelIndex)), this, SLOT(itemSelectedEndStrike(QModelIndex)));
     lineEdit_searchInstrument_leg2->installEventFilter(eventFilterEnd);
     endStrikeListView->installEventFilter(eventFilterEnd);



     //model_searchInstrument_F2F_Leg1 =  ContractDetail::getInstance().Get_model_searchInstrument_F2F_Leg1();
     CustomSearchWidget *startstrikeCustomWidget = new CustomSearchWidget(startStrikeListView,model_searchInstrument_F2F_Leg1);
     connect(lineEdit_searchInstrument_leg1, SIGNAL(textEdited(QString)),startstrikeCustomWidget, SLOT(filterItems(QString)));
     connect(lineEdit_searchInstrument_leg1, SIGNAL(textChanged(QString)),this, SLOT(slotStartHide(QString)));
     connect(lineEdit_searchInstrument_leg2, SIGNAL(textChanged(QString)),this, SLOT(slotEndHide(QString)));


     CustomSearchWidget *strikeCustomWidget = new CustomSearchWidget(endStrikeListView,model_searchInstrument_F2F_Leg2);
     connect(lineEdit_searchInstrument_leg2, SIGNAL(textEdited(QString)),strikeCustomWidget, SLOT(filterItems(QString)));
}

void add_algo_f2f::create_AutoFillModel_SearchInstrument(){
    /* model_searchInstrument_F2F_Leg1->clear();
    model_searchInstrument_F2F_Leg2->clear();

    qDebug()<<"sorted_keys_F2F Size: "<<sorted_keys_F2F.size();
    for(int i=0;i<sorted_keys_F2F.length();i++){
        const auto& contract = sharedData->contract_table_hash[sorted_keys_F2F[i]];

        unsigned int unix_time= contract.Expiry;
        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
        dt = dt.addYears(10);
        QString Expiry=dt.toString("MMM dd yyyy").toUpper();
        QString instrument_name = contract.InstrumentName;


        QStandardItem *itemF2FL1 = new QStandardItem;
        itemF2FL1->setText(instrument_name+" "+Expiry);
        itemF2FL1->setData(contract.TokenNumber, Qt::UserRole + 1);
        model_searchInstrument_F2F_Leg1->appendRow(itemF2FL1);
    }*/
}
void add_algo_f2f::selectedAction(){
    foo_token_number_start_strike = "";
    foo_token_number_end_strike = "";
    lineEdit_searchInstrument_leg1->clear();
    lineEdit_searchInstrument_leg2->clear();
    startStrikeListView->hide();
    endStrikeListView->hide();

    model_searchInstrument_F2F_Leg1->clear();

    // Create a lambda function for processing in the background
        QFuture<void> future = QtConcurrent::run([=]() {
        QElapsedTimer timer1;
        timer1.start();
        emit progressSignal(true,"Loading...");
        for(int i=0;i<F2F_Tokens.length();i++){
                /**********Create model for model_searchInstrument_F2F_Leg1*************************/
                const auto& contract = sharedData->contract_table_hash[F2F_Tokens[i]];
                unsigned int unix_time= contract.Expiry;
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
                QString instrument_name = contract.InstrumentName;


                QStandardItem *itemF2FL1 = new QStandardItem;
                itemF2FL1->setText(instrument_name+" "+Expiry);
                itemF2FL1->setData(contract.TokenNumber, Qt::UserRole + 1);
                QString compositeKey = instrument_name + "-" + dt.toString("yyyyMMdd");
                // Set the composite key as data for sorting
                itemF2FL1->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
                QMetaObject::invokeMethod(this, [=]() {
                    model_searchInstrument_F2F_Leg1->appendRow(itemF2FL1);
                }, Qt::QueuedConnection);
                /********************************************************************/
        }
        emit progressSignal(false,"");

        qDebug()<<"F2F Total data:"<<F2F_Tokens.count() << ", model_searchInstrument_F2F_Leg1  Time:" << timer1.elapsed() << "milliseconds";

        // Close the progress dialog once done
    });


    // Position startStrikeListView below lineEdit_searchInstrument_leg1
    QPoint globalPosSS = lineEdit_searchInstrument_leg1->mapToGlobal(lineEdit_searchInstrument_leg1->geometry().bottomLeft());
    QPoint posSS = startStrikeListView->parentWidget()->mapFromGlobal(globalPosSS);
    startStrikeListView->move(posSS.x(), posSS.y());

    // Position endStrikeListView below lineEdit_searchInstrument_leg2
    QPoint globalPosES(globalPosSS.x()+360,globalPosSS.y());
    QPoint posES = endStrikeListView->parentWidget()->mapFromGlobal(globalPosES);
    endStrikeListView->move(posES.x(), posES.y());


    //create qcompleter and fill with abovie model
  /*  custom_q_completer *Start_strike_combination_Completer = new custom_q_completer( this);
    Start_strike_combination_Completer->setModel(model_searchInstrument_F2F_Leg1);
    Start_strike_combination_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    Start_strike_combination_Completer->setCompletionMode(QCompleter::PopupCompletion);
    Start_strike_combination_Completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    QListView *viewLeg1 = (QListView *)Start_strike_combination_Completer->popup();
    viewLeg1->setUniformItemSizes(true);
    viewLeg1->setLayoutMode(QListView::Batched);
    lineEdit_searchInstrument_leg1->setCompleter(Start_strike_combination_Completer);
    connect(lineEdit_searchInstrument_leg1, SIGNAL(textChanged(QString)),Start_strike_combination_Completer, SLOT(newfilterItems(QString)));
    lineEdit_searchInstrument_leg1->setCompleter(Start_strike_combination_Completer);


    //foo_token_number assined for currently selected algo combination.
    connect(Start_strike_combination_Completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index){
        foo_token_number_start_strike = Start_strike_combination_Completer->get_foo_token_number_for_selected(index);
        //qDebug()<<"foo_token_number_start_strike: "<<foo_token_number_start_strike;
    });*/

    //create qcompleter and fill with abovie model
    /*custom_q_completer *completerLeg2 = new custom_q_completer(this);
    completerLeg2->setModel(model_searchInstrument_F2F_Leg2);
    completerLeg2->setCaseSensitivity(Qt::CaseInsensitive);
    completerLeg2->setCompletionMode(QCompleter::PopupCompletion);
    completerLeg2->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    QListView *viewLeg2 = (QListView *)completerLeg2->popup();
    viewLeg2->setUniformItemSizes(true);
    viewLeg2->setLayoutMode(QListView::Batched);
    lineEdit_searchInstrument_leg2->setCompleter(completerLeg2);

    //foo_token_number assined for currently selected algo combination.
    connect(completerLeg2, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index){
       foo_token_number_end_strike = completerLeg2->get_foo_token_number_for_selected(index);
       qDebug()<<"foo_token_number_end_strike: "<<foo_token_number_end_strike;
    });*/

   // lineEdit_searchInstrument_leg1->setPlaceholderText("Search Leg1 Instrument");
   // lineEdit_searchInstrument_leg2->setPlaceholderText("Search Leg2 Instrument");

}

void add_algo_f2f::slotStartHide(QString)
{
    endStrikeListView->hide();
}

void add_algo_f2f::slotEndHide(QString)
{
    startStrikeListView->hide();
}


void add_algo_f2f::instrumentEditFinishedAction(){


    lineEdit_searchInstrument_leg2->clear();
    foo_token_number_end_strike = "";

    QString key = foo_token_number_start_strike;
    QString Instr_Name = sharedData->contract_table_hash[key].InstrumentName;
    QString Option_Type =sharedData->contract_table_hash[key].OptionType;
    long long startExpiry = sharedData->contract_table_hash[key].Expiry;
    //float start_strike = sharedData->contract_table_hash[key].StrikePrice;


    //QStandardItemModel *model_end_strike = new QStandardItemModel;
    model_searchInstrument_F2F_Leg2->clear();
    for(int i=0;i<F2F_Tokens.length();i++) {
        if(!sharedData->contract_table_hash.contains(F2F_Tokens[i]))
            continue;
        contract_table tmp = sharedData->contract_table_hash[F2F_Tokens[i]];

//          float end_strike = tmp.StrikePrice;
//        if(start_strike>end_strike)
//          continue;
        if(tmp.InstrumentName==Instr_Name&&tmp.Expiry!=startExpiry){

            unsigned int unix_time= tmp.Expiry;
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
            QStandardItem *itemF2FL2 = new QStandardItem;
            itemF2FL2->setText(Instr_Name+" "+Expiry);
            itemF2FL2->setData(tmp.TokenNumber, Qt::UserRole + 1);

            QString compositeKey = Instr_Name + "-" + dt.toString("yyyyMMdd");
            itemF2FL2->setData(compositeKey, ConvertAlog_Model_Roles::CustomSortingDataRole);
            model_searchInstrument_F2F_Leg2->appendRow(itemF2FL2);


            // qDebug()<<"Leg2: "<<Instr_Name+" "+Expiry;


        }
    }


    /*custom_q_completer *completerLeg2 = new custom_q_completer(this);
    completerLeg2->setModel(model_searchInstrument_F2F_Leg2);
    completerLeg2->setCaseSensitivity(Qt::CaseInsensitive);
    completerLeg2->setCompletionMode(QCompleter::PopupCompletion);
    completerLeg2->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    completerLeg2->setFilterMode(Qt::MatchContains);
    QListView *viewLeg2 = (QListView *)completerLeg2->popup();
    viewLeg2->setUniformItemSizes(true);
    viewLeg2->setLayoutMode(QListView::Batched);
    lineEdit_searchInstrument_leg2->setCompleter(completerLeg2);

    //foo_token_number assined for currently selected algo combination.
    connect(completerLeg2, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index){
        foo_token_number_end_strike = completerLeg2->get_foo_token_number_for_selected(index);
        //qDebug()<<"foo_token_number_end_strike: "<<foo_token_number_end_strike;
    });*/


}
QString add_algo_f2f::fixDecimal(double num,int decimal_precision){
    QString str = QString::number(num, 'f', decimal_precision+1);
    return str;
}
QString   add_algo_f2f::get_Algo_Name(PortfolioType algo_type,int leg1_token_number,int leg2_token_number,int leg3_token_number,double devicer,int decimal_precision){

    QString Algo_Name = "F2F-"+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"MMM",algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"MMM",algo_type);
    return Algo_Name.toUpper();
}
void add_algo_f2f::generateAlgo(){

    if(foo_token_number_start_strike==""){// || foo_token_number_end_strike==""){
        QMessageBox msgBox;
        msgBox.setText("Please select Leg1 Instrument name");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    if(foo_token_number_end_strike==""){// || foo_token_number_end_strike==""){
        QMessageBox msgBox;
        msgBox.setText("Please select Leg2 Instrument name");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }



    QString keyStart = foo_token_number_start_strike;
    //QString keyEnd = foo_token_number_end_strike;
    // QString Instr_Name = sharedData->contract_table_hash[keyStart].InstrumentName;

    //QString Option_Type = contract_table_hash[keyStart].option_type;



    QList<QString> Leg1_token_number_list;
    QList<QString> Leg2_token_number_list;
    QList<QString> Leg1_list;
    QList<QString> Leg2_list;

    Leg1_token_number_list.append(foo_token_number_start_strike);
    Leg2_token_number_list.append(foo_token_number_end_strike);

    unsigned int unix_timestart= sharedData->contract_table_hash[foo_token_number_start_strike].Expiry;
    //QDateTime dtstart = QDateTime::fromSecsSinceEpoch(unix_timestart);
//    dtstart = dtstart.addYears(10);
//    int targetYear = dtstart.date().year();
//    bool isLeapYear = QDate::isLeapYear(targetYear);

//    // If it is a leap year, and the date is after Feb 29, subtract one day
//    if (isLeapYear && dtstart.date() > QDate(targetYear, 2, 29)) {
//        dtstart = dtstart.addDays(-1);
//    }
    QDateTime dt;
    dt.setOffsetFromUtc(0);
    dt.setDate(QDate(1980, 1, 1));
    dt.setTime(QTime(0, 0, 0));
    dt = dt.addSecs(unix_timestart);
    QDateTime dt1 = QDateTime::fromSecsSinceEpoch(unix_timestart);
    dt1 = dt1.addYears(10);
    QDate newDate = QDate(dt1.date().year(), dt.date().month(), dt.date().day());
    dt.setDate(newDate);
    QString ExpiryTmpStart=dt.toString("MMM dd yyyy").toUpper();

    unsigned int unix_timeend= sharedData->contract_table_hash[foo_token_number_end_strike].Expiry;
//    QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_timeend);
//    dt = dt.addYears(10);
//    int target_Year = dt.date().year();
//    bool isLeap_Year = QDate::isLeapYear(targetYear);

//    // If it is a leap year, and the date is after Feb 29, subtract one day
//    if (isLeapYear && dt.date() > QDate(targetYear, 2, 29)) {
//        dt = dt.addDays(-1);
//    }
    QDateTime dt2;
    dt2.setOffsetFromUtc(0);
    dt2.setDate(QDate(1980, 1, 1));
    dt2.setTime(QTime(0, 0, 0));
    dt2 = dt2.addSecs(unix_timeend);
    QDateTime dt3 = QDateTime::fromSecsSinceEpoch(unix_timeend);
    dt3 = dt3.addYears(10);
    QDate newDates = QDate(dt3.date().year(), dt2.date().month(), dt2.date().day());
    dt.setDate(newDate);
    QString ExpiryTmpEnd=dt2.toString("MMM dd yyyy").toUpper();
    Leg1_list.append(ExpiryTmpStart);
    Leg2_list.append(ExpiryTmpEnd);


    /* for(int i=0;i<expiry_dateList.length()-1;i++){
       for(int j=i+1;j<expiry_dateList.length();j++){
           unsigned int unix_timestart= sharedData->contract_table_hash[token_numebrList[i]].Expiry;
           QDateTime dtstart = QDateTime::fromSecsSinceEpoch(unix_timestart);
           dtstart = dtstart.addYears(10);
           QString ExpiryTmpStart=dtstart.toString("MMM dd yyyy").toUpper();

           unsigned int unix_time= sharedData->contract_table_hash[token_numebrList[j]].Expiry;
           QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
           dt = dt.addYears(10);
           QString ExpiryTmpEnd=dt.toString("MMM dd yyyy").toUpper();
           Leg1_list.append(ExpiryTmpStart);
           Leg2_list.append(ExpiryTmpEnd);

           Leg1_token_number_list.append(token_numebrList[i]);
           Leg2_token_number_list.append(token_numebrList[j]);


       }
    }*/

    if(Leg1_list.size()<1){
        QMessageBox msgBox;
        msgBox.setText("Not able to generate algos for given input, please select different combination.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }

    QStringList duplicateList;
    for(int i=0;i<Leg1_list.size();i++){
        QString Instr_Name =get_Algo_Name(PortfolioType::F2F,Leg1_token_number_list[i].toInt(),Leg2_token_number_list[i].toInt(),0,sharedData->devicer,sharedData->decimal_precision);

        //check the data already added to table
        bool exist=false;
        for(int k=0;k<sharedData->algo_data_list.length();k++){
            algo_data_to_insert tmp = sharedData->algo_data_list[k];
            if(tmp.Algo_Name == Instr_Name && tmp.Leg1_token_number == Leg1_token_number_list[i] && tmp.Leg2_token_number == Leg2_token_number_list[i]){
                exist=true;
                break;
            }
        }
        //check for if item  is already added or upload in table and will not be in sharedData->algo_data_list
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QString Instr_Name_Tmp="";
            QString Leg1_list_tmp = "";
            QString Leg2_list_tmp = "";

            QTableWidgetItem *item0 = tableWidget->item(row, 0);
            if (item0) {
                Instr_Name_Tmp = item0->text();
            }
            QTableWidgetItem *item1 = tableWidget->item(row, 1);
            if (item1) {
                Leg1_list_tmp = item1->text();
            }
            QTableWidgetItem *item2 = tableWidget->item(row, 2);
            if (item2) {
                Leg2_list_tmp = item2->text();
            }
            if(Instr_Name==Instr_Name_Tmp&&Leg1_list[i]==Leg1_list_tmp&&Leg2_list[i]==Leg2_list_tmp){
                exist=true;
                break;
            }

        }


        if(exist){
            duplicateList.append("Duplication entry is not allowed!.");
            continue;
        }

        sharedData->unique_id++;

        tableWidget->insertRow( tableWidget->rowCount());
        QTableWidgetItem *c0 = new QTableWidgetItem();
        c0->setData(Qt::UserRole + 1,sharedData->unique_id);
        c0->setData(Qt::DisplayRole,Instr_Name);
        c0->setFlags(c0->flags() ^ Qt::ItemIsEditable);
        c0->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 0, c0);

        QTableWidgetItem *c1 = new QTableWidgetItem(Leg1_list[i]);
        c1->setFlags(c1->flags() ^ Qt::ItemIsEditable);
        c1->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1,1, c1);

        QTableWidgetItem *c2 = new QTableWidgetItem(Leg2_list[i]);
        c2->setFlags(c2->flags() ^ Qt::ItemIsEditable);
        c2->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 2, c2);


//        QTableWidgetItem *c3 = new QTableWidgetItem("-");
//        c3->setFlags(c3->flags() ^ Qt::ItemIsEditable);
//        c3->setTextAlignment(Qt::AlignCenter);
//        tableWidget->setItem(tableWidget->rowCount()-1, 3, c3);

//        QTableWidgetItem *c4 = new QTableWidgetItem("NA");
//        c4->setFlags(c4->flags() ^ Qt::ItemIsEditable);
//        tableWidget->setItem(tableWidget->rowCount()-1, 4, c4);

//        QTableWidgetItem *c5 = new QTableWidgetItem("NA");
//        c5->setFlags(c5->flags() ^ Qt::ItemIsEditable);
//        tableWidget->setItem(tableWidget->rowCount()-1, 5, c5);

        QTableWidgetItem *c3 = new QTableWidgetItem("To be Uploaded");
        c3->setFlags(c3->flags() ^ Qt::ItemIsEditable);
        c3->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(tableWidget->rowCount()-1, 3, c3);



        algo_data_to_insert data;
        data.Algo_Status = "DisabledByUser";
        data.algo_type = QString::number(PortfolioType::F2F);
        data.exchange = sharedData->exchange;
        data.user_id = sharedData->foo_user_id;
        data.Algo_Name = Instr_Name;
        data.Leg1_token_number = Leg1_token_number_list[i];
        data.Leg2_token_number = Leg2_token_number_list[i];
        data.Leg3_token_number = "";
        data.indicator = "1";
        data.Leg1_Strike = "";
        data.Leg2_Strike = "";
        data.Leg3_Strike = "";
        data.table_row_unique_id=sharedData->unique_id;
        data.option_type = "";//Option_Type;
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


void add_algo_f2f::itemSelectedStartStrike(QModelIndex index)
{
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        if (dData.isValid())
        {
            for (int row = 0; row < model_searchInstrument_F2F_Leg1->rowCount(); ++row)
            {
                QModelIndex index = model_searchInstrument_F2F_Leg1->index(row, 0);
                // Check if the item's display role value matches
                QVariant displayData = model_searchInstrument_F2F_Leg1->data(index, Qt::DisplayRole);
                if (displayData.isValid() && displayData.toString() == dData)
                {
                    QVariant userData = model_searchInstrument_F2F_Leg1->data(index, Qt::UserRole + 1);
                    if (userData.isValid())
                    {
                        foo_token_number_start_strike = userData.toString();
                        lineEdit_searchInstrument_leg1->setText(index.data(Qt::DisplayRole).toString());
                        lineEdit_searchInstrument_leg1->setCursorPosition(0);
                        instrumentEditFinishedAction();
                        startStrikeListView->hide();
                        lineEdit_searchInstrument_leg2->setFocus();

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

void add_algo_f2f::itemSelectedEndStrike(QModelIndex index)
{
    if(index.isValid())
    {
        QVariant dData = index.data(Qt::DisplayRole);
        if (dData.isValid())
        {
            for (int row = 0; row < model_searchInstrument_F2F_Leg2->rowCount(); ++row)
            {
                QModelIndex index = model_searchInstrument_F2F_Leg2->index(row, 0);
                // Check if the item's display role value matches
                QVariant displayData = model_searchInstrument_F2F_Leg2->data(index, Qt::DisplayRole);
                if (displayData.isValid() && displayData.toString() == dData)
                {
                    QVariant userData = model_searchInstrument_F2F_Leg2->data(index, Qt::UserRole + 1);
                    if (userData.isValid())
                    {
                        foo_token_number_end_strike = userData.toString();
                        lineEdit_searchInstrument_leg2->setText(index.data(Qt::DisplayRole).toString());
                        lineEdit_searchInstrument_leg2->setCursorPosition(0);
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
