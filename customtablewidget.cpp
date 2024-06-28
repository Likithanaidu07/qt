#include "CustomTableWidget.h"

void CustomTableWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab)
    {
        int currentRow = currentRow();
        int currentColumn = currentColumn();
        int totalRows = rowCount();
        int totalColumns = columnCount();

        if (currentRow == totalRows - 1 && currentColumn == totalColumns - 1)
        {
            // Move focus to the next widget, e.g., pushButtonUpload
            QWidget *nextWidget = findNextWidget();
            if (nextWidget)
            {
                nextWidget->setFocus();
                return; // Stop further event processing
            }
        }
    }
    // Call the base class implementation for other keys
    QTableWidget::keyPressEvent(event);
}

QWidget* CustomTableWidget::findNextWidget()
{
    // Implement logic to find and return the next widget
    // For example, return the widget you want to focus on after the table widget
    // Assuming `parentWidget` is the parent widget containing the pushButtonUpload
    if (auto parentWidget = parentWidget())
    {
        return parentWidget->findChild<QPushButton*>("pushButtonUpload");
    }
    return nullptr; // Replace with actual logic if needed
}
