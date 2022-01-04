#ifndef BLECUSTOMDEVICE_H
#define BLECUSTOMDEVICE_H

#include <QObject>

#include "QVector"
#include "QMap"
#include "QStringList"

#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QBluetoothUuid"
#include "QUuid"

#include "blecustomservice.h"

#include "qmetatype.h"

Q_DECLARE_METATYPE(quint128);

class BleCustomDevice: public QObject
{
    Q_OBJECT
public:

    typedef enum {
        Ok,
        ServceNotFound,
        CharacteristicNotFound,
        CantConnectTodeevice,
        UndefinedServiceUuidError,
        WriteCharacteristicError,
    } BleCustomDeviceStatus;

    BleCustomDevice(void *uContext,
                    QBluetoothDeviceInfo targetBleDevice,
                    QVector<quint128> targetServisesList,
                    QObject *parent);
    void connectToDevice();
    QBluetoothDeviceInfo *getBluetoothDeviceInfo();
    BleCustomDeviceStatus characteristicWrite(quint128 service, quint128 characteristic,
                                              QByteArray data);
    BleCustomDeviceStatus characteristicRead(quint128 service, quint128 characteristic);

signals:
    void receiveData(void *uContext,
                     quint128 service,
                     quint128 characteristic,
                     QByteArray data);
    void connectionStateChange(void *uContext,
                               BleCustomDeviceStatus status);

private:
    void *userContext;
    QBluetoothDeviceInfo bleDeviceInfo;
    QLowEnergyController *bleDevice;
    /*This is the list of servises UUID's that shoul be on the device*/
    QVector<quint128> userServisesList;
    /*The list of discovered services: service UUID - service handler*/
    QMap<QBluetoothUuid, BleCustomService *> servicesList;
    int servicesCnt;

private slots:

    /*
     * BLE device generic slots
     */
    void deviceConnected();
    void deviceDisconnected();
    void deviceStateChanged(QLowEnergyController::ControllerState state);
    void deviceError(QLowEnergyController::Error newError);
    void deviceServiceDiscovered(const QBluetoothUuid &newService);
    void deviceDiscoveryFinished();

    /*
     * BleServices processing slots
     */
    void serviceDiscoveryComplete(quint128 service, QVector<quint128> characteristicsList);
    void serviceCharacteristicReadRx(quint128 service, quint128 characteristic,
                                      const QByteArray &value);
};

#endif // BLECUSTOMDEVICE_H
