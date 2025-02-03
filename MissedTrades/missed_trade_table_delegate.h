#ifndef MISSED_TRADE_TABLE_DELEGATE_H
#define MISSED_TRADE_TABLE_DELEGATE_H

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
#include "MissedTrades/missed_trade_table_model.h"
#include "MissedTrades/missed_trades_headerview.h"


    class missed_trade_table_delegate : public QStyledItemDelegate
    {
        Q_OBJECT
    public:
        explicit missed_trade_table_delegate(QObject *parent = nullptr);
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    private:

        //   QString m_highlightText;
        double price_diff_incrementer = 0.05;
        double quantity_incrementer = 1.0;
        QString market_type;
        QPen pen;
        QTableView* view;
        //    QColor getOrderStateColor(int orderState) const;

    };

#endif // MISSED_TRADE_TABLE_DELEGATE_H
