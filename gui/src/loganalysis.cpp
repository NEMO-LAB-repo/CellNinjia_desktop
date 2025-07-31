#include "../include/loganalysis.h"
#include <QtCore/QThread>
#include <QtCore/QMutexLocker>
#include <QtCore/QRegularExpression>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtCore/QStandardPaths>
#include <algorithm>
#include <map>

LogAnalysisWidget::LogAnalysisWidget(QWidget *parent)
    : QWidget(parent)
    , m_analysisWorker(nullptr)
    , m_analysisThread(nullptr)
    , m_analysisInProgress(false)
    , m_updateTimer(new QTimer(this))
{
    setupUI();
    
    connect(m_updateTimer, &QTimer::timeout, this, &LogAnalysisWidget::onUpdateCharts);
    m_updateTimer->setInterval(2000); // Update every 2 seconds during analysis
}

LogAnalysisWidget::~LogAnalysisWidget()
{
    if (m_analysisThread) {
        m_analysisThread->quit();
        m_analysisThread->wait();
    }
}

void LogAnalysisWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // Control panel
    setupControlPanel();
    
    // Tab widget for different analysis views
    m_tabWidget = new QTabWidget(this);
    
    setupOverviewTab();
    setupMessageTypesTab();
    setupTemporalTab();
    setupPatternsTab();
    setupPerformanceTab();
    setupExportTab();
    
    m_mainLayout->addWidget(m_controlGroup);
    m_mainLayout->addWidget(m_tabWidget, 1);
    
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &LogAnalysisWidget::onTabChanged);
}

void LogAnalysisWidget::setupControlPanel()
{
    m_controlGroup = new QGroupBox("Analysis Control", this);
    auto *layout = new QHBoxLayout(m_controlGroup);
    
    m_startAnalysisBtn = new QPushButton("🚀 Start Analysis", this);
    m_stopAnalysisBtn = new QPushButton("⏹️ Stop Analysis", this);
    m_clearResultsBtn = new QPushButton("🗑️ Clear Results", this);
    
    m_startAnalysisBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    m_stopAnalysisBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; }");
    m_stopAnalysisBtn->setEnabled(false);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    
    m_statusLabel = new QLabel("Ready for analysis", this);
    m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    
    layout->addWidget(m_startAnalysisBtn);
    layout->addWidget(m_stopAnalysisBtn);
    layout->addWidget(m_clearResultsBtn);
    layout->addStretch();
    layout->addWidget(m_statusLabel);
    layout->addWidget(m_progressBar);
    
    connect(m_startAnalysisBtn, &QPushButton::clicked, this, &LogAnalysisWidget::startAnalysis);
    connect(m_stopAnalysisBtn, &QPushButton::clicked, this, &LogAnalysisWidget::stopAnalysis);
    connect(m_clearResultsBtn, &QPushButton::clicked, this, &LogAnalysisWidget::clearResults);
}

void LogAnalysisWidget::setupOverviewTab()
{
    m_overviewTab = new QWidget();
    m_tabWidget->addTab(m_overviewTab, "📊 Overview");
    
    auto *layout = new QVBoxLayout(m_overviewTab);
    
    // Key metrics
    auto *metricsGroup = new QGroupBox("Key Metrics", this);
    auto *metricsLayout = new QGridLayout(metricsGroup);
    
    m_totalMessagesLabel = new QLabel("0", this);
    m_uniqueTypesLabel = new QLabel("0", this);
    m_analysisTimeLabel = new QLabel("--", this);
    m_averageRateLabel = new QLabel("0.0 msg/s", this);
    m_errorRateLabel = new QLabel("0.0%", this);
    
    // Style the metrics
    QString metricStyle = "font-size: 24px; font-weight: bold; color: #2196F3; border: 1px solid #ddd; padding: 10px; text-align: center;";
    m_totalMessagesLabel->setStyleSheet(metricStyle);
    m_uniqueTypesLabel->setStyleSheet(metricStyle);
    m_analysisTimeLabel->setStyleSheet(metricStyle);
    m_averageRateLabel->setStyleSheet(metricStyle);
    m_errorRateLabel->setStyleSheet(metricStyle);
    
    metricsLayout->addWidget(new QLabel("Total Messages:"), 0, 0);
    metricsLayout->addWidget(m_totalMessagesLabel, 0, 1);
    metricsLayout->addWidget(new QLabel("Unique Types:"), 0, 2);
    metricsLayout->addWidget(m_uniqueTypesLabel, 0, 3);
    metricsLayout->addWidget(new QLabel("Analysis Duration:"), 1, 0);
    metricsLayout->addWidget(m_analysisTimeLabel, 1, 1);
    metricsLayout->addWidget(new QLabel("Average Rate:"), 1, 2);
    metricsLayout->addWidget(m_averageRateLabel, 1, 3);
    
    layout->addWidget(metricsGroup);
    
    // Summary table
    auto *summaryGroup = new QGroupBox("Analysis Summary", this);
    auto *summaryLayout = new QVBoxLayout(summaryGroup);
    
    m_summaryTable = new QTableWidget(this);
    m_summaryTable->setColumnCount(2);
    m_summaryTable->setHorizontalHeaderLabels({"Metric", "Value"});
    m_summaryTable->horizontalHeader()->setStretchLastSection(true);
    
    summaryLayout->addWidget(m_summaryTable);
    layout->addWidget(summaryGroup);
}

