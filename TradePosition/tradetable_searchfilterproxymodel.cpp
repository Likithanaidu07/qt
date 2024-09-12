#include "tradetable_searchfilterproxymodel.h"

tradetable_searchfilterproxymodel::tradetable_searchfilterproxymodel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool tradetable_searchfilterproxymodel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
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
