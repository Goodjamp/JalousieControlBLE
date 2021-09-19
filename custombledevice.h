#ifndef CUSTOMBLEDEVICE_H
#define CUSTOMBLEDEVICE_H

#include <QObject>

#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QtBluetooth/QBluetoothUuid"

class customBleDevice: public QObject
{
    Q_OBJECT
public:
    customBleDevice(QBluetoothDeviceInfo targetBleDevice,
                    QMap<QByteArray,QVector<QByteArray>> servCharList);

private slots:

    /********BLE device generic slots******************/
    void deviceConnected();
    void deviceDisconnected();
    void deviceStateChanged(QLowEnergyController::ControllerState state);
    void deviceError(QLowEnergyController::Error newError);
    void deviceServiceDiscovered(const QBluetoothUuid &newService);
    void deviceDiscoveryFinished();

    /********Service generic slots******************/
    void serviceProtostateChanged(QLowEnergyService::ServiceState newState);
    void serviceProtoCharRead(const QLowEnergyCharacteristic &info,
                              const QByteArray &value);

private:
    QLowEnergyController *bleDevice;
    QVector<QBluetoothUuid> servisesList;
    QLowEnergyService *service;
    QList<QLowEnergyCharacteristic> charsList;
    QMap<QByteArray,QVector<QByteArray>> servCharTargetList;

};

#endif // CUSTOMBLEDEVICE_H
