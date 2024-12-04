#include "watch_cards.h"
#include "ui_watch_cards.h"
#include "QListWidgetItem"
#include "watch_data_list_item.h"
#include <QKeyEvent>

Watch_cards::Watch_cards(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Watch_cards)
{
    ui->setupUi(this);
   // this->setFixedSize(330, 325);

    ui->widget_15->setVisible(false);
    initWatchWindow();
}

Watch_cards::~Watch_cards()
{
    delete ui;
}
QDataStream &operator>>(QDataStream &in, Indices_Data_Struct &data) {
    in >> data.indexName >> data.indexValue >> data.change >> data.percentagechange
        >> data.netChangeIndicator >> data.openingIdx >> data.closingIdx
        >> data.highIndexValue >> data.lowIndexValue >> data.marketCapitialisation
        >> data.display_widget_idx;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Indices_Data_Struct &data) {
    out << data.indexName << data.indexValue << data.change << data.percentagechange
        << data.netChangeIndicator << data.openingIdx << data.closingIdx
        << data.highIndexValue << data.lowIndexValue << data.marketCapitialisation
        << data.display_widget_idx;
    return out;
}
void Watch_cards::initWatchWindow(){
    loadWatch_InstrumentNames();
    loadIndicesDataListFromFile(indicesDataList);
    showSaveWatchOnListView();

}



void Watch_cards::loadWatch_InstrumentNames(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if(settings.contains("Watch_InstrumentNames")){
            QString watch=  settings.value("Watch_InstrumentNames").toString();
            if(watch=="")
                savedWatchItems.clear();
            else
                savedWatchItems = watch.split(";");
        }
        if(settings.contains("Watch_InstrumentNames_MainWin")){
            QString watch=  settings.value("Watch_InstrumentNames_MainWin").toString();
            if(watch=="")
                savedWatchItemsMainWin.clear();
            else
                savedWatchItemsMainWin = watch.split(";");
        }


        settings.endGroup();
    }

}

//void Watch_cards::saveIndicesDataListToFile(const QHash<QString, Indices_Data_Struct> &indicesDataList) {
//    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
//    QString fileName = appDataPath+"/Data/indices_data.bin";
//    QFile file(fileName);
//    if (!file.open(QIODevice::WriteOnly)) {
//        qWarning("Could not open file for writing.");
//        return;
//    }
//    QDataStream out(&file);
//    out << indicesDataList;
//    file.close();
//}

void Watch_cards::loadIndicesDataListFromFile(QHash<QString, Indices_Data_Struct> &indicesDataList) {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = appDataPath+"/Data/indices_data.bin";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open file for reading--> indices_data.bin");
        return;
    }
    QDataStream in(&file);
    in >> indicesDataList;
    file.close();
}


void Watch_cards::indicesDataRecv_Slot(Indices_Data_Struct data){
   // emit indicesDataRecv_Signal_watch_card(data);

    //remove below code
    bool newIndicesData = false;
    bool updatedIndicesData = false;

    indicesDataMutex.lock();
    if(!indicesDataList.contains(data.indexName)){
        indicesDataList.insert(data.indexName,data);
        newIndicesData = true;
    }
    //check any of the value changed
    else if((indicesDataList[data.indexName].indexValue!=data.indexValue) || (indicesDataList[data.indexName].change!=data.change) || (indicesDataList[data.indexName].percentagechange!=data.percentagechange)){
        indicesDataList[data.indexName]=data;
        updatedIndicesData = true;
    }

    indicesDataMutex.unlock();
    // qDebug()<<"indicesDataRecv_Slot: " <<data.indexName;

    if(newIndicesData==true&&ui->lineEditWatchSearch->text()=="")
        showSaveWatchOnListView();

    if(updatedIndicesData==true){

        refreshIndicesListViewItem(data);

        QListWidgetItem *item = ui->listWidgetWatch->currentItem();
        if (item) {
            watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(item));
            if (widget) {
                updateSelecteWatch_UI( widget->data);
            }
        }

    }

}

