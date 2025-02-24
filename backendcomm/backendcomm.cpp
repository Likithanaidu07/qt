/***********Socket client send data to backend server****************/
#include "backendcomm.h"
#include "QThread"
#include "QAbstractSocket"
#include <QElapsedTimer>
#include <defines.h>

//#define ENABLE_DEBUG_MSG

BackendComm::BackendComm(QObject *parent) : QObject(parent)
{

    connect(this,SIGNAL(sendDataSignal(QByteArray)),this,SLOT(sendDataSlot(QByteArray)));
    connect(this, SIGNAL(threadMoved()), this, SLOT(onThreadMoved()));

    createSocket();
    runThread = true;
    QFuture<void> fut = QtConcurrent::run(&BackendComm::processQueue,this);
}

void BackendComm::moveToThreadAndNotify(QThread* thread) {
    // Move the object to the specified thread
    moveToThread(thread);

    // Emit the signal to notify about the thread move
    emit threadMoved();
}
void BackendComm::onThreadMoved() {
    // Thread moved, call connectServer() here
    connectServer();
}


void BackendComm::createSocket(){
    socket = new QTcpSocket(this);
   // connect(this, &BackendComm::newMessageReceived, this, &BackendComm::processMessage);
    connect(socket, &QTcpSocket::readyRead, this, &BackendComm::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &BackendComm::socket_disconnected);
    connect(socket, &QAbstractSocket::errorOccurred, this, &BackendComm::displayError);
    connect(socket, &QAbstractSocket::connected, this, &BackendComm::socket_connected);

    //this->connectServer();

}

void BackendComm::sendDataSlot(QByteArray byteArray){
    //  mesg = mesg+"#";
    // qDebug()<<"sending data to socket...."<<byteArray;

    if(socket)
    {
        if(socket->isOpen())
        {
            // qDebug()<<"Sending socket Data"<<mesg;
            if(socket->state()==QAbstractSocket::ConnectedState){

                socket->write(byteArray);

                if (socket->waitForBytesWritten()) {
                    // qDebug() << "Backend Data sent successfully.";
                    quint16 command;
                    memcpy(&command, byteArray.constData(), sizeof(quint16)); // Extract command
                    if (command != BACKEND_CMD_TYPE::CMD_ID_KEEP_ALIVE) {
                        emit socketData("Backend Data sent successfully. command = " + QString::number(command), SocketDataType::BACKEND_COMM_SEND_SUCCESS);
                    }


                }
                else {
                    // qDebug() << "Failed to send Backend data:" << socket->errorString();
                    quint16 command;
                    memcpy(&command, byteArray.constData(), sizeof(quint16)); // Extract command
                    if (command != BACKEND_CMD_TYPE::CMD_ID_KEEP_ALIVE) {
                        emit socketData("Backend Data sent failed.... command = " + QString::number(command), SocketDataType::BACKEND_COMM_SEND_FAILED);
                    }

                }

                //  QDataStream socketStream(socket);
                // socketStream.setVersion(QDataStream::Qt_5_15);

                /* QByteArray header;
                header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(mesg.size()).toUtf8());
                header.prepend(mesg.toUtf8());
                header.resize(128);*/

                //  QByteArray byteArray = mesg.toUtf8();
                //  byteArray.resize(64); //sedning 64 bytes fixed length
                //byteArray.prepend(header);
                //  socketStream << byteArray;

                /* QByteArray byteArray = mesg.toUtf8();
                QByteArray dataSize;
                QDataStream out(&dataSize, QIODevice::WriteOnly);
                out << (quint32)byteArray.size(); // add size prefix to data
                socket->write(dataSize);
                socket->write(byteArray);*/
            }
            else if(socket->state()==QAbstractSocket::UnconnectedState){
                emit socketData("BackendComm Client: Socket doesn't seem to be connected, connecting....",SocketDataType::BACKEND_COMM_SOCKET_INFO);
                connectServer();
            }
            else{
                emit socketData("BackendComm: Socket doesn't seem to be connecting....",SocketDataType::BACKEND_COMM_SOCKET_INFO);
            }

        }
        else{
            //QMessageBox::critical(this,"BackendComm","Socket doesn't seem to be opened");
            emit socketData("BackendComm: Socket doesn't seem to be opened",SocketDataType::BACKEND_COMM_SOCKET_ERROR);
            //initSocket();
            connectServer();

        }
    }
    else{

        createSocket();
        emit socketData("BackendComm: Not connected",SocketDataType::BACKEND_COMM_SOCKET_ERROR);
        //initSocket();

    }
}

