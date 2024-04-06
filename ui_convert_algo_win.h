/********************************************************************************
** Form generated from reading UI file 'convert_algo_win.ui'
**
** Created by: Qt User Interface Compiler version 6.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONVERT_ALGO_WIN_H
#define UI_CONVERT_ALGO_WIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConvertAlgo_Win
{
public:
    QGridLayout *gridLayout;
    QWidget *ConvertAlgo_TitleBar;
    QGridLayout *gridLayout_4;
    QToolButton *Close;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_Cancel;
    QPushButton *pushButtonUpload;
    QLineEdit *lineEdit_Start_strike;
    QComboBox *comboBox_AlgoType;
    QSpacerItem *horizontalSpacer;
    QListView *listView_startStrike;
    QLineEdit *lineEdit_StrikeDifference;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButtonAdd;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *lineEdit_Fut;
    QSpacerItem *verticalSpacer;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QToolButton *pushButton_Reset;
    QTableWidget *tableWidget;
    QToolButton *pushButtonDelete;
    QToolButton *pushButtonSelectAll;
    QLabel *label;
    QLineEdit *lineEdit_EndStrike;
    QListView *listViewEndStrike;

    void setupUi(QDialog *ConvertAlgo_Win)
    {
        if (ConvertAlgo_Win->objectName().isEmpty())
            ConvertAlgo_Win->setObjectName(QString::fromUtf8("ConvertAlgo_Win"));
        ConvertAlgo_Win->resize(1000, 410);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConvertAlgo_Win->sizePolicy().hasHeightForWidth());
        ConvertAlgo_Win->setSizePolicy(sizePolicy);
        ConvertAlgo_Win->setMinimumSize(QSize(1000, 410));
        ConvertAlgo_Win->setMaximumSize(QSize(1000, 410));
        ConvertAlgo_Win->setStyleSheet(QString::fromUtf8("QDialog{\n"
"box-shadow: 4px 4px 12px 0px rgba(0, 0, 0, 0.25);\n"
"}"));
        gridLayout = new QGridLayout(ConvertAlgo_Win);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        ConvertAlgo_TitleBar = new QWidget(ConvertAlgo_Win);
        ConvertAlgo_TitleBar->setObjectName(QString::fromUtf8("ConvertAlgo_TitleBar"));
        sizePolicy.setHeightForWidth(ConvertAlgo_TitleBar->sizePolicy().hasHeightForWidth());
        ConvertAlgo_TitleBar->setSizePolicy(sizePolicy);
        ConvertAlgo_TitleBar->setMinimumSize(QSize(800, 32));
        ConvertAlgo_TitleBar->setMaximumSize(QSize(1000, 32));
        ConvertAlgo_TitleBar->setStyleSheet(QString::fromUtf8("#ConvertAlgo_TitleBar{\n"
"background: #314458;\n"
"}"));
        gridLayout_4 = new QGridLayout(ConvertAlgo_TitleBar);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(15);
        gridLayout_4->setContentsMargins(-1, 7, -1, 8);
        Close = new QToolButton(ConvertAlgo_TitleBar);
        Close->setObjectName(QString::fromUtf8("Close"));
        Close->setMinimumSize(QSize(14, 14));
        Close->setMaximumSize(QSize(14, 14));
        Close->setStyleSheet(QString::fromUtf8("#Close{\n"
"background: transparent;\n"
"}"));

        gridLayout_4->addWidget(Close, 0, 2, 1, 1);

        label_2 = new QLabel(ConvertAlgo_TitleBar);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 17));
        label_2->setMaximumSize(QSize(108, 17));
        label_2->setStyleSheet(QString::fromUtf8("#label_2{\n"
"color: #F8F9FA;\n"
"font-family: Inter;\n"
"font-size: 14px;\n"
"font-style: normal;\n"
"font-weight: 700;\n"
"line-height: normal;\n"
"}"));

        gridLayout_4->addWidget(label_2, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 1, 1, 1);


        gridLayout->addWidget(ConvertAlgo_TitleBar, 0, 0, 1, 1);

        widget_2 = new QWidget(ConvertAlgo_Win);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setStyleSheet(QString::fromUtf8("#widget_2{\n"
"background: #F2F5F8;\n"
"}"));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(9);
        gridLayout_2->setVerticalSpacing(0);
        gridLayout_2->setContentsMargins(12, 0, 12, 14);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(17);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_Cancel = new QPushButton(widget_2);
        pushButton_Cancel->setObjectName(QString::fromUtf8("pushButton_Cancel"));
        pushButton_Cancel->setMinimumSize(QSize(85, 32));
        pushButton_Cancel->setMaximumSize(QSize(95, 16777215));
        pushButton_Cancel->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border-radius: 7px;\n"
"background: #1585C0;\n"
"color: #FFF;\n"
"box-shadow: 0px 2px 4px 0px rgba(0, 0, 0, 0.25);\n"
"font-family: Work Sans;\n"
"font-size: 14px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_2->addWidget(pushButton_Cancel);

        pushButtonUpload = new QPushButton(widget_2);
        pushButtonUpload->setObjectName(QString::fromUtf8("pushButtonUpload"));
        sizePolicy.setHeightForWidth(pushButtonUpload->sizePolicy().hasHeightForWidth());
        pushButtonUpload->setSizePolicy(sizePolicy);
        pushButtonUpload->setMinimumSize(QSize(95, 32));
        pushButtonUpload->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border-radius: 7px;\n"
"background: #1585C0;\n"
"box-shadow: 0px 2px 4px 0px rgba(0, 0, 0, 0.75);\n"
"color: #FFF;\n"
"font-family: Work Sans;\n"
"font-size: 14px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_2->addWidget(pushButtonUpload);


        gridLayout_2->addLayout(horizontalLayout_2, 6, 0, 1, 10);

        lineEdit_Start_strike = new QLineEdit(widget_2);
        lineEdit_Start_strike->setObjectName(QString::fromUtf8("lineEdit_Start_strike"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_Start_strike->sizePolicy().hasHeightForWidth());
        lineEdit_Start_strike->setSizePolicy(sizePolicy1);
        lineEdit_Start_strike->setMinimumSize(QSize(0, 28));
        lineEdit_Start_strike->setMaximumSize(QSize(16777215, 28));
        lineEdit_Start_strike->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(lineEdit_Start_strike, 1, 4, 1, 1);

        comboBox_AlgoType = new QComboBox(widget_2);
        comboBox_AlgoType->setObjectName(QString::fromUtf8("comboBox_AlgoType"));
        comboBox_AlgoType->setMinimumSize(QSize(85, 28));
        comboBox_AlgoType->setMaximumSize(QSize(16777215, 28));
        comboBox_AlgoType->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"border-radius: 6px;\n"
"border: 0.5px solid #343A40;\n"
"background: #FFF;\n"
" text-align: center;\n"
"}\n"
"\n"
"QComboBox::drop-down{\n"
"border-radius: 6px;\n"
"background: #FFF;\n"
" text-align: center;\n"
"\n"
"}\n"
""));

        gridLayout_2->addWidget(comboBox_AlgoType, 1, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(15, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 8, 1, 1);

        listView_startStrike = new QListView(widget_2);
        listView_startStrike->setObjectName(QString::fromUtf8("listView_startStrike"));
        listView_startStrike->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_2->addWidget(listView_startStrike, 2, 4, 1, 1);

        lineEdit_StrikeDifference = new QLineEdit(widget_2);
        lineEdit_StrikeDifference->setObjectName(QString::fromUtf8("lineEdit_StrikeDifference"));
        sizePolicy1.setHeightForWidth(lineEdit_StrikeDifference->sizePolicy().hasHeightForWidth());
        lineEdit_StrikeDifference->setSizePolicy(sizePolicy1);
        lineEdit_StrikeDifference->setMinimumSize(QSize(0, 28));
        lineEdit_StrikeDifference->setMaximumSize(QSize(16777215, 28));
        lineEdit_StrikeDifference->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(lineEdit_StrikeDifference, 1, 6, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_3, 5, 0, 1, 10);

        pushButtonAdd = new QPushButton(widget_2);
        pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
        sizePolicy.setHeightForWidth(pushButtonAdd->sizePolicy().hasHeightForWidth());
        pushButtonAdd->setSizePolicy(sizePolicy);
        pushButtonAdd->setMinimumSize(QSize(90, 28));
        pushButtonAdd->setMaximumSize(QSize(90, 28));
        pushButtonAdd->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(pushButtonAdd, 1, 9, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_2, 3, 0, 1, 10);

        lineEdit_Fut = new QLineEdit(widget_2);
        lineEdit_Fut->setObjectName(QString::fromUtf8("lineEdit_Fut"));
        sizePolicy1.setHeightForWidth(lineEdit_Fut->sizePolicy().hasHeightForWidth());
        lineEdit_Fut->setSizePolicy(sizePolicy1);
        lineEdit_Fut->setMinimumSize(QSize(0, 28));

        gridLayout_2->addWidget(lineEdit_Fut, 1, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 0, 0, 1, 10);

        widget = new QWidget(widget_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setStyleSheet(QString::fromUtf8("#widget{\n"
"border-radius: 8px;\n"
"background: #E0E8EE;\n"
"}"));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(12, 8, 12, 12);
        pushButton_Reset = new QToolButton(widget);
        pushButton_Reset->setObjectName(QString::fromUtf8("pushButton_Reset"));
        pushButton_Reset->setMaximumSize(QSize(58, 16));
        pushButton_Reset->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(pushButton_Reset, 0, 2, 1, 1);

        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setFocusPolicy(Qt::NoFocus);
        tableWidget->setStyleSheet(QString::fromUtf8("QTableView::item {\n"
"    background-color: #FFF;\n"
"    border: none; \n"
"    color: #495057;\n"
"    font-family: Work Sans;\n"
"    font-size: 8px;\n"
"    font-style: normal;\n"
"    font-weight: 600;\n"
"    line-height: normal;\n"
"}\n"
"\n"
"QTableView::item:alternate {\n"
"    border: none; \n"
"    background-color: #F5FAFD; \n"
"}\n"
"\n"
"QTableView::item:selected {\n"
"    background-color: #42A5F5;\n"
"    color: white; /* Optionally, set the text color for selected items */\n"
"}\n"
""));

        gridLayout_3->addWidget(tableWidget, 1, 0, 1, 4);

        pushButtonDelete = new QToolButton(widget);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));
        pushButtonDelete->setMaximumSize(QSize(58, 16));
        pushButtonDelete->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(pushButtonDelete, 0, 3, 1, 1);

        pushButtonSelectAll = new QToolButton(widget);
        pushButtonSelectAll->setObjectName(QString::fromUtf8("pushButtonSelectAll"));
        pushButtonSelectAll->setMaximumSize(QSize(58, 16));
        pushButtonSelectAll->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(pushButtonSelectAll, 0, 1, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);


        gridLayout_2->addWidget(widget, 4, 0, 1, 10);

        lineEdit_EndStrike = new QLineEdit(widget_2);
        lineEdit_EndStrike->setObjectName(QString::fromUtf8("lineEdit_EndStrike"));
        sizePolicy1.setHeightForWidth(lineEdit_EndStrike->sizePolicy().hasHeightForWidth());
        lineEdit_EndStrike->setSizePolicy(sizePolicy1);
        lineEdit_EndStrike->setMinimumSize(QSize(0, 28));
        lineEdit_EndStrike->setMaximumSize(QSize(16777215, 28));
        lineEdit_EndStrike->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(lineEdit_EndStrike, 1, 5, 1, 1);

        listViewEndStrike = new QListView(widget_2);
        listViewEndStrike->setObjectName(QString::fromUtf8("listViewEndStrike"));
        listViewEndStrike->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_2->addWidget(listViewEndStrike, 2, 5, 1, 1);


        gridLayout->addWidget(widget_2, 1, 0, 1, 1);

        QWidget::setTabOrder(comboBox_AlgoType, lineEdit_Fut);
        QWidget::setTabOrder(lineEdit_Fut, lineEdit_Start_strike);
        QWidget::setTabOrder(lineEdit_Start_strike, lineEdit_EndStrike);
        QWidget::setTabOrder(lineEdit_EndStrike, lineEdit_StrikeDifference);
        QWidget::setTabOrder(lineEdit_StrikeDifference, pushButtonAdd);
        QWidget::setTabOrder(pushButtonAdd, pushButton_Cancel);
        QWidget::setTabOrder(pushButton_Cancel, pushButtonUpload);
        QWidget::setTabOrder(pushButtonUpload, pushButtonSelectAll);
        QWidget::setTabOrder(pushButtonSelectAll, pushButton_Reset);
        QWidget::setTabOrder(pushButton_Reset, pushButtonDelete);
        QWidget::setTabOrder(pushButtonDelete, Close);
        QWidget::setTabOrder(Close, tableWidget);

        retranslateUi(ConvertAlgo_Win);

        QMetaObject::connectSlotsByName(ConvertAlgo_Win);
    } // setupUi

    void retranslateUi(QDialog *ConvertAlgo_Win)
    {
        ConvertAlgo_Win->setWindowTitle(QCoreApplication::translate("ConvertAlgo_Win", "Dialog", nullptr));
        Close->setText(QString());
        label_2->setText(QCoreApplication::translate("ConvertAlgo_Win", "Add Algo", nullptr));
        pushButton_Cancel->setText(QCoreApplication::translate("ConvertAlgo_Win", "Cancel", nullptr));
        pushButtonUpload->setText(QCoreApplication::translate("ConvertAlgo_Win", "Add", nullptr));
        lineEdit_Start_strike->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Start Instrument", nullptr));
        lineEdit_StrikeDifference->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Strike Difference", nullptr));
        pushButtonAdd->setText(QCoreApplication::translate("ConvertAlgo_Win", "Create", nullptr));
        lineEdit_Fut->setText(QString());
        lineEdit_Fut->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "FUT Instrument", nullptr));
        pushButton_Reset->setText(QCoreApplication::translate("ConvertAlgo_Win", "Reset", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("ConvertAlgo_Win", "Delete", nullptr));
        pushButtonSelectAll->setText(QCoreApplication::translate("ConvertAlgo_Win", "Select All", nullptr));
        label->setText(QCoreApplication::translate("ConvertAlgo_Win", "Algo List", nullptr));
        lineEdit_EndStrike->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "End Instrument", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConvertAlgo_Win: public Ui_ConvertAlgo_Win {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERT_ALGO_WIN_H
