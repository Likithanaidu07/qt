#ifndef TRADETABLEHEADERVIEW_H
#define TRADETABLEHEADERVIEW_H

#include <QHeaderView>
class tradetableheaderview : public QHeaderView {
      Q_OBJECT

public:
    explicit tradetableheaderview(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
signals:
     void filterChanged(int column, const QString& text);
};

#endif // TRADETABLEHEADERVIEW_H






