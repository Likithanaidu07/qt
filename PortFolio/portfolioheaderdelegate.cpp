#include "portfolioheaderdelegate.h"

#include <QHeaderView>
#include <QApplication>
#include "QPainter"

PortfolioHeaderDelegate::PortfolioHeaderDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

void PortfolioHeaderDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    // Set different colors based on the column index (customize this part as per your requirement)
//    QColor headerColor;
//    if (index.column() == 0) {
//        headerColor = QColor(Qt::red);
//    } else if (index.column() == 1) {
//        headerColor = QColor(Qt::green);
//    } else {
//        headerColor = QColor(Qt::blue);
//    }

//    // Draw the header with the desired color
//    painter->save();
//    painter->fillRect(option.rect, headerColor);
//    painter->restore();

//    // Draw the header text
//    QStyleOptionHeader opt;
//    opt.rect = option.rect;
//    opt.state = QStyle::State_Enabled;
//    opt.textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
//    opt.text = index.data(Qt::DisplayRole).toString();

//    QApplication::style()->drawControl(QStyle::CE_Header, &opt, painter);
}
