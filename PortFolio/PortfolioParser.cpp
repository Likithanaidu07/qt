#include "PortfolioParser.h"
#include <cmath>
#include "Conversions.h"
#include "ContractDetail.h"

bool PortfolioParser::ToObject(QSqlQuery& query, PortfolioObject& obj, QHash<QString, MBP_Data_Struct> MBP_Data_Hash,QHash<QString, PortfolioAvgPrice> averagePriceList,double devicer_,int decimal_precision_)
{
    // static Conversions *convert = new Conversions();

    try
    {
        obj.edting.storeRelaxed(0);
        obj.PortfolioNumber = query.value("PortfolioNumber").toInt();
        obj.decimal_precision = decimal_precision_;
        obj.devicer = devicer_;
        obj.TradedHighlight = false;


        PortfolioType type = static_cast<PortfolioType>(query.value("PortfolioType").toInt());

        obj.PortfolioType = QString::number(type);
        obj.PortfolioTypeEnum = type;
        obj.Cost = "-";

        obj.IsBroker = query.value("IsBroker").toBool();
        obj.TraderID = query.value("TraderID").toInt();
        obj.ClientID = query.value("ClientID").toInt();

        obj.Leg1TokenNo = query.value("Leg1TokenNo").toInt();
        if(obj.Leg1TokenNo>0){
            if(!ContractDetail::getInstance().checkTokenExist(obj.Leg1TokenNo)){
               qDebug()<< "Leg1TokenNo: "<<obj.Leg1TokenNo<<" not exist in Contract table for PortfolioNumber: "<<obj.PortfolioNumber<<", need to delete it.";
               return false;
            }
        }
        obj.Leg2TokenNo = query.value("Leg2TokenNo").toInt();
        if(obj.Leg2TokenNo>0){
            if(!ContractDetail::getInstance().checkTokenExist(obj.Leg2TokenNo)){
               qDebug()<< "Leg2TokenNo: "<<obj.Leg2TokenNo<<" not exist in Contract table for PortfolioNumber: "<<obj.PortfolioNumber<<", need to delete it.";
               return false;
            }
        }
        obj.Leg3TokenNo = query.value("Leg3TokenNo").toInt();
        if(obj.Leg3TokenNo>0){
            if(!ContractDetail::getInstance().checkTokenExist(obj.Leg3TokenNo)){
               qDebug()<< "Leg3TokenNo: "<<obj.Leg3TokenNo<<" not exist in Contract table for PortfolioNumber: "<<obj.PortfolioNumber<<", need to delete it.";
               return false;
            }
        }
        obj.Leg4TokenNo = query.value("Leg4TokenNo").toInt();
        if(obj.Leg4TokenNo>0){
            if(!ContractDetail::getInstance().checkTokenExist(obj.Leg4TokenNo)){
               qDebug()<< "Leg4TokenNo: "<<obj.Leg4TokenNo<<" not exist in Contract table for PortfolioNumber: "<<obj.PortfolioNumber<<",  need to delete it.";
               return false;
            }
        }
        obj.Leg5TokenNo = query.value("Leg5TokenNo").toInt();
        if(obj.Leg5TokenNo>0){
            if(!ContractDetail::getInstance().checkTokenExist(obj.Leg5TokenNo)){
               qDebug()<< "Leg5TokenNo: "<<obj.Leg5TokenNo<<" not exist in Contract table for PortfolioNumber: "<<obj.PortfolioNumber<<",  need to delete it.";
               return false;
            }
        }
        obj.Leg6TokenNo = query.value("Leg6TokenNo").toInt();
        if(obj.Leg6TokenNo>0){
            if(!ContractDetail::getInstance().checkTokenExist(obj.Leg6TokenNo)){
               qDebug()<< "Leg6TokenNo: "<<obj.Leg6TokenNo<<" not exist in Contract table for PortfolioNumber: "<<obj.PortfolioNumber<<",  need to delete it.";
               return false;
            }
        }

        /*SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.Leg1TokenNo));
            SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.Leg2TokenNo));
            SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.Leg3TokenNo));
            SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.Leg4TokenNo));
            SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.Leg5TokenNo));
            SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.Leg6TokenNo));*/
        obj.VolumeFreezeQty = ContractDetail::getInstance().GetVolumeFreezeQty(obj.Leg1TokenNo,type)-1;
        obj.Leg1LotSizeOrg = ContractDetail::getInstance().GetLotSize(obj.Leg1TokenNo,type);
        obj.Leg2LotSize = ContractDetail::getInstance().GetLotSize(obj.Leg2TokenNo,type);
        obj.Leg3LotSize = ContractDetail::getInstance().GetLotSize(obj.Leg3TokenNo,type);
        obj.Leg4LotSize = ContractDetail::getInstance().GetLotSize(obj.Leg4TokenNo,type);
        obj.Leg5LotSize = ContractDetail::getInstance().GetLotSize(obj.Leg5TokenNo,type);
        obj.Leg6LotSize = ContractDetail::getInstance().GetLotSize(obj.Leg6TokenNo,type);

        obj.SellMarketRate = 0.0;
        obj.BuyMarketRate = 0.0;

        int lotSize = obj.GetLotSize();
        if(lotSize==0){
            obj.AlgoName = "Cannot get lotSize from Contract, token not found in Contract ";
            return false;
        }

        int totalQty = 0;

       obj.OrderQuantity = 0;
        totalQty = query.value("SellTotalQuantity").toInt() / lotSize;

       obj.SellPriceDifference = query.value("SellPriceDifference").toDouble() / static_cast<double>(devicer_);

        obj.SellTradedQuantity = query.value("SellTradedQuantity").toInt() / lotSize;
        obj.SellTotalQuantity = totalQty;
        obj.SellRemainingQuantity = totalQty - obj.SellTradedQuantity;
        obj.Alias = query.value("Alias").toString();
        if(obj.Alias=="")
            obj.Alias="-";
        obj.Depth = query.value("DepthConfigured").toInt();
        obj.MaxLoss = query.value("MaxLoss").toDouble()/devicer_;
        totalQty = query.value("BuyTotalQuantity").toInt() / lotSize;
        obj.BuyPriceDifference = query.value("BuyPriceDifference").toDouble()/devicer_;// convert->ToRupees(query.value("BuyPriceDifference").toDouble());
        obj.BuyTradedQuantity = query.value("BuyTradedQuantity").toInt() / lotSize;
        obj.BuyTotalQuantity = totalQty;
        obj.BuyRemainingQuantity = totalQty - obj.BuyTradedQuantity;


        // If there is a specific logic to update OrderQuantity, handle it here
//        int newOrderQty = query.value("OrderQuantity").toInt() / lotSize;
//        if (newOrderQty > 0) {
//            obj.OrderQuantity = newOrderQty;
//        }
//        if (obj.BuyTotalQuantity > 0 || obj.SellTotalQuantity > 0) {
//            obj.OrderQuantity = 1;
//        }
        obj.OrderQuantity = query.value("OrderQuantity").toInt()/ lotSize;

        obj.InstrumentName = ContractDetail::getInstance().GetInstrumentName(obj.GetFirstToken(),type);

        obj.Leg1 = "-";
        obj.Leg2 = "-";
        obj.Leg3 = "-";
        obj.Leg4 = "-";
        obj.Leg5 = "-";
        obj.Leg6 = "-";

        obj.AdditionalData1 = "-";
        obj.AdditionalData2 = "-";
        obj.AdditionalData3 = "-";
        obj.AdditionalData4 = "-";

        obj.Leg1Ratio = query.value("Leg1Ratio").toInt();
        obj.Leg2Ratio = query.value("Leg2Ratio").toInt();
        obj.Leg3Ratio = query.value("Leg3Ratio").toInt();
        obj.Leg4Ratio = query.value("Leg4Ratio").toInt();
        obj.Leg5Ratio = query.value("Leg5Ratio").toInt();
        obj.Leg6Ratio = query.value("Leg6Ratio").toInt();

        obj.Expiry = ContractDetail::getInstance().GetExpiry(obj.GetFirstToken(), "dd-MMM-yy",type);
        obj.ExpiryMonth = ContractDetail::getInstance().GetExpiry(obj.GetFirstToken(), "MMM",type);
        obj.ExpiryBinary = ContractDetail::getInstance().GetExpiry(obj.GetFirstToken(),type);

        obj.Status = query.value("Status").toString() == "Active";
        QString statusVal = query.value("Status").toString();
        //Active  DisabledByUser Filled
        if(statusVal=="Active")
            obj.StatusVal = QString::number(portfolio_status::Active);
        else if(statusVal=="DisabledByUser")
            obj.StatusVal = QString::number(portfolio_status::DisabledByUser);
        else
            obj.StatusVal = QString::number(portfolio_status::Filled);

        if(obj.PortfolioType==QString::number(PortfolioType::BFLY_BID)){
            obj.QuantityRatio="1:2:1";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::CR)){
            obj.QuantityRatio="1:1:1";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::CR_JELLY)){
            obj.QuantityRatio="1:1:1";

        }
        else if(obj.PortfolioType==QString::number(PortfolioType::F2F)){
            obj.QuantityRatio="1:1";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::BX_BID)){
            obj.QuantityRatio="1:1:1:1";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::BS1221)){
            obj.QuantityRatio="1:2:2:1";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::BS1331)){
            obj.QuantityRatio="1:3:3:1";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::OPEN2L)){
            obj.QuantityRatio=QString::number(obj.Leg1Ratio)+":"+QString::number(obj.Leg2Ratio);
        }
        else{
            obj.QuantityRatio="N/A";
        }


        if(obj.PortfolioType==QString::number(PortfolioType::CR)){
            obj.BidLeg = ContractDetail::getInstance().GetStockName(obj.Leg1TokenNo,obj.PortfolioType.toInt());
        }

        else if(obj.PortfolioType==QString::number(PortfolioType::CR_JELLY)){
            obj.BidLeg = ContractDetail::getInstance().GetStockName(obj.Leg1TokenNo,obj.PortfolioType.toInt());
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::BX_BID)){
            obj.BidLeg = "-";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::BS1221)){
            obj.BidLeg = "-";
        }
        else if(obj.PortfolioType==QString::number(PortfolioType::BS1331)){
            obj.BidLeg = "-";
        }
        else{
            obj.BidLeg = ContractDetail::getInstance().GetStockName(obj.Leg2TokenNo,obj.PortfolioType.toInt());

        }

        //obj.SkipMarketStrike= "-";
        obj.FuturePrice="-";


        QHash<QString, contract_table> contractDetails = ContractDetail::getInstance().GetContracts("FUT");

        QString intrumentName = obj.InstrumentName;
        QHash<QString, contract_table> filtered;


        for (auto it = contractDetails.begin(); it != contractDetails.end(); ++it) {
            const QString& key = it.key();
            const contract_table& value = it.value();
            if (value.InstrumentName == intrumentName) {
                filtered.insert(key, value);
            }
        }


        QList<QString> sorted_keys = filtered.keys();
         std::sort(sorted_keys.begin(), sorted_keys.end(), [&](const QString& key1, const QString& key2) {
             const contract_table& value1 = filtered.value(key1);
             const contract_table& value2 = filtered.value(key2);

            return value1.Expiry < value2.Expiry;
         });



        if (!filtered.empty())
        {
            obj.FutToken = filtered[sorted_keys.at(0)].TokenNumber;
            obj.FuturePrice = "-";
            // SlowDataReader::GetInstance().RegisterContract(static_cast<uint>(obj.FutToken));
        }
        switch (type)
        {
        case PortfolioType::F2F:
            obj.Leg1 = ContractDetail::getInstance().GetExpiry(obj.Leg1TokenNo, "ddMMMyy",type);
            obj.Leg2 = ContractDetail::getInstance().GetExpiry(obj.Leg2TokenNo, "ddMMMyy",type);
            break;



        case PortfolioType::BY:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);

            obj.AdditionalData1 = query.value("AdditionalData1").toString();//.Convert.ToString(reader["AdditionalData1"]);
            break;

        case PortfolioType::OPEN_BY:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);

            obj.AdditionalData1 = query.value("AdditionalData1").toString();
            obj.AdditionalData2 = query.value("AdditionalData2").toString();
            obj.AdditionalData3 = query.value("AdditionalData3").toString();
            break;


        case PortfolioType::R2L:
        {
            contract_table c2 = ContractDetail::getInstance().GetDetail(obj.Leg1TokenNo);

            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);

            QString ratio = query.value("AdditionalData3").toString();//Convert.ToString(reader["AdditionalData3"]);
            int xRatio = query.value("AdditionalData1").toInt();//Convert.ToInt32(reader["AdditionalData1"]);
            int yRatio = query.value("AdditionalData2").toInt();//Convert.ToInt32(reader["AdditionalData2"]);

            obj.Leg1Multiplier = xRatio;
            obj.Leg2Multiplier = yRatio;

            QString result = QString("%1, %2:%3").arg(ratio).arg(xRatio).arg(yRatio);
            result.append(", ").append(c2.OptionType);
            obj.Leg3 = result;

            obj.AdditionalData1 = QString::number(xRatio);//.ToString();
            obj.AdditionalData2 = QString::number(yRatio);//yRatio.ToString();
            obj.AdditionalData3 = ratio;
        }
        break;

