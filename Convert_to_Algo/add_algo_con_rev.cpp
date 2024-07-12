#include "add_algo_con_rev.h"


add_algo_con_rev::add_algo_con_rev(QObject *parent)
    : QObject{parent}
{

    // model_FUT_CON_REV = new QStandardItemModel;
    sharedData = &AddAlgoSharedVar::getInstance();

}
void add_algo_con_rev::copyUIElement(QTableWidget *tableWidget_,QLineEdit *lineEdit_Start_strike_,QLineEdit *lineEdit_EndStrike_,QLineEdit *lineEdit_Fut_){
    lineEdit_Start_strike = lineEdit_Start_strike_;
    lineEdit_EndStrike = lineEdit_EndStrike_;
    lineEdit_Fut = lineEdit_Fut_;
    tableWidget = tableWidget_;
}

void add_algo_con_rev::create_AutoFillModel_FutInstrument(){
//    model_FUT_CON_REV->clear();
//    for(int i=0;i<sorted_keys_F2F.length();i++){
//        const auto& contract = sharedData->contract_table_hash[sorted_keys_F2F[i]];

//        QString instrument_name = contract.InstrumentName;//" "+Expiry;
//        unsigned int unix_time= contract.Expiry;
//        QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//        dt = dt.addYears(10);
//        QString Expiry=dt.toString("MMM dd yyyy").toUpper();

//        QStandardItem *itemFut = new QStandardItem;
//        itemFut->setText(instrument_name+" "+Expiry);
//        itemFut->setData(contract.TokenNumber, Qt::UserRole + 1);
//        model_FUT_CON_REV->appendRow(itemFut);
//    }
}

void add_algo_con_rev::create_AutoFillModel_StartStrike(){
    lineEdit_Start_strike->clear();
    lineEdit_EndStrike->clear();
    foo_token_number_start_strike="";
    foo_token_number_end_strike="";
    QString key = foo_token_number_fut;

    QString Instr_Name = sharedData->contract_table_hash[key].InstrumentName;
    QStandardItemModel *model_start_strike = new QStandardItemModel;


    //create list based on the Fut input and populate start strike model the data is of same as butterfly
    for(int i=0;i<sorted_keys_CON_REV.length();i++) {
        contract_table tmp = sharedData->contract_table_hash[sorted_keys_CON_REV[i]];

        if(tmp.InstrumentName==Instr_Name&&tmp.OptionType=="CE"){
            unsigned int unix_time= tmp.Expiry;
            QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
            dt = dt.addYears(10);
            QString ExpiryTmp=dt.toString("MMM dd yyyy").toUpper();
            QString algo_combination = tmp.InstrumentName+" "+ExpiryTmp+" "+QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);//+" "+tmp.OptionType;
            QStandardItem *item = new QStandardItem;
            item->setText(algo_combination);
            item->setData(tmp.TokenNumber, Qt::UserRole + 1);
            model_start_strike->appendRow(item);
            //qDebug()<<"Instr_Name: "<<Instr_Name;

        }
    }

    custom_q_completer *end_strike_combination_Completer = new custom_q_completer(this);
    end_strike_combination_Completer->setModel(model_start_strike);
    end_strike_combination_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    end_strike_combination_Completer->setCompletionMode(QCompleter::PopupCompletion);
    end_strike_combination_Completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    end_strike_combination_Completer->setFilterMode(Qt::MatchContains);
    QListView *view = (QListView *)end_strike_combination_Completer->popup();
    view->setUniformItemSizes(true);
    view->setLayoutMode(QListView::Batched);
    lineEdit_Start_strike->setCompleter(end_strike_combination_Completer);

    //foo_token_number assined for currently selected algo combination.
    connect(end_strike_combination_Completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index){
        foo_token_number_start_strike = end_strike_combination_Completer->get_foo_token_number_for_selected(index);
        //qDebug()<<"foo_token_number_end_strike: "<<foo_token_number_start_strike;
    });


}
void add_algo_con_rev::selectedAction(){

    foo_token_number_start_strike = "";
    foo_token_number_end_strike = "";
    foo_token_number_fut="";

    //create qcompleter and fill with CON_REV model
    custom_q_completer *Start_strike_combination_Completer = new custom_q_completer( this);
    Start_strike_combination_Completer->setModel(model_FUT_CON_REV);
    Start_strike_combination_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    Start_strike_combination_Completer->setCompletionMode(QCompleter::PopupCompletion);
    Start_strike_combination_Completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    connect(lineEdit_Start_strike, SIGNAL(textChanged(QString)),Start_strike_combination_Completer, SLOT(newfilterItems(QString)));
    lineEdit_Start_strike->setCompleter(Start_strike_combination_Completer);
    QListView *view = (QListView *)Start_strike_combination_Completer->popup();
    view->setUniformItemSizes(true);
    view->setLayoutMode(QListView::Batched);
    lineEdit_Fut->setCompleter(Start_strike_combination_Completer);

    //foo_token_number assined for currently selected algo combination.
    connect(Start_strike_combination_Completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index){
        foo_token_number_fut = Start_strike_combination_Completer->get_foo_token_number_for_selected(index);
        //qDebug()<<"foo_token_number_fut: "<<foo_token_number_fut;
    });
}


