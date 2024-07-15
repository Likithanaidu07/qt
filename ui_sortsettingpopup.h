/********************************************************************************
** Form generated from reading UI file 'sortsettingpopup.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_SortSettingPopUp
{
public:
    QPushButton *pushButtonAdd;
    QDialogButtonBox *buttonBox;
    QPushButton *pushButtonRemove;
    QTableWidget *tableWidget;
    QComboBox *comboBoxSortItems;

    void setupUi(QDialog *SortSettingPopUp)
    {
        if (SortSettingPopUp->objectName().isEmpty())
            SortSettingPopUp->setObjectName(QString::fromUtf8("SortSettingPopUp"));
        SortSettingPopUp->setWindowModality(Qt::ApplicationModal);
        SortSettingPopUp->resize(245, 352);
        SortSettingPopUp->setMinimumSize(QSize(245, 352));
        SortSettingPopUp->setMaximumSize(QSize(245, 352));
        pushButtonAdd = new QPushButton(SortSettingPopUp);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
        pushButtonAdd->setGeometry(QRect(100, 10, 50, 24));
        pushButtonAdd->setMinimumSize(QSize(50, 0));
        pushButtonAdd->setMaximumSize(QSize(50, 16777215));
        buttonBox = new QDialogButtonBox(SortSettingPopUp);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(9, 319, 166, 24));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
        pushButtonRemove = new QPushButton(SortSettingPopUp);
        pushButtonRemove->setObjectName(QString::fromUtf8("pushButtonRemove"));
        pushButtonRemove->setGeometry(QRect(152, 9, 71, 24));
        tableWidget = new QTableWidget(SortSettingPopUp);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(9, 39, 221, 248));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setStretchLastSection(false);
        comboBoxSortItems = new QComboBox(SortSettingPopUp);
        comboBoxSortItems->setObjectName(QString::fromUtf8("comboBoxSortItems"));
        comboBoxSortItems->setGeometry(QRect(9, 9, 81, 24));

        retranslateUi(SortSettingPopUp);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, SortSettingPopUp, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, SortSettingPopUp, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(SortSettingPopUp);
    } // setupUi

    void retranslateUi(QDialog *SortSettingPopUp)
    {
        SortSettingPopUp->setWindowTitle(QCoreApplication::translate("SortSettingPopUp", "Algo Sorting", nullptr));
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
