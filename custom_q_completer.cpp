#include "custom_q_completer.h"
const int custom_q_completer::CompleteRole = Qt::UserRole + 1;

custom_q_completer::custom_q_completer(QObject *parent)
    : QCompleter{parent}
{

}


QString custom_q_completer::pathFromIndex(const QModelIndex &index) const{
    return index.data().toString();
    //return index.data(CompleteRole).toString(); //if you want to dsiplay value instead text enable this
}

QString custom_q_completer::get_foo_token_number_for_selected(const QModelIndex &index) const
{
    QString code = index.data(CompleteRole).toString();
    return code;
}


CustomSearchWidget::CustomSearchWidget(QListView *customListView, QStandardItemModel *_model, QWidget *parent):
    QWidget(parent), listView(customListView),model(_model)
{
    // ... rest of the constructor remains unchanged
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int column = 0; column < model->columnCount(); ++column) {
            QModelIndex index = model->index(row, column);
            QVariant data = index.data(Qt::DisplayRole);
            if (data.isValid()) {
                completionData << data.toString();
            }
        }
    }

    stringListModel = new QStringListModel(completionData, this);

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);  // Set case insensitivity
    proxyModel->setSourceModel(stringListModel);

    // proxyModel->setSourceModel(model);
    listView->setModel(proxyModel);
    listView->hide();

}
void CustomSearchWidget::filterItems(const QString &text) {
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(listView->model());
    if (!proxyModel)
        return;

    if (text.isEmpty()) {
        // If the search text is empty, clear the filter and hide the list view
        proxyModel->setFilterRegularExpression("");
        proxyModel->sort(0, Qt::AscendingOrder);  // Sort by the first column, adjust as needed
        listView->hide();
        return;
    }

    // Step 1: Perform the "starts with" search
    QStringList searchTerms = text.split(" ", Qt::SkipEmptyParts);

    QString startsWithPattern;
    int i = 0;
    for (const QString &term : searchTerms) {
        if (i == 0) {
            startsWithPattern += "^" + QRegularExpression::escape(term);
        } else {
            startsWithPattern += "(?=.*" + QRegularExpression::escape(term) + ")";
        }
        i++;
    }

    QRegularExpression startsWithRegex(startsWithPattern, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(startsWithRegex);

    // Show or hide the list view based on the filter result
    if (proxyModel->rowCount() > 0) {
        proxyModel->sort(0, Qt::AscendingOrder);  // Sort by the first column, adjust as needed
        listView->show();
        return;  // If matches are found, exit the function
    }

    // Step 2: Perform the "contains" search if no matches are found
    QString containsPattern = ".*";
    for (const QString &term : searchTerms) {
        containsPattern += QRegularExpression::escape(term) + ".*";
    }

    QRegularExpression containsRegex(containsPattern, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(containsRegex);

    // Show or hide the list view based on the filter result
    if (proxyModel->rowCount() > 0) {
        proxyModel->sort(0, Qt::AscendingOrder);  // Sort by the first column, adjust as needed
        listView->show();
    } else {
        listView->hide();
    }
}
