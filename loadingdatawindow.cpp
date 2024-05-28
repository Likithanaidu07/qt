#include "loadingdatawindow.h"
#include "ui_loadingdatawindow.h"

loadingdatawindow::loadingdatawindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loadingdatawindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    //setAttribute(Qt::WA_TranslucentBackground);
    setWindowModality(Qt::ApplicationModal); // Ensure this window blocks interaction with other windows

}

loadingdatawindow::~loadingdatawindow()
{
    delete ui;
}