void add_algo_con_rev::startStrikeEditFinishedAction(){
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

    // float expiry_date_start = contract_table_hash[key].expiry_date.toFloat();

    QStandardItemModel *model_end_strike = new QStandardItemModel;
    for(int i=0;i<sorted_keys_CON_REV.length();i++) {
        contract_table tmp = sharedData->contract_table_hash[sorted_keys_CON_REV[i]];
        float end_strike = tmp.StrikePrice;
        //qDebug()<<tmp.instrument_name<<"=="<<Instr_Name<<"   "<<tmp.option_type<<"=="<<Option_Type<<"   "<<Expiry<<"=="<<tmp.expiry_date;
        if(start_strike>end_strike)
            continue;
        if(tmp.InstrumentName==Instr_Name&&tmp.OptionType=="PE"&&Expiry==tmp.Expiry){
            unsigned int unix_time= tmp.Expiry;
            QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
            dt = dt.addYears(10);
            QString ExpiryTmp=dt.toString("MMM dd yyyy").toUpper();
            QString algo_combination = tmp.InstrumentName+" "+ExpiryTmp+" "+QString::number(tmp.StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision);//+" "+tmp.OptionType;
            QStandardItem *item = new QStandardItem;
            item->setText(algo_combination);
            item->setData(tmp.TokenNumber, Qt::UserRole + 1);
            model_end_strike->appendRow(item);

        }
    }
    custom_q_completer *end_strike_combination_Completer = new custom_q_completer(this);
    end_strike_combination_Completer->setModel(model_end_strike);
    end_strike_combination_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    end_strike_combination_Completer->setCompletionMode(QCompleter::PopupCompletion);
    end_strike_combination_Completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    end_strike_combination_Completer->setFilterMode(Qt::MatchContains);
    QListView *view = (QListView *)end_strike_combination_Completer->popup();
    view->setUniformItemSizes(true);
    view->setLayoutMode(QListView::Batched);
    lineEdit_EndStrike->setCompleter(end_strike_combination_Completer);

    //foo_token_number assined for currently selected algo combination.
    connect(end_strike_combination_Completer, QOverload<const QModelIndex &>::of(&QCompleter::activated), [=](const QModelIndex &index){
        foo_token_number_end_strike = end_strike_combination_Completer->get_foo_token_number_for_selected(index);
        // qDebug()<<"foo_token_number_end_strike: "<<foo_token_number_end_strike;
    });

}
void add_algo_con_rev::generateAlgo(){

    if(foo_token_number_start_strike=="" || foo_token_number_end_strike=="" || foo_token_number_fut==""){
        QMessageBox msgBox;
        msgBox.setText("Cannot Use current Algo combination.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }


    QString keyFut = foo_token_number_fut;
    QString keyStart = foo_token_number_start_strike;
    QString keyEnd = foo_token_number_end_strike;
    QString Instr_Name = sharedData->contract_table_hash[keyStart].InstrumentName;
    QString Option_Type = sharedData->contract_table_hash[keyStart].OptionType;
    int startStrike =  sharedData->contract_table_hash[keyStart].StrikePrice; // will be in paise so converted to Rs
    int endStrike =  sharedData->contract_table_hash[keyEnd].StrikePrice; // will be in paise so converted to Rs
    if(startStrike>endStrike){
        QMessageBox msgBox;
        msgBox.setText("Start Strike Cannot greater than End Strike.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    int Leg1TokenNumeber = sharedData->contract_table_hash[keyFut].TokenNumber;
    int Leg1_Strike =  sharedData->contract_table_hash[keyFut].StrikePrice;
    //
    unsigned int unix_time= sharedData->contract_table_hash[keyFut].Expiry;
    QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
    dt = dt.addYears(10);
    QString Leg1_val=dt.toString("MMM dd yyyy").toUpper();



    QStringList Leg2_token_number_list;
    QStringList Leg3_token_number_list;
    QStringList Leg2_list;
    QStringList Leg3_list;
    QStringList Leg2_strike_list;
    QStringList Leg3_strike_list;
    QStringList Algo_Name_list;

    long long expiryDateStartStrike = sharedData->contract_table_hash[keyStart].Expiry;
    //filter based on  selected combination
    QStringList filteredKeys;
    for(int i=0;i<sorted_keys_CON_REV.length();i++) {
        if(Instr_Name!= sharedData->contract_table_hash[sorted_keys_CON_REV[i]].InstrumentName||sharedData->contract_table_hash[sorted_keys_CON_REV[i]].Expiry!=expiryDateStartStrike)
            continue;
        QString optionType = sharedData->contract_table_hash[sorted_keys_CON_REV[i]].OptionType;



        int tmpStrike =  sharedData->contract_table_hash[sorted_keys_CON_REV[i]].StrikePrice; // will be in paise so converted to Rs
        if(tmpStrike>endStrike||tmpStrike<startStrike)
            continue;

        if(optionType=="CE"||optionType=="PE")
            filteredKeys.append(sorted_keys_CON_REV[i]);
    }
    //qDebug()<<filteredKeys.length();
    // get all data to diplay-----note: BFLY sorted based on strike price.
    for(int i=0;i<filteredKeys.length();i++) {

        QString optionType = sharedData->contract_table_hash[filteredKeys[i]].OptionType;
        if(optionType=="CE"){
            int leg2 = sharedData->contract_table_hash[filteredKeys[i]].StrikePrice;
            for(int j=i+1;j<filteredKeys.length();j++){
                if(leg2==sharedData->contract_table_hash[filteredKeys[j]].StrikePrice&&sharedData->contract_table_hash[filteredKeys[j]].OptionType=="PE"){
                    QDateTime dt1 = QDateTime::fromSecsSinceEpoch(sharedData->contract_table_hash[filteredKeys[i]].Expiry);
                    dt1 = dt1.addYears(10);
                    QString Leg2_date=dt1.toString("MMM dd yyyy").toUpper();
                    QString leg2=Leg2_date+" "+QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" CE";

                    QDateTime dt3 = QDateTime::fromSecsSinceEpoch(sharedData->contract_table_hash[filteredKeys[j]].Expiry);
                    dt3 = dt3.addYears(10);
                    QString Leg3_date=dt3.toString("MMM dd yyyy").toUpper();
                    QString leg3=Leg3_date+" "+QString::number(sharedData->contract_table_hash[filteredKeys[j]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" PE";

                    Leg2_token_number_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[i]].TokenNumber));
                    Leg3_token_number_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[j]].TokenNumber));
                    Leg2_strike_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice));
                    Leg3_strike_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[j]].StrikePrice));
                    Leg2_list.append(leg2);
                    Leg3_list.append(leg3);
                    //Con-Rev- EURUSD 24 JUN 1.005 CE - 0.005 - EURUSD 23 AUG PE.

