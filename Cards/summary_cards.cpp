#include "summary_cards.h"
#include "ui_summary_cards.h"

Summary_cards::Summary_cards(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Summary_cards)
{
    ui->setupUi(this);

    this->setFixedSize(330, 270);

    ui->widget_14->setVisible(false);
}
QString Summary_cards::fixDecimal(double num, int decimal_precision) {
    QString str = QString::number(num, 'f', decimal_precision);  // Removed +1 for correct precision
    return str;
}


void Summary_cards::updateSummaryData(const QStringList &data) {
    if (data.size() >= 8) {
        ui->label_3->setText(data[0]);
        ui->label_3->setAlignment(Qt::AlignCenter);

        ui->label_25->setText(data[1]);
        ui->label_25->setAlignment(Qt::AlignCenter);

        ui->label_30->setText(data[2]);
        ui->label_30->setAlignment(Qt::AlignCenter);


        ui->label_23->setText(data[3]);
        ui->label_23->setAlignment(Qt::AlignCenter);

        ui->label_26->setText(data[4]);
        ui->label_26->setAlignment(Qt::AlignCenter);

        ui->label_31->setText(data[5]);
        ui->label_31->setAlignment(Qt::AlignCenter);

        ui->label_24->setText(data[6]);
        ui->label_24->setAlignment(Qt::AlignCenter);

        ui->label_27->setText(data[7]);
        ui->label_27->setAlignment(Qt::AlignCenter);

        ui->label_32->setText(data[8]);
        ui->label_32->setAlignment(Qt::AlignCenter);
    }
}


Summary_cards::~Summary_cards()
{
    delete ui;
}
