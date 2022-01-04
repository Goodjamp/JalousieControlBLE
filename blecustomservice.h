#ifndef BLECUSTOMSERVICE_H
#define BLECUSTOMSERVICE_H

#include <QObject>

#include "QMap"
#include "QVector"
#include "QByteArray"

#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QtBluetooth/QBluetoothUuid"

class BleCustomService : public QObject
{
    Q_OBJECT

public:

    typedef enum {
        Ok,
        UndefinedUuidError,
        DataSizeError,
        ServiceCreateError,
    } Status;

    BleCustomService(QLowEnergyController *targetCotroller,
                     QBluetoothUuid targetService,
                     QObject *paren);
    Status startDiscoveryCharacteristics(void);
    Status characteristicWrite(quint128 characteristic,
                               const QByteArray &value);
    Status characteristicRead(quint128 characteristic);

signals:
    void characteristicDiscoveryComplete(quint128 service, QVector<quint128> charList);
    void characteristicReceive(quint128 service, quint128 characteristic,
                               const QByteArray &value);

private:
    QLowEnergyService *service;
    QMap<QBluetoothUuid, QLowEnergyCharacteristic> charsList;

    /********Service generic slots******************/
private slots:
    void detailsDiscovered(QLowEnergyService::ServiceState newState);
    void characteristicReadRx(const QLowEnergyCharacteristic &info,
                              const QByteArray &value);

};

#endif // BLECUSTOMSERVICE_H
