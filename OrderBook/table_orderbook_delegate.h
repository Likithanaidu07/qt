#ifndef TABLE_ORDERBOOK_DELEGATE_H
#define TABLE_ORDERBOOK_DELEGATE_H

#include <QObject>
#include "QPalette"
#include "QStyleOptionViewItem"
#include "QColor"
#include <QDateTimeEdit>
#include <QContextMenuEvent>
#include <QMenu>
#include "QStyledItemDelegate"
#include "qpen.h"
#include <QAbstractTableModel>
#include <QPainter>


#include "defines.h"
#include "TradePosition/OrderBook_Object.h"
#include "TradePosition/trade_table_model.h"


class Table_OrderBook_Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
   explicit Table_OrderBook_Delegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
//   QList<OrderBookObject*> OrderBook_Data_List;
};

#endif // TABLE_ORDERBOOK_DELEGATE_H
