#include "tradetableheaderview.h"

#include <QApplication>
#include "QPainter"
#include "defines.h"
#include "QComboBox"


tradetableheaderview::tradetableheaderview(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {
    setSectionsMovable(true);
    setSectionsClickable(true);

}

void tradetableheaderview::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
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
    if (logicalIndex == OrderBook_Idx::AlgoNo_OB)

    {
        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }
    else if(logicalIndex == OrderBook_Idx::AlgoName_OB)
    {
         //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }
    else if(logicalIndex == OrderBook_Idx::ExchPrice_OB||
             logicalIndex == OrderBook_Idx::UserPrice_OB ||
             logicalIndex == OrderBook_Idx::Jackpot_OB ||
             logicalIndex == OrderBook_Idx::TradedLot_OB ||
             logicalIndex == OrderBook_Idx::RemainingLot_OB)
    {
        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }

    else if(logicalIndex == OrderBook_Idx::TradeTime_OB||
             logicalIndex == OrderBook_Idx::BidLegState_OB||
             logicalIndex == OrderBook_Idx::Leg1State_OB ||
             logicalIndex == OrderBook_Idx::Leg3State_OB ||
             logicalIndex == OrderBook_Idx::Leg4State_OB)
    {
        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }

//    else if(logicalIndex == PortfolioData_Idx::_ExpiryDateTime||
//             logicalIndex == PortfolioData_Idx::_Leg1||
//             logicalIndex == PortfolioData_Idx::_Cost)
//    {
//        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
//    }


    // Draw the header text

    painter->drawText(rect, align, model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString());

    // Add a right-side border
    //    painter->setPen(QColor::fromRgba(qRgba(50, 50, 50, 53))); // Set the border color
    if(logicalIndex == OrderBook_Idx::AlgoName_OB
        || logicalIndex == OrderBook_Idx::Leg4State_OB
        || logicalIndex == OrderBook_Idx::RemainingLot_OB){
        //painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
    else
    {
        painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
 }

