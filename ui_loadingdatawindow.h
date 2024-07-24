/********************************************************************************
** Form generated from reading UI file 'loadingdatawindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADINGDATAWINDOW_H
#define UI_LOADINGDATAWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loadingdatawindow
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QProgressBar *progressBar;
    QSpacerItem *verticalSpacer_2;
    QLabel *label;

    void setupUi(QWidget *loadingdatawindow)
    {
        if (loadingdatawindow->objectName().isEmpty())
            loadingdatawindow->setObjectName("loadingdatawindow");
        loadingdatawindow->resize(453, 185);
        loadingdatawindow->setStyleSheet(QString::fromUtf8("  border: 1px solid grey;\n"
"  border-radius: 1px;\n"
"  background-color: #6C757D;\n"
""));
        horizontalLayout = new QHBoxLayout(loadingdatawindow);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        progressBar = new QProgressBar(loadingdatawindow);
        progressBar->setObjectName("progressBar");
        progressBar->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        progressBar->setMaximum(0);
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label = new QLabel(loadingdatawindow);
        label->setObjectName("label");

        verticalLayout->addWidget(label);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(loadingdatawindow);

        QMetaObject::connectSlotsByName(loadingdatawindow);
    } // setupUi

    void retranslateUi(QWidget *loadingdatawindow)
    {
        loadingdatawindow->setWindowTitle(QCoreApplication::translate("loadingdatawindow", "Form", nullptr));
        label->setText(QCoreApplication::translate("loadingdatawindow", "Loading Data...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loadingdatawindow: public Ui_loadingdatawindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADINGDATAWINDOW_H