//        case PortfolioType::BS:
//        case PortfolioType::BX1221:
//        {

//            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
//            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
//            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
//            obj.AdditionalData1 = query.value("AdditionalData1").toString();//Convert.ToString(reader["AdditionalData1"]);
//            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg5TokenNo,type)+query.value("AdditionalData1").toString();

//            break;
//        }

        case PortfolioType::CR:
            obj.Expiry = ContractDetail::getInstance().GetExpiry(obj.Leg2TokenNo, "dd-MMM-yy",type);
            obj.ExpiryBinary = ContractDetail::getInstance().GetExpiry(obj.Leg2TokenNo,type);

            obj.Leg1 = ContractDetail::getInstance().GetExpiry(obj.Leg1TokenNo, "ddMMMyy",type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
            break;

        case PortfolioType::CR_JELLY:
            obj.Expiry = ContractDetail::getInstance().GetExpiry(obj.Leg2TokenNo, "dd-MMM-yy",type);
            obj.ExpiryDateTime = ContractDetail::getInstance().GetExpiryDate(obj.Leg1TokenNo,type);
            obj.ExpiryBinary = ContractDetail::getInstance().GetExpiry(obj.Leg1TokenNo,type);

            obj.Leg1 = ContractDetail::getInstance().GetExpiry(obj.Leg1TokenNo, "ddMMMyy",type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
            break;



        case PortfolioType::BOX:
        case PortfolioType::OPEN_BOX:
        case PortfolioType::JBX:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
            //obj.Leg3 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg5TokenNo))+(query.value("AdditionalData3").toString());
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg5TokenNo,type)+query.value("AdditionalData3").toString();
            break;

        case PortfolioType::BX_3L:
        case PortfolioType::JBX_3L:
            if (obj.Leg1TokenNo == 0)
            {
                //obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo))+(query.value("AdditionalData2").toString());
                obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());
                obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());
                obj.Leg3 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());

            }
            else if (obj.Leg2TokenNo == 0)
            {
                obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());
                obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());
                obj.Leg3 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());
            }
            else if (obj.Leg3TokenNo == 0)
            {
                obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());
                obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());
                obj.Leg3 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());

            }
            else if (obj.Leg4TokenNo == 0)
            {
                obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());
                obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());
                obj.Leg3 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());
            }
            break;

        case PortfolioType::BYC_4L:
        {
            obj.AdditionalData1 = "-";// Convert.ToString(reader["AdditionalData1"]);
            obj.AdditionalData2 = query.value("AdditionalData2").toString();//Convert.ToString(reader["AdditionalData2"]);
            obj.AdditionalData3 = query.value("AdditionalData3").toString();//Convert.ToString(reader["AdditionalData3"]);
            obj.AdditionalData4 = query.value("AdditionalData4").toString();//Convert.ToString(reader["AdditionalData4"]);

            contract_table leg1 = ContractDetail::getInstance().GetDetail(obj.Leg2TokenNo);
            contract_table leg2 = ContractDetail::getInstance().GetDetail(obj.Leg3TokenNo);
            obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(leg1.TokenNumber,type))+(leg1.OptionType);
            obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(leg2.TokenNumber,type))+(leg2.OptionType);//String.Format("{0}{1}", leg2.GetStrikePrice(), leg2.OptionType);

            contract_table detail = ContractDetail::getInstance().GetDetail(obj.AdditionalData2.toInt());
            obj.Leg3 = (ContractDetail::getInstance().GetStrikePrice(detail.TokenNumber,type))+(detail.OptionType);
            break;
        }

        case PortfolioType::BYC_5L:
        {
            obj.AdditionalData1 = "-";// Convert.ToString(reader["AdditionalData1"]);
            obj.AdditionalData2 = query.value("AdditionalData2").toString();//Convert.ToString(reader["AdditionalData2"]);
            obj.AdditionalData3 = query.value("AdditionalData3").toString();//Convert.ToString(reader["AdditionalData3"]);
            obj.AdditionalData4 = query.value("AdditionalData4").toString();//Convert.ToString(reader["AdditionalData4"]);

            contract_table leg1 = ContractDetail::getInstance().GetDetail(obj.Leg2TokenNo);
            contract_table leg2 = ContractDetail::getInstance().GetDetail(obj.Leg4TokenNo);
            obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(leg1.TokenNumber,type))+(leg1.OptionType);
            obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(leg2.TokenNumber,type))+(leg2.OptionType);

            contract_table detail1 = ContractDetail::getInstance().GetDetail(obj.AdditionalData2.toInt());
            contract_table detail2 = ContractDetail::getInstance().GetDetail(obj.AdditionalData3.toInt());
            obj.Leg3 = QString("{0}{1}, {2}{3}")+(ContractDetail::getInstance().GetStrikePrice(detail1.TokenNumber,type))+(detail1.OptionType)+(ContractDetail::getInstance().GetStrikePrice(detail2.TokenNumber,type))+(detail2.OptionType);
            break;
        }
        case PortfolioType::BYC_6L:
        {
            obj.AdditionalData1 = "-";//Convert.ToString(reader["AdditionalData1"]);
            obj.AdditionalData2 = query.value("AdditionalData2").toString();//Convert.ToString(reader["AdditionalData2"]);
            obj.AdditionalData3 = query.value("AdditionalData3").toString();//Convert.ToString(reader["AdditionalData3"]);
            obj.AdditionalData4 = query.value("AdditionalData4").toString();//Convert.ToString(reader["AdditionalData4"]);

            contract_table leg1 = ContractDetail::getInstance().GetDetail(obj.Leg2TokenNo);
            contract_table leg2 = ContractDetail::getInstance().GetDetail(obj.Leg5TokenNo);
            obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(leg1.TokenNumber,type), leg1.OptionType);
            obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(leg1.TokenNumber,type), leg2.OptionType);

            contract_table detail1 = ContractDetail::getInstance().GetDetail(obj.AdditionalData2.toInt());
            contract_table detail2 = ContractDetail::getInstance().GetDetail(obj.AdditionalData3.toInt());
            contract_table detail3 = ContractDetail::getInstance().GetDetail(obj.AdditionalData4.toInt());
            obj.Leg3 = QString("{0}{1}, {2}{3}, {4}{5}")+(ContractDetail::getInstance().GetStrikePrice(detail1.TokenNumber,type)+(detail1.OptionType)+(ContractDetail::getInstance().GetStrikePrice(detail3.TokenNumber,type))+(detail3.OptionType));
            break;
        }

        case PortfolioType::BX_2L:
        case PortfolioType::JBX_2L:
            if (obj.Leg1TokenNo == 0)
            {
                if (obj.Leg2TokenNo == 0)
                {
                    //leg 1 & 2 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());
                }
                else if (obj.Leg3TokenNo == 0)
                {
                    //leg 1 & 3 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());
                }
                else if (obj.Leg4TokenNo == 0)
                {
                    //leg 1 & 4 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                }
            }
            else if (obj.Leg2TokenNo == 0)
            {
                if (obj.Leg1TokenNo == 0)
                {
                    //leg 2 & 1 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                }
                else if (obj.Leg3TokenNo == 0)
                {
                    //leg 2 & 3 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                }
                else if (obj.Leg4TokenNo == 0)
                {
                    //leg 2 & 4 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                }
            }
            else if (obj.Leg3TokenNo == 0)
            {
                if (obj.Leg2TokenNo == 0)
                {
                    //leg 3 & 2 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                }
                else if (obj.Leg1TokenNo == 0)
                {
                    //leg 3 & 1 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                }
                else if (obj.Leg4TokenNo == 0)
                {
                    //leg 3 & 4 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                }
            }
            else if (obj.Leg4TokenNo == 0)
            {
                if (obj.Leg2TokenNo == 0)
                {
                    //leg 4 & 2 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                }
                else if (obj.Leg3TokenNo == 0)
                {
                    //leg 4 & 3 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                }
                else if (obj.Leg1TokenNo == 0)
                {
                    //leg 4 & 1 skipped
                    obj.Leg1 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type))+(query.value("AdditionalData2").toString());//, Convert.ToString(reader["AdditionalData2"]));
                    obj.Leg2 = (ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type))+(query.value("AdditionalData1").toString());//, Convert.ToString(reader["AdditionalData1"]));
                }
            }
            break;
        case PortfolioType::BFLY_BID:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);

            obj.AdditionalData1 = query.value("AdditionalData1").toString();//.Convert.ToString(reader["AdditionalData1"]);
            break;

        case PortfolioType::BX_BID:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
            obj.Leg4 = ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type);

            obj.AdditionalData1 = query.value("AdditionalData1").toString();//.Convert.ToString(reader["AdditionalData1"]);
            break;

        case PortfolioType::BS1221:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
            obj.Leg4 = ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type);

            obj.AdditionalData1 = query.value("AdditionalData1").toString();//.Convert.ToString(reader["AdditionalData1"]);
            break;

        case PortfolioType::BS1331:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            obj.Leg3 = ContractDetail::getInstance().GetStrikePrice(obj.Leg3TokenNo,type);
            obj.Leg4 = ContractDetail::getInstance().GetStrikePrice(obj.Leg4TokenNo,type);

            obj.AdditionalData1 = query.value("AdditionalData1").toString();//.Convert.ToString(reader["AdditionalData1"]);
            break;

        case PortfolioType::OPEN2L:
            obj.Leg1 = ContractDetail::getInstance().GetStrikePrice(obj.Leg1TokenNo,type);
            obj.Leg2 = ContractDetail::getInstance().GetStrikePrice(obj.Leg2TokenNo,type);
            break;


        default:
            break;
        }

        totalQty = 0;
        lotSize = obj.GetLotSize() * std::abs(obj.Leg1Multiplier);

        totalQty = query.value("SellTotalQuantity").toInt() / lotSize;
        obj.SellPriceDifference = query.value("SellPriceDifference").toDouble()/devicer_;//convert->ToRupees(query.value("SellPriceDifference").toDouble());
        obj.SellTradedQuantity = query.value("SellTradedQuantity").toInt() / lotSize;
        obj.SellTotalQuantity = totalQty;
        obj.SellRemainingQuantity = totalQty - obj.SellTradedQuantity;

        totalQty = query.value("BuyTotalQuantity").toInt() / lotSize;
        //            if(query.value("BuyPriceDifference")=="")
        //                obj.BuyPriceDifference="-";
        //            else
        obj.BuyPriceDifference =  query.value("BuyPriceDifference").toDouble()/devicer_;//convert->ToRupees(query.value("BuyPriceDifference").toDouble());
        obj.BuyTradedQuantity = query.value("BuyTradedQuantity").toInt() / lotSize;
        obj.BuyTotalQuantity = totalQty;
        obj.BuyRemainingQuantity = totalQty - obj.BuyTradedQuantity;
        CalculatePriceDifference(obj,MBP_Data_Hash,devicer_,decimal_precision_);
        CalculateAveragePrice(obj,averagePriceList,devicer_,decimal_precision_);
        obj.AlgoName = get_Algo_Name(obj.PortfolioTypeEnum,obj.Leg1TokenNo,obj.Leg2TokenNo,obj.Leg3TokenNo,obj.Leg4TokenNo,devicer_,decimal_precision_,obj.AlgoNameForSorting);

        //obj.AlgoName = obj.AlgoNameForSorting;// this should remove
        return true;
    }
    catch (const std::exception& ex)
    {
        // Program::logger.Error("PortfolioParser", ex);
    }

    return false;
}

