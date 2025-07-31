#include "../include/mainwindow.h"
#include "cellninjia_core.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_tabWidget(nullptr)
    , m_isConnected(false)
    , m_isMonitoring(false)
    , m_updateTimer(new QTimer(this))
{
    setupUI();
    setupMenuBar();
    loadMessageTypes();
    updateButtonStates();
    
    // Setup update timer
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateStatistics);
    m_updateTimer->setInterval(1000); // Update every second
    
    setWindowTitle("CellNinjia Desktop - Next-Generation Network Monitor");
    setMinimumSize(1200, 800);
    resize(1400, 900);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    auto *mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Create tab widget
    m_tabWidget = new QTabWidget(this);
    mainLayout->addWidget(m_tabWidget);
    
    setupConnectionPanel();
    setupMessageConfigPanel();
    setupMonitoringPanel();
    setupStatusPanel();
}

void MainWindow::setupConnectionPanel()
{
    m_connectionTab = new QWidget();
    m_tabWidget->addTab(m_connectionTab, "🔌 Connection");
    
    auto *layout = new QVBoxLayout(m_connectionTab);
    
    // Connection Settings Group
    auto *connectionGroup = new QGroupBox("Device Connection", this);
    auto *connectionLayout = new QGridLayout(connectionGroup);
    
    // Serial Port Selection
    connectionLayout->addWidget(new QLabel("Serial Port:"), 0, 0);
    m_portCombo = new QComboBox(this);
    m_portCombo->setEditable(true);
    connectionLayout->addWidget(m_portCombo, 0, 1);
    
    // Refresh ports button
    auto *refreshButton = new QPushButton("🔄 Refresh", this);
    connectionLayout->addWidget(refreshButton, 0, 2);
    connect(refreshButton, &QPushButton::clicked, [this]() {
        m_portCombo->clear();
        const auto ports = QSerialPortInfo::availablePorts();
        for (const auto &port : ports) {
            m_portCombo->addItem(port.portName() + " - " + port.description(), port.portName());
        }
        if (m_portCombo->count() == 0) {
            m_portCombo->addItem("/dev/ttyUSB0");
        }
    });
    
    // Baud Rate Selection
    connectionLayout->addWidget(new QLabel("Baud Rate:"), 1, 0);
    m_baudCombo = new QComboBox(this);
    m_baudCombo->addItems({"9600", "19200", "38400", "57600", "115200"});
    m_baudCombo->setCurrentText("9600");
    connectionLayout->addWidget(m_baudCombo, 1, 1);
    
    // Connection Status
    connectionLayout->addWidget(new QLabel("Status:"), 2, 0);
    m_connectionStatus = new QLabel("Disconnected", this);
    m_connectionStatus->setStyleSheet("color: red; font-weight: bold;");
    connectionLayout->addWidget(m_connectionStatus, 2, 1);
    
    // Connection Progress
    m_connectionProgress = new QProgressBar(this);
    m_connectionProgress->setVisible(false);
    connectionLayout->addWidget(m_connectionProgress, 3, 0, 1, 3);
    
    layout->addWidget(connectionGroup);
    
    // Connection Buttons
    auto *buttonLayout = new QHBoxLayout();
    m_connectButton = new QPushButton("🔗 Connect", this);
    m_disconnectButton = new QPushButton("❌ Disconnect", this);
    
    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::connectToDevice);
    connect(m_disconnectButton, &QPushButton::clicked, this, &MainWindow::disconnectFromDevice);
    
    buttonLayout->addWidget(m_connectButton);
    buttonLayout->addWidget(m_disconnectButton);
    buttonLayout->addStretch();
    
    layout->addLayout(buttonLayout);
    layout->addStretch();
    
    // Initialize with available ports
    refreshButton->click();
}

