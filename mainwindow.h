#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QRgb>

#include "QtBluetooth/QBluetoothDeviceDiscoveryAgent"
#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QtBluetooth/QBluetoothUuid"

#include "blecustomdiscovery.h"
#include "blecustomdevice.h"
#include "blecustomservice.h"
#include "jalousieitem.h"
#include "customspiner.h"

#define TARGET_BLE_DEVICE_NAME    "JalousieControl"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*
     * Next code with Jcp (jalousie control protocol) descride the protocol
     * above the BLE and must be moved to the dedicated class
     */
    typedef enum {
        JcpOk,
        JcpBuffSizeError,
    } JcpState;

    #pragma pack(push, 1)
    typedef struct {
        uint8_t header;
        uint8_t on;
        uint8_t blink;
        uint8_t colorRed;
        uint8_t colorGreen;
        uint8_t colorBlue;
    } JcpSetLightStateCommand;
    #pragma pack(pop)

    JcpState jcpSetLightState(uint8_t buff[], uint32_t *buffSize,
                              uint8_t colorRed, uint8_t colorGreen, uint8_t colorBlue,
                              bool on, bool blink);

private slots:
    /*
     * JalousieItem class slots
     */
    void jalItemPosChange(void *uContext, int value);
    void jalItemMove(void *uContext, bool left);
    void jalItemLightOn(void *uContext, bool lightOn, QRgb color);

    /*
     * BleCustomDevice slots
     */
    void deviceReceiveData(void *uContext,
                           quint128 service,
                           quint128 characteristic,
                           QByteArray data);
    void deviceConnectionStateChange(void *uContext,
                                     BleCustomDevice::BleCustomDeviceStatus status);

    /*
     * BleCustomDiscovery slots
     */
    void bleDevicesDiscoverComplete(BleCustomDiscovery::Status status, QVector<QBluetoothDeviceInfo> devices);

    /*
     * UI slots
     */
    void on_AddDevicesButton_clicked();

private:
    const BleCustomDiscovery::TargtDeviceMetaData deviceMetaData = {TARGET_BLE_DEVICE_NAME};

    Ui::MainWindow *ui;
    BleCustomDiscovery bleCustomDiscovery;
    QVector<BleCustomDevice *> bleDevicesList;
    QVector<int> userContextList;
    QMap<int, JalousieItem *> jalousieList;
    CustomSpiner *customSpiner;
};
#endif // MAINWINDOW_H
