#ifndef TRADETABLE_SEARCHFILTERPROXYMODEL_H
#define TRADETABLE_SEARCHFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>
#include <QRegularExpression>

class tradetable_searchfilterproxymodel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit tradetable_searchfilterproxymodel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // TRADETABLE_SEARCHFILTERPROXYMODEL_H
