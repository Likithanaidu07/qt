#include "logs_cards.h"
#include "ui_logs_cards.h"

Logs_cards::Logs_cards(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logs_cards)
{
    ui->setupUi(this);

    this->setFixedSize(330, 230);

    ui->widget_16->setVisible(false);
}

Logs_cards::~Logs_cards()
{
    delete ui;
}



void Logs_cards::logDataSignal_Slot(QStringList data)
{
    ui->textEdit->clear();
    QString logsContent;

    for (int i = data.length() - 1; i >= 0; i--) {
        logsContent.append(data[i] + "\n"); // Collect logs in reverse order
    }

    ui->textEdit->setText(logsContent);
}


