# BW-BO2 Monitor Qt Application

这是一个基于 Qt 5.14 的桌面应用程序，用于监控和显示蓝牙设备（如 BW-BO2）的生命体征数据。该应用程序支持蓝牙设备的扫描、连接以及数据展示。

## 功能
- 扫描附近的蓝牙设备。
- 连接到指定的蓝牙设备。
- 显示设备的生命体征数据（如 SpO₂、Pulse、Fatigue、Blood Pressure、Respiration）。
- 提供健康建议和警报信息。

## 技术栈
- **Qt 5.14**：用于构建跨平台的桌面应用程序。
- **C++**：主要编程语言。
- **Qt Widgets**：用于创建图形用户界面。
- **Qt Bluetooth**：用于蓝牙设备的扫描和连接。

## 项目结构
```
BWOBO2Monitor_qt/
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── resources.qrc
├── icons/
│   └── bluetooth.png
└── README.md
```

## 如何运行
### 前提条件
- 安装 Qt 5.14 或更高版本。
- 安装 MinGW 或其他支持的编译器。
- 确保您的系统支持蓝牙功能，并已启用蓝牙。

### 编译步骤
1. 打开 Qt Creator。
2. 选择 "File" -> "Open File or Project"，然后选择 `BWOBO2Monitor_qt.pro` 文件。
3. 确保选择正确的 Kit（例如 "Desktop Qt 5.14.2 MinGW 64-bit"）。
4. 点击 "Run" 或按 `Ctrl+R` 编译并运行项目。

### 手动编译
1. 打开终端或命令提示符。
2. 切换到项目目录：
   ```bash
   cd path/to/BWOBO2Monitor_qt
   ```
3. 运行 `qmake`：
   ```bash
   G:\Qt5.14.2\5.14.2\mingw73_64\bin\qmake.exe -o Makefile BWOBO2Monitor_qt.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
   ```
4. 使用 `make` 编译项目：
   ```bash
   G:\Qt5.14.2\Tools\mingw730_64\bin\mingw32-make.exe
   ```

## 使用说明
1. 启动应用程序后，点击 "扫描设备" 按钮开始扫描附近的蓝牙设备。
2. 在设备列表中选择要连接的设备，点击该设备项即可尝试连接。
3. 连接成功后，设备的生命体征数据将显示在核心数据仪表盘中。
4. 应用程序还会提供健康建议和警报信息。

## 注意事项
- 确保蓝牙设备已正确配对并可被发现。
- 如果连接失败，请检查蓝牙设备的 UUID 是否与代码中的 UUID 匹配。
- 该应用程序仅支持蓝牙 SPP（串行端口协议）连接。

## 资源文件
- `resources.qrc`：包含应用程序所需的图标和其他资源。
- `icons/bluetooth.png`：蓝牙图标，用于显示在顶部状态栏。



