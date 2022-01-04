#include "blecustomservice.h"

#include "QUuid"
#include "QDebug"

BleCustomService::BleCustomService(QLowEnergyController *targetCotroller,
                                   QBluetoothUuid targetService,
                                   QObject *parent) : QObject(parent)
{
    qDebug()<<"BleCustomService: connect to servise";

    service = targetCotroller->createServiceObject(targetService);
    connect(service, &QLowEnergyService::stateChanged,
            this, &BleCustomService::detailsDiscovered);
    connect(service, &QLowEnergyService::characteristicRead,
            this, &BleCustomService::characteristicReadRx);
}

BleCustomService::Status BleCustomService::startDiscoveryCharacteristics(void)
{
    if (service == nullptr) {
        return ServiceCreateError;
    }

    service->discoverDetails();

    return Ok;
}

void BleCustomService::detailsDiscovered(QLowEnergyService::ServiceState newState)
{
    QVector<quint128> characteristicInfo;

    qDebug()<<"BleCustomService: Servis state Changed with status"<<newState;

    switch (newState) {
    case QLowEnergyService::InvalidService:
        qDebug()<<"BleCustomService: status error";
        return;

    case QLowEnergyService::DiscoveryRequired:  // we know start/end handle but nothing more
    case QLowEnergyService::DiscoveringServices:// discoverDetails() called and running
    case QLowEnergyService::LocalService:
        qDebug()<<"BleCustomService: unnown status";
        return;

    case QLowEnergyService::ServiceDiscovered:  // all details have been synchronized
        charsList.clear();
        qDebug()<<"BleCustomService: characteristics discovered complete";
        for (const QLowEnergyCharacteristic ch : service->characteristics()) {
            charsList.insert(ch.uuid(), ch);
            characteristicInfo.push_back(ch.uuid().toUInt128());
            qDebug()<<"BleCustomService: characteristic UUID"<<ch.uuid();
        }
        emit characteristicDiscoveryComplete(service->serviceUuid().toUInt128(), characteristicInfo);
        return;
    }
}

void BleCustomService::characteristicReadRx(const QLowEnergyCharacteristic &info,
                                            const QByteArray &value)
{
    qDebug()<<"BleCustomService: Read char size"<<value.size();

    emit characteristicReceive(service->serviceUuid().toUInt128(), info.uuid().toUInt128(), value);
}

BleCustomService::Status BleCustomService::characteristicRead(quint128 characteristic)
{
    if (charsList.contains(QBluetoothUuid(characteristic)) == false) {
        return UndefinedUuidError;
    }

    service->readCharacteristic(charsList.value(QBluetoothUuid(characteristic)));

    return Ok;
}

BleCustomService::Status BleCustomService::characteristicWrite(quint128 characteristic,
                                                              const QByteArray &value)
{
    if (value.size() == 0) {
        return DataSizeError;
    }

    if (charsList.contains(QBluetoothUuid(characteristic)) == false) {
        return UndefinedUuidError;
    }

    service->writeCharacteristic(charsList.value(QBluetoothUuid(characteristic)),
                                 value);
    return Ok;
}
