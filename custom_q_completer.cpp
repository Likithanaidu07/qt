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

void CustomSearchWidget::filterItems(const QString &text)
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(listView->model());
    if (!proxyModel)
        return;

    if (text.isEmpty()) {
        // If the search text is empty, clear the filter and hide the list view
        proxyModel->setFilterWildcard("");
        listView->hide();
        return;
    }

    QStringList searchTerms = text.split(" ", Qt::SkipEmptyParts);

    // Construct a regular expression pattern for each search term
    QString pattern;

    int index = 0;
    for (const QString &term : searchTerms)
    {
        if (index == 0)
        {
            // For the first term, require that it starts with the search text
            pattern += "^" + QRegularExpression::escape(term);
        }
        else
        {
            pattern += "(?=.*" + QRegularExpression::escape(term) + ")";
        }
        index++;
    }
    // Combine the patterns with the logical AND operator
    pattern = "^" + pattern;

    // Create a regular expression for the pattern
    QRegularExpression regex(pattern, QRegularExpression::CaseInsensitiveOption);
    proxyModel->setFilterRegularExpression(regex);
    if (proxyModel->rowCount() > 0)
    {
        listView->show();
    }
    else
    {
        listView->hide();
    }
}
