#include "open_position_delegate.h"
#include "qsortfilterproxymodel.h"
#include <QLineEdit>
#include <QPushButton>

open_position_delegate::open_position_delegate(QObject *parent)  : QStyledItemDelegate{parent}
{

}

QWidget *open_position_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    int c = index.column();
    if (c == Open_Position_Idx::Square_Off)
    {
        QPushButton *editor = new QPushButton("Square Off", parent);
        connect(editor, &QPushButton::clicked, [=]() {
            qDebug() << "Square Off button clicked in column:" << c;
        });
        return editor;
    }
}

void open_position_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    auto c = index.column();
    auto r = index.row();
    QStyleOptionViewItem op(option);
    op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
    op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    open_position_model *model=(open_position_model*) index.model();
    QStringList order_list=model->open_position_data_list.at(index.row());

    QColor color;
    int x_add = 1;

    if (c == Open_Position_Idx::Buy_Sell ||
        c == Open_Position_Idx::Open_Lot ||
        c == Open_Position_Idx::open_Avg ) {

        QString buy_sell = order_list[Open_Position_Idx::Buy_Sell];

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
    }
    else if (index.column() == Open_Position_Idx::Square_Off)
    {
        QPushButton button("Square Off");
        button.setStyleSheet("background-color: #E0F1FF; border: 1px solid black;");
        op.palette.setColor(QPalette::Highlight, Qt::transparent);
        op.palette.setColor(QPalette::HighlightedText, Qt::black);
        button.resize(option.rect.size());

        QPixmap pixmap(option.rect.size());
        button.render(&pixmap);

        painter->drawPixmap(option.rect.topLeft(), pixmap);
         painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
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


    }*/ else {
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


//    if (c == OrderBook_Idx::TradeTime_OB || c == OrderBook_Idx::AlgoNo_OB || c == OrderBook_Idx::UserPrice_OB) {
//        QStyleOptionViewItem op(option);

//        double borderWidth = 1;
//        QColor myColor(108, 117, 125);
//        QPen pen(myColor);
//        pen.setWidthF(borderWidth);
//        painter->setPen(pen);
//        painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
//    }

//    double borderWidth = 0.5;
//    QColor myColor(108, 117, 125);
//    QPen pen(myColor);
//    pen.setWidthF(borderWidth);
//    painter->setPen(pen);
//    QPoint p1 = option.rect.bottomLeft();
//    QPoint p2 = option.rect.bottomRight();
//    p1.setX(p1.x() - x_add);
//    p2.setX(p2.x() + x_add);
//    painter->drawLine(p1, p2);
//    // painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

//    QString text = mappedIndex.data(Qt::DisplayRole).toString();
//    int startIndex = text.indexOf(m_highlightText, 0, Qt::CaseInsensitive);
//    QStyleOptionViewItem opt = option;
//    initStyleOption(&opt, mappedIndex);

//    painter->save();  // Save the painter state
//    painter->setClipRect(opt.rect);
//    painter->setFont(opt.font);

//    if (startIndex >= 0) {
//        QString before = text.left(startIndex);
//        QString match = text.mid(startIndex, m_highlightText.length());
//        QString after = text.mid(startIndex + m_highlightText.length());

//        QRect fullTextRect = painter->boundingRect(opt.rect, Qt::AlignCenter, text);
//        int beforeWidth = painter->fontMetrics().horizontalAdvance(before);
//        int matchWidth = painter->fontMetrics().horizontalAdvance(match);

//        int highlightStartX = fullTextRect.left() + beforeWidth;
//        QRect highlightRect(highlightStartX, opt.rect.top(), matchWidth, opt.rect.height());

//        painter->fillRect(highlightRect, QColor(229, 229, 227));

//        // Ensure text is always drawn in black


//        painter->setPen(textColor);
//        painter->drawText(opt.rect, Qt::AlignCenter, before + match + after);
//    } else {
//        // Ensure text is always drawn in black
//        painter->setPen(textColor);
//        painter->drawText(option.rect, Qt::AlignCenter, text);
//    }

//    painter->restore();


    /* int orderState = mappedIndex.model()->data(index, Qt::UserRole).toInt();

    // Determine the text color based on the order state
    QColor textColor;
    switch (orderState) {
    case 5:
    case 13:
        textColor = QColor(203, 5, 5); // Red
        break;
    case 6:
        textColor = QColor(250, 42, 85); // Pinkish
        break;
    case 8:
        textColor = QColor(0, 128, 0); // Green
        break;
    case 10:
        textColor = QColor(0, 0, 139); // Dark Blue
        break;
    default:
        textColor = QColor(0, 0, 0); // Black
        break;
    }



    // Set the text color in the palette
    opt.palette.setColor(QPalette::Text, textColor);*/
    // opt.palette.setColor(QPalette::Highlight, Qt::transparent);


    // Call the base class paint method to handle the rest
    // QStyledItemDelegate::paint(painter, opt, mappedIndex);


}
