#ifndef LOADINGDATAWINDOW_H
#define LOADINGDATAWINDOW_H

#include <QWidget>

namespace Ui {
class loadingdatawindow;
}

class loadingdatawindow : public QWidget
{
    Q_OBJECT

public:
    explicit loadingdatawindow(QWidget *parent = nullptr);
    ~loadingdatawindow();

private:
    Ui::loadingdatawindow *ui;
};

#endif // LOADINGDATAWINDOW_H
