#include "table_orderbook_delegate.h"

Table_OrderBook_Delegate::Table_OrderBook_Delegate(QObject *parent)  : QStyledItemDelegate{parent}
{

}
bool Table_OrderBook_Delegate::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::FocusOut) {
        auto view = qobject_cast<QTableView*>(object->parent());
        if (view) {
            view->clearSelection();
        }
    }
    return QStyledItemDelegate::eventFilter(object, event);
}
void Table_OrderBook_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    auto c= index.column();
    auto r= index.row();
    QStyleOptionViewItem op(option);
    op.palette.setColor(QPalette::All, QPalette::Window , Qt::red);
    op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    Trade_Table_Model *model=(Trade_Table_Model*) index.model();
    QStringList order_list=model->trade_data_list.at(index.row());
    //    QStringList order_list;
    QList<QString> fifthColumnValues;

    /*for (int i = 0; i < model->trade_data_list.size(); ++i) {
            QString fifthColumnValue = order_list.at(7);
            fifthColumnValues.append(fifthColumnValue);
    }*/
    if(order_list.at(7)=="Sell"){

    }
    else{

    }
    if ( c==OrderBook_Idx::ExchPrice_OB ||
        c==OrderBook_Idx::UserPrice_OB ||
        c==OrderBook_Idx::Jackpot_OB ||
        c==OrderBook_Idx::TradedLot_OB ||
        c==OrderBook_Idx::RemainingLot_OB )
    {

        Trade_Table_Model *model=(Trade_Table_Model*) index.model();
        QStringList order_list=model->trade_data_list.at(index.row());
        QString buy_sell = order_list[OrderBook_Idx::BuyorSell_OB];

        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight , color);

        if(buy_sell == "Sell"){
            QColor color("#FED9D9");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);

            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-5);
            p2.setX(p2.x()+5);
            painter->drawLine(p1,p2);
        }
        else if(buy_sell == "Buy")
        {
            QColor color("#D6FCF0");
            op.palette.setColor(QPalette::Highlight , Qt::transparent);
            op.palette.setColor(QPalette::HighlightedText , Qt::black);
            painter->fillRect(option.rect, color);

            double borderWidth = 0.5;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
        }

        QStyledItemDelegate::paint(painter, op, index);
    }
    else{
        QColor color("#E0F1FF");
        //       // painter->fillRect(option.rect, color);
        //        op.palette.setColor(QPalette::Highlight , Qt::transparent);
        //        op.palette.setColor(QPalette::HighlightedText , Qt::black);
        painter->fillRect(option.rect, color);
    }
    if(c==OrderBook_Idx::TradeTime_OB || c==OrderBook_Idx::AlgoNo_OB|| c==OrderBook_Idx::UserPrice_OB){
        QStyleOptionViewItem op(option);


        // if(c==OrderBook_Idx::BidLegState_OB){
        double borderWidth = 1;
        QColor myColor(108, 117, 125);
        QPen pen(myColor);
        pen.setWidthF(borderWidth);
        painter->setPen(pen);
        painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());

    }
    //        else if (c==OrderBook_Idx::BidLegState_OB){
    //            double borderWidth = 1;
    //            QColor myColor(108, 117, 125);
    //            QPen pen(myColor);
    //            pen.setWidthF(borderWidth);
    //            painter->setPen(pen);
    //            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
    //        }


    //    }
    double borderWidth = 0.5;
    QColor myColor(108, 117, 125);
    QPen pen(myColor);
    pen.setWidthF(borderWidth);
    painter->setPen(pen);
    QPoint p1= option.rect.bottomLeft();
    QPoint p2= option.rect.bottomRight();
    p1.setX(p1.x()-5);
    p2.setX(p2.x()+5);


    painter->drawLine(p1,p2);

    painter->setPen(pen);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    QStyledItemDelegate::paint(painter, op, index);
}
