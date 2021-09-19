#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QtBluetooth/QBluetoothDeviceDiscoveryAgent"
#include "QtBluetooth/QLowEnergyController"
#include "QtBluetooth/QLowEnergyService"
#include "QtBluetooth/QLowEnergyCharacteristic"
#include "QtBluetooth/QBluetoothUuid"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addDevice(const QBluetoothDeviceInfo &device);
    /*Services discovered*/
    void servConnected();
    void servDisconnected();
    void servStateChanged(QLowEnergyController::ControllerState state);
    void servError(QLowEnergyController::Error newError);
    void servServiceDiscovered(const QBluetoothUuid &newService);
    void servDiscoveryFinished();


    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);
    void servCharRead(const QLowEnergyCharacteristic &info,
                      const QByteArray &value);

    void on_tableScanRez_cellClicked(int row, int column);
    void on_pushButton_clicked();

    void on_tableWidgetServises_cellClicked(int row, int column);

    void on_pushButtonReadChr_clicked();

    void on_pushButtonWriteChar_clicked();

private:
    Ui::MainWindow *ui;
    QBluetoothDeviceDiscoveryAgent *bleDescoverAgent;
    QLowEnergyController *controller;
    QVector<QBluetoothDeviceInfo> devicesArroundList;
    QVector<QBluetoothUuid> servisesList;
    QLowEnergyService *service;
    QList<QLowEnergyCharacteristic> charsList;

};
#endif // MAINWINDOW_H
