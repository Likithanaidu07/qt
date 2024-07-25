/********************************************************************************
** Form generated from reading UI file 'convert_algo_win.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConvertAlgo_Win
{
public:
    QGridLayout *gridLayout;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButtonAdd;
    QSpacerItem *verticalSpacer_3;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QPushButton *pushButtonDelete;
    QTableWidget *tableWidget;
    QLabel *label;
    QPushButton *pushButton_Reset;
    QPushButton *pushButtonSelectAll;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButtonUpload;
    QPushButton *pushButton_Cancel;
    QSpacerItem *verticalSpacer;
    QComboBox *comboBox_AlgoType;
    QStackedWidget *stackedWidget;
    QWidget *pageBtfyBid;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_Start_strike_BtfyBid;
    QLineEdit *lineEdit_EndStrike_BtfyBid;
    QLineEdit *lineEdit_StrikeDifference_BtfyBid;
    QWidget *page_f2f;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_Start_strike_f2f;
    QLineEdit *lineEdit_EndStrike_f2f;
    QWidget *pageBtfy;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_Start_strike_Btfy;
    QLineEdit *lineEdit_EndStrike_Btfy;
    QLineEdit *lineEdit_StrikeDifference_Btfy;
    QWidget *pageConvRev;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *lineEdit_Fut_ConvRev;
    QLineEdit *lineEdit_Start_strike_ConvRev;
    QLineEdit *lineEdit_EndStrike_ConvRev;
    QWidget *ConvertAlgo_TitleBar;
    QGridLayout *gridLayout_4;
    QToolButton *Close;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *ConvertAlgo_Win)
    {
        if (ConvertAlgo_Win->objectName().isEmpty())
            ConvertAlgo_Win->setObjectName("ConvertAlgo_Win");
        ConvertAlgo_Win->resize(1000, 410);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConvertAlgo_Win->sizePolicy().hasHeightForWidth());
        ConvertAlgo_Win->setSizePolicy(sizePolicy);
        ConvertAlgo_Win->setMinimumSize(QSize(1000, 410));
        ConvertAlgo_Win->setMaximumSize(QSize(1000, 410));
        ConvertAlgo_Win->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(ConvertAlgo_Win);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(ConvertAlgo_Win);
        widget_2->setObjectName("widget_2");
        widget_2->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setHorizontalSpacing(9);
        gridLayout_2->setVerticalSpacing(0);
        gridLayout_2->setContentsMargins(12, 0, 12, 14);
        verticalSpacer_2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_2, 3, 0, 1, 10);

        pushButtonAdd = new QPushButton(widget_2);
        pushButtonAdd->setObjectName("pushButtonAdd");
        sizePolicy.setHeightForWidth(pushButtonAdd->sizePolicy().hasHeightForWidth());
        pushButtonAdd->setSizePolicy(sizePolicy);
        pushButtonAdd->setMinimumSize(QSize(90, 28));
        pushButtonAdd->setMaximumSize(QSize(90, 28));
        pushButtonAdd->setStyleSheet(QString::fromUtf8(""));
        pushButtonAdd->setFlat(true);

        gridLayout_2->addWidget(pushButtonAdd, 1, 8, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer_3, 5, 0, 1, 10);

        widget = new QWidget(widget_2);
        widget->setObjectName("widget");
        widget->setFocusPolicy(Qt::NoFocus);
        widget->setStyleSheet(QString::fromUtf8("#widget{\n"
"border-radius: 8px;\n"
"background: #E0E8EE;\n"
"}"));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName("gridLayout_3");
        pushButtonDelete = new QPushButton(widget);
        pushButtonDelete->setObjectName("pushButtonDelete");
        pushButtonDelete->setMaximumSize(QSize(58, 16));
        pushButtonDelete->setStyleSheet(QString::fromUtf8(""));
        pushButtonDelete->setFlat(true);

        gridLayout_3->addWidget(pushButtonDelete, 0, 3, 1, 1);

        tableWidget = new QTableWidget(widget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setFocusPolicy(Qt::TabFocus);
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

        gridLayout_3->addWidget(tableWidget, 3, 0, 1, 4);

        label = new QLabel(widget);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        pushButton_Reset = new QPushButton(widget);
        pushButton_Reset->setObjectName("pushButton_Reset");
        pushButton_Reset->setMaximumSize(QSize(58, 16));
        pushButton_Reset->setStyleSheet(QString::fromUtf8(""));
        pushButton_Reset->setFlat(true);

        gridLayout_3->addWidget(pushButton_Reset, 0, 2, 1, 1);

        pushButtonSelectAll = new QPushButton(widget);
        pushButtonSelectAll->setObjectName("pushButtonSelectAll");
        pushButtonSelectAll->setMaximumSize(QSize(58, 16));
        pushButtonSelectAll->setFocusPolicy(Qt::TabFocus);
        pushButtonSelectAll->setStyleSheet(QString::fromUtf8(""));
        pushButtonSelectAll->setFlat(true);

        gridLayout_3->addWidget(pushButtonSelectAll, 0, 1, 1, 1);


        gridLayout_2->addWidget(widget, 4, 0, 1, 10);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(17);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButtonUpload = new QPushButton(widget_2);
        pushButtonUpload->setObjectName("pushButtonUpload");
        sizePolicy.setHeightForWidth(pushButtonUpload->sizePolicy().hasHeightForWidth());
        pushButtonUpload->setSizePolicy(sizePolicy);
        pushButtonUpload->setMinimumSize(QSize(95, 32));
        pushButtonUpload->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border-radius: 7px;\n"
"background: #1585C0;\n"
"color: #FFF;\n"
"font-family: Work Sans;\n"
"font-size: 14px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));
        pushButtonUpload->setFlat(true);

        horizontalLayout_2->addWidget(pushButtonUpload);

        pushButton_Cancel = new QPushButton(widget_2);
        pushButton_Cancel->setObjectName("pushButton_Cancel");
        pushButton_Cancel->setMinimumSize(QSize(85, 32));
        pushButton_Cancel->setMaximumSize(QSize(95, 16777215));
        pushButton_Cancel->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"border-radius: 7px;\n"
"background: #1585C0;\n"
"color: #FFF;\n"
"font-family: Work Sans;\n"
"font-size: 14px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));
        pushButton_Cancel->setFlat(true);

        horizontalLayout_2->addWidget(pushButton_Cancel);


        gridLayout_2->addLayout(horizontalLayout_2, 6, 0, 1, 10);

        verticalSpacer = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_2->addItem(verticalSpacer, 0, 0, 1, 10);

        comboBox_AlgoType = new QComboBox(widget_2);
        comboBox_AlgoType->setObjectName("comboBox_AlgoType");
        comboBox_AlgoType->setMinimumSize(QSize(92, 28));
        comboBox_AlgoType->setMaximumSize(QSize(90, 28));
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

        stackedWidget = new QStackedWidget(widget_2);
        stackedWidget->setObjectName("stackedWidget");
        pageBtfyBid = new QWidget();
        pageBtfyBid->setObjectName("pageBtfyBid");
        horizontalLayout = new QHBoxLayout(pageBtfyBid);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(9, 2, -1, 2);
        lineEdit_Start_strike_BtfyBid = new QLineEdit(pageBtfyBid);
        lineEdit_Start_strike_BtfyBid->setObjectName("lineEdit_Start_strike_BtfyBid");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_Start_strike_BtfyBid->sizePolicy().hasHeightForWidth());
        lineEdit_Start_strike_BtfyBid->setSizePolicy(sizePolicy1);
        lineEdit_Start_strike_BtfyBid->setMinimumSize(QSize(0, 28));
        lineEdit_Start_strike_BtfyBid->setMaximumSize(QSize(16777215, 28));
        lineEdit_Start_strike_BtfyBid->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(lineEdit_Start_strike_BtfyBid);

        lineEdit_EndStrike_BtfyBid = new QLineEdit(pageBtfyBid);
        lineEdit_EndStrike_BtfyBid->setObjectName("lineEdit_EndStrike_BtfyBid");
        sizePolicy1.setHeightForWidth(lineEdit_EndStrike_BtfyBid->sizePolicy().hasHeightForWidth());
        lineEdit_EndStrike_BtfyBid->setSizePolicy(sizePolicy1);
        lineEdit_EndStrike_BtfyBid->setMinimumSize(QSize(0, 28));
        lineEdit_EndStrike_BtfyBid->setMaximumSize(QSize(16777215, 28));
        lineEdit_EndStrike_BtfyBid->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(lineEdit_EndStrike_BtfyBid);

        lineEdit_StrikeDifference_BtfyBid = new QLineEdit(pageBtfyBid);
        lineEdit_StrikeDifference_BtfyBid->setObjectName("lineEdit_StrikeDifference_BtfyBid");
        sizePolicy1.setHeightForWidth(lineEdit_StrikeDifference_BtfyBid->sizePolicy().hasHeightForWidth());
        lineEdit_StrikeDifference_BtfyBid->setSizePolicy(sizePolicy1);
        lineEdit_StrikeDifference_BtfyBid->setMinimumSize(QSize(90, 28));
        lineEdit_StrikeDifference_BtfyBid->setMaximumSize(QSize(90, 28));
        lineEdit_StrikeDifference_BtfyBid->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(lineEdit_StrikeDifference_BtfyBid);

        stackedWidget->addWidget(pageBtfyBid);
        page_f2f = new QWidget();
        page_f2f->setObjectName("page_f2f");
        horizontalLayout_3 = new QHBoxLayout(page_f2f);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        lineEdit_Start_strike_f2f = new QLineEdit(page_f2f);
        lineEdit_Start_strike_f2f->setObjectName("lineEdit_Start_strike_f2f");
        sizePolicy1.setHeightForWidth(lineEdit_Start_strike_f2f->sizePolicy().hasHeightForWidth());
        lineEdit_Start_strike_f2f->setSizePolicy(sizePolicy1);
        lineEdit_Start_strike_f2f->setMinimumSize(QSize(0, 28));
        lineEdit_Start_strike_f2f->setMaximumSize(QSize(16777215, 28));
        lineEdit_Start_strike_f2f->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(lineEdit_Start_strike_f2f);

        lineEdit_EndStrike_f2f = new QLineEdit(page_f2f);
        lineEdit_EndStrike_f2f->setObjectName("lineEdit_EndStrike_f2f");
        sizePolicy1.setHeightForWidth(lineEdit_EndStrike_f2f->sizePolicy().hasHeightForWidth());
        lineEdit_EndStrike_f2f->setSizePolicy(sizePolicy1);
        lineEdit_EndStrike_f2f->setMinimumSize(QSize(0, 28));
        lineEdit_EndStrike_f2f->setMaximumSize(QSize(16777215, 28));
        lineEdit_EndStrike_f2f->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(lineEdit_EndStrike_f2f);

        stackedWidget->addWidget(page_f2f);
        pageBtfy = new QWidget();
        pageBtfy->setObjectName("pageBtfy");
        horizontalLayout_4 = new QHBoxLayout(pageBtfy);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        lineEdit_Start_strike_Btfy = new QLineEdit(pageBtfy);
        lineEdit_Start_strike_Btfy->setObjectName("lineEdit_Start_strike_Btfy");
        sizePolicy1.setHeightForWidth(lineEdit_Start_strike_Btfy->sizePolicy().hasHeightForWidth());
        lineEdit_Start_strike_Btfy->setSizePolicy(sizePolicy1);
        lineEdit_Start_strike_Btfy->setMinimumSize(QSize(0, 28));
        lineEdit_Start_strike_Btfy->setMaximumSize(QSize(16777215, 28));
        lineEdit_Start_strike_Btfy->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(lineEdit_Start_strike_Btfy);

        lineEdit_EndStrike_Btfy = new QLineEdit(pageBtfy);
        lineEdit_EndStrike_Btfy->setObjectName("lineEdit_EndStrike_Btfy");
        sizePolicy1.setHeightForWidth(lineEdit_EndStrike_Btfy->sizePolicy().hasHeightForWidth());
        lineEdit_EndStrike_Btfy->setSizePolicy(sizePolicy1);
        lineEdit_EndStrike_Btfy->setMinimumSize(QSize(0, 28));
        lineEdit_EndStrike_Btfy->setMaximumSize(QSize(16777215, 28));
        lineEdit_EndStrike_Btfy->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(lineEdit_EndStrike_Btfy);

        lineEdit_StrikeDifference_Btfy = new QLineEdit(pageBtfy);
        lineEdit_StrikeDifference_Btfy->setObjectName("lineEdit_StrikeDifference_Btfy");
        sizePolicy1.setHeightForWidth(lineEdit_StrikeDifference_Btfy->sizePolicy().hasHeightForWidth());
        lineEdit_StrikeDifference_Btfy->setSizePolicy(sizePolicy1);
        lineEdit_StrikeDifference_Btfy->setMinimumSize(QSize(90, 28));
        lineEdit_StrikeDifference_Btfy->setMaximumSize(QSize(90, 28));
        lineEdit_StrikeDifference_Btfy->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(lineEdit_StrikeDifference_Btfy);

        stackedWidget->addWidget(pageBtfy);
        pageConvRev = new QWidget();
        pageConvRev->setObjectName("pageConvRev");
        horizontalLayout_5 = new QHBoxLayout(pageConvRev);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        lineEdit_Fut_ConvRev = new QLineEdit(pageConvRev);
        lineEdit_Fut_ConvRev->setObjectName("lineEdit_Fut_ConvRev");
        sizePolicy1.setHeightForWidth(lineEdit_Fut_ConvRev->sizePolicy().hasHeightForWidth());
        lineEdit_Fut_ConvRev->setSizePolicy(sizePolicy1);
        lineEdit_Fut_ConvRev->setMinimumSize(QSize(0, 28));

        horizontalLayout_5->addWidget(lineEdit_Fut_ConvRev);

        lineEdit_Start_strike_ConvRev = new QLineEdit(pageConvRev);
        lineEdit_Start_strike_ConvRev->setObjectName("lineEdit_Start_strike_ConvRev");
        sizePolicy1.setHeightForWidth(lineEdit_Start_strike_ConvRev->sizePolicy().hasHeightForWidth());
        lineEdit_Start_strike_ConvRev->setSizePolicy(sizePolicy1);
        lineEdit_Start_strike_ConvRev->setMinimumSize(QSize(0, 28));
        lineEdit_Start_strike_ConvRev->setMaximumSize(QSize(16777215, 28));
        lineEdit_Start_strike_ConvRev->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(lineEdit_Start_strike_ConvRev);

        lineEdit_EndStrike_ConvRev = new QLineEdit(pageConvRev);
        lineEdit_EndStrike_ConvRev->setObjectName("lineEdit_EndStrike_ConvRev");
        sizePolicy1.setHeightForWidth(lineEdit_EndStrike_ConvRev->sizePolicy().hasHeightForWidth());
        lineEdit_EndStrike_ConvRev->setSizePolicy(sizePolicy1);
        lineEdit_EndStrike_ConvRev->setMinimumSize(QSize(0, 28));
        lineEdit_EndStrike_ConvRev->setMaximumSize(QSize(16777215, 28));
        lineEdit_EndStrike_ConvRev->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_5->addWidget(lineEdit_EndStrike_ConvRev);

        stackedWidget->addWidget(pageConvRev);

        gridLayout_2->addWidget(stackedWidget, 1, 3, 1, 1);


        gridLayout->addWidget(widget_2, 1, 0, 1, 1);

        ConvertAlgo_TitleBar = new QWidget(ConvertAlgo_Win);
        ConvertAlgo_TitleBar->setObjectName("ConvertAlgo_TitleBar");
        sizePolicy.setHeightForWidth(ConvertAlgo_TitleBar->sizePolicy().hasHeightForWidth());
        ConvertAlgo_TitleBar->setSizePolicy(sizePolicy);
        ConvertAlgo_TitleBar->setMinimumSize(QSize(800, 32));
        ConvertAlgo_TitleBar->setMaximumSize(QSize(1000, 32));
        ConvertAlgo_TitleBar->setStyleSheet(QString::fromUtf8("#ConvertAlgo_TitleBar{\n"
"background: #314458;\n"
"}"));
        gridLayout_4 = new QGridLayout(ConvertAlgo_TitleBar);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setHorizontalSpacing(15);
        gridLayout_4->setContentsMargins(-1, 7, -1, 8);
        Close = new QToolButton(ConvertAlgo_TitleBar);
        Close->setObjectName("Close");
        Close->setMinimumSize(QSize(14, 14));
        Close->setMaximumSize(QSize(14, 14));
        Close->setFocusPolicy(Qt::NoFocus);
        Close->setStyleSheet(QString::fromUtf8("#Close{\n"
"background: transparent;\n"
"}"));

        gridLayout_4->addWidget(Close, 0, 2, 1, 1);

        label_2 = new QLabel(ConvertAlgo_TitleBar);
        label_2->setObjectName("label_2");
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

        QWidget::setTabOrder(comboBox_AlgoType, pushButtonAdd);
        QWidget::setTabOrder(pushButtonAdd, pushButtonSelectAll);
        QWidget::setTabOrder(pushButtonSelectAll, pushButton_Reset);
        QWidget::setTabOrder(pushButton_Reset, pushButtonDelete);
        QWidget::setTabOrder(pushButtonDelete, tableWidget);
        QWidget::setTabOrder(tableWidget, pushButtonUpload);
        QWidget::setTabOrder(pushButtonUpload, pushButton_Cancel);

        retranslateUi(ConvertAlgo_Win);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(ConvertAlgo_Win);
    } // setupUi

    void retranslateUi(QDialog *ConvertAlgo_Win)
    {
        ConvertAlgo_Win->setWindowTitle(QCoreApplication::translate("ConvertAlgo_Win", "Dialog", nullptr));
        pushButtonAdd->setText(QCoreApplication::translate("ConvertAlgo_Win", "Create Algo", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("ConvertAlgo_Win", "Delete", nullptr));
        label->setText(QCoreApplication::translate("ConvertAlgo_Win", "Algo List", nullptr));
        pushButton_Reset->setText(QCoreApplication::translate("ConvertAlgo_Win", "Reset", nullptr));
        pushButtonSelectAll->setText(QCoreApplication::translate("ConvertAlgo_Win", "Select All", nullptr));
        pushButtonUpload->setText(QCoreApplication::translate("ConvertAlgo_Win", "Add", nullptr));
        pushButton_Cancel->setText(QCoreApplication::translate("ConvertAlgo_Win", "Cancel", nullptr));
        lineEdit_Start_strike_BtfyBid->setText(QString());
        lineEdit_Start_strike_BtfyBid->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Start Strike (min 3 letters)", nullptr));
        lineEdit_EndStrike_BtfyBid->setText(QString());
        lineEdit_EndStrike_BtfyBid->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "End Strike (min 3 letters)", nullptr));
        lineEdit_StrikeDifference_BtfyBid->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Strike Difference", nullptr));
        lineEdit_Start_strike_f2f->setText(QString());
        lineEdit_Start_strike_f2f->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Start Strike (min 3 letters)", nullptr));
        lineEdit_EndStrike_f2f->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "End Strike (min 3 letters)", nullptr));
        lineEdit_Start_strike_Btfy->setText(QString());
        lineEdit_Start_strike_Btfy->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Start Strike (min 3 letters)", nullptr));
        lineEdit_EndStrike_Btfy->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "End Strike (min 3 letters)", nullptr));
        lineEdit_StrikeDifference_Btfy->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Strike Difference", nullptr));
        lineEdit_Fut_ConvRev->setText(QString());
        lineEdit_Fut_ConvRev->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "FUT Instrument", nullptr));
        lineEdit_Start_strike_ConvRev->setText(QString());
        lineEdit_Start_strike_ConvRev->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "Start Strike (min 3 letters)", nullptr));
        lineEdit_EndStrike_ConvRev->setPlaceholderText(QCoreApplication::translate("ConvertAlgo_Win", "End Strike (min 3 letters)", nullptr));
        Close->setText(QString());
        label_2->setText(QCoreApplication::translate("ConvertAlgo_Win", "Add Algo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConvertAlgo_Win: public Ui_ConvertAlgo_Win {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONVERT_ALGO_WIN_H
