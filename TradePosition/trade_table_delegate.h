#ifndef TRADE_TABLE_DELEGATE_H
#define TRADE_TABLE_DELEGATE_H
#include <QObject>

#include "QPalette"
#include "QStyleOptionViewItem"
#include "QColor"
#include "QDateTimeEdit"
#include <QContextMenuEvent>
#include <QMenu>
#include "QStyledItemDelegate"
#include "defines.h"
#include "qpen.h"
#include "qtableview.h"
#include <QAbstractTableModel>

class trade_table_delegate : public QStyledItemDelegate
{
public:
    trade_table_delegate();
    Q_OBJECT
public:
    explicit trade_table_delegate(QObject *parent = nullptr);
   // QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  //  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
 //   void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const override;
 // void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  // bool eventFilter(QObject *obj, QEvent *event) override ;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;


    mutable QModelIndex currentIndex;
    mutable QAbstractItemModel *mutableModel;

   // bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
   // void editFinished(QString text, QModelIndex idx) const;
 //   void tabKeyPressed(nav_direction);

private:


    double price_diff_incrementer = 0.05;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;


};

#endif // TRADE_TABLE_DELEGATE_H