/*static bool ToSpreadObject(QSqlQuery& query, PortfolioObject& obj)
    {
        try
        {
            obj.PortfolioNumber = query.value("PortfolioNumber").toInt();

            PortfolioType type = static_cast<PortfolioType>(query.value("PortfolioType").toInt());

            obj.PortfolioType = QString::number(type);
            obj.PortfolioTypeEnum = type;

            obj.IsBroker = query.value("IsBroker").toBool();
            obj.TraderID = query.value("TraderID").toInt();

            obj.Leg1TokenNo = query.value("Leg1TokenNo").toInt();
            obj.Leg2TokenNo = query.value("Leg2TokenNo").toInt();
            obj.Leg3TokenNo = query.value("Leg3TokenNo").toInt();
            obj.Leg4TokenNo = query.value("Leg4TokenNo").toInt();
            obj.Leg5TokenNo = query.value("Leg5TokenNo").toInt();
            obj.Leg6TokenNo = query.value("Leg6TokenNo").toInt();

            obj.SellMarketRate = 0.0;

            int totalQty = 0;

            obj.Leg1LotSizeOrg = SpreadContractDetail::GetDetail(obj.Leg1TokenNo).MinLotQty1;
            obj.Leg2LotSize = SpreadContractDetail::GetDetail(obj.Leg1TokenNo).MinLotQty2;

            int lotSize = obj.GetLotSize();

            totalQty = query.value("SellTotalQuantity").toInt() / lotSize;
            obj.SellPriceDifference = query.value("SellPriceDifference").toDouble()/devicer;//convert.ToRupees(query.value("SellPriceDifference").toDouble());
            obj.SellTradedQuantity = query.value("SellTradedQuantity").toInt() / lotSize;
            obj.SellTotalQuantity = totalQty;
            obj.SellRemainingQuantity = totalQty - obj.SellTradedQuantity;

            totalQty = query.value("BuyTotalQuantity").toInt() / lotSize;
            obj.BuyPriceDifference = query.value("BuyPriceDifference").toDouble()/devicer;//convert.ToRupees(query.value("BuyPriceDifference").toDouble());
            obj.BuyTradedQuantity = query.value("BuyTradedQuantity").toInt() / lotSize;
            obj.BuyTotalQuantity = totalQty;
            obj.BuyRemainingQuantity = totalQty - obj.BuyTradedQuantity;

            obj.InstrumentName = SpreadContractDetail::GetInstrumentName(obj.Leg1TokenNo);

            obj.Leg1 = "-";
            obj.Leg2 = "-";
            obj.Leg3 = "-";
            obj.Leg4 = "-";
            obj.Leg5 = "-";
            obj.Leg6 = "-";

            obj.AdditionalData1 = "-";
            obj.AdditionalData2 = "-";
            obj.AdditionalData3 = "-";
            obj.AdditionalData4 = "-";

            obj.Expiry = SpreadContractDetail::GetExpiry1(obj.Leg1TokenNo, "dd-MMM-yy");
            obj.ExpiryDateTime = SpreadContractDetail::GetExpiryDate1(obj.Leg1TokenNo);
            obj.ExpiryBinary = SpreadContractDetail::GetDetail(obj.Leg1TokenNo).Expiry1;

            obj.Status = query.value("Status").toString() == QString::fromStdString(PortfolioStatus::Active.ToString());


            obj.Leg1 = SpreadContractDetail::GetExpiry1(obj.Leg1TokenNo, "ddMMMyy");
            obj.Leg2 = SpreadContractDetail::GetExpiry2(obj.Leg1TokenNo, "ddMMMyy");

            return true;
        }
        catch (const std::exception& ex)
        {
            Program::logger.Error("PortfolioParser", ex);
        }

        return false;
    }*/
