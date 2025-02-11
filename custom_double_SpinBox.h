#ifndef CUSTOM_DOUBLE_SPINBOX_H
#define CUSTOM_DOUBLE_SPINBOX_H
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QKeyEvent>

    class CustomDoubleSpinBox : public QDoubleSpinBox
    {
        Q_OBJECT

    public:
        CustomDoubleSpinBox(QWidget *parent = nullptr) : QDoubleSpinBox(parent)
        {
            // Set the minimum value (for example, 1.0)
            setMinimum(1.0);
        }

    protected:
        // Override the validate method to check for invalid input
        QValidator::State validate(QString &input, int &pos) const override
        {
            bool ok;
            double value = input.toDouble(&ok);

            if (ok && value == 0.0)
            {
                // Return Intermediate state to allow showing a popup
                return QValidator::Intermediate;
            }

            // Otherwise, use the default validation
            return QDoubleSpinBox::validate(input, pos);
        }

        // Override the fixup method to handle invalid input
        void fixup(QString &input) const override
        {
            bool ok;
            double value = input.toDouble(&ok);

            if (ok && value <= 0.0)
            {
                // Show a popup message
                QMessageBox::warning(const_cast<QWidget*>(this->parentWidget()),
                                     "Invalid Price",
                                     "The Price cannot be 0. Resetting to minimum Price value.");

                // Reset the value to the minimum allowed value
                input = QString::number(minimum());
            }
        }
    };

#endif // CUSTOM_DOUBLE_SPINBOX_H
