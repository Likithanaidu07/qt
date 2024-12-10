#include "slowdatasocket.h"
#define ENABLE_DEBUG_MSG


/************Socket server receive data from slowdata exchange**********************/

#include "QRegularExpression"
#include <Ws2tcpip.h>
#include <lzo/lzo1z.h>
#include "QSettings"
#include "QDir"
//#define SLOW_DATA_DEBUG
#include <QStandardPaths>

#pragma pack (push, 2)
struct BroadcastPackData
{
    CHAR netId[2];
    SHORT numPackets;
    CHAR packData[512];
};
#pragma pop

#pragma pack (push, 2)
struct BroadcastCompressedPacket
{
    SHORT length;
    CHAR compressedData[512];
};
#pragma pop

#pragma pack (push, 2)
struct BroadcastData
{
    //SHORT length;
    CHAR data[512];
};
#pragma pop

#pragma pack (push, 2)
struct BCAST_HEADER
{
    CHAR reserved[2];
    CHAR reserved1[2];
    LONG logTime;
    CHAR alphaChar[2];
    SHORT transactionCode;
    SHORT errorCode;
    LONG bcSeqNo;
    CHAR reserved2;
    CHAR reserved3[3];
    CHAR timestamp2[8];
    unsigned char filler2[8];
    SHORT messageLength;
};
#pragma pop

#pragma pack (push, 2)
struct MBP_INFORMATION
{
    LONG quantity;
    LONG price;
    SHORT numberOfOrders;
    SHORT bbBuySellFlag;
};
#pragma pop

#pragma pack (push, 1)
struct INTERACTIVE_ONLY_MBP_DATA
{
    LONG token;
    SHORT bookType;
    SHORT tradingStatus;
    LONG volumeTradedToday;
    LONG lastTradedPrice;
    CHAR netChangeIndicator;
    CHAR reservedExtraUnknown;
    LONG netPriceChangeFromClosingPrice;
    LONG lastTradeQuantity;
    LONG lastTradeTime;
    LONG averageTradePrice;
    SHORT auctionNumber;
    SHORT auctionStatus;
    SHORT initiatorType;
    LONG initiatorPrice;
    LONG initiatorQuantity;
    LONG auctionPrice;
    LONG auctionQuantity;
    MBP_INFORMATION recordBuffer[10];
    SHORT bbTotalBuyFlag;
    SHORT bbTotalSellFlag;
    DOUBLE totalBuyQuantity;
    DOUBLE totalSellQuantity;
    SHORT stIndicator;
    LONG closingPrice;
    LONG openPrice;
    LONG highPrice;
    LONG lowPrice;
};
#pragma pop

#pragma pack (push, 2)
struct MS_BCAST_ONLY_MBP
{
    BCAST_HEADER header;
    SHORT noOfRecords;
    INTERACTIVE_ONLY_MBP_DATA mbpData[2];
};
#pragma pop



SlowDataSocket::SlowDataSocket(QObject *parent)
    : QThread{parent}
{
    run_thread = true;
    MBP_Data_Struct  MBP_Data;

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    QString market_type = "fo";
    if(groups.contains("GeneralSettings")){
        settings.beginGroup("GeneralSettings");
        if (settings.contains("market_type"))
            market_type = settings.value("market_type").toString();
        settings.endGroup();
    }
    if(market_type=="fo"){
        decimal_precision = FO_DECIMAL_PRECISION;
    }
    else{
        decimal_precision = CDS_DECIMAL_PRECISION;
    }
}

