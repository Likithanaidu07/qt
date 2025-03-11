#ifndef PORTFOLIOCUSTOMSORTING_H
#define PORTFOLIOCUSTOMSORTING_H

#include <QStringList>
#include <QVector>
#include <QString>
#include <QList>
#include <algorithm>
#include <iostream>
#include <QPair>
#include <QObject>
#define SORTING_NEW

#ifdef SORTING_NEW
class portfolioCustomSorting: public QObject
{
    Q_OBJECT
public:
    explicit portfolioCustomSorting(QObject *parent = 0,QString conne_name="main_db_conn");
    QVector<int> sortPortFolio(const QStringList& list);
    void loadSortConfig();



 private:
    QMap<QString, QString> extractFields(const QString& input);
     QStringList sortData(const QStringList& inputData, const QString& sortRule);
     bool do_compare(const QString& a, const QString& b, const QStringList& sortRules);
     QDate parseExpiry(const QString& expiry);
     QString sortRule;
};

#else

class portfolioCustomSorting: public QObject
{
    Q_OBJECT
public:
    explicit portfolioCustomSorting(QObject *parent = 0,QString conne_name="main_db_conn");
    QVector<int> sortPortFolio(const QStringList &list);
    void loadSortConfig();

 private:
    QVector<int> priorityColumnIdxs;
    QStringList sortOrders;




    int columnNameToIDx(QString);
    bool customComparator(const QString &a, const QString &b);


};

#endif // PORTFOLIOCUSTOMSORTING_H
#endif