QString PortfolioParser::get_Algo_Name(PortfolioType algo_type,int leg1_token_number,int leg2_token_number,int leg3_token_number,int leg4_token_number,double devicer,int decimal_precision,QString &Algo_Name_For_Sorting){
    QString Algo_Name="-";
    if(algo_type==PortfolioType::F2F){
        Algo_Name = "F2F-";//-Nifty";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"MMM",algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"MMM",algo_type);
        Algo_Name_For_Sorting = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMMyyyy",algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"MMM",algo_type)+"-0-ZE";


    }
    else if(algo_type==PortfolioType::BY){
        Algo_Name = "Bfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type) +"-"+QString::number(diff);
        Algo_Name_For_Sorting = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type) +"-"+QString::number(diff);

    }
    else if(algo_type==PortfolioType::OPEN_BY){
        Algo_Name = "OpenBfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type) +"-"+QString::number(diff);
        Algo_Name_For_Sorting = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type) +"-"+QString::number(diff);

    }
    else if(algo_type==PortfolioType::CR){
        Algo_Name = "CR-";//Nifty-17000";
        //            QString StockName = ContractDetail::getInstance().GetStockName(leg2_token_number);
        //            StockName.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type);
        Algo_Name_For_Sorting   = "CR-"+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-0-ZE"; // For CR no  strike diff and optiontype in algoname, so make some dummy data so sorting will work

    }
    else if(algo_type==PortfolioType::CR_JELLY){
        Algo_Name = "CRJELLY-";//Nifty-17000";
        //            QString StockName = ContractDetail::getInstance().GetStockName(leg2_token_number);
        //            StockName.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type);
        Algo_Name_For_Sorting   = "CRJELLY-"+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type)+"-0-ZE"; // // For CRJELLY no strike diff and optiontype in algoname, so make some dummy data so sorting will work

    }
    else if(algo_type==PortfolioType::BOX){
        Algo_Name = "BX-";//18100-18200";
        //            QString StockNameLeg1 = ContractDetail::getInstance().GetStockName(leg1_token_number);
        //            StockNameLeg1.chop(2);
        //            QString StockNameLeg3 = ContractDetail::getInstance().GetStockName(leg3_token_number);
        //            StockNameLeg3.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type);
        Algo_Name_For_Sorting = "BX-"+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMMyyyy",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type);

    }
    else if(algo_type==PortfolioType::OPEN_BOX){
        Algo_Name = "OpenBOX-";//18100-18200";
        //            QString StockNameLeg1 = ContractDetail::getInstance().GetStockName(leg1_token_number);
        //            StockNameLeg1.chop(2);
        //            QString StockNameLeg3 = ContractDetail::getInstance().GetStockName(leg3_token_number);
        //            StockNameLeg3.chop(2);
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMM",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type);
        Algo_Name_For_Sorting = "OpenBOX-"+ContractDetail::getInstance().GetInstrumentName(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg1_token_number,"ddMMMyyyy",algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type)+"-"+ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type);

    }
    else if(algo_type==PortfolioType::BFLY_BID){
        Algo_Name = "Bfly-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
        Algo_Name_For_Sorting = "Bfly-"+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::BX_BID){
        Algo_Name = "BX-";//Nifty-18000-CE-200";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff);
        Algo_Name_For_Sorting = "BX-"+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-ZE";
    }
    else if(algo_type==PortfolioType::BS1221 ){
        Algo_Name = "BS12-";//Nifty-18000-CE-200";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+
                ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+
                ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);

        Algo_Name_For_Sorting = "BS12-"+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+
                ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+
                ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::BS1331 ){
        Algo_Name = "BS13-";//Nifty-18000-CE-200";
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+
                ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+
                ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);

        Algo_Name_For_Sorting = "BS13-"+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+
                ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMMyyyy",algo_type)+"-"+
                ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetStrikePrice(leg3_token_number,algo_type) +"-"+
                ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }
    else if(algo_type==PortfolioType::OPEN2L){
        Algo_Name = "Open2L-";
        double diff = (ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type).toDouble()- ContractDetail::getInstance().GetStrikePrice(leg1_token_number,algo_type).toDouble());
        Algo_Name = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
        Algo_Name_For_Sorting = Algo_Name+ContractDetail::getInstance().GetInstrumentName(leg2_token_number,algo_type)+"-"+ContractDetail::getInstance().GetExpiry(leg2_token_number,"ddMMM",algo_type)+"-"+ ContractDetail::getInstance().GetStrikePrice(leg2_token_number,algo_type) +"-"+QString::number(diff)+"-"+ContractDetail::getInstance().GetOptionType(leg1_token_number,algo_type);
    }


    return Algo_Name.toUpper();
}

QString PortfolioParser::fixDecimal(double num,int decimal_precision){
    QString str = QString::number(num, 'f', decimal_precision);
    return str;
}

