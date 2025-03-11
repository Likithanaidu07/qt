#include "table_orderbook_delegate.h"
#include "qsortfilterproxymodel.h"
#include <QLineEdit>

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

//    if (event->type() == QEvent::KeyPress) {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        if (keyEvent->key() == Qt::Key_Escape) {
//            if (auto searchField = view->parent()->findChild<QLineEdit*>("lineEditSearch")) {
//                searchField->clear();  // Clear the search field
//            }
//            m_highlightText.clear();  // Clear any text highlight

//            qDebug("Search cleared on Esc key press.");
//            view->update();
//            return true;
//        }
//    }

    // Pass the event to the base class or other event handlers
    return QStyledItemDelegate::eventFilter(object, event);
}


void Table_OrderBook_Delegate::setHighlightText(const QString &text) {
    // Implement logic to store and handle highlight text
    m_highlightText = text;
}

QColor Table_OrderBook_Delegate::getOrderStateColor(int orderState) const{
    switch (orderState) {
        case 5:
        case 13:
            return QColor(203, 5, 5);    // Red
        case 6:
            return QColor(250, 42, 85);  // Pinkish
        case 8:
            return QColor(0, 128, 0);    // Green
        case 10:
            return QColor(0, 0, 139);    // Dark Blue
        default:
            return QColor(0, 0, 0);      // Black
    }
}


