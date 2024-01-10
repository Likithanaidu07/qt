#ifndef UIDOCKWIDGET_H
#define UIDOCKWIDGET_H

#include <QDockWidget>
#include <QMouseEvent>
#include <QDrag>
//#include "UiUtils.h"

//class UiDockWidget : public QDockWidget
//{
//    Q_OBJECT
//public:
//    UiDockWidget(UiUtils::DockWidgetType p_Type, const QString& p_Title, QWidget* p_Parent = nullptr);

//signals:
//    void WidgetClosed(UiUtils::DockWidgetType p_Type);

//protected:
//    virtual void closeEvent(QCloseEvent* p_Event);

//public:
//    UiUtils::DockWidgetType m_WidgetType;
//};

//#endif // UIDOCKWIDGET_H
class DockWidget : public QDockWidget {
public:
    DockWidget(const QString &title, QWidget *parent = nullptr)
        : QDockWidget(title, parent) {
        setAcceptDrops(true);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
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
};
#endif
