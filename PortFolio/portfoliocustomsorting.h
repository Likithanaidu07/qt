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


class portfolioCustomSorting: public QObject
{
    Q_OBJECT
public:
    explicit portfolioCustomSorting(QObject *parent = 0,QString conne_name="main_db_conn");
    QVector<int> sortPortFolio(const QStringList &list);

 private:
    QVector<int> priorityColumnIdxs;
    QStringList sortOrders;




    int columnNameToIDx(QString);
    bool customComparator(const QString &a, const QString &b);


};

#endif // PORTFOLIOCUSTOMSORTING_H
