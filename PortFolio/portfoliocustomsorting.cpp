#include "portfoliocustomsorting.h"
#include "QStandardPaths"
#include "QSettings"
#include <QDate>


portfolioCustomSorting::portfolioCustomSorting(QObject *parent,QString conne_name):
        QObject(parent)
{
    //loadSortConfig();
}

void portfolioCustomSorting::loadSortConfig(){

    priorityColumnIdxs.clear();
    sortOrders.clear();

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
    else if(colStr=="Algo Type")
        return 1;
    else if(colStr=="Instrument Name")
        return 2;
    else if(colStr=="Expiry")
        return 3;
    else if(colStr=="Middle Strike")
        return 4;
    else if(colStr=="Strike Difference")
        return 5;
    else if(colStr=="Option Type")
        return 6;
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

            auto aVal = aParts[colIndex];
            auto bVal = bParts[colIndex];

            // Handle the special case for PE/CE/ZE in case of CR jelly and CR where optiontype is not there and for camparison a tmp value of ZE given, also the strike diff give 0 so it will automatticaly sodrt
                        if ((aVal == "ZE" && (bVal == "PE" || bVal == "CE")) ||
                            (bVal == "ZE" && (aVal == "PE" || aVal == "CE"))) {
                            // Always put ZE last
                            return (aVal == "ZE") ? false : true;
                        }

            //means its "algostatus", "Middle Strike" or "strike diff" so it shold consider as number sorting, rest all should be
            // alphabetical sort
            bool isNumaric = false;
            if(colIndex==0 ||colIndex==4 || colIndex==5){
               isNumaric = true;
            }

            if (order == "Ascending") {
                // if number convert to double and compare
                if(isNumaric){
                    if (aParts[colIndex].toDouble() < bParts[colIndex].toDouble()) {
                        return true;
                    } else if (aParts[colIndex].toDouble() > bParts[colIndex].toDouble()) {
                        return false;
                    }
                }
                // else do alpahabetci compare.
                else{
                    if (aParts[colIndex] < bParts[colIndex]) {
                        return true;
                    } else if (aParts[colIndex] > bParts[colIndex]) {
                        return false;
                    }
                }

            }
            else if (order == "Descending") {
                if(isNumaric){
                    if (aParts[colIndex].toDouble() > bParts[colIndex].toDouble()) {
                        return true;
                    } else if (aParts[colIndex].toDouble() < bParts[colIndex].toDouble()) {
                        return false;
                    }
                }
                else{
                    if (aParts[colIndex] > bParts[colIndex]) {
                        return true;
                    } else if (aParts[colIndex] < bParts[colIndex]) {
                        return false;
                    }
                }

            }


            //CE should come first so sort Ascending order
            /*if (order == "CE") {
                if (aParts[colIndex] < bParts[colIndex]) {
                    return true;
                } else if (aParts[colIndex] > bParts[colIndex]) {
                    return false;
                }
            }
            //PE should come first so sort Descending order
            else if (order == "PE") {
                if (aParts[colIndex] > bParts[colIndex]) {
                    return true;
                } else if (aParts[colIndex] < bParts[colIndex]) {
                    return false;
                }
            }*/


            // CE should come first (Ascending order)
                        if (order == "CE") {
                            if (aVal < bVal) {
                                return true;
                            } else if (aVal > bVal) {
                                return false;
                            }
                        }
                        // PE should come first (Descending order)
                        else if (order == "PE") {
                            if (aVal > bVal) {
                                return true;
                            } else if (aVal < bVal) {
                                return false;
                            }
                        }



            //Disabled algo(status=0) should come first so sort Ascending order
            if (order == "Disabled") {

                if (aParts[colIndex].toDouble() < bParts[colIndex].toDouble()) {
                    return true;
                } else if (aParts[colIndex].toDouble() > bParts[colIndex].toDouble()) {
                    return false;
                }
            }
            //Enabled algo(status=01) should come first so sort Descending order
            else if (order == "Enabled") {
                if (aParts[colIndex].toDouble() > bParts[colIndex].toDouble()) {
                    return true;
                } else if (aParts[colIndex].toDouble() < bParts[colIndex].toDouble()) {
                    return false;
                }
            }
            else if (order == "Sort by Far Date" || order == "Sort By Near Date") {
                 QDate dateA = QDate::fromString(aParts[colIndex], "ddMMMyyyy");
                 QDate dateB = QDate::fromString(bParts[colIndex], "ddMMMyyyy");
                 if (dateA.isValid() && dateB.isValid()) {
                    if (order == "Sort by Far Date") {
                        if (dateA > dateB) {
                            return true;
                        }
                        else if (dateA < dateB) {
                             return false;
                         }
                    }
                    else if (order == "Sort By Near Date") {
                        if (dateA < dateB) {
                            return true;
                        }
                        else if (dateA > dateB) {
                            return false;
                        }
                    }
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