void PortfolioParser::CalculateAveragePrice(PortfolioObject &portfolio,    QHash<QString, PortfolioAvgPrice> averagePriceList,double devicer, int decimal_precision){
    switch (portfolio.PortfolioTypeEnum)
    {
    case PortfolioType::F2F:
    {
        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice = DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice= DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }
        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber+ 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber+ 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        if (leg2BuyPrice == DBL_MAX || leg1SellPrice == DBL_MAX) {
            portfolio.SellAveragePrice = "-";
        }
        else {
            double diffVal = leg1SellPrice - leg2BuyPrice;
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != diff) {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = diff;
            }
        }
        if (leg1BuyPrice == DBL_MAX || leg2SellPrice == DBL_MAX)  {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            double diffVal = leg2SellPrice - leg1BuyPrice;
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (diff != portfolio.BuyAveragePrice) {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = diff;
            }
        }
        break;
    }
    case PortfolioType::BY:{

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice = DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice =  DBL_MAX;
        double leg3BuyPrice =  DBL_MAX;
        double leg3SellPrice =  DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        if (leg2SellPrice == DBL_MAX || leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX){
            portfolio.SellAveragePrice = "-";
        }
        else{
            double diffVal = -(2 * leg2SellPrice) + leg1BuyPrice + leg3BuyPrice;
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = diff;
            }

        }
        if (leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else{
            double diffVal = -leg1SellPrice - leg3SellPrice + (2 * leg2BuyPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = diff;
            }

        }
        break;
    }
    case PortfolioType::OPEN_BY:{

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice = DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice =  DBL_MAX;
        double leg3BuyPrice =  DBL_MAX;
        double leg3SellPrice =  DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        if (leg2SellPrice == DBL_MAX || leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX){
            portfolio.SellAveragePrice = "-";
        }
        else{
            double diffVal = -(2 * leg2SellPrice) + leg1BuyPrice + leg3BuyPrice;
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = diff;
            }

        }
        if (leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else{
            double diffVal = -leg1SellPrice - leg3SellPrice + (2 * leg2BuyPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = diff;
            }

        }
        break;
    }

    case PortfolioType::CR:{
        QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
        int strikePrice = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
        if(Contract.contains(QString::number(portfolio.Leg2TokenNo)))
            strikePrice = Contract[QString::number(portfolio.Leg2TokenNo)].StrikePrice;

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice = DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice = DBL_MAX;
        double leg3BuyPrice = DBL_MAX;
        double leg3SellPrice = DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }

        if (leg2SellPrice == DBL_MAX|| leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else{
            double diffVal = (leg1BuyPrice + leg3BuyPrice) - ((strikePrice / devicer) + leg2SellPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = diff;
            }
        }
        if (leg2BuyPrice == DBL_MAX || leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            double diffVal = ((strikePrice / devicer) + leg2BuyPrice) - (leg1SellPrice + leg3SellPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != diff)
            {
                portfolio.AverageUpdateTime =QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = diff;
            }
        }


        break;
    }

    case PortfolioType::CR_JELLY:{
        QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
        int strikePrice = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
        if(Contract.contains(QString::number(portfolio.Leg2TokenNo)))
            strikePrice = Contract[QString::number(portfolio.Leg2TokenNo)].StrikePrice;

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice = DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice = DBL_MAX;
        double leg3BuyPrice = DBL_MAX;
        double leg3SellPrice = DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }

        if (leg2SellPrice == DBL_MAX|| leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else{
            double diffVal = (leg1BuyPrice + leg3BuyPrice) - ((strikePrice / devicer) + leg2SellPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = diff;
            }
        }
        if (leg2BuyPrice == DBL_MAX || leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            double diffVal = ((strikePrice / devicer) + leg2BuyPrice) - (leg1SellPrice + leg3SellPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != diff)
            {
                portfolio.AverageUpdateTime =QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = diff;
            }
        }


        break;
    }
    case PortfolioType::BOX:{
        double leg1BuyPrice = 0;//DBL_MAX;
        double leg1SellPrice = 0;//DBL_MAX;
        double leg2BuyPrice = 0;// DBL_MAX;
        double leg2SellPrice =  0;//DBL_MAX;
        double leg3BuyPrice =  0;//DBL_MAX;
        double leg3SellPrice =  0;//DBL_MAX;
        double leg4BuyPrice =  0;//DBL_MAX;
        double leg4SellPrice =  0;//DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        QString key4 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4SellPrice = leg4.AvgPrice;
        }
        key4 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4BuyPrice = leg4.AvgPrice;
        }

        QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
        int l1StrikePrice = 0;
        int l3StrikePrice = 0;
        if(Contract.contains(QString::number(portfolio.Leg1TokenNo)))
            l1StrikePrice = Contract[QString::number(portfolio.Leg1TokenNo)].StrikePrice;
        if(Contract.contains(QString::number(portfolio.Leg3TokenNo)))
            l3StrikePrice = Contract[QString::number(portfolio.Leg3TokenNo)].StrikePrice;

        double strikeDiff = (1.0 * (l3StrikePrice - l1StrikePrice)) / devicer;
        bool ret = false;
        if (leg1BuyPrice == DBL_MAX || leg4BuyPrice == DBL_MAX  || leg2SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            double diff = ((leg1BuyPrice + leg4BuyPrice) - (leg2SellPrice + leg3SellPrice)) - strikeDiff;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != d)
            {
                portfolio.AverageUpdateTime =QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = d;
            }
        }
        if (leg1SellPrice == DBL_MAX || leg4SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else
        {
            double diff = strikeDiff - ((leg1SellPrice + leg4SellPrice) - (leg2BuyPrice + leg3BuyPrice));

            QString d = QString::number(diff, 'f', decimal_precision);

            if (portfolio.SellAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = d;
            }
        }


        break;
    }

    case PortfolioType::BX_BID:{
        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice =DBL_MAX;
        double leg2BuyPrice =  DBL_MAX;
        double leg2SellPrice =  DBL_MAX;
        double leg3BuyPrice =  DBL_MAX;
        double leg3SellPrice =  DBL_MAX;
        double leg4BuyPrice =  DBL_MAX;
        double leg4SellPrice = DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        QString key4 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4SellPrice = leg4.AvgPrice;
        }
        key4 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4BuyPrice = leg4.AvgPrice;
        }

        QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
        int l1StrikePrice = 0;
        int l3StrikePrice = 0;
        if(Contract.contains(QString::number(portfolio.Leg1TokenNo)))
            l1StrikePrice = Contract[QString::number(portfolio.Leg1TokenNo)].StrikePrice;
        if(Contract.contains(QString::number(portfolio.Leg3TokenNo)))
            l3StrikePrice = Contract[QString::number(portfolio.Leg3TokenNo)].StrikePrice;

        double strikeDiff = (1.0 * (l3StrikePrice - l1StrikePrice)) / devicer;
        bool ret = false;
        if (leg3SellPrice == DBL_MAX || leg2SellPrice == DBL_MAX  || leg4BuyPrice == DBL_MAX || leg1BuyPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
           // double diff = strikeDiff - ((leg1SellPrice + leg4SellPrice) - (leg2BuyPrice + leg3BuyPrice));
           // double diff =  -strikeDiff  - leg3SellPrice - leg2SellPrice + leg4BuyPrice + leg1BuyPrice;
            double diff =  leg3SellPrice + leg2SellPrice - leg4BuyPrice - leg1BuyPrice + strikeDiff;
           // diff = diff*-1;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != d)
            {
                portfolio.AverageUpdateTime =QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = d;
            }
        }
        if (leg3BuyPrice == DBL_MAX || leg2BuyPrice == DBL_MAX || leg4SellPrice == DBL_MAX || leg1SellPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else
        {
            //double diff = leg3BuyPrice + leg2BuyPrice - leg4SellPrice - leg1SellPrice + strikeDiff;
            double diff =  -strikeDiff - leg3BuyPrice - leg2BuyPrice + leg1SellPrice +leg4SellPrice;
            //diff = diff*-1;
            QString d = QString::number(diff, 'f', decimal_precision);

            if (portfolio.SellAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = d;
            }
        }


        break;
    }
    case PortfolioType::OPEN_BOX:{
        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice =DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice = DBL_MAX;
        double leg3BuyPrice = DBL_MAX;
        double leg3SellPrice = DBL_MAX;
        double leg4BuyPrice = DBL_MAX;
        double leg4SellPrice =DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        QString key4 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4SellPrice = leg4.AvgPrice;
        }
        key4 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4BuyPrice = leg4.AvgPrice;
        }

        QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
        int l1StrikePrice = 0;
        int l3StrikePrice = 0;
        if(Contract.contains(QString::number(portfolio.Leg1TokenNo)))
            l1StrikePrice = Contract[QString::number(portfolio.Leg1TokenNo)].StrikePrice;
        if(Contract.contains(QString::number(portfolio.Leg3TokenNo)))
            l3StrikePrice = Contract[QString::number(portfolio.Leg3TokenNo)].StrikePrice;

        double strikeDiff = (1.0 * (l3StrikePrice - l1StrikePrice)) / devicer;
        bool ret = false;
        if (leg1BuyPrice == DBL_MAX || leg4BuyPrice == DBL_MAX  || leg2SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            double diff = ((leg1BuyPrice + leg4BuyPrice) - (leg2SellPrice + leg3SellPrice)) - strikeDiff;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != d)
            {
                portfolio.AverageUpdateTime =QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = d;
            }
        }
        if (leg1SellPrice == DBL_MAX || leg4SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else
        {
            double diff = strikeDiff - ((leg1SellPrice + leg4SellPrice) - (leg2BuyPrice + leg3BuyPrice));

            QString d = QString::number(diff, 'f', decimal_precision);

            if (portfolio.SellAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = d;
            }
        }


        break;
    }
    case PortfolioType::BFLY_BID:{

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice = DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice =  DBL_MAX;
        double leg3BuyPrice =  DBL_MAX;
        double leg3SellPrice =  DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        if (leg2SellPrice == DBL_MAX || leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX){
            portfolio.SellAveragePrice = "-";
        }
        else{
            double diffVal = -(2 * leg2SellPrice) + leg1BuyPrice + leg3BuyPrice;
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = diff;
            }

        }
        if (leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else{
            double diffVal = -leg1SellPrice - leg3SellPrice + (2 * leg2BuyPrice);
            QString diff = QString::number(diffVal, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != diff)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = diff;
            }

        }
        break;
    }

    case PortfolioType::BS1221:{

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice =DBL_MAX;
        double leg2BuyPrice =  DBL_MAX;
        double leg2SellPrice =  DBL_MAX;
        double leg3BuyPrice =  DBL_MAX;
        double leg3SellPrice =  DBL_MAX;
        double leg4BuyPrice =  DBL_MAX;
        double leg4SellPrice = DBL_MAX;


            QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
            if(averagePriceList.contains(key1)){
                PortfolioAvgPrice leg1 = averagePriceList[key1];
                leg1SellPrice = leg1.AvgPrice;
            }

            key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
            if(averagePriceList.contains(key1)){
                PortfolioAvgPrice leg1 = averagePriceList[key1];
                leg1BuyPrice = leg1.AvgPrice;
            }

            QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
            if(averagePriceList.contains(key2)){
                PortfolioAvgPrice leg2 = averagePriceList[key2];
                leg2BuyPrice = leg2.AvgPrice;
            }
            key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
            if(averagePriceList.contains(key2)){
                PortfolioAvgPrice leg2 = averagePriceList[key2];
                leg2SellPrice = leg2.AvgPrice;
            }
            QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
            if(averagePriceList.contains(key3)){
                PortfolioAvgPrice leg3 = averagePriceList[key3];
                leg3SellPrice = leg3.AvgPrice;
            }
            key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
            if(averagePriceList.contains(key3)){
                PortfolioAvgPrice leg3 = averagePriceList[key3];
                leg3BuyPrice = leg3.AvgPrice;
            }
            QString key4 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg4TokenNo);
            if(averagePriceList.contains(key4)){
                PortfolioAvgPrice leg4 = averagePriceList[key4];
                leg4BuyPrice = leg4.AvgPrice;
            }
            key4 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg4TokenNo);
            if(averagePriceList.contains(key4)){
                PortfolioAvgPrice leg4 = averagePriceList[key4];
                leg4SellPrice = leg4.AvgPrice;
            }



        if (leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX || leg2SellPrice == DBL_MAX || leg4SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            //double diff = -leg1SellPrice - (3 * leg3SellPrice) + (3 * leg2BuyPrice) + leg4BuyPrice;
            double diff = -leg1BuyPrice - (2 * leg3BuyPrice) + (2 * leg2SellPrice) + leg4SellPrice;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = d;
            }
        }

        if (leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX || leg4BuyPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else
        {
            //double diff = -leg4SellPrice - (3 * leg2SellPrice) + (3 * leg3BuyPrice) + leg1BuyPrice;
            double diff = -leg4BuyPrice - (2 * leg2BuyPrice) + (2 * leg3SellPrice) + leg1SellPrice;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = d;
            }
        }

        break;
    }

    case PortfolioType::BS1331:{

        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice =DBL_MAX;
        double leg2BuyPrice =  DBL_MAX;
        double leg2SellPrice =  DBL_MAX;
        double leg3BuyPrice =  DBL_MAX;
        double leg3SellPrice =  DBL_MAX;
        double leg4BuyPrice =  DBL_MAX;
        double leg4SellPrice = DBL_MAX;

        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }
        QString key3 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3SellPrice = leg3.AvgPrice;
        }
        key3 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg3TokenNo);
        if(averagePriceList.contains(key3)){
            PortfolioAvgPrice leg3 = averagePriceList[key3];
            leg3BuyPrice = leg3.AvgPrice;
        }
        QString key4 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4BuyPrice = leg4.AvgPrice;
        }
        key4 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg4TokenNo);
        if(averagePriceList.contains(key4)){
            PortfolioAvgPrice leg4 = averagePriceList[key4];
            leg4SellPrice = leg4.AvgPrice;
        }

        if (leg1BuyPrice == DBL_MAX || leg3BuyPrice == DBL_MAX || leg2SellPrice == DBL_MAX || leg4SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            //double diff = -leg1SellPrice - (3 * leg3SellPrice) + (3 * leg2BuyPrice) + leg4BuyPrice;
            double diff = -leg1BuyPrice - (3 * leg3BuyPrice) + (3 * leg2SellPrice) + leg4SellPrice;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = d;
            }
        }

        if (leg1SellPrice == DBL_MAX || leg3SellPrice == DBL_MAX || leg2BuyPrice == DBL_MAX || leg4BuyPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else
        {
            //double diff = -leg4SellPrice - (3 * leg2SellPrice) + (3 * leg3BuyPrice) + leg1BuyPrice;
            double diff = -leg4BuyPrice - (3 * leg2BuyPrice) + (3 * leg3SellPrice) + leg1SellPrice;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.SellAveragePrice = d;
            }
        }
    }

    case PortfolioType::OPEN2L:{
        double leg1BuyPrice = DBL_MAX;
        double leg1SellPrice =DBL_MAX;
        double leg2BuyPrice = DBL_MAX;
        double leg2SellPrice = DBL_MAX;
        QString key1 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1SellPrice = leg1.AvgPrice;
        }
        key1 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg1TokenNo);
        if(averagePriceList.contains(key1)){
            PortfolioAvgPrice leg1 = averagePriceList[key1];
            leg1BuyPrice = leg1.AvgPrice;
        }

        QString key2 = QString::number(portfolio.PortfolioNumber)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2BuyPrice = leg2.AvgPrice;
        }
        key2 = QString::number(portfolio.PortfolioNumber + 1500000)+"_"+QString::number(portfolio.Leg2TokenNo);
        if(averagePriceList.contains(key2)){
            PortfolioAvgPrice leg2 = averagePriceList[key2];
            leg2SellPrice = leg2.AvgPrice;
        }


        QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
        double ratio = portfolio.Leg1Ratio/portfolio.Leg2Ratio;


        if (leg2BuyPrice == DBL_MAX || leg1SellPrice == DBL_MAX)
        {
            portfolio.SellAveragePrice = "-";
        }
        else
        {
            double diff = (leg1SellPrice - ratio * leg2BuyPrice);

            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.SellAveragePrice != d)
            {
                portfolio.AverageUpdateTime =QDateTime::currentDateTime();
                portfolio.SellAveragePrice = d;
            }
        }
        if (leg1BuyPrice == DBL_MAX || leg2SellPrice == DBL_MAX)
        {
            portfolio.BuyAveragePrice = "-";
        }
        else
        {
            double diff = ratio * leg2SellPrice - leg1BuyPrice;
            QString d = QString::number(diff, 'f', decimal_precision);
            if (portfolio.BuyAveragePrice != d)
            {
                portfolio.AverageUpdateTime = QDateTime::currentDateTime();
                portfolio.BuyAveragePrice = d;
            }
        }


        break;
    }

    default:
        break;
    }
}

