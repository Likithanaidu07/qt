#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QStyledItemDelegate>

class CustomDelegate : public QItemDelegate
{
public:

    explicit CustomDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override ;
};

#endif // CUSTOMDELEGATE_H
