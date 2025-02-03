#ifndef OPEN_POSITION_DELEGATE_H
#define OPEN_POSITION_DELEGATE_H

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
#include "OpenPosition/open_position_model.h"


class open_position_delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit open_position_delegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
     QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

//protected:
//    bool eventFilter(QObject *object, QEvent *event) override;
private:

 //   QString m_highlightText;
    double price_diff_incrementer = 0.05;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;
//    QColor getOrderStateColor(int orderState) const;

};

#endif // OPEN_POSITION_DELEGATE_H
