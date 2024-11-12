#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H


#include <QTableWidget>
#include <QPushButton>
#include <QKeyEvent>

class CustomTableWidget : public QTableWidget {
    Q_OBJECT

public:
    explicit CustomTableWidget(QWidget *parent = nullptr);
    void setNextFocusButton(QPushButton *button);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;

private:
    QPushButton *nextButton = nullptr;
};


#endif // CUSTOMTABLEWIDGET_H
