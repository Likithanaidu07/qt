#ifndef SLOWDATAINDICES_H
#define SLOWDATAINDICES_H

#include <QObject>
#include "slowdata_indices_socket.h"
#include "QMutex"

class SlowDataIndices : public QObject
{
    Q_OBJECT
public:
    explicit SlowDataIndices(QObject *parent = nullptr);
    void stopSlowDataSocket();
    void startSlowDataSocket();
    //QHash<QString, Indices_Data_Struct> getMBP_Data_Hash();
    void setLeg_n_token(QStringList);
private:
    Slowdata_Indices_Socket *slowData_Indices_socket;
    // static QMutex mutex;
    //   static QMutex MBP_Data_Hash_Mutex;
    // static QMutex leg_n_tokens_Mutex;

    //  static QHash<QString, Indices_Data_Struct> MBP_Data_Hash;
    // static QStringList leg_n_tokens;

private slots:
    void socket_conn_info_Slot(QString info);
    void dataExchangeSlot(Indices_Data_Struct data);

signals:
    void indicesDataSignal(Indices_Data_Struct data);
};

#endif // SLOWDATAINDICES_H
