#ifndef NETPOS_SEARCHFILTERPROXYMODEL_H
#define NETPOS_SEARCHFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>
#include <QRegularExpression>

class netpos_searchfilterproxymodel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit netpos_searchfilterproxymodel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};


#endif // NETPOS_SEARCHFILTERPROXYMODEL_H
