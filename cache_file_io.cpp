#include "cache_file_io.h"
#include "QStandardPaths"
#include "QDebug"
cache_file_io::cache_file_io(QObject *parent)
    : QObject{parent}
{

       appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
}


void cache_file_io::writeFile_ExludedAlgos_ExchangePriceLimit(const QStringList &list)
{
    const QString &filePath = appDataPath+"/exludedAlgos_ExchangePriceLimit.dat";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << list.join(",");  // Join QStringList with commas
        file.close();
    }
    else{
        qDebug()<<"writeFile_ExludedAlgos_ExchangePriceLimit: error opening file!";
    }
}

QStringList cache_file_io::readFile_ExludedAlgos_ExchangePriceLimit()
{
    const QString &filePath = appDataPath+"/exludedAlgos_ExchangePriceLimit.dat";



    QStringList list;
    QFile file(filePath);

       if (!file.exists()) {
           return list;  // Return an empty list if file doesn't exist
       }

       if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           QTextStream in(&file);
           QString line = in.readLine();  // Read the entire line
           list = line.split(",");        // Split by commas
           file.close();
       } else {
           qDebug()<<"readFile_ExludedAlgos_ExchangePriceLimit: error opening file! "<<filePath;

       }

       return list;
}
