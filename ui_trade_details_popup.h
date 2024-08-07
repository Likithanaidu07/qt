/********************************************************************************
** Form generated from reading UI file 'trade_details_popup.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRADE_DETAILS_POPUP_H
#define UI_TRADE_DETAILS_POPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_TradeDetailsPopup
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;

    void setupUi(QDialog *TradeDetailsPopup)
    {
        if (TradeDetailsPopup->objectName().isEmpty())
            TradeDetailsPopup->setObjectName(QString::fromUtf8("TradeDetailsPopup"));
        TradeDetailsPopup->setWindowModality(Qt::ApplicationModal);
        TradeDetailsPopup->resize(727, 179);
        gridLayout = new QGridLayout(TradeDetailsPopup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(TradeDetailsPopup);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setStretchLastSection(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        retranslateUi(TradeDetailsPopup);

        QMetaObject::connectSlotsByName(TradeDetailsPopup);
    } // setupUi

    void retranslateUi(QDialog *TradeDetailsPopup)
    {
        TradeDetailsPopup->setWindowTitle(QCoreApplication::translate("TradeDetailsPopup", "Trade Details", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("TradeDetailsPopup", "Sl No", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("TradeDetailsPopup", "OrderId", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("TradeDetailsPopup", "AlgoNo", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("TradeDetailsPopup", "Stockname", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("TradeDetailsPopup", "Price", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("TradeDetailsPopup", "Lots", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("TradeDetailsPopup", "Time", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TradeDetailsPopup: public Ui_TradeDetailsPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRADE_DETAILS_POPUP_H
