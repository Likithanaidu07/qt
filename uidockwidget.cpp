#include "uidockwidget.h"

DockWidget::DockWidget(const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    setAcceptDrops(true);
}

void DockWidget::mousePressEvent(QMouseEvent *event)  {
    if (event->button() == Qt::LeftButton) {
        // Create a pixmap of the dock widget's content
        QPixmap pixmap(size());
        render(&pixmap);

        // Create a drag object and set the pixmap as its visual representation
        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);

        // Start the drag operation
        drag->exec(Qt::MoveAction);
    }
    QDockWidget::mousePressEvent(event);
}