void MainWindow::setupMessageConfigPanel()
{
    m_configTab = new QWidget();
    m_tabWidget->addTab(m_configTab, "⚙️ Configuration");
    
    auto *layout = new QVBoxLayout(m_configTab);
    
    // Mode Selection Group
    auto *modeGroup = new QGroupBox("Operating Mode", this);
    auto *modeLayout = new QGridLayout(modeGroup);
    
    modeLayout->addWidget(new QLabel("Mode:"), 0, 0);
    m_modeCombo = new QComboBox(this);
    m_modeCombo->addItem("🔴 Online (Real-time decoding)", "online");
    m_modeCombo->addItem("💾 Offline (Raw data collection)", "offline");
    modeLayout->addWidget(m_modeCombo, 0, 1);
    
    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onModeChanged);
    
    layout->addWidget(modeGroup);
    
    // Output File Group
    auto *fileGroup = new QGroupBox("Output File", this);
    auto *fileLayout = new QHBoxLayout(fileGroup);
    
    m_outputFileEdit = new QLineEdit(this);
    m_outputFileEdit->setPlaceholderText("Select output file...");
    m_browseButton = new QPushButton("📁 Browse", this);
    
    connect(m_browseButton, &QPushButton::clicked, this, &MainWindow::selectOutputFile);
    
    fileLayout->addWidget(m_outputFileEdit);
    fileLayout->addWidget(m_browseButton);
    
    layout->addWidget(fileGroup);
    
    // Message Types Group
    auto *messageGroup = new QGroupBox("Message Types", this);
    auto *messageLayout = new QVBoxLayout(messageGroup);
    
    auto *messageHeader = new QLabel("Select message types to monitor:");
    messageHeader->setStyleSheet("font-weight: bold;");
    messageLayout->addWidget(messageHeader);
    
    m_messageTypeList = new QListWidget(this);
    m_messageTypeList->setSelectionMode(QAbstractItemView::MultiSelection);
    messageLayout->addWidget(m_messageTypeList);
    
    // Quick selection buttons
    auto *quickSelectLayout = new QHBoxLayout();
    auto *selectAllBtn = new QPushButton("✅ Select All", this);
    auto *selectNoneBtn = new QPushButton("❌ Select None", this);
    auto *selectDefaultBtn = new QPushButton("🎯 Default Set", this);
    
    connect(selectAllBtn, &QPushButton::clicked, [this]() {
        for (int i = 0; i < m_messageTypeList->count(); ++i) {
            m_messageTypeList->item(i)->setSelected(true);
        }
    });
    
    connect(selectNoneBtn, &QPushButton::clicked, [this]() {
        m_messageTypeList->clearSelection();
    });
    
    connect(selectDefaultBtn, &QPushButton::clicked, [this]() {
        m_messageTypeList->clearSelection();
        // Select default message types
        const QStringList defaultTypes = {"NR5G L2 UL TB", "NR5G L2 UL BSR", "NR5G MAC DCI Info"};
        for (int i = 0; i < m_messageTypeList->count(); ++i) {
            auto *item = m_messageTypeList->item(i);
            if (defaultTypes.contains(item->text())) {
                item->setSelected(true);
            }
        }
    });
    
    quickSelectLayout->addWidget(selectAllBtn);
    quickSelectLayout->addWidget(selectNoneBtn);
    quickSelectLayout->addWidget(selectDefaultBtn);
    quickSelectLayout->addStretch();
    
    messageLayout->addLayout(quickSelectLayout);
    layout->addWidget(messageGroup);
    
    // Control Buttons
    auto *controlLayout = new QHBoxLayout();
    m_startButton = new QPushButton("🚀 Start Monitoring", this);
    m_stopButton = new QPushButton("⏹️ Stop Monitoring", this);
    
    m_startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    m_stopButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; }");
    
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::startMonitoring);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::stopMonitoring);
    
    controlLayout->addWidget(m_startButton);
    controlLayout->addWidget(m_stopButton);
    controlLayout->addStretch();
    
    layout->addLayout(controlLayout);
}

