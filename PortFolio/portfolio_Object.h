#ifndef PORTFOLIO_OBJECT_H
#define PORTFOLIO_OBJECT_H
#include <QString>
#include <QDateTime>
#include <cmath>
#include "defines.h"
#include "QAtomicInt"


struct TradeIdentification
{
    // Define TradeIdentification struct members here
};




class PortfolioObject
{
public:



    /*********This data need to be displayed in portfolio table*****************/
    double MPCLeverage = 10.0;
    double devicer;
    int decimal_precision;

    bool Status; // "Status"
    QString StatusVal;
    int PortfolioNumber; // "P.No";
    QString AlgoName; // new column added on Qt
    double SellMarketRate; //"-MR";
    QString SellAveragePrice; //"-APD"
    double SellPriceDifference; // "Price Diff";
    int SellTotalQuantity; // "Total Qty";
    int SellTradedQuantity; //"Traded Qty";
    int SellRemainingQuantity; //"Rem Qty";
    double BuyMarketRate;//"+MR";
    QString BuyAveragePrice;// "+APD";          // there is some MarketRateUpdator thread in C# code check it for update this value
    double BuyPriceDifference; // "Price Diff";
    int BuyTotalQuantity; // "Total Qty";
    int BuyTradedQuantity; // "Traded Qty";
    int BuyRemainingQuantity; // "Rem Qty";
    int OrderQuantity; // "Order Quantity";
    QString InstrumentName; // "Ins. Name";
    QDateTime ExpiryDateTime; // "Expiry"
    QString Leg1; //Leg1
    QString Leg2; //Leg2
    QString Leg3; //Leg3
    QString AdditionalData1; //Type
    QString PortfolioType; //"Portfolio Type"
    QString Price(){ //Price
        if (PortfolioTypeEnum == PortfolioType::BY || PortfolioTypeEnum == PortfolioType::BS || PortfolioTypeEnum == PortfolioType::BS1221)
            return QString::number(Leg2.toDouble() - Leg1.toDouble(), 'f',decimal_precision);
        if (PortfolioTypeEnum == PortfolioType::R3L)
            return QString("%1:%2:%3").arg(Leg1Multiplier).arg(Leg2Multiplier).arg(Leg3Multiplier);
        return "-";
    }
    QString Cost; //Cost
    QString FuturePrice; //"Future Price";
    QAtomicInt edting;
    /***********************************************************************************/




    QDateTime AverageUpdateTime;
    int FutToken;

    qint32 TraderID;
    qint32 ClientID;
    bool IsBroker;
    enum PortfolioType PortfolioTypeEnum;

    QString Leg4;
    QString Leg5;
    QString Leg6;
    int Leg1LotSizeOrg;
    int Leg2LotSize;
    int Leg3LotSize;
    int Leg4LotSize;
    int Leg5LotSize;
    int Leg6LotSize;
    int Leg1TokenNo;
    int Leg2TokenNo;
    int Leg3TokenNo;
    int Leg4TokenNo;
    int Leg5TokenNo;
    int Leg6TokenNo;
    qint64 ExpiryBinary;
    QString Expiry;
    QString ExpiryMonth;
    QString AdditionalData2;
    QString AdditionalData3;
    QString AdditionalData4;
    int Leg1Ratio;
    int Leg2Ratio;
    int Leg3Ratio;
    int Leg4Ratio;
    int Leg5Ratio;
    int Leg6Ratio;

    bool Highlight;
    TradeIdentification TradeIdentity;
    bool IsHighlighted;
    int Leg1Multiplier;
    int Leg2Multiplier;
    int Leg3Multiplier;
    int Leg4Multiplier;
    double BuyMRWithLeverage()
    {
        double withLeverage = BuyMarketRate;
        return withLeverage - std::abs(MPCLeverage * 1.0 * withLeverage / 100.0);
    }

    double SellMRWithLeverage()
    {
        double withLeverage = SellMarketRate;
        return withLeverage - std::abs(MPCLeverage * 1.0 * withLeverage / 100.0);
    }


    int GetLotSize()
    {
        int lotSize = Leg1LotSizeOrg;

        if (lotSize == 0)
        {
            lotSize = Leg2LotSize;
        }

        if (lotSize == 0)
        {
            lotSize = Leg3LotSize;
        }

        return lotSize;
    }

    int GetFirstToken()
    {
        int token = Leg1TokenNo;

        if (token == 0)
        {
            token = Leg2TokenNo;
        }

        if (token == 0)
        {
            token = Leg3TokenNo;
        }

        return token;
    }