void PortfolioParser::CalculatePriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash,double devicer,int decimal_precision) {
    try {

        if(MBP_Data_Hash.contains(QString::number(portfolio.FutToken))){
            MBP_Data_Struct mbpData = MBP_Data_Hash[QString::number(portfolio.FutToken)];
            portfolio.FuturePrice = QString::number(mbpData.lastTradedPrice.toDouble() * 1.0 / devicer, 'f', decimal_precision);

        }




        switch (portfolio.PortfolioTypeEnum) {
        case PortfolioType::F2F:
            return CalculateF2FPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision);
        case PortfolioType::BY:
            return CalculateBFLYPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);
        case PortfolioType::OPEN_BY:
            return CalculateOPENBFLYPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision);
        case PortfolioType::CR:
        case PortfolioType::CR_JELLY:
            return CalculateConRevPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);

        case PortfolioType::BX_MKT:
        case PortfolioType::BOX:
        case PortfolioType::BX_BID:
            return CalculateBoxPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);
        case PortfolioType::BS1221:
            return CalculateBS1221PriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);
        case PortfolioType::BS1331:
            return CalculateBS1331PriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);


        case PortfolioType::OPEN_BOX:
            return CalculateOpenBoxPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);

        case PortfolioType::OPEN2L:
            return CalculateOpen2LPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);


            /* case PortfolioType::R2L:
                     return CalculateR2LPriceDifference(portfolio);
                 case PortfolioType::R3L:
                     return Calculate3LPriceDifference(portfolio);
                 case PortfolioType::BS:
                     return CalculateBSPriceDifference(portfolio);
                 case PortfolioType::BS1221:
                     return CalculateBS1221PriceDifference(portfolio);

                 case PortfolioType::JBX:
                     return CalculateJellyBoxPriceDifference(portfolio);
                 case PortfolioType::JBX_2L:
                 case PortfolioType::JBX_3L:
                     return CalculateJellyBox2LPriceDifference(portfolio);
                 case PortfolioType::BX_3L:*/
            /*{
                         //return CalculateBox3LPriceDifference(portfolio);
                      }//*/
            /* return;
                 case PortfolioType::BX_2L:
                     return CalculateBox2LPriceDifference(portfolio);
                 case PortfolioType::BYC_4L:
                     return CalculateBYC4LPriceDifference(portfolio);
                 case PortfolioType::BYC_5L:
                     return CalculateBYC5LPriceDifference(portfolio);
                 case PortfolioType::BYC_6L:
                     return CalculateBYC6LPriceDifference(portfolio);*/
        case PortfolioType::BFLY_BID:
            return CalculateBFLYPriceDifference(portfolio,MBP_Data_Hash,devicer,decimal_precision,portfolio.PortfolioTypeEnum);


        default:
            break;
        }
    } catch (...) {}


}

