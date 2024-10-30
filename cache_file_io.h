#ifndef CACHE_FILE_IO_H
#define CACHE_FILE_IO_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QStringList>
class cache_file_io : public QObject
{
    Q_OBJECT
public:
    explicit cache_file_io(QObject *parent = nullptr);

    QStringList readFile_ExludedAlgos_ExchangePriceLimit();
    void writeFile_ExludedAlgos_ExchangePriceLimit(const QStringList &list);

signals:

private:
     QString appDataPath;

};

#endif // CACHE_FILE_IO_H