    PortfolioObject()
    {
        Leg1Multiplier = 1;
        Leg2Multiplier = 1;
        Leg3Multiplier = 1;
        Leg4Multiplier = 1;
    }
    bool operator==(const PortfolioObject &other) const
    {
        // Compare all the member variables for equality
        return (
            Status == other.Status &&
            PortfolioNumber == other.PortfolioNumber &&
            SellMarketRate == other.SellMarketRate &&
            SellAveragePrice == other.SellAveragePrice &&
            SellPriceDifference == other.SellPriceDifference &&
            SellTotalQuantity == other.SellTotalQuantity &&
            SellTradedQuantity == other.SellTradedQuantity &&
            SellRemainingQuantity == other.SellRemainingQuantity &&
            BuyMarketRate == other.BuyMarketRate &&
            BuyAveragePrice == other.BuyAveragePrice &&
            BuyPriceDifference == other.BuyPriceDifference &&
            BuyTotalQuantity == other.BuyTotalQuantity &&
            BuyTradedQuantity == other.BuyTradedQuantity &&
            BuyRemainingQuantity == other.BuyRemainingQuantity &&
            OrderQuantity == other.OrderQuantity &&
            InstrumentName == other.InstrumentName &&
            ExpiryDateTime == other.ExpiryDateTime &&
            Leg1 == other.Leg1 &&
            Leg2 == other.Leg2 &&
            Leg3 == other.Leg3 &&
            AdditionalData1 == other.AdditionalData1 &&
            PortfolioType == other.PortfolioType &&
            Cost == other.Cost &&
            FuturePrice == other.FuturePrice &&
            AverageUpdateTime == other.AverageUpdateTime &&
            FutToken == other.FutToken &&
            TraderID == other.TraderID &&
            ClientID == other.ClientID &&
            IsBroker == other.IsBroker &&
            PortfolioTypeEnum == other.PortfolioTypeEnum &&
            Leg4 == other.Leg4 &&
            Leg5 == other.Leg5 &&
            Leg6 == other.Leg6 &&
            Leg1LotSizeOrg == other.Leg1LotSizeOrg &&
            Leg2LotSize == other.Leg2LotSize &&
            Leg3LotSize == other.Leg3LotSize &&
            Leg4LotSize == other.Leg4LotSize &&
            Leg5LotSize == other.Leg5LotSize &&
            Leg6LotSize == other.Leg6LotSize &&
            Leg1TokenNo == other.Leg1TokenNo &&
            Leg2TokenNo == other.Leg2TokenNo &&
            Leg3TokenNo == other.Leg3TokenNo &&
            Leg4TokenNo == other.Leg4TokenNo &&
            Leg5TokenNo == other.Leg5TokenNo &&
            Leg6TokenNo == other.Leg6TokenNo &&
            ExpiryBinary == other.ExpiryBinary &&
            Expiry == other.Expiry &&
            AdditionalData2 == other.AdditionalData2 &&
            AdditionalData3 == other.AdditionalData3 &&
            AdditionalData4 == other.AdditionalData4 &&
            Highlight == other.Highlight &&
            IsHighlighted == other.IsHighlighted &&
            Leg1Multiplier == other.Leg1Multiplier &&
            Leg2Multiplier == other.Leg2Multiplier &&
            Leg3Multiplier == other.Leg3Multiplier &&
            Leg4Multiplier == other.Leg4Multiplier
            );
    }

    bool operator!=(const PortfolioObject &other) const
    {
        return !(*this == other);
    }

    QString getPortFolioTypeString(QString portfolioType){
        QHash <QString, QString> PortFolioType_String;

        PortFolioType_String.insert(QString::number(PortfolioType::F2F), "FtF");
        PortFolioType_String.insert(QString::number(PortfolioType::BY), "BY");
        PortFolioType_String.insert(QString::number(PortfolioType::CR), "CR");
        //  PortFolioType_String.insert(QString::number(PortfolioType::CR_SKIP), "2L CR");

        PortFolioType_String.insert(QString::number(PortfolioType::JBX), "BOX");
        PortFolioType_String.insert(QString::number(PortfolioType::JBX_3L), "3L BOX");
        PortFolioType_String.insert(QString::number(PortfolioType::JBX_2L), "2L BOX");
        //  PortFolioType_String.insert(QString::number(PortfolioType::BOX_ML), "MBOX");
        PortFolioType_String.insert(QString::number(PortfolioType::BYC_6L), "BY 6L");
        PortFolioType_String.insert(QString::number(PortfolioType::BYC_5L), "BY 5L");
        PortFolioType_String.insert(QString::number(PortfolioType::BYC_4L), "BY 4L");

        PortFolioType_String.insert(QString::number(PortfolioType::BS1221), "BY 1221");
        PortFolioType_String.insert(QString::number(PortfolioType::BS), "BY 1331");
        PortFolioType_String.insert(QString::number(PortfolioType::R2L), "RATIO");

        if(PortFolioType_String.contains(portfolioType))
            return PortFolioType_String[portfolioType];
        else
            return "-";
    }
};



#endif // PORTFOLIO_OBJECT_H
