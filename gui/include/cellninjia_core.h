#ifndef CELLNINJIA_CORE_H
#define CELLNINJIA_CORE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtSerialPort/QSerialPort>
#include <memory>

class CellNinjiaCore : public QObject
{
    Q_OBJECT

public:
    enum class OperatingMode {
        Online,
        Offline
    };

    explicit CellNinjiaCore(QObject *parent = nullptr);
    ~CellNinjiaCore();

    // Connection management
    bool connectToDevice(const QString &portName, qint32 baudRate);
    void disconnectFromDevice();
    bool isConnected() const;

    // Configuration
    void setOperatingMode(OperatingMode mode);
    void setOutputFile(const QString &filePath);
    void setMessageTypes(const QStringList &messageTypes);

    // Monitoring control
    bool startMonitoring();
    void stopMonitoring();
    bool isMonitoring() const;

    // Statistics
    qint64 getMessagesReceived() const;
    qint64 getBytesTransferred() const;
    qint64 getMonitoringDuration() const;

signals:
    void connected();
    void disconnected();
    void connectionError(const QString &error);
    void dataReceived(const QByteArray &data);
    void messageDecoded(const QString &messageType, const QString &decodedData);
    void statisticsUpdated();

private slots:
    void handleSerialData();
    void handleSerialError(QSerialPort::SerialPortError error);

private:
    void processReceivedData(const QByteArray &data);
    bool generateDiagConfig();

    std::unique_ptr<QSerialPort> m_serialPort;
    OperatingMode m_mode;
    QString m_outputFile;
    QStringList m_messageTypes;
    bool m_monitoring;
    
    // Statistics
    qint64 m_messagesReceived;
    qint64 m_bytesTransferred;
    qint64 m_monitoringStartTime;
    
    // Buffer for incomplete messages
    QByteArray m_receiveBuffer;
};

#endif // CELLNINJIA_CORE_H