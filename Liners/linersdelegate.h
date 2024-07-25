#ifndef LINERSDELEGATE_H
#define LINERSDELEGATE_H

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
#include "Liners/liners_model.h"
#include "Liners/linersheaderview.h"

class linersdelegate : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit linersdelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    //   QList<OrderBookObject*> OrderBook_Data_List;
private:


    double price_diff_incrementer = 0.05;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;
};


#endif // LINERSDELEGATE_H