QString SlowDataSocket::fixDecimal(double num,double decimal_precision){
    QString str = QString::number(num, 'f', decimal_precision);
    //  str.remove( QRegularExpression("0+$") ); // Remove any number of trailing 0's
    //  str.remove( QRegularExpression("\\.$") ); // If the last character is just a '.' then remove it
    return str;
}
void SlowDataSocket::run()
{
    mc_port     = 34330;
    const char* mc_addr_str = "233.1.2.6";
   const  char* interface_ip_addr_str = "0.0.0.0";

    std::string ipStdString;
    std::string ipStdString1;

    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    if(groups.contains("ExchangeServerDetails")){
        settings.beginGroup("ExchangeServerDetails");
        if (settings.contains("fo_mult_ip")){
            QString ip_address = settings.value("fo_mult_ip").toString();
            ipStdString = ip_address.toStdString();
            mc_addr_str = ipStdString.c_str();
        }
        else{
            qDebug()<<"Warning: Settings.ini not contain fo_mult_ip";
        }
        if (settings.contains("mult_interface_ip")){
            QString ip_address = settings.value("mult_interface_ip").toString();
            ipStdString1 = ip_address.toStdString();
            interface_ip_addr_str = ipStdString1.c_str();
        }
        else{
            qDebug()<<"Warning: Settings.ini not contain mult_interface_ip";
        }


        if (settings.contains("fo_mult_port")){
            mc_port = settings.value("fo_mult_port").toString().toUShort();
        }
        else{
            qDebug()<<"Warning: Settings.ini not contain fo_mult_ip";
        }
        settings.endGroup();
    }



    int sock;
    int flag_on = 1;
    struct sockaddr_in mc_addr;
    char recv_str[1024];
    int  recv_len;
    struct ip_mreq mc_req;

    struct sockaddr_in from_addr;
    int from_len;
    WSADATA wsa;


    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        wchar_t *s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, WSAGetLastError(),
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPWSTR)&s, 0, NULL);
        fprintf(stderr, "%S\n", s);
        LocalFree(s);
        emit socket_conn_info_Signal("WSAStartup() failed");

        exit(EXIT_FAILURE);
    }
#ifdef ENABLE_DEBUG_MSG
    qDebug()<<("Initialised slow data socket.\n");
#endif
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("socket() failed");
#endif
        wchar_t *s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, WSAGetLastError(),
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPWSTR)&s, 0, NULL);
        fprintf(stderr, "%S\n", s);
        LocalFree(s);


        emit socket_conn_info_Signal("ssocket() failed");

        return;
    }

    if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag_on, sizeof(flag_on))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("setsockopt() failed");
