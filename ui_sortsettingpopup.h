/********************************************************************************
** Form generated from reading UI file 'sortsettingpopup.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SORTSETTINGPOPUP_H
#define UI_SORTSETTINGPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_SortSettingPopUp
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButtonAdd;
    QDialogButtonBox *buttonBox;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonRemove;
    QTableWidget *tableWidget;
    QComboBox *comboBoxSortItems;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *SortSettingPopUp)
    {
        if (SortSettingPopUp->objectName().isEmpty())
            SortSettingPopUp->setObjectName("SortSettingPopUp");
        SortSettingPopUp->setWindowModality(Qt::ApplicationModal);
        SortSettingPopUp->resize(245, 352);
        SortSettingPopUp->setMinimumSize(QSize(245, 352));
        SortSettingPopUp->setMaximumSize(QSize(245, 352));
        gridLayout = new QGridLayout(SortSettingPopUp);
        gridLayout->setObjectName("gridLayout");
        pushButtonAdd = new QPushButton(SortSettingPopUp);
        pushButtonAdd->setObjectName("pushButtonAdd");
        pushButtonAdd->setMinimumSize(QSize(50, 0));
        pushButtonAdd->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(pushButtonAdd, 0, 1, 1, 1);

        buttonBox = new QDialogButtonBox(SortSettingPopUp);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 5);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 3, 1, 1);

        pushButtonRemove = new QPushButton(SortSettingPopUp);
        pushButtonRemove->setObjectName("pushButtonRemove");

        gridLayout->addWidget(pushButtonRemove, 0, 2, 1, 1);

        tableWidget = new QTableWidget(SortSettingPopUp);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName("tableWidget");
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setStretchLastSection(false);

        gridLayout->addWidget(tableWidget, 1, 0, 2, 4);

        comboBoxSortItems = new QComboBox(SortSettingPopUp);
        comboBoxSortItems->setObjectName("comboBoxSortItems");

        gridLayout->addWidget(comboBoxSortItems, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);


        retranslateUi(SortSettingPopUp);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SortSettingPopUp, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SortSettingPopUp, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SortSettingPopUp);
    } // setupUi

    void retranslateUi(QDialog *SortSettingPopUp)
    {
        SortSettingPopUp->setWindowTitle(QCoreApplication::translate("SortSettingPopUp", "Algo Sort Settings", nullptr));
        pushButtonAdd->setText(QCoreApplication::translate("SortSettingPopUp", "Add", nullptr));
        pushButtonRemove->setText(QCoreApplication::translate("SortSettingPopUp", "Remove", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("SortSettingPopUp", "Column", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("SortSettingPopUp", "Sort Order", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SortSettingPopUp: public Ui_SortSettingPopUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SORTSETTINGPOPUP_H
