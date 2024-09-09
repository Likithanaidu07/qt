#ifndef DEFINES_H
#define DEFINES_H
#include <QObject>
#include "QVariant"
#include <QString>
//#include <defines.h>

#define ContractFilePathFTP "contracts.bin"
extern QAtomicInt data_exchangeTimestamp;
extern QAtomicInt portfolio_table_updating_db;
extern std::atomic<int> portfolio_table_slected_idx_for_editing;
extern QString userNameLogged;

//extern  QString MARKET_TYPE; // This will be either fo or cds. will load from setting.ini when app start
#define CDS_DEVICER 10000000.0
#define CDS_DECIMAL_PRECISION 4
#define CDS_PRICE_DIFF_INCREMENTER 0.0025

#define FO_DEVICER 100.0
#define FO_DECIMAL_PRECISION 2
#define FO_PRICE_DIFF_INCREMENTER 0.05


enum nav_direction{
    nav_forward=1,
    nav_backward=2
};
enum portfolio_status{
    Filled = 0,
    DisabledByUser = 1,
    Active = 2,
};
enum SwitchState {
    Unchecked=0,
    Checked=1,
    alsochecked=2,
};
/*enum PortfolioStatus
{
    Active,
    DisabledByUser,
    Filled
};*/

enum IndicesDataWidgetID{
    WIDGET_NIFTY_50=0,
    WIDGET_NIFTY_BANK=1,
    WIDGET_NIFTY_FINANCIAL_SERVICES=2,
    WIDGET_INDIA_VIX=3,
};

enum PortfolioData_Idx
{
    _Status = 0,
    _PortfolioNumber,
    _AlgoName,
    _Alias,
    _BuyMarketRate,
    _BuyAveragePrice,
    _BuyPriceDifference,
    _BuyTotalQuantity,
    _BuyTradedQuantity,
    _BuyRemainingQuantity,
    _SellMarketRate,
    _SellAveragePrice,
    _SellPriceDifference,
    _SellTotalQuantity,
    _SellTradedQuantity,
    _SellRemainingQuantity,
    _OrderQuantity,
    _MaxLoss,
    _ExpiryDateTime,
    _Cost, //currently up to this the data is displayed on portfolio table in the same order as the enum
    _QuantityRatio,
 //   _SkipMarketStrike,
    _BidLeg,
    _FuturePrice,
    _InstrumentName,
    _Leg1,
    _Leg2,
    _Leg3,
    _AdditionalData1,
    _PortfolioType,
    _Price,

};

static int CombinedTrackerData_Length = 15;
enum CombinedTrackerData_Idx{
    CT_Rule_No=0,
    CT_Symbol,
    CT_Strategy,
    CT_Expiry,
    CT_Type,
    CT_Left_Leg,
    CT_Center_Leg,
    CT_Right_Leg,
    CT_Bid_Qty,
    CT_Bid_Lots,
    CT_Bid_Avg_Price,
    CT_Ask_Qty,
    CT_Ask_Lots,
    CT_Ask_Avg_Price,
    CT_Profit
};

enum OrderBook_Idx{
    OrderId_OB=0,
    AlgoName_OB,
    UserPrice_OB,
    ExchPrice_OB,
    Jackpot_OB,
    TradedLot_OB,
    RemainingLot_OB,
    TradeTime_OB,
    BidLegState_OB,
    Leg1State_OB,
    Leg3State_OB,
    AlgoNo_OB,
    BidLegStateVal_OB,
    Leg1StateVal_OB,
    Leg3StateVal_OB,
    Expiry_OB,
    BuyorSell_OB,
    LotSize_OB,
    TraderData_OB
};

enum Liners_Idx{
    AlgoId = 0,
    AlgoName,
    BuyAvgPrice,
    BuyQtyinLots,
    SellAvgPrice,
    SellQtyinLots,
    NetQty,
    Profit,
};

enum NET_POS
{
    SNo_NP = 0,
    StockName_NP,
    BuyTotalLot_NP,
    SellTotalLot_NP,
    BuyValue_NP,
    SellValue_NP,
    BuyAvgPrice_NP,
    SellAvgPrice_NP,
    NetQty_NP,
    Profit_NP,
    MTM,
    TokenNo_NP,
};

enum PortfolioType
{
    F2F = 1,
    BY,

    BOX,
    BS,
    BS1221,
    BX_3L,
    BX_2L,
    BYC_4L,
    BYC_5L,
    BYC_6L,

    BX_MKT = 13,

    R2L = 200,
    OPEN_BY = 205, // open btfy
    OPEN_BOX = 206, // open btfy
    F1_F2 = 207,
    BFLY_BID = 250,
    CR = 251,
    CR_JELLY= 252,
    BX_BID=254,
    JBX,
    JBX_3L,
    JBX_2L,
    R3L,


