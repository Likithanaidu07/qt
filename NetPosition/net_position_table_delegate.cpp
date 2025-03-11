#include "net_position_table_delegate.h"
#include "qsortfilterproxymodel.h"
#include <QLineEdit>

net_position_table_delegate::net_position_table_delegate(QObject *parent)  : QStyledItemDelegate{parent}
{
}
void net_position_table_delegate::setHighlightText(const QString &text) {
    // Implement logic to store and handle highlight text
    m_highlightText = text;
}
void net_position_table_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem op(option);
    const QSortFilterProxyModel *proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    QModelIndex mappedIndex = proxyModel->mapToSource(index);

    auto c = index.column();
    auto r = index.row();
    const Net_Position_Table_Model *model = qobject_cast<const Net_Position_Table_Model*>(proxyModel->sourceModel());
    int RowCount = model->net_pos_data_list.size();

    bool LastRow = (r == RowCount - 1);  // Check if it's the last row
    QColor textColor = QColor(0, 0, 0);
    QColor color;
    int x_add = 1;

    if (c == NET_POS::BuyAvgPrice_NP || c == NET_POS::BuyTotalLot_NP || c == NET_POS::BuyValue_NP) {
        color = QColor("#42A5F5");
        op.palette.setColor(QPalette::Highlight, color);
        color = QColor("#D6FCF0");
        op.palette.setColor(QPalette::Highlight, Qt::transparent);
        op.palette.setColor(QPalette::HighlightedText, Qt::black);
        painter->fillRect(option.rect, color);
    }
    else if (c == NET_POS::SellAvgPrice_NP || c == NET_POS::SellTotalLot_NP || c == NET_POS::SellValue_NP) {
        color = QColor("#FED9D9");
        op.palette.setColor(QPalette::Highlight, Qt::transparent);
        op.palette.setColor(QPalette::HighlightedText, Qt::black);
        painter->fillRect(option.rect, color);
    }
    else {
        QColor color("#E0F1FF");
        QColor HighlightColor("#42A5F5");
        if (option.state & QStyle::State_Active)
            op.palette.setColor(QPalette::Highlight, HighlightColor);
        else
            op.palette.setColor(QPalette::Highlight, color);

        if (option.state & QStyle::State_Selected && !LastRow)
            painter->fillRect(option.rect, HighlightColor);
        else
            painter->fillRect(option.rect, color);
    }

    double borderWidth = 0.5;
    QColor borderColor(108, 117, 125);
    QPen pen(borderColor);
    pen.setWidthF(borderWidth);
    painter->setPen(pen);
    QPoint p1 = option.rect.bottomLeft();
    QPoint p2 = option.rect.bottomRight();
    p1.setX(p1.x() - x_add);
    p2.setX(p2.x() + x_add);
    painter->drawLine(p1, p2);

    QString text = mappedIndex.data(Qt::DisplayRole).toString();
    int startIndex = text.indexOf(m_highlightText, 0, Qt::CaseInsensitive);
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, mappedIndex);

    painter->save();
    painter->setClipRect(opt.rect);
    painter->setFont(opt.font);

    if (LastRow) {
        QFont boldFont;
        boldFont.setBold(true);
        painter->setFont(boldFont);

        if (c == 0) {  // First column: Blue & Bold
            textColor = QColor("#FFD700");
             painter->fillRect(option.rect, QColor("#42A5F5"));
        } else {  // Other columns: Highlighted & Bold
            textColor = QColor(Qt::black);
            painter->fillRect(option.rect, QColor("#42A5F5"));  // Light Yellow Highlight
        }

        // Draw a border around the last row's cell
        QPen lastRowBorder(Qt::black, 1.5);  // Bold black border
        painter->setPen(lastRowBorder);

        QRect cellRect = option.rect;
        if (c == 0) {
            painter->drawLine(cellRect.topLeft(), cellRect.bottomLeft());
        }
        if (c == model->columnCount() - 1) {
            painter->drawLine(cellRect.topRight(), cellRect.bottomRight());
        }
        if (r == RowCount - 1) {
            painter->drawLine(cellRect.topLeft(), cellRect.topRight());
        }
        painter->drawLine(cellRect.bottomLeft(), cellRect.bottomRight());
    }

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

        painter->setPen(textColor);
        painter->drawText(opt.rect, Qt::AlignCenter, before + match + after);
    } else {
        painter->setPen(textColor);
        painter->drawText(option.rect, Qt::AlignCenter, text);
    }

    painter->restore();
}



