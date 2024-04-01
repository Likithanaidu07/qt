#include "eventfilterlistview.h"
#include <QEvent>
#include "qevent.h"

bool EventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == lineEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Up )
        {
            // Set focus to the QListView
            listView->setFocus();
            listView->setCurrentIndex(listView->model()->index(0, 0));
            return true;  // Event handled
        }
    }
     else if (obj == listView && event->type() == QEvent::KeyPress)
     {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Enter
                   || keyEvent->key() == Qt::Key_Tab
                   || keyEvent->key() == Qt::Key_Return)
        {
            // Handle Enter key press
            QModelIndex selectedIndex = listView->currentIndex();
            if (selectedIndex.isValid())
            {
                emit signalItemSelected(selectedIndex);
            }
            return true;  // Event handled
        }
     }

    return QObject::eventFilter(obj, event);
}


