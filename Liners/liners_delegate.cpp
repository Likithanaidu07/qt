#include "liners_delegate.h"

liners_delegate::liners_delegate(QObject *parent)  : QStyledItemDelegate{parent}
{
}

void liners_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto c = index.column();
    auto r = index.row();
    QStyleOptionViewItem op(option);
    op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
    op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    Liners_Model *model=(Liners_Model*) index.model();
    QStringList order_list=model->liners_data_list.at(index.row());

    QColor color;
    int x_add = 1;
    if (c == Liners_Idx::BuyAvgPrice || c == Liners_Idx::BuyQtyinLots)
    {

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
    else if (c == Liners_Idx::SellAvgPrice || c == Liners_Idx::SellQtyinLots)
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

