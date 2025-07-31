#include "../include/loganalysis.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QMutexLocker>
#include <QtCore/QRegularExpression>
#include <QtCore/QDateTime>
#include <algorithm>
#include <map>
#include <set>

// LogAnalysisWorker Implementation
LogAnalysisWorker::LogAnalysisWorker(QObject *parent)
    : QObject(parent)
    , m_stopRequested(false)
{
}

void LogAnalysisWorker::setLogFile(const QString &filePath)
{
    m_logFilePath = filePath;
    m_messages.clear();
    m_messageTypes.clear();
}

void LogAnalysisWorker::setRealTimeData(const QStringList &messages, const QStringList &messageTypes)
{
    m_messages = messages;
    m_messageTypes = messageTypes;
    m_logFilePath.clear();
}

void LogAnalysisWorker::startAnalysis()
{
    QMutexLocker locker(&m_mutex);
    m_stopRequested = false;
    m_result = LogAnalysisWidget::AnalysisResult();
    
    try {
        emit progress(10);
        
        // Load data if from file
        if (!m_logFilePath.isEmpty()) {
            QFile file(m_logFilePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                emit error("Could not open log file: " + m_logFilePath);
                return;
            }
            
            QTextStream in(&file);
            m_messages.clear();
            m_messageTypes.clear();
            
            while (!in.atEnd() && !m_stopRequested) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty()) {
                    m_messages.append(line);
                    m_messageTypes.append(extractMessageType(line));
                }
                
                if (m_messages.size() % 1000 == 0) {
                    int progress = qMin(50, (m_messages.size() / 1000) * 5);
                    emit this->progress(10 + progress);
                }
            }
        }
        
        if (m_stopRequested) {
            emit error("Analysis was cancelled");
            return;
        }
        
        emit progress(60);
        
        // Basic analysis
        analyzeMessages();
        
        if (m_stopRequested) return;
        emit progress(70);
        
        // Pattern detection
        detectPatterns();
        
        if (m_stopRequested) return;
        emit progress(80);
        
        // Anomaly detection
        detectAnomalies();
        
        if (m_stopRequested) return;
        emit progress(90);
        
        // Network events
        analyzeNetworkEvents();
        
        if (m_stopRequested) return;
        emit progress(95);
        
        // Calculate final statistics
        calculateStatistics();
        
        emit progress(100);
        emit finished(m_result);
        
    } catch (const std::exception &e) {
        emit error(QString("Analysis failed: %1").arg(e.what()));
    }
}

void LogAnalysisWorker::stopAnalysis()
{
    QMutexLocker locker(&m_mutex);
    m_stopRequested = true;
}

void LogAnalysisWorker::analyzeMessages()
{
    if (m_messages.isEmpty()) {
        return;
    }
    
    m_result.totalMessages = m_messages.size();
    m_result.analysisStartTime = QDateTime::currentDateTime();
    
    // Count message types
    std::map<QString, int> typeCounts;
    std::set<QString> uniqueTypes;
    
    for (int i = 0; i < m_messages.size() && !m_stopRequested; ++i) {
        const QString &message = m_messages[i];
        QString messageType = (i < m_messageTypes.size()) ? m_messageTypes[i] : extractMessageType(message);
        
        typeCounts[messageType]++;
        uniqueTypes.insert(messageType);
        
        // Count errors and warnings
        if (isErrorMessage(message)) {
            m_result.errorMessages++;
        } else if (isWarningMessage(message)) {
            m_result.warningMessages++;
        }
        
        // Extract timestamp for temporal analysis
        QDateTime timestamp = extractTimestamp(message);
        if (timestamp.isValid()) {
            // Group by hour for rate calculation
            QDateTime hourKey = QDateTime(timestamp.date(), QTime(timestamp.time().hour(), 0));
            m_result.messageRateOverTime[hourKey]++;
            
            // Calculate data throughput (approximate)
            int size = extractMessageSize(message);
            m_result.dataThroughputOverTime[hourKey] += size / 1024.0; // KB
        }
    }
    
    // Convert to Qt containers
    for (const auto &pair : typeCounts) {
        m_result.messageTypeCounts[pair.first] = pair.second;
        
        // Calculate percentages
        qreal percentage = (qreal(pair.second) / m_result.totalMessages) * 100.0;
        m_result.messageTypePercentages[pair.first] = percentage;
        
        // Group by protocol layer
        QString layer = extractProtocolLayer(pair.first);
        m_result.layerCounts[layer] += pair.second;
    }
    
    m_result.uniqueMessageTypes = uniqueTypes.size();
    
    // Calculate layer percentages
    for (auto it = m_result.layerCounts.begin(); it != m_result.layerCounts.end(); ++it) {
        qreal percentage = (qreal(it.value()) / m_result.totalMessages) * 100.0;
        m_result.layerPercentages[it.key()] = percentage;
    }
}

