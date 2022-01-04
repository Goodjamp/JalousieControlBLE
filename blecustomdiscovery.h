#ifndef BLECUSTOMDISCOVERY_H
#define BLECUSTOMDISCOVERY_H

#include <QObject>

#include <QVector>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include "blecustomservice.h"

class BleCustomDiscovery: public QObject
{
    Q_OBJECT

public:

    typedef struct {
        QString name;
    } TargtDeviceMetaData;

    typedef enum {
        Ok,
        DiscoverdInProcess,
        DiscoverdCompleteOk,
        DiscoverdCompleteError,
    } Status;

    BleCustomDiscovery();
    int getDiscoveredDevices(QVector<QBluetoothDeviceInfo> devices);
    Status startDiscovered(TargtDeviceMetaData metaData, int discoveredTimeout);
    void stopDiscovered();

signals:
    void bleCustomDiscoveryDeviceDiscovered(QBluetoothDeviceInfo device);
    void bleCustomDiscoveryDiscoveredComplete(Status status, QVector<QBluetoothDeviceInfo> devices);

private:
    TargtDeviceMetaData userDeviceMetaData;
    QBluetoothDeviceDiscoveryAgent *bleDiscoveryAgent;
    QVector<QBluetoothDeviceInfo> discoveredDevices;

private slots:
    void discoveredDevice(const QBluetoothDeviceInfo &device);
    void scanFinished();
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);

};

#endif // BLECUSTOMDISCOVERY_H
