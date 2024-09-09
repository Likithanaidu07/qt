#include "linersheaderview.h"
#include <QApplication>
#include "QPainter"
#include "defines.h"

linersheaderview::linersheaderview(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {
    setSectionsMovable(true);
}

void linersheaderview::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
    // Create the portfolio header delegate
    /* PortfolioHeaderDelegate headerDelegate;

    // Set the custom header delegate for the header section
    headerDelegate.paint(painter, QStyleOptionViewItem(), QModelIndex());

    // Draw the section text
    QStyleOptionHeader opt;
    opt.rect = rect;
    opt.state = QStyle::State_Enabled;
    opt.textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    opt.text = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();

    QApplication::style()->drawControl(QStyle::CE_Header, &opt, painter);*/

    Qt::Alignment align = (Qt::AlignHCenter | Qt::AlignVCenter);
//    if (logicalIndex == Liners_Idx::AlgoId)

//    {
//        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
//    }
//    else if(logicalIndex == Liners_Idx::AlgoName)
//    {
//        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
//    }
//    else if(logicalIndex == Liners_Idx::BuyAvgPrice||
//             logicalIndex == Liners_Idx::BuyQtyinLots ||
//             logicalIndex == Liners_Idx::SellAvgPrice ||
//             logicalIndex == Liners_Idx::SellQtyinLots ||
//             logicalIndex == Liners_Idx::NetQty ||
//             logicalIndex == Liners_Idx::Profit)
//    {
//        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
//    }

//    else if(logicalIndex == OrderBook_Idx::BidLegState_OB||
//             logicalIndex == OrderBook_Idx::Leg1State_OB ||
//             logicalIndex == OrderBook_Idx::Leg3State_OB)
//    {
//        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
//    }

    //    else if(logicalIndex == PortfolioData_Idx::_ExpiryDateTime||
    //             logicalIndex == PortfolioData_Idx::_Leg1||
    //             logicalIndex == PortfolioData_Idx::_Cost)
    //    {
    //        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    //    }


    // Draw the header text

    painter->drawText(rect, align, model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString());

    // Add a right-side border
        painter->setPen(QColor::fromRgba(qRgba(50, 50, 50, 53))); // Set the border color
    if(logicalIndex == Liners_Idx::BuyAvgPrice
        ||logicalIndex ==  Liners_Idx:: SellAvgPrice
        || logicalIndex == Liners_Idx::NetQty){
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

