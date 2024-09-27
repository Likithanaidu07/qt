/********************************************************************************
** Form generated from reading UI file 'settings_window.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_WINDOW_H
#define UI_SETTINGS_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings_Window
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabGeneral;
    QWidget *tabHotkeys;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonSaveHotKeys;
    QSpacerItem *horizontalSpacer;
    QTreeView *treeView;

    void setupUi(QDialog *Settings_Window)
    {
        if (Settings_Window->objectName().isEmpty())
            Settings_Window->setObjectName(QString::fromUtf8("Settings_Window"));
        Settings_Window->resize(722, 553);
        gridLayout = new QGridLayout(Settings_Window);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(Settings_Window);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::West);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabWidget->setElideMode(Qt::ElideNone);
        tabGeneral = new QWidget();
        tabGeneral->setObjectName(QString::fromUtf8("tabGeneral"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("folder");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(tabGeneral, icon, QString());
        tabHotkeys = new QWidget();
        tabHotkeys->setObjectName(QString::fromUtf8("tabHotkeys"));
        gridLayout_2 = new QGridLayout(tabHotkeys);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButtonSaveHotKeys = new QPushButton(tabHotkeys);
        pushButtonSaveHotKeys->setObjectName(QString::fromUtf8("pushButtonSaveHotKeys"));

        gridLayout_2->addWidget(pushButtonSaveHotKeys, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        treeView = new QTreeView(tabHotkeys);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout_2->addWidget(treeView, 0, 0, 1, 2);

        QIcon icon1;
        iconThemeName = QString::fromUtf8("applications-accessories");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        tabWidget->addTab(tabHotkeys, icon1, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(Settings_Window);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Settings_Window);
    } // setupUi

    void retranslateUi(QDialog *Settings_Window)
    {
        Settings_Window->setWindowTitle(QCoreApplication::translate("Settings_Window", "Settings", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabGeneral), QCoreApplication::translate("Settings_Window", "General", nullptr));
        pushButtonSaveHotKeys->setText(QCoreApplication::translate("Settings_Window", "Save", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabHotkeys), QCoreApplication::translate("Settings_Window", "Hotkeys", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Settings_Window: public Ui_Settings_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_WINDOW_H
