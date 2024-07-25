#ifndef LINERS_DELEGATE_H
#define LINERS_DELEGATE_H

#include <QObject>
#include <QPalette>
#include <QStyleOptionViewItem>
#include <QColor>
#include <QDateTimeEdit>
#include <QContextMenuEvent>
#include <QMenu>
#include <QStyledItemDelegate>
#include <QPen>
#include <QPainter>
#include <QTableView>
#include <QAbstractTableModel>
#include "Liners/liners_model.h"
#include "Liners/linersheaderview.h"

class liners_delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
   explicit liners_delegate(QObject *parent = nullptr);
   void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    double price_diff_incrementer = 0.05;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;
};

#endif // LINERS_DELEGATE_H
