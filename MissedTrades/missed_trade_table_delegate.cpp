#include "missed_trade_table_delegate.h"
#include "qsortfilterproxymodel.h"
#include <QLineEdit>
#include <QPushButton>

#include "MissedTrades/missed_trade_table_model.h"

missed_trade_table_delegate::missed_trade_table_delegate(QObject *parent)  : QStyledItemDelegate{parent}
{

}

QWidget *missed_trade_table_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{

    if (index.column() == Missed_Trades_Idx::Retry_Order_Bt) {
        QVariant value = index.data(Qt::EditRole);
        if (value.toInt() == 0) {  // Show button only if value is 0
            QPushButton *editor = new QPushButton("Retry", parent);
            connect(editor, &QPushButton::clicked, [=]() {
                emit retryButtonClicked(index.row());
            });
            return editor;
        }
    }
}

bool missed_trade_table_delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {
    if (index.column() == Missed_Trades_Idx::Retry_Order_Bt) {
        QVariant value = index.data(Qt::DisplayRole);

        // Only allow click if value is 0
        if (value.toString() == "0" && event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (option.rect.contains(mouseEvent->pos())) {
                emit retryButtonClicked(index.row());
                return true;
            }
        }

        return false; // Ignore the event when value is not 0
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void missed_trade_table_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    auto c = index.column();
    auto r = index.row();
    QStyleOptionViewItem op(option);
    op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
    op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    Missed_Trade_Table_Model *model=(Missed_Trade_Table_Model*) index.model();
    QStringList order_list=model->missed_trade_data_list.at(index.row());

    QColor color;
    int x_add = 1;

    if (c == Missed_Trades_Idx::BuySell ||
        c == Missed_Trades_Idx::Lot ||
        c == Missed_Trades_Idx::Type ) {

        QString buy_sell = order_list[Missed_Trades_Idx::BuySell];

        // Default blue color for Buy/Sell highlight
        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight, color);

        if (buy_sell == "Sell") {
            QColor color("#FED9D9");  // Light red for Sell
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
        else if (buy_sell == "Buy") {
            QColor color("#D6FCF0");  // Light green for Buy
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
        QStyledItemDelegate::paint(painter, op, index);

    }

    /* else if (c == Open_Position_Idx::StockName ) {


        //QColor textColor(108, 117, 125);
        QColor bgColor("#E0F1FF");
        painter->fillRect(option.rect, bgColor);

        // Set the text color and draw the text
        painter->setPen(textColor);
        painter->drawText(option.rect, mappedIndex.data().toString(), QTextOption(Qt::AlignCenter));

        // Optionally call the base class method for default painting
        QStyledItemDelegate::paint(painter, option, mappedIndex);


        QPen pen(textColor);
        pen.setWidthF(0.5);
        painter->setPen(pen);
        QColor HighlightColor("#42A5F5");

        QPoint p1 = option.rect.bottomLeft();
        QPoint p2 = option.rect.bottomRight();
        p1.setX(p1.x() - x_add);
        p2.setX(p2.x() + x_add);
        painter->drawLine(p1, p2);


    }*/
    /*else if (index.column() == Missed_Trades_Idx::Retry_Order_Bt)
    {


        QPushButton button("Retry");
        button.setStyleSheet("background-color: #E0F1FF; border: 1px solid black;");
        op.palette.setColor(QPalette::Highlight, Qt::transparent);
        op.palette.setColor(QPalette::HighlightedText, Qt::black);
        button.resize(option.rect.size());

        QPixmap pixmap(option.rect.size());
        button.render(&pixmap);

        painter->drawPixmap(option.rect.topLeft(), pixmap);
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    }*/

      else if  (index.column() == Missed_Trades_Idx::Retry_Order_Bt) {
        QVariant value = index.data(Qt::DisplayRole);

        if (value.toString() == "0") {
            // Draw button-like appearance
            QPushButton button("Retry");
            button.setStyleSheet("background-color: #E0F1FF; border: 1px solid black;");
            button.resize(option.rect.size());

            QPixmap pixmap(option.rect.size());
            button.render(&pixmap);
            painter->drawPixmap(option.rect.topLeft(), pixmap);
        }
        else if(value.toString() == "1"){
            QColor color("#E0F1FF");
            // Draw text instead of button
            painter->fillRect(option.rect, color);
            painter->setPen(Qt::black);
            painter->drawText(option.rect, Qt::AlignCenter, "Retried");

        }
        else if (value.toString() == "Traded") {
            QColor backgroundColor("#E0F1FF");  // Light blue background
            painter->fillRect(option.rect, backgroundColor);

            QFont font = painter->font();
            font.setBold(true);  // Make text bold
            painter->setFont(font);

            painter->setPen(QColor(0, 128, 0));  // Text color
            painter->drawText(option.rect, Qt::AlignCenter, "Traded");
        }
        else if (value.toString() == "Rejected") {
            QColor backgroundColor("#E0F1FF");  // Light blue background
            painter->fillRect(option.rect, backgroundColor);

            QFont font = painter->font();
            font.setBold(true);  // Make text bold
            painter->setFont(font);
            painter->setPen(QColor(203, 5, 5));  // Red text
            painter->drawText(option.rect, Qt::AlignCenter, "Rejected");
        }
        else if (value.toString() == "Partial Trade") {
            QColor backgroundColor("#E0F1FF");  // Light blue background
            painter->fillRect(option.rect, backgroundColor);

            QFont font = painter->font();
            font.setBold(true);  // Make text bold
            painter->setFont(font);
            painter->setPen(QColor("#0D587D"));  // Green text
            painter->drawText(option.rect, Qt::AlignCenter, "Partial Trade");
        }
        else if (value.toString() == "Cancelled") {
            QColor backgroundColor("#E0F1FF");  // Light blue background
            painter->fillRect(option.rect, backgroundColor);

            QFont font = painter->font();
            font.setBold(true);  // Make text bold
            painter->setFont(font);
            painter->setPen(QColor(203, 5, 5));  // Red text
            painter->drawText(option.rect, Qt::AlignCenter, "Cancelled");
        }

        else{
            QColor color("#E0F1FF");
            // Draw text instead of button
            painter->fillRect(option.rect, color);
            painter->setPen(Qt::black);
            painter->drawText(option.rect, Qt::AlignCenter, value.toString());
        }


    }

    else {
        QColor color("#E0F1FF");
        QColor HighlightColor("#42A5F5");
        if(option.state & QStyle::State_Active)
            op.palette.setColor(QPalette::Highlight, HighlightColor);
        else
            op.palette.setColor(QPalette::Highlight, color);

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, HighlightColor);
        else
            painter->fillRect(option.rect, color);

        QStyledItemDelegate::paint(painter, op, index);

    }

    //for Retry_Order_Bt do not draw border
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
