#include "sortsettingpopup.h"
#include "ui_sortsettingpopup.h"
#include "QStandardPaths"
#include "QSettings"

SortSettingPopUp::SortSettingPopUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SortSettingPopUp)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList list{"Algo Status","Algo Name", "Strategy","Expiry","Start Strike","End Strike","Option Type"};
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

              QString selectedText = tok[0];

              int row = ui->tableWidget->rowCount();
              ui->tableWidget->insertRow(row);

              // First column with text
              QTableWidgetItem *item = new QTableWidgetItem(selectedText);
              ui->tableWidget->setItem(row, 0, item);

              // Second column with ComboBox
              QComboBox *comboBox = new QComboBox();
              comboBox->addItem("Ascending");
              comboBox->addItem("Descending");
              ui->tableWidget->setCellWidget(row, 1, comboBox);
              comboBox->setCurrentText(tok[1]);
          }

      }
    }
    ui->comboBoxSortItems->addItems(list);






}

SortSettingPopUp::~SortSettingPopUp()
{
    delete ui;
}

void SortSettingPopUp::on_buttonBox_accepted()
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


}



void SortSettingPopUp::on_buttonBox_rejected()
{
    this->close();
}


void SortSettingPopUp::on_pushButtonRemove_clicked()
{
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
      comboBox->addItem("Ascending");
      comboBox->addItem("Descending");
      ui->tableWidget->setCellWidget(row, 1, comboBox);
}



