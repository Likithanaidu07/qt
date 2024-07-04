#include "portfoliocustomsorting.h"
#include "QStandardPaths"
#include "QSettings"


portfolioCustomSorting::portfolioCustomSorting(QObject *parent,QString conne_name):
        QObject(parent)
    {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/sort_data.dat", QSettings::IniFormat);
    if (settings.contains("portfolio_sort_data"))
    {
      QString saveStr =  settings.value("portfolio_sort_data").toString();
      QStringList saveStrList = saveStr.split(";");
      for(int i=0;i<saveStrList.size();i++){
          QStringList tok = saveStrList[i].split(":");
          if(tok.length()==2){

              QString columnToSort = tok[0]; //"Algo Status","Algo Name", "Strategy","Expiry","Start Strike","End Strike","Option Type"
              QString order = tok[1]; // Ascending or Descending or
              int idx = columnNameToIDx(columnToSort);
              if(idx!=-1){
                 priorityColumnIdxs.append(idx);
                 sortOrders.append(order);
              }
              else{
                  qDebug()<<"Error: portfolio_sort_data reade error, "<<columnToSort <<" not expected string";
              }
          }
          else{
              qDebug()<<"Error: portfolio_sort_data reade error, "<<saveStrList[i];
          }

      }
           // Sort
          // Create a list of pairs
          QVector<QPair<int, QString>> pairs;
          for (int i = 0; i < priorityColumnIdxs.size(); ++i) {
              pairs.append(qMakePair(priorityColumnIdxs[i], sortOrders[i]));
          }

          // Sort the list of pairs based on the first element of the pair
          std::sort(pairs.begin(), pairs.end(), [](const QPair<int, QString>& a, const QPair<int, QString>& b) {
              return a.first < b.first;
          });

          // Extract the sorted indices and orders
          for (int i = 0; i < pairs.size(); ++i) {
              priorityColumnIdxs[i] = pairs[i].first;
              sortOrders[i] = pairs[i].second;
          }



    }
}
int portfolioCustomSorting::columnNameToIDx(QString colStr){
    if(colStr=="Algo Status")
        return 0;
    else if(colStr=="Algo Status")
        return 1;
    else if(colStr=="Algo Name")
        return 2;
    else if(colStr=="Strategy")
        return 3;
    else if(colStr=="Expiry")
        return 4;
    else if(colStr=="Start Strike")
        return 5;
    else if(colStr=="End Strike")
        return 6;
    else if(colStr=="Option Type")
        return 7;
    else
     return -1;
}

bool portfolioCustomSorting::customComparator(const QString &a, const QString &b) {
    QStringList aParts = a.split('-');
    QStringList bParts = b.split('-');

    for (int i = 0; i < priorityColumnIdxs.size(); ++i) {
        int colIndex = priorityColumnIdxs[i];
        QString order = sortOrders[i];

        if (aParts.size() > colIndex && bParts.size() > colIndex) {
            if (order == "Ascending") {
                if (aParts[colIndex] < bParts[colIndex]) {
                    return true;
                } else if (aParts[colIndex] > bParts[colIndex]) {
                    return false;
                }
            } else if (order == "Descending") {
                if (aParts[colIndex] > bParts[colIndex]) {
                    return true;
                } else if (aParts[colIndex] < bParts[colIndex]) {
                    return false;
                }
            }
        }
    }

    return false; // If all columns are equal, keep original order
}


QVector<int> portfolioCustomSorting::sortPortFolio(const QStringList &list) {
    QVector<QPair<QString, int>> indexedList;
        for (int i = 0; i < list.size(); ++i) {
            indexedList.append(QPair<QString, int>(list[i], i));
        }

        std::sort(indexedList.begin(), indexedList.end(), [this](const QPair<QString, int> &a, const QPair<QString, int> &b) {
            return customComparator(a.first, b.first);
        });

        QVector<int> ranks(list.size());
        for (int rank = 0; rank < indexedList.size(); ++rank) {
            ranks[indexedList[rank].second] = rank;
        }

        return ranks;
}
