#include "portfolio_searchfilterproxymodel.h"
#include <QRegularExpression>

Portfolio_SearchFilterProxyModel::Portfolio_SearchFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool Portfolio_SearchFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
    // Retrieve the current filter regular expression
    QRegularExpression regex = filterRegularExpression();

    for (int i = 0; i < sourceModel()->columnCount(); ++i) {
        QModelIndex index = sourceModel()->index(source_row, i, source_parent);
        // Use the regex to match the data in the row
        if (sourceModel()->data(index).toString().contains(regex)) {
            return true;
        }
    }
    return false;
}