void LogAnalysisWorker::detectPatterns()
{
    if (m_messages.size() < 3) {
        return; // Need at least 3 messages for pattern detection
    }
    
    // Simple pattern detection - look for repeated sequences
    std::map<QString, int> patternCounts;
    const int windowSize = 3;
    
    for (int i = 0; i <= m_messages.size() - windowSize && !m_stopRequested; ++i) {
        QStringList window;
        for (int j = 0; j < windowSize; ++j) {
            QString msgType = (i + j < m_messageTypes.size()) ? 
                             m_messageTypes[i + j] : 
                             extractMessageType(m_messages[i + j]);
            window.append(msgType);
        }
        
        QString pattern = window.join(" -> ");
        patternCounts[pattern]++;
    }
    
    // Extract frequent patterns (support >= 3)
    for (const auto &pair : patternCounts) {
        if (pair.second >= 3) {
            m_result.frequentPatterns.append(QString("%1 (support: %2)")
                                           .arg(pair.first).arg(pair.second));
        }
    }
    
    // Sort by frequency
    std::sort(m_result.frequentPatterns.begin(), m_result.frequentPatterns.end(),
              [](const QString &a, const QString &b) {
                  QRegularExpression re("support: (\\d+)");
                  auto matchA = re.match(a);
                  auto matchB = re.match(b);
                  if (matchA.hasMatch() && matchB.hasMatch()) {
                      return matchA.captured(1).toInt() > matchB.captured(1).toInt();
                  }
                  return false;
              });
    
    // Keep only top 10
    if (m_result.frequentPatterns.size() > 10) {
        m_result.frequentPatterns = m_result.frequentPatterns.mid(0, 10);
    }
    
    // Detect message sequences
    analyzeTemporalData();
}

void LogAnalysisWorker::detectAnomalies()
{
    // Statistical anomaly detection
    
    // 1. Detect unusual message type frequencies
    qreal avgCount = qreal(m_result.totalMessages) / m_result.uniqueMessageTypes;
    qreal threshold = avgCount * 3; // Messages appearing 3x more than average
    
    for (auto it = m_result.messageTypeCounts.begin(); it != m_result.messageTypeCounts.end(); ++it) {
        if (it.value() > threshold) {
            m_result.anomalies.append(QString("High frequency: %1 (%2 occurrences)")
                                    .arg(it.key()).arg(it.value()));
        }
    }
    
    // 2. Detect unusual error rates
    if (m_result.errorRate > 10.0) {
        m_result.anomalies.append(QString("High error rate: %1%").arg(m_result.errorRate, 0, 'f', 2));
    }
    
    // 3. Detect gaps in message timeline
    if (!m_result.messageRateOverTime.isEmpty()) {
        auto times = m_result.messageRateOverTime.keys();
        std::sort(times.begin(), times.end());
        
        for (int i = 1; i < times.size(); ++i) {
            qint64 gap = times[i-1].msecsTo(times[i]);
            if (gap > 3600000) { // Gap > 1 hour
                m_result.anomalies.append(QString("Large time gap: %1 to %2")
                                        .arg(times[i-1].toString())
                                        .arg(times[i].toString()));
            }
        }
    }
    
    // 4. Detect unusual message patterns
    for (const QString &message : m_messages) {
        if (m_stopRequested) break;
        
        // Look for unusual patterns
        if (message.contains(QRegularExpression("\\b(FATAL|CRITICAL|PANIC)\\b", QRegularExpression::CaseInsensitiveOption))) {
            m_result.anomalies.append("Critical error detected: " + message.left(100));
        }
        
        // Look for repeated identical messages (potential spam)
        // This is a simplified check - in practice, you'd want more sophisticated detection
    }
    
    // Limit anomalies to top 20
    if (m_result.anomalies.size() > 20) {
        m_result.anomalies = m_result.anomalies.mid(0, 20);
    }
}

