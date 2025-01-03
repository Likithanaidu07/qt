#include "order_f1_f2_delegate.h"
#include <QLineEdit>
#include "order_f1_f2_model.h"

Order_F1_F2_Delegate::Order_F1_F2_Delegate(QObject *parent)  : QStyledItemDelegate{parent}
{

}

bool Order_F1_F2_Delegate::eventFilter(QObject *object, QEvent *event) {
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


void Order_F1_F2_Delegate::setHighlightText(const QString &text) {
    // Implement logic to store and handle highlight text
    m_highlightText = text;
}

QColor Order_F1_F2_Delegate::getOrderStateColor(int orderState) const{
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


void Order_F1_F2_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

    QStyleOptionViewItem op(option);
   // op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
  //  op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

   // const QSortFilterProxyModel *proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
  //  QModelIndex mappedIndex = proxyModel->mapToSource(index);
   // const Trade_Table_Model *model = qobject_cast<const Trade_Table_Model*>(proxyModel->sourceModel());


    Order_F1_F2_Model *model=(Order_F1_F2_Model*) index.model();
    QStringList order_list=model->f1f2_order_data_list.at(index.row());

    auto c = index.column();
    auto r = index.row();
    int x_add=1;

    QColor textColor = QColor(0, 0, 0);
    int orderState = -1;

    if (c == F1F2_Order_Data_Idx::Leg1State_F1F2) {
        orderState = order_list[F1F2_Order_Data_Idx::Leg1StateVal_F1F2].toInt();
    } else if (c == F1F2_Order_Data_Idx::Leg3State_F1F2) {
        orderState = order_list[F1F2_Order_Data_Idx::Leg3StateVal_F1F2].toInt();
    } else if (c == F1F2_Order_Data_Idx::Leg4State_F1F2) {
        orderState = order_list[F1F2_Order_Data_Idx::Leg4StateVal_F1F2].toInt();
    } else if (c == F1F2_Order_Data_Idx::BidLegState_F1F2) {
        orderState = order_list[F1F2_Order_Data_Idx::BidLegStateVal_F1F2].toInt();
    }

    if (orderState != -1) {
        textColor = getOrderStateColor(orderState);
    }



    if (c == F1F2_Order_Data_Idx::ExchPrice_F1F2 ||
        c == F1F2_Order_Data_Idx::UserPrice_F1F2 ||
        c == F1F2_Order_Data_Idx::TradedLot_F1F2 /*||
        c == F1F2_Order_Data_Idx::RemainingLot_F1F2*/) {

        QString buy_sell = order_list[F1F2_Order_Data_Idx::BuyorSell_F1F2];

        QColor color("#42A5F5");
        op.palette.setColor(QPalette::Highlight, color);

        if (buy_sell == "Sell") {
            QColor color("#FED9D9");
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
            QColor color("#D6FCF0");
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

    } else if (c == F1F2_Order_Data_Idx::OrderId_F1F2 ||
               c == F1F2_Order_Data_Idx::AlgoName_F1F2 ||
//               c == F1F2_Order_Data_Idx::AlgoNo_F1F2 ||
               c == F1F2_Order_Data_Idx::Leg1State_F1F2 ||
               c == F1F2_Order_Data_Idx::Leg3State_F1F2 ||
               c == F1F2_Order_Data_Idx::Leg4State_F1F2   ||
               c == F1F2_Order_Data_Idx::BidLegState_F1F2 ||
               c == F1F2_Order_Data_Idx::TradeTime_F1F2) {


        //QColor textColor(108, 117, 125);
        QColor bgColor("#E0F1FF");
        QString Leg1_OrderState = order_list[F1F2_Order_Data_Idx::Leg1StateVal_F1F2];
        QString Leg3_OrderState = order_list[F1F2_Order_Data_Idx::Leg3StateVal_F1F2];
        QString Leg4_OrderState = order_list[F1F2_Order_Data_Idx::Leg4StateVal_F1F2];
        QString Leg2_OrderState = order_list[F1F2_Order_Data_Idx::BidLegStateVal_F1F2];

        if (Leg1_OrderState == "6" || Leg3_OrderState == "6" || Leg4_OrderState == "6" || Leg2_OrderState == "6") {
            bgColor = QColor(215, 207, 232);  // Light purple background color
        }
        else if (Leg1_OrderState == "5" || Leg3_OrderState == "5" || Leg4_OrderState == "5" || Leg2_OrderState == "5") {
            bgColor = QColor(215, 207, 232);  // Light purple background color
        }
        else if (Leg1_OrderState == "13" || Leg3_OrderState == "13" || Leg4_OrderState == "13" || Leg2_OrderState == "13") {
            bgColor = QColor(215, 207, 232);  // Light purple background color
        }

        // Set the background color
        painter->fillRect(option.rect, bgColor);

        // Set the text color and draw the text
        painter->setPen(textColor);
        painter->drawText(option.rect, index.data().toString(), QTextOption(Qt::AlignCenter));

        // Optionally call the base class method for default painting
        QStyledItemDelegate::paint(painter, option, index);


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

    } else {
        QColor color("#E0F1FF");
        painter->fillRect(option.rect, color);
    }

    if (c == F1F2_Order_Data_Idx::TradeTime_F1F2 /*|| c == F1F2_Order_Data_Idx::AlgoNo_F1F2*/ || c == F1F2_Order_Data_Idx::UserPrice_F1F2) {
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
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

    QString text = index.data(Qt::DisplayRole).toString();
    int startIndex = text.indexOf(m_highlightText, 0, Qt::CaseInsensitive);
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

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



}