void Watch_cards::refreshIndicesListViewItem(const Indices_Data_Struct& data) {
    for (int j = 0; j < ui->listWidgetWatch->count(); j++) {
        QListWidgetItem* existingItem = ui->listWidgetWatch->item(j);
        watch_Data_List_Item* existingWidget = qobject_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(existingItem));

        if (existingWidget && existingWidget->data.indexName == data.indexName) {
            // Item exists, update it with new data
            existingWidget->setData(data, &savedWatchItemsMainWin);
            return; // Exit the function once the item is updated
        }
    }

}

void Watch_cards::on_lineEditWatchSearch_textChanged(const QString &text)
{
    if(text==""){
        showSaveWatchOnListView();
        return;
    }

    indicesDataMutex.lock();

    QHash<QString, Indices_Data_Struct> indicesDataListTmp = indicesDataList;
    indicesDataListTmp.detach();
    indicesDataMutex.unlock();


    /*QStringList nameList;
    nameList.append("Nifty50");
    nameList.append("BankNifty");
    nameList.append("FinNifty");
    nameList.append("SmallCap");
    nameList.append("ABC");
    nameList.append("DDD");
    nameList.append("ee");
    nameList.append("dd");
    nameList.append("ABC");
    nameList.append("de");
    nameList.append("ddd");
    nameList.append("wsd");
    nameList.append("dfe");
    nameList.append("fg");
    nameList.append("gf");
    nameList.append("rt");
    nameList.append("fg");*/


    /*QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
    QStringList filteredList;

    for (auto it = indicesDataListTmp.constBegin(); it != indicesDataListTmp.constEnd(); ++it) {
        const QString &key = it.key();
        if (regex.match(key).hasMatch()) {
            filteredList.append(key);
        }
        // const Indices_Data_Struct &value = it.value();
    }*/


    QRegularExpression regex(text, QRegularExpression::CaseInsensitiveOption);
    QStringList filteredList;
    QStringList prioritizedList;

    for (auto it = indicesDataListTmp.constBegin(); it != indicesDataListTmp.constEnd(); ++it) {
        const QString &key = it.key();
        if (regex.match(key).hasMatch()) {
            // Check if the key exists in savedWatchItemsMainWin
            if (savedWatchItemsMainWin.contains(key)) {
                prioritizedList.append(key); // Add to prioritized list
            } else {
                filteredList.append(key); // Add to regular filtered list
            }
        }
    }

    // Combine prioritized items and remaining filtered items
    filteredList = prioritizedList + filteredList;



    /*for(const QString &name : nameList) {
        if (regex.match(name).hasMatch()) {
            filteredList.append(name);
        }
    }*/

    ui->listWidgetWatch->clear();
    int selecteIDx = -1;
    for(int i=0;i<filteredList.size();i++){
        auto item = new QListWidgetItem();
        auto widget1 = new watch_Data_List_Item(this);
        Indices_Data_Struct data = indicesDataListTmp[filteredList[i]];
        widget1->setData(data,&savedWatchItemsMainWin);
        item->setSizeHint(widget1->sizeHint());

        ui->listWidgetWatch->addItem(item);
        ui->listWidgetWatch->setItemWidget(item, widget1);
        if(data.indexName==watchItemSelectedindexName)
            selecteIDx = ui->listWidgetWatch->count()-1;

        // Connect the signal to a lambda function
        connect(widget1, &watch_Data_List_Item::add_watch_item_to_main_win_signal, this, [this](bool checked,Indices_Data_Struct dataSelected) {
            // Lambda function code to handle the signal
            if (checked) {
                // Perform action if checkbox is checked
               // qDebug() << "Checkbox checked for item" << dataSelected.indexName;
                addToSavedWatchItems(dataSelected,"Watch_InstrumentNames_MainWin");

            } else {
                // Perform action if checkbox is unchecked
                //qDebug() << "Checkbox unchecked for item" << dataSelected.indexName;
                removeFromSavedWatchItems(dataSelected,"Watch_InstrumentNames_MainWin");

            }
        });
    }
    if(selecteIDx>-1){
        ui->listWidgetWatch->setCurrentRow(selecteIDx);
    }


}
void Watch_cards::showSaveWatchOnListView(){

    ui->listWidgetWatch->clear();
    if(savedWatchItems.size()==0){
        ui->watchListSelectedDetails_Parent->setVisible(false);
        return;
    }
    indicesDataMutex.lock();

    QHash<QString, Indices_Data_Struct> indicesDataListTmp = indicesDataList;
    indicesDataListTmp.detach();
    indicesDataMutex.unlock();

    QStringList prioritizedList;
    QStringList nonPrioritizedList;

    for (const QString &item : savedWatchItems) {
        // Check if the item exists in savedWatchItemsMainWin
        if (savedWatchItemsMainWin.contains(item)) {
            prioritizedList.append(item); // Add to prioritized list
        } else {
            nonPrioritizedList.append(item); // Add to non-prioritized list
        }
    }

    // Combine prioritized and non-prioritized lists
    savedWatchItems = prioritizedList + nonPrioritizedList;


    int selecteIDx = -1;
    for(int i=0;i<savedWatchItems.length();i++){
        if(indicesDataListTmp.contains(savedWatchItems[i])){
            auto item = new QListWidgetItem();
            auto widget1 = new watch_Data_List_Item(this);
            Indices_Data_Struct data = indicesDataListTmp[savedWatchItems[i]];
            widget1->setData(data,&savedWatchItemsMainWin);
            item->setSizeHint(widget1->sizeHint());
            ui->listWidgetWatch->addItem(item);
            ui->listWidgetWatch->setItemWidget(item, widget1);
            if(data.indexName==watchItemSelectedindexName)
                selecteIDx = ui->listWidgetWatch->count()-1;

            // Connect the signal to a lambda function
            connect(widget1, &watch_Data_List_Item::add_watch_item_to_main_win_signal, this, [this](bool checked,Indices_Data_Struct dataSelected) {
                // Lambda function code to handle the signal
                if (checked) {
                    // Perform action if checkbox is checked
                    //qDebug() << "Checkbox checked for item" << dataSelected.indexName;
                    addToSavedWatchItems(dataSelected,"Watch_InstrumentNames_MainWin");

                } else {
                    // Perform action if checkbox is unchecked
                    removeFromSavedWatchItems(dataSelected,"Watch_InstrumentNames_MainWin");
                   //qDebug() << "Checkbox unchecked for item" << dataSelected.indexName;
                }
            });
        }
    }
    if(selecteIDx>-1){
        ui->listWidgetWatch->setCurrentRow(selecteIDx);
    }
    if(ui->listWidgetWatch->selectedItems().count()==0){
        ui->watchListSelectedDetails_Parent->setVisible(false);
    }



}

