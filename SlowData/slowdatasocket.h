#ifndef SLOWDATASOCKET_H
#define SLOWDATASOCKET_H

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

class SlowDataSocket : public QThread
{
    Q_OBJECT
public:
    explicit SlowDataSocket(QObject *parent = nullptr);
    void run() override;
    void sendUDP_TO_Quit();
    QString fixDecimal(double num,double decimal_precision);

signals:
    void dataSignal(MBP_Data_Struct data);
    void socket_conn_info_Signal(QString info);
    void received_dataSignal(QStringList data);

private:
    bool run_thread;
    unsigned short mc_port;
    double decimal_precision;

};

#endif // SLOWDATASOCKET_H
