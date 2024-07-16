#pragma once

#include <QObject>
#include <QStringListModel>
#include <QCompleter>
#include <QListView>
#include <QSortFilterProxyModel>
#include <ui_convert_algo_win.h>
#include <QStandardItemModel>
#include "QSortFilterProxyModel"
#include "QDate"

class CustomSortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    CustomSortFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {
        QString leftText = sourceModel()->data(left, Qt::DisplayRole).toString();
           QString rightText = sourceModel()->data(right, Qt::DisplayRole).toString();

           QString leftPrefix = leftText.split(" ").first();
           QString rightPrefix = rightText.split(" ").first();

           if (leftPrefix == rightPrefix) {
               QDate leftDate = sourceModel()->data(left, Qt::UserRole + 2).toDate();
               QDate rightDate = sourceModel()->data(right, Qt::UserRole + 2).toDate();
               return leftDate < rightDate;
           } else {
               return leftPrefix < rightPrefix;
           }
    }

};

class CustomSearchWidget : public QWidget
{
    Q_OBJECT
public:

    CustomSearchWidget(QListView *customListView, QStandardItemModel *_model, QWidget *parent = nullptr);

public slots:

    void filterItems(const QString &text);

private:

    QListView *listView;
    QStandardItemModel *model;
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

private:
    QListView *listView;

    // QStandardItemModel *startStrikeModelData;

};
