#include "blecustomdevice.h"

#include "QMap"
#include "QVector"
#include "QStringList"
#include "QByteArray"

#include "QtBluetooth/QLowEnergyController"
#include "QUuid"
#include "QDebug"

BleCustomDevice::BleCustomDevice(void *uContext,
                                 QBluetoothDeviceInfo targetBleDevice,
                                 QVector<quint128> targetServisesList,
                                 QObject *parent)
{
    userContext = uContext;
    userServisesList = targetServisesList;
    bleDeviceInfo = targetBleDevice;
    qDebug()<<"BleCustomDevice: Start connect to BLE device"<<targetBleDevice.name();
    bleDevice = QLowEnergyController::createCentral(targetBleDevice, parent);
    connect(bleDevice, &QLowEnergyController::connected,
            this, &BleCustomDevice::deviceConnected);
    connect(bleDevice, &QLowEnergyController::disconnected,
            this, &BleCustomDevice::deviceDisconnected);
    connect(bleDevice, &QLowEnergyController::serviceDiscovered,
            this, &BleCustomDevice::deviceServiceDiscovered);
    connect(bleDevice, &QLowEnergyController::discoveryFinished,
            this, &BleCustomDevice::deviceDiscoveryFinished);

    /*Connect to device*/
    bleDevice->connectToDevice();
}

BleCustomDevice::BleCustomDeviceStatus BleCustomDevice::characteristicWrite(quint128 service,
                                                                            quint128 characteristic,
                                                                            QByteArray data)
{
    BleCustomService::Status rez;

    if (servicesList.contains(QBluetoothUuid(service)) == false) {
        return UndefinedServiceUuidError;
    }
    rez = servicesList.value(QBluetoothUuid(service))->characteristicWrite(characteristic, data);
    if ( rez != BleCustomService::Ok) {
        qDebug()<<"BleCustomDevice: write characteristic error: "<<rez;
        return WriteCharacteristicError;
    }
    return Ok;
}


void BleCustomDevice::connectToDevice(void)
{
    bleDevice->connectToDevice();
}

QBluetoothDeviceInfo *BleCustomDevice::getBluetoothDeviceInfo()
{
    return &bleDeviceInfo;
}

BleCustomDevice::BleCustomDeviceStatus BleCustomDevice::characteristicRead(quint128 service, quint128 characteristic)
{
    BleCustomService::Status rez;

    if (servicesList.contains(QBluetoothUuid(service)) == false) {
        return UndefinedServiceUuidError;
    }
    rez = servicesList.value(QBluetoothUuid(service))->characteristicRead(characteristic);
    if ( rez != BleCustomService::Ok) {
        qDebug()<<"BleCustomDevice: write characteristic error: "<<rez;
        return WriteCharacteristicError;
    }
    return Ok;
}

void BleCustomDevice::deviceConnected()
{
    qDebug()<<"BleCustomDevice: Device connected, start discover servises";
    servicesList.clear();
    bleDevice->discoverServices();
}

void BleCustomDevice::deviceDisconnected()
{
    qDebug()<<"BleCustomDevice: Device disconnected";
    /*
     * TODO Add signal to usr interface
     */
}

void BleCustomDevice::deviceServiceDiscovered(const QBluetoothUuid &newService)
{
    qDebug()<<"BleCustomDevice: discoverd service: "<<newService.toString();
    /*
     * Compare new found srvices with services from the target list
     */
    foreach(auto service, userServisesList) {
        if (memcmp(service.data, newService.toUInt128().data, sizeof(service.data)) == 0) {
            qDebug()<<"BleCustomDevice: target service found: "<<newService.toString();
            servicesList.insert(newService, nullptr);
        }
    }
}

void BleCustomDevice::deviceDiscoveryFinished()
{
    qDebug()<<"BleCustomDevice: Discovery servises complete, discovered services quantity: "<<servicesList.size();
    if (servicesList.size()
        != userServisesList.size()) {
        qDebug()<<"BleCustomDevice: Error quantity of servises, start disconnect device";
        bleDevice->disconnect();
        return;
    }
    qDebug()<<"BleCustomDevice: Start discover characteristics for the all target services";
    servicesCnt = 0;
    foreach(auto serviceUuid, servicesList.keys()) {
        servicesList[serviceUuid] = new BleCustomService(bleDevice, serviceUuid, this);
        connect(servicesList.value(serviceUuid), &BleCustomService::characteristicDiscoveryComplete,
                this, &BleCustomDevice::serviceDiscoveryComplete);
        connect(servicesList.value(serviceUuid), &BleCustomService::characteristicReceive,
                this, &BleCustomDevice::serviceCharacteristicReadRx);
        servicesList.value(serviceUuid)->startDiscoveryCharacteristics();
    }
}

void BleCustomDevice::serviceDiscoveryComplete(quint128 service, QVector<quint128> characteristicsList)
{
    qDebug()<<"BleCustomDevice: discovery for service complete: "<<QBluetoothUuid(service).toString();

    foreach(auto characteristic, characteristicsList) {
        qDebug()<<"    BleCustomDevice: characteristic: "<<QBluetoothUuid(characteristic).toString();
    }
    /*
     * To Do ass testing characteristics list
     */
    if (++servicesCnt == servicesList.size()) {
        qDebug()<<"BleCustomDevice: discovery for all services complete";
        emit connectionStateChange(userContext, Ok);
    }
}

void BleCustomDevice::serviceCharacteristicReadRx(quint128 service, quint128 characteristic,
                                                  const QByteArray &value)
{
    qDebug()<<"BleCustomDevice: rx data service: "<<QBluetoothUuid(service).toString();
    qDebug()<<"BleCustomDevice: rx data characteristic: "<<QBluetoothUuid(characteristic).toString();

    emit receiveData(userContext, service, characteristic, value);
}

/********Service generic slots******************/

void BleCustomDevice::deviceStateChanged(QLowEnergyController::ControllerState state)
{}

void BleCustomDevice::deviceError(QLowEnergyController::Error newError)
{}

