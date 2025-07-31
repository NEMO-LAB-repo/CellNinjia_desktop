# CellNinjia GUI - Advanced Features

## 🚀 High-Level Features Overview

CellNinjia GUI provides a comprehensive interface for next-generation network monitoring with advanced analysis capabilities.

### 📊 **Data Visualization**
- **Real-time Charts**: Live message rate, data throughput, signal strength
- **Distribution Charts**: Message type distribution, protocol layer analysis
- **Interactive Features**: Zoom, pan, export charts as PNG/PDF/SVG
- **Multiple Chart Types**: Line, area, bar charts with professional styling

### ⚙️ **Configuration Management**
- **Save/Load Configurations**: Persistent settings for different scenarios
- **Preset Configurations**: 5G Research, LTE Monitoring, Debug modes
- **Recent Configurations**: Quick access to recently used settings
- **JSON Import/Export**: Share configurations between systems

### 🔍 **Advanced Message Filtering**
- **Content Filters**: Regex patterns, case-sensitive search
- **Time-based Filters**: Date/time range filtering
- **Size Filters**: Message size constraints
- **Layer Filters**: Protocol layer-specific filtering (NAS, RRC, MAC, etc.)
- **Custom Filters**: Advanced scripting for complex criteria
- **Quick Filters**: Pre-defined filters for common scenarios

### 📈 **Log Analysis Tools**
- **Pattern Detection**: Automatic identification of frequent patterns
- **Anomaly Detection**: Statistical analysis for unusual events
- **Network Event Detection**: Handover, connection, data transfer events
- **Performance Metrics**: Message rates, throughput analysis
- **Error Analysis**: Error rate calculation and categorization
- **Temporal Analysis**: Time-series analysis with trend detection

### 📤 **Export Functionality**
- **Multiple Formats**: Plain text, CSV, JSON, XML, HTML, PDF, Excel
- **Export Types**: Raw data, filtered data, analysis reports, charts only
- **Template System**: Custom export templates with variable substitution
- **Compression**: ZIP/GZIP compression for large datasets
- **Batch Export**: Multiple format export in single operation

## 🎯 **Advanced GUI Components**

### ChartWidget
```cpp
// Real-time data visualization
ChartWidget *messageRateChart = new ChartWidget(ChartWidget::MessageRate);
messageRateChart->addDataPoint(messagesPerSecond);
messageRateChart->exportChart(); // Export to image
```

### ConfigManager
```cpp
// Configuration persistence
ConfigManager::CellNinjiaConfig config = configManager->getDefaultConfiguration();
config.monitoring.selectedMessageTypes = {"NR5G L2 UL TB", "NR5G MAC DCI Info"};
configManager->saveConfiguration(config, "5g_research.json");
```

### AdvancedFilterDialog
```cpp
// Complex message filtering
AdvancedFilterDialog::FilterCriteria criteria;
criteria.contentPattern = "Error|Warning";
criteria.useRegex = true;
criteria.protocolLayers = {"NAS", "RRC"};
criteria.useTimeFilter = true;
```

### LogAnalysisWidget
```cpp
// Comprehensive log analysis
LogAnalysisWidget *analyzer = new LogAnalysisWidget();
analyzer->analyzeLogFile("captured_data.log");
// Generates detailed analysis report with statistics and patterns
```

### ExportManager
```cpp
// Flexible data export
ExportManager::ExportOptions options;
options.format = ExportManager::PDF;
options.type = ExportManager::Complete;
options.includeCharts = true;
exportManager->exportData(data, options);
```

## 🔧 **Integration with Existing Code**

The GUI seamlessly integrates with existing CellNinjia functionality:

- **CellNinjiaCore**: Qt wrapper around existing serial communication
- **Shared Libraries**: Reuses `decode_preprocess.cpp` and ASN.1 decoding
- **Message Types**: Full support for all 42 message types
- **Compatibility**: Maintains backward compatibility with CLI version

## 📋 **Usage Scenarios**

### 🔬 **Research Workflow**
1. Load research configuration preset
2. Connect to device and start monitoring
3. Apply advanced filters for specific message types
4. Monitor real-time charts and statistics
5. Analyze patterns and anomalies
6. Export comprehensive report with charts

### 🐛 **Debug Workflow**
1. Load debug configuration with verbose logging
2. Apply error message filters
3. Monitor for specific patterns or anomalies
4. Analyze error sequences and timing
5. Export filtered data for further analysis

### 📊 **Performance Analysis**
1. Set up throughput and rate monitoring
2. Collect data over extended period
3. Analyze temporal patterns and trends
4. Generate performance reports
5. Export charts and statistics

## 🚀 **Future Extensions**

The modular design allows for easy extension:

- **Plugin System**: Custom analysis modules
- **Remote Monitoring**: Network-based device access
- **Machine Learning**: AI-powered pattern recognition
- **Real-time Alerts**: Threshold-based notifications
- **Multi-device Support**: Simultaneous monitoring
- **Cloud Integration**: Remote data storage and analysis

## 📚 **Technical Architecture**

```
GUI Architecture:
├── MainWindow (Qt5::Widgets)
│   ├── Connection Management
│   ├── Configuration Interface
│   └── Real-time Monitoring
│
├── Advanced Features
│   ├── ChartWidget (Qt5::Charts)
│   ├── ConfigManager (JSON persistence)
│   ├── AdvancedFilter (Complex filtering)
│   ├── LogAnalysis (Pattern detection)
│   └── ExportManager (Multi-format export)
│
└── Core Integration
    ├── CellNinjiaCore (Qt wrapper)
    ├── Serial Communication (Qt5::SerialPort)
    └── Message Decoding (existing C++ code)
```

This advanced GUI transforms CellNinjia from a command-line tool into a comprehensive research platform for next-generation network analysis.