void BackendComm::connectServer()
{

    //  qDebug()<<"currentThreadId: " << QThread::currentThreadId();

    int port = 6000;
    QString ip_address = "192.168.0.11";//"192.168.15.63";
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QSettings settings(appDataPath + "/settings.ini", QSettings::IniFormat);
    QStringList groups = settings.childGroups();
    if(groups.contains("BackendServerDetails")){
        settings.beginGroup("BackendServerDetails");
        ip_address = settings.value("ip").toString();
        port = settings.value("port").toInt();
        settings.endGroup();

    }
    //  ip_address = "192.168.0.11";//"192.168.15.63";

    emit socketData("BackendComm: Connecting to IP: "+ip_address+" and port: "+QString::number(port),SocketDataType::BACKEND_COMM_SOCKET_INFO);
    socket->connectToHost(QHostAddress(ip_address),port);
    if (socket->waitForConnected(-1))
    {
        emit socketData("waitForConnected: ",SocketDataType::BACKEND_COMM_SOCKET_CONNECTED);

    }
    else{
        emit socketData("BackendComm: Socket waitForConnected failed...",SocketDataType::BACKEND_COMM_SOCKET_ERROR);
        QTimer::singleShot(5000, this, &BackendComm::connectServer);

    }
    int p = 0;
}



QByteArray BackendComm::createPacket(quint16 command, QByteArray message)
{
    // Get the length prefix for the message
    /* QByteArray lengthPrefix;
    QDataStream lengthPrefixStream(&lengthPrefix, QIODevice::WriteOnly);
    lengthPrefixStream.setByteOrder(QDataStream::LittleEndian); // Set little-endian byte order
    lengthPrefixStream << qint32(message.size() + 2); // 2 bytes for the command

    QByteArray commandPrefix;
    QDataStream commandPrefixStream(&commandPrefix, QIODevice::WriteOnly);
    commandPrefixStream.setByteOrder(QDataStream::LittleEndian); // Set little-endian byte order
    commandPrefixStream << qint16(command); // 2 bytes for the command


    // Concatenate the length prefix and the message
    QByteArray ret(lengthPrefix + commandPrefix+ message);*/
    QByteArray ret;
    QDataStream retStream(&ret, QIODevice::WriteOnly);
    retStream.setByteOrder(QDataStream::LittleEndian); // Set little-endian byte order
    retStream << qint16(message.size() + 2);
    retStream << qint16(command);
    ret = ret+message;
    // ret.resize(64);
    return ret;
}

QByteArray BackendComm::intToByteArray(int value)
{
    QByteArray byteArray;
    byteArray.resize(4); // Set the size to 4 bytes

    byteArray[3] = static_cast<char>((value >> 24) & 0xFF);
    byteArray[2] = static_cast<char>((value >> 16) & 0xFF);
    byteArray[1] = static_cast<char>((value >> 8) & 0xFF);
    byteArray[0] = static_cast<char>(value & 0xFF);
    return byteArray;
}

void BackendComm::sendKeepAlive(){
    quint16 command = BACKEND_CMD_TYPE::CMD_ID_KEEP_ALIVE;
    QByteArray data = intToByteArray(UserId);
    QByteArray pkt = this->createPacket(command, data);
    this->insertData(pkt);
}



void BackendComm::insertData(QByteArray data)
{
    QMutexLocker locker(&m_mutex);
    m_dataQueue.enqueue(data);

}


void BackendComm::processQueue()
{

    keepAliveSendTimer.start();
    // QElapsedTimer timer;
    // timer.start();
    while (runThread) {
        if (!m_dataQueue.isEmpty()) {
            QMutexLocker locker(&m_mutex);
            QByteArray data = m_dataQueue.dequeue();
            locker.unlock();
            emit sendDataSignal(data);
            //timer.restart();
            // m_socket.write(data.toUtf8());
            // m_socket.flush();
            keepAliveSendTimer.restart();
        }
        QThread::msleep(200); // Sleep for a while to avoid busy loop
        // if no data send for last 5 second send heartbeat
        // if(timer.elapsed()>5000){
        // this->insertData("H");
        // }
        // if greater than 10 sec
        if(keepAliveSendTimer.elapsed()>10000){
            sendKeepAlive();
            keepAliveSendTimer.restart();
        }
    }

    qDebug()<<"Exiting backend communication thread....";


}

