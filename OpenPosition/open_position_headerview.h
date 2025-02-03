#ifndef OPEN_POSITION_HEADERVIEW_H
#define OPEN_POSITION_HEADERVIEW_H

#include <QHeaderView>
    class open_position_headerview : public QHeaderView {
    Q_OBJECT

public:
    explicit open_position_headerview(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;

};

#endif // OPEN_POSITION_HEADERVIEW_H
