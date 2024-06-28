#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>
#include <QKeyEvent>
#include <QWidget>

class CustomTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    using QTableWidget::QTableWidget;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QWidget *findNextWidget();
};

#endif // CUSTOMTABLEWIDGET_H