void PortfolioParser::CalculateOpen2LPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type)
{
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }


    double ratio = portfolio.Leg1Ratio/portfolio.Leg2Ratio;




    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];


    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();


    portfolio.Cost = QString::number((( (leg1.lastTradedPrice.toDouble() + leg2.lastTradedPrice.toDouble()) * 2 * ratio ) / devicer), 'f', decimal_precision);

    double diff = (-ratio * leg2SellPrice + leg1BuyPrice);
    diff = diff / devicer;


    if (diff != portfolio.SellMarketRate)
    {
        portfolio.SellMarketRate = diff;
    }

    diff = -leg1SellPrice + ratio * leg2BuyPrice;
    diff = diff / devicer;

    if (portfolio.BuyMarketRate != diff)
    {
        portfolio.BuyMarketRate = diff;
    }

    return;
}

void PortfolioParser::CalculateF2FPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash,double devicer,int decimal_precision)
{
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate =0;//DBL_MAX;
        return;
    }


    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();


    portfolio.Cost = QString::number((((leg1.lastTradedPrice.toDouble() + leg2.lastTradedPrice.toDouble()) * 2 * 0.000085) / devicer), 'f', decimal_precision);

    double diff = (leg1BuyPrice - leg2SellPrice) / devicer;
    if (portfolio.SellMarketRate != diff)
    {
        portfolio.SellMarketRate = diff;
    }

    diff = (leg2BuyPrice - leg1SellPrice) / devicer;
    if (portfolio.BuyMarketRate != diff)
    {
        portfolio.BuyMarketRate = diff;
    }

    return;
}

void PortfolioParser::CalculateOPENBFLYPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash,double devicer, int decimal_precision)
{

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();

    portfolio.Cost = QString::number((((leg1.lastTradedPrice.toDouble() * 2 + leg2.lastTradedPrice.toDouble() * 4 +
                                        leg3.lastTradedPrice.toDouble() * 2) * 0.0009) / devicer), 'f', decimal_precision);

    double diff = (-(2 * leg2SellPrice) + leg1BuyPrice + leg3BuyPrice) / devicer;

    if (portfolio.SellMarketRate != diff)
    {
        portfolio.SellMarketRate = diff;
    }

    diff = (-leg1SellPrice - leg3SellPrice + (2 * leg2BuyPrice)) / devicer;
    if (diff != portfolio.BuyMarketRate)
    {
        portfolio.BuyMarketRate = diff;
    }

    return;
}

void PortfolioParser::CalculateBFLYPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash,double devicer, int decimal_precision, int type)
{

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    /*if (!SlowDataReader::GetBuyPrice(leg1, leg1BuyPrice) || !SlowDataReader::GetBuyPrice(leg2, leg2BuyPrice) ||
           !SlowDataReader::GetBuyPrice(leg3, leg3BuyPrice) || !SlowDataReader::GetSellPrice(leg1, leg1SellPrice) ||
           !SlowDataReader::GetSellPrice(leg2, leg2SellPrice) || !SlowDataReader::GetSellPrice(leg3, leg3SellPrice))
       {
           return ;
       }*/




    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();

    portfolio.Cost = QString::number((((leg1.lastTradedPrice.toDouble() * 2 + leg2.lastTradedPrice.toDouble() * 4 +
                                        leg3.lastTradedPrice.toDouble() * 2) * 0.0009) / devicer), 'f', decimal_precision);

    double diff = (-(2 * leg2SellPrice) + leg1BuyPrice + leg3BuyPrice) / devicer;

    if (portfolio.SellMarketRate != diff)
    {
        portfolio.SellMarketRate = diff;
    }

    diff = (-leg1SellPrice - leg3SellPrice + (2 * leg2BuyPrice)) / devicer;
    if (diff != portfolio.BuyMarketRate)
    {
        portfolio.BuyMarketRate = diff;
    }

    return;
}

void PortfolioParser::CalculateConRevPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash,double devicer,int decimal_precision, int type)
{


    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return ;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return ;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return ;
    }

    /*if (!SlowDataReader::GetBuyPrice(leg1, leg1BuyPrice) || !SlowDataReader::GetBuyPrice(leg2, leg2BuyPrice) ||
           !SlowDataReader::GetBuyPrice(leg3, leg3BuyPrice) || !SlowDataReader::GetSellPrice(leg1, leg1SellPrice) ||
           !SlowDataReader::GetSellPrice(leg2, leg2SellPrice) || !SlowDataReader::GetSellPrice(leg3, leg3SellPrice))
       {
           return false;
       }*/
    QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
    int strikePrice = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;

    if(Contract.contains(QString::number(portfolio.Leg2TokenNo)))
        strikePrice = Contract[QString::number(portfolio.Leg2TokenNo)].StrikePrice;
    else{
        qDebug()<<"Waring: Cannot get Leg2TokenNo = "<<portfolio.Leg2TokenNo<<" CR for Price calculation "<<"portfopliNo="<<portfolio.PortfolioNumber;
    }




    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();




    portfolio.Cost = QString::number((((leg1.lastTradedPrice.toDouble() * 2 * 0.000085) +
                                       (leg2.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg3.lastTradedPrice.toDouble() * 2 * 0.0009)) / devicer), 'f', decimal_precision);

   // strikePrice = strikePrice*devicer;
    bool ret = false;
    double diff = (-strikePrice - leg2SellPrice + leg1BuyPrice + leg3BuyPrice) * 1.0 / devicer;

   /* qDebug()

        <<"strikePrice="<<strikePrice<<
        "portfolioNo="<<portfolio.PortfolioNumber<<
        " Leg1TokenNo="<<portfolio.Leg1TokenNo<<
        " Leg2TokenNo="<<portfolio.Leg2TokenNo<<
        " Leg3TokenNo="<<portfolio.Leg3TokenNo;

    qDebug()<<
        " leg1BuyPrice="<<QString::asprintf("%.0f", leg1BuyPrice)<<" leg1SellPrice="<<QString::asprintf("%.0f", leg1SellPrice)<<
        " leg2BuyPrice="<<QString::asprintf("%.0f", leg2BuyPrice)<<"   leg2SellPrice="<<QString::asprintf("%.0f", leg2SellPrice)<<
        " leg3BuyPrice="<<QString::asprintf("%.0f", leg3BuyPrice)<<"   leg3SellPrice="<<QString::asprintf("%.0f", leg3SellPrice);
    qDebug()<<"\n\n";*/
    if (portfolio.SellMarketRate != diff)
    {
        ret = true;
        portfolio.SellMarketRate = diff;
    }

    diff = (strikePrice + leg2BuyPrice - leg1SellPrice - leg3SellPrice) * 1.0 / devicer;
    if (portfolio.BuyMarketRate != diff)
    {
        ret = true;
        portfolio.BuyMarketRate = diff;
    }

    return;
}



