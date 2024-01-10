#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include "SwitchButton/switchbutton.h"



class CustomDelegate : public QItemDelegate
{
public:
    explicit CustomDelegate(QObject *parent = nullptr) : QItemDelegate(parent)

    {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)

        const

        override

    {
        if (index.column() == 0) {
            // Create your custom widget instance here
            SwitchButton *widget = new SwitchButton(parent);
            // Set any initial properties of the widget
            // ...

            // Connect the widget's signals to your desired slots
            // ...

            return widget;
        } else {
            return QItemDelegate::createEditor(parent, option, index);
        }
    }
};

#endif // CUSTOMDELEGATE_H
