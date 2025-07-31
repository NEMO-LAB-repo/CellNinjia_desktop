# CellNinjia GUI API 参考文档

## 📋 API概览

本文档详细描述了CellNinjia GUI各个组件的公共接口，为开发者提供完整的API参考。

## 🏠 MainWindow API

### 类定义
```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    // 公共接口
    bool isConnected() const;
    bool isMonitoring() const;
    QString getCurrentPort() const;
    qint32 getCurrentBaudRate() const;
    
public slots:
    void connectToDevice();
    void disconnectFromDevice();
    void startMonitoring();
    void stopMonitoring();
    void selectOutputFile();
    void loadConfiguration(const QString &filePath);
    void saveConfiguration(const QString &filePath);
    
signals:
    void connectionStateChanged(bool connected);
    void monitoringStateChanged(bool monitoring);
    void deviceConnected(const QString &portName);
    void deviceDisconnected();
    void monitoringStarted();
    void monitoringStopped();
    void errorOccurred(const QString &error);
};
```

### 使用示例
```cpp
// 创建主窗口
MainWindow *window = new MainWindow();

// 连接信号
connect(window, &MainWindow::connectionStateChanged,
        [](bool connected) {
            qDebug() << "Connection state:" << connected;
        });

// 程序化控制
window->connectToDevice();  // 连接设备
window->startMonitoring();  // 开始监控
```

## 📊 ChartWidget API

### 类定义
```cpp
class ChartWidget : public QWidget
{
    Q_OBJECT
    
public:
    enum ChartType {
        MessageRate,
        DataThroughput,
        MessageTypes,
        SignalStrength,
        LayerDistribution
    };
    
    explicit ChartWidget(ChartType type, QWidget *parent = nullptr);
    ~ChartWidget();
    
    // 数据操作
    void addDataPoint(qreal value, const QDateTime &timestamp = QDateTime::currentDateTime());
    void addMessageType(const QString &messageType, int count);
    void setSignalStrength(qreal strength, const QDateTime &timestamp = QDateTime::currentDateTime());
    void updateLayerDistribution(const QMap<QString, int> &layerCounts);
    void clearData();
    
    // 配置
    void setMaxDataPoints(int maxPoints);
    void setUpdateInterval(int milliseconds);
    void setTheme(const QString &theme);
    
    // 导出
    bool exportChart(const QString &filePath, const QString &format = "PNG");
    QPixmap getChartPixmap() const;
    
public slots:
    void updateChart();
    void exportChart();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    
signals:
    void chartUpdated();
    void dataPointAdded(qreal value, const QDateTime &timestamp);
    void exportRequested();
};
```

### 使用示例
```cpp
// 创建消息速率图表
ChartWidget *rateChart = new ChartWidget(ChartWidget::MessageRate);
rateChart->setMaxDataPoints(1000);
rateChart->setUpdateInterval(1000);

// 添加数据点
rateChart->addDataPoint(45.2);  // 45.2 messages/second

// 导出图表
rateChart->exportChart("/path/to/chart.png", "PNG");
```

## 🔧 CellNinjiaCore API

### 类定义
```cpp
class CellNinjiaCore : public QObject
{
    Q_OBJECT
    
public:
    enum class OperatingMode {
        Online,   // 实时解码
        Offline   // 原始数据收集
    };
    
    explicit CellNinjiaCore(QObject *parent = nullptr);
    ~CellNinjiaCore();
    
    // 连接管理
    bool connectToDevice(const QString &portName, qint32 baudRate);
    void disconnectFromDevice();
    bool isConnected() const;
    
    // 配置
    void setOperatingMode(OperatingMode mode);
    void setOutputFile(const QString &filePath);
    void setMessageTypes(const QStringList &messageTypes);
    
    // 监控控制
    bool startMonitoring();
    void stopMonitoring();
    bool isMonitoring() const;
    
    // 统计信息
    qint64 getMessagesReceived() const;
    qint64 getBytesTransferred() const;
    qint64 getMonitoringDuration() const;
    qreal getMessageRate() const;
    qreal getDataThroughput() const;
    
signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void dataReceived(const QByteArray &data);
    void messageDecoded(const QString &messageType, const QString &decodedData);
    void statisticsUpdated();
    void monitoringStarted();
    void monitoringStopped();
};
```

