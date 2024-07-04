#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>
#include <QPushButton>

class CustomTableWidget : public QTableWidget {
    Q_OBJECT

public:
    CustomTableWidget(QPushButton *button2, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPushButton *button2;
};

#endif // CUSTOMTABLEWIDGET_H