void LogAnalysisWorker::calculateStatistics()
{
    m_result.analysisEndTime = QDateTime::currentDateTime();
    m_result.analysisDuration = m_result.analysisStartTime.msecsTo(m_result.analysisEndTime);
    
    // Calculate error rate
    if (m_result.totalMessages > 0) {
        m_result.errorRate = (qreal(m_result.errorMessages) / m_result.totalMessages) * 100.0;
    }
    
    // Calculate message rates
    if (!m_result.messageRateOverTime.isEmpty()) {
        qreal sum = 0;
        int maxRate = 0;
        
        for (auto it = m_result.messageRateOverTime.begin(); it != m_result.messageRateOverTime.end(); ++it) {
            sum += it.value();
            maxRate = qMax(maxRate, it.value());
        }
        
        m_result.averageMessageRate = sum / m_result.messageRateOverTime.size();
        m_result.peakMessageRate = maxRate;
    }
    
    // Calculate data throughput
    if (!m_result.dataThroughputOverTime.isEmpty()) {
        qreal sum = 0;
        qreal maxThroughput = 0;
        
        for (auto it = m_result.dataThroughputOverTime.begin(); it != m_result.dataThroughputOverTime.end(); ++it) {
            sum += it.value();
            maxThroughput = qMax(maxThroughput, it.value());
        }
        
        m_result.averageDataThroughput = sum / m_result.dataThroughputOverTime.size();
        m_result.peakDataThroughput = maxThroughput;
    }
    
    // Extract top errors
    std::map<QString, int> errorCounts;
    for (const QString &message : m_messages) {
        if (isErrorMessage(message)) {
            // Extract error type (simplified)
            QRegularExpression re("(Error|ERROR|Fail|FAIL)\\s*:?\\s*([^\\n]+)");
            auto match = re.match(message);
            if (match.hasMatch()) {
                QString errorType = match.captured(2).left(50); // First 50 chars
                errorCounts[errorType]++;
            }
        }
    }
    
    // Convert to sorted list
    std::vector<std::pair<QString, int>> errorVector(errorCounts.begin(), errorCounts.end());
    std::sort(errorVector.begin(), errorVector.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });
    
    for (const auto &pair : errorVector) {
        m_result.topErrors.append(QString("%1 (%2 times)").arg(pair.first).arg(pair.second));
        if (m_result.topErrors.size() >= 10) break;
    }
}

void LogAnalysisWorker::analyzeTemporalData()
{
    // Group messages by time windows for sequence analysis
    std::map<QDateTime, QStringList> timeWindows;
    
    for (int i = 0; i < m_messages.size() && !m_stopRequested; ++i) {
        QDateTime timestamp = extractTimestamp(m_messages[i]);
        if (timestamp.isValid()) {
            // Group by 10-minute windows
            QDateTime windowKey = QDateTime(timestamp.date(), 
                                          QTime(timestamp.time().hour(), 
                                                (timestamp.time().minute() / 10) * 10));
            
            QString msgType = (i < m_messageTypes.size()) ? m_messageTypes[i] : extractMessageType(m_messages[i]);
            timeWindows[windowKey].append(msgType);
        }
    }
    
    // Analyze sequences within time windows
    for (const auto &window : timeWindows) {
        if (window.second.size() >= 3) {
            QString sequence = window.second.join(" -> ");
            if (sequence.length() < 200) { // Avoid very long sequences
                m_result.messageSequences[window.first.toString()].append(sequence);
            }
        }
    }
}

