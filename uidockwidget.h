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
    explicit DockWidget(const QString &title, QWidget *parent = nullptr);

protected:

    void mousePressEvent(QMouseEvent *event) override;
};
#endif
