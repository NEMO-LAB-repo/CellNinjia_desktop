#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    struct ConnectionConfig {
        QString portName;
        qint32 baudRate;
        bool autoConnect;
    };

    struct MonitoringConfig {
        QString operatingMode; // "online" or "offline"
        QString outputFile;
        QStringList selectedMessageTypes;
        int maxDataPoints;
        bool autoSave;
        int autoSaveInterval; // minutes
    };

    struct DisplayConfig {
        bool showCharts;
        bool showStatistics;
        bool showRealTimeLog;
        QString logLevel;
        int maxLogLines;
        QString theme;
    };

    struct ExportConfig {
        QString defaultExportPath;
        QString defaultFormat;
        bool includeTimestamps;
        bool includeStatistics;
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

    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager();

    // Configuration management
    bool saveConfiguration(const CellNinjiaConfig &config, const QString &filePath = QString());
    bool loadConfiguration(CellNinjiaConfig &config, const QString &filePath);
    QStringList getRecentConfigurations() const;
    bool deleteConfiguration(const QString &filePath);

    // Preset configurations
    CellNinjiaConfig getDefaultConfiguration() const;
    CellNinjiaConfig get5GResearchConfiguration() const;
    CellNinjiaConfig getLTEMonitoringConfiguration() const;
    CellNinjiaConfig getDebugConfiguration() const;

    // Settings persistence
    void saveSettings(const CellNinjiaConfig &config);
    CellNinjiaConfig loadSettings();

    // Validation
    bool validateConfiguration(const CellNinjiaConfig &config, QString &errorMessage);

    // Import/Export
    bool exportToJson(const CellNinjiaConfig &config, const QString &filePath);
    bool importFromJson(CellNinjiaConfig &config, const QString &filePath);

signals:
    void configurationChanged(const CellNinjiaConfig &config);
    void recentConfigurationsUpdated();

private:
    void updateRecentConfigurations(const QString &filePath);
    QString getConfigDirectory() const;
    QJsonObject configToJson(const CellNinjiaConfig &config) const;
    CellNinjiaConfig jsonToConfig(const QJsonObject &json) const;

    QSettings *m_settings;
    static const int MAX_RECENT_CONFIGS = 10;
};

#endif // CONFIGMANAGER_H