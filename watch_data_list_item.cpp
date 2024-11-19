#include "watch_data_list_item.h"
#include "ui_watch_data_list_item.h"
#include "QMessageBox"

watch_Data_List_Item::watch_Data_List_Item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::watch_Data_List_Item)
{
    ui->setupUi(this);

    ui->name->setStyleSheet("font-family: 'Work Sans';"
                        "font-size: 14px;"
                        "font-weight: 500;"
                        "color:#2F3339"
                        );

    ui->value1->setStyleSheet("font-family: 'Work Sans';"
                        "font-size: 12px;"
                        "font-weight: 500;"
                        "color:#008000"
                        );

    ui->value2->setStyleSheet("font-family: 'Work Sans';"
                        "font-size: 11px;"
                        "font-weight: 400;"
                        "color:#495057"
                        );
}

void watch_Data_List_Item::setData(Indices_Data_Struct data, QStringList* savedWatchItemsMainWinPtr_) {

    bool redArrow = true;
    //if(data.netChangeIndicator=="+")
    // redArrow = false;
    if(data.change.toDouble()>=0)
        redArrow = false;

    this->data = data;
    this->savedWatchItemsMainWinPtr = savedWatchItemsMainWinPtr_; // Assign pointer

    if(redArrow){
        QPixmap pixmap(":/arrow_red.png");
        pixmap = pixmap.scaled(ui->indicator->width(),ui->indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->indicator->setPixmap(pixmap);
        ui->indicator->setMask(pixmap.mask());
        ui->value1->setStyleSheet("font-family: 'Work Sans';"
                                  "font-size: 13px;"
                                  "font-weight: 600;"
                                  "color:#CC3437"
                                  );
    }
    else{
        QPixmap pixmap(":/arrow_green.png");
        pixmap = pixmap.scaled(ui->indicator->width(),ui->indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->indicator->setPixmap(pixmap);
        ui->indicator->setMask(pixmap.mask());

        ui->value1->setStyleSheet("font-family: 'Work Sans';"
                                  "font-size: 13px;"
                                  "font-weight: 600;"
                                  "color:#008000"
                                  );
    }

    QString percent = (data.change+"("+data.percentagechange+"%)");
    ui->name->setText(data.indexName);
    ui->value1->setText(data.indexValue);
    ui->value2->setText(percent);

    ui->checkBoxShowMainScreen->setChecked(savedWatchItemsMainWinPtr->contains(data.indexName));

}


watch_Data_List_Item::~watch_Data_List_Item()
{
    delete ui;
}



void watch_Data_List_Item::on_checkBoxShowMainScreen_clicked(bool checked)
{
    if(checked){
        if(savedWatchItemsMainWinPtr->size()==6){
            QMessageBox msgBox;
            msgBox.setText("Cannot add more than 6 Index , please remove 1 item and try again.!");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            ui->checkBoxShowMainScreen->setChecked(false);

            return;
        }
    }

   emit add_watch_item_to_main_win_signal(checked,data);

}

