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
#include "QTimer"
#include "defines.h"

class CustomSortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    CustomSortFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
    void invalidateCustomFilter() {
            this->invalidateFilter();
     }
protected:
     /* bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {
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
      }*/

   /* bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {
            QString leftKey = sourceModel()->data(left, ConvertAlog_Model_Roles::CustomSortingDataRole).toString();
            QString rightKey = sourceModel()->data(right, ConvertAlog_Model_Roles::CustomSortingDataRole).toString();
            return leftKey < rightKey;
        }*/

 bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {
            QString leftKey = sourceModel()->data(left, ConvertAlog_Model_Roles::CustomSortingDataRole).toString();
            QString rightKey = sourceModel()->data(right, ConvertAlog_Model_Roles::CustomSortingDataRole).toString();

            QStringList leftParts = leftKey.split('-');
            QStringList rightParts = rightKey.split('-');

            // Compare prefix
            if (leftParts[0] != rightParts[0]) {
                return leftParts[0] < rightParts[0];
            }

            // Compare date
            if (leftParts[1] != rightParts[1]) {
                return leftParts[1] < rightParts[1];
            }

            // Compare numeric value if present
            if (leftParts.size() == 3 && rightParts.size() == 3) {
                bool okLeft, okRight;
                double leftNumeric = leftParts[2].toDouble(&okLeft);
                double rightNumeric = rightParts[2].toDouble(&okRight);
                if (okLeft && okRight) {
                    return leftNumeric < rightNumeric;
                }
            }

            // If only one has a numeric part, treat it as greater
            if (leftParts.size() == 3) {
                return false;
            }
            if (rightParts.size() == 3) {
                return true;
            }

            return false;
        }





};

class CustomSearchWidget : public QWidget
{
    Q_OBJECT
public:

    CustomSearchWidget(QListView *customListView, QStandardItemModel *_model, QWidget *parent = nullptr);
    QString name;
    bool sortAscendingOrder;

public slots:

    void filterItems(const QString &text);
    void do_filterItems(const QString &text);

private:

    QListView *listView;
    QStandardItemModel *model;
    QStringList completionData;
    QStringListModel *stringListModel;
    QTimer *debounceTimer;  // Add this line
    QString currentText;
    QString id;
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
