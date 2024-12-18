#ifndef TABLE_PORTFOLIOS_DELEGATE_H
#define TABLE_PORTFOLIOS_DELEGATE_H

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



// Integer Validator, only allow value between bottom and top
class IntDiffValidator : public QIntValidator
{
public:
    IntDiffValidator(int bottom, int top, QObject *parent) :
        QIntValidator(bottom, top, parent)
    {

    }

    QValidator::State validate(QString &s, int &i) const override
    {
        if (s.isEmpty() || s == "-") {
            return QValidator::Intermediate;
        }

        bool ok;
        int value = s.toInt(&ok);

        if (ok) {
            if (value >= bottom() && value <= top()) {
                return QValidator::Acceptable;
            } else {
                return QValidator::Invalid;
            }
        } else {
            return QValidator::Invalid;
        }
    }
};

class DiffValidator : public QDoubleValidator
{
private:
    bool is_cds;

public:
    DiffValidator(double bottom, double top, int decimals, bool is_cds, QObject *parent) :
        QDoubleValidator(bottom, top, decimals, parent),
        is_cds(is_cds)
    {
    }

    QValidator::State validate(QString &s, int &i) const override
    {
        if (s.isEmpty() || s == "-") {
            return QValidator::Intermediate;
        }

        QChar decimalPoint = locale().decimalPoint().at(0);

        // Handle the case where the string starts with a decimal point
        QString modifiedStr = s;
        if (s.startsWith(decimalPoint)) {
            modifiedStr.prepend("0");
        }

        // Check for decimals and precision
        if (modifiedStr.indexOf(decimalPoint) != -1) {
            int charsAfterPoint = modifiedStr.length() - modifiedStr.indexOf(decimalPoint) - 1;

            if (charsAfterPoint > decimals()) {
                return QValidator::Invalid;
            }

            // Check last digit (0 or 5) for cds only
            if (is_cds && charsAfterPoint == decimals()) {
                QString lastDigit = modifiedStr.at(modifiedStr.length() - 1);
                if (lastDigit != "0" && lastDigit != "5") {
                    return QValidator::Invalid;
                }
            }
        }

        // Validate the number range
        bool ok;
        double d = locale().toDouble(modifiedStr, &ok);

        if (ok && d >= bottom() && d <= top()) {
            return QValidator::Acceptable;
        } else {
            return QValidator::Invalid;
        }
    }
};



class Table_Portfolios_Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Table_Portfolios_Delegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,  const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool eventFilter(QObject *obj, QEvent *event) override ;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    Table_Portfolios_Delegate(QTableView* tableView);

    mutable QModelIndex currentIndex;
    mutable QAbstractItemModel *mutableModel;
    void setHighlightText(const QString &text);

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void editFinished(QString text, QModelIndex idx) const;
    void tabKeyPressed(nav_direction);
    void escapeKeyPressed(int);

private:

    QString m_highlightText;
    double price_diff_incrementer = 0.01;
    double quantity_incrementer = 1.0;
    QString market_type;
    QPen pen;
    QTableView* view;



};

#endif // TABLE_PORTFOLIOS_DELEGATE_H




