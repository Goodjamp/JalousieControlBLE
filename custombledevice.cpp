#include "custombledevice.h"

#include "QMap"
#include "QVector"
#include "QByteArray"

#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QUuid"
#include "QDebug"

customBleDevice::customBleDevice(QBluetoothDeviceInfo targetBleDevice,
                                 QMap<QByteArray,QVector<QByteArray>> servCharList)
{
    servCharTargetList = servCharList;
    qDebug()<<"Connect to BLE device"<<targetBleDevice.name();
    bleDevice = QLowEnergyController::createCentral(targetBleDevice, this);
    connect(bleDevice, &QLowEnergyController::connected,
            this, &customBleDevice::deviceConnected);
    connect(bleDevice, &QLowEnergyController::disconnected,
            this, &customBleDevice::deviceDisconnected);
    connect(bleDevice, &QLowEnergyController::serviceDiscovered,
            this, &customBleDevice::deviceServiceDiscovered);
    connect(bleDevice, &QLowEnergyController::discoveryFinished,
            this, &customBleDevice::deviceDiscoveryFinished);

    bleDevice->connectToDevice();
}

void customBleDevice::deviceConnected()
{}

void customBleDevice::deviceDisconnected()
{}

void customBleDevice::deviceStateChanged(QLowEnergyController::ControllerState state)
{}

void customBleDevice::deviceError(QLowEnergyController::Error newError)
{}

void customBleDevice::deviceServiceDiscovered(const QBluetoothUuid &newService)
{
    servisesList.push_back(newService);
}

void customBleDevice::deviceDiscoveryFinished()
{
    foreach(auto service, servisesList){
        if (service.minimumSize() == 2) {
            continue;
        }
        if (servCharTargetList.value(service.toByteArray()).size() == 0) {
                continue;
        }
    }
}

/********Service generic slots******************/
void customBleDevice::serviceProtostateChanged(QLowEnergyService::ServiceState newState)
{}

void customBleDevice::serviceProtoCharRead(const QLowEnergyCharacteristic &info,
                                           const QByteArray &value)
{}