void LogAnalysisWorker::analyzeNetworkEvents()
{
    m_result.handoverEvents = NetworkEventDetector::detectHandoverEvents(m_messages);
    m_result.connectionEvents = NetworkEventDetector::detectConnectionEvents(m_messages);
    m_result.dataTransferEvents = NetworkEventDetector::detectDataTransferEvents(m_messages);
}

QString LogAnalysisWorker::extractMessageType(const QString &message)
{
    // Simple message type extraction - look for known patterns
    QRegularExpression re("(NR5G\\s+\\w+(?:\\s+\\w+)*)");
    auto match = re.match(message);
    if (match.hasMatch()) {
        return match.captured(1);
    }
    
    // Look for other patterns
    if (message.contains("Error", Qt::CaseInsensitive)) return "Error Message";
    if (message.contains("Warning", Qt::CaseInsensitive)) return "Warning Message";
    if (message.contains("Info", Qt::CaseInsensitive)) return "Info Message";
    
    return "Unknown Message Type";
}

QString LogAnalysisWorker::extractProtocolLayer(const QString &messageType)
{
    if (messageType.contains("NAS", Qt::CaseInsensitive)) return "NAS";
    if (messageType.contains("RRC", Qt::CaseInsensitive)) return "RRC";
    if (messageType.contains("PDCP", Qt::CaseInsensitive)) return "PDCP";
    if (messageType.contains("RLC", Qt::CaseInsensitive)) return "RLC";
    if (messageType.contains("MAC", Qt::CaseInsensitive)) return "MAC";
    if (messageType.contains("LL1", Qt::CaseInsensitive) || messageType.contains("ML1", Qt::CaseInsensitive)) return "Physical";
    return "Unknown";
}

QDateTime LogAnalysisWorker::extractTimestamp(const QString &message)
{
    // Try different timestamp formats
    QStringList patterns = {
        "\\d{4}-\\d{2}-\\d{2}\\s+\\d{2}:\\d{2}:\\d{2}",           // 2023-01-01 12:34:56
        "\\d{2}/\\d{2}/\\d{4}\\s+\\d{2}:\\d{2}:\\d{2}",           // 01/01/2023 12:34:56
        "\\d{2}:\\d{2}:\\d{2}",                                    // 12:34:56
        "\\[\\d{4}-\\d{2}-\\d{2}\\s+\\d{2}:\\d{2}:\\d{2}\\]"      // [2023-01-01 12:34:56]
    };
    
    for (const QString &pattern : patterns) {
        QRegularExpression re(pattern);
        auto match = re.match(message);
        if (match.hasMatch()) {
            QString timestampStr = match.captured(0);
            timestampStr = timestampStr.remove('[').remove(']'); // Remove brackets if present
            
            // Try different formats
            QDateTime dt = QDateTime::fromString(timestampStr, "yyyy-MM-dd hh:mm:ss");
            if (dt.isValid()) return dt;
            
            dt = QDateTime::fromString(timestampStr, "MM/dd/yyyy hh:mm:ss");
            if (dt.isValid()) return dt;
            
            // If only time, use current date
            if (timestampStr.contains(QRegularExpression("^\\d{2}:\\d{2}:\\d{2}$"))) {
                QTime time = QTime::fromString(timestampStr, "hh:mm:ss");
                if (time.isValid()) {
                    return QDateTime(QDate::currentDate(), time);
                }
            }
        }
    }
    
    return QDateTime(); // Invalid datetime
}

int LogAnalysisWorker::extractMessageSize(const QString &message)
{
    // Estimate message size based on content length
    // In a real implementation, you might parse actual size fields
    return message.length();
}

bool LogAnalysisWorker::isErrorMessage(const QString &message)
{
    return message.contains(QRegularExpression("\\b(Error|ERROR|Fail|FAIL|Exception|EXCEPTION)\\b"));
}

bool LogAnalysisWorker::isWarningMessage(const QString &message)
{
    return message.contains(QRegularExpression("\\b(Warning|WARNING|Warn|WARN)\\b"));
}

