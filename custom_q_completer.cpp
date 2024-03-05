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

void CustomSearchWidget::filterItems(const QString &text)
{
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(listView->model());
    if (proxyModel)
    {

        if (!text.isEmpty())
        {
            if (text.contains(" "))
            {
                stringListModel->setStringList(filteredData1);

                // Split the text by spaces
                QStringList parts = text.split(" ", Qt::SkipEmptyParts);
                // Handle each part separately

                qDebug() << " text "<< parts;

                for (const QString &part : parts)
                {
                    proxyModel->setFilterRegularExpression(QRegularExpression(".*" + QRegularExpression::escape(part) + ".*", QRegularExpression::CaseInsensitiveOption));

                    // proxyModel->setFilterRegularExpression(QRegularExpression(".*" + QRegularExpression::escape(text) + ".*", QRegularExpression::CaseInsensitiveOption));
                    // if (proxyModel->rowCount() != 0) {
                    //     filteredData.clear();
                    // }
                    // Update filteredData with new items
                    qDebug() << " row count "<< proxyModel->rowCount() ;

                    if (proxyModel->rowCount() != 0)
                    {
                        filteredData.clear();
                    }


                    for (int row = 0; row < proxyModel->rowCount(); ++row)
                    {
                        QModelIndex index = proxyModel->index(row, 0);
                        QVariant data = index.data(Qt::DisplayRole);
                        if (data.isValid())
                        {
                            filteredData.append(data.toString());
                        }
                    }
                }
            }
            else
            {
                stringListModel->setStringList(completionData);

                // If no spaces, use the original logic
                proxyModel->setFilterRegularExpression(QRegularExpression("^" + QRegularExpression::escape(text), QRegularExpression::CaseInsensitiveOption));
                if (proxyModel->rowCount() == 0)
                {
                    proxyModel->setFilterWildcard(text);
                }

                if (proxyModel->rowCount() != 0)
                {
                    filteredData.clear();
                    filteredData1.clear();
                }

                for (int row = 0; row < proxyModel->rowCount(); ++row)
                {
                    QModelIndex index = proxyModel->index(row, 0);
                    QVariant data = index.data(Qt::DisplayRole);
                    if (data.isValid()) {
                        filteredData.append(data.toString());
                        filteredData1.append(data.toString());

                    }
                }
            }
        }
        else
        {
            // If text is empty, clear the filter
            proxyModel->setFilterWildcard("");
            // Reset filteredData to the original list
            filteredData = completionData;
        }
    }

    // Update the underlying model with filteredData
    stringListModel->setStringList(filteredData);

    // Show or hide the list view based on the filter result
    if (text.trimmed().isEmpty()) {
        listView->hide();
    } else {
        listView->show();
    }
}

void CustomSearchWidget::newfilterItems()
{
    QString text = "";

    // e.g. casting to the class you know its connected with
    QLineEdit* lineEdit_1 = qobject_cast<QLineEdit*>(sender());
    if( lineEdit_1 != NULL )
    {
        text = lineEdit_1->text();
    }


    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(listView->model());
    if (proxyModel) {
        if (!text.isEmpty()) {
            proxyModel->setFilterRegularExpression(QRegularExpression("^" + QRegularExpression::escape(text), QRegularExpression::CaseInsensitiveOption));
            if (proxyModel->rowCount() == 0) {
                proxyModel->setFilterWildcard(text);
            }
        } else {
            proxyModel->setFilterWildcard("");
        }
    }
    if (!text.isEmpty()) {
        listView->show();
    } else {
        listView->hide();
    }
}