void MainWindow::setupMonitoringPanel()
{
    m_monitorTab = new QWidget();
    m_tabWidget->addTab(m_monitorTab, "📊 Monitoring");
    
    auto *layout = new QVBoxLayout(m_monitorTab);
    
    // Statistics Group
    auto *statsGroup = new QGroupBox("Real-time Statistics", this);
    auto *statsLayout = new QGridLayout(statsGroup);
    
    m_messagesReceived = new QLabel("0", this);
    m_dataTransferred = new QLabel("0 KB", this);
    m_monitoringTime = new QLabel("00:00:00", this);
    
    m_messagesReceived->setStyleSheet("font-size: 18px; font-weight: bold; color: #2196F3;");
    m_dataTransferred->setStyleSheet("font-size: 18px; font-weight: bold; color: #FF9800;");
    m_monitoringTime->setStyleSheet("font-size: 18px; font-weight: bold; color: #4CAF50;");
    
    statsLayout->addWidget(new QLabel("Messages Received:"), 0, 0);
    statsLayout->addWidget(m_messagesReceived, 0, 1);
    statsLayout->addWidget(new QLabel("Data Transferred:"), 0, 2);
    statsLayout->addWidget(m_dataTransferred, 0, 3);
    statsLayout->addWidget(new QLabel("Monitoring Time:"), 0, 4);
    statsLayout->addWidget(m_monitoringTime, 0, 5);
    
    layout->addWidget(statsGroup);
    
    // Main content splitter
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Log Display
    auto *logGroup = new QGroupBox("Live Data Log", this);
    auto *logLayout = new QVBoxLayout(logGroup);
    
    m_logDisplay = new QTextEdit(this);
    m_logDisplay->setReadOnly(true);
    m_logDisplay->setFont(QFont("Courier", 9));
    logLayout->addWidget(m_logDisplay);
    
    // Statistics Table
    auto *tableGroup = new QGroupBox("Message Statistics", this);
    auto *tableLayout = new QVBoxLayout(tableGroup);
    
    m_statisticsTable = new QTableWidget(this);
    m_statisticsTable->setColumnCount(3);
    m_statisticsTable->setHorizontalHeaderLabels({"Message Type", "Count", "Last Received"});
    m_statisticsTable->horizontalHeader()->setStretchLastSection(true);
    tableLayout->addWidget(m_statisticsTable);
    
    splitter->addWidget(logGroup);
    splitter->addWidget(tableGroup);
    splitter->setSizes({600, 400});
    
    layout->addWidget(splitter);
}

void MainWindow::setupStatusPanel()
{
    m_statusBar = statusBar();
    m_statusBar->showMessage("Ready");
}

void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();
    
    // File Menu
    auto *fileMenu = m_menuBar->addMenu("&File");
    
    auto *openAction = fileMenu->addAction("&Open Configuration...");
    auto *saveAction = fileMenu->addAction("&Save Configuration...");
    fileMenu->addSeparator();
    auto *exitAction = fileMenu->addAction("E&xit");
    
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Tools Menu
    auto *toolsMenu = m_menuBar->addMenu("&Tools");
    auto *decoderAction = toolsMenu->addAction("&Offline Decoder...");
    auto *aboutAction = toolsMenu->addAction("&About CellNinjia");
    
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About CellNinjia", 
            "CellNinjia Desktop v1.0\n\n"
            "A comprehensive next-generation network monitoring tool\n"
            "for 5G/LTE cellular network research and analysis.\n\n"
            "Monitors both RAN and mobile core network communications,\n"
            "decoding messages exchanged between UE and network infrastructure.");
    });
}

void MainWindow::loadMessageTypes()
{
    // Load all 42 supported message types
    const QStringList messageTypes = {
        "NR5G NAS SM5G Plain OTA Outgoing Msg",
        "NR5G NAS MM5G State",
        "NR5G NAS Plain Message Container",
        "NR5G NAS MM5G NSSAI Info",
        "NR5G RRC MIB Info",
        "NR5G RRC Serving Cell Info",
        "NR5G RRC Configuration Info",
        "NR5G RRC Supported CA Combos",
        "NR5G PDCP DL Data Pdu",
        "NR5G PDCP DL Rbs Stats",
        "NR5G L2 DL Config",
        "NR5G RLC DL Stats",
        "NR5G RLC DL Status PDU",
        "NR5G L2 DL DATA PDU",
        "NR5G PDCP UL Stats",
        "NR5G PDCP UL Control Pdu",
        "NR5G RLC UL Stats",
        "NR5G L2 UL Config",
        "NR5G L2 UL TB",
        "NR5G L2 UL BSR",
        "NR5G MAC UL TB Stats",
        "NR5G MAC DCI Info",
        "NR5G MAC DL TB Report",
        "NR5G MAC PDSCH Status",
        "NR5G MAC PDSCH Stats",
        "NR5G MAC RACH Trigger",
        "NR5G MAC CDRX Events Info",
        "NR5G MAC UCI Information",
        "NR5G MAC Flow Control",
        "NR5G MAC CSF Report",
        "NR5G ML1 Searcher Measurement Database Update Ext",
        "NR5G RRC OTA Packet",
        "NR5G MAC RACH Attempt",
        "NR5G RLC UL Status PDU",
        "NR5G ML1 Searcher Conn Eval",
        "NR5G ML1 Antenna Switch Diversity",
        "NR5G LL1 FW Serving FTL",
        "NR5G ML1 Search HO Acq Confirm",
        "NR5G L2 UL Data PDU",
        "NR5G LL1 FW RX Control AGC",
        "NR5G MAC UL Physical Channel Schedule Report"
    };
    
    for (const QString &messageType : messageTypes) {
        m_messageTypeList->addItem(messageType);
    }
}

