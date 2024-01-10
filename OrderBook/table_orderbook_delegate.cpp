#include "table_orderbook_delegate.h"

Table_OrderBook_Delegate::Table_OrderBook_Delegate(QObject *parent)  : QStyledItemDelegate{parent}
{

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


    QColor color("#E0F1FF");
    if(c==0){
        painter->fillRect(option.rect, color);
    }
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
