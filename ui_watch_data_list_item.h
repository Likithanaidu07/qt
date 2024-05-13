/********************************************************************************
** Form generated from reading UI file 'watch_data_list_item.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCH_DATA_LIST_ITEM_H
#define UI_WATCH_DATA_LIST_ITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_watch_Data_List_Item
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *name;
    QSpacerItem *horizontalSpacer;
    QLabel *value1;
    QLabel *indicator;
    QLabel *value2;

    void setupUi(QWidget *watch_Data_List_Item)
    {
        if (watch_Data_List_Item->objectName().isEmpty())
            watch_Data_List_Item->setObjectName(QString::fromUtf8("watch_Data_List_Item"));
        watch_Data_List_Item->resize(310, 53);
        horizontalLayout = new QHBoxLayout(watch_Data_List_Item);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        name = new QLabel(watch_Data_List_Item);
        name->setObjectName(QString::fromUtf8("name"));
        QFont font;
        font.setPointSize(9);
        font.setBold(false);
        name->setFont(font);

        horizontalLayout->addWidget(name);

        horizontalSpacer = new QSpacerItem(22, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        value1 = new QLabel(watch_Data_List_Item);
        value1->setObjectName(QString::fromUtf8("value1"));
        value1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(value1);

        indicator = new QLabel(watch_Data_List_Item);
        indicator->setObjectName(QString::fromUtf8("indicator"));
        indicator->setMinimumSize(QSize(10, 6));
        indicator->setMaximumSize(QSize(10, 6));

        horizontalLayout->addWidget(indicator);

        value2 = new QLabel(watch_Data_List_Item);
        value2->setObjectName(QString::fromUtf8("value2"));

        horizontalLayout->addWidget(value2);


        retranslateUi(watch_Data_List_Item);

        QMetaObject::connectSlotsByName(watch_Data_List_Item);
    } // setupUi

    void retranslateUi(QWidget *watch_Data_List_Item)
    {
        watch_Data_List_Item->setWindowTitle(QCoreApplication::translate("watch_Data_List_Item", "Form", nullptr));
        name->setText(QCoreApplication::translate("watch_Data_List_Item", "Nifty Fifty", nullptr));
        value1->setText(QCoreApplication::translate("watch_Data_List_Item", "1936.75", nullptr));
        indicator->setText(QCoreApplication::translate("watch_Data_List_Item", "^", nullptr));
        value2->setText(QCoreApplication::translate("watch_Data_List_Item", "9.50 (0.05%)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class watch_Data_List_Item: public Ui_watch_Data_List_Item {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCH_DATA_LIST_ITEM_H
