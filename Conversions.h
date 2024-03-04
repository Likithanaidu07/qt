#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <QtCore>

class Conversions
{
public:
    qint64 To_Epoch(const QDateTime &dt);
    QString ToDateTime(qint64 seconds);
    QString ToTime(qint64 seconds);
    QString To_DateTime(qint64 EpochSeconds);
    int To_Paise(qint64 rupees);
    QString To_Rupees(qint64 paise);
    double ToRupees(double paise);
};


#endif // CONVERSIONS_H
