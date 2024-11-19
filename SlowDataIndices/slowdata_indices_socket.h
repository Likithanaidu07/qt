#ifndef SLOWDATA_INDICES_SOCKET_H
#define SLOWDATA_INDICES_SOCKET_H

#include "defines.h"
#include <QObject>
#include <QThread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <winsock2.h>
#include <ws2ipdef.h>
#include "qDebug"
#define MAX_LEN 1024;

class Slowdata_Indices_Socket : public QThread
{
    Q_OBJECT
public:
    explicit Slowdata_Indices_Socket(QObject *parent = nullptr);
    void run() override;
    void sendUDP_TO_Quit();
    void getIndexFilter();

signals:
    void dataSignal(Indices_Data_Struct data);
    void socket_conn_info_Signal(QString info);
    void received_dataSignal(QStringList data);

private:
    bool run_thread;
    unsigned short mc_port;
    QStringList indexNameFilter;
    QStringList indexNameAll;
};

#endif // SLOWDATA_INDICES_SOCKET_H
