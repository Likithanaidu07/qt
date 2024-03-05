#include "customDelegate.h"
#include "SwitchButton/switchbutton.h"

CustomDelegate::CustomDelegate(QObject *parent): QItemDelegate(parent)
{

}

QWidget *CustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)
    const
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
