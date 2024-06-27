#include "slowdata_indices_socket.h"

//#define ENABLE_DEBUG_MSG

/************Socket server receive data from slowdata exchange**********************/

#include "QRegularExpression"
#include <Ws2tcpip.h>
#include <lzo/lzo1z.h>
#include "QSettings"
#include "QDir"
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
    char reserved1[2];
    char reserved2[2];
    qint32 logTime;
    char alphaChar[2];
    qint16 transactionCode;
    qint16 errorCode;
    qint32 bcSeqNo;
    char reserved3;
    char reserved4[3];
    char timeStamp2[8];
    char filler2[8];
    qint16 messageLength;
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
struct MS_INDICES
{
    CHAR indexName[22];
    quint32 indexValue;
    quint32 highIndexValue;
    quint32 lowIndexValue;
    quint32 openingIdx;
    quint32 closingIDx;
    quint32 percentChange;
    quint32 yearlyHigh;
    quint32 yearlyLow;
    quint32 noOfUpmoves;
    quint32 noOfDownmoves;
    DOUBLE marketCapitialisation;
    CHAR netChangeIndicator;
    CHAR reserved;
};
#pragma pop

#pragma pack (push, 2)
struct MS_BCAST_INDICES
{
    BCAST_HEADER header;
    SHORT noOfRecords;
    MS_INDICES msiData[6];
};
#pragma pop




Slowdata_Indices_Socket::Slowdata_Indices_Socket(QObject *parent)
    : QThread{parent}
{
    run_thread = true;
    MBP_Data_Struct  MBP_Data;

}

void Slowdata_Indices_Socket::run()
{
    mc_port     = 34330;
    const char* mc_addr_str = "233.1.2.6";
    std::string ipStdString;
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    if(groups.contains("ExchangeServerDetails")){
        settings.beginGroup("ExchangeServerDetails");
        if (settings.contains("cm_mult_ip")){
            QString ip_address = settings.value("cm_mult_ip").toString();
            ipStdString = ip_address.toStdString();
            mc_addr_str = ipStdString.c_str();
        }
        else{
            qDebug()<<"Warning: Settings.ini not contain cm_mult_ip";
        }

        if (settings.contains("cm_mult_port")){
            mc_port = settings.value("cm_mult_port").toString().toUShort();
        }
        else{
            qDebug()<<"Warning: Settings.ini not contain cm_mult_port";
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
    qDebug()<<("Initialised Indices socket.\n");
#endif
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("Slowdata_Indices_Socket: socket() failed");
#endif

        wchar_t *s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, WSAGetLastError(),
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPWSTR)&s, 0, NULL);
        fprintf(stderr, "%S\n", s);
        LocalFree(s);


        emit socket_conn_info_Signal("Slowdata_Indices_Socket: ssocket() failed");

        return;
    }

    if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&flag_on, sizeof(flag_on))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("Slowdata_Indices_Socket: setsockopt() failed");
