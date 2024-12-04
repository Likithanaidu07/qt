#ifndef SLOWDATA_H
#define SLOWDATA_H

#include <QObject>
#include "slowdatasocket.h"
#include "QMutex"

class SlowData : public QObject
{
    Q_OBJECT
public:
    explicit SlowData(QObject *parent = nullptr);
    void stopSlowDataSocket();
    void startSlowDataSocket();
    QHash<QString, MBP_Data_Struct> getMBP_Data_Hash();
    void setLeg_n_token(QStringList);
    void addLeg_n_token(QString);
    QStringList getMonitoringTokens();
    void saveMBPDataToFile();
    void loadMBPDataFromFile();

private:
    SlowDataSocket *slowData_socket;
    static QMutex mutex;
    static QMutex MBP_Data_Hash_Mutex;
    static QMutex leg_n_tokens_Mutex;
    static QHash<QString, MBP_Data_Struct> MBP_Data_Hash;
    static QStringList leg_n_tokens;

private slots:
    void socket_conn_info_Slot(QString info);
    void dataExchangeSlot(MBP_Data_Struct data);

signals:
   void newSlowDataReceivedSignal(const QHash<QString, MBP_Data_Struct> &data);

};

#endif // SLOWDATA_H