//                    QString Algo_Name ="Con-Rev- "+Instr_Name+" "+dt1.toString("yyyy MMM").toUpper()+" "+
//                                        QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)
//                                        +" "+optionType+" - "+QString::number(((endStrike-startStrike)/sharedData->strike_price_devider),'f',sharedData->decimal_precision)+" - "
//                                        +" "+Instr_Name+" "+dt3.toString("yyyy MMM").toUpper();
//                    Algo_Name = "CR-";
//                    //            QString StockName = ContractDetail::getInstance().GetStockName(leg2_token_number);
//                    //            StockName.chop(2);
//                    Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM")+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number);

//                    QString Algo_Name ="CR-"+Instr_Name+"-"+dt.toString("ddMMM").toUpper()+"-"+QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+"-"+dt2.toString("ddMMM").toUpper();
//                    Algo_Name_list.append(Algo_Name);


                    QString Algo_Name ="CR- "+Instr_Name+" "+dt1.toString("yyyy MMM").toUpper()+" "+
                                        QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)
                                        +" "+optionType+" - "+QString::number(((endStrike-startStrike)/sharedData->strike_price_devider),'f',sharedData->decimal_precision)+" - "
                                        +" "+Instr_Name+" "+dt3.toString("yyyy MMM").toUpper();
                    Algo_Name_list.append(Algo_Name);


                    break;
                }

            }
        }
        else if(optionType=="PE"){
            int leg3 = sharedData->contract_table_hash[filteredKeys[i]].StrikePrice;
            for(int j=i+1;j<filteredKeys.length();j++){
                if(leg3==sharedData->contract_table_hash[filteredKeys[j]].StrikePrice&&sharedData->contract_table_hash[filteredKeys[j]].OptionType=="CE"){
                    QDateTime dt1 = QDateTime::fromSecsSinceEpoch(sharedData->contract_table_hash[filteredKeys[j]].Expiry);
                    dt1 = dt1.addYears(10);


                    QString Leg2_date=dt1.toString("MMM dd yyyy").toUpper();
                    QString leg2=Leg2_date+" "+QString::number(sharedData->contract_table_hash[filteredKeys[j]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" CE";

                    QDateTime dt3 = QDateTime::fromSecsSinceEpoch(sharedData->contract_table_hash[filteredKeys[i]].Expiry);
                    dt3 = dt3.addYears(10);
                    QString Leg3_date=dt3.toString("MMM dd yyyy").toUpper();
                    QString leg3=Leg3_date+" "+QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+" PE";

                    Leg2_token_number_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[j]].TokenNumber));
                    Leg3_token_number_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[i]].TokenNumber));
                    Leg2_strike_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[j]].StrikePrice));
                    Leg3_strike_list.append(QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice));
                    Leg2_list.append(leg2);
                    Leg3_list.append(leg3);
                    QString Algo_Name ="CR-"+Instr_Name+"-"+dt.toString("ddMMM").toUpper()+"-"+QString::number(sharedData->contract_table_hash[filteredKeys[i]].StrikePrice/sharedData->strike_price_devider,'f',sharedData->decimal_precision)+"-"+dt3.toString("ddMMM").toUpper();
                    Algo_Name_list.append(Algo_Name);
                    break;
                }

            }
        }
        else
            continue;

    }

    //populate table and algo data.
