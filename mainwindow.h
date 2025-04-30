#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QListWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onScanButtonClicked();
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onDeviceConnected();
    void onDeviceDisconnected();
    void onDeviceListItemClicked(QListWidgetItem *item);

private:
    void setupUI();

    Ui::MainWindow *ui;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;
    QList<QBluetoothDeviceInfo> discoveredDevices;
    QTimer connectionTimer;
};

#endif // MAINWINDOW_H
