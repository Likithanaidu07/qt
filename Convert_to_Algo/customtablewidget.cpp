#include "customtablewidget.h"

#include <QKeyEvent>

CustomTableWidget::CustomTableWidget(QPushButton *button2, QWidget *parent)
    : QTableWidget(parent), button2(button2) {}

void CustomTableWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Tab) {
        int row = currentRow();
        int column = currentColumn();
        if (row == rowCount() - 1 && column == columnCount() - 1) {
            // Move focus to button2 and reset the focus to the first cell
            button2->setFocus();
            setCurrentCell(0, 0);
            return;
        }
    }
    QTableWidget::keyPressEvent(event);
}

