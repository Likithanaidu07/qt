/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loginwindow
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_forgot;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_contact;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_wron_input;
    QLineEdit *lineEditUsername;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_7;
    QLabel *footer;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_8;
    QSpacerItem *verticalSpacer_6;
    QSpacerItem *verticalSpacer_4;
    QLineEdit *lineEdit_Password;
    QHBoxLayout *horizontalLayout_6;
    QWidget *title_bar;
    QToolButton *minimize;
    QToolButton *close;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar;
    QPushButton *pushButtonlogin;
    QHBoxLayout *horizontalLayout;
    QLabel *logo;
    QLabel *name;

    void setupUi(QWidget *loginwindow)
    {
        if (loginwindow->objectName().isEmpty())
            loginwindow->setObjectName("loginwindow");
        loginwindow->resize(424, 665);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(loginwindow->sizePolicy().hasHeightForWidth());
        loginwindow->setSizePolicy(sizePolicy);
        loginwindow->setMinimumSize(QSize(424, 665));
        loginwindow->setMaximumSize(QSize(424, 692));
        loginwindow->setStyleSheet(QString::fromUtf8("#loginwindow{\n"
"  background-color: #6C757D;\n"
"}"));
        gridLayout = new QGridLayout(loginwindow);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(-1, 9, -1, -1);
        widget = new QWidget(loginwindow);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(104, 14));
        widget->setMaximumSize(QSize(16777215, 40));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(-1, 0, -1, 0);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label_forgot = new QLabel(widget);
        label_forgot->setObjectName("label_forgot");
        label_forgot->setEnabled(true);
        sizePolicy.setHeightForWidth(label_forgot->sizePolicy().hasHeightForWidth());
        label_forgot->setSizePolicy(sizePolicy);
        label_forgot->setMinimumSize(QSize(104, 14));
        label_forgot->setMaximumSize(QSize(104, 14));
        label_forgot->setStyleSheet(QString::fromUtf8("#label_forgot{\n"
"color: #FFF;\n"
"font-family: Work Sans;\n"
"font-size: 12px;\n"
"font-style: normal;\n"
"font-weight: 400;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_3->addWidget(label_forgot);

        horizontalSpacer_5 = new QSpacerItem(4, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        label_contact = new QLabel(widget);
        label_contact->setObjectName("label_contact");
        sizePolicy.setHeightForWidth(label_contact->sizePolicy().hasHeightForWidth());
        label_contact->setSizePolicy(sizePolicy);
        label_contact->setMinimumSize(QSize(98, 14));
        label_contact->setMaximumSize(QSize(98, 14));
        label_contact->setStyleSheet(QString::fromUtf8("#label_contact{\n"
"color: #98282E;\n"
"\n"
"font-family: Work Sans;\n"
"font-size: 12px;\n"
"font-style: normal;\n"
"font-weight: 400;\n"
"line-height: normal;\n"
"}"));

        horizontalLayout_3->addWidget(label_contact);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        gridLayout->addWidget(widget, 14, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_3, 6, 1, 1, 1);

        verticalSpacer_5 = new QSpacerItem(8, 8, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_5, 13, 1, 1, 1);

        label_wron_input = new QLabel(loginwindow);
        label_wron_input->setObjectName("label_wron_input");
        label_wron_input->setLayoutDirection(Qt::RightToLeft);
        label_wron_input->setStyleSheet(QString::fromUtf8("#label_wron_input{\n"
"color: #98282E;\n"
"font-family: Work Sans;\n"
"font-size: 12px;\n"
"font-style: normal;\n"
"font-weight: 400;\n"
"line-height: normal;\n"
"}"));
        label_wron_input->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_wron_input, 10, 1, 1, 1);

        lineEditUsername = new QLineEdit(loginwindow);
        lineEditUsername->setObjectName("lineEditUsername");
        sizePolicy.setHeightForWidth(lineEditUsername->sizePolicy().hasHeightForWidth());
        lineEditUsername->setSizePolicy(sizePolicy);
        lineEditUsername->setMinimumSize(QSize(310, 40));
        lineEditUsername->setMaximumSize(QSize(310, 40));
        lineEditUsername->setStyleSheet(QString::fromUtf8("#lineEditUsername{\n"
"border-radius: 6px;\n"
"border: 1px solid #ADB5BD;\n"
"background: #D9D9D9;\n"
"padding: 4px 10px 4px 10px;\n"
"}"));

        gridLayout->addWidget(lineEditUsername, 5, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(81, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 2, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalSpacer_7 = new QSpacerItem(20, 147, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_7);

        footer = new QLabel(loginwindow);
        footer->setObjectName("footer");
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(footer->sizePolicy().hasHeightForWidth());
        footer->setSizePolicy(sizePolicy1);
        footer->setStyleSheet(QString::fromUtf8("#footer{\n"
"	text-decoration: underline;\n"
"color: #FFF;\n"
"\n"
"font-size: 12px;\n"
"font-style: normal;\n"
"font-weight: 400;\n"
"line-height: normal;\n"
"}"));
        footer->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(footer);

        label = new QLabel(loginwindow);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);


        gridLayout->addLayout(verticalLayout_2, 15, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 71, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 4, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(82, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 19, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_8, 11, 1, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 24, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_6, 16, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_4, 9, 1, 1, 1);

        lineEdit_Password = new QLineEdit(loginwindow);
        lineEdit_Password->setObjectName("lineEdit_Password");
        sizePolicy.setHeightForWidth(lineEdit_Password->sizePolicy().hasHeightForWidth());
        lineEdit_Password->setSizePolicy(sizePolicy);
        lineEdit_Password->setMinimumSize(QSize(310, 40));
        lineEdit_Password->setMaximumSize(QSize(310, 40));
        lineEdit_Password->setStyleSheet(QString::fromUtf8("#lineEdit_Password{\n"
"border-radius: 6px;\n"
"border: 1px solid #ADB5BD;\n"
"background: #D9D9D9;\n"
"padding: 4px 10px 4px 10px;\n"
"}"));
        lineEdit_Password->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_Password, 7, 1, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(20);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(-1, -1, -1, 9);
        title_bar = new QWidget(loginwindow);
        title_bar->setObjectName("title_bar");
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(title_bar->sizePolicy().hasHeightForWidth());
        title_bar->setSizePolicy(sizePolicy2);
        title_bar->setMinimumSize(QSize(0, 29));

        horizontalLayout_6->addWidget(title_bar);

        minimize = new QToolButton(loginwindow);
        minimize->setObjectName("minimize");
        minimize->setMaximumSize(QSize(14, 14));
        minimize->setStyleSheet(QString::fromUtf8("#minimize{\n"
"background: transparent;\n"
"}"));

        horizontalLayout_6->addWidget(minimize);

        close = new QToolButton(loginwindow);
        close->setObjectName("close");
        close->setMaximumSize(QSize(14, 14));
        close->setStyleSheet(QString::fromUtf8("#close{\n"
"background: transparent;\n"
"}"));

        horizontalLayout_6->addWidget(close);


        gridLayout->addLayout(horizontalLayout_6, 0, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 70, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);

        progressBar = new QProgressBar(loginwindow);
        progressBar->setObjectName("progressBar");
        progressBar->setMinimumSize(QSize(0, 10));
        progressBar->setMaximumSize(QSize(16777215, 10));
        progressBar->setStyleSheet(QString::fromUtf8("#progressBar {\n"
"    border-radius: 5px;\n"
"   background: #D9D9D9;\n"
"\n"
"}\n"
"#progressBar::chunk {\n"
"    border-radius: 5px;\n"
"background: #212529;\n"
"}"));
        progressBar->setTextVisible(false);

        gridLayout->addWidget(progressBar, 8, 1, 1, 1);

        pushButtonlogin = new QPushButton(loginwindow);
        pushButtonlogin->setObjectName("pushButtonlogin");
        sizePolicy1.setHeightForWidth(pushButtonlogin->sizePolicy().hasHeightForWidth());
        pushButtonlogin->setSizePolicy(sizePolicy1);
        pushButtonlogin->setMinimumSize(QSize(310, 40));
        pushButtonlogin->setMaximumSize(QSize(310, 40));
        pushButtonlogin->setStyleSheet(QString::fromUtf8("#pushButtonlogin{\n"
"border-radius: 4px;\n"
"background: #212529;\n"
"box-shadow: 0px 2px 8px 0px #2D3134;\n"
"\n"
"    font-style: normal;\n"
"    font-weight: 400;\n"
"    font-size: 18px;\n"
"    color: #FFF;\n"
"}"));
        pushButtonlogin->setIconSize(QSize(48, 21));
        pushButtonlogin->setCheckable(false);
        pushButtonlogin->setAutoDefault(true);

        gridLayout->addWidget(pushButtonlogin, 12, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        logo = new QLabel(loginwindow);
        logo->setObjectName("logo");
        sizePolicy.setHeightForWidth(logo->sizePolicy().hasHeightForWidth());
        logo->setSizePolicy(sizePolicy);
        logo->setMinimumSize(QSize(28, 29));
        logo->setMaximumSize(QSize(28, 29));

        horizontalLayout->addWidget(logo);

        name = new QLabel(loginwindow);
        name->setObjectName("name");
        sizePolicy.setHeightForWidth(name->sizePolicy().hasHeightForWidth());
        name->setSizePolicy(sizePolicy);
        name->setMinimumSize(QSize(231, 23));
        name->setMaximumSize(QSize(231, 23));
        name->setStyleSheet(QString::fromUtf8("#name{\n"
"width: 231.618px;\n"
"height: 23.086px;\n"
"flex-shrink: 0;\n"
"fill: #E8E8E8;\n"
"}"));

        horizontalLayout->addWidget(name);


        gridLayout->addLayout(horizontalLayout, 3, 1, 1, 1);

        QWidget::setTabOrder(lineEditUsername, lineEdit_Password);
        QWidget::setTabOrder(lineEdit_Password, pushButtonlogin);
        QWidget::setTabOrder(pushButtonlogin, close);
        QWidget::setTabOrder(close, minimize);

        retranslateUi(loginwindow);

        QMetaObject::connectSlotsByName(loginwindow);
    } // setupUi

    void retranslateUi(QWidget *loginwindow)
    {
        loginwindow->setWindowTitle(QCoreApplication::translate("loginwindow", "Login", nullptr));
        label_forgot->setText(QCoreApplication::translate("loginwindow", "Forgot Password?", nullptr));
        label_contact->setText(QCoreApplication::translate("loginwindow", "Contact Support", nullptr));
        label_wron_input->setText(QString());
        lineEditUsername->setText(QString());
        lineEditUsername->setPlaceholderText(QCoreApplication::translate("loginwindow", "Username", nullptr));
        footer->setText(QCoreApplication::translate("loginwindow", "ALGO METHODS SOFTWARE & SERVICES PVT LTD", nullptr));
        label->setText(QCoreApplication::translate("loginwindow", "1.1 20-05-2024_11_00_PM", nullptr));
        lineEdit_Password->setText(QString());
        lineEdit_Password->setPlaceholderText(QCoreApplication::translate("loginwindow", "Password", nullptr));
        minimize->setText(QCoreApplication::translate("loginwindow", "_", nullptr));
        close->setText(QString());
        pushButtonlogin->setText(QCoreApplication::translate("loginwindow", "Login", nullptr));
        logo->setText(QString());
        name->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class loginwindow: public Ui_loginwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