    Spread = 1000,
    INVALID
};

enum algo_data_insert_status{
    EXIST=0,
    FAILED=1,
    INSERTED=2,
    WARING=3,
    UPDATED=4,
    _DB_ERROR=5,
    LIMIT_REACHED=6,
};

enum T_DataType{
    T_NUMBER = 0,
    T_TEXT = 1,
    T_BOOL = 2
};

enum T_Table{
    PORTFOLIO = 0,
    TRADE = 1,
    NET_POS = 2,
    Liners,
    SUMMARY,
    MISSED_TRADE,
};

enum T_LoginErroCode{
    USERNAME_WRONG = 0,
    PASSWORD_WRONG = 1,
    EMPTY_CREDENTIALS = 2,
    DB_ERROR = 3,
    OK=4,
    NOK=5,


};

struct algo_data_to_insert{
    QString algo_type;
    QString user_id;
    QString Algo_Status;
    QString indicator;
    QString Leg1_token_number;
    QString Leg2_token_number;
    QString Leg3_token_number;
    QString Leg4_token_number;
    QString Leg5_token_number;
    QString Leg1_Strike;
    QString Leg2_Strike;
    QString Leg3_Strike;
    QString option_type;
    QString  Algo_Name;
    QString exchange;
    QString AdditionalData1;
    QString AdditionalData2;
    QString AdditionalData3;
    QString AdditionalData4;
    QString Leg1Ratio;
    QString Leg2Ratio;
    QString Leg3Ratio;
    QString Leg4Ratio;
    QString Leg5Ratio;
    QString Leg6Ratio;

    bool uploaded;
    int table_row_unique_id;
    int data_id;

};


struct PortFolioData_Less{
    QString PortfolioType;
    QString Expiry;
};

struct PortfolioAvgPrice{
    int PortfolioNumber;
    double AvgPrice;
    int TokenNo;
    QString BuySellIndicator;
    int LocalOrderNumber;
};

struct contract_table{
    QString InstrumentType;
    QString InstrumentName ;
    QString OptionType;
    int StrikePrice;
    int LotSize;
    long long Expiry;
    int TokenNumber;
    QString StockName;
    int MinimumSpread;
    double VolumeFreezeQty;
    int OperatingRangeslowPriceRange;
    int OperatingRangeshighPriceRange;
};
struct userInfo{
    int UserId;
    QString UserName;
    bool loggedIn;
    bool dbError;
    QString loginResponse;
    int errorCode;
    int MaxPortfolioCount;
    int IDXOpenLimit;
    int STKOpenLimit;

    QMap<PortfolioType,QStringList> algoFilterMap;

};

struct cellData{
    bool checkable=false;
    QString db_col_name;
    QString displayVal;
    QVariant originalVal;
    int tablePosition;
    T_DataType dataType;
};



struct mbpInfo
{
    QString quantity;
    QString price;
    QString numberOfOrders;
    QString bbBuySellFlag;
};
struct MBP_Data_Struct{
    QString token;
    QString volumeTradedToday;
    QString netChangeIndicator;
    QString netPriceChangeFromClosingPrice;
    QString lastTradedPrice;
    QString lastTradeQuantity;
    QString lastTradeTime;
    QString averageTradePrice;
    QString totalBuyQuantity;
    QString totalSellQuantity;
    QString closingPrice;
    QString openPrice;
    QString highPrice;
    QString lowPrice;
    QList<mbpInfo> recordBuffer;

};

struct Indices_Data_Struct{
    QString indexName;
    QString indexValue;
    QString change;
    QString percentagechange;
    QString netChangeIndicator;
    QString openingIdx;
    QString closingIdx;
    QString highIndexValue;
    QString lowIndexValue;
    QString marketCapitialisation;
    int display_widget_idx;
};

enum BID_TYPE{
    INDEX_ALGO_BFLY_BID=0,
    INDEX_ALGO_F2F,
    INDEX_ALGO_BFLY,
    INDEX_ALGO_CON_REV,
    INDEX_ALGO_BOX,
    INDEX_ALGO_Open_BFLY,
    INDEX_ALGO_Open_BOX
};

enum NOTIFICATION_TYPE
{
    CMD_ID_DEFAULT = 0,
    CMD_ID_PORTTFOLIO_NEW_1 = 1,
    CMD_ID_TRADE_UPDATED_100 = 100,
    CMD_ID_TRADE_UPDATED_200 = 200,
    CMD_ID_PORTTFOLIO_UPDATED_201 = 201,
    CMD_ID_TRADE_UPDATED_CMD_300 = 300,
    CMD_ID_PORTTFOLIO_UPDATED_301 = 301,
};


enum ConvertAlog_Model_Roles {
    CustomSortingDataRole = Qt::UserRole + 2, // This should be 2, 1 is already used.
};

#endif // DEFINES_H
