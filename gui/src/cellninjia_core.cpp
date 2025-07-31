#include "../include/cellninjia_core.h"
#include "../../src/decode_preprocess.h" // Include existing decode functions
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

CellNinjiaCore::CellNinjiaCore(QObject *parent)
    : QObject(parent)
    , m_serialPort(std::make_unique<QSerialPort>(this))
    , m_mode(OperatingMode::Online)
    , m_monitoring(false)
    , m_messagesReceived(0)
    , m_bytesTransferred(0)
    , m_monitoringStartTime(0)
{
    // Connect serial port signals
    connect(m_serialPort.get(), &QSerialPort::readyRead, 
            this, &CellNinjiaCore::handleSerialData);
    connect(m_serialPort.get(), &QSerialPort::errorOccurred,
            this, &CellNinjiaCore::handleSerialError);
}

CellNinjiaCore::~CellNinjiaCore()
{
    if (m_monitoring) {
        stopMonitoring();
    }
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

bool CellNinjiaCore::connectToDevice(const QString &portName, qint32 baudRate)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        emit connected();
        return true;
    } else {
        emit connectionError(m_serialPort->errorString());
        return false;
    }
}

void CellNinjiaCore::disconnectFromDevice()
{
    if (m_monitoring) {
        stopMonitoring();
    }
    
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
        emit disconnected();
    }
}

bool CellNinjiaCore::isConnected() const
{
    return m_serialPort && m_serialPort->isOpen();
}

void CellNinjiaCore::setOperatingMode(OperatingMode mode)
{
    m_mode = mode;
}

void CellNinjiaCore::setOutputFile(const QString &filePath)
{
    m_outputFile = filePath;
}

void CellNinjiaCore::setMessageTypes(const QStringList &messageTypes)
{
    m_messageTypes = messageTypes;
}

bool CellNinjiaCore::startMonitoring()
{
    if (!isConnected()) {
        return false;
    }
    
    if (m_outputFile.isEmpty()) {
        return false;
    }
    
    // Generate diagnostic configuration based on selected message types
    if (!generateDiagConfig()) {
        return false;
    }
    
    m_monitoring = true;
    m_monitoringStartTime = QDateTime::currentMSecsSinceEpoch();
    m_messagesReceived = 0;
    m_bytesTransferred = 0;
    
    return true;
}

void CellNinjiaCore::stopMonitoring()
{
    m_monitoring = false;
}

bool CellNinjiaCore::isMonitoring() const
{
    return m_monitoring;
}

qint64 CellNinjiaCore::getMessagesReceived() const
{
    return m_messagesReceived;
}

qint64 CellNinjiaCore::getBytesTransferred() const
{
    return m_bytesTransferred;
}

qint64 CellNinjiaCore::getMonitoringDuration() const
{
    if (m_monitoringStartTime == 0) {
        return 0;
    }
    return QDateTime::currentMSecsSinceEpoch() - m_monitoringStartTime;
}

void CellNinjiaCore::handleSerialData()
{
    if (!m_monitoring) {
        return;
    }
    
    QByteArray data = m_serialPort->readAll();
    if (data.isEmpty()) {
        return;
    }
    
    m_bytesTransferred += data.size();
    emit dataReceived(data);
    
    processReceivedData(data);
    emit statisticsUpdated();
}

void CellNinjiaCore::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        emit connectionError(m_serialPort->errorString());
    }
}

void CellNinjiaCore::processReceivedData(const QByteArray &data)
{
    // Add to receive buffer
    m_receiveBuffer.append(data);
    
    if (m_mode == OperatingMode::Offline) {
        // For offline mode, just write raw data to file
        QFile file(m_outputFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(data);
            file.close();
        }
    } else {
        // For online mode, process and decode messages
        // This is where we would integrate with the existing decode_preprocess functions
        
        // TODO: Integrate with existing enter_binary() and decode() functions
        // For now, simulate message processing
        
        static int messageCounter = 0;
        if (m_receiveBuffer.size() >= 64) { // Minimum message size
            messageCounter++;
            m_messagesReceived++;
            
            // Simulate decoded message
            QString messageType = m_messageTypes.isEmpty() ? "Unknown" : 
                                 m_messageTypes[messageCounter % m_messageTypes.size()];
            QString decodedData = QString("Message #%1: %2 bytes received at %3")
                                 .arg(messageCounter)
                                 .arg(data.size())
                                 .arg(QDateTime::currentDateTime().toString());
            
            emit messageDecoded(messageType, decodedData);
            
            // Write decoded data to file
            QFile file(m_outputFile);
            if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                QTextStream stream(&file);
                stream << decodedData << "\n";
                file.close();
            }
            
            // Clear processed data from buffer
            m_receiveBuffer = m_receiveBuffer.mid(64);
        }
    }
}

bool CellNinjiaCore::generateDiagConfig()
{
    // This function should integrate with the existing generate_diag_config function
    // from decode_preprocess.cpp
    
    if (m_messageTypes.isEmpty()) {
        return false;
    }
    
    // Convert QStringList to const char* array for existing function
    std::vector<const char*> messageTypesCStr;
    QList<QByteArray> messageTypesBA;
    
    for (const QString &msgType : m_messageTypes) {
        messageTypesBA.append(msgType.toUtf8());
        messageTypesCStr.push_back(messageTypesBA.last().constData());
    }
    
    // TODO: Call existing generate_diag_config function
    // bool success = generate_diag_config(m_serialPort->handle(), 
    //                                     messageTypesCStr.data(), 
    //                                     messageTypesCStr.size());
    
    // For now, simulate successful config generation
    qDebug() << "Generated diagnostic configuration for" << m_messageTypes.size() << "message types";
    return true;
}