### 使用示例
```cpp
// 创建核心对象
CellNinjiaCore *core = new CellNinjiaCore();

// 配置
core->setOperatingMode(CellNinjiaCore::OperatingMode::Online);
core->setMessageTypes({"NR5G L2 UL TB", "NR5G MAC DCI Info"});
core->setOutputFile("/path/to/output.txt");

// 连接和监控
if (core->connectToDevice("/dev/ttyUSB0", 9600)) {
    core->startMonitoring();
}

// 处理解码的消息
connect(core, &CellNinjiaCore::messageDecoded,
        [](const QString &type, const QString &data) {
            qDebug() << "Decoded message:" << type << data;
        });
```

## ⚙️ ConfigManager API

### 数据结构
```cpp
struct ConnectionConfig {
    QString portName;
    qint32 baudRate;
    bool autoConnect;
};

struct MonitoringConfig {
    QString operatingMode;
    QString outputFile;
    QStringList selectedMessageTypes;
    int maxDataPoints;
    bool autoSave;
    int autoSaveInterval;
};

struct CellNinjiaConfig {
    ConnectionConfig connection;
    MonitoringConfig monitoring;
    DisplayConfig display;
    ExportConfig exportSettings;
    QString configName;
    QString description;
    QDateTime created;
    QDateTime lastModified;
};
```

### 类定义
```cpp
class ConfigManager : public QObject
{
    Q_OBJECT
    
public:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();
    
    // 配置文件操作
    bool saveConfiguration(const CellNinjiaConfig &config, const QString &filePath = QString());
    bool loadConfiguration(CellNinjiaConfig &config, const QString &filePath);
    bool deleteConfiguration(const QString &filePath);
    
    // 预设配置
    CellNinjiaConfig getDefaultConfiguration() const;
    CellNinjiaConfig get5GResearchConfiguration() const;
    CellNinjiaConfig getLTEMonitoringConfiguration() const;
    CellNinjiaConfig getDebugConfiguration() const;
    
    // 最近配置
    QStringList getRecentConfigurations() const;
    void addRecentConfiguration(const QString &filePath);
    
    // 设置持久化
    void saveSettings(const CellNinjiaConfig &config);
    CellNinjiaConfig loadSettings();
    
    // 验证
    bool validateConfiguration(const CellNinjiaConfig &config, QString &errorMessage);
    
    // 导入导出
    bool exportToJson(const CellNinjiaConfig &config, const QString &filePath);
    bool importFromJson(CellNinjiaConfig &config, const QString &filePath);
    
signals:
    void configurationChanged(const CellNinjiaConfig &config);
    void recentConfigurationsUpdated();
    void configurationSaved(const QString &filePath);
    void configurationLoaded(const QString &filePath);
};
```

### 使用示例
```cpp
// 创建配置管理器
ConfigManager *configMgr = new ConfigManager();

// 创建配置
ConfigManager::CellNinjiaConfig config = configMgr->getDefaultConfiguration();
config.connection.portName = "/dev/ttyUSB0";
config.connection.baudRate = 9600;
config.monitoring.selectedMessageTypes = {"NR5G L2 UL TB", "NR5G L2 UL BSR"};

// 保存配置
configMgr->saveConfiguration(config, "my_config.json");

// 加载配置
ConfigManager::CellNinjiaConfig loadedConfig;
if (configMgr->loadConfiguration(loadedConfig, "my_config.json")) {
    // 使用加载的配置
}
```

## 🔍 AdvancedFilterDialog API