void BackendComm::quitDataFetchThread(){
    runThread = false;

}


BackendComm::~BackendComm()
{
    runThread = false;
    if(socket->isOpen())
        socket->close();

    delete socket;
}

void BackendComm::readSocket()
{

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
       QByteArray data = socket->readAll();

       // Ensure that the received data has at least 4 bytes (2 bytes for length, 2 bytes for command)
       if (data.size() >= 4)
       {
           QDataStream stream(data);
           stream.setByteOrder(QDataStream::LittleEndian); // Ensure we read in the correct byte order

           // Read the total size (first 2 bytes, not really needed but part of the packet)
           quint16 totalSize;
           stream >> totalSize;

           // Read the command (next 2 bytes)
           quint16 command;
           stream >> command;

           qDebug() << "BackendComm::  Command received: " << command;


           switch (command) {
               case BACKEND_CMD_TYPE::CMD_ID_TRADE_UPDATED_200:
                        emit socketData("CMD_ID_TRADE_UPDATED_200",SocketDataType::BACKEND_COMM_SOCKET_DATA);
                        break;
               case BACKEND_CMD_TYPE::CMD_ID_TRADE_UPDATED_CMD_300:
                        emit socketData("CMD_ID_TRADE_UPDATED_CMD_300",SocketDataType::BACKEND_COMM_SOCKET_DATA);
                        break;
               default:
                    break;
           }

           // Read the rest of the message (remaining bytes)
          // QByteArray message = data.mid(4); // Skipping first 4 bytes (size + command)
          // qDebug() << "BackendComm:: Message received: " << message.toHex();
       }
       else
       {
           qDebug() << "BackendComm:: Received data is too short to contain a valid packet.";
       }

//   QByteArray data = socket->readAll();
//           qDebug() << "Data received: " << data;



//    QByteArray buffer;

//    QDataStream socketStream(socket);
//    socketStream.setVersion(QDataStream::Qt_6_3);

//    socketStream.startTransaction();
//    socketStream >> buffer;

//    if(!socketStream.commitTransaction())
//    {
//        QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
//        emit newMessageReceived(message);
//        return;
//    }

//    QString header = buffer.mid(0,128);
//    QString fileType = header.split(",")[0].split(":")[1];

//    buffer = buffer.mid(128);

//    qDebug()<<"Backend data Received:"<<header;


//    if(fileType=="message"){
//        QString message = QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString()));
//        emit newMessageReceived(message);
//    }
}

void BackendComm::socket_connected(){
    emit socketData("BackendComm: Socket connected...",SocketDataType::BACKEND_COMM_SOCKET_CONNECTED);

}

void BackendComm::socket_disconnected()
{
    socket->deleteLater();
    socket=nullptr;
    emit socketData("BackendComm: Socket Disconnected...",SocketDataType::BACKEND_COMM_SOCKET_WARNING);
    // ui->statusBar->showMessage("Disconnected!");
    createSocket();
    QTimer::singleShot(5000, this, &BackendComm::connectServer);
}

void BackendComm::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        //QMessageBox::information(this, "BackendComm", "The host was not found. Please check the host name and port settings.");
        emit socketData("QTCPClient: The host was not found. Please check the host name and port settings.",SocketDataType::BACKEND_COMM_SOCKET_ERROR);

        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit socketData("BackendComm: The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.",SocketDataType::BACKEND_COMM_SOCKET_ERROR);
        //  QMessageBox::information(this, "QTCPClient", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
    default:
        emit socketData(QString("BackendComm: The following error occurred: %1.").arg(socket->errorString()),SocketDataType::BACKEND_COMM_SOCKET_ERROR);

        //  QMessageBox::information(this, "BackendComm", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}


void BackendComm::processMessage(const QString& str)
{
    emit socketData(str,SocketDataType::BACKEND_COMM_SOCKET_DATA);
}