void LogAnalysisWidget::setupMessageTypesTab()
{
    m_messageTypesTab = new QWidget();
    m_tabWidget->addTab(m_messageTypesTab, "📋 Message Types");
    
    auto *layout = new QVBoxLayout(m_messageTypesTab);
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Message types table
    auto *tableGroup = new QGroupBox("Message Type Statistics", this);
    auto *tableLayout = new QVBoxLayout(tableGroup);
    
    m_messageTypesTable = new QTableWidget(this);
    m_messageTypesTable->setColumnCount(4);
    m_messageTypesTable->setHorizontalHeaderLabels({"Message Type", "Count", "Percentage", "Avg Size"});
    m_messageTypesTable->horizontalHeader()->setStretchLastSection(true);
    m_messageTypesTable->setSortingEnabled(true);
    
    tableLayout->addWidget(m_messageTypesTable);
    
    // Charts
    auto *chartsGroup = new QGroupBox("Distribution Charts", this);
    auto *chartsLayout = new QVBoxLayout(chartsGroup);
    
    m_messageTypesChart = new ChartWidget(ChartWidget::MessageTypes, this);
    m_layerDistributionChart = new ChartWidget(ChartWidget::LayerDistribution, this);
    
    chartsLayout->addWidget(m_messageTypesChart);
    chartsLayout->addWidget(m_layerDistributionChart);
    
    splitter->addWidget(tableGroup);
    splitter->addWidget(chartsGroup);
    splitter->setSizes({400, 600});
    
    layout->addWidget(splitter);
}

void LogAnalysisWidget::setupTemporalTab()
{
    m_temporalTab = new QWidget();
    m_tabWidget->addTab(m_temporalTab, "⏰ Temporal Analysis");
    
    auto *layout = new QVBoxLayout(m_temporalTab);
    auto *splitter = new QSplitter(Qt::Vertical, this);
    
    // Charts
    auto *chartsGroup = new QGroupBox("Time Series Analysis", this);
    auto *chartsLayout = new QHBoxLayout(chartsGroup);
    
    m_messageRateChart = new ChartWidget(ChartWidget::MessageRate, this);
    m_throughputChart = new ChartWidget(ChartWidget::DataThroughput, this);
    
    chartsLayout->addWidget(m_messageRateChart);
    chartsLayout->addWidget(m_throughputChart);
    
    // Timeline table
    auto *timelineGroup = new QGroupBox("Timeline Events", this);
    auto *timelineLayout = new QVBoxLayout(timelineGroup);
    
    m_timelineTable = new QTableWidget(this);
    m_timelineTable->setColumnCount(4);
    m_timelineTable->setHorizontalHeaderLabels({"Timestamp", "Event Type", "Description", "Impact"});
    m_timelineTable->horizontalHeader()->setStretchLastSection(true);
    
    timelineLayout->addWidget(m_timelineTable);
    
    splitter->addWidget(chartsGroup);
    splitter->addWidget(timelineGroup);
    splitter->setSizes({400, 300});
    
    layout->addWidget(splitter);
}

