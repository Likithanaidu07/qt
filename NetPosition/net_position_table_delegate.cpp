#include "net_position_table_delegate.h"

net_position_table_delegate::net_position_table_delegate(QObject *parent)  : QStyledItemDelegate{parent}
{
}
void net_position_table_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto c = index.column();
    auto r = index.row();
    QStyleOptionViewItem op(option);
    op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
    op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    Net_Position_Table_Model *model=(Net_Position_Table_Model*) index.model();
    QStringList order_list=model->net_pos_data_list.at(index.row());

    QColor color;
    int x_add=1;
    if (c == NET_POS::BuyAvgPrice_NP || c == NET_POS::BuyTotalLot_NP ||c== NET_POS::BuyValue_NP)
    {
        Net_Position_Table_Model *model=(Net_Position_Table_Model*) index.model();
        QStringList order_list = model->net_pos_data_list.at(index.row());
        color = QColor("#42A5F5");
        op.palette.setColor(QPalette::Highlight, color);

        color = QColor("#D6FCF0");
        op.palette.setColor(QPalette::Highlight, Qt::transparent);
        op.palette.setColor(QPalette::HighlightedText, Qt::black);
        painter->fillRect(option.rect, color);

        double borderWidth = 0.5;
        QColor myColor(108, 117, 125);
        QPen pen(myColor);
        pen.setWidthF(borderWidth);
        painter->setPen(pen);

        QPoint p1 = option.rect.bottomLeft();
        QPoint p2 = option.rect.bottomRight();
        p1.setX(p1.x() - x_add);
        p2.setX(p2.x() + x_add);
        painter->drawLine(p1, p2);
    }
    else if (c== NET_POS::SellAvgPrice_NP || c == NET_POS::SellTotalLot_NP ||c== NET_POS::SellValue_NP)
    {
        color = QColor("#FED9D9");
        op.palette.setColor(QPalette::Highlight, Qt::transparent);
        op.palette.setColor(QPalette::HighlightedText, Qt::black);
        painter->fillRect(option.rect, color);

        double borderWidth = 0.5;
        QColor myColor(108, 117, 125);
        QPen pen(myColor);
        pen.setWidthF(borderWidth);
        painter->setPen(pen);
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    }
    else{
        QColor color("#E0F1FF");
        //       // painter->fillRect(option.rect, color);
        //        op.palette.setColor(QPalette::Highlight , Qt::transparent);
        //        op.palette.setColor(QPalette::HighlightedText , Qt::black);

        QColor HighlightColor("#42A5F5");
        if(option.state & QStyle::State_Active)
            op.palette.setColor(QPalette::Highlight, HighlightColor);
        else
            op.palette.setColor(QPalette::Highlight, color);

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, HighlightColor);
        else
           painter->fillRect(option.rect, color);
    }

    QStyledItemDelegate::paint(painter, op, index);
    double borderWidth = 0.5;
    QColor myColor(108, 117, 125);
    QPen pen(myColor);
    pen.setWidthF(borderWidth);
    painter->setPen(pen);
    QPoint p1= option.rect.bottomLeft();
    QPoint p2= option.rect.bottomRight();
    p1.setX(p1.x()-x_add);
    p2.setX(p2.x()+x_add);


    painter->drawLine(p1,p2);

    painter->setPen(pen);
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
}

