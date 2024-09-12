#include "table_orderbook_delegate.h"
#include "qsortfilterproxymodel.h"

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

void Table_OrderBook_Delegate::setHighlightText(const QString &text) {
    // Implement logic to store and handle highlight text
    m_highlightText = text;
}

void Table_OrderBook_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto c = index.column();
    auto r = index.row();
    QStyleOptionViewItem op(option);
    op.palette.setColor(QPalette::All, QPalette::Window, Qt::red);
    op.palette.setColor(QPalette::All, QPalette::WindowText, Qt::red);

    const QSortFilterProxyModel *proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    const Trade_Table_Model *model = qobject_cast<const Trade_Table_Model*>(proxyModel->sourceModel());

    QStringList order_list = model->trade_data_list.at(index.row());
    QList<QString> fifthColumnValues;

    if (order_list.at(7) == "Sell") {

    } else {

    }

    if (c == OrderBook_Idx::ExchPrice_OB ||
        c == OrderBook_Idx::UserPrice_OB ||
        c == OrderBook_Idx::Jackpot_OB ||
        c == OrderBook_Idx::TradedLot_OB ||
        c == OrderBook_Idx::RemainingLot_OB) {

        QStringList order_list = model->trade_data_list.at(index.row());
        QString buy_sell = order_list[OrderBook_Idx::BuyorSell_OB];

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
            p1.setX(p1.x() - 5);
            p2.setX(p2.x() + 5);
            painter->drawLine(p1, p2);
        } else if (buy_sell == "Buy") {
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

    } else if (c == OrderBook_Idx::OrderId_OB ||
               c == OrderBook_Idx::AlgoName_OB ||
               c == OrderBook_Idx::AlgoNo_OB ||
               c == OrderBook_Idx::Leg1State_OB ||
               c == OrderBook_Idx::Leg3State_OB ||
               c == OrderBook_Idx::BidLegState_OB ||
               c == OrderBook_Idx::TradeTime_OB) {

        QColor textColor(108, 117, 125);
        QPen pen(textColor);
        pen.setWidthF(0.5);
        painter->setPen(pen);
        QColor HighlightColor("#42A5F5");

        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, HighlightColor);
        else
            painter->fillRect(option.rect, QColor("#E0F1FF"));

        QPoint p1 = option.rect.bottomLeft();
        QPoint p2 = option.rect.bottomRight();
        p1.setX(p1.x() - 5);
        p2.setX(p2.x() + 5);
        painter->drawLine(p1, p2);

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
    p1.setX(p1.x() - 5);
    p2.setX(p2.x() + 5);
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

        painter->fillRect(highlightRect, QColor(163, 163, 161));

        // Ensure text is always drawn in black
        painter->setPen(Qt::black);
        painter->drawText(opt.rect, Qt::AlignCenter, before + match + after);
    } else {
        // Ensure text is always drawn in black
        painter->setPen(Qt::black);
        painter->drawText(option.rect, Qt::AlignCenter, text);
    }

    painter->restore();
}
