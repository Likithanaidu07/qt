#ifndef PORTFOLIOHEADERVIEW_H
#define PORTFOLIOHEADERVIEW_H

#include <QHeaderView>

class PortfolioHeaderView : public QHeaderView {
    Q_OBJECT

public:
    explicit PortfolioHeaderView(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
};
#endif




