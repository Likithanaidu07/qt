/********************************************************************************
** Form generated from reading UI file 'watch_cards.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCH_CARDS_H
#define UI_WATCH_CARDS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Watch_cards
{
public:
    QWidget *watchWidget;
    QVBoxLayout *verticalLayout_8;
    QWidget *widget_15;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_18;
    QSpacerItem *watch;
    QVBoxLayout *verticalLayout_12;
    QLineEdit *lineEditWatchSearch;
    QListWidget *listWidgetWatch;
    QSpacerItem *verticalSpacer;
    QWidget *watchListSelectedDetails_Parent;
    QVBoxLayout *verticalLayout_23;
    QHBoxLayout *horizontalLayout_13;
    QLabel *Index_Name;
    QLabel *watch_val1;
    QLabel *watch_indicator;
    QLabel *watch_val2;
    QSpacerItem *horizontalSpacer_11;
    QPushButton *Buy_Watch_Button;
    QPushButton *Sell_Watch_Button;
    QWidget *horizontalLayout_16;
    QHBoxLayout *watchListSelectedDetailsParent;
    QVBoxLayout *verticalLayout_24;
    QLabel *Open_Label;
    QLabel *Open_Num;
    QVBoxLayout *verticalLayout_27;
    QLabel *High_Label;
    QLabel *High_Num;
    QVBoxLayout *verticalLayout_30;
    QLabel *Low_Label;
    QLabel *Low_Num;
    QVBoxLayout *verticalLayout_28;
    QLabel *Volume_Label;
    QLabel *Volume_Num;

    void setupUi(QDialog *Watch_cards)
    {
        if (Watch_cards->objectName().isEmpty())
            Watch_cards->setObjectName(QString::fromUtf8("Watch_cards"));
        Watch_cards->resize(326, 324);
        watchWidget = new QWidget(Watch_cards);
        watchWidget->setObjectName(QString::fromUtf8("watchWidget"));
        watchWidget->setGeometry(QRect(0, 10, 327, 310));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(watchWidget->sizePolicy().hasHeightForWidth());
        watchWidget->setSizePolicy(sizePolicy);
        watchWidget->setMinimumSize(QSize(0, 0));
        watchWidget->setMaximumSize(QSize(380, 310));
        watchWidget->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_8 = new QVBoxLayout(watchWidget);
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(3, 3, 3, 3);
        widget_15 = new QWidget(watchWidget);
        widget_15->setObjectName(QString::fromUtf8("widget_15"));
        widget_15->setMaximumSize(QSize(16777215, 30));
        widget_15->setStyleSheet(QString::fromUtf8("background-color: rgba(43, 107, 111, 1);\n"
""));
        horizontalLayout_11 = new QHBoxLayout(widget_15);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(-1, 5, -1, 5);
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_7);

        label_18 = new QLabel(widget_15);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setStyleSheet(QString::fromUtf8("#label_18{\n"
"color: #FFFFFF;\n"
"text-align: center;\n"
"font-family: Work Sans;\n"
"font-size: 16px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_11->addWidget(label_18);

        watch = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(watch);


        verticalLayout_8->addWidget(widget_15);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(5);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(0, 10, 0, -1);
        lineEditWatchSearch = new QLineEdit(watchWidget);
        lineEditWatchSearch->setObjectName(QString::fromUtf8("lineEditWatchSearch"));
        lineEditWatchSearch->setMinimumSize(QSize(0, 30));
        lineEditWatchSearch->setStyleSheet(QString::fromUtf8("   background-image: url(:/search.png);\n"
"    background-repeat: no-repeat;\n"
"    background-position: right center;\n"
"	background-origin: content;\n"
"    padding-right: 20px; \n"
"    padding-left: 6px;\n"
"    padding-top: 2px;\n"
"    padding-bottom: 2px;\n"
"    border: 1px solid #C2CCC5;\n"
"    color: #3D3D3D;\n"
"    font-size: 12px;\n"
"    border-radius: 14px; /* Adjust this value for your desired border radius */\n"
"\n"
""));

        verticalLayout_12->addWidget(lineEditWatchSearch);

        listWidgetWatch = new QListWidget(watchWidget);
        listWidgetWatch->setObjectName(QString::fromUtf8("listWidgetWatch"));
        listWidgetWatch->setStyleSheet(QString::fromUtf8("#listWidgetWatch{\n"
"background: #F3FEFF;\n"
"border-bottom: 21px solid gray; /* Adjust border width for desired shadow size */\n"
"\n"
"  border-bottom-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, /* Gradient from top to bottom */\n"
"                                    stop: 0 rgba(0, 0, 0, 0), /* Black with some transparency at top */\n"
"                                    stop: 1 rgba(0, 0, 0,.2) /* Fades to transparent at bottom */\n"
"                                   );\n"
"}\n"
"#listWidgetWatch::item:selected { \n"
"background-color: #A1E3E7; \n"
"}"));
        listWidgetWatch->setFrameShape(QFrame::NoFrame);
        listWidgetWatch->setLineWidth(1);
        listWidgetWatch->setMovement(QListView::Free);
        listWidgetWatch->setProperty("isWrapping", QVariant(false));
        listWidgetWatch->setResizeMode(QListView::Fixed);
        listWidgetWatch->setLayoutMode(QListView::SinglePass);

        verticalLayout_12->addWidget(listWidgetWatch);


        verticalLayout_8->addLayout(verticalLayout_12);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_8->addItem(verticalSpacer);

        watchListSelectedDetails_Parent = new QWidget(watchWidget);
        watchListSelectedDetails_Parent->setObjectName(QString::fromUtf8("watchListSelectedDetails_Parent"));
        watchListSelectedDetails_Parent->setStyleSheet(QString::fromUtf8("#widget_17{\n"
"	border-top-left-radius: 0px;\n"
"    border-top-right-radius: 0px;\n"
"    border-bottom-right-radius: 7px;\n"
"    border-bottom-left-radius: 7px;\n"
"	background: #F7FCFC;\n"
"	backdrop-filter: blur(4.5px);\n"
"	background-color: rgb(255, 255, 255);\n"
"\n"
"\n"
"}"));
        verticalLayout_23 = new QVBoxLayout(watchListSelectedDetails_Parent);
        verticalLayout_23->setSpacing(12);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        verticalLayout_23->setContentsMargins(-1, 8, 9, 9);
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(3);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        Index_Name = new QLabel(watchListSelectedDetails_Parent);
        Index_Name->setObjectName(QString::fromUtf8("Index_Name"));

        horizontalLayout_13->addWidget(Index_Name);

        watch_val1 = new QLabel(watchListSelectedDetails_Parent);
        watch_val1->setObjectName(QString::fromUtf8("watch_val1"));

        horizontalLayout_13->addWidget(watch_val1);

        watch_indicator = new QLabel(watchListSelectedDetails_Parent);
        watch_indicator->setObjectName(QString::fromUtf8("watch_indicator"));
        watch_indicator->setMinimumSize(QSize(6, 10));
        watch_indicator->setMaximumSize(QSize(6, 10));

        horizontalLayout_13->addWidget(watch_indicator);

        watch_val2 = new QLabel(watchListSelectedDetails_Parent);
        watch_val2->setObjectName(QString::fromUtf8("watch_val2"));

        horizontalLayout_13->addWidget(watch_val2);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_11);

        Buy_Watch_Button = new QPushButton(watchListSelectedDetails_Parent);
        Buy_Watch_Button->setObjectName(QString::fromUtf8("Buy_Watch_Button"));
        Buy_Watch_Button->setEnabled(true);
        Buy_Watch_Button->setMinimumSize(QSize(26, 26));
        Buy_Watch_Button->setMaximumSize(QSize(26, 26));
        Buy_Watch_Button->setStyleSheet(QString::fromUtf8("#Add_Watch_Button{\n"
"border-radius: 2px;\n"
"background: #197CB1;\n"
"color: #FFF;\n"
"font-size: 12px;\n"
"font-style: normal;\n"
"font-weight: 700;\n"
"line-height: normal;\n"
"}\n"
"\n"
"#Add_Watch_Button:hover {\n"
"    background: #156693; /* Change color on hover */\n"
"}\n"
"\n"
"#Add_Watch_Button:disabled {\n"
"    background: #CCC; /* Change color when disabled */\n"
"    color: #666; /* Change text color when disabled */\n"
"}"));

        horizontalLayout_13->addWidget(Buy_Watch_Button);

        Sell_Watch_Button = new QPushButton(watchListSelectedDetails_Parent);
        Sell_Watch_Button->setObjectName(QString::fromUtf8("Sell_Watch_Button"));
        Sell_Watch_Button->setEnabled(true);
        Sell_Watch_Button->setMinimumSize(QSize(26, 26));
        Sell_Watch_Button->setMaximumSize(QSize(26, 26));
        Sell_Watch_Button->setStyleSheet(QString::fromUtf8("#Subtract_Watch_Button{\n"
"border-radius: 2px;\n"
"background: #CC3437;\n"
"color: #FFF;\n"
"font-size: 12px;\n"
"font-style: normal;\n"
"font-weight: 700;\n"
"line-height: normal;\n"
"}\n"
"#Subtract_Watch_Button:hover {\n"
"    background: #B92E31; /* Change color on hover */\n"
"}\n"
"\n"
"#Subtract_Watch_Button:disabled {\n"
"    background: #CCC; /* Change color when disabled */\n"
"    color: #666; /* Change text color when disabled */\n"
"}"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("edit-delete");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("../"), QSize(), QIcon::Normal, QIcon::Off);
        }
        Sell_Watch_Button->setIcon(icon);

        horizontalLayout_13->addWidget(Sell_Watch_Button);


        verticalLayout_23->addLayout(horizontalLayout_13);

        horizontalLayout_16 = new QWidget(watchListSelectedDetails_Parent);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        watchListSelectedDetailsParent = new QHBoxLayout(horizontalLayout_16);
        watchListSelectedDetailsParent->setSpacing(9);
        watchListSelectedDetailsParent->setObjectName(QString::fromUtf8("watchListSelectedDetailsParent"));
        verticalLayout_24 = new QVBoxLayout();
        verticalLayout_24->setSpacing(2);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        Open_Label = new QLabel(horizontalLayout_16);
        Open_Label->setObjectName(QString::fromUtf8("Open_Label"));
        QFont font;
        font.setPointSize(10);
        Open_Label->setFont(font);
        Open_Label->setStyleSheet(QString::fromUtf8("color: #848484;"));

        verticalLayout_24->addWidget(Open_Label);

        Open_Num = new QLabel(horizontalLayout_16);
        Open_Num->setObjectName(QString::fromUtf8("Open_Num"));
        QFont font1;
        font1.setPointSize(11);
        Open_Num->setFont(font1);
        Open_Num->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_24->addWidget(Open_Num);


        watchListSelectedDetailsParent->addLayout(verticalLayout_24);

        verticalLayout_27 = new QVBoxLayout();
        verticalLayout_27->setSpacing(2);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        High_Label = new QLabel(horizontalLayout_16);
        High_Label->setObjectName(QString::fromUtf8("High_Label"));
        High_Label->setFont(font);
        High_Label->setStyleSheet(QString::fromUtf8("color: #848484;"));
        High_Label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_27->addWidget(High_Label);

        High_Num = new QLabel(horizontalLayout_16);
        High_Num->setObjectName(QString::fromUtf8("High_Num"));
        High_Num->setFont(font1);
        High_Num->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_27->addWidget(High_Num);


        watchListSelectedDetailsParent->addLayout(verticalLayout_27);

        verticalLayout_30 = new QVBoxLayout();
        verticalLayout_30->setSpacing(2);
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        Low_Label = new QLabel(horizontalLayout_16);
        Low_Label->setObjectName(QString::fromUtf8("Low_Label"));
        Low_Label->setFont(font);
        Low_Label->setStyleSheet(QString::fromUtf8("color: #848484;"));
        Low_Label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_30->addWidget(Low_Label);

        Low_Num = new QLabel(horizontalLayout_16);
        Low_Num->setObjectName(QString::fromUtf8("Low_Num"));
        Low_Num->setFont(font1);
        Low_Num->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_30->addWidget(Low_Num);


        watchListSelectedDetailsParent->addLayout(verticalLayout_30);

        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setSpacing(2);
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        Volume_Label = new QLabel(horizontalLayout_16);
        Volume_Label->setObjectName(QString::fromUtf8("Volume_Label"));
        Volume_Label->setFont(font);
        Volume_Label->setStyleSheet(QString::fromUtf8("color: #848484;"));
        Volume_Label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_28->addWidget(Volume_Label);

        Volume_Num = new QLabel(horizontalLayout_16);
        Volume_Num->setObjectName(QString::fromUtf8("Volume_Num"));
        Volume_Num->setFont(font1);
        Volume_Num->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_28->addWidget(Volume_Num);


        watchListSelectedDetailsParent->addLayout(verticalLayout_28);


        verticalLayout_23->addWidget(horizontalLayout_16);


        verticalLayout_8->addWidget(watchListSelectedDetails_Parent);


        retranslateUi(Watch_cards);

        QMetaObject::connectSlotsByName(Watch_cards);
    } // setupUi

    void retranslateUi(QDialog *Watch_cards)
    {
        Watch_cards->setWindowTitle(QCoreApplication::translate("Watch_cards", "Watch", nullptr));
        label_18->setText(QCoreApplication::translate("Watch_cards", "Watch", nullptr));
        Index_Name->setText(QString());
        watch_val1->setText(QString());
        watch_indicator->setText(QString());
        watch_val2->setText(QString());
        Buy_Watch_Button->setText(QCoreApplication::translate("Watch_cards", "B", nullptr));
        Sell_Watch_Button->setText(QCoreApplication::translate("Watch_cards", "S", nullptr));
        Open_Label->setText(QCoreApplication::translate("Watch_cards", "Open", nullptr));
        Open_Num->setText(QCoreApplication::translate("Watch_cards", "0.00", nullptr));
        High_Label->setText(QCoreApplication::translate("Watch_cards", "High", nullptr));
        High_Num->setText(QCoreApplication::translate("Watch_cards", "0.00", nullptr));
        Low_Label->setText(QCoreApplication::translate("Watch_cards", "Low", nullptr));
        Low_Num->setText(QCoreApplication::translate("Watch_cards", "0.00", nullptr));
        Volume_Label->setText(QCoreApplication::translate("Watch_cards", "Volume", nullptr));
        Volume_Num->setText(QCoreApplication::translate("Watch_cards", "0.00", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Watch_cards: public Ui_Watch_cards {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCH_CARDS_H
