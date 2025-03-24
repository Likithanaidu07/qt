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
#define FO_PRICE_DIFF_INCREMENTER 0.01



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
    NA=0,
    WIDGET_NIFTY_50=1,
    WIDGET_NIFTY_BANK=2,
    WIDGET_NIFTY_FINANCIAL_SERVICES=3,
    WIDGET_INDIA_VIX=4,
    WIDGET_NIFTY_MIDCAP_50=5,
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
    _Depth,
    _ExpiryDateTime,
    _Cost, //currently up to this the data is displayed on portfolio table in the same order as the enum
    _QuantityRatio,
 //   _SkipMarketStrike,
   // _BidLeg,
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
    AlgoNo_OB=0,
    OrderId_OB,
    AlgoName_OB,
    UserPrice_OB,
    ExchPrice_OB,
    Jackpot_OB,
    TradedLot_OB,
    RemainingLot_OB,
    TradeTime_OB,
    Leg1State_OB,
    BidLegState_OB,
    Leg3State_OB,
    Leg4State_OB,
    Leg1StateVal_OB,
    BidLegStateVal_OB,
    Leg3StateVal_OB,
    Leg4StateVal_OB,
    Expiry_OB,
    BuyorSell_OB,
    LotSize_OB,
    TraderData_OB,
    TradeTable_Hihglight_OB
};


enum F1F2_Order_Data_Idx{
    //AlgoNo_F1F2=0,
    OrderId_F1F2=0,
    AlgoName_F1F2,
    UserPrice_F1F2,
    ExchPrice_F1F2,
    TradedLot_F1F2,
    //RemainingLot_F1F2,
    TradeTime_F1F2,
    Leg1State_F1F2,
    BidLegState_F1F2,
    Leg3State_F1F2,
    Leg4State_F1F2,
    Leg1StateVal_F1F2,
    BidLegStateVal_F1F2,
    Leg3StateVal_F1F2,
    Leg4StateVal_F1F2,
    Expiry_F1F2,
    BuyorSell_F1F2,
    LotSize_F1F2,
    TraderData_F1F2
};


enum Liners_Idx{
    AlgoId = 0,
    AlgoName,
    BuyAvgPrice,
    BuyQtyinLots,
    SellAvgPrice,
    SellQtyinLots,
    NetQty,
   // Profit,

};

enum Missed_Trades_Idx{
    Missed_Trade_Id=0,
    OrderId,
    BuySell,
    Type,
    Lot,
    AlgoNo,
    Stock_Name,
    Message,
    Price,
    DateTime,
    Retry_Order_Bt, // if it is 1 alreadye retried.
    Traded_Price,
    Traded_Lot,
    Token_No,

};

enum Open_Position_Idx{
   StockName=0,
    Buy_Sell,
    Open_Lot,
    open_Avg,
    Time,
    Square_Off,
   TokenNo,
};

enum NET_POS
{
    StockName_NP=0,
    BuyTotalLot_NP,
    SellTotalLot_NP,
    BuyValue_NP,
    SellValue_NP,
    BuyAvgPrice_NP,
    SellAvgPrice_NP,
    NetQty_NP,
    Profit_NP,
    MTM_NP,
    Margin_Used_NP,
    lotSize_NP,
    TokenNo_NP,
};

enum PortfolioType
{

    BY,

    BOX,
    BS,

    BX_3L,
    BX_2L,
    BYC_4L,
    BYC_5L,
    BYC_6L,

    BX_MKT = 13,

    R2L = 200,
    OPEN_BY = 205, // open btfy
    OPEN_BOX = 206, // open btfy
    F1_F2 = 208,
    BFLY_BID = 250,
    CR = 251,
    CR_JELLY= 252,
    F2F = 253,
    BX_BID=254,
    BS1221=255,
    BS1331=256,
    OPEN2L=257,
    JBX,
    JBX_3L,
    JBX_2L,
    R3L,


    Spread = 1000,
    INVALID
};

enum InstrumentType{
    FUT_INSTRUMENT = 50000, // IMPORTANT! make sure this value does not given PortfolioType enum, if given there changes here with different value
    OPT_INSTRUMENT = 50001, // IMPORTANT! make sure this value does not given PortfolioType enum, if given there changes here with different value

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
    INACTIVE=5,


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
    QString Leg4_Strike;
    QString option_type;
    QString Algo_Name;
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
    QString Alias="";

    bool uploaded;
    int table_row_unique_id;
    int data_id;

};


struct PortFolioData_Less{
    QString PortfolioType;
    QString Expiry;
    int lotSize;
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
    int MaxActiveCount;
    QStringList ExFilterPF;
    QMap<int,QStringList> algoFilterMap;
    long exp_mar;

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
// Serialize mbpInfo
QDataStream &operator<<(QDataStream &out, const mbpInfo &info);
QDataStream &operator>>(QDataStream &in, mbpInfo &info);


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

// Serialization operators
QDataStream &operator<<(QDataStream &out, const MBP_Data_Struct &data);
QDataStream &operator>>(QDataStream &in, MBP_Data_Struct &data);

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

QDataStream &operator>>(QDataStream &in, Indices_Data_Struct &data);
QDataStream &operator<<(QDataStream &out, const Indices_Data_Struct &data);


struct HotKeyDetails{
    QString action;
    QString description;
    QString shortcut;
    bool enabled;
    // Define the '==' operator for comparison
    bool operator==(const HotKeyDetails &other) const {
        return (action == other.action &&
                description == other.description &&
                shortcut == other.shortcut &&
                enabled == other.enabled);
    }

    // Define the '!=' operator for comparison
    bool operator!=(const HotKeyDetails &other) const {
        return !(*this == other);
    }
};

struct HotKeysData_Grouped{
    QString category;
    QString title;
    QList<HotKeyDetails>  HotKeyList;

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

enum BACKEND_CMD_TYPE
{
    CMD_ID_KEEP_ALIVE = 0,
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

struct Cell_Cache{
    QString prevVal;
    QString newVal;
};


struct portfolioImportExportData{
    QString PortfolioType;
    QString TraderID;
    QString ClientID;
    QString Status;
    QString IsBroker;
    QString Leg1TokenNo;
    QString Leg2TokenNo;
    QString Leg3TokenNo;
    QString Leg4TokenNo;
    QString Leg5TokenNo;
    QString Leg6TokenNo;
    QString SellPriceDifference;
    QString SellTotalQuantity;
    QString SellTradedQuantity;
    QString BuyPriceDifference;
    QString BuyTotalQuantity;
    QString BuyTradedQuantity;
    QString AdditionalData1;
    QString AdditionalData2;
    QString AdditionalData3;
    QString AdditionalData4;
    QString Alias;
};

// Serialize portfolioImportExportData to QDataStream
QDataStream &operator<<(QDataStream &out, const portfolioImportExportData &data);

// Deserialize portfolioImportExportData from QDataStream
QDataStream &operator>>(QDataStream &in, portfolioImportExportData &data);
#endif // DEFINES_H
