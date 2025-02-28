#ifndef NET_POSITION_TABLE_DELEGATE_H
#define NET_POSITION_TABLE_DELEGATE_H


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
#include "qtableview.h"
#include "defines.h"
#include "NetPosition/net_position_table_model.h"


class net_position_table_delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit net_position_table_delegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //   QList<OrderBookObject*> OrderBook_Data_List;
      void setHighlightText(const QString &text);
private:

 QString m_highlightText;
    double price_diff_incrementer = 0.01;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;
};

#endif // NET_POSITION_TABLE_DELEGATE_H