void PortfolioParser::CalculateBoxPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type)
{

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg4TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    /* if (!SlowDataReader::GetBuyPrice(leg1, leg1BuyPrice) || !SlowDataReader::GetBuyPrice(leg2, leg2BuyPrice) ||
           !SlowDataReader::GetBuyPrice(leg3, leg3BuyPrice) || !SlowDataReader::GetBuyPrice(leg4, leg4BuyPrice) ||
           !SlowDataReader::GetSellPrice(leg1, leg1SellPrice) || !SlowDataReader::GetSellPrice(leg2, leg2SellPrice) ||
           !SlowDataReader::GetSellPrice(leg3, leg3SellPrice) || !SlowDataReader::GetSellPrice(leg4, leg4SellPrice))
       {
           return;
       }*/
    QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
    int strikePriceLeg3 = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
    if(Contract.contains(QString::number(portfolio.Leg3TokenNo)))
        strikePriceLeg3 = Contract[QString::number(portfolio.Leg3TokenNo)].StrikePrice;
    int strikePriceLeg1 = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
    if(Contract.contains(QString::number(portfolio.Leg1TokenNo)))
        strikePriceLeg1 = Contract[QString::number(portfolio.Leg1TokenNo)].StrikePrice;



    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    MBP_Data_Struct leg4 = MBP_Data_Hash[QString::number(portfolio.Leg4TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();
    double leg4BuyPrice = leg4.recordBuffer[0].price.toDouble();
    double leg4SellPrice = leg4.recordBuffer[5].price.toDouble();

    portfolio.Cost = QString::number((((leg1.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg2.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg3.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg4.lastTradedPrice.toDouble() * 2 * 0.0009)) / devicer), 'f', decimal_precision);

    double strikeDiff =  strikePriceLeg3 -  strikePriceLeg1;
    double diff = leg3BuyPrice + leg2BuyPrice - leg4SellPrice - leg1SellPrice + strikeDiff;
    diff = diff / devicer;


    if (diff != portfolio.BuyMarketRate)
    {
        portfolio.BuyMarketRate = diff;
    }

    diff = -strikeDiff  - leg3SellPrice - leg2SellPrice + leg4BuyPrice + leg1BuyPrice;
    diff = diff / devicer;

    if (portfolio.SellMarketRate != diff)
    {
        portfolio.SellMarketRate = diff;
    }

    return;
}

void PortfolioParser::CalculateBS1221PriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type)
{

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg4TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    /* if (!SlowDataReader::GetBuyPrice(leg1, leg1BuyPrice) || !SlowDataReader::GetBuyPrice(leg2, leg2BuyPrice) ||
           !SlowDataReader::GetBuyPrice(leg3, leg3BuyPrice) || !SlowDataReader::GetBuyPrice(leg4, leg4BuyPrice) ||
           !SlowDataReader::GetSellPrice(leg1, leg1SellPrice) || !SlowDataReader::GetSellPrice(leg2, leg2SellPrice) ||
           !SlowDataReader::GetSellPrice(leg3, leg3SellPrice) || !SlowDataReader::GetSellPrice(leg4, leg4SellPrice))
       {
           return;
       }*/
   // QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
    // int strikePriceLeg3 = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
    // if(Contract.contains(QString::number(portfolio.Leg3TokenNo)))
    //     strikePriceLeg3 = Contract[QString::number(portfolio.Leg3TokenNo)].StrikePrice;
    // int strikePriceLeg1 = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
    // if(Contract.contains(QString::number(portfolio.Leg1TokenNo)))
    //     strikePriceLeg1 = Contract[QString::number(portfolio.Leg1TokenNo)].StrikePrice;



    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    MBP_Data_Struct leg4 = MBP_Data_Hash[QString::number(portfolio.Leg4TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();
    double leg4BuyPrice = leg4.recordBuffer[0].price.toDouble();
    double leg4SellPrice = leg4.recordBuffer[5].price.toDouble();
    double costMultiplier = 0.0009;
    double Cost = (leg1.lastTradedPrice.toDouble() * 2 +
                   leg2.lastTradedPrice.toDouble() * 4 +
                   leg3.lastTradedPrice.toDouble() * 4 +
                   leg4.lastTradedPrice.toDouble() * 2) * costMultiplier;
    Cost = Cost/devicer;
    portfolio.Cost = QString::number(Cost,'f', decimal_precision);


    double BuyMarketRate = (leg4BuyPrice + (2 * leg2BuyPrice)) - (leg1SellPrice + (2 * leg3SellPrice));
    BuyMarketRate = BuyMarketRate / devicer;
    portfolio.BuyMarketRate = BuyMarketRate;


    double SellMarketRate = (leg1BuyPrice + (2 * leg3BuyPrice)) - (leg4SellPrice + (2 * leg2SellPrice));

    SellMarketRate = SellMarketRate / devicer;
    portfolio.SellMarketRate = SellMarketRate;

    return;
}

void PortfolioParser::CalculateBS1331PriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type)
{

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg4TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }



    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    MBP_Data_Struct leg4 = MBP_Data_Hash[QString::number(portfolio.Leg4TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();
    double leg4BuyPrice = leg4.recordBuffer[0].price.toDouble();
    double leg4SellPrice = leg4.recordBuffer[5].price.toDouble();


    double costMultiplier = 0.0009;
    double Cost = ((leg1.lastTradedPrice.toDouble() * 2 +
                    leg2.lastTradedPrice.toDouble() * 6 +
                    leg3.lastTradedPrice.toDouble() * 6 +
                    leg4.lastTradedPrice.toDouble() * 2) * costMultiplier);


    Cost = Cost/devicer;
    portfolio.Cost = QString::number(Cost,'f', decimal_precision);


    double BuyMarketRate = (leg4BuyPrice + (3 * leg2BuyPrice)) - (leg1SellPrice + (3 * leg3SellPrice));
    BuyMarketRate = BuyMarketRate / devicer;
    portfolio.BuyMarketRate = BuyMarketRate;

    double SellMarketRate = (leg1BuyPrice + (3 * leg3BuyPrice)) - (leg4SellPrice + (3 * leg2SellPrice));
    SellMarketRate = SellMarketRate / devicer;
    portfolio.SellMarketRate = SellMarketRate;

    return;
}

void PortfolioParser::CalculateOpenBoxPriceDifference(PortfolioObject &portfolio, QHash<QString, MBP_Data_Struct> MBP_Data_Hash, double devicer, int decimal_precision, int type)
{

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg1TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg2TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg3TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }
    if (!MBP_Data_Hash.contains(QString::number(portfolio.Leg4TokenNo)))
    {
        portfolio.BuyMarketRate = 0;//DBL_MAX;
        portfolio.SellMarketRate = 0;//DBL_MAX;
        return;
    }

    /* if (!SlowDataReader::GetBuyPrice(leg1, leg1BuyPrice) || !SlowDataReader::GetBuyPrice(leg2, leg2BuyPrice) ||
           !SlowDataReader::GetBuyPrice(leg3, leg3BuyPrice) || !SlowDataReader::GetBuyPrice(leg4, leg4BuyPrice) ||
           !SlowDataReader::GetSellPrice(leg1, leg1SellPrice) || !SlowDataReader::GetSellPrice(leg2, leg2SellPrice) ||
           !SlowDataReader::GetSellPrice(leg3, leg3SellPrice) || !SlowDataReader::GetSellPrice(leg4, leg4SellPrice))
       {
           return;
       }*/
     QHash<QString, contract_table> Contract =  ContractDetail::getInstance().GetContracts();
    int strikePriceLeg3 = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
    if(Contract.contains(QString::number(portfolio.Leg3TokenNo)))
        strikePriceLeg3 = Contract[QString::number(portfolio.Leg3TokenNo)].StrikePrice;
    int strikePriceLeg1 = 0;//ContractDetail::GetDetail(portfolio.Leg2TokenNo).StrikePrice;
    if(Contract.contains(QString::number(portfolio.Leg1TokenNo)))
        strikePriceLeg1 = Contract[QString::number(portfolio.Leg1TokenNo)].StrikePrice;



    MBP_Data_Struct leg1 = MBP_Data_Hash[QString::number(portfolio.Leg1TokenNo)];
    MBP_Data_Struct leg2 = MBP_Data_Hash[QString::number(portfolio.Leg2TokenNo)];
    MBP_Data_Struct leg3 = MBP_Data_Hash[QString::number(portfolio.Leg3TokenNo)];
    MBP_Data_Struct leg4 = MBP_Data_Hash[QString::number(portfolio.Leg4TokenNo)];
    double leg1BuyPrice = leg1.recordBuffer[0].price.toDouble();
    double leg1SellPrice = leg1.recordBuffer[5].price.toDouble();
    double leg2BuyPrice = leg2.recordBuffer[0].price.toDouble();
    double leg2SellPrice = leg2.recordBuffer[5].price.toDouble();
    double leg3BuyPrice = leg3.recordBuffer[0].price.toDouble();
    double leg3SellPrice = leg3.recordBuffer[5].price.toDouble();
    double leg4BuyPrice = leg4.recordBuffer[0].price.toDouble();
    double leg4SellPrice = leg4.recordBuffer[5].price.toDouble();

    portfolio.Cost = QString::number((((leg1.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg2.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg3.lastTradedPrice.toDouble() * 2 * 0.0009) +
                                       (leg4.lastTradedPrice.toDouble() * 2 * 0.0009)) / devicer), 'f', decimal_precision);

    double diff = leg1BuyPrice + leg4BuyPrice - leg2SellPrice - leg3SellPrice - strikePriceLeg3 +  strikePriceLeg1;
    diff = diff / devicer;


    if (diff != portfolio.BuyMarketRate)
    {
        portfolio.BuyMarketRate = diff;
    }

    diff = strikePriceLeg3 - strikePriceLeg1 - leg1SellPrice - leg4SellPrice + leg2BuyPrice + leg3BuyPrice;
    diff = diff / devicer;

    if (portfolio.SellMarketRate != diff)
    {
        portfolio.SellMarketRate = diff;
    }

    return;
}
