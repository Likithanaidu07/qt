#pragma once

#include <QObject>
#include <QStringListModel>
#include <QCompleter>
#include <QListView>
#include <QSortFilterProxyModel>
#include <ui_convert_algo_win.h>
#include <QStandardItemModel>

class CustomSearchWidget : public QWidget
{
    Q_OBJECT
public:

    CustomSearchWidget(QListView *customListView, QStandardItemModel *_model, QWidget *parent = nullptr)
        : QWidget(parent), listView(customListView),model(_model)
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

        QStringList items = completionData;
        filteredData = completionData;

        stringListModel = new QStringListModel(completionData, this);

        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);  // Set case insensitivity
        proxyModel->setSourceModel(stringListModel);

        // proxyModel->setSourceModel(model);
        listView->setModel(proxyModel);
        listView->hide();

    }

private slots:
    void filterItems(const QString &text);
    void newfilterItems();

private:

    QListView *listView;
    QStandardItemModel *model;
    QLineEdit *lineEdit;
    QStringList filteredData;  // Separate list for currently displayed items
    QStringList filteredData1;  // Separate list for currently displayed items
    QStringList completionData;

    QStringListModel *stringListModel;
};



class custom_q_completer : public QCompleter
{
    Q_OBJECT
public:
    explicit custom_q_completer(QObject *parent = nullptr);

    // explicit custom_q_completer(QStandardItemModel *data, QObject *parent = nullptr);

    static const int CompleteRole;
    QString get_foo_token_number_for_selected(const QModelIndex &index) const;
    virtual QString pathFromIndex(const QModelIndex &index) const;

signals:

private slots:

    void newfilterItems(QString text) {

        QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(this->model());
        if (proxyModel)
        {
            if (!text.isEmpty())
            {
                proxyModel->setFilterRegularExpression(QRegularExpression(QRegularExpression::escape(text), QRegularExpression::CaseInsensitiveOption));
                if (proxyModel->rowCount() == 0)
                {
                    proxyModel->setFilterWildcard(text);
                }
            }
            else
            {
                proxyModel->setFilterWildcard("");
            }
        }

        if (!text.isEmpty()) {

        } else {
            listView->hide();
        }

    }

// private slots:
    // void filterStartStrikeCompleter(const QString &text) {

    //         // Filter completion data based on substrings
    //         QStandardItemModel *filteredData = new QStandardItemModel;
    //         QString userInput = text.trimmed().toLower();
    //         if (startStrikeModelData) {
    //             for (int row = 0; row < startStrikeModelData->rowCount(); ++row) {
    //                 QModelIndex index = startStrikeModelData->index(row, 0); // Assuming single column model
    //                 QString itemText = startStrikeModelData->data(index, Qt::DisplayRole).toString();

    //                 // qInfo() << "===== data 1" <<startStrikeModelData->item(row, Qt::UserRole + 1);
    //                 // qInfo() << "===== data 2" <<startStrikeModelData->item(row,0)->data(Qt::UserRole+1);


    //                 if (itemText.toLower().contains(userInput))
    //                 {
    //                     QStandardItem *item = new QStandardItem;
    //                     item->setText(itemText);
    //                        item->setData(row, Qt::UserRole + 1);
    //                     filteredData->appendRow(item);
    //                 }
    //             }
    //         }
    //         else {
    //             qDebug() << "Error: completionData is null!";
    //             // Handle the error appropriately
    //         }

    //         setModel(filteredData);
    //     }

    // void filterEndStrikeCompleter(const QString &text) {{

    //         // Filter completion data based on substrings
    //         QStandardItemModel *filteredData = new QStandardItemModel;
    //         QString userInput = text.trimmed().toLower();
    //         if (startStrikeModelData) {
    //             for (int row = 0; row < startStrikeModelData->rowCount(); ++row) {
    //                 QModelIndex index = startStrikeModelData->index(row, 0); // Assuming single column model
    //                 QString itemText = startStrikeModelData->data(index, Qt::DisplayRole).toString();

    //                 // qInfo() << "===== data 1" <<startStrikeModelData->item(row, Qt::UserRole + 1);
    //                 // qInfo() << "===== data 2" <<startStrikeModelData->item(row,0)->data(Qt::UserRole+1);


    //                 if (itemText.toLower().contains(userInput))
    //                 {
    //                     QStandardItem *item = new QStandardItem;
    //                     item->setText(itemText);
    //                     item->setData(row, Qt::UserRole + 1);
    //                     filteredData->appendRow(item);
    //                 }
    //             }
    //         }
    //         else {
    //             qDebug() << "Error: completionData is null!";
    //             // Handle the error appropriately
    //         }

    //         setModel(filteredData);
    //     }}

private:
    QListView *listView;

    // QStandardItemModel *startStrikeModelData;

};
