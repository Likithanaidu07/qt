#include "sortsettingpopup.h"
#include "ui_sortsettingpopup.h"
#include "QStandardPaths"
#include "QSettings"

SortSettingPopUp::SortSettingPopUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortSettingPopUp)
{
    ui->setupUi(this);
    saveFlg = false;
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList list{"Algo Status","Instrument Name", "Algo Type","Expiry","Middle Strike","Strike Difference","Option Type"};
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/sort_data.dat", QSettings::IniFormat);
    if (settings.contains("portfolio_sort_data"))
    {
      QString saveStr =  settings.value("portfolio_sort_data").toString();
      QStringList saveStrList = saveStr.split(";");
      for(int i=0;i<saveStrList.size();i++){
          QStringList tok = saveStrList[i].split(":");
          if(tok.length()==2){
              if(list.contains(tok[0]))
                 list.removeAll(tok[0]);

              QString sortItemTxt = tok[0];
              QString sortOrderTxt = tok[1];


              int row = ui->tableWidget->rowCount();
              ui->tableWidget->insertRow(row);

              // First column with text
              QTableWidgetItem *item = new QTableWidgetItem(sortItemTxt);
              ui->tableWidget->setItem(row, 0, item);

              // Second column with ComboBox
              QComboBox *comboBox = new QComboBox();

              if(sortItemTxt == "Algo Status"){
                  comboBox->addItem("Enabled");
                  comboBox->addItem("Disabled");
                  comboBox->setCurrentText(sortOrderTxt);
              }
              else if(sortItemTxt == "Option Type"){
                  comboBox->addItem("CE");
                  comboBox->addItem("PE");
                  comboBox->setCurrentText(sortOrderTxt);
              }
              else if(sortItemTxt == "Expiry"){
                  comboBox->addItem("Sort by Far Date");

                  comboBox->addItem("Sort By Near Date");
                  comboBox->setCurrentText(sortOrderTxt);
              }
              else{
                  comboBox->addItem("Ascending");
                  comboBox->addItem("Descending");
                  comboBox->setCurrentText(sortOrderTxt);
              }
              ui->tableWidget->setCellWidget(row, 1, comboBox);
              QObject::connect(comboBox, &QComboBox::currentTextChanged, [=](const QString &text) {
                  saveFlg = true;
                 });
          }

      }
    }
    ui->comboBoxSortItems->addItems(list);






}

SortSettingPopUp::~SortSettingPopUp()
{
    delete ui;
}


void SortSettingPopUp::saveConfig()
{
    QStringList saveStr;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *itemText = ui->tableWidget->item(row, 0);
        QWidget *itemCombo = ui->tableWidget->cellWidget(row, 1);
        if (itemCombo&&itemText) {
            QComboBox *comboBox = qobject_cast<QComboBox*>(itemCombo);
            QString sortOrder = comboBox->currentText();
            QString sortItemText = itemText->text();
            saveStr.append(sortItemText+":"+sortOrder);
        }
    }

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/sort_data.dat", QSettings::IniFormat);
    settings.setValue("portfolio_sort_data",saveStr.join(";"));

    emit reloadSortSettingSignal();
}
void SortSettingPopUp::on_buttonBox_accepted()
{
    saveConfig();
}



void SortSettingPopUp::on_buttonBox_rejected()
{
    this->close();
}


void SortSettingPopUp::on_pushButtonRemove_clicked()
{
     saveFlg = true;
    int row = ui->tableWidget->currentRow();
        if (row != -1) {
            // Get the text from the first column
            QTableWidgetItem *item = ui->tableWidget->item(row, 0);
            if (item) {
                QString filterItemText = item->text();
                ui->comboBoxSortItems->addItem(filterItemText);
                if(!ui->pushButtonAdd->isEnabled())
                    ui->pushButtonAdd->setEnabled(true);
            }

            // Remove the selected row
            ui->tableWidget->removeRow(row);
        }
}


void SortSettingPopUp::on_pushButtonAdd_clicked()
{
      saveFlg = true;
      QString selectedText = ui->comboBoxSortItems->currentText();
      ui->comboBoxSortItems->removeItem(ui->comboBoxSortItems->currentIndex());
      if(ui->comboBoxSortItems->currentText()==""){
          ui->pushButtonAdd->setDisabled(true);
      }

      int row = ui->tableWidget->rowCount();
      ui->tableWidget->insertRow(row);

      // First column with text
      QTableWidgetItem *item = new QTableWidgetItem(selectedText);
      ui->tableWidget->setItem(row, 0, item);

      // Second column with ComboBox
       QComboBox *comboBox = new QComboBox();
      if(selectedText == "Algo Status"){
          comboBox->addItem("Enabled");
          comboBox->addItem("Disabled"); 
      }
      else if(selectedText == "Option Type"){
          comboBox->addItem("CE");
          comboBox->addItem("PE");
      }
      else if(selectedText == "Expiry"){
          comboBox->addItem("Sort by Far Date");
          comboBox->addItem("Sort By Near Date");
      }
      else{
         comboBox->addItem("Ascending");
         comboBox->addItem("Descending");
      }
      ui->tableWidget->setCellWidget(row, 1, comboBox);
}



void SortSettingPopUp::closeEvent(QCloseEvent *event) {

    if(saveFlg){
        QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Save Changes", "Do you want to save your changes?",
                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

                if (reply == QMessageBox::Yes) {
                    // Call the function to save data
                    saveConfig();
                    event->accept();
                } else if (reply == QMessageBox::No) {
                    event->accept();
                } else {
                    event->ignore();
                }
    }
    else{
        event->accept();
    }

}


