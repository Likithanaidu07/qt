/********************************************************************************
** Form generated from reading UI file 'f1_f2_buysell.ui'
**
** Created by: Qt User Interface Compiler version 6.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F1_F2_BUYSELL_H
#define UI_F1_F2_BUYSELL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_F1_F2_BuySell
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_Lots;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_4;
    QSpacerItem *verticalSpacer_2;
    QComboBox *comboBoxBuySell;
    QLineEdit *lineEdit_Price;
    QLabel *label;
    QPushButton *pushButtonSubmit;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QLineEdit *lineEdit_Stockname;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QDialog *F1_F2_BuySell)
    {
        if (F1_F2_BuySell->objectName().isEmpty())
            F1_F2_BuySell->setObjectName(QString::fromUtf8("F1_F2_BuySell"));
        F1_F2_BuySell->setWindowModality(Qt::ApplicationModal);
        F1_F2_BuySell->resize(680, 100);
        F1_F2_BuySell->setMinimumSize(QSize(680, 100));
        F1_F2_BuySell->setMaximumSize(QSize(680, 100));
        gridLayout = new QGridLayout(F1_F2_BuySell);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(F1_F2_BuySell);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 2, 1, 1);

        lineEdit_Lots = new QLineEdit(F1_F2_BuySell);
        lineEdit_Lots->setObjectName(QString::fromUtf8("lineEdit_Lots"));
        lineEdit_Lots->setMinimumSize(QSize(0, 28));
        lineEdit_Lots->setMaximumSize(QSize(70, 16777215));

        gridLayout->addWidget(lineEdit_Lots, 2, 4, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 2, 0, 1, 1);

        label_4 = new QLabel(F1_F2_BuySell);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 4, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 2, 1, 1);

        comboBoxBuySell = new QComboBox(F1_F2_BuySell);
        comboBoxBuySell->addItem(QString());
        comboBoxBuySell->addItem(QString());
        comboBoxBuySell->setObjectName(QString::fromUtf8("comboBoxBuySell"));
        comboBoxBuySell->setMinimumSize(QSize(60, 28));

        gridLayout->addWidget(comboBoxBuySell, 2, 1, 1, 1);

        lineEdit_Price = new QLineEdit(F1_F2_BuySell);
        lineEdit_Price->setObjectName(QString::fromUtf8("lineEdit_Price"));
        lineEdit_Price->setMinimumSize(QSize(0, 28));
        lineEdit_Price->setMaximumSize(QSize(110, 16777215));

        gridLayout->addWidget(lineEdit_Price, 2, 3, 1, 1);

        label = new QLabel(F1_F2_BuySell);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        pushButtonSubmit = new QPushButton(F1_F2_BuySell);
        pushButtonSubmit->setObjectName(QString::fromUtf8("pushButtonSubmit"));
        pushButtonSubmit->setMinimumSize(QSize(90, 28));
        pushButtonSubmit->setMaximumSize(QSize(90, 28));

        gridLayout->addWidget(pushButtonSubmit, 2, 5, 1, 1);

        label_3 = new QLabel(F1_F2_BuySell);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 2, 1, 1);

        lineEdit_Stockname = new QLineEdit(F1_F2_BuySell);
        lineEdit_Stockname->setObjectName(QString::fromUtf8("lineEdit_Stockname"));
        lineEdit_Stockname->setMinimumSize(QSize(200, 28));

        gridLayout->addWidget(lineEdit_Stockname, 2, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 2, 6, 1, 1);

        QWidget::setTabOrder(comboBoxBuySell, lineEdit_Stockname);
        QWidget::setTabOrder(lineEdit_Stockname, lineEdit_Price);
        QWidget::setTabOrder(lineEdit_Price, lineEdit_Lots);
        QWidget::setTabOrder(lineEdit_Lots, pushButtonSubmit);

        retranslateUi(F1_F2_BuySell);

        QMetaObject::connectSlotsByName(F1_F2_BuySell);
    } // setupUi

    void retranslateUi(QDialog *F1_F2_BuySell)
    {
        F1_F2_BuySell->setWindowTitle(QCoreApplication::translate("F1_F2_BuySell", "F1 F2", nullptr));
        label_2->setText(QCoreApplication::translate("F1_F2_BuySell", "Stockname", nullptr));
        label_4->setText(QCoreApplication::translate("F1_F2_BuySell", "No. of Lots", nullptr));
        comboBoxBuySell->setItemText(0, QCoreApplication::translate("F1_F2_BuySell", "Buy", nullptr));
        comboBoxBuySell->setItemText(1, QCoreApplication::translate("F1_F2_BuySell", "Sell", nullptr));

        label->setText(QCoreApplication::translate("F1_F2_BuySell", "Buy Sell", nullptr));
        pushButtonSubmit->setText(QCoreApplication::translate("F1_F2_BuySell", "Submit", nullptr));
        label_3->setText(QCoreApplication::translate("F1_F2_BuySell", "Price", nullptr));
    } // retranslateUi

};

namespace Ui {
    class F1_F2_BuySell: public Ui_F1_F2_BuySell {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F1_F2_BUYSELL_H
