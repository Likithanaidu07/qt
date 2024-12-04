#include "slowdata.h"
#include "QStandardPaths"
#include "QFile"

QMutex SlowData::mutex;
QMutex SlowData::MBP_Data_Hash_Mutex; // Definition and initialization of the QMutex member variable
QHash<QString, MBP_Data_Struct> SlowData::MBP_Data_Hash;
QStringList SlowData::leg_n_tokens;
QMutex SlowData::leg_n_tokens_Mutex; // Definition and initialization of the QMutex member variable

SlowData::SlowData(QObject *parent)
    : QObject{parent}
{
}

void SlowData::startSlowDataSocket()
{
    slowData_socket = new SlowDataSocket();
    QObject::connect(slowData_socket, SIGNAL(socket_conn_info_Signal(QString)), this, SLOT(socket_conn_info_Slot(QString)));
    QObject::connect(slowData_socket, SIGNAL(dataSignal(MBP_Data_Struct)), this, SLOT(dataExchangeSlot(MBP_Data_Struct)));
    slowData_socket->start();

    loadMBPDataFromFile();

}
void SlowData::stopSlowDataSocket()
{

    saveMBPDataToFile();

    //stop data_exch_socket
    slowData_socket->sendUDP_TO_Quit();
    slowData_socket->quit();
    slowData_socket->wait();
    delete slowData_socket;
}


void SlowData::socket_conn_info_Slot(QString info){
    // ui->textEditLog->append(info);
    //qDebug()<<"SlowDataSocket: "<<info;
}

void SlowData::dataExchangeSlot(MBP_Data_Struct data){

    //qDebug()<<data.token;
    QMutexLocker lock(&leg_n_tokens_Mutex);
    if(leg_n_tokens.contains(data.token)){
        QMutexLocker lock(&MBP_Data_Hash_Mutex);
        // the insert will overwrite the date if data.token already exist
        //qDebug()<<"MBP_Data_Struct Token: "<<data.token;
        MBP_Data_Hash.insert(data.token, data);
        emit newSlowDataReceivedSignal(MBP_Data_Hash);
    }

}

void SlowData::setLeg_n_token(QStringList tokens) {
    QMutexLocker lock(&leg_n_tokens_Mutex);

    // Append only tokens that are not already in leg_n_tokens
    for (const QString& token : tokens) {
        if (!leg_n_tokens.contains(token)) {
            leg_n_tokens.append(token);
        }
    }
}

void SlowData::addLeg_n_token(QString token){
    QMutexLocker lock(&leg_n_tokens_Mutex);
    if(!leg_n_tokens.contains(token)){
        leg_n_tokens.append(token);
    }
   // qDebug()<<"leg_n_tokens: "<<leg_n_tokens;

}

QStringList SlowData::getMonitoringTokens(){
    QMutexLocker lock(&leg_n_tokens_Mutex);
    return leg_n_tokens;
}


QHash<QString, MBP_Data_Struct> SlowData::getMBP_Data_Hash()
{
    QMutexLocker lock(&MBP_Data_Hash_Mutex);
    return MBP_Data_Hash;
}
// Serialize mbpInfo
QDataStream &operator<<(QDataStream &out, const mbpInfo &info) {
    out << info.quantity;
    out << info.price;
    out << info.numberOfOrders;
    out << info.bbBuySellFlag;
    return out;
}

// Deserialize mbpInfo
QDataStream &operator>>(QDataStream &in, mbpInfo &info) {
    in >> info.quantity;
    in >> info.price;
    in >> info.numberOfOrders;
    in >> info.bbBuySellFlag;
    return in;
}
// Serialize
QDataStream &operator<<(QDataStream &out, const MBP_Data_Struct &data) {
    out << data.token;
    out << data.volumeTradedToday;
    out << data.netChangeIndicator;
    out << data.netPriceChangeFromClosingPrice;
    out << data.lastTradedPrice;
    out << data.lastTradeQuantity;
    out << data.lastTradeTime;
    out << data.averageTradePrice;
    out << data.totalBuyQuantity;
    out << data.totalSellQuantity;
    out << data.closingPrice;
    out << data.openPrice;
    out << data.highPrice;
    out << data.lowPrice;
    out << data.recordBuffer; // Ensure mbpInfo is serializable
    return out;
}

// Deserialize
QDataStream &operator>>(QDataStream &in, MBP_Data_Struct &data) {
    in >> data.token;
    in >> data.volumeTradedToday;
    in >> data.netChangeIndicator;
    in >> data.netPriceChangeFromClosingPrice;
    in >> data.lastTradedPrice;
    in >> data.lastTradeQuantity;
    in >> data.lastTradeTime;
    in >> data.averageTradePrice;
    in >> data.totalBuyQuantity;
    in >> data.totalSellQuantity;
    in >> data.closingPrice;
    in >> data.openPrice;
    in >> data.highPrice;
    in >> data.lowPrice;
    in >> data.recordBuffer; // Ensure mbpInfo is deserializable
    return in;
}

void SlowData::saveMBPDataToFile(){
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = appDataPath+"/Data/mbp_data.bin";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not open file for writing.");
        return;
    }
    QDataStream out(&file);
    out << MBP_Data_Hash;
    file.close();
}

void SlowData::loadMBPDataFromFile() {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = appDataPath + "/Data/mbp_data.bin";
    QFile file(fileName);

    if (!file.exists()) {
        qWarning(QString("File does not exist. No data to load."+fileName).toStdString().c_str());
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning(QString("Could not open file for reading."+fileName).toStdString().c_str());

        return;
    }

    QDataStream in(&file);

    // Ensure the stream version matches what was used during serialization
    //in.setVersion(QDataStream::Qt_5_15); // Example: adjust version as per your setup

    // Load the data
    try {
        in >> MBP_Data_Hash;
    } catch (...) {
        qWarning("Failed to deserialize MBP_Data_Hash. File might be corrupted.");
    }

    file.close();
}