// PatternDetector Implementation
QStringList PatternDetector::detectFrequentPatterns(const QStringList &messages, int minSupport)
{
    std::map<QString, int> patternCounts;
    
    // Simple n-gram pattern detection
    for (int n = 2; n <= 4; ++n) { // 2-gram to 4-gram
        for (int i = 0; i <= messages.size() - n; ++i) {
            QStringList ngram;
            for (int j = 0; j < n; ++j) {
                ngram.append(messages[i + j]);
            }
            QString pattern = ngram.join(" ");
            patternCounts[pattern]++;
        }
    }
    
    QStringList frequentPatterns;
    for (const auto &pair : patternCounts) {
        if (pair.second >= minSupport) {
            frequentPatterns.append(QString("%1 (support: %2)").arg(pair.first).arg(pair.second));
        }
    }
    
    return frequentPatterns;
}

QStringList PatternDetector::detectAnomalies(const QStringList &messages, const QMap<QString, int> &typeCounts)
{
    QStringList anomalies;
    
    // Statistical anomaly detection based on message frequency
    int totalMessages = 0;
    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        totalMessages += it.value();
    }
    
    if (totalMessages == 0) return anomalies;
    
    qreal avgFrequency = qreal(totalMessages) / typeCounts.size();
    qreal threshold = avgFrequency * 3; // 3 times average
    
    for (auto it = typeCounts.begin(); it != typeCounts.end(); ++it) {
        if (it.value() > threshold) {
            anomalies.append(QString("High frequency anomaly: %1 (%2 occurrences)")
                           .arg(it.key()).arg(it.value()));
        }
    }
    
    return anomalies;
}

QMap<QString, QStringList> PatternDetector::detectMessageSequences(const QStringList &messages, int windowSize)
{
    QMap<QString, QStringList> sequences;
    
    for (int i = 0; i <= messages.size() - windowSize; ++i) {
        QStringList window;
        for (int j = 0; j < windowSize; ++j) {
            window.append(messages[i + j]);
        }
        
        QString key = QString("Window_%1").arg(i);
        sequences[key] = window;
        
        // Limit to avoid too many sequences
        if (sequences.size() > 100) break;
    }
    
    return sequences;
}

// NetworkEventDetector Implementation
QStringList NetworkEventDetector::detectHandoverEvents(const QStringList &messages)
{
    QStringList events;
    
    QRegularExpression handoverRegex("\\b(Handover|HO|Mobility|Cell\\s+Change)\\b", QRegularExpression::CaseInsensitiveOption);
    
    for (const QString &message : messages) {
        if (handoverRegex.match(message).hasMatch()) {
            events.append(message.left(100)); // First 100 characters
        }
        
        if (events.size() >= 50) break; // Limit results
    }
    
    return events;
}

QStringList NetworkEventDetector::detectConnectionEvents(const QStringList &messages)
{
    QStringList events;
    
    QRegularExpression connectionRegex("\\b(Connect|Disconnect|Setup|Release|Establish)\\b", QRegularExpression::CaseInsensitiveOption);
    
    for (const QString &message : messages) {
        if (connectionRegex.match(message).hasMatch()) {
            events.append(message.left(100));
        }
        
        if (events.size() >= 50) break;
    }
    
    return events;
}

QStringList NetworkEventDetector::detectDataTransferEvents(const QStringList &messages)
{
    QStringList events;
    
    QRegularExpression dataRegex("\\b(Data|Transfer|Transmit|Receive|Upload|Download)\\b", QRegularExpression::CaseInsensitiveOption);
    
    for (const QString &message : messages) {
        if (dataRegex.match(message).hasMatch()) {
            events.append(message.left(100));
        }
        
        if (events.size() >= 50) break;
    }
    
    return events;
}

QStringList NetworkEventDetector::detectRRCStateChanges(const QStringList &messages)
{
    QStringList events;
    
    QRegularExpression rrcRegex("\\bRRC\\s+(State|Connection|Setup|Release|Reconfiguration)\\b", QRegularExpression::CaseInsensitiveOption);
    
    for (const QString &message : messages) {
        if (rrcRegex.match(message).hasMatch()) {
            events.append(message.left(100));
        }
        
        if (events.size() >= 50) break;
    }
    
    return events;
}