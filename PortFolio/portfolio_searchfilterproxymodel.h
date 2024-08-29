#ifndef PORTFOLIO_SEARCHFILTERPROXYMODEL_H
#define PORTFOLIO_SEARCHFILTERPROXYMODEL_H


#include <QSortFilterProxyModel>

class Portfolio_SearchFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit Portfolio_SearchFilterProxyModel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
};

#endif // PORTFOLIO_SEARCHFILTERPROXYMODEL_H