#endif
        emit socket_conn_info_Signal("Slowdata_Indices_Socket: setsockopt() failed");

        return;
    }

    memset(&mc_addr,0,sizeof(mc_addr));
    mc_addr.sin_family = AF_INET;
    mc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    mc_addr.sin_port = htons(mc_port);

    if((bind(sock,(struct sockaddr *) &mc_addr,sizeof(mc_addr))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("Slowdata_Indices_Socket: bind() failed");
#endif
        emit socket_conn_info_Signal("Slowdata_Indices_Socket: Error: bind() failed");
        return;
    }

    mc_req.imr_multiaddr.s_addr = inet_addr(mc_addr_str);
    mc_req.imr_interface.s_addr = htonl(INADDR_ANY);

    if((setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(const char*) &mc_req,sizeof(mc_req))) < 0)
    {
#ifdef ENABLE_DEBUG_MSG
        qDebug()<<("Slowdata_Indices_Socket: setsockopt() failed");
#endif
        emit socket_conn_info_Signal("Slowdata_Indices_Socket: Error: setsockopt() failed");

        return;
    }

    //FILE* fd = fopen("dump.bin", "wb");

    unsigned int dst_len = 1024;
    unsigned char dst[1024];

    while(run_thread)
    {
        memset(recv_str,0,sizeof(recv_str));
        from_len = sizeof(from_addr);
        memset(&from_addr,0,from_len);

        // emit socket_conn_info_Signal("Waiting to recveive data....");

        if((recv_len = recvfrom(sock, recv_str,1024,0,(struct sockaddr*)&mc_addr, &from_len))<0)
        {
#ifdef ENABLE_DEBUG_MSG
            qDebug()<<("Slowdata_Indices_Socket: recvfrom() failed");
#endif
            emit socket_conn_info_Signal("Slowdata_Indices_SocketError: recvfrom() failed");

            break;
        }


        if (recv_len <= 0) continue;


        /* *************DEBUGGING CODE*****************/
        /*    size_t dataSize = sizeof(recv_str) - 1;
           QString hexString;

           // Convert binary data to hexadecimal representation
           for (size_t i = 0; i < 100; ++i) {
              // hexString += QString::number(i)+"-->"+QString("%1 ").arg(static_cast<unsigned char>(recv_str[i]), 2, 16, QLatin1Char('0'))+"   ";
             //  hexString += QString("0x%1, ").arg(static_cast< char>(recv_str[i]), 2, 16, QLatin1Char('0'));
                 hexString += QString("%1 ").arg(static_cast< char>(recv_str[i]), 2, 16, QLatin1Char('0'));

           }
           qDebug()<<("MS_BCAST_INDICES data: ----- ")<<hexString;

           QString hexValue1 = QString("recv_str[24]-%1").arg(static_cast< char>(recv_str[24]), 2, 16, QChar('0'));
           QString hexValue2 = QString("recv_str[25]-%1").arg(static_cast< char>(recv_str[25]), 2, 16, QChar('0'));
           qDebug()<<hexValue1<<hexValue2;
           // Extract characters from recv_str[24] and recv_str[25]
               char byte1 = recv_str[25];  // Note: Swap index 25 and 24
               char byte2 = recv_str[24];

               // Convert bytes to a short value (little-endian byte order)
               short transactionCode = static_cast<short>(byte1) | (static_cast<short>(byte2) << 8);
               if(transactionCode== 7207){

                   BCAST_HEADER* header = reinterpret_cast<BCAST_HEADER*>(recv_str + 14);


                   short st = ntohs(header->transactionCode);
                   MS_BCAST_INDICES* mbi = reinterpret_cast<MS_BCAST_INDICES*>(recv_str + 14);

                   int p=0;
               }


              qDebug() << "Transaction Code: " << transactionCode;*/


        MS_BCAST_INDICES* mbi = reinterpret_cast<MS_BCAST_INDICES*>(recv_str + 14);
        const SHORT transactionCode = ntohs(mbi->header.transactionCode);
        //qDebug()<<("recv data Thread 1 : transactionCode---- ")<<transactionCode<<"mbp->header.messageLength: "<<mbp->header.messageLength;
        //qDebug()<<("recv slow data indices : transactionCode---- ")<<transactionCode<<"mbp->header.messageLength: "<<mbi->header.messageLength;

        if (transactionCode == 7207 || transactionCode == 7216){


            // qDebug()<<("recv slow data : transactionCode---- ")<<transactionCode<<"mbp->header.messageLength: "<<mbi->header.messageLength;
            qint64 timeStampIST =  ntohl(mbi->header.logTime) - 19800; // exchange timestamp is in GMT, so subtract 5 hours 30 minutes to it.
            data_exchangeTimestamp.storeRelaxed(timeStampIST);
            for (int j = 0; j < ntohs(mbi->noOfRecords); ++j) {
                QString indexName =  QString(mbi->msiData[j].indexName);
                long indexValue  = ntohl(mbi->msiData[j].indexValue);
                long highIndexValue = ntohl(mbi->msiData[j].highIndexValue);
                long lowIndexValue = ntohl(mbi->msiData[j].lowIndexValue);
                long openingIdx = ntohl(mbi->msiData[j].openingIdx);
                long closingIDx = ntohl(mbi->msiData[j].closingIDx);
                long percentChange = ntohl(mbi->msiData[j].percentChange);
                long yearlyHigh = ntohl(mbi->msiData[j].yearlyHigh);
                long yearlyLow = ntohl(mbi->msiData[j].yearlyLow);
                long noOfUpmoves = ntohl(mbi->msiData[j].noOfUpmoves);
                long noOfDownmoves = ntohl(mbi->msiData[j].noOfDownmoves);
                double marketCapitialisation = ntohd(mbi->msiData[j].marketCapitialisation);
                QString netChangeIndicator = QString(mbi->msiData[j].netChangeIndicator);
                QString reserved = QString(mbi->msiData[j].reserved);

                QString dataStr = "indexName: "+indexName+"\n"+
                                  "indexValue: "+QString::number(indexValue)+"\n"+
                                  "highIndexValue: "+QString::number(highIndexValue)+"\n"+
                                  "lowIndexValue: "+QString::number(lowIndexValue)+"\n"+
                                  "openingIdx: "+QString::number(openingIdx)+"\n"+
                                  "closingIDx: "+QString::number(closingIDx)+"\n"+
                                  "percentChange: "+QString::number(percentChange)+"\n"+
                                  "yearlyHigh: "+QString::number(yearlyHigh)+"\n"+
                                  "yearlyLow: "+QString::number(yearlyLow)+"\n"+
                                  "noOfUpmoves: "+QString::number(noOfUpmoves)+"\n"+
                                  "noOfDownmoves: "+QString::number(noOfDownmoves)+"\n"+
                                  "marketCapitialisation: "+QString::number(marketCapitialisation)+"\n"+
                                  "netChangeIndicator: "+netChangeIndicator+"\n"+
                                  "reserved: "+reserved;

                emit socket_conn_info_Signal("MS_BCAST_INDICES Data: \n================\n"+dataStr+"\n\n");
                indexName = indexName.trimmed();
                // qDebug()<<"indexName:  "<<indexName;
                double indexValueD = indexValue/100.0; //"N", "BN", "FN","VIX"
                if(indexName =="Nifty 50"){
                    Indices_Data_Struct data;
                    data.indexName =indexName;
                    data.indexValue = QString::number(indexValueD,'f',2);
                    data.change = QString::number(((indexValue-closingIDx)/100.0),'f',2);
                    data.percentagechange = QString::number(((percentChange)/100.0),'f',2);
                    data.netChangeIndicator = netChangeIndicator;
                    data.highIndexValue = QString::number(highIndexValue);
                    data.lowIndexValue = QString::number(lowIndexValue);
                    data.openingIdx = QString::number(openingIdx);
                    data.closingIdx = QString::number(closingIDx);
                    data.marketCapitialisation = QString::number(marketCapitialisation);
                    data.display_widget_idx = IndicesDataWidgetID::WIDGET_NIFTY_50;
                    emit dataSignal(data);
                }
                else if(indexName == "Nifty Bank"){
                    Indices_Data_Struct data;
                    data.indexName =indexName;
                    data.indexValue = QString::number(indexValueD,'f',2);
                    data.change = QString::number(((indexValue-closingIDx)/100.0),'f',2);
                    data.percentagechange = QString::number(((percentChange)/100.0),'f',2);
                    data.netChangeIndicator = netChangeIndicator;
                    data.highIndexValue = QString::number(highIndexValue);
                    data.lowIndexValue = QString::number(lowIndexValue);
                    data.openingIdx = QString::number(openingIdx);
                    data.closingIdx = QString::number(closingIDx);
                    data.marketCapitialisation = QString::number(marketCapitialisation);
                    data.display_widget_idx = IndicesDataWidgetID::WIDGET_NIFTY_BANK;
                    emit dataSignal(data);
                }
                else if(indexName == "India VIX"){
                    Indices_Data_Struct data;
                    data.indexName =indexName;
                    indexValueD = indexValueD/100.0; // for VIX it need to device with 10000 follow swastika app
                    data.indexValue = QString::number(indexValueD,'f',2);
                    data.change = QString::number(((indexValue-closingIDx)/10000.0),'f',2);// for VIX it need to device with 10000 follow swastika app
                    data.percentagechange = QString::number(((percentChange)/10000.0),'f',2);
                    data.netChangeIndicator = netChangeIndicator;
                    data.highIndexValue = QString::number(highIndexValue);
                    data.lowIndexValue = QString::number(lowIndexValue);
                    data.openingIdx = QString::number(openingIdx);
                    data.closingIdx = QString::number(closingIDx);
                    data.marketCapitialisation = QString::number(marketCapitialisation);
                    data.display_widget_idx = IndicesDataWidgetID::WIDGET_INDIA_VIX;
                    emit dataSignal(data);
                }
                else if(indexName == "Nifty Fin Service"){
                    Indices_Data_Struct data;
                    data.indexName =indexName;
                    data.indexValue = QString::number(indexValueD,'f',2);
                    data.change = QString::number(((indexValue-closingIDx)/100.0),'f',2);
                    data.percentagechange = QString::number(((percentChange)/100.0),'f',2);
                    data.netChangeIndicator = netChangeIndicator;
                    data.highIndexValue = QString::number(highIndexValue);
                    data.lowIndexValue = QString::number(lowIndexValue);
                    data.openingIdx = QString::number(openingIdx);
                    data.closingIdx = QString::number(closingIDx);
                    data.marketCapitialisation = QString::number(marketCapitialisation);
                    data.display_widget_idx = IndicesDataWidgetID::WIDGET_NIFTY_FINANCIAL_SERVICES;
                    emit dataSignal(data);
                }




                /*  if(indexName.contains("Nifty 50")){
                                size_t dataSize = sizeof(recv_str) - 1;
                                 QString hexString;

                                 // Convert binary data to hexadecimal representation
                                 for (size_t i = 0; i < dataSize; ++i) {
                                    // hexString += QString::number(i)+"-->"+QString("%1 ").arg(static_cast<unsigned char>(recv_str[i]), 2, 16, QLatin1Char('0'))+"   ";
                                   //  hexString += QString("0x%1, ").arg(static_cast< char>(recv_str[i]), 2, 16, QLatin1Char('0'));
                                       hexString += QString("%1 ").arg(static_cast< char>(recv_str[i]), 2, 16, QLatin1Char('0'));

                                 }
                                 qDebug()<<"MS_BCAST_INDICES data("<<indexName<<"):  "<<hexString;
                                }*/

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
}

void Slowdata_Indices_Socket::sendUDP_TO_Quit(){
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


