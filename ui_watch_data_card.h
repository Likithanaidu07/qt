/********************************************************************************
** Form generated from reading UI file 'watch_data_card.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCH_DATA_CARD_H
#define UI_WATCH_DATA_CARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_watch_data_card
{
public:
    QGridLayout *gridLayout;
    QWidget *Background;
    QGridLayout *gridLayout_2;
    QLabel *indicator;
    QSpacerItem *horizontalSpacer;
    QLabel *value2;
    QLabel *name;
    QLabel *value1;
    QLabel *value3;

    void setupUi(QWidget *watch_data_card)
    {
        if (watch_data_card->objectName().isEmpty())
            watch_data_card->setObjectName(QString::fromUtf8("watch_data_card"));
        watch_data_card->resize(170, 48);
        watch_data_card->setMinimumSize(QSize(170, 0));
        watch_data_card->setMaximumSize(QSize(153, 16777215));
        gridLayout = new QGridLayout(watch_data_card);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Background = new QWidget(watch_data_card);
        Background->setObjectName(QString::fromUtf8("Background"));
        Background->setMaximumSize(QSize(16777215, 16777215));
        Background->setStyleSheet(QString::fromUtf8(""));
        gridLayout_2 = new QGridLayout(Background);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(4, 4, 4, 4);
        indicator = new QLabel(Background);
        indicator->setObjectName(QString::fromUtf8("indicator"));
        indicator->setMinimumSize(QSize(10, 6));
        indicator->setMaximumSize(QSize(10, 6));

        gridLayout_2->addWidget(indicator, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 5, 1, 1);

        value2 = new QLabel(Background);
        value2->setObjectName(QString::fromUtf8("value2"));
        value2->setMinimumSize(QSize(50, 0));
        value2->setMaximumSize(QSize(50, 16777215));

        gridLayout_2->addWidget(value2, 1, 4, 1, 1);

        name = new QLabel(Background);
        name->setObjectName(QString::fromUtf8("name"));
        name->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setPointSize(9);
        font.setBold(false);
        name->setFont(font);
        name->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(name, 0, 0, 1, 4);

        value1 = new QLabel(Background);
        value1->setObjectName(QString::fromUtf8("value1"));
        value1->setMinimumSize(QSize(70, 0));
        value1->setMaximumSize(QSize(70, 16777215));
        value1->setLayoutDirection(Qt::LeftToRight);
        value1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(value1, 1, 0, 1, 1);

        value3 = new QLabel(Background);
        value3->setObjectName(QString::fromUtf8("value3"));
        value3->setMaximumSize(QSize(50, 16777215));
        value3->setLayoutDirection(Qt::RightToLeft);
        value3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(value3, 0, 4, 1, 1);


        gridLayout->addWidget(Background, 0, 0, 1, 3);


        retranslateUi(watch_data_card);

        QMetaObject::connectSlotsByName(watch_data_card);
    } // setupUi

    void retranslateUi(QWidget *watch_data_card)
    {
        watch_data_card->setWindowTitle(QCoreApplication::translate("watch_data_card", "Form", nullptr));
        indicator->setText(QCoreApplication::translate("watch_data_card", "^", nullptr));
        value2->setText(QCoreApplication::translate("watch_data_card", "9.50 ", nullptr));
        name->setText(QCoreApplication::translate("watch_data_card", "Nifty Fifty", nullptr));
        value1->setText(QCoreApplication::translate("watch_data_card", "1936.75", nullptr));
        value3->setText(QCoreApplication::translate("watch_data_card", "(0.05%)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class watch_data_card: public Ui_watch_data_card {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCH_DATA_CARD_H
