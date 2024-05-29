#ifndef ORDER_DETAIL_POPUP_DELEGATE_H
#define ORDER_DETAIL_POPUP_DELEGATE_H

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


// Double  Validator, only allow value beteween bottom and top and last digit should be 0 or 5
class DiffValidator : public QDoubleValidator
{
private:
    bool is_cds;

public:
    DiffValidator(double bottom, double top, int decimals,bool is_cds, QObject * parent) :
        QDoubleValidator(bottom, top, decimals, parent),
        is_cds(is_cds)
    {
    }

    QValidator::State validate(QString &s, int &i) const
    {
        if (s.isEmpty() || s == "-") {
            return QValidator::Intermediate;
        }

        QChar decimalPoint = locale().decimalPoint().at(0); // or locale().decimalPoint().first();

        if(s.indexOf(decimalPoint) != -1) {
            int charsAfterPoint = s.length() - s.indexOf(decimalPoint) - 1;

            if (charsAfterPoint > decimals()) {
                return QValidator::Invalid;
            }

            // check last digi it 0 or 5 for cds only
            if (is_cds) {
                if(charsAfterPoint==decimals()){
                    QString lastDigit = s.at(s.length() - 1); // Using at()
                    if(lastDigit!="0"&&lastDigit!="5"){
                        return QValidator::Invalid;
                    }
                }
            }
        }

        bool ok;
        double d = locale().toDouble(s, &ok);

        if (ok && d >= bottom() && d <= top()) {
            return QValidator::Acceptable;
        } else {
            return QValidator::Invalid;
        }
    }
};


class order_detail_popup_delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit order_detail_popup_delegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool eventFilter(QObject *obj, QEvent *event) override ;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    order_detail_popup_delegate(QTableView* tableView);

    mutable QModelIndex currentIndex;
    mutable QAbstractItemModel *mutableModel;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:
    void editFinished(QString text, QModelIndex idx) const;
    void tabKeyPressed(nav_direction);

private:


    double price_diff_incrementer = 0.05;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;


};

#endif // TABLE_PORTFOLIOS_DELEGATE_H




