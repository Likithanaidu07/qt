#include "cards.h"
#include "ui_cards.h"

Cards::Cards(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cards)
{
    ui->setupUi(this);
}

Cards::~Cards()
{
    delete ui;
}