void LogAnalysisWidget::setupPatternsTab()
{
    m_patternsTab = new QWidget();
    m_tabWidget->addTab(m_patternsTab, "🔍 Pattern Analysis");
    
    auto *layout = new QHBoxLayout(m_patternsTab);
    auto *leftSplitter = new QSplitter(Qt::Vertical, this);
    
    // Patterns tree
    auto *patternsGroup = new QGroupBox("Frequent Patterns", this);
    auto *patternsLayout = new QVBoxLayout(patternsGroup);
    
    m_patternsTree = new QTreeWidget(this);
    m_patternsTree->setHeaderLabels({"Pattern", "Support", "Confidence"});
    patternsLayout->addWidget(m_patternsTree);
    
    // Anomalies tree
    auto *anomaliesGroup = new QGroupBox("Detected Anomalies", this);
    auto *anomaliesLayout = new QVBoxLayout(anomaliesGroup);
    
    m_anomaliesTree = new QTreeWidget(this);
    m_anomaliesTree->setHeaderLabels({"Anomaly Type", "Severity", "Description"});
    anomaliesLayout->addWidget(m_anomaliesTree);
    
    // Sequences tree
    auto *sequencesGroup = new QGroupBox("Message Sequences", this);
    auto *sequencesLayout = new QVBoxLayout(sequencesGroup);
    
    m_sequencesTree = new QTreeWidget(this);
    m_sequencesTree->setHeaderLabels({"Sequence", "Frequency", "Duration"});
    sequencesLayout->addWidget(m_sequencesTree);
    
    leftSplitter->addWidget(patternsGroup);
    leftSplitter->addWidget(anomaliesGroup);
    leftSplitter->addWidget(sequencesGroup);
    
    // Details panel
    auto *detailsGroup = new QGroupBox("Pattern Details", this);
    auto *detailsLayout = new QVBoxLayout(detailsGroup);
    
    m_patternDetails = new QTextEdit(this);
    m_patternDetails->setReadOnly(true);
    detailsLayout->addWidget(m_patternDetails);
    
    layout->addWidget(leftSplitter, 2);
    layout->addWidget(detailsGroup, 1);
}

void LogAnalysisWidget::setupPerformanceTab()
{
    m_performanceTab = new QWidget();
    m_tabWidget->addTab(m_performanceTab, "⚡ Performance");
    
    auto *layout = new QVBoxLayout(m_performanceTab);
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Performance metrics table
    auto *perfGroup = new QGroupBox("Performance Metrics", this);
    auto *perfLayout = new QVBoxLayout(perfGroup);
    
    m_performanceTable = new QTableWidget(this);
    m_performanceTable->setColumnCount(3);
    m_performanceTable->setHorizontalHeaderLabels({"Metric", "Value", "Status"});
    m_performanceTable->horizontalHeader()->setStretchLastSection(true);
    
    perfLayout->addWidget(m_performanceTable);
    
    // Errors table
    auto *errorsGroup = new QGroupBox("Error Analysis", this);
    auto *errorsLayout = new QVBoxLayout(errorsGroup);
    
    m_errorsTable = new QTableWidget(this);
    m_errorsTable->setColumnCount(4);
    m_errorsTable->setHorizontalHeaderLabels({"Error Type", "Count", "Percentage", "Last Seen"});
    m_errorsTable->horizontalHeader()->setStretchLastSection(true);
    
    errorsLayout->addWidget(m_errorsTable);
    
    // Events table  
    auto *eventsGroup = new QGroupBox("Network Events", this);
    auto *eventsLayout = new QVBoxLayout(eventsGroup);
    
    m_eventsTable = new QTableWidget(this);
    m_eventsTable->setColumnCount(3);
    m_eventsTable->setHorizontalHeaderLabels({"Event Type", "Timestamp", "Details"});
    m_eventsTable->horizontalHeader()->setStretchLastSection(true);
    
    eventsLayout->addWidget(m_eventsTable);
    
    auto *rightSplitter = new QSplitter(Qt::Vertical, this);
    rightSplitter->addWidget(errorsGroup);
    rightSplitter->addWidget(eventsGroup);
    
    splitter->addWidget(perfGroup);
    splitter->addWidget(rightSplitter);
    
    layout->addWidget(splitter);
}

