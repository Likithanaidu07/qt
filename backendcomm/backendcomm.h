#ifndef BACKENDCOMM_H
#define BACKENDCOMM_H


#include <QObject>
#include <QTcpSocket>
#include "QSettings"
#include "QDir"
#include <QQueue>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>




enum SocketDataType{
    BACKEND_COMM_SOCKET_DATA=1,
    BACKEND_COMM_SOCKET_ERROR=2,
    BACKEND_COMM_SOCKET_WARNING=3,
    BACKEND_COMM_SOCKET_INFO=4,
    BACKEND_COMM_SOCKET_CONNECTED=5,
    BACKEND_COMM_SEND_SUCCESS=6,
    BACKEND_COMM_SEND_FAILED=7

};



class BackendComm : public QObject
{
    Q_OBJECT
public:
    explicit BackendComm(QObject *parent = nullptr);
    void connectServer();
    ~BackendComm();
    void quitDataFetchThread();
    void moveToThreadAndNotify(QThread* thread);
    QByteArray createPacket(quint16 command, QByteArray message);
    int UserId;



signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void dataReceived(const QByteArray &data);
    void sendDataSignal(QByteArray);
    void socketData(QString,SocketDataType);
    void newMessageReceived(QString);
    void threadMoved();
public slots:
    void insertData(QByteArray);
    //void disconnect();
    void sendDataSlot(QByteArray);


private slots:
    /* void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);*/
    void processQueue();
    void readSocket();
    void socket_disconnected();
    void socket_connected();
    void displayError(QAbstractSocket::SocketError socketError);
    void processMessage(const QString& str);
    void onThreadMoved();

private:
    QTcpSocket *socket;
    QQueue<QByteArray> m_dataQueue;
    QMutex m_mutex;
    bool runThread;
    void createSocket();
    void sendKeepAlive();
    QByteArray intToByteArray(int value);
    QElapsedTimer keepAliveSendTimer;


};



#endif // BACKENDCOMM_H
