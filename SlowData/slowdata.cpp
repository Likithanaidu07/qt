#include "slowdata.h"
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
}
void SlowData::stopSlowDataSocket()
{
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

void SlowData::setLeg_n_token(QStringList tokens){
    QMutexLocker lock(&leg_n_tokens_Mutex);
    leg_n_tokens.clear();
    leg_n_tokens = tokens;
}

void SlowData::addLeg_n_token(QString token){
    QMutexLocker lock(&leg_n_tokens_Mutex);
    if(!leg_n_tokens.contains(token)){
        leg_n_tokens.append(token);
        qDebug()<<"leg_n_tokens: "<<leg_n_tokens;
    }
}


QHash<QString, MBP_Data_Struct> SlowData::getMBP_Data_Hash()
{
    QMutexLocker lock(&MBP_Data_Hash_Mutex);
    return MBP_Data_Hash;
}