void LogAnalysisWidget::setupExportTab()
{
    m_exportTab = new QWidget();
    m_tabWidget->addTab(m_exportTab, "📤 Export");
    
    auto *layout = new QVBoxLayout(m_exportTab);
    
    // Export options
    auto *optionsGroup = new QGroupBox("Export Options", this);
    auto *optionsLayout = new QHBoxLayout(optionsGroup);
    
    m_exportFormatCombo = new QComboBox(this);
    m_exportFormatCombo->addItems({"HTML Report", "PDF Report", "CSV Data", "JSON Data", "Text Report"});
    
    m_exportButton = new QPushButton("📤 Export Analysis", this);
    m_saveReportButton = new QPushButton("💾 Save Report", this);
    
    m_exportButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; font-weight: bold; }");
    m_saveReportButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    optionsLayout->addWidget(new QLabel("Format:"));
    optionsLayout->addWidget(m_exportFormatCombo);
    optionsLayout->addStretch();
    optionsLayout->addWidget(m_saveReportButton);
    optionsLayout->addWidget(m_exportButton);
    
    // Report preview
    auto *previewGroup = new QGroupBox("Report Preview", this);
    auto *previewLayout = new QVBoxLayout(previewGroup);
    
    m_reportPreview = new QTextEdit(this);
    m_reportPreview->setReadOnly(true);
    m_reportPreview->setFont(QFont("monospace", 9));
    
    previewLayout->addWidget(m_reportPreview);
    
    layout->addWidget(optionsGroup);
    layout->addWidget(previewGroup, 1);
    
    connect(m_exportButton, &QPushButton::clicked, this, &LogAnalysisWidget::exportReport);
    connect(m_saveReportButton, &QPushButton::clicked, this, &LogAnalysisWidget::saveReport);
}

void LogAnalysisWidget::analyzeLogFile(const QString &filePath)
{
    if (m_analysisInProgress) {
        QMessageBox::warning(this, "Analysis in Progress", "Please stop the current analysis first.");
        return;
    }
    
    // Create and setup worker thread
    m_analysisWorker = new LogAnalysisWorker();
    m_analysisThread = new QThread();
    m_analysisWorker->moveToThread(m_analysisThread);
    
    // Connect signals
    connect(m_analysisThread, &QThread::started, m_analysisWorker, &LogAnalysisWorker::startAnalysis);
    connect(m_analysisWorker, &LogAnalysisWorker::progress, this, &LogAnalysisWidget::onAnalysisProgress);
    connect(m_analysisWorker, &LogAnalysisWorker::finished, this, &LogAnalysisWidget::onAnalysisCompleted);
    connect(m_analysisWorker, &LogAnalysisWorker::error, this, &LogAnalysisWidget::onAnalysisError);
    
    // Cleanup on thread finish
    connect(m_analysisThread, &QThread::finished, m_analysisWorker, &QObject::deleteLater);
    connect(m_analysisThread, &QThread::finished, m_analysisThread, &QObject::deleteLater);
    
    m_analysisWorker->setLogFile(filePath);
    m_analysisThread->start();
    
    m_analysisInProgress = true;
    m_startAnalysisBtn->setEnabled(false);
    m_stopAnalysisBtn->setEnabled(true);
    m_progressBar->setVisible(true);
    m_updateTimer->start();
    
    emit analysisStarted();
}

void LogAnalysisWidget::analyzeRealTimeData(const QStringList &messages, const QStringList &messageTypes)
{
    if (m_analysisInProgress) {
        return; // Skip if analysis is already in progress
    }
    
    m_analysisWorker = new LogAnalysisWorker();
    m_analysisThread = new QThread();
    m_analysisWorker->moveToThread(m_analysisThread);
    
    connect(m_analysisThread, &QThread::started, m_analysisWorker, &LogAnalysisWorker::startAnalysis);
    connect(m_analysisWorker, &LogAnalysisWorker::finished, this, &LogAnalysisWidget::onAnalysisCompleted);
    connect(m_analysisWorker, &LogAnalysisWorker::error, this, &LogAnalysisWidget::onAnalysisError);
    
    m_analysisWorker->setRealTimeData(messages, messageTypes);
    m_analysisThread->start();
}