### 数据结构
```cpp
struct FilterCriteria {
    // 消息类型过滤
    QStringList includeMessageTypes;
    QStringList excludeMessageTypes;
    
    // 内容过滤
    QString contentPattern;
    bool useRegex;
    bool caseSensitive;
    
    // 时间过滤
    QDateTime startTime;
    QDateTime endTime;
    bool useTimeFilter;
    
    // 大小过滤
    int minMessageSize;
    int maxMessageSize;
    bool useSizeFilter;
    
    // 协议层过滤
    QStringList protocolLayers;
    
    // 高级过滤
    QString customFilter;
    bool enableCustomFilter;
    
    // 显示选项
    bool highlightMatches;
    QString highlightColor;
    bool showOnlyMatches;
    int maxResults;
};
```

### 类定义
```cpp
class AdvancedFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdvancedFilterDialog(QWidget *parent = nullptr);
    ~AdvancedFilterDialog();
    
    // 过滤条件
    FilterCriteria getFilterCriteria() const;
    void setFilterCriteria(const FilterCriteria &criteria);
    
    // 快速过滤
    static FilterCriteria getQuickFilter(const QString &filterType);
    
    // 预设管理
    void savePreset(const QString &name, const FilterCriteria &criteria);
    FilterCriteria loadPreset(const QString &name);
    QStringList getAvailablePresets() const;
    
signals:
    void filterApplied(const FilterCriteria &criteria);
    void previewRequested(const FilterCriteria &criteria);
    void presetSaved(const QString &name);
    void presetLoaded(const QString &name);

public slots:
    void applyFilter();
    void resetFilter();
    void previewFilter();
};

class MessageFilter : public QObject
{
    Q_OBJECT
    
public:
    explicit MessageFilter(QObject *parent = nullptr);
    
    // 过滤操作
    bool matchesFilter(const QString &message, const QString &messageType, 
                      const QDateTime &timestamp, int messageSize,
                      const FilterCriteria &criteria);
    
    QStringList filterMessages(const QStringList &messages, 
                              const QStringList &messageTypes,
                              const QList<QDateTime> &timestamps,
                              const QList<int> &messageSizes,
                              const FilterCriteria &criteria);
    
    // 统计信息
    int getMatchCount() const;
    qreal getMatchPercentage() const;
};
```

### 使用示例
```cpp
// 创建过滤对话框
AdvancedFilterDialog *filterDialog = new AdvancedFilterDialog();

// 设置过滤条件
AdvancedFilterDialog::FilterCriteria criteria;
criteria.contentPattern = "Error|Warning";
criteria.useRegex = true;
criteria.caseSensitive = false;
criteria.protocolLayers = {"NAS", "RRC"};

filterDialog->setFilterCriteria(criteria);

// 应用过滤
connect(filterDialog, &AdvancedFilterDialog::filterApplied,
        [](const AdvancedFilterDialog::FilterCriteria &criteria) {
            // 使用过滤条件
        });
```

## 📈 LogAnalysisWidget API

### 数据结构
```cpp
struct AnalysisResult {
    // 基本统计
    int totalMessages;
    int uniqueMessageTypes;
    QDateTime analysisStartTime;
    QDateTime analysisEndTime;
    qint64 analysisDuration;
    
    // 消息类型统计
    QMap<QString, int> messageTypeCounts;
    QMap<QString, qreal> messageTypePercentages;
    
    // 协议层统计
    QMap<QString, int> layerCounts;
    QMap<QString, qreal> layerPercentages;
    
    // 时序分析
    QMap<QDateTime, int> messageRateOverTime;
    QMap<QDateTime, qreal> dataThroughputOverTime;
    
    // 模式分析
    QStringList frequentPatterns;
    QStringList anomalies;
    QMap<QString, QStringList> messageSequences;
    
    // 性能指标
    qreal averageMessageRate;
    qreal peakMessageRate;
    qreal averageDataThroughput;
    qreal peakDataThroughput;
    
    // 质量指标
    int errorMessages;
    int warningMessages;
    qreal errorRate;
    QStringList topErrors;
    
    // 网络事件
    QStringList handoverEvents;
    QStringList connectionEvents;
    QStringList dataTransferEvents;
};
```

