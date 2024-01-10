#include "slowdataindices.h"

/*QMutex SlowDataIndices::mutex;
QMutex SlowDataIndices::MBP_Data_Hash_Mutex; // Definition and initialization of the QMutex member variable
QHash<QString, MBP_Data_Struct> SlowDataIndices::MBP_Data_Hash;
QStringList SlowDataIndices::leg_n_tokens;
QMutex SlowDataIndices::leg_n_tokens_Mutex; // Definition and initialization of the QMutex member variable

*/
SlowDataIndices::SlowDataIndices(QObject *parent)
    : QObject{parent}
{

}

void SlowDataIndices::startSlowDataSocket()
{
    slowData_Indices_socket = new Slowdata_Indices_Socket();
    QObject::connect(slowData_Indices_socket, SIGNAL(socket_conn_info_Signal(QString)), this, SLOT(socket_conn_info_Slot(QString)));
    QObject::connect(slowData_Indices_socket, SIGNAL(dataSignal(Indices_Data_Struct)), this, SLOT(dataExchangeSlot(Indices_Data_Struct)));
    slowData_Indices_socket->start();
}
void SlowDataIndices::stopSlowDataSocket()
{
    //stop data_exch_socket
    slowData_Indices_socket->sendUDP_TO_Quit();
    slowData_Indices_socket->quit();
    slowData_Indices_socket->wait();
    delete slowData_Indices_socket;
}


void SlowDataIndices::socket_conn_info_Slot(QString info){
    // ui->textEditLog->append(info);
    // qDebug().noquote()<<"Indices Data "<<info;
}

void SlowDataIndices::dataExchangeSlot(Indices_Data_Struct data){

    //  qDebug()<<data.indexName;
    emit indicesDataSignal(data);
    /* QMutexLocker lock(&leg_n_tokens_Mutex);
    if(leg_n_tokens.contains(data.token)){
        QMutexLocker lock(&MBP_Data_Hash_Mutex);
        MBP_Data_Hash.insert(data.token, data);
    }*/

}

/*void SlowDataIndices::setLeg_n_token(QStringList tokens){
     QMutexLocker lock(&leg_n_tokens_Mutex);
     leg_n_tokens.clear();
     leg_n_tokens = tokens;
}

QHash<QString, MBP_Data_Struct> SlowDataIndices::getMBP_Data_Hash()
{
    QMutexLocker lock(&MBP_Data_Hash_Mutex);
    return MBP_Data_Hash;
}*/
