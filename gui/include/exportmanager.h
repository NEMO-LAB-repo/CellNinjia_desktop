#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QJsonObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressDialog>
#include "loganalysis.h"

class ExportWorker;

class ExportManager : public QObject
{
    Q_OBJECT

public:
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
        QString compressionFormat; // "zip", "gzip"
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

    explicit ExportManager(QObject *parent = nullptr);
    ~ExportManager();

    // Export operations
    bool exportData(const ExportData &data, const ExportOptions &options);
    bool exportToFile(const QString &filePath, const ExportData &data, const ExportOptions &options);
    
    // Async export
    void exportDataAsync(const ExportData &data, const ExportOptions &options);
    
    // Template management
    bool saveTemplate(const QString &templateName, const QString &templateContent);
    QString loadTemplate(const QString &templateName);
    QStringList getAvailableTemplates();
    bool deleteTemplate(const QString &templateName);
    
    // Format validation
    bool validateExportOptions(const ExportOptions &options, QString &errorMessage);
    QStringList getSupportedFormats();
    QString getDefaultExtension(ExportFormat format);
    
    // Utility functions
    static QString formatSizeToString(qint64 size);
    static QString formatDurationToString(qint64 milliseconds);
    static ExportFormat formatFromString(const QString &formatString);
    static QString formatToString(ExportFormat format);

signals:
    void exportStarted();
    void exportProgress(int percentage, const QString &status);
    void exportCompleted(const QString &filePath);
    void exportError(const QString &error);

private slots:
    void onExportProgress(int percentage, const QString &status);
    void onExportCompleted(const QString &filePath);
    void onExportError(const QString &error);

private:
    bool exportToPlainText(const ExportData &data, const ExportOptions &options, const QString &filePath);
    bool exportToCSV(const ExportData &data, const ExportOptions &options, const QString &filePath);
    bool exportToJSON(const ExportData &data, const ExportOptions &options, const QString &filePath);
    bool exportToXML(const ExportData &data, const ExportOptions &options, const QString &filePath);
    bool exportToHTML(const ExportData &data, const ExportOptions &options, const QString &filePath);
    bool exportToPDF(const ExportData &data, const ExportOptions &options, const QString &filePath);
    bool exportToExcel(const ExportData &data, const ExportOptions &options, const QString &filePath);
    
    QString generateHTMLReport(const ExportData &data, const ExportOptions &options);
    QString generateXMLContent(const ExportData &data, const ExportOptions &options);
    QJsonObject generateJSONContent(const ExportData &data, const ExportOptions &options);
    
    bool compressFile(const QString &filePath, const QString &compressionFormat);
    QString getTemplatesDirectory() const;

    ExportWorker *m_worker;
    QThread *m_workerThread;
};

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();

    ExportManager::ExportOptions getExportOptions() const;
    void setExportData(const ExportManager::ExportData &data);

signals:
    void exportRequested(const ExportManager::ExportOptions &options);

private slots:
    void onFormatChanged();
    void onTypeChanged();
    void onBrowseOutputPath();
    void onPreviewExport();
    void onStartExport();
    void onTemplateChanged();
    void onSaveTemplate();
    void onLoadTemplate();

private:
    void setupUI();
    void setupFormatOptions();
    void setupContentOptions();
    void setupFilterOptions();
    void setupAdvancedOptions();
    void updatePreview();
    
    // UI Components - will be implemented in the .cpp file
    class Private;
    Private *d;
};

class ExportWorker : public QObject
{
    Q_OBJECT

public:
    explicit ExportWorker(QObject *parent = nullptr);
    
    void setExportData(const ExportManager::ExportData &data);
    void setExportOptions(const ExportManager::ExportOptions &options);

public slots:
    void startExport();
    void cancelExport();

signals:
    void progress(int percentage, const QString &status);
    void finished(const QString &filePath);
    void error(const QString &error);

private:
    void performExport();
    void updateProgress(int percentage, const QString &status);
    
    ExportManager::ExportData m_data;
    ExportManager::ExportOptions m_options;
    bool m_cancelled;
    QMutex m_mutex;
};

// Template engine for custom export formats
class ExportTemplateEngine
{
public:
    static QString processTemplate(const QString &templateContent, 
                                  const ExportManager::ExportData &data,
                                  const ExportManager::ExportOptions &options);
    
    static QStringList getAvailableVariables();
    static QString getVariableDescription(const QString &variable);
    
private:
    static QString replaceVariables(const QString &content, const QMap<QString, QString> &variables);
    static QMap<QString, QString> buildVariableMap(const ExportManager::ExportData &data,
                                                   const ExportManager::ExportOptions &options);
};

#endif // EXPORTMANAGER_H