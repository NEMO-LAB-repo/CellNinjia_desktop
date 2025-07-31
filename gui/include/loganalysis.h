#ifndef LOGANALYSIS_H
#define LOGANALYSIS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTabWidget>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCharts/QChartView>
#include "chartwidget.h"

class LogAnalysisWorker;

class LogAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    struct AnalysisResult {
        // General Statistics
        int totalMessages;
        int uniqueMessageTypes;
        QDateTime analysisStartTime;
        QDateTime analysisEndTime;
        qint64 analysisDuration;
        
        // Message Type Statistics
        QMap<QString, int> messageTypeCounts;
        QMap<QString, qreal> messageTypePercentages;
        
        // Protocol Layer Statistics
        QMap<QString, int> layerCounts;
        QMap<QString, qreal> layerPercentages;
        
        // Temporal Analysis
        QMap<QDateTime, int> messageRateOverTime;
        QMap<QDateTime, qreal> dataThroughputOverTime;
        
        // Pattern Analysis
        QStringList frequentPatterns;
        QStringList anomalies;
        QMap<QString, QStringList> messageSequences;
        
        // Performance Metrics
        qreal averageMessageRate;
        qreal peakMessageRate;
        qreal averageDataThroughput;
        qreal peakDataThroughput;
        
        // Quality Metrics
        int errorMessages;
        int warningMessages;
        qreal errorRate;
        QStringList topErrors;
        
        // Network Events
        QStringList handoverEvents;
        QStringList connectionEvents;
        QStringList dataTransferEvents;
    };

    explicit LogAnalysisWidget(QWidget *parent = nullptr);
    ~LogAnalysisWidget();

    void analyzeLogFile(const QString &filePath);
    void analyzeRealTimeData(const QStringList &messages, const QStringList &messageTypes);
    void exportAnalysisReport(const QString &filePath);

signals:
    void analysisStarted();
    void analysisProgress(int percentage);
    void analysisCompleted(const AnalysisResult &result);
    void analysisError(const QString &error);

public slots:
    void startAnalysis();
    void stopAnalysis();
    void saveReport();
    void exportReport();

private slots:
    void onAnalysisProgress(int percentage);
    void onAnalysisCompleted(const AnalysisResult &result);
    void onAnalysisError(const QString &error);
    void onTabChanged(int index);
    void onUpdateCharts();
    void onExportChart();

private:
    void setupUI();
    void setupOverviewTab();
    void setupMessageTypesTab();
    void setupTemporalTab();
    void setupPatternsTab();
    void setupPerformanceTab();
    void setupExportTab();
    
    void updateOverviewTab(const AnalysisResult &result);
    void updateMessageTypesTab(const AnalysisResult &result);
    void updateTemporalTab(const AnalysisResult &result);
    void updatePatternsTab(const AnalysisResult &result);
    void updatePerformanceTab(const AnalysisResult &result);
    
    void clearResults();
    void generateReport(const AnalysisResult &result, QTextStream &stream);
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;
    
    // Overview Tab
    QWidget *m_overviewTab;
    QLabel *m_totalMessagesLabel;
    QLabel *m_uniqueTypesLabel;
    QLabel *m_analysisTimeLabel;
    QLabel *m_averageRateLabel;
    QLabel *m_errorRateLabel;
    QTableWidget *m_summaryTable;
    
    // Message Types Tab
    QWidget *m_messageTypesTab;
    QTableWidget *m_messageTypesTable;
    ChartWidget *m_messageTypesChart;
    ChartWidget *m_layerDistributionChart;
    
    // Temporal Analysis Tab
    QWidget *m_temporalTab;
    ChartWidget *m_messageRateChart;
    ChartWidget *m_throughputChart;
    QTableWidget *m_timelineTable;
    
    // Pattern Analysis Tab
    QWidget *m_patternsTab;
    QTreeWidget *m_patternsTree;
    QTreeWidget *m_anomaliesTree;
    QTreeWidget *m_sequencesTree;
    QTextEdit *m_patternDetails;
    
    // Performance Tab
    QWidget *m_performanceTab;
    QTableWidget *m_performanceTable;
    QTableWidget *m_errorsTable;
    QTableWidget *m_eventsTable;
    
    // Export Tab
    QWidget *m_exportTab;
    QTextEdit *m_reportPreview;
    QComboBox *m_exportFormatCombo;
    QPushButton *m_exportButton;
    QPushButton *m_saveReportButton;
    
    // Control Panel
    QGroupBox *m_controlGroup;
    QPushButton *m_startAnalysisBtn;
    QPushButton *m_stopAnalysisBtn;
    QPushButton *m_clearResultsBtn;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
    
    // Analysis Engine
    LogAnalysisWorker *m_analysisWorker;
    QThread *m_analysisThread;
    AnalysisResult m_currentResult;
    bool m_analysisInProgress;
    
    // Update Timer
    QTimer *m_updateTimer;
};

class LogAnalysisWorker : public QObject
{
    Q_OBJECT

public:
    explicit LogAnalysisWorker(QObject *parent = nullptr);
    
    void setLogFile(const QString &filePath);
    void setRealTimeData(const QStringList &messages, const QStringList &messageTypes);

public slots:
    void startAnalysis();
    void stopAnalysis();

signals:
    void progress(int percentage);
    void finished(const LogAnalysisWidget::AnalysisResult &result);
    void error(const QString &error);

private:
    void analyzeMessages();
    void detectPatterns();
    void detectAnomalies();
    void calculateStatistics();
    void analyzeTemporalData();
    void analyzeNetworkEvents();
    
    QString extractMessageType(const QString &message);
    QString extractProtocolLayer(const QString &messageType);
    QDateTime extractTimestamp(const QString &message);
    int extractMessageSize(const QString &message);
    bool isErrorMessage(const QString &message);
    bool isWarningMessage(const QString &message);
    
    QString m_logFilePath;
    QStringList m_messages;
    QStringList m_messageTypes;
    LogAnalysisWidget::AnalysisResult m_result;
    bool m_stopRequested;
    QMutex m_mutex;
};

// Utility classes for pattern detection and analysis
class PatternDetector
{
public:
    static QStringList detectFrequentPatterns(const QStringList &messages, int minSupport = 3);
    static QStringList detectAnomalies(const QStringList &messages, const QMap<QString, int> &typeCounts);
    static QMap<QString, QStringList> detectMessageSequences(const QStringList &messages, int windowSize = 10);
};

class NetworkEventDetector
{
public:
    static QStringList detectHandoverEvents(const QStringList &messages);
    static QStringList detectConnectionEvents(const QStringList &messages);
    static QStringList detectDataTransferEvents(const QStringList &messages);
    static QStringList detectRRCStateChanges(const QStringList &messages);
};

#endif // LOGANALYSIS_H