#ifndef PORTFOLIOPARSER_H
#define PORTFOLIOPARSER_H
#include <QString>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QString>
#include <QDateTime>
#include "portfolio_Object.h"
#include "defines.h"
#include "QDateTime"

class PortfolioParser
{
public:

    bool ToObject(QSqlQuery &query, PortfolioObject &obj, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, QHash<QString, PortfolioAvgPrice> averagePriceList, double devicer_, int decimal_precision_);
    QString get_Algo_Name(PortfolioType algo_type, int leg1_token_number, int leg2_token_number, int leg3_token_number,int leg4_token_number, double devicer, int decimal_precision,QString &Algo_Name_For_Sorting);
    QString fixDecimal(double num, int decimal_precision);
    void CalculateAveragePrice(PortfolioObject &portfolio, QHash<QString, PortfolioAvgPrice> averagePriceList, double devicer, int decimal_precision);
    void CalculatePriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision);
    void CalculateF2FPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision);
    void CalculateOPENBFLYPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision);
    void CalculateBFLYPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);
    void CalculateConRevPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);
    void CalculateBoxPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);
    void CalculateBS1221PriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);
    void CalculateBS1331PriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);

    void CalculateOpenBoxPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);
    void CalculateOpen2LPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type);

};


#endif // PORTFOLIOPARSER_H
