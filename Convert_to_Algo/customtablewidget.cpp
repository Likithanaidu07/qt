#include "customtablewidget.h"



CustomTableWidget::CustomTableWidget(QWidget *parent)
    : QTableWidget(parent)
{


}

void CustomTableWidget::setNextFocusButton(QPushButton *button)
{
    nextButton = button;
}

void CustomTableWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        // Move focus to the button when Tab is pressed
        if (nextButton) {
            nextButton->setFocus();
            return;
        }
    }
    QTableWidget::keyPressEvent(event);
}

void CustomTableWidget::focusInEvent(QFocusEvent *event)
{
    QTableWidget::focusInEvent(event);

    // Select the first row when the table receives focus
    if (rowCount() > 0 && selectedItems().isEmpty()) {
        selectRow(0);          // Select the first row
    }
}