void MainWindow::connectToDevice()
{
    // Implementation will integrate with existing CellNinjia core
    m_connectionStatus->setText("Connecting...");
    m_connectionStatus->setStyleSheet("color: orange; font-weight: bold;");
    m_connectionProgress->setVisible(true);
    m_connectionProgress->setRange(0, 0); // Indeterminate progress
    
    // Simulate connection process
    QTimer::singleShot(2000, this, [this]() {
        m_isConnected = true;
        m_connectionStatus->setText("Connected");
        m_connectionStatus->setStyleSheet("color: green; font-weight: bold;");
        m_connectionProgress->setVisible(false);
        updateButtonStates();
        m_statusBar->showMessage("Connected to " + m_portCombo->currentText());
    });
}

void MainWindow::disconnectFromDevice()
{
    m_isConnected = false;
    m_isMonitoring = false;
    m_connectionStatus->setText("Disconnected");
    m_connectionStatus->setStyleSheet("color: red; font-weight: bold;");
    updateButtonStates();
    m_updateTimer->stop();
    m_statusBar->showMessage("Disconnected");
}

void MainWindow::startMonitoring()
{
    if (!m_isConnected) {
        QMessageBox::warning(this, "Warning", "Please connect to a device first.");
        return;
    }
    
    if (m_outputFileEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select an output file.");
        return;
    }
    
    m_isMonitoring = true;
    updateButtonStates();
    m_updateTimer->start();
    m_tabWidget->setCurrentIndex(2); // Switch to monitoring tab
    m_statusBar->showMessage("Monitoring started - " + m_modeCombo->currentText());
}

void MainWindow::stopMonitoring()
{
    m_isMonitoring = false;
    updateButtonStates();
    m_updateTimer->stop();
    m_statusBar->showMessage("Monitoring stopped");
}

void MainWindow::selectOutputFile()
{
    QString filter = m_modeCombo->currentData().toString() == "online" ? 
                    "Text files (*.txt);;All files (*)" : 
                    "Log files (*.log);;All files (*)";
    
    QString fileName = QFileDialog::getSaveFileName(this, "Select Output File", 
                                                   QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                   filter);
    if (!fileName.isEmpty()) {
        m_outputFileEdit->setText(fileName);
        m_currentOutputFile = fileName;
    }
}

void MainWindow::onModeChanged()
{
    QString mode = m_modeCombo->currentData().toString();
    if (mode == "online") {
        m_outputFileEdit->setPlaceholderText("Select output file for decoded data (*.txt)");
    } else {
        m_outputFileEdit->setPlaceholderText("Select output file for raw binary data (*.log)");
    }
    m_outputFileEdit->clear();
}

void MainWindow::onMessageTypeChanged()
{
    // Handle message type selection changes
}

void MainWindow::updateConnectionStatus()
{
    // Update connection status
}

void MainWindow::displayReceivedData(const QString &data)
{
    m_logDisplay->append(data);
    // Keep log size manageable
    if (m_logDisplay->document()->blockCount() > 1000) {
        auto cursor = m_logDisplay->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 100);
        cursor.removeSelectedText();
    }
}

void MainWindow::updateStatistics()
{
    if (!m_isMonitoring) return;
    
    // Update statistics (placeholder implementation)
    static int messageCount = 0;
    static qint64 startTime = QDateTime::currentMSecsSinceEpoch();
    
    messageCount += qrand() % 10;
    qint64 elapsed = QDateTime::currentMSecsSinceEpoch() - startTime;
    
    m_messagesReceived->setText(QString::number(messageCount));
    m_dataTransferred->setText(QString::number(messageCount * 64 / 1024.0, 'f', 1) + " KB");
    
    int seconds = elapsed / 1000;
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    
    m_monitoringTime->setText(QString("%1:%2:%3")
                             .arg(hours, 2, 10, QChar('0'))
                             .arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0')));
}

void MainWindow::updateButtonStates()
{
    m_connectButton->setEnabled(!m_isConnected);
    m_disconnectButton->setEnabled(m_isConnected);
    m_startButton->setEnabled(m_isConnected && !m_isMonitoring);
    m_stopButton->setEnabled(m_isMonitoring);
}