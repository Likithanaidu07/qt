#ifndef PORTFOLIOHEADERDELEGATE_H
#define PORTFOLIOHEADERDELEGATE_H



#include <QStyledItemDelegate>

class PortfolioHeaderDelegate : public QStyledItemDelegate {
public:
    explicit PortfolioHeaderDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // PORTFOLIOHEADERDELEGATE_H


