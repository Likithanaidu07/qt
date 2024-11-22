#ifndef MISSED_TRADES_HEADERVIEW_H
#define MISSED_TRADES_HEADERVIEW_H

#include <QAbstractTableModel>
#include <QHeaderView>
    class missed_trades_headerview : public QHeaderView {
        Q_OBJECT

    public:
        explicit missed_trades_headerview(Qt::Orientation orientation, QWidget* parent = nullptr);

    protected:
        void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
    };

#endif // MISSED_TRADES_HEADERVIEW_H
