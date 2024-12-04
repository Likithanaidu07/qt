#include "watch_data_card.h"
#include "ui_watch_data_card.h"


watch_data_card::watch_data_card(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::watch_data_card)
{
    ui->setupUi(this);
    ui->Background->setStyleSheet("background-color: lightgray; "
                                  "border: 1px solid lightgray; "
                                   "border-radius: 3px;");

    ui->name->setStyleSheet("font-family: 'Work Sans';"
                        "font-size: 12px;"
                        "font-weight: 500;"
                        "color:#2F3339"
                        );

    ui->value1->setStyleSheet("font-family: 'Work Sans';"
                        "font-size: 10px;"
                        "font-weight: 400;"
                        "color:#008000"
                        );

    ui->value2->setStyleSheet("font-family: 'Work Sans';"
                        "font-size: 10px;"
                        "font-weight: 400;"
                        "color:#495057"
                        );



}
QString watch_data_card::getIndexName(){
    return data.indexName;
}

void watch_data_card::setData(Indices_Data_Struct data) {
    bool redArrow = true;
    //if(data.netChangeIndicator=="+")
    // redArrow = false;
    if(data.change.toDouble()>=0)
        redArrow = false;

    this->data = data;

    if(redArrow){
        QPixmap pixmap(":/arrow_red.png");
        pixmap = pixmap.scaled(ui->indicator->width(),ui->indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->indicator->setPixmap(pixmap);
        ui->indicator->setMask(pixmap.mask());
        ui->value1->setStyleSheet("font-family: 'Work Sans';"
                                  "font-size: 12px;"
                                  "font-weight: 400;"
                                  "color:#CC3437"
                                  );
    }
    else{
        QPixmap pixmap(":/arrow_green.png");
        pixmap = pixmap.scaled(ui->indicator->width(),ui->indicator->height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->indicator->setPixmap(pixmap);
        ui->indicator->setMask(pixmap.mask());

        ui->value1->setStyleSheet("font-family: 'Work Sans';"
                                  "font-size: 12px;"
                                  "font-weight: 400;"
                                  "color:#008000"
                                  );
    }

   //QString percent = (data.change+"("+data.percentagechange+"%)");
    ui->name->setText(data.indexName);
    ui->value1->setText(data.indexValue);
    ui->value2->setText(data.change);
    ui->value3->setText(data.percentagechange+"%");


    int newWidth = ui->Background->sizeHint().width(); // Use sizeHint() to estimate the required width
    //qDebug()<<newWidth;
    this->setMinimumWidth(newWidth);
    this->setMaximumWidth(newWidth);

}


watch_data_card::~watch_data_card()
{
    delete ui;
}