void LogAnalysisWidget::startAnalysis()
{
    QString filePath = QFileDialog::getOpenFileName(this, 
        "Select Log File", 
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Log files (*.log *.txt);;All files (*)");
    
    if (!filePath.isEmpty()) {
        analyzeLogFile(filePath);
    }
}

void LogAnalysisWidget::stopAnalysis()
{
    if (m_analysisWorker) {
        m_analysisWorker->stopAnalysis();
    }
    
    m_analysisInProgress = false;
    m_startAnalysisBtn->setEnabled(true);
    m_stopAnalysisBtn->setEnabled(false);
    m_progressBar->setVisible(false);
    m_updateTimer->stop();
    m_statusLabel->setText("Analysis stopped");
}

void LogAnalysisWidget::clearResults()
{
    m_currentResult = AnalysisResult();
    
    // Clear all displays
    m_totalMessagesLabel->setText("0");
    m_uniqueTypesLabel->setText("0");
    m_analysisTimeLabel->setText("--");
    m_averageRateLabel->setText("0.0 msg/s");
    m_errorRateLabel->setText("0.0%");
    
    m_summaryTable->setRowCount(0);
    m_messageTypesTable->setRowCount(0);
    m_timelineTable->setRowCount(0);
    m_performanceTable->setRowCount(0);
    m_errorsTable->setRowCount(0);
    m_eventsTable->setRowCount(0);
    
    m_patternsTree->clear();
    m_anomaliesTree->clear();
    m_sequencesTree->clear();
    m_patternDetails->clear();
    m_reportPreview->clear();
    
    // Clear charts
    if (m_messageTypesChart) m_messageTypesChart->clearData();
    if (m_layerDistributionChart) m_layerDistributionChart->clearData();
    if (m_messageRateChart) m_messageRateChart->clearData();
    if (m_throughputChart) m_throughputChart->clearData();
}

void LogAnalysisWidget::onAnalysisProgress(int percentage)
{
    m_progressBar->setValue(percentage);
    m_statusLabel->setText(QString("Analyzing... %1%").arg(percentage));
    emit analysisProgress(percentage);
}

void LogAnalysisWidget::onAnalysisCompleted(const AnalysisResult &result)
{
    m_currentResult = result;
    m_analysisInProgress = false;
    m_startAnalysisBtn->setEnabled(true);
    m_stopAnalysisBtn->setEnabled(false);
    m_progressBar->setVisible(false);
    m_updateTimer->stop();
    
    m_statusLabel->setText("Analysis completed successfully");
    m_statusLabel->setStyleSheet("color: green; font-weight: bold;");
    
    // Update all tabs with results
    updateOverviewTab(result);
    updateMessageTypesTab(result);
    updateTemporalTab(result);
    updatePatternsTab(result);
    updatePerformanceTab(result);
    
    // Generate report preview
    QString reportContent;
    QTextStream stream(&reportContent);
    generateReport(result, stream);
    m_reportPreview->setPlainText(reportContent);
    
    emit analysisCompleted(result);
}

void LogAnalysisWidget::onAnalysisError(const QString &error)
{
    m_analysisInProgress = false;
    m_startAnalysisBtn->setEnabled(true);
    m_stopAnalysisBtn->setEnabled(false);
    m_progressBar->setVisible(false);
    m_updateTimer->stop();
    
    m_statusLabel->setText("Analysis failed: " + error);
    m_statusLabel->setStyleSheet("color: red; font-weight: bold;");
    
    QMessageBox::critical(this, "Analysis Error", "Analysis failed:\n" + error);
    emit analysisError(error);
}

void LogAnalysisWidget::updateOverviewTab(const AnalysisResult &result)
{
    m_totalMessagesLabel->setText(QString::number(result.totalMessages));
    m_uniqueTypesLabel->setText(QString::number(result.uniqueMessageTypes));
    m_analysisTimeLabel->setText(QString("%1 ms").arg(result.analysisDuration));
    m_averageRateLabel->setText(QString("%1 msg/s").arg(result.averageMessageRate, 0, 'f', 2));
    m_errorRateLabel->setText(QString("%1%").arg(result.errorRate, 0, 'f', 2));
    
    // Update summary table
    m_summaryTable->setRowCount(0);
    QStringList summaryItems = {
        "Total Messages|" + QString::number(result.totalMessages),
        "Unique Message Types|" + QString::number(result.uniqueMessageTypes),
        "Analysis Duration|" + QString("%1 ms").arg(result.analysisDuration),
        "Average Message Rate|" + QString("%1 msg/s").arg(result.averageMessageRate, 0, 'f', 2),
        "Peak Message Rate|" + QString("%1 msg/s").arg(result.peakMessageRate, 0, 'f', 2),
        "Error Messages|" + QString::number(result.errorMessages),
        "Warning Messages|" + QString::number(result.warningMessages),
        "Error Rate|" + QString("%1%").arg(result.errorRate, 0, 'f', 2)
    };
    
    for (const QString &item : summaryItems) {
        QStringList parts = item.split('|');
        if (parts.size() == 2) {
            int row = m_summaryTable->rowCount();
            m_summaryTable->insertRow(row);
            m_summaryTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            m_summaryTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
        }
    }
}

void LogAnalysisWidget::updateMessageTypesTab(const AnalysisResult &result)
{
    // Update message types table
    m_messageTypesTable->setRowCount(0);
    
    for (auto it = result.messageTypeCounts.begin(); it != result.messageTypeCounts.end(); ++it) {
        int row = m_messageTypesTable->rowCount();
        m_messageTypesTable->insertRow(row);
        
        m_messageTypesTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        m_messageTypesTable->setItem(row, 1, new QTableWidgetItem(QString::number(it.value())));
        
        qreal percentage = result.messageTypePercentages.value(it.key(), 0.0);
        m_messageTypesTable->setItem(row, 2, new QTableWidgetItem(QString("%1%").arg(percentage, 0, 'f', 2)));
        
        // Calculate average size (placeholder)
        m_messageTypesTable->setItem(row, 3, new QTableWidgetItem("N/A"));
    }
    
    // Update charts
    if (m_messageTypesChart) {
        for (auto it = result.messageTypeCounts.begin(); it != result.messageTypeCounts.end(); ++it) {
            m_messageTypesChart->addMessageType(it.key(), it.value());
        }
    }
    
    if (m_layerDistributionChart) {
        m_layerDistributionChart->updateLayerDistribution(result.layerCounts);
    }
}

void LogAnalysisWidget::updateTemporalTab(const AnalysisResult &result)
{
    // Update timeline table
    m_timelineTable->setRowCount(0);
    
    // Add handover events
    for (const QString &event : result.handoverEvents) {
        int row = m_timelineTable->rowCount();
        m_timelineTable->insertRow(row);
        m_timelineTable->setItem(row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
        m_timelineTable->setItem(row, 1, new QTableWidgetItem("Handover"));
        m_timelineTable->setItem(row, 2, new QTableWidgetItem(event));
        m_timelineTable->setItem(row, 3, new QTableWidgetItem("Medium"));
    }
    
    // Add connection events
    for (const QString &event : result.connectionEvents) {
        int row = m_timelineTable->rowCount();
        m_timelineTable->insertRow(row);
        m_timelineTable->setItem(row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
        m_timelineTable->setItem(row, 1, new QTableWidgetItem("Connection"));
        m_timelineTable->setItem(row, 2, new QTableWidgetItem(event));
        m_timelineTable->setItem(row, 3, new QTableWidgetItem("High"));
    }
    
    // Update charts with temporal data
    if (m_messageRateChart) {
        for (auto it = result.messageRateOverTime.begin(); it != result.messageRateOverTime.end(); ++it) {
            m_messageRateChart->addDataPoint(it.value(), it.key());
        }
    }
    
    if (m_throughputChart) {
        for (auto it = result.dataThroughputOverTime.begin(); it != result.dataThroughputOverTime.end(); ++it) {
            m_throughputChart->addDataPoint(it.value(), it.key());
        }
    }
}

void LogAnalysisWidget::updatePatternsTab(const AnalysisResult &result)
{
    // Update patterns tree
    m_patternsTree->clear();
    for (const QString &pattern : result.frequentPatterns) {
        auto *item = new QTreeWidgetItem(m_patternsTree);
        item->setText(0, pattern);
        item->setText(1, "High"); // Placeholder
        item->setText(2, "85%"); // Placeholder
    }
    
    // Update anomalies tree
    m_anomaliesTree->clear();
    for (const QString &anomaly : result.anomalies) {
        auto *item = new QTreeWidgetItem(m_anomaliesTree);
        item->setText(0, anomaly);
        item->setText(1, "Medium"); // Placeholder
        item->setText(2, "Unusual pattern detected");
    }
    
    // Update sequences tree
    m_sequencesTree->clear();
    for (auto it = result.messageSequences.begin(); it != result.messageSequences.end(); ++it) {
        auto *item = new QTreeWidgetItem(m_sequencesTree);
        item->setText(0, it.key());
        item->setText(1, QString::number(it.value().size()));
        item->setText(2, "N/A");
    }
}

void LogAnalysisWidget::updatePerformanceTab(const AnalysisResult &result)
{
    // Update performance table
    m_performanceTable->setRowCount(0);
    
    QStringList perfMetrics = {
        "Average Message Rate|" + QString("%1 msg/s").arg(result.averageMessageRate, 0, 'f', 2) + "|Good",
        "Peak Message Rate|" + QString("%1 msg/s").arg(result.peakMessageRate, 0, 'f', 2) + "|Good",
        "Average Throughput|" + QString("%1 KB/s").arg(result.averageDataThroughput, 0, 'f', 2) + "|Good",
        "Peak Throughput|" + QString("%1 KB/s").arg(result.peakDataThroughput, 0, 'f', 2) + "|Good",
        "Error Rate|" + QString("%1%").arg(result.errorRate, 0, 'f', 2) + "|" + (result.errorRate > 5.0 ? "Warning" : "Good")
    };
    
    for (const QString &metric : perfMetrics) {
        QStringList parts = metric.split('|');
        if (parts.size() == 3) {
            int row = m_performanceTable->rowCount();
            m_performanceTable->insertRow(row);
            m_performanceTable->setItem(row, 0, new QTableWidgetItem(parts[0]));
            m_performanceTable->setItem(row, 1, new QTableWidgetItem(parts[1]));
            m_performanceTable->setItem(row, 2, new QTableWidgetItem(parts[2]));
        }
    }
    
    // Update errors table
    m_errorsTable->setRowCount(0);
    for (const QString &error : result.topErrors) {
        int row = m_errorsTable->rowCount();
        m_errorsTable->insertRow(row);
        m_errorsTable->setItem(row, 0, new QTableWidgetItem(error));
        m_errorsTable->setItem(row, 1, new QTableWidgetItem("1")); // Placeholder
        m_errorsTable->setItem(row, 2, new QTableWidgetItem("0.1%")); // Placeholder
        m_errorsTable->setItem(row, 3, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
    }
    
    // Update events table
    m_eventsTable->setRowCount(0);
    QStringList allEvents = result.handoverEvents + result.connectionEvents + result.dataTransferEvents;
    for (const QString &event : allEvents) {
        int row = m_eventsTable->rowCount();
        m_eventsTable->insertRow(row);
        
        QString eventType = "Unknown";
        if (result.handoverEvents.contains(event)) eventType = "Handover";
        else if (result.connectionEvents.contains(event)) eventType = "Connection";
        else if (result.dataTransferEvents.contains(event)) eventType = "Data Transfer";
        
        m_eventsTable->setItem(row, 0, new QTableWidgetItem(eventType));
        m_eventsTable->setItem(row, 1, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
        m_eventsTable->setItem(row, 2, new QTableWidgetItem(event));
    }
}

void LogAnalysisWidget::generateReport(const AnalysisResult &result, QTextStream &stream)
{
    stream << "CellNinjia Log Analysis Report\n";
    stream << "==============================\n\n";
    
    stream << "Analysis Summary:\n";
    stream << "- Total Messages: " << result.totalMessages << "\n";
    stream << "- Unique Message Types: " << result.uniqueMessageTypes << "\n";
    stream << "- Analysis Duration: " << result.analysisDuration << " ms\n";
    stream << "- Average Message Rate: " << QString::number(result.averageMessageRate, 'f', 2) << " msg/s\n";
    stream << "- Peak Message Rate: " << QString::number(result.peakMessageRate, 'f', 2) << " msg/s\n";
    stream << "- Error Rate: " << QString::number(result.errorRate, 'f', 2) << "%\n\n";
    
    stream << "Message Type Distribution:\n";
    for (auto it = result.messageTypeCounts.begin(); it != result.messageTypeCounts.end(); ++it) {
        qreal percentage = result.messageTypePercentages.value(it.key(), 0.0);
        stream << "- " << it.key() << ": " << it.value() << " (" << QString::number(percentage, 'f', 2) << "%)\n";
    }
    
    stream << "\nProtocol Layer Distribution:\n";
    for (auto it = result.layerCounts.begin(); it != result.layerCounts.end(); ++it) {
        qreal percentage = result.layerPercentages.value(it.key(), 0.0);
        stream << "- " << it.key() << ": " << it.value() << " (" << QString::number(percentage, 'f', 2) << "%)\n";
    }
    
    if (!result.frequentPatterns.isEmpty()) {
        stream << "\nFrequent Patterns:\n";
        for (const QString &pattern : result.frequentPatterns) {
            stream << "- " << pattern << "\n";
        }
    }
    
    if (!result.anomalies.isEmpty()) {
        stream << "\nDetected Anomalies:\n";
        for (const QString &anomaly : result.anomalies) {
            stream << "- " << anomaly << "\n";
        }
    }
    
    if (!result.topErrors.isEmpty()) {
        stream << "\nTop Errors:\n";
        for (const QString &error : result.topErrors) {
            stream << "- " << error << "\n";
        }
    }
    
    stream << "\nNetwork Events:\n";
    stream << "- Handover Events: " << result.handoverEvents.size() << "\n";
    stream << "- Connection Events: " << result.connectionEvents.size() << "\n";
    stream << "- Data Transfer Events: " << result.dataTransferEvents.size() << "\n";
}

void LogAnalysisWidget::saveReport()
{
    QString filePath = QFileDialog::getSaveFileName(this, 
        "Save Analysis Report", 
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/analysis_report.txt",
        "Text files (*.txt);;All files (*)");
    
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            generateReport(m_currentResult, stream);
            QMessageBox::information(this, "Report Saved", "Analysis report saved to:\n" + filePath);
        }
    }
}

void LogAnalysisWidget::exportReport()
{
    // This would integrate with ExportManager for different formats
    saveReport(); // For now, just save as text
}

void LogAnalysisWidget::onTabChanged(int index)
{
    // Update charts when tab becomes visible
    if (index == 1 && m_messageTypesChart) { // Message Types tab
        m_messageTypesChart->updateChart();
        m_layerDistributionChart->updateChart();
    } else if (index == 2 && m_messageRateChart) { // Temporal tab
        m_messageRateChart->updateChart();
        m_throughputChart->updateChart();
    }
}

void LogAnalysisWidget::onUpdateCharts()
{
    // Update charts during analysis if needed
    if (m_tabWidget->currentIndex() == 1) {
        if (m_messageTypesChart) m_messageTypesChart->updateChart();
        if (m_layerDistributionChart) m_layerDistributionChart->updateChart();
    } else if (m_tabWidget->currentIndex() == 2) {
        if (m_messageRateChart) m_messageRateChart->updateChart();
        if (m_throughputChart) m_throughputChart->updateChart();
    }
}

void LogAnalysisWidget::onExportChart()
{
    // Export current visible chart
    ChartWidget *currentChart = nullptr;
    
    int tabIndex = m_tabWidget->currentIndex();
    if (tabIndex == 1) {
        currentChart = m_messageTypesChart;
    } else if (tabIndex == 2) {
        currentChart = m_messageRateChart;
    }
    
    if (currentChart) {
        currentChart->exportChart();
    }
}