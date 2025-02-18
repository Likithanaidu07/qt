#include "cache_file_io.h"
#include "QStandardPaths"
#include "QDebug"
cache_file_io::cache_file_io(QObject *parent)
    : QObject{parent}
{

       appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
}


void cache_file_io::over_writeTradedDataList_DisabledOn_MaxLoss_Thr_Reach(const QStringList &list)
{
    const QString &filePath = appDataPath+"/tradedDataList_DisabledOn_MaxLoss_Thr_Reach.dat";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << list.join(",");  // Join QStringList with commas
        file.close();
    }
    else{
        qDebug()<<"writeTradedDataList_DisabledOn_MaxLoss_Thr_Reach: error opening file!";
    }
}

QStringList cache_file_io::readTradedDataList_DisabledOn_MaxLoss_Thr_Reach()
{
    const QString &filePath = appDataPath+"/tradedDataList_DisabledOn_MaxLoss_Thr_Reach.dat";



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
           qDebug()<<"readTradedDataList_DisabledOn_MaxLoss_Thr_Reach: error opening file! "<<filePath;

       }

       return list;
}
