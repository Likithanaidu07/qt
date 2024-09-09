#ifndef CONVERT_ALGO_SHARED_VAR_H
#define CONVERT_ALGO_SHARED_VAR_H

#include "defines.h"
#include <QHash>
#include <QString>
#include <QList>



class AddAlgoSharedVar {
public:
    static AddAlgoSharedVar& getInstance();

    QHash<QString, contract_table> contract_table_hash;
   // QHash<QString, contract_table> curr_contract_table_hash; // this contract table for curr
   // QHash<QString, contract_table> fo_contract_table_hash; // this contract table for fo
    QStringList FO_F2F_data_list_Sorted_Key;
    QStringList FO_BFLY_data_list_Sorted_Key;
    QStringList FO_BFLY_BID_data_list_Sorted_Key;

    //  QStringList CURR_F2F_data_list_Sorted_Key;
    // QStringList CURR_BFLY_data_list_Sorted_Key;

    int unique_id;
    int ROW_LIMIT;
    double strike_price_devider;
    double devicer;
    double decimal_precision;
    QList<algo_data_to_insert> algo_data_list;
    QString foo_user_id;
    int MaxPortfolioCount;
    QString exchange;

private:
    AddAlgoSharedVar();

    // Disable copy constructor and assignment operator
    AddAlgoSharedVar(const AddAlgoSharedVar&) = delete;
    AddAlgoSharedVar& operator=(const AddAlgoSharedVar&) = delete;
};

#endif // CONVERT_ALGO_SHARED_VAR_H
