#include "net_position_table_headerview.h"
#include <QApplication>
#include "QPainter"
#include "defines.h"


net_position_table_headerview::net_position_table_headerview(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {
    setSectionsMovable(true);
}

void net_position_table_headerview::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {


    Qt::Alignment align = (Qt::AlignHCenter | Qt::AlignVCenter);


    painter->drawText(rect, align, model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString());

    // Add a right-side border
    painter->setPen(QColor::fromRgba(qRgba(50, 50, 50, 53))); // Set the border color
    if(logicalIndex == NET_POS::BuyAvgPrice_NP
        ||logicalIndex ==  NET_POS:: BuyTotalLot_NP
        || logicalIndex == NET_POS::BuyValue_NP){
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
