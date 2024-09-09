#ifndef TRADE_TABLE_FILTERPROXYMODEL_H
#define TRADE_TABLE_FILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class trade_table_filterproxymodel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit trade_table_filterproxymodel(QObject *parent = nullptr);
public slots:
    // Override to provide custom filtering logic
    void setFilter(int column, const QString& text);

protected:
    // Override to provide custom filtering logic
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    int filterColumn = -1;
    QString filterText;
};

#endif // TRADE_TABLE_FILTERPROXYMODEL_H
