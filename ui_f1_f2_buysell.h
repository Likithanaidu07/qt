/********************************************************************************
** Form generated from reading UI file 'f1_f2_buysell.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_F1_F2_BUYSELL_H
#define UI_F1_F2_BUYSELL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_F1_F2_BuySell
{
public:
    QGridLayout *gridLayout;
    QComboBox *comboBoxBuySell;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_3;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *lineEdit_Stockname;
    QLabel *label_4;
    QPushButton *pushButtonSubmit;
    QDoubleSpinBox *doubleSpinBox_price;
    QLabel *label_2;
    QSpinBox *spinBoxLot;

    void setupUi(QDialog *F1_F2_BuySell)
    {
        if (F1_F2_BuySell->objectName().isEmpty())
            F1_F2_BuySell->setObjectName("F1_F2_BuySell");
        F1_F2_BuySell->setWindowModality(Qt::ApplicationModal);
        F1_F2_BuySell->resize(680, 100);
        F1_F2_BuySell->setMinimumSize(QSize(680, 100));
        F1_F2_BuySell->setMaximumSize(QSize(680, 100));
        gridLayout = new QGridLayout(F1_F2_BuySell);
        gridLayout->setObjectName("gridLayout");
        comboBoxBuySell = new QComboBox(F1_F2_BuySell);
        comboBoxBuySell->addItem(QString());
        comboBoxBuySell->addItem(QString());
        comboBoxBuySell->setObjectName("comboBoxBuySell");
        comboBoxBuySell->setMinimumSize(QSize(60, 28));

        gridLayout->addWidget(comboBoxBuySell, 2, 1, 1, 1);

        label = new QLabel(F1_F2_BuySell);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 2, 8, 1, 1);

        label_3 = new QLabel(F1_F2_BuySell);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 1, 3, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 2, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 2, 1, 1);

        lineEdit_Stockname = new QLineEdit(F1_F2_BuySell);
        lineEdit_Stockname->setObjectName("lineEdit_Stockname");
        lineEdit_Stockname->setMinimumSize(QSize(200, 28));

        gridLayout->addWidget(lineEdit_Stockname, 2, 2, 1, 1);

        label_4 = new QLabel(F1_F2_BuySell);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 1, 5, 1, 1);

        pushButtonSubmit = new QPushButton(F1_F2_BuySell);
        pushButtonSubmit->setObjectName("pushButtonSubmit");
        pushButtonSubmit->setMinimumSize(QSize(90, 28));
        pushButtonSubmit->setMaximumSize(QSize(90, 28));

        gridLayout->addWidget(pushButtonSubmit, 2, 7, 1, 1);

        doubleSpinBox_price = new QDoubleSpinBox(F1_F2_BuySell);
        doubleSpinBox_price->setObjectName("doubleSpinBox_price");
        doubleSpinBox_price->setMinimumSize(QSize(100, 0));
        doubleSpinBox_price->setMaximum(9999.989999999999782);
        doubleSpinBox_price->setStepType(QAbstractSpinBox::DefaultStepType);

        gridLayout->addWidget(doubleSpinBox_price, 2, 3, 1, 2);

        label_2 = new QLabel(F1_F2_BuySell);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 2, 1, 1);

        spinBoxLot = new QSpinBox(F1_F2_BuySell);
        spinBoxLot->setObjectName("spinBoxLot");
        spinBoxLot->setMinimumSize(QSize(80, 0));
        spinBoxLot->setMaximum(99999999);

        gridLayout->addWidget(spinBoxLot, 2, 5, 1, 2);

        QWidget::setTabOrder(comboBoxBuySell, lineEdit_Stockname);
        QWidget::setTabOrder(lineEdit_Stockname, pushButtonSubmit);

        retranslateUi(F1_F2_BuySell);

        QMetaObject::connectSlotsByName(F1_F2_BuySell);
    } // setupUi

    void retranslateUi(QDialog *F1_F2_BuySell)
    {
        F1_F2_BuySell->setWindowTitle(QCoreApplication::translate("F1_F2_BuySell", "Manual\302\240Orders", nullptr));
        comboBoxBuySell->setItemText(0, QCoreApplication::translate("F1_F2_BuySell", "Buy", nullptr));
        comboBoxBuySell->setItemText(1, QCoreApplication::translate("F1_F2_BuySell", "Sell", nullptr));

        label->setText(QCoreApplication::translate("F1_F2_BuySell", "Buy Sell", nullptr));
        label_3->setText(QCoreApplication::translate("F1_F2_BuySell", "Price", nullptr));
        label_4->setText(QCoreApplication::translate("F1_F2_BuySell", "No. of Lots", nullptr));
        pushButtonSubmit->setText(QCoreApplication::translate("F1_F2_BuySell", "Submit", nullptr));
        label_2->setText(QCoreApplication::translate("F1_F2_BuySell", "Stockname", nullptr));
    } // retranslateUi

};

namespace Ui {
    class F1_F2_BuySell: public Ui_F1_F2_BuySell {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_F1_F2_BUYSELL_H
