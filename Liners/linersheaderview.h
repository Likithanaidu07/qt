#ifndef LINERSHEADERVIEW_H
#define LINERSHEADERVIEW_H

#include <QHeaderView>
class linersheaderview : public QHeaderView {
    Q_OBJECT

public:
    explicit linersheaderview(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
};

#endif // LINERSHEADERVIEW_H