#endif
        emit socket_conn_info_Signal("setsockopt() failed");

        return;
    }

    memset(&mc_addr,0,sizeof(mc_addr));
    mc_addr.sin_family = AF_INET;
    mc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    mc_addr.sin_port = htons(mc_port);

    if((bind(sock,(struct sockaddr *) &mc_addr,sizeof(mc_addr))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("bind() failed");
#endif
        emit socket_conn_info_Signal("Error: bind() failed");
        return;
    }

   // mc_req.imr_multiaddr.s_addr = inet_addr(mc_addr_str);
   // mc_req.imr_interface.s_addr = htonl(INADDR_ANY);
    //mc_req.imr_interface.s_addr = inet_addr(interface_ip_addr_str);

    // Convert multicast address to binary
    if (inet_pton(AF_INET, mc_addr_str, &mc_req.imr_multiaddr.s_addr) <= 0) {
    #ifdef ENABLE_DEBUG_MSG
        qDebug() << "Error: Invalid multicast address:" << mc_addr_str;
    #endif
        emit socket_conn_info_Signal("Error: Invalid multicast address");
        return;
    }

    // Convert interface IP address to binary
    if (inet_pton(AF_INET, interface_ip_addr_str, &mc_req.imr_interface.s_addr) <= 0) {
    #ifdef ENABLE_DEBUG_MSG
        qDebug() << "Error: Invalid interface address:" << interface_ip_addr_str;
    #endif
        emit socket_conn_info_Signal("Error: Invalid interface address");
        return;
    }


    if((setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(const char*) &mc_req,sizeof(mc_req))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("setsockopt() failed");
#endif
        emit socket_conn_info_Signal("Error: setsockopt() failed");

        return;
    }

    //FILE* fd = fopen("dump.bin", "wb");

    unsigned int dst_len = 1024;
    unsigned char dst[1024];



    while(run_thread)
    {
        // Prepare file descriptor set
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);

        // Set timeout (optional)
        struct timeval timeout;
        timeout.tv_sec = 1;  // 1-second timeout
        timeout.tv_usec = 0;

        // Monitor socket using select
        int select_result = select(sock + 1, &read_fds, NULL, NULL, &timeout);

        if (select_result < 0)
        {
#ifdef ENABLE_DEBUG_MSG
            qDebug() << "select() failed";
#endif
            emit socket_conn_info_Signal("Error: select() failed");
            break;
        }
        else if (select_result == 0)
        {
            // Timeout occurred, no data to read
            continue;
        }

        // Check if the socket is ready for reading
        if (FD_ISSET(sock, &read_fds))
        {
            memset(recv_str, 0, sizeof(recv_str));
            from_len = sizeof(from_addr);
            memset(&from_addr, 0, from_len);

            recv_len = recvfrom(sock, recv_str, 1024, 0, (struct sockaddr *)&mc_addr, &from_len);
            if (recv_len < 0)
            {
#ifdef ENABLE_DEBUG_MSG
                qDebug() << "recvfrom() failed";
#endif
                emit socket_conn_info_Signal("Error: recvfrom() failed");
                break;
            }

            if (recv_len > 0)
            {
                // Process the received data
                SHORT prevLength = 0;
                BroadcastPackData* packData = reinterpret_cast<BroadcastPackData*>(recv_str);
                const SHORT numPackets = ntohs(packData->numPackets);
                for (SHORT i = 0; i < numPackets; ++i)
                {
                    BroadcastData* bcastData = nullptr;
                    BroadcastCompressedPacket* cmpPacket = reinterpret_cast<BroadcastCompressedPacket*>(packData->packData + prevLength);
                    const SHORT cmpLength = ntohs(cmpPacket->length);
                    prevLength += cmpLength;

                    SHORT bcastLength = cmpLength;
                    if (cmpLength > 0)
                    {
                        int rCode = lzo1z_decompress((lzo_bytep) cmpPacket->compressedData, cmpLength,
                                                     (lzo_bytep) dst, (lzo_uintp)&dst_len, 0);
                        if (rCode == LZO_E_OK)
                        {
                            bcastData = reinterpret_cast<BroadcastData*>(dst);
                            bcastLength = dst_len;
                        }
                    }
                    else if (cmpLength == 0)
                    {
                        bcastData = reinterpret_cast<BroadcastData*>(cmpPacket->compressedData);
                    }

                    if ((bcastData == nullptr) || (bcastLength <= 0)) continue;


                    MS_BCAST_ONLY_MBP* mbp = reinterpret_cast<MS_BCAST_ONLY_MBP*>(bcastData->data + 8);
                    const SHORT transactionCode = ntohs(mbp->header.transactionCode);


#ifdef SLOW_DATA_DEBUG
                    qDebug()<<("recv slow data : transactionCode---- ")<<transactionCode<<"mbp->header.messageLength: "<<mbp->header.messageLength;
#endif

                    if (transactionCode != 7208) continue; // BCAST_ONLY_MBP
                    if (ntohs(mbp->header.messageLength) != 470) continue; // size of MS_BCAST_ONLY_MBP is 470


                    //fwrite(mbp, 1, 470, fd);
                    //fflush(fd);
                    //qDebug()<<mbp->header.timestamp2[0];
                    // int timstamp = mbp->header.timestamp2;
                    /*  for (int i = 0; i < sizeof(mbp->header.timestamp2); i++)
            {
                qDebug()<<"mbp->header.timestamp2["<<i<<"] = " <<static_cast<quint8>(mbp->header.timestamp2[i]);
            }*/

                    //qDebug()<<"logTime: "<<ntohl(mbp->header.logTime);
                    qint64 timeStampIST =  ntohl(mbp->header.logTime) - 19800; // exchange timestamp is in GMT, so subtract 5 hours 30 minutes to it.
                    data_exchangeTimestamp.storeRelaxed(timeStampIST);


                    for (int j = 0; j < ntohs(mbp->noOfRecords); ++j)
                    {
                        double startValue = ntohl(mbp->mbpData[j].netPriceChangeFromClosingPrice);
                        double endValue = ntohl(mbp->mbpData[j].lastTradedPrice);
                        double netPriceChangeFromClosingPricePerc = (startValue - endValue)/startValue;
                        netPriceChangeFromClosingPricePerc = netPriceChangeFromClosingPricePerc*100.0;

                        // qDebug()<<"startValue: "<<startValue<<" endValue: "<<endValue;
                        // qDebug()<<"netPriceChangeFromClosingPricePerc: "<<netPriceChangeFromClosingPricePerc;


                        MBP_Data_Struct  MBP_Data;
                        MBP_Data.token = QString::number(ntohl(mbp->mbpData[j].token));
                        MBP_Data.volumeTradedToday = QString::number(ntohl(mbp->mbpData[j].volumeTradedToday));
                        MBP_Data.netChangeIndicator = QString(mbp->mbpData[j].netChangeIndicator);//fixDecimal(ntohl(mbp->mbpData[j].netChangeIndicator)/100.0);
                        MBP_Data.netPriceChangeFromClosingPrice = QString::number(ntohl(mbp->mbpData[j].netPriceChangeFromClosingPrice));//fixDecimal(netPriceChangeFromClosingPricePerc);
                        MBP_Data.lastTradedPrice = fixDecimal(ntohl(mbp->mbpData[j].lastTradedPrice),decimal_precision);
                        MBP_Data.lastTradeQuantity = QString::number(ntohl(mbp->mbpData[j].lastTradeQuantity));
                        MBP_Data.lastTradeTime = QString::number(ntohl(mbp->mbpData[j].lastTradeTime));
                        MBP_Data.averageTradePrice = fixDecimal(ntohl(mbp->mbpData[j].averageTradePrice),decimal_precision);
                        MBP_Data.totalBuyQuantity = QString::number(ntohd(mbp->mbpData[j].totalBuyQuantity));
                        MBP_Data.totalSellQuantity = QString::number(ntohd(mbp->mbpData[j].totalSellQuantity));
                        MBP_Data.closingPrice = fixDecimal(ntohl(mbp->mbpData[j].closingPrice),decimal_precision);
                        MBP_Data.openPrice = fixDecimal(ntohl(mbp->mbpData[j].openPrice),decimal_precision);
                        MBP_Data.highPrice = fixDecimal(ntohl(mbp->mbpData[j].highPrice),decimal_precision);
                        MBP_Data.lowPrice = fixDecimal(ntohl(mbp->mbpData[j].lowPrice),decimal_precision);

                        //first 5 value is Buy data and the remaining 5 value will be sell data
                        for(int i=0;i<10;i++){
                            mbpInfo _mbpInfoTmp;
                            _mbpInfoTmp.quantity = fixDecimal(ntohl(mbp->mbpData[j].recordBuffer[i].quantity),decimal_precision);
                            _mbpInfoTmp.price = QString::number(ntohl(mbp->mbpData[j].recordBuffer[i].price));
                            _mbpInfoTmp.numberOfOrders = QString::number(ntohs(mbp->mbpData[j].recordBuffer[i].numberOfOrders));
                            _mbpInfoTmp.bbBuySellFlag = QString::number(ntohs(mbp->mbpData[j].recordBuffer[i].bbBuySellFlag));
                            MBP_Data.recordBuffer.append(_mbpInfoTmp);
                        }
#ifdef SLOW_DATA_DEBUG
                        qDebug()<<"SlowData:  token = "<<MBP_Data.token;
                        qDebug()<<"SlowData:  price0 = "<<MBP_Data.recordBuffer[0].price;
                        qDebug()<<"SlowData:  price1 = "<<MBP_Data.recordBuffer[5].price;
#endif
                        emit dataSignal(MBP_Data);



                    }
                }
            }
        }




    }

    if((setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,(const char*)&mc_req, sizeof(mc_req))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("setsocketopt() failed");
#endif
        emit socket_conn_info_Signal("Error: setsocketopt() failed");

        return;

    }

    //fclose(fd);

    closesocket(sock);
    WSACleanup();

    qDebug()<<"Exiting slow data socket thread....";

}

void SlowDataSocket::sendUDP_TO_Quit(){


    run_thread = false;
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[1024];
    char message[1024]="quit";
    WSADATA wsa;

    //Initialise winsock
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //create socket
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    //setup address structure
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(mc_port);
    si_other.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    //send the message
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
    {
        printf("sendto() failed with error code : %d" , WSAGetLastError());
    }
    closesocket(s);
    WSACleanup();


}



