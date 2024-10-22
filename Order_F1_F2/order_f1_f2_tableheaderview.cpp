#include "order_f1_f2_tableheaderview.h"

#include <QApplication>
#include "QPainter"
#include "defines.h"
#include "QComboBox"



order_f1_f2_tableheaderview::order_f1_f2_tableheaderview(Qt::Orientation orientation, QWidget* parent) : QHeaderView(orientation, parent) {
    setSectionsMovable(true);
    setSectionsClickable(true);

}

void order_f1_f2_tableheaderview::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const {
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
    /*if (logicalIndex == F1F2_Order_Data_Idx::AlgoNo_F1F2)

    {
        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }
    else*/ if(logicalIndex == F1F2_Order_Data_Idx::AlgoName_F1F2)
    {
         //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }
    else if(logicalIndex == F1F2_Order_Data_Idx::ExchPrice_F1F2||
             logicalIndex == F1F2_Order_Data_Idx::UserPrice_F1F2 ||
             logicalIndex == F1F2_Order_Data_Idx::TradedLot_F1F2 /*||
             logicalIndex == F1F2_Order_Data_Idx::RemainingLot_F1F2*/)
    {
        //        painter->fillRect(rect, QColor::fromRgba(qRgba(108,117,125,255)));
    }

    else if(logicalIndex == F1F2_Order_Data_Idx::TradeTime_F1F2||
             logicalIndex == F1F2_Order_Data_Idx::BidLegState_F1F2||
             logicalIndex == F1F2_Order_Data_Idx::Leg1State_F1F2 ||
             logicalIndex == F1F2_Order_Data_Idx::Leg3State_F1F2 ||
             logicalIndex == F1F2_Order_Data_Idx::Leg4State_F1F2)
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
    if(logicalIndex == F1F2_Order_Data_Idx::AlgoName_F1F2
        || logicalIndex == F1F2_Order_Data_Idx::Leg4State_F1F2
        /*|| logicalIndex == F1F2_Order_Data_Idx::RemainingLot_F1F2*/){
        //painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
    else
    {
        painter->setPen(QColor("#6C757D"));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }
 }

