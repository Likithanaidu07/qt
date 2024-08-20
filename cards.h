#ifndef CARDS_H
#define CARDS_H

#include <QDialog>

namespace Ui {
class Cards;
}

class Cards : public QDialog
{
    Q_OBJECT

public:
    explicit Cards(QWidget *parent = nullptr);
    ~Cards();

private:
    Ui::Cards *ui;
};

#endif // CARDS_H
