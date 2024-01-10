#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <QtCore>

class Conversions
{
public:
    qint64 To_Epoch(const QDateTime& dt)
    {
        QDateTime unixEpoch(QDate(1980, 1, 1), QTime(0, 0, 0));
        qint64 selectedExpiryEpoch = unixEpoch.secsTo(dt);
        return selectedExpiryEpoch;
    }

    QString ToDateTime(qint64 seconds)
    {
        QDateTime unixEpoch(QDate(1980, 1, 1), QTime(0, 0, 0));
        QDateTime dt = unixEpoch.addSecs(seconds);
        return dt.toString("yyyy-MM-dd HH:mm:ss");
    }

    QString ToTime(qint64 seconds)
    {
        QDateTime unixEpoch(QDate(1980, 1, 1), QTime(0, 0, 0));
        QDateTime dt = unixEpoch.addSecs(seconds);
        return dt.toString("HH:mm:ss");
    }

    QString To_DateTime(qint64 EpochSeconds)
    {
        QDateTime dt(QDate(1980, 1, 1), QTime(0, 0, 0), Qt::UTC);
        qint64 OrderCreatedTimeLocalUpdate = (EpochSeconds / 1000000) + 19800;
        QString OrderCreatedReadableUpdate = dt.addSecs(OrderCreatedTimeLocalUpdate).toString();
        QString timeOrderCreatedUpdate = OrderCreatedReadableUpdate.right(11);
        QString timeOnlyOrderCreatedUpdate = timeOrderCreatedUpdate.left(timeOrderCreatedUpdate.length() - 3);
        qint64 OrderCreatedMicrosUpdate = EpochSeconds % 1000000;
        QStringList OrderCreatedListUpdate;
        OrderCreatedListUpdate << timeOnlyOrderCreatedUpdate << QString::number(OrderCreatedMicrosUpdate);
        QString completeStringOrderCreatedUpdate = OrderCreatedListUpdate.join(":");
        return completeStringOrderCreatedUpdate;
    }

    QString To_Rupees(qint64 paise)
    {
        double Rupees_Decimal = static_cast<double>(paise) / 100.0;
        QString Rupees_String = QString::number(Rupees_Decimal, 'f', 2);
        return Rupees_String;
    }

    double ToRupees(double paise)
    {
        return paise / 100.0;
    }

    int To_Paise(qint64 rupees)
    {
        qint64 Decimal_Paise = static_cast<qint64>(rupees * 100);
        return static_cast<int>(Decimal_Paise);
    }
};


#endif // CONVERSIONS_H
