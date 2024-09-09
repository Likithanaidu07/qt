#include "trade_table_filterproxymodel.h"
#include <QRegularExpression>  // Include the correct header for QRegularExpression

trade_table_filterproxymodel::trade_table_filterproxymodel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void trade_table_filterproxymodel::setFilter(int column, const QString &text) {
    setFilterKeyColumn(column);  // Set the column to filter
    setFilterRegularExpression(QRegularExpression(text, QRegularExpression::CaseInsensitiveOption));  // Apply the filter using the provided text
}

bool trade_table_filterproxymodel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    if (filterKeyColumn() < 0) {
        // If filterKeyColumn is -1, search across all columns
        for (int i = 0; i < sourceModel()->columnCount(); ++i) {
            QModelIndex index = sourceModel()->index(sourceRow, i, sourceParent);
            if (sourceModel()->data(index).toString().contains(filterRegularExpression())) {
                return true; // Accept the row if any column contains the filter text
            }
        }
        return false; // Reject if none of the columns match
    } else {
        // Filter based on a specific column
        QModelIndex index = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);
        return sourceModel()->data(index).toString().contains(filterRegularExpression());
    }
}
