#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QTableWidgetItem"
#include "QtBluetooth/QBluetoothDeviceDiscoveryAgent"
#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QUuid"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bleDescoverAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(bleDescoverAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &MainWindow::addDevice);
    ui->tableScanRez->insertColumn(0);
    qDebug()<<"Table widjet geom"<<ui->tableScanRez->width();
    qDebug()<<"Table widjet size"<<ui->tableScanRez->width();
    ui->tableScanRez->horizontalHeader()->setMaximumSectionSize(ui->tableScanRez->width());
    ui->tableScanRez->horizontalHeader()->setMinimumSectionSize(ui->tableScanRez->width());
    ui->tableWidgetServises->insertColumn(0);
    ui->tableWidgetServises->horizontalHeader()->setMinimumSectionSize(400);
    ui->tableWidgetServises->horizontalHeader()->setMaximumSectionSize(400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addDevice(const QBluetoothDeviceInfo &device)
{

    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        ui->tableScanRez->insertRow(devicesArroundList.size());
        ui->tableScanRez->setItem(devicesArroundList.size(), 0, new QTableWidgetItem(device.name()));
        devicesArroundList.push_back(device);
        qDebug()<<"Device ser cnt"<<device.serviceUuids().size();
        foreach(auto item, device.serviceUuids())
        {
            if (item.minimumSize() == 2) {
                qDebug()<<QBluetoothUuid::serviceClassToString(static_cast<QBluetoothUuid::ServiceClassUuid>(item.toUInt16(NULL)));
            } else {
                qDebug()<<item.toString();
            }
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->tableScanRez->horizontalHeader()->setMaximumSectionSize(ui->tableScanRez->width());
    ui->tableScanRez->horizontalHeader()->setMinimumSectionSize(ui->tableScanRez->width());
    ui->tableWidgetServises->horizontalHeader()->setMaximumSectionSize(ui->tableWidgetServises->width());
    ui->tableWidgetServises->horizontalHeader()->setMinimumSectionSize(ui->tableWidgetServises->width());
    qDebug()<<"------------Start scaning---------------";
    bleDescoverAgent->setLowEnergyDiscoveryTimeout(10000);
    bleDescoverAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    devicesArroundList.clear();
}

void MainWindow::servConnected()
{
    qDebug()<<"servConnected ";
    controller->discoverServices();
}

void MainWindow::servDisconnected()
{
    qDebug()<<"servDisconnected ";
}

void MainWindow::servStateChanged(QLowEnergyController::ControllerState state)
{
   qDebug()<<"Sevices ";
}

void MainWindow::servError(QLowEnergyController::Error newError)
{

}

void MainWindow::servServiceDiscovered(const QBluetoothUuid &servDiscoveryFinished)
{
    ui->tableWidgetServises->insertRow(servisesList.size());
    if (servDiscoveryFinished.minimumSize() == 2) {
        ui->tableWidgetServises->setItem(servisesList.size(), 0,
                                         new QTableWidgetItem(QBluetoothUuid::serviceClassToString(static_cast<QBluetoothUuid::ServiceClassUuid>(servDiscoveryFinished.toUInt16(NULL)))));
        qDebug()<<QBluetoothUuid::serviceClassToString(static_cast<QBluetoothUuid::ServiceClassUuid>(servDiscoveryFinished.toUInt16(NULL)));
    } else {
        ui->tableWidgetServises->setItem(servisesList.size(), 0,
                                         new QTableWidgetItem(servDiscoveryFinished.toString()));
        qDebug()<<servDiscoveryFinished.toString();
    }

    servisesList.push_back(servDiscoveryFinished);
}

void MainWindow::servDiscoveryFinished()
{

}

void MainWindow::on_tableScanRez_cellClicked(int row, int column)
{
    qDebug()<<"Connect to device"<<devicesArroundList[row].name();
    controller = QLowEnergyController::createCentral(devicesArroundList[row], this);
    connect(controller, &QLowEnergyController::connected,
            this, &MainWindow::servConnected);
    connect(controller, &QLowEnergyController::disconnected,
            this, &MainWindow::servDisconnected);
    connect(controller, &QLowEnergyController::serviceDiscovered,
            this, &MainWindow::servServiceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished,
            this, &MainWindow::servDiscoveryFinished);

    controller->connectToDevice();
    servisesList.clear();
    ui->tableWidgetServises->setRowCount(0);
}

void MainWindow::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    qDebug()<<"Servis disc comp"<<newState;
    charsList = service->characteristics();
    qDebug()<<"Char cnt"<<charsList.size();
    for (const QLowEnergyCharacteristic &ch : charsList) {
        qDebug()<<"Char Name"<<ch.uuid();
        qDebug()<<"Char UUID"<<ch.name();
        qDebug()<<"Char UUID"<<ch.properties();
    }
}

void MainWindow::servCharRead(const QLowEnergyCharacteristic &info,
                              const QByteArray &value)
{
    qDebug()<<"Read char size"<<value.size();
    QString rezRead;
    foreach(int val, value) {
        rezRead += " " + QString::number(val);
    }
    rezRead += "\n";
    ui->textEditRxConsol->insertPlainText(rezRead);
}

void MainWindow::on_tableWidgetServises_cellClicked(int row, int column)
{
    qDebug()<<"Connect to servise";
    service = controller->createServiceObject(servisesList[row]);
    connect(service, &QLowEnergyService::stateChanged,
            this, &MainWindow::serviceDetailsDiscovered);
    connect(service, &QLowEnergyService::characteristicRead,
            this, &MainWindow::servCharRead);
    service->discoverDetails();
}

void MainWindow::on_pushButtonReadChr_clicked()
{
    service->readCharacteristic(charsList[0]);
}

void MainWindow::on_pushButtonWriteChar_clicked()
{
    static char val = 0;
    QByteArray data;
    data.push_back(val++);
    service->writeCharacteristic(charsList[0], data);
}
