#include "portfolioheaderview.h"
//#include "PortFolio/portfolioheaderdelegate.h"
#include <QApplication>
#include "QPainter"
#include "defines.h"

PortfolioHeaderView::PortfolioHeaderView(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {

    setSectionsMovable(true);

}

void PortfolioHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
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
    if (logicalIndex == PortfolioData_Idx::_Status||
        logicalIndex == PortfolioData_Idx::_PortfolioNumber||
        logicalIndex == PortfolioData_Idx::_AlgoName)
    {
//        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }
    else if(logicalIndex == PortfolioData_Idx::_BuyMarketRate||
             logicalIndex == PortfolioData_Idx::_BuyAveragePrice ||
             logicalIndex == PortfolioData_Idx::_BuyPriceDifference ||
             logicalIndex == PortfolioData_Idx::_BuyTotalQuantity ||
             logicalIndex == PortfolioData_Idx::_BuyTradedQuantity ||
             logicalIndex == PortfolioData_Idx::_BuyRemainingQuantity)
    {
//        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }

    else if(logicalIndex == PortfolioData_Idx::_SellMarketRate||
             logicalIndex == PortfolioData_Idx::_SellAveragePrice||
             logicalIndex == PortfolioData_Idx::_SellPriceDifference||
             logicalIndex == PortfolioData_Idx::_SellTotalQuantity||
             logicalIndex == PortfolioData_Idx::_SellTradedQuantity||
             logicalIndex == PortfolioData_Idx::_SellRemainingQuantity)
    {
//        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }

    else if(logicalIndex == PortfolioData_Idx::_ExpiryDateTime||
             logicalIndex == PortfolioData_Idx::_Leg1||
             logicalIndex == PortfolioData_Idx::_Cost)
    {
//        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }


    // Draw the header text

    painter->drawText(rect, align, model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString());

    // Add a right-side border
//    painter->setPen(QColor::fromRgba(qRgba(50, 50, 50, 53))); // Set the border color
    if(logicalIndex == PortfolioData_Idx::_Alias
        ||logicalIndex ==  PortfolioData_Idx:: _BuyRemainingQuantity
        || logicalIndex == PortfolioData_Idx::_SellRemainingQuantity){
        //painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
    else
    {
        painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }


}

