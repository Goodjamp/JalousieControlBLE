#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QTableWidgetItem"
#include "QString"
#include "QVector"
#include "QDebug"

#include "QtBluetooth/QBluetoothDeviceDiscoveryAgent"
#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QUuid"

#include "blecustomdiscovery.h"
#include "blecustomdevice.h"
#include "blecustomservice.h"

#define PROTO_SERV_POS                       0
#define DEBUG_SERV_POS                       1

#define PROTO_SERV_LIGHT_CONTROL_CHAR_POS    0
#define PROTO_SERV_JAL_CONTROL_CHAR_POS      1
#define DEBUG_SERV_CHAR_POS                  2

const QVector<quint128> servicesList = {{0x00, 0x0f, 0x10, 0x00, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01},
                                        {0x01, 0x0f, 0x20, 0x00, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01}};
const QVector<quint128> characteristicsList = {
    {0x00, 0x0f, 0x10, 0x01, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01},
    {0x00, 0x0f, 0x10, 0x02, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01},
    {0x01, 0x0f, 0x20, 0x01, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01},
};

int temp = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&bleCustomDiscovery, &BleCustomDiscovery::bleCustomDiscoveryDiscoveredComplete, this, &MainWindow::bleDevicesDiscoverComplete);

    /*
     * Next codde used only for the test, remove
     */
    jalousieList.insert(temp, new JalousieItem(static_cast<void *>(&temp), this));
    static_cast<QHBoxLayout*>(ui->jalListLayout->layout())->insertWidget(0,
                                                                         jalousieList.value(temp));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bleDevicesDiscoverComplete(BleCustomDiscovery::Status status, QVector<QBluetoothDeviceInfo> devices)
{
    if (status != BleCustomDiscovery::Ok) {
        qDebug()<<"Main: descovery complete error"<<status;
    }

    qDebug()<<"Main: device"<<devices.size();

    if (devices.size() == 0) {
        return;
    }
    for (int k = 0; k < devices.size(); k++) {
        userContextList.push_back(k);
        bleDevicesList.push_back(new BleCustomDevice((void *)&userContextList.last(), devices[k], servicesList, this));
        connect(bleDevicesList.last(), &BleCustomDevice::receiveData, this, &MainWindow::deviceReceiveData);
        connect(bleDevicesList.last(), &BleCustomDevice::connectionStateChange, this, &MainWindow::deviceConnectionStateChange);
        bleDevicesList.last()->connectToDevice();
    }
}

void MainWindow::deviceReceiveData(void *uContext,
                                   quint128 service,
                                   quint128 characteristic,
                                   QByteArray data)
{
    int *inUserContext = (int *)uContext;

    if (*inUserContext > bleDevicesList.size()) {
        return;
    }
    (void)service;
    (void)characteristic;
    (void)data;
}

void MainWindow::deviceConnectionStateChange(void *uContext,
                                             BleCustomDevice::BleCustomDeviceStatus status)
{
    int *inUserContext = (int *)uContext;

    if (*inUserContext > bleDevicesList.size()) {
        return;
    }
    if (status == BleCustomDevice::Ok) {
        qDebug()<<"Main: BLE device connected "<< *inUserContext<<": "<<bleDevicesList[*inUserContext]->getBluetoothDeviceInfo()->address().toString();
    } else {
        qDebug()<<"Main: BLE device connected error"<<status;
    }
    jalousieList.insert(*inUserContext, new JalousieItem(uContext, this));
    static_cast<QHBoxLayout*>(ui->jalListLayout->layout())->insertWidget(0,
                                                                         jalousieList.value(*inUserContext));
    connect(jalousieList.value(*inUserContext), &JalousieItem::posChange, this, &MainWindow::jalItemPosChange);
    connect(jalousieList.value(*inUserContext), &JalousieItem::move, this, &MainWindow::jalItemMove);
    connect(jalousieList.value(*inUserContext), &JalousieItem::lightOn, this, &MainWindow::jalItemLightOn);
}

void MainWindow::jalItemPosChange(void *uContext, int value){
    if (uContext == nullptr) {
        qDebug()<<"Main: pos change slot error: nullptr";
        return;
    }
    qDebug()<<"Main: set pos value"<<value;

    int *inUserContext = (int *)uContext;

    if (*inUserContext > bleDevicesList.size()) {
        return;
    }

    char commandBuff[] = {1, static_cast<char>(0xFF & value)};
    QByteArray packet = {commandBuff, sizeof(commandBuff)};
    bleDevicesList[*inUserContext]->characteristicWrite(servicesList[PROTO_SERV_POS],
                                                        characteristicsList[PROTO_SERV_JAL_CONTROL_CHAR_POS],
                                                        packet);
}

void MainWindow::jalItemMove(void *uContext, bool left){
    if (uContext == nullptr) {
        qDebug()<<"Main: move slot error: nullptr";
        return;
    }
    qDebug()<<"Main: move value"<<left;
}

MainWindow::JcpState MainWindow::jcpSetLightState(uint8_t buff[], uint32_t *buffSize,
                                                  uint8_t colorRed, uint8_t colorGreen, uint8_t colorBlue,
                                                  bool on, bool blink)
{
    if (sizeof(JcpSetLightStateCommand) > *buffSize
        || buff == NULL) {
        return JcpBuffSizeError;
    }

    JcpSetLightStateCommand *command = (JcpSetLightStateCommand *)buff;

    *buffSize = sizeof(JcpSetLightStateCommand);
    command->colorRed = colorRed;
    command->colorGreen = colorGreen;
    command->colorBlue = colorBlue;
    command->header = 1;
    command->on = on;
    command->blink = blink;

    return JcpBuffSizeError;
}

void MainWindow::jalItemLightOn(void *uContext, bool lightOn, QRgb color){
    if (uContext == nullptr) {
        qDebug()<<"Main: light on slot error: nullptr";
        return;
    }
    qDebug()<<"Main: light on value"<<lightOn;
    qDebug()<<"Main: light on color"<<"R:"<<(0xFF & (color>>16))<<" G:"<<(0xFF & (color >> 8))<<" B:"<<(0xFF & color);

    int *inUserContext = (int *)uContext;

    (void)color;
    char commandBuff[64];
    uint32_t messageSize = sizeof(commandBuff);
    jcpSetLightState((uint8_t *)commandBuff, &messageSize,
                     qRed(color), qGreen(color), qBlue(color),
                     lightOn, 1);
    QByteArray packet = {commandBuff, static_cast<int>(messageSize)};
    bleDevicesList[*inUserContext]->characteristicWrite(servicesList[PROTO_SERV_POS],
                                                        characteristicsList[PROTO_SERV_JAL_CONTROL_CHAR_POS],
                                                        packet);
}


void MainWindow::on_AddDevicesButton_clicked()
{
    qDebug()<<"Main: -----Start scaning--------";
    bleDevicesList.clear();
    userContextList.clear();
    for(int k = 0; k < jalousieList.size(); k++) {
        // ToDo disconnect all connected devices
        //jalousieList.value(k)->disconnect();
    }
    jalousieList.clear();

    bleCustomDiscovery.startDiscovered(deviceMetaData, 5000);
}
