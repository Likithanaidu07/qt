#pragma once

class MainWindow;

class UiUtils
{
public:

    enum DockWidgetType
    {
        DOCK_WIDGET_ALGORITHM,
        DOCK_WIDGET_ORDER_BOOK,
        DOCK_WIDGET_POSITIONS,
        DOCK_WIDGET_LINERS
    };

    static MainWindow* GetMainWindow()
    {
        return m_MainWindow;
    }
    static void SetMainWindow(MainWindow* p_MainWindow)
    {
        m_MainWindow = p_MainWindow;
    }

private:
    static MainWindow *m_MainWindow;
};