void Table_OrderBook_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QStyleOptionViewItem op(option);
   // op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
  //  op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    const QSortFilterProxyModel *proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    QModelIndex mappedIndex = proxyModel->mapToSource(index);
    const Trade_Table_Model *model = qobject_cast<const Trade_Table_Model*>(proxyModel->sourceModel());
    auto c = mappedIndex.column();
    auto r = mappedIndex.row();
    QStringList order_list = model->trade_data_list.at(mappedIndex.row());
    QList<QString> fifthColumnValues;

    QColor textColor = QColor(0, 0, 0);
    int orderState = -1;
    int x_add = 1;

    if (c == OrderBook_Idx::Leg1State_OB) {
        orderState = order_list[OrderBook_Idx::Leg1StateVal_OB].toInt();
    } else if (c == OrderBook_Idx::Leg3State_OB) {
        orderState = order_list[OrderBook_Idx::Leg3StateVal_OB].toInt();
    } else if (c == OrderBook_Idx::Leg4State_OB) {
        orderState = order_list[OrderBook_Idx::Leg4StateVal_OB].toInt();
    } else if (c == OrderBook_Idx::BidLegState_OB) {
        orderState = order_list[OrderBook_Idx::BidLegStateVal_OB].toInt();
    }

    if (orderState != -1) {
        textColor = getOrderStateColor(orderState);
    }



    if (c == OrderBook_Idx::ExchPrice_OB ||
        c == OrderBook_Idx::UserPrice_OB ||
        c == OrderBook_Idx::Jackpot_OB ||
        c == OrderBook_Idx::TradedLot_OB ||
        c == OrderBook_Idx::RemainingLot_OB) {

        QString buy_sell = order_list[OrderBook_Idx::BuyorSell_OB];

//        if (c == OrderBook_Idx::ExchPrice_OB) {
//            QString ExchPrice = order_list[OrderBook_Idx::ExchPrice_OB];
//            QString userPriceStr = order_list[OrderBook_Idx::UserPrice_OB];  // Variable for user price
//            bool okJackpot, okUserPrice;

//            double ExchPriceValue = ExchPrice.toDouble(&okJackpot);  // Convert jackpot QString to double
//            double userPriceValue = userPriceStr.toDouble(&okUserPrice);  // Convert user price QString to double

//            // Check if both conversions were successful
//            if (okJackpot && okUserPrice) {
//                // Check if jackpot value is less than 20% of user price
//                if (ExchPriceValue < (userPriceValue * 0.2)) {
//                    QColor lightReddishPink("#FF9A9A");  // Light reddish pink color
//                    op.palette.setColor(QPalette::Highlight, Qt::transparent);
//                    op.palette.setColor(QPalette::HighlightedText, Qt::black);
//                    painter->fillRect(option.rect, lightReddishPink);

//                    // Set text color and print the jackpot value inside the cell
//                    painter->setPen(Qt::black);  // Set the text color to black
//                    painter->drawText(option.rect, Qt::AlignCenter, ExchPrice);  // Draw the value centered in the cell

//                    double borderWidth = 0.5;
//                    QColor myColor(108, 117, 125);
//                    QPen pen(myColor);
//                    pen.setWidthF(borderWidth);
//                    painter->setPen(pen);
//                    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

//                    // Optionally return early if no further drawing is needed
//                    return;
//                }
//            }
//        }



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

     else if (c == OrderBook_Idx::OrderId_OB ||
               c == OrderBook_Idx::AlgoName_OB ||
               c == OrderBook_Idx::AlgoNo_OB ||
               c == OrderBook_Idx::Leg1State_OB ||
               c == OrderBook_Idx::Leg3State_OB ||
               c == OrderBook_Idx::Leg4State_OB   ||
               c == OrderBook_Idx::BidLegState_OB ||
               c == OrderBook_Idx::TradeTime_OB) {


        //QColor textColor(108, 117, 125);
        QColor bgColor("#E0F1FF");
        QString Leg1_OrderState = order_list[OrderBook_Idx::Leg1StateVal_OB];
        QString Leg3_OrderState = order_list[OrderBook_Idx::Leg3StateVal_OB];
        QString Leg4_OrderState = order_list[OrderBook_Idx::Leg4StateVal_OB];
        QString Leg2_OrderState = order_list[OrderBook_Idx::BidLegStateVal_OB];

        if (Leg1_OrderState == "6" || Leg3_OrderState == "6" || Leg4_OrderState == "6" || Leg2_OrderState == "6") {
            bgColor = QColor(215, 207, 232);  // Light purple background color
        }
        else if (Leg1_OrderState == "5" || Leg3_OrderState == "5" || Leg4_OrderState == "5" || Leg2_OrderState == "5") {
            bgColor = QColor(215, 207, 232);  // Light purple background color
        }
        else if (Leg1_OrderState == "13" || Leg3_OrderState == "13" || Leg4_OrderState == "13" || Leg2_OrderState == "13") {
            bgColor = QColor(215, 207, 232);  // Light purple background color
        }
        else if (Leg1_OrderState == "8" || Leg3_OrderState == "8" || Leg4_OrderState == "8" || Leg2_OrderState == "8") {
          bgColor = QColor(190, 180, 220);  // little light purple background color
        }
       // Set the background color
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




        if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active) {
            textColor = QColor(255, 255, 255);               // Apply white text color
            painter->fillRect(option.rect, HighlightColor);  // Apply the same highlight color
        }
        else
            painter->fillRect(option.rect, bgColor);


        QPoint p1 = option.rect.bottomLeft();
        QPoint p2 = option.rect.bottomRight();
        p1.setX(p1.x() - x_add);
        p2.setX(p2.x() + x_add);
        painter->drawLine(p1, p2);


        //order_list
        if(order_list[TradeTable_Hihglight_OB]=="1"){
            QColor color("#cfc0ce");
            if (option.state & QStyle::State_Selected && option.state & QStyle::State_Active)
                painter->fillRect(option.rect, HighlightColor);
            else
                painter->fillRect(option.rect, color);
            QPoint p1= option.rect.bottomLeft();
            QPoint p2= option.rect.bottomRight();
            p1.setX(p1.x()-x_add);
            p2.setX(p2.x()+x_add);
            painter->drawLine(p1,p2);
 }

    } else {
        QColor color("#E0F1FF");
        painter->fillRect(option.rect, color);
    }


    if (c == OrderBook_Idx::TradeTime_OB || c == OrderBook_Idx::AlgoNo_OB || c == OrderBook_Idx::UserPrice_OB) {
        QStyleOptionViewItem op(option);

        double borderWidth = 1;
        QColor myColor(108, 117, 125);
        QPen pen(myColor);
        pen.setWidthF(borderWidth);
        painter->setPen(pen);
        painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
    }

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
   // painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QString text = mappedIndex.data(Qt::DisplayRole).toString();
    int startIndex = text.indexOf(m_highlightText, 0, Qt::CaseInsensitive);
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, mappedIndex);

    painter->save();  // Save the painter state
    painter->setClipRect(opt.rect);
    painter->setFont(opt.font);

    if (startIndex >= 0) {
        QString before = text.left(startIndex);
        QString match = text.mid(startIndex, m_highlightText.length());
        QString after = text.mid(startIndex + m_highlightText.length());

        QRect fullTextRect = painter->boundingRect(opt.rect, Qt::AlignCenter, text);
        int beforeWidth = painter->fontMetrics().horizontalAdvance(before);
        int matchWidth = painter->fontMetrics().horizontalAdvance(match);

        int highlightStartX = fullTextRect.left() + beforeWidth;
        QRect highlightRect(highlightStartX, opt.rect.top(), matchWidth, opt.rect.height());

        painter->fillRect(highlightRect, QColor(229, 229, 227));

        // Ensure text is always drawn in black


        painter->setPen(textColor);
        painter->drawText(opt.rect, Qt::AlignCenter, before + match + after);
    } else {
        // Ensure text is always drawn in black
        painter->setPen(textColor);
        painter->drawText(option.rect, Qt::AlignCenter, text);
    }

    painter->restore();


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

