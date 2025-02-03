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

    explicit PortfolioObject();

    /*********This data need to be displayed in portfolio table*****************/
    double MPCLeverage = 10.0;
    double devicer;
    int decimal_precision;

    bool Status; // "Status"
    QString StatusVal;
    int PortfolioNumber; // "P.No";
    QString AlgoName; // new column added on Qt
    QString Alias;
    double MaxLoss;
    int Depth;
    double SellPriceDifferenceAsDouble;
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
    QString Cost; //Cost
    QString FuturePrice; //"Future Price";
    QAtomicInt edting;
    QString FutPrice;
    QString StockName;
    QString BidLeg;
  //  QString SkipMarketStrike;
    QString QuantityRatio;
    /***********************************************************************************/

    bool TradedHighlight; // this used to hilight portfolio table row if trade happens for that portfolio.
    QString AlgoNameForSorting; // this variable is used for custom soring algorithm

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
    double VolumeFreezeQty;

    bool operator ==(const PortfolioObject &other) const;
    bool operator !=(const PortfolioObject &other) const;
    QString getPortFolioTypeString(QString portfolioType);
    int GetFirstToken();
    int GetLotSize();
    double SellMRWithLeverage();
    double BuyMRWithLeverage();
    QString Price();
      QString user_id;
};



#endif // PORTFOLIO_OBJECT_H
