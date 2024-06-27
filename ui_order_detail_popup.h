/********************************************************************************
** Form generated from reading UI file 'order_detail_popup.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORDER_DETAIL_POPUP_H
#define UI_ORDER_DETAIL_POPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OrderDetail_Popup
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QWidget *widget_3;
    QGridLayout *gridLayout_3;
    QLabel *label_27;
    QLabel *label_18;
    QLabel *label_28;
    QLabel *label_25;
    QLabel *label_16;
    QLabel *label_26;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_15;
    QLabel *label_14;
    QLabel *label_29;
    QLabel *label_30;
    QLabel *label_31;
    QLabel *label_32;
    QLabel *label_17;
    QTableWidget *tableWidget_Buy;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_9;
    QLabel *label_21;
    QLabel *label_4;
    QLabel *label_20;
    QLabel *label_7;
    QLabel *label_22;
    QLabel *label_5;
    QLabel *label_19;
    QLabel *label_8;
    QLabel *label_23;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_10;
    QLabel *label_24;
    QSpacerItem *horizontalSpacer_2;
    QTableWidget *tableWidget_Sell;

    void setupUi(QDialog *OrderDetail_Popup)
    {
        if (OrderDetail_Popup->objectName().isEmpty())
            OrderDetail_Popup->setObjectName("OrderDetail_Popup");
        OrderDetail_Popup->resize(1090, 589);
        OrderDetail_Popup->setStyleSheet(QString::fromUtf8("background-color:#FFF;"));
        gridLayout = new QGridLayout(OrderDetail_Popup);
        gridLayout->setObjectName("gridLayout");
        buttonBox = new QDialogButtonBox(OrderDetail_Popup);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 6, 3, 1, 1);

        widget_3 = new QWidget(OrderDetail_Popup);
        widget_3->setObjectName("widget_3");
        widget_3->setMinimumSize(QSize(1, 1));
        widget_3->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(widget_3);
        gridLayout_3->setObjectName("gridLayout_3");
        label_27 = new QLabel(widget_3);
        label_27->setObjectName("label_27");
        label_27->setMinimumSize(QSize(1, 1));
        label_27->setMaximumSize(QSize(100, 100));
        label_27->setFrameShape(QFrame::NoFrame);
        label_27->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_27, 2, 1, 1, 1);

        label_18 = new QLabel(widget_3);
        label_18->setObjectName("label_18");
        label_18->setMinimumSize(QSize(1, 1));
        label_18->setMaximumSize(QSize(100, 100));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        label_18->setFont(font);
        label_18->setFrameShape(QFrame::NoFrame);
        label_18->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_18, 2, 4, 1, 1);

        label_28 = new QLabel(widget_3);
        label_28->setObjectName("label_28");
        label_28->setMinimumSize(QSize(1, 1));
        label_28->setMaximumSize(QSize(100, 100));
        label_28->setFrameShape(QFrame::NoFrame);
        label_28->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_28, 3, 1, 1, 1);

        label_25 = new QLabel(widget_3);
        label_25->setObjectName("label_25");
        label_25->setMinimumSize(QSize(1, 1));
        label_25->setMaximumSize(QSize(100, 100));
        label_25->setFrameShape(QFrame::NoFrame);
        label_25->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_25, 0, 1, 1, 1);

        label_16 = new QLabel(widget_3);
        label_16->setObjectName("label_16");
        label_16->setMinimumSize(QSize(1, 1));
        label_16->setMaximumSize(QSize(100, 100));
        label_16->setFont(font);
        label_16->setFrameShape(QFrame::NoFrame);
        label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_16, 1, 4, 1, 1);

        label_26 = new QLabel(widget_3);
        label_26->setObjectName("label_26");
        label_26->setMinimumSize(QSize(1, 1));
        label_26->setMaximumSize(QSize(100, 100));
        label_26->setFrameShape(QFrame::NoFrame);
        label_26->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_26, 1, 1, 1, 1);

        label_11 = new QLabel(widget_3);
        label_11->setObjectName("label_11");
        label_11->setMinimumSize(QSize(1, 1));
        label_11->setMaximumSize(QSize(100, 100));
        label_11->setFont(font);
        label_11->setFrameShape(QFrame::NoFrame);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_11, 0, 0, 1, 1);

        label_12 = new QLabel(widget_3);
        label_12->setObjectName("label_12");
        label_12->setMinimumSize(QSize(1, 1));
        label_12->setMaximumSize(QSize(100, 100));
        label_12->setFont(font);
        label_12->setFrameShape(QFrame::NoFrame);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_12, 1, 0, 1, 1);

        label_13 = new QLabel(widget_3);
        label_13->setObjectName("label_13");
        label_13->setMinimumSize(QSize(1, 1));
        label_13->setMaximumSize(QSize(100, 100));
        label_13->setFont(font);
        label_13->setFrameShape(QFrame::NoFrame);
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_13, 2, 0, 1, 1);

        label_15 = new QLabel(widget_3);
        label_15->setObjectName("label_15");
        label_15->setMinimumSize(QSize(1, 1));
        label_15->setMaximumSize(QSize(100, 100));
        label_15->setFont(font);
        label_15->setFrameShape(QFrame::NoFrame);
        label_15->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_15, 0, 4, 1, 1);

        label_14 = new QLabel(widget_3);
        label_14->setObjectName("label_14");
        label_14->setMinimumSize(QSize(1, 1));
        label_14->setMaximumSize(QSize(100, 100));
        label_14->setFont(font);
        label_14->setFrameShape(QFrame::NoFrame);
        label_14->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_14, 3, 0, 1, 1);

        label_29 = new QLabel(widget_3);
        label_29->setObjectName("label_29");
        label_29->setMinimumSize(QSize(1, 1));
        label_29->setMaximumSize(QSize(100, 100));
        label_29->setFrameShape(QFrame::NoFrame);
        label_29->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_29, 0, 5, 1, 1);

        label_30 = new QLabel(widget_3);
        label_30->setObjectName("label_30");
        label_30->setMinimumSize(QSize(1, 1));
        label_30->setMaximumSize(QSize(100, 100));
        label_30->setFrameShape(QFrame::NoFrame);
        label_30->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_30, 1, 5, 1, 1);

        label_31 = new QLabel(widget_3);
        label_31->setObjectName("label_31");
        label_31->setMinimumSize(QSize(1, 1));
        label_31->setMaximumSize(QSize(100, 100));
        label_31->setFrameShape(QFrame::NoFrame);
        label_31->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_31, 2, 5, 1, 1);

        label_32 = new QLabel(widget_3);
        label_32->setObjectName("label_32");
        label_32->setMinimumSize(QSize(1, 1));
        label_32->setMaximumSize(QSize(100, 100));
        label_32->setFrameShape(QFrame::NoFrame);
        label_32->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_32, 3, 5, 1, 1);

        label_17 = new QLabel(widget_3);
        label_17->setObjectName("label_17");
        label_17->setFont(font);

        gridLayout_3->addWidget(label_17, 3, 4, 1, 1);


        gridLayout->addWidget(widget_3, 0, 2, 1, 2);

        tableWidget_Buy = new QTableWidget(OrderDetail_Popup);
        if (tableWidget_Buy->columnCount() < 6)
            tableWidget_Buy->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_Buy->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_Buy->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_Buy->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_Buy->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_Buy->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_Buy->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget_Buy->setObjectName("tableWidget_Buy");
        tableWidget_Buy->setMinimumSize(QSize(50, 50));
        tableWidget_Buy->setStyleSheet(QString::fromUtf8(""));
        tableWidget_Buy->setLineWidth(1);
        tableWidget_Buy->setIconSize(QSize(10, 10));
        tableWidget_Buy->verticalHeader()->setMinimumSectionSize(15);
        tableWidget_Buy->verticalHeader()->setDefaultSectionSize(25);

        gridLayout->addWidget(tableWidget_Buy, 4, 0, 1, 1);

        widget = new QWidget(OrderDetail_Popup);
        widget->setObjectName("widget");
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setHorizontalSpacing(10);

        gridLayout->addWidget(widget, 5, 2, 1, 1);

        widget_2 = new QWidget(OrderDetail_Popup);
        widget_2->setObjectName("widget_2");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy);
        widget_2->setMinimumSize(QSize(0, 0));
        widget_2->setLayoutDirection(Qt::LeftToRight);
        widget_2->setAutoFillBackground(false);
        widget_2->setStyleSheet(QString::fromUtf8(""));
        gridLayout_4 = new QGridLayout(widget_2);
        gridLayout_4->setObjectName("gridLayout_4");
        label = new QLabel(widget_2);
        label->setObjectName("label");
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(1, 1));
        label->setMaximumSize(QSize(100, 100));
        label->setFont(font);
        label->setFrameShape(QFrame::NoFrame);
        label->setMidLineWidth(1);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setWordWrap(false);

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(widget_2);
        label_3->setObjectName("label_3");
        label_3->setMaximumSize(QSize(100, 100));
        label_3->setFrameShape(QFrame::NoFrame);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_3, 0, 1, 1, 1);

        label_9 = new QLabel(widget_2);
        label_9->setObjectName("label_9");
        label_9->setMinimumSize(QSize(1, 1));
        label_9->setMaximumSize(QSize(100, 100));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::NoFrame);
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_9, 0, 2, 1, 1);

        label_21 = new QLabel(widget_2);
        label_21->setObjectName("label_21");
        label_21->setMinimumSize(QSize(1, 1));
        label_21->setMaximumSize(QSize(100, 100));
        label_21->setFrameShape(QFrame::NoFrame);
        label_21->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_21, 0, 3, 1, 1);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName("label_4");
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setFont(font);
        label_4->setFrameShape(QFrame::NoFrame);
        label_4->setMidLineWidth(1);
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4->setWordWrap(false);

        gridLayout_4->addWidget(label_4, 1, 0, 1, 1);

        label_20 = new QLabel(widget_2);
        label_20->setObjectName("label_20");
        label_20->setMaximumSize(QSize(100, 100));
        label_20->setFrameShape(QFrame::NoFrame);
        label_20->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_20, 1, 1, 1, 1);

        label_7 = new QLabel(widget_2);
        label_7->setObjectName("label_7");
        label_7->setFont(font);
        label_7->setFrameShape(QFrame::NoFrame);
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_7, 1, 2, 1, 1);

        label_22 = new QLabel(widget_2);
        label_22->setObjectName("label_22");
        label_22->setMinimumSize(QSize(1, 1));
        label_22->setMaximumSize(QSize(100, 100));
        label_22->setFrameShape(QFrame::NoFrame);
        label_22->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_22, 1, 3, 1, 1);

        label_5 = new QLabel(widget_2);
        label_5->setObjectName("label_5");
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setFont(font);
        label_5->setFrameShape(QFrame::NoFrame);
        label_5->setMidLineWidth(1);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5->setWordWrap(false);

        gridLayout_4->addWidget(label_5, 2, 0, 1, 1);

        label_19 = new QLabel(widget_2);
        label_19->setObjectName("label_19");
        label_19->setMaximumSize(QSize(100, 100));
        label_19->setFrameShape(QFrame::NoFrame);
        label_19->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_19, 2, 1, 1, 1);

        label_8 = new QLabel(widget_2);
        label_8->setObjectName("label_8");
        label_8->setFont(font);
        label_8->setFrameShape(QFrame::NoFrame);
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_8, 2, 2, 1, 1);

        label_23 = new QLabel(widget_2);
        label_23->setObjectName("label_23");
        label_23->setMinimumSize(QSize(1, 1));
        label_23->setMaximumSize(QSize(100, 100));
        label_23->setFrameShape(QFrame::NoFrame);
        label_23->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_23, 2, 3, 1, 1);

        label_6 = new QLabel(widget_2);
        label_6->setObjectName("label_6");
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setFont(font);
        label_6->setFrameShape(QFrame::NoFrame);
        label_6->setMidLineWidth(1);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6->setWordWrap(false);

        gridLayout_4->addWidget(label_6, 3, 0, 1, 1);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(100, 10));
        label_2->setMaximumSize(QSize(100, 100));
        label_2->setFrameShape(QFrame::NoFrame);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_2, 3, 1, 1, 1);

        label_10 = new QLabel(widget_2);
        label_10->setObjectName("label_10");
        label_10->setFont(font);
        label_10->setFrameShape(QFrame::NoFrame);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_10, 3, 2, 1, 1);

        label_24 = new QLabel(widget_2);
        label_24->setObjectName("label_24");
        label_24->setMinimumSize(QSize(1, 1));
        label_24->setMaximumSize(QSize(100, 100));
        label_24->setFrameShape(QFrame::NoFrame);
        label_24->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_24, 3, 3, 1, 1);


        gridLayout->addWidget(widget_2, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(515, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 6, 0, 1, 1);

        tableWidget_Sell = new QTableWidget(OrderDetail_Popup);
        if (tableWidget_Sell->columnCount() < 6)
            tableWidget_Sell->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_Sell->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_Sell->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_Sell->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_Sell->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_Sell->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_Sell->setHorizontalHeaderItem(5, __qtablewidgetitem11);
        tableWidget_Sell->setObjectName("tableWidget_Sell");
        tableWidget_Sell->setStyleSheet(QString::fromUtf8(""));
        tableWidget_Sell->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(tableWidget_Sell, 4, 3, 1, 1);


        retranslateUi(OrderDetail_Popup);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, OrderDetail_Popup, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, OrderDetail_Popup, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(OrderDetail_Popup);
    } // setupUi

    void retranslateUi(QDialog *OrderDetail_Popup)
    {
        OrderDetail_Popup->setWindowTitle(QCoreApplication::translate("OrderDetail_Popup", "Algo Info", nullptr));
        label_27->setText(QString());
        label_18->setText(QCoreApplication::translate("OrderDetail_Popup", "Rejected Order", nullptr));
        label_28->setText(QString());
        label_25->setText(QString());
        label_16->setText(QCoreApplication::translate("OrderDetail_Popup", "Canceled Order", nullptr));
        label_26->setText(QString());
        label_11->setText(QCoreApplication::translate("OrderDetail_Popup", "Bid Leg", nullptr));
        label_12->setText(QCoreApplication::translate("OrderDetail_Popup", "Buy Avg", nullptr));
        label_13->setText(QCoreApplication::translate("OrderDetail_Popup", "Sell Avg", nullptr));
        label_15->setText(QCoreApplication::translate("OrderDetail_Popup", "Total Order", nullptr));
        label_14->setText(QCoreApplication::translate("OrderDetail_Popup", "Lot Size", nullptr));
        label_29->setText(QString());
        label_30->setText(QString());
        label_31->setText(QString());
        label_32->setText(QString());
        label_17->setText(QCoreApplication::translate("OrderDetail_Popup", "Executed Order", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_Buy->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("OrderDetail_Popup", "Traded Price", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_Buy->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("OrderDetail_Popup", "Trigger Price", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_Buy->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("OrderDetail_Popup", "Jackpot", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_Buy->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("OrderDetail_Popup", "TL", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_Buy->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("OrderDetail_Popup", "Rem Lot", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_Buy->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("OrderDetail_Popup", "Time", nullptr));
        label->setText(QCoreApplication::translate("OrderDetail_Popup", "Algo ID", nullptr));
        label_3->setText(QCoreApplication::translate("OrderDetail_Popup", "1", nullptr));
        label_9->setText(QCoreApplication::translate("OrderDetail_Popup", "Algo Name", nullptr));
        label_21->setText(QString());
        label_4->setText(QCoreApplication::translate("OrderDetail_Popup", "Buy Value ", nullptr));
        label_20->setText(QString());
        label_7->setText(QCoreApplication::translate("OrderDetail_Popup", "Buy QTY", nullptr));
        label_22->setText(QString());
        label_5->setText(QCoreApplication::translate("OrderDetail_Popup", "Sell Value", nullptr));
        label_19->setText(QString());
        label_8->setText(QCoreApplication::translate("OrderDetail_Popup", "Sell QTY", nullptr));
        label_23->setText(QString());
        label_6->setText(QCoreApplication::translate("OrderDetail_Popup", "Net QTY", nullptr));
        label_2->setText(QString());
        label_10->setText(QCoreApplication::translate("OrderDetail_Popup", "P&L", nullptr));
        label_24->setText(QString());
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_Sell->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("OrderDetail_Popup", "Traded Price", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_Sell->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("OrderDetail_Popup", "Trigger Price", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_Sell->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("OrderDetail_Popup", "Jackpot", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_Sell->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("OrderDetail_Popup", "Traded Lot", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_Sell->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("OrderDetail_Popup", "Remaining Lot", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_Sell->horizontalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("OrderDetail_Popup", "Traded Time", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OrderDetail_Popup: public Ui_OrderDetail_Popup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORDER_DETAIL_POPUP_H
