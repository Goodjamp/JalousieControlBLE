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

QVector<quint128> servicesList = {{0x00, 0x0f, 0x10, 0x00, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01},
                                 {0x01, 0x0f, 0x20, 0x00, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01}};

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
}

void MainWindow::jalItemMove(void *uContext, bool left){
    if (uContext == nullptr) {
        qDebug()<<"Main: move slot error: nullptr";
        return;
    }
    qDebug()<<"Main: move value"<<left;
}

void MainWindow::jalItemLightOn(void *uContext, bool lightOn, QColor color){
    if (uContext == nullptr) {
        qDebug()<<"Main: light on slot error: nullptr";
        return;
    }
    qDebug()<<"Main: light on value"<<lightOn;
    (void)color;
}


void MainWindow::on_AddDevicesButton_clicked()
{
    qDebug()<<"Main: -----Start scaning--------";
    bleDevicesList.clear();
    userContextList.clear();
    for(uint32_t k = 0; k < jalousieList.size(); k++) {
        // ToDo disconnect all connected devices
        //jalousieList.value(k)->disconnect();
    }
    jalousieList.clear();

    bleCustomDiscovery.startDiscovered(deviceMetaData, 5000);
}
