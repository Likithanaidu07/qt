#include "portfolio_Object.h"

PortfolioObject::PortfolioObject()
{
    Leg1Multiplier = 1;
    Leg2Multiplier = 1;
    Leg3Multiplier = 1;
    Leg4Multiplier = 1;

}

bool PortfolioObject::operator==(const PortfolioObject &other) const
{
    // Compare all the member variables for equality
    return (
        Status == other.Status &&
        PortfolioNumber == other.PortfolioNumber &&
        Alias == other.Alias &&
        MaxLoss  == other.MaxLoss &&
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
        TradedHighlight == other.TradedHighlight &&
        IsHighlighted == other.IsHighlighted &&
        Leg1Multiplier == other.Leg1Multiplier &&
        Leg2Multiplier == other.Leg2Multiplier &&
        Leg3Multiplier == other.Leg3Multiplier &&
        Leg4Multiplier == other.Leg4Multiplier
        );
}

bool PortfolioObject::operator!=(const PortfolioObject &other) const
{
    return !(*this == other);
}

QString PortfolioObject::getPortFolioTypeString(QString portfolioType)
{
    QHash <QString, QString> PortFolioType_String;

    PortFolioType_String.insert(QString::number(PortfolioType::F2F), "FtF");
    PortFolioType_String.insert(QString::number(PortfolioType::BY), "BY");
    PortFolioType_String.insert(QString::number(PortfolioType::CR), "CR");
    PortFolioType_String.insert(QString::number(PortfolioType::CR_JELLY), "CR_JELLY");

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
    PortFolioType_String.insert(QString::number(PortfolioType::BFLY_BID), "BLFY BID");
    PortFolioType_String.insert(QString::number(PortfolioType::F1_F2), "F1 F2");

    if(PortFolioType_String.contains(portfolioType))
        return PortFolioType_String[portfolioType];
    else
        return "-";
}

QString PortfolioObject::Price()
{
    //Price
    if (PortfolioTypeEnum == PortfolioType::BY || PortfolioTypeEnum == PortfolioType::BS || PortfolioTypeEnum == PortfolioType::BS1221
        || PortfolioTypeEnum == PortfolioType::BFLY_BID)
        return QString::number(Leg2.toDouble() - Leg1.toDouble(), 'f',decimal_precision);
    if (PortfolioTypeEnum == PortfolioType::R3L)
        return QString("%1:%2:%3").arg(Leg1Multiplier).arg(Leg2Multiplier).arg(Leg3Multiplier);
    return "-";
}

double PortfolioObject::BuyMRWithLeverage()
{
    double withLeverage = BuyMarketRate;
    return withLeverage - std::abs(MPCLeverage * 1.0 * withLeverage / 100.0);
}

double PortfolioObject::SellMRWithLeverage()
{
    double withLeverage = SellMarketRate;
    return withLeverage - std::abs(MPCLeverage * 1.0 * withLeverage / 100.0);
}


int PortfolioObject::GetLotSize()
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

int PortfolioObject::GetFirstToken()
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