//    unsigned int unix_time= sharedData->contract_table_hash[keyFut].Expiry;
//    QDateTime dt = QDateTime::fromSecsSinceEpoch(unix_time);
//    dt = dt.addYears(10);
//    QString Leg1_val=dt.toString("MMM dd yyyy").toUpper();

    QStringList duplicateList;

    for(int i=0;i<Leg2_list.size();i++){

        bool exist=false;
        for(int k=0;k<sharedData->algo_data_list.length();k++){
            algo_data_to_insert tmp = sharedData->algo_data_list[k];
            if(tmp.Algo_Name == Algo_Name_list[i] && tmp.Leg1_token_number ==  QString::number(Leg1TokenNumeber) && tmp.Leg2_token_number == Leg2_token_number_list[i]&& tmp.Leg3_token_number == Leg3_token_number_list[i]){
                exist=true;
                break;
            }
        }

        //check for if item  is already added or upload in table and will not be in sharedData->algo_data_list
        for (int row = 0; row < tableWidget->rowCount(); ++row) {
            QString Instr_Name_Tmp="";
            QString Leg1_val_tmp = "";
            QString Leg2_list_tmp = "";
            QString Leg3_list_tmp = "";

            QTableWidgetItem *item0 = tableWidget->item(row, 0);
            if (item0) {
                Instr_Name_Tmp = item0->text();
            }
            QTableWidgetItem *item1 = tableWidget->item(row, 1);
            if (item1) {
                Leg1_val_tmp = item1->text();
            }
            QTableWidgetItem *item2 = tableWidget->item(row, 2);
            if (item2) {
                Leg2_list_tmp = item2->text();
            }
            QTableWidgetItem *item3 = tableWidget->item(row, 3);
            if (item3) {
                Leg3_list_tmp = item3->text();
            }
            if(Algo_Name_list[i]==Instr_Name_Tmp&&
                Leg1_val_tmp == Leg1_val&&
                Leg2_list_tmp == Leg2_list[i]&&
                Leg3_list_tmp == Leg3_list[i]
                ){
                exist=true;
                break;
            }

        }

        if(exist){
            duplicateList.append("Duplicate entry Skipping, Leg1="+ QString::number(Leg1TokenNumeber)+", Leg2="+Leg2_token_number_list[i]+", Leg3="+Leg3_token_number_list[i]);
            continue;
        }

        tableWidget->insertRow( tableWidget->rowCount() );

        sharedData->unique_id++;
        QTableWidgetItem *c0 = new QTableWidgetItem();
        c0->setData(Qt::UserRole + 1,sharedData->unique_id);
        c0->setData(Qt::DisplayRole,Algo_Name_list[i]);
        c0->setFlags(c0->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 0, c0);


        QTableWidgetItem *c1 = new QTableWidgetItem(Leg1_val);
        c1->setFlags(c1->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 1, c1);

        QTableWidgetItem *c2 = new QTableWidgetItem(Leg2_list[i]);
        c2->setFlags(c2->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 2, c2);

        QTableWidgetItem *c3 = new QTableWidgetItem(Leg3_list[i]);
        c3->setFlags(c3->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 3, c3);

        QTableWidgetItem *c4 = new QTableWidgetItem("NA");
        c4->setFlags(c4->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 4, c4);

        QTableWidgetItem *c5 = new QTableWidgetItem("NA");
        c5->setFlags(c5->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 5, c5);


        QTableWidgetItem *c6 = new QTableWidgetItem("To be Uploaded");
        c6->setFlags(c6->flags() ^ Qt::ItemIsEditable);
        tableWidget->setItem(tableWidget->rowCount()-1, 6, c6);

        algo_data_to_insert data;
        data.Algo_Status = "DisabledByUser";
        data.algo_type = QString::number(PortfolioType::CR);
        data.exchange = sharedData->exchange;
        data.user_id = sharedData->foo_user_id;
        data.Algo_Name = Algo_Name_list[i];
        data.Leg1_token_number = QString::number(Leg1TokenNumeber);
        data.Leg2_token_number = Leg2_token_number_list[i];
        data.Leg3_token_number = Leg3_token_number_list[i];
        data.indicator = "1";
        data.Leg1_Strike = QString::number(Leg1_Strike);
        data.Leg2_Strike = Leg2_strike_list[i];
        data.Leg3_Strike = Leg3_strike_list[i];
        data.option_type = Option_Type;
        data.table_row_unique_id = sharedData->unique_id;
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


}





