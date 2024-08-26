/********************************************************************************
** Form generated from reading UI file 'summary_cards.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUMMARY_CARDS_H
#define UI_SUMMARY_CARDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Summary_cards
{
public:
    QWidget *summary_widget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_14;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_17;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_6;
    QLabel *label_23;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_20;
    QLabel *label_24;
    QGroupBox *groupBox_12;
    QVBoxLayout *verticalLayout_21;
    QLabel *label_15;
    QLabel *label_33;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_10;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_7;
    QLabel *label_25;
    QGroupBox *groupBox_9;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_8;
    QLabel *label_26;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_9;
    QLabel *label_27;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_17;
    QLabel *label_10;
    QLabel *label_28;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_16;
    QVBoxLayout *verticalLayout_18;
    QLabel *m2mlabel;
    QLabel *label_30;
    QGroupBox *groupBox_15;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_13;
    QLabel *label_31;
    QGroupBox *groupBox_14;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_14;
    QLabel *label_32;
    QGroupBox *groupBox_11;
    QVBoxLayout *verticalLayout_22;
    QLabel *label_16;
    QLabel *label_34;

    void setupUi(QDialog *Summary_cards)
    {
        if (Summary_cards->objectName().isEmpty())
            Summary_cards->setObjectName("Summary_cards");
        Summary_cards->setWindowModality(Qt::NonModal);
        Summary_cards->resize(336, 276);
        Summary_cards->setModal(false);
        summary_widget = new QWidget(Summary_cards);
        summary_widget->setObjectName("summary_widget");
        summary_widget->setGeometry(QRect(0, 0, 331, 271));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(summary_widget->sizePolicy().hasHeightForWidth());
        summary_widget->setSizePolicy(sizePolicy);
        summary_widget->setMinimumSize(QSize(0, 0));
        summary_widget->setMaximumSize(QSize(380, 350));
        summary_widget->setStyleSheet(QString::fromUtf8("#summary_widget{\n"
"border-radius: 12px;\n"
"border: 3px solid #C0DDE1;\n"
"background: #C2DFE3;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(summary_widget);
        verticalLayout_2->setSpacing(4);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(3, 3, 3, 5);
        widget_14 = new QWidget(summary_widget);
        widget_14->setObjectName("widget_14");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_14->sizePolicy().hasHeightForWidth());
        widget_14->setSizePolicy(sizePolicy1);
        widget_14->setMinimumSize(QSize(0, 0));
        widget_14->setMaximumSize(QSize(16777215, 25));
        widget_14->setStyleSheet(QString::fromUtf8("background-color: rgba(49, 86, 118, 1);\n"
""));
        horizontalLayout_8 = new QHBoxLayout(widget_14);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalLayout_8->setContentsMargins(-1, 2, -1, 5);
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_5);

        label_17 = new QLabel(widget_14);
        label_17->setObjectName("label_17");
        label_17->setStyleSheet(QString::fromUtf8("#label_17{\n"
"color: #FFFFFF;\n"
"text-align: center;\n"
"font-family: Work Sans;\n"
"font-size: 16px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_8->addWidget(label_17);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_6);


        verticalLayout_2->addWidget(widget_14);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(7, -1, 7, -1);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName("verticalLayout_4");
        groupBox = new QGroupBox(summary_widget);
        groupBox->setObjectName("groupBox");
        groupBox->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_10 = new QVBoxLayout(groupBox);
        verticalLayout_10->setObjectName("verticalLayout_10");
        verticalLayout_10->setContentsMargins(3, 2, 3, 2);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setStyleSheet(QString::fromUtf8(" border-radius: 6px;\n"
""));

        verticalLayout_10->addWidget(label_3);


        verticalLayout_4->addWidget(groupBox);

        groupBox_4 = new QGroupBox(summary_widget);
        groupBox_4->setObjectName("groupBox_4");
        verticalLayout_13 = new QVBoxLayout(groupBox_4);
        verticalLayout_13->setObjectName("verticalLayout_13");
        verticalLayout_13->setContentsMargins(-1, 2, -1, 2);
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName("label_6");
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_13->addWidget(label_6);

        label_23 = new QLabel(groupBox_4);
        label_23->setObjectName("label_23");

        verticalLayout_13->addWidget(label_23);


        verticalLayout_4->addWidget(groupBox_4);

        groupBox_2 = new QGroupBox(summary_widget);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_14 = new QVBoxLayout(groupBox_2);
        verticalLayout_14->setObjectName("verticalLayout_14");
        verticalLayout_14->setContentsMargins(-1, 2, -1, 2);
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName("label_20");
        label_20->setAlignment(Qt::AlignCenter);

        verticalLayout_14->addWidget(label_20);

        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName("label_24");

        verticalLayout_14->addWidget(label_24);


        verticalLayout_4->addWidget(groupBox_2);

        groupBox_12 = new QGroupBox(summary_widget);
        groupBox_12->setObjectName("groupBox_12");
        verticalLayout_21 = new QVBoxLayout(groupBox_12);
        verticalLayout_21->setObjectName("verticalLayout_21");
        verticalLayout_21->setContentsMargins(2, 2, 2, 2);
        label_15 = new QLabel(groupBox_12);
        label_15->setObjectName("label_15");
        label_15->setMinimumSize(QSize(100, 0));
        label_15->setAlignment(Qt::AlignCenter);

        verticalLayout_21->addWidget(label_15);

        label_33 = new QLabel(groupBox_12);
        label_33->setObjectName("label_33");

        verticalLayout_21->addWidget(label_33);


        verticalLayout_4->addWidget(groupBox_12);


        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        groupBox_10 = new QGroupBox(summary_widget);
        groupBox_10->setObjectName("groupBox_10");
        verticalLayout_6 = new QVBoxLayout(groupBox_10);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(-1, 2, -1, 2);
        label_7 = new QLabel(groupBox_10);
        label_7->setObjectName("label_7");
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_7);

        label_25 = new QLabel(groupBox_10);
        label_25->setObjectName("label_25");

        verticalLayout_6->addWidget(label_25);


        verticalLayout_3->addWidget(groupBox_10);

        groupBox_9 = new QGroupBox(summary_widget);
        groupBox_9->setObjectName("groupBox_9");
        verticalLayout_7 = new QVBoxLayout(groupBox_9);
        verticalLayout_7->setObjectName("verticalLayout_7");
        verticalLayout_7->setContentsMargins(-1, 2, -1, 2);
        label_8 = new QLabel(groupBox_9);
        label_8->setObjectName("label_8");
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_8);

        label_26 = new QLabel(groupBox_9);
        label_26->setObjectName("label_26");

        verticalLayout_7->addWidget(label_26);


        verticalLayout_3->addWidget(groupBox_9);

        groupBox_6 = new QGroupBox(summary_widget);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setMinimumSize(QSize(0, 0));
        verticalLayout_15 = new QVBoxLayout(groupBox_6);
        verticalLayout_15->setObjectName("verticalLayout_15");
        verticalLayout_15->setContentsMargins(3, 2, 3, 2);
        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName("label_9");
        label_9->setMinimumSize(QSize(0, 0));
        label_9->setAlignment(Qt::AlignCenter);
        label_9->setWordWrap(false);

        verticalLayout_15->addWidget(label_9);

        label_27 = new QLabel(groupBox_6);
        label_27->setObjectName("label_27");

        verticalLayout_15->addWidget(label_27);


        verticalLayout_3->addWidget(groupBox_6);

        groupBox_7 = new QGroupBox(summary_widget);
        groupBox_7->setObjectName("groupBox_7");
        verticalLayout_17 = new QVBoxLayout(groupBox_7);
        verticalLayout_17->setObjectName("verticalLayout_17");
        verticalLayout_17->setContentsMargins(3, 2, 3, 2);
        label_10 = new QLabel(groupBox_7);
        label_10->setObjectName("label_10");
        label_10->setAlignment(Qt::AlignCenter);

        verticalLayout_17->addWidget(label_10);

        label_28 = new QLabel(groupBox_7);
        label_28->setObjectName("label_28");

        verticalLayout_17->addWidget(label_28);


        verticalLayout_3->addWidget(groupBox_7);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        groupBox_16 = new QGroupBox(summary_widget);
        groupBox_16->setObjectName("groupBox_16");
        verticalLayout_18 = new QVBoxLayout(groupBox_16);
        verticalLayout_18->setObjectName("verticalLayout_18");
        verticalLayout_18->setContentsMargins(-1, 2, -1, 2);
        m2mlabel = new QLabel(groupBox_16);
        m2mlabel->setObjectName("m2mlabel");
        m2mlabel->setAlignment(Qt::AlignCenter);

        verticalLayout_18->addWidget(m2mlabel);

        label_30 = new QLabel(groupBox_16);
        label_30->setObjectName("label_30");

        verticalLayout_18->addWidget(label_30);


        verticalLayout_5->addWidget(groupBox_16);

        groupBox_15 = new QGroupBox(summary_widget);
        groupBox_15->setObjectName("groupBox_15");
        verticalLayout_19 = new QVBoxLayout(groupBox_15);
        verticalLayout_19->setObjectName("verticalLayout_19");
        verticalLayout_19->setContentsMargins(-1, 2, -1, 2);
        label_13 = new QLabel(groupBox_15);
        label_13->setObjectName("label_13");
        label_13->setAlignment(Qt::AlignCenter);

        verticalLayout_19->addWidget(label_13);

        label_31 = new QLabel(groupBox_15);
        label_31->setObjectName("label_31");

        verticalLayout_19->addWidget(label_31);


        verticalLayout_5->addWidget(groupBox_15);

        groupBox_14 = new QGroupBox(summary_widget);
        groupBox_14->setObjectName("groupBox_14");
        verticalLayout_20 = new QVBoxLayout(groupBox_14);
        verticalLayout_20->setObjectName("verticalLayout_20");
        verticalLayout_20->setContentsMargins(3, 2, 3, 2);
        label_14 = new QLabel(groupBox_14);
        label_14->setObjectName("label_14");
        label_14->setAlignment(Qt::AlignCenter);
        label_14->setWordWrap(false);

        verticalLayout_20->addWidget(label_14);

        label_32 = new QLabel(groupBox_14);
        label_32->setObjectName("label_32");

        verticalLayout_20->addWidget(label_32);


        verticalLayout_5->addWidget(groupBox_14);

        groupBox_11 = new QGroupBox(summary_widget);
        groupBox_11->setObjectName("groupBox_11");
        verticalLayout_22 = new QVBoxLayout(groupBox_11);
        verticalLayout_22->setObjectName("verticalLayout_22");
        verticalLayout_22->setContentsMargins(-1, 2, -1, 2);
        label_16 = new QLabel(groupBox_11);
        label_16->setObjectName("label_16");
        label_16->setAlignment(Qt::AlignCenter);

        verticalLayout_22->addWidget(label_16);

        label_34 = new QLabel(groupBox_11);
        label_34->setObjectName("label_34");

        verticalLayout_22->addWidget(label_34);


        verticalLayout_5->addWidget(groupBox_11);


        horizontalLayout_3->addLayout(verticalLayout_5);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(Summary_cards);

        QMetaObject::connectSlotsByName(Summary_cards);
    } // setupUi

    void retranslateUi(QDialog *Summary_cards)
    {
        Summary_cards->setWindowTitle(QCoreApplication::translate("Summary_cards", "Summary", nullptr));
        label_17->setText(QCoreApplication::translate("Summary_cards", "User Summary", nullptr));
        groupBox->setTitle(QString());
        label_2->setText(QCoreApplication::translate("Summary_cards", "Algo Count ", nullptr));
        label_3->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_4->setTitle(QString());
        label_6->setText(QCoreApplication::translate("Summary_cards", "Buy value", nullptr));
        label_23->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_2->setTitle(QString());
        label_20->setText(QCoreApplication::translate("Summary_cards", "Buy Lot", nullptr));
        label_24->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_12->setTitle(QString());
        label_15->setText(QCoreApplication::translate("Summary_cards", "M2M", nullptr));
        label_33->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_10->setTitle(QString());
        label_7->setText(QCoreApplication::translate("Summary_cards", "Order Count", nullptr));
        label_25->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_9->setTitle(QString());
        label_8->setText(QCoreApplication::translate("Summary_cards", "Sell Value", nullptr));
        label_26->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_6->setTitle(QString());
        label_9->setText(QCoreApplication::translate("Summary_cards", "Sell Lot", nullptr));
        label_27->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_7->setTitle(QString());
        label_10->setText(QCoreApplication::translate("Summary_cards", "Avl Margin", nullptr));
        label_28->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_16->setTitle(QString());
        m2mlabel->setText(QCoreApplication::translate("Summary_cards", "Trade Count", nullptr));
        label_30->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_15->setTitle(QString());
        label_13->setText(QCoreApplication::translate("Summary_cards", "Profit", nullptr));
        label_31->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_14->setTitle(QString());
        label_14->setText(QCoreApplication::translate("Summary_cards", "Net Lot", nullptr));
        label_32->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
        groupBox_11->setTitle(QString());
        label_16->setText(QCoreApplication::translate("Summary_cards", "Used Margin", nullptr));
        label_34->setText(QCoreApplication::translate("Summary_cards", "<html>\n"
"<head/>\n"
"<body>\n"
"    <p style=\"text-align: center;\">\n"
"        <span style=\"font-size: 11pt; font-weight: 700;\">\n"
"            -\n"
"        </span>\n"
"    </p>\n"
"</body>\n"
"</html>\n"
"", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Summary_cards: public Ui_Summary_cards {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUMMARY_CARDS_H
