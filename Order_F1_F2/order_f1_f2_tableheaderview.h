#ifndef ORDER_F1_F2_TABLEHEADERVIEW_H
#define ORDER_F1_F2_TABLEHEADERVIEW_H
#include <QHeaderView>



class order_f1_f2_tableheaderview : public QHeaderView {
      Q_OBJECT

public:
    explicit order_f1_f2_tableheaderview(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const override;
signals:
     void filterChanged(int column, const QString& text);
};

#endif // ORDER_F1_F2_TABLEHEADERVIEW_H








