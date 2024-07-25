#ifndef NET_POSITION_TABLE_HEADERVIEW_H
#define NET_POSITION_TABLE_HEADERVIEW_H

#include <QHeaderView>
 class net_position_table_headerview : public QHeaderView {
    Q_OBJECT

public:
    explicit net_position_table_headerview(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
};


#endif // NET_POSITION_TABLE_HEADERVIEW_H
