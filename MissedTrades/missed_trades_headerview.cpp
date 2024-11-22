#include "missed_trades_headerview.h"
#include <QApplication>
#include "QPainter"
#include "defines.h"


missed_trades_headerview::missed_trades_headerview(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {
    setSectionsMovable(true);
}

void missed_trades_headerview::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {


    Qt::Alignment align = (Qt::AlignHCenter | Qt::AlignVCenter);


    painter->drawText(rect, align, model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString());

    // Add a right-side border
    painter->setPen(QColor::fromRgba(qRgba(50, 50, 50, 53))); // Set the border color
    if(logicalIndex == Missed_Trades_Idx::AlgoNo
        ||logicalIndex ==  Missed_Trades_Idx::BuySell
        || logicalIndex == Missed_Trades_Idx::DateTime){
        //painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
    else
    {
        painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }

    painter->drawLine(rect.topRight(), rect.bottomRight());

}