### 类定义
```cpp
class LogAnalysisWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LogAnalysisWidget(QWidget *parent = nullptr);
    ~LogAnalysisWidget();
    
    // 分析操作
    void analyzeLogFile(const QString &filePath);
    void analyzeRealTimeData(const QStringList &messages, const QStringList &messageTypes);
    void analyzeMessages(const QStringList &messages);
    
    // 结果访问
    AnalysisResult getAnalysisResult() const;
    bool hasAnalysisResult() const;
    
    // 导出
    void exportAnalysisReport(const QString &filePath);
    void exportAnalysisData(const QString &filePath, const QString &format);
    
    // 配置
    void setAnalysisOptions(const QVariantMap &options);
    QVariantMap getAnalysisOptions() const;
    
public slots:
    void startAnalysis();
    void stopAnalysis();
    void clearResults();
    void refreshAnalysis();
    
signals:
    void analysisStarted();
    void analysisProgress(int percentage);
    void analysisCompleted(const AnalysisResult &result);
    void analysisError(const QString &error);
    void resultExported(const QString &filePath);
};
```

### 使用示例
```cpp
// 创建分析组件
LogAnalysisWidget *analyzer = new LogAnalysisWidget();

// 配置分析选项
QVariantMap options;
options["detectPatterns"] = true;
options["detectAnomalies"] = true;
options["minPatternSupport"] = 3;
analyzer->setAnalysisOptions(options);

// 分析日志文件
analyzer->analyzeLogFile("/path/to/logfile.txt");

// 处理分析结果
connect(analyzer, &LogAnalysisWidget::analysisCompleted,
        [](const LogAnalysisWidget::AnalysisResult &result) {
            qDebug() << "Total messages:" << result.totalMessages;
            qDebug() << "Error rate:" << result.errorRate;
        });
```

## 📤 ExportManager API

### 数据结构
```cpp
enum ExportFormat {
    PlainText,
    CSV,
    JSON,
    XML,
    HTML,
    PDF,
    Excel
};

enum ExportType {
    RawData,
    FilteredData,
    AnalysisReport,
    StatisticsOnly,
    ChartsOnly,
    Complete
};

struct ExportOptions {
    ExportFormat format;
    ExportType type;
    QString outputPath;
    bool includeTimestamps;
    bool includeStatistics;
    bool includeCharts;
    bool includeMetadata;
    bool compressOutput;
    QString compressionFormat;
    QDateTime startTime;
    QDateTime endTime;
    QStringList selectedMessageTypes;
    bool useTimeRange;
    bool useMessageTypeFilter;
    QString customTemplate;
    bool useCustomTemplate;
};

struct ExportData {
    QStringList messages;
    QStringList messageTypes;
    QList<QDateTime> timestamps;
    QList<int> messageSizes;
    LogAnalysisWidget::AnalysisResult analysisResult;
    QMap<QString, QVariant> metadata;
    QList<QPixmap> charts;
};
```

### 类定义
```cpp
class ExportManager : public QObject
{
    Q_OBJECT
    
public:
    explicit ExportManager(QObject *parent = nullptr);
    ~ExportManager();
    
    // 导出操作
    bool exportData(const ExportData &data, const ExportOptions &options);
    bool exportToFile(const QString &filePath, const ExportData &data, const ExportOptions &options);
    void exportDataAsync(const ExportData &data, const ExportOptions &options);
    
    // 模板管理
    bool saveTemplate(const QString &templateName, const QString &templateContent);
    QString loadTemplate(const QString &templateName);
    QStringList getAvailableTemplates();
    bool deleteTemplate(const QString &templateName);
    
    // 格式支持
    QStringList getSupportedFormats();
    QString getDefaultExtension(ExportFormat format);
    bool validateExportOptions(const ExportOptions &options, QString &errorMessage);
    
    // 工具函数
    static QString formatSizeToString(qint64 size);
    static QString formatDurationToString(qint64 milliseconds);
    static ExportFormat formatFromString(const QString &formatString);
    static QString formatToString(ExportFormat format);
    
signals:
    void exportStarted();
    void exportProgress(int percentage, const QString &status);
    void exportCompleted(const QString &filePath);
    void exportError(const QString &error);
    void templateSaved(const QString &templateName);
    void templateLoaded(const QString &templateName);
};
```

