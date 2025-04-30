#include "mainwindow.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPixmap>

// 模拟 UI 类
namespace Ui {
class MainWindow {
public:
    QLabel *bluetoothStatus;
    QLabel *deviceName;
    QLabel *deviceStatus;
    QPushButton *btnScan;
    QListWidget *deviceList;
    QLabel *spo2Value;
    QLabel *pulseValue;
    QLabel *fatigueValue;
    QLabel *bloodPressureValue;
    QLabel *respirationValue;
    QLabel *alertText;
    QLabel *healthAdvice;

    void setupUi(QMainWindow *window) {
        QWidget *centralWidget = new QWidget(window);

        // 顶部状态栏
        QHBoxLayout *statusLayout = new QHBoxLayout();
        bluetoothStatus = new QLabel();
        bluetoothStatus->setPixmap(QPixmap(":/icons/bluetooth.png").scaled(50, 50));
        deviceName = new QLabel("BW-BO2 Monitor");
        deviceStatus = new QLabel("未连接");
        btnScan = new QPushButton("扫描设备");

        statusLayout->addWidget(bluetoothStatus);
        statusLayout->addWidget(deviceName);
        statusLayout->addWidget(deviceStatus);
        statusLayout->addWidget(btnScan);

        // 核心数据仪表盘
        QVBoxLayout *dashboardLayout = new QVBoxLayout();
        spo2Value = new QLabel("--%");
        pulseValue = new QLabel("--bpm");
        fatigueValue = new QLabel("--%");
        bloodPressureValue = new QLabel("--/-- mmHg");
        respirationValue = new QLabel("--/min");

        dashboardLayout->addWidget(new QLabel("SpO₂:"));
        dashboardLayout->addWidget(spo2Value);
        dashboardLayout->addWidget(new QLabel("Pulse:"));
        dashboardLayout->addWidget(pulseValue);
        dashboardLayout->addWidget(new QLabel("Fatigue:"));
        dashboardLayout->addWidget(fatigueValue);
        dashboardLayout->addWidget(new QLabel("Blood Pressure:"));
        dashboardLayout->addWidget(bloodPressureValue);
        dashboardLayout->addWidget(new QLabel("Respiration:"));
        dashboardLayout->addWidget(respirationValue);

        // 设备列表
        deviceList = new QListWidget();

        // 健康建议卡片
        QVBoxLayout *adviceLayout = new QVBoxLayout();
        QLabel *adviceTitle = new QLabel("Health Advice");
        healthAdvice = new QLabel("Connecting to device...");
        adviceLayout->addWidget(adviceTitle);
        adviceLayout->addWidget(healthAdvice);

        // 警报卡片
        QVBoxLayout *alertLayout = new QVBoxLayout();
        QLabel *alertTitle = new QLabel("Alert");
        alertText = new QLabel("Connecting to device...");
        alertLayout->addWidget(alertTitle);
        alertLayout->addWidget(alertText);

        // 主布局
        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->addLayout(statusLayout);
        mainLayout->addLayout(dashboardLayout);
        mainLayout->addWidget(deviceList);
        mainLayout->addLayout(adviceLayout);
        mainLayout->addLayout(alertLayout);

        window->setCentralWidget(centralWidget);
    }
};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化蓝牙设备发现代理
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &MainWindow::onDeviceDiscovered);

    // 初始化蓝牙套接字
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(socket, &QBluetoothSocket::connected, this, &MainWindow::onDeviceConnected);
    connect(socket, &QBluetoothSocket::disconnected, this, &MainWindow::onDeviceDisconnected);

    // 连接列表项点击事件
    connect(ui->deviceList, &QListWidget::itemClicked, this, &MainWindow::onDeviceListItemClicked);

    // 初始化连接超时计时器
    connectionTimer.setSingleShot(true);
    connect(&connectionTimer, &QTimer::timeout, this, [this]() {
        QMessageBox::warning(this, "Connection Timeout", "Failed to connect to device.");
    });

    // 连接扫描按钮点击事件
    connect(ui->btnScan, &QPushButton::clicked, this, &MainWindow::onScanButtonClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onScanButtonClicked() {
    discoveredDevices.clear();
    ui->deviceList->clear();
    discoveryAgent->start();
}

void MainWindow::onDeviceDiscovered(const QBluetoothDeviceInfo &device) {
    discoveredDevices.append(device);
    ui->deviceList->addItem(device.name() + " (" + device.address().toString() + ")");
}

void MainWindow::onDeviceConnected() {
    QMessageBox::information(this, "Connected", "Device connected successfully!");
    connectionTimer.stop();
}

void MainWindow::onDeviceDisconnected() {
    QMessageBox::information(this, "Disconnected", "Device disconnected!");
}

void MainWindow::onDeviceListItemClicked(QListWidgetItem *item) {
    if (!item) return;

    QString deviceInfo = item->text();
    QString address = deviceInfo.split('(').last().remove(')');

    QBluetoothDeviceInfo device;
    for (const auto &info : discoveredDevices) {
        if (info.address().toString() == address) {
            device = info;
            break;
        }
    }

    // 替换 isNull() 检查
    if (device.address().isNull()) {
        QMessageBox::warning(this, "Invalid Device", "Selected device is not valid.");
        return;
    }

    socket->connectToService(device.address(), QBluetoothUuid(QBluetoothUuid::SerialPort));
    connectionTimer.start(10000); // 10秒超时
}
