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

    CustomSearchWidget(QListView *customListView, QStandardItemModel *_model, QWidget *parent = nullptr);
private slots:
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
