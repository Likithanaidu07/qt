#include "portfoliocustomsorting.h"
#include "QStandardPaths"
#include "QSettings"
#include <QDate>


portfolioCustomSorting::portfolioCustomSorting(QObject *parent,QString conne_name):
        QObject(parent)
{
    //loadSortConfig();
}

#ifdef SORTING_NEW


void portfolioCustomSorting::loadSortConfig(){
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
        QSettings settings(appDataPath+"/sort_data.dat", QSettings::IniFormat);
        if (settings.contains("portfolio_sort_data"))
        {
           sortRule =  settings.value("portfolio_sort_data").toString();
        }
}
// Helper function to extract fields from the input string
QMap<QString, QString> portfolioCustomSorting::extractFields(const QString& input) {
    QMap<QString, QString> fields;
    QStringList parts = input.split("-");
    fields["Status"] = parts[0];
    fields["AlgoType"] = parts[1];
    fields["InstrumentName"] = parts[2];
    fields["Expiry"] = parts[3];
    fields["MiddleStrike"] = parts[4];
    fields["StrikeDifference"] = parts[5];
    fields["OptionType"] = parts[6];
    return fields;
}

// Helper function to convert expiry string (e.g., "13MAR") to QDate
QDate portfolioCustomSorting::parseExpiry(const QString& expiry) {
    QString format = "ddMMMyyyy"; // Format for dates like "13MAR"
    return QDate::fromString(expiry, format);
}

bool portfolioCustomSorting::do_compare(const QString& a, const QString& b, const QStringList& sortRules) {
    QMap<QString, QString> fieldsA = extractFields(a);
    QMap<QString, QString> fieldsB = extractFields(b);

    for (const QString& rule : sortRules) {
        QStringList ruleParts = rule.split(":");
        QString key = ruleParts[0];
        QString order = ruleParts[1];

        if (key == "Instrument Name") {
            if (fieldsA["InstrumentName"] != fieldsB["InstrumentName"]) {
                return (order == "Ascending") ? fieldsA["InstrumentName"] < fieldsB["InstrumentName"] : fieldsA["InstrumentName"] > fieldsB["InstrumentName"];
            }
        } else if (key == "Option Type") {
            QString optionTypeA = fieldsA["OptionType"];
            QString optionTypeB = fieldsB["OptionType"];

            // Handle ZE (always comes last)
            if (optionTypeA == "ZE" && optionTypeB != "ZE") {
                return false; // ZE comes after CE/PE
            }
            if (optionTypeB == "ZE" && optionTypeA != "ZE") {
                return true; // CE/PE comes before ZE
            }

            // Handle CE and PE based on the order
            if (optionTypeA != optionTypeB) {
                if (order == "PE") {
                    return (optionTypeA == "PE"); // PE comes first if order is PE
                } else {
                    return (optionTypeA == "CE"); // CE comes first if order is CE
                }
            }
        } else if (key == "Middle Strike") {
            if (fieldsA["MiddleStrike"] != fieldsB["MiddleStrike"]) {
                return (order == "Ascending") ? fieldsA["MiddleStrike"].toInt() < fieldsB["MiddleStrike"].toInt() : fieldsA["MiddleStrike"].toInt() > fieldsB["MiddleStrike"].toInt();
            }
        } else if (key == "Algo Status") {
            if (fieldsA["Status"] != fieldsB["Status"]) {
                return (order == "Enabled") ? fieldsA["Status"] == "1" : fieldsA["Status"] != "1";
            }
        } else if (key == "Algo Type") {
            if (fieldsA["AlgoType"] != fieldsB["AlgoType"]) {
                return (order == "Ascending") ? fieldsA["AlgoType"] < fieldsB["AlgoType"] : fieldsA["AlgoType"] > fieldsB["AlgoType"];
            }
        } else if (key == "Expiry") {
            QDate dateA = parseExpiry(fieldsA["Expiry"]);
            QDate dateB = parseExpiry(fieldsB["Expiry"]);
            if (dateA != dateB) {
                return (order == "Sort by Far Date") ? dateA > dateB : dateA < dateB;
            }
        } else if (key == "Strike Difference") {
            if (fieldsA["StrikeDifference"] != fieldsB["StrikeDifference"]) {
                return (order == "Ascending") ? fieldsA["StrikeDifference"].toInt() < fieldsB["StrikeDifference"].toInt() : fieldsA["StrikeDifference"].toInt() > fieldsB["StrikeDifference"].toInt();
            }
        }
    }
    return false;
}

// Main function to sort the input data based on the sortRule
QStringList portfolioCustomSorting::sortData(const QStringList& inputData, const QString& sortRule) {
   // QStringList sortRules = sortRule.split(";");
    QStringList sortRules = sortRule.split(";", Qt::SkipEmptyParts);

    QStringList sortedData = inputData;

    std::sort(sortedData.begin(), sortedData.end(), [this, &sortRules](const QString& a, const QString& b) {
        return this->do_compare(a, b, sortRules);
    });

    return sortedData;
}


// Function to return the rank of each item in the original list
QVector<int> portfolioCustomSorting::sortPortFolio(const QStringList& list) {
    QVector<int> ranks(list.size()); // Vector to store ranks
    if (sortRule.isEmpty() || sortRule == "") {
        // If sortRule is empty, assign ranks in the order of the original list
        for (int i = 0; i < list.size(); ++i) {
            ranks[i] = i; // Assign ranks as 0, 1, 2, ...
        }
        return ranks;
    }

    QStringList sortedList = sortData(list, sortRule); // Sort the list

    // Create a map to store the index of each item in the sorted list
    QMultiMap<QString, int> sortedIndexMap;
    for (int i = 0; i < sortedList.size(); ++i) {
        sortedIndexMap.insert(sortedList[i], i);
    }

    // Assign ranks based on the original list
    for (int i = 0; i < list.size(); ++i) {
        QString item = list[i];
        QList<int> indices = sortedIndexMap.values(item);
        if (!indices.isEmpty()) {
            ranks[i] = indices.takeFirst(); // Assign the first available rank
            sortedIndexMap.remove(item, ranks[i]); // Remove the assigned rank from the map
        }
    //    qDebug()<<"input: "<<list[i]<<" rank: "<<ranks[i];


    }

    return ranks;
}

#else
void portfolioCustomSorting::loadSortConfig(){

    priorityColumnIdxs.clear();
    sortOrders.clear();

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/Data";
    QSettings settings(appDataPath+"/sort_data.dat", QSettings::IniFormat);
    if (settings.contains("portfolio_sort_data"))
    {
      QString saveStr =  settings.value("portfolio_sort_data").toString();
      if(saveStr!=""){
          QStringList saveStrList = saveStr.split(";");
          for(int i=0;i<saveStrList.size();i++){
              QStringList tok = saveStrList[i].split(":");
              if(tok.length()==2){

                  QString columnToSort = tok[0]; //"Algo Status","Algo Name", "Strategy","Expiry","Start Strike","End Strike","Option Type"
                  QString order = tok[1]; // Ascending or Descending or0000000000000000
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
//                        if (order == "CE") {
//                            if (aVal < bVal) {
//                                return true;
//                            } else if (aVal > bVal) {
//                                return false;
//                            }
//                        }
//                        // PE should come first (Descending order)
//                        else if (order == "PE") {
//                            if (aVal > bVal) {
//                                return true;
//                            } else if (aVal < bVal) {
//                                return false;
//                            }
//                        }



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
#endif
