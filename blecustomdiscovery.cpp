#include "blecustomdiscovery.h"

#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QDebug>

BleCustomDiscovery::BleCustomDiscovery()
{
    discoveredDevices.clear();
    bleDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(bleDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BleCustomDiscovery::discoveredDevice);
    connect(bleDiscoveryAgent,  static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error),
            this, &BleCustomDiscovery::scanError);
    connect(bleDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BleCustomDiscovery::scanFinished);
    connect(bleDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
            this, &BleCustomDiscovery::scanFinished);
}

BleCustomDiscovery::Status BleCustomDiscovery::startDiscovered(TargtDeviceMetaData metaData, int discoveredTimeout)
{
    if (bleDiscoveryAgent->isActive()) {
        return DiscoverdInProcess;
    }

    discoveredDevices.clear();
    userDeviceMetaData = metaData;
    qDebug()<<"BleCustomDiscovery: target BLE device name"<<userDeviceMetaData.name;
    bleDiscoveryAgent->setLowEnergyDiscoveryTimeout(discoveredTimeout);
    bleDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    return Ok;
}

void BleCustomDiscovery::stopDiscovered()
{
    bleDiscoveryAgent->stop();
}

void BleCustomDiscovery::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    qDebug()<<"BleCustomDiscovery: scan error";
    emit bleCustomDiscoveryDiscoveredComplete(DiscoverdCompleteError, discoveredDevices);
}

void BleCustomDiscovery::scanFinished()
{
    qDebug()<<"BleCustomDiscovery: discovery complete";
    emit bleCustomDiscoveryDiscoveredComplete(DiscoverdCompleteOk, discoveredDevices);
}

void BleCustomDiscovery::discoveredDevice(const QBluetoothDeviceInfo &device)
{
    qDebug()<<"BleCustomDiscovery: device discovered "<<device.name();
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        if (QString::compare(userDeviceMetaData.name, device.name()) != 0) {
            return;
        }
        qDebug()<<"BleCustomDiscovery: target device found ";
        discoveredDevices.push_back(device);
        emit bleCustomDiscoveryDeviceDiscovered(device);
    }
}