void Watch_cards::on_listWidgetWatch_itemClicked(QListWidgetItem *item)
{
    if (!item) {
        ui->watchListSelectedDetails_Parent->setHidden(true);
        return; // Handle no item clicked scenario
    }

    watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(item));
    if (!widget) {
        ui->watchListSelectedDetails_Parent->setHidden(true);
        return; // Handle case where no widget is associated with the item
    }

    watchItemSelectedindexName = widget->data.indexName;

    updateSelecteWatch_UI(widget->data);

}

void Watch_cards::updateSelecteWatch_UI( Indices_Data_Struct data){
    ui->watchListSelectedDetails_Parent->setVisible(true);

    bool redArrow = true;
    //if(data.netChangeIndicator=="+")
    // redArrow = false;
    if(data.change.toDouble()>=0)
        redArrow = false;

    if(redArrow){
        QPixmap pixmap(":/arrow_red.png");
        pixmap = pixmap.scaled(ui->watch_indicator->width(),ui->watch_indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->watch_indicator->setPixmap(pixmap);
        ui->watch_indicator->setMask(pixmap.mask());
        ui->watch_indicator->setStyleSheet("font-family: 'Work Sans';"
                                           "font-size: 10px;"
                                           "font-weight: 500;"
                                           "color:#CC3437"
                                           );
    }
    else{
        QPixmap pixmap(":/arrow_green.png");
        pixmap = pixmap.scaled(ui->watch_indicator->width(),ui->watch_indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->watch_indicator->setPixmap(pixmap);
        ui->watch_indicator->setMask(pixmap.mask());

        ui->watch_val1->setStyleSheet("font-family: 'Work Sans';"
                                      "font-size: 10px;"
                                      "font-weight: 500;"
                                      "color:#008000"
                                      );
    }

    QString percent = (data.change+"("+data.percentagechange+"%)");
    ui->Index_Name->setText(data.indexName);
    ui->watch_val1->setText(data.indexValue);
    ui->watch_val2->setText(percent);
    ui->Open_Num->setText(data.openingIdx);
    ui->High_Num->setText(data.highIndexValue);
    ui->Low_Num->setText(data.lowIndexValue);
    ui->Volume_Num->setText(data.marketCapitialisation);



}

void Watch_cards::on_listWidgetWatch_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItem *selectedItem = ui->listWidgetWatch->currentItem();
    if (selectedItem) {
        watch_Data_List_Item *widget = static_cast<watch_Data_List_Item*>(ui->listWidgetWatch->itemWidget(selectedItem));

        //if selected item already in saved item list remove from else or add it
        if(savedWatchItems.contains(widget->data.indexName)){
            removeFromSavedWatchItems(widget->data,"Watch_InstrumentNames");
        }
        else{
            addToSavedWatchItems(widget->data,"Watch_InstrumentNames");
        }

        ui->lineEditWatchSearch->clear();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Please select an item from Watch.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

}

void Watch_cards::addToSavedWatchItems(Indices_Data_Struct data,QString key)
{
    if(key=="Watch_InstrumentNames"){
        if(!savedWatchItems.contains(data.indexName)){
            savedWatchItems.append(data.indexName);
            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
            settings.beginGroup("GeneralSettings");
            if(savedWatchItems.length()==1)
                settings.setValue(key,savedWatchItems[0]);
            else
                settings.setValue(key,savedWatchItems.join(";"));
            settings.endGroup();

            if(ui->lineEditWatchSearch->text()=="")
                showSaveWatchOnListView();

        }
        else{
            QMessageBox msgBox;
            msgBox.setText("IndexName already added.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }

    else if(key=="Watch_InstrumentNames_MainWin"){



        if(!savedWatchItemsMainWin.contains(data.indexName)){
            savedWatchItemsMainWin.append(data.indexName);
            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
            settings.beginGroup("GeneralSettings");
            if(savedWatchItemsMainWin.length()==1)
                settings.setValue(key,savedWatchItemsMainWin[0]);
            else
                settings.setValue(key,savedWatchItemsMainWin.join(";"));
            settings.endGroup();

            emit add_remove_watch_card_signal(true,data);

        }
        else{
            QMessageBox msgBox;
            msgBox.setText("IndexName already added.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
}

void Watch_cards::removeFromSavedWatchItems( Indices_Data_Struct data,QString key)
{
    if(key=="Watch_InstrumentNames"){
        if(savedWatchItems.contains(data.indexName)){
            savedWatchItems.removeOne(data.indexName);
            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
            settings.beginGroup("GeneralSettings");
            if(savedWatchItems.length()==1)
                settings.setValue(key,savedWatchItems[0]);
            else
                settings.setValue(key,savedWatchItems.join(";"));
            settings.endGroup();
            if(ui->lineEditWatchSearch->text()=="")
                showSaveWatchOnListView();


        }
        else{
            QMessageBox msgBox;
            msgBox.setText("IndexName not exist in Saved List.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
    else if(key=="Watch_InstrumentNames_MainWin"){
        if(savedWatchItemsMainWin.contains(data.indexName)){
            savedWatchItemsMainWin.removeOne(data.indexName);
            QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
            settings.beginGroup("GeneralSettings");
            if(savedWatchItemsMainWin.length()==1)
                settings.setValue(key,savedWatchItemsMainWin[0]);
            else
                settings.setValue(key,savedWatchItemsMainWin.join(";"));
            settings.endGroup();

            emit add_remove_watch_card_signal(false,data);

        }
        else{
            QMessageBox msgBox;
            msgBox.setText("IndexName not exist in Saved List.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }

}
void Watch_cards::on_listWidgetWatch_itemSelectionChanged()
{
    if(ui->listWidgetWatch->selectedItems().count()==0){
        ui->watchListSelectedDetails_Parent->setVisible(false);
    }
}
void Watch_cards::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        ui->lineEditWatchSearch->clear();
    }
}
