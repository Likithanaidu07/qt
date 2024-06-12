#include "trade_table_delegate.h"
#include <QLineEdit>
#include "QApplication"
#include "SwitchButton/switchbutton.h"
#include "QPainter"
#include "defines.h"
#include "QSettings"
#include "QDir"
#include <QValidator>
#include <QStandardPaths>
#include <QCheckBox>

trade_table_delegate::trade_table_delegate()
{

}
QSize trade_table_delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Get the text from the model
    QString text = index.data(Qt::DisplayRole).toString();

    // Calculate the size needed for the cell based on the content
    QFontMetrics fm(option.font);
    QSize size = fm.size(0, text);

    // Optionally, add some padding
    size += QSize(10, 10);

    return size;
}
void trade_table_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto c= index.column();
    auto r= index.row();
    QStyleOptionViewItem op(option);
    if(c==OrderBook_Idx::BidLegState_OB || c==OrderBook_Idx::Leg1State_OB || c==OrderBook_Idx::Leg3State_OB ){
        QStyleOptionViewItem op(option);


        if(c==OrderBook_Idx::BidLegState_OB){
            double borderWidth = 1;
            QColor myColor(108, 117, 125);
            QPen pen(myColor);
            pen.setWidthF(borderWidth);
            painter->setPen(pen);
            painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
        }

}
}

