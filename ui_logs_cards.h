/********************************************************************************
** Form generated from reading UI file 'logs_cards.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGS_CARDS_H
#define UI_LOGS_CARDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Logs_cards
{
public:
    QWidget *Logs_Widget;
    QVBoxLayout *verticalLayout_9;
    QWidget *widget_16;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_19;
    QSpacerItem *horizontalSpacer_10;
    QTextEdit *textEdit;

    void setupUi(QDialog *Logs_cards)
    {
        if (Logs_cards->objectName().isEmpty())
            Logs_cards->setObjectName(QString::fromUtf8("Logs_cards"));
        Logs_cards->setWindowModality(Qt::NonModal);
        Logs_cards->resize(319, 232);
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        Logs_cards->setFont(font);
        Logs_cards->setContextMenuPolicy(Qt::CustomContextMenu);
        Logs_cards->setWindowTitle(QString::fromUtf8("Logs"));
        Logs_Widget = new QWidget(Logs_cards);
        Logs_Widget->setObjectName(QString::fromUtf8("Logs_Widget"));
        Logs_Widget->setEnabled(true);
        Logs_Widget->setGeometry(QRect(0, 0, 321, 231));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Logs_Widget->sizePolicy().hasHeightForWidth());
        Logs_Widget->setSizePolicy(sizePolicy);
        Logs_Widget->setMinimumSize(QSize(0, 0));
        Logs_Widget->setMaximumSize(QSize(380, 380));
        Logs_Widget->setStyleSheet(QString::fromUtf8("#Logs_Widget{\n"
"border-radius: 12px;\n"
"\n"
"background: #fff;\n"
"}"));
        verticalLayout_9 = new QVBoxLayout(Logs_Widget);
        verticalLayout_9->setSpacing(3);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(3, 3, 3, 3);
        widget_16 = new QWidget(Logs_Widget);
        widget_16->setObjectName(QString::fromUtf8("widget_16"));
        widget_16->setMaximumSize(QSize(16777215, 25));
        widget_16->setLayoutDirection(Qt::LeftToRight);
        widget_16->setAutoFillBackground(false);
        widget_16->setStyleSheet(QString::fromUtf8("background-color: rgb(77, 77, 115);\n"
""));
        horizontalLayout_12 = new QHBoxLayout(widget_16);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(-1, 2, -1, 5);
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);

        label_19 = new QLabel(widget_16);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setStyleSheet(QString::fromUtf8("#label_19{\n"
"color: #FFFFFF;\n"
"text-align: center;\n"
"font-family: Work Sans;\n"
"font-size: 16px;\n"
"font-style: normal;\n"
"font-weight: 600;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_12->addWidget(label_19);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_10);


        verticalLayout_9->addWidget(widget_16);

        textEdit = new QTextEdit(Logs_Widget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setStyleSheet(QString::fromUtf8("#textEdit{\n"
"border-top-left-radius: 0px;\n"
"    border-top-right-radius: 0px;\n"
"    border-bottom-right-radius: 8px;\n"
"    border-bottom-left-radius: 8px;\n"
"background: #FFFFFF;\n"
"line-height:1;\n"
"}"));
        textEdit->setReadOnly(false);
        textEdit->setCursorWidth(1);

        verticalLayout_9->addWidget(textEdit);


        retranslateUi(Logs_cards);

        QMetaObject::connectSlotsByName(Logs_cards);
    } // setupUi

    void retranslateUi(QDialog *Logs_cards)
    {
        label_19->setText(QCoreApplication::translate("Logs_cards", "Logs", nullptr));
        textEdit->setMarkdown(QString());
        textEdit->setHtml(QCoreApplication::translate("Logs_cards", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        (void)Logs_cards;
    } // retranslateUi

};

namespace Ui {
    class Logs_cards: public Ui_Logs_cards {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGS_CARDS_H