### 使用示例
```cpp
// 创建导出管理器
ExportManager *exportMgr = new ExportManager();

// 准备导出数据
ExportManager::ExportData data;
data.messages = {"Message 1", "Message 2", "Message 3"};
data.messageTypes = {"Type A", "Type B", "Type A"};
// ... 设置其他数据

// 配置导出选项
ExportManager::ExportOptions options;
options.format = ExportManager::CSV;
options.type = ExportManager::Complete;
options.outputPath = "/path/to/export.csv";
options.includeTimestamps = true;
options.includeStatistics = true;

// 执行导出
exportMgr->exportData(data, options);

// 处理导出完成
connect(exportMgr, &ExportManager::exportCompleted,
        [](const QString &filePath) {
            qDebug() << "Export completed:" << filePath;
        });
```

## 🔧 工具类和辅助函数

### PatternDetector
```cpp
class PatternDetector
{
public:
    static QStringList detectFrequentPatterns(const QStringList &messages, int minSupport = 3);
    static QStringList detectAnomalies(const QStringList &messages, const QMap<QString, int> &typeCounts);
    static QMap<QString, QStringList> detectMessageSequences(const QStringList &messages, int windowSize = 10);
    
    // 配置
    static void setMinPatternLength(int length);
    static void setMaxPatternLength(int length);
    static void setMinSupport(int support);
};
```

### NetworkEventDetector
```cpp
class NetworkEventDetector
{
public:
    static QStringList detectHandoverEvents(const QStringList &messages);
    static QStringList detectConnectionEvents(const QStringList &messages);
    static QStringList detectDataTransferEvents(const QStringList &messages);
    static QStringList detectRRCStateChanges(const QStringList &messages);
    
    // 自定义事件检测
    static QStringList detectCustomEvents(const QStringList &messages, const QRegularExpression &pattern);
};
```

### DataValidator
```cpp
class DataValidator
{
public:
    static bool validateSerialData(const QByteArray &data);
    static bool validateConfiguration(const ConfigManager::CellNinjiaConfig &config);
    static bool validateMessageFormat(const QString &message);
    static bool validateTimestamp(const QDateTime &timestamp);
    static bool validateFilePath(const QString &filePath);
    
    // 数据清理
    static QByteArray sanitizeSerialData(const QByteArray &data);
    static QString sanitizeMessage(const QString &message);
};
```

## 📝 错误代码和异常处理

### 错误代码定义
```cpp
enum class ErrorCode {
    NoError = 0,
    
    // 连接错误 (1xx)
    ConnectionFailed = 100,
    PortNotFound = 101,
    PermissionDenied = 102,
    DeviceDisconnected = 103,
    
    // 配置错误 (2xx)
    InvalidConfiguration = 200,
    ConfigFileNotFound = 201,
    ConfigParseError = 202,
    
    // 数据错误 (3xx)
    InvalidDataFormat = 300,
    DataCorrupted = 301,
    MessageDecodeFailed = 302,
    
    // 文件错误 (4xx)
    FileNotFound = 400,
    FileAccessDenied = 401,
    FileWriteError = 402,
    
    // 系统错误 (5xx)
    OutOfMemory = 500,
    ThreadError = 501,
    SystemError = 502
};
```

### 异常类
```cpp
class CellNinjiaException : public std::exception
{
public:
    CellNinjiaException(ErrorCode code, const QString &message);
    
    ErrorCode errorCode() const;
    QString errorMessage() const;
    const char* what() const noexcept override;
    
private:
    ErrorCode m_code;
    QString m_message;
};
```

这个API文档为CellNinjia GUI的所有主要组件提供了完整的接口规范，帮助开发者理解和使用各个功能模块。