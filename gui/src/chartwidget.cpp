#include "../include/chartwidget.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtCore/QStandardPaths>
#include <QtGui/QPainter>
#include <QtCharts/QBarCategoryAxis>

ChartWidget::ChartWidget(ChartType type, QWidget *parent)
    : QWidget(parent)
    , m_chartType(type)
    , m_chart(new QChart())
    , m_chartView(new QChartView(m_chart, this))
    , m_updateTimer(new QTimer(this))
    , m_lineSeries(nullptr)
    , m_areaSeries(nullptr)
    , m_axisY(nullptr)
    , m_axisX(nullptr)
    , m_barSeries(nullptr)
    , m_categoriesAxis(nullptr)
    , m_maxDataPoints(200)
    , m_maxValue(0)
    , m_minValue(0)
{
    setupChart();
    
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
    
    // Setup update timer
    connect(m_updateTimer, &QTimer::timeout, this, &ChartWidget::updateChart);
    m_updateTimer->setInterval(1000); // Update every second
    m_updateTimer->start();
}

ChartWidget::~ChartWidget() = default;

void ChartWidget::setupChart()
{
    m_chart->setTheme(QChart::ChartThemeBlueNms);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    
    switch (m_chartType) {
    case MessageRate:
        setupMessageRateChart();
        break;
    case DataThroughput:
        setupDataThroughputChart();
        break;
    case MessageTypes:
        setupMessageTypesChart();
        break;
    case SignalStrength:
        setupSignalStrengthChart();
        break;
    case LayerDistribution:
        setupLayerDistributionChart();
        break;
    }
}

void ChartWidget::setupMessageRateChart()
{
    m_chart->setTitle("📈 Message Rate (msg/sec)");
    
    m_lineSeries = new QLineSeries();
    m_lineSeries->setName("Messages/sec");
    m_lineSeries->setPen(QPen(QColor(33, 150, 243), 2));
    
    m_areaSeries = new QAreaSeries(m_lineSeries);
    m_areaSeries->setName("Message Rate");
    m_areaSeries->setBrush(QBrush(QColor(33, 150, 243, 50)));
    
    m_chart->addSeries(m_areaSeries);
    
    m_axisX = new QDateTimeAxis;
    m_axisX->setTickCount(10);
    m_axisX->setFormat("hh:mm:ss");
    m_axisX->setTitleText("Time");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_areaSeries->attachAxis(m_axisX);
    
    m_axisY = new QValueAxis;
    m_axisY->setLabelFormat("%i");
    m_axisY->setTitleText("Messages per Second");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_areaSeries->attachAxis(m_axisY);
}

void ChartWidget::setupDataThroughputChart()
{
    m_chart->setTitle("📊 Data Throughput (KB/sec)");
    
    m_lineSeries = new QLineSeries();
    m_lineSeries->setName("Throughput");
    m_lineSeries->setPen(QPen(QColor(255, 152, 0), 2));
    
    m_areaSeries = new QAreaSeries(m_lineSeries);
    m_areaSeries->setName("Data Throughput");
    m_areaSeries->setBrush(QBrush(QColor(255, 152, 0, 50)));
    
    m_chart->addSeries(m_areaSeries);
    
    m_axisX = new QDateTimeAxis;
    m_axisX->setTickCount(10);
    m_axisX->setFormat("hh:mm:ss");
    m_axisX->setTitleText("Time");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_areaSeries->attachAxis(m_axisX);
    
    m_axisY = new QValueAxis;
    m_axisY->setLabelFormat("%.1f");
    m_axisY->setTitleText("KB per Second");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_areaSeries->attachAxis(m_axisY);
}

void ChartWidget::setupMessageTypesChart()
{
    m_chart->setTitle("📋 Message Type Distribution");
    
    m_barSeries = new QBarSeries();
    m_chart->addSeries(m_barSeries);
    
    m_categoriesAxis = new QCategoryAxis();
    m_categoriesAxis->setTitleText("Message Types");
    m_chart->addAxis(m_categoriesAxis, Qt::AlignBottom);
    m_barSeries->attachAxis(m_categoriesAxis);
    
    m_axisY = new QValueAxis;
    m_axisY->setLabelFormat("%i");
    m_axisY->setTitleText("Message Count");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_barSeries->attachAxis(m_axisY);
}

void ChartWidget::setupSignalStrengthChart()
{
    m_chart->setTitle("📶 Signal Strength (dBm)");
    
    m_lineSeries = new QLineSeries();
    m_lineSeries->setName("Signal Strength");
    m_lineSeries->setPen(QPen(QColor(76, 175, 80), 3));
    
    m_chart->addSeries(m_lineSeries);
    
    m_axisX = new QDateTimeAxis;
    m_axisX->setTickCount(10);
    m_axisX->setFormat("hh:mm:ss");
    m_axisX->setTitleText("Time");
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_lineSeries->attachAxis(m_axisX);
    
    m_axisY = new QValueAxis;
    m_axisY->setLabelFormat("%i");
    m_axisY->setTitleText("Signal Strength (dBm)");
    m_axisY->setRange(-120, -30); // Typical cellular signal range
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_lineSeries->attachAxis(m_axisY);
}

void ChartWidget::setupLayerDistributionChart()
{
    m_chart->setTitle("🌐 Protocol Layer Distribution");
    
    m_barSeries = new QBarSeries();
    m_chart->addSeries(m_barSeries);
    
    m_categoriesAxis = new QCategoryAxis();
    m_categoriesAxis->setTitleText("Protocol Layers");
    m_chart->addAxis(m_categoriesAxis, Qt::AlignBottom);
    m_barSeries->attachAxis(m_categoriesAxis);
    
    m_axisY = new QValueAxis;
    m_axisY->setLabelFormat("%i");
    m_axisY->setTitleText("Message Count");
    m_chart->addAxis(m_axisY, Qt::AlignLeft);
    m_barSeries->attachAxis(m_axisY);
}

void ChartWidget::addDataPoint(qreal value, const QDateTime &timestamp)
{
    if (m_chartType == MessageTypes || m_chartType == LayerDistribution) {
        return; // These charts don't use time-series data
    }
    
    m_dataPoints.append(qMakePair(timestamp, value));
    
    // Keep only the latest data points
    while (m_dataPoints.size() > m_maxDataPoints) {
        m_dataPoints.removeFirst();
    }
    
    // Update min/max values
    m_maxValue = qMax(m_maxValue, value);
    m_minValue = qMin(m_minValue, value);
}

void ChartWidget::addMessageType(const QString &messageType, int count)
{
    if (m_chartType != MessageTypes && m_chartType != LayerDistribution) {
        return;
    }
    
    m_messageTypeCounts[messageType] = count;
}

void ChartWidget::setSignalStrength(qreal strength, const QDateTime &timestamp)
{
    if (m_chartType == SignalStrength) {
        addDataPoint(strength, timestamp);
    }
}

void ChartWidget::updateLayerDistribution(const QMap<QString, int> &layerCounts)
{
    if (m_chartType != LayerDistribution) {
        return;
    }
    
    m_layerCounts = layerCounts;
    updateChart();
}

void ChartWidget::clearData()
{
    m_dataPoints.clear();
    m_messageTypeCounts.clear();
    m_layerCounts.clear();
    m_maxValue = 0;
    m_minValue = 0;
    
    if (m_lineSeries) {
        m_lineSeries->clear();
    }
    if (m_barSeries) {
        m_barSeries->clear();
    }
}

void ChartWidget::setMaxDataPoints(int maxPoints)
{
    m_maxDataPoints = maxPoints;
}

void ChartWidget::updateChart()
{
    switch (m_chartType) {
    case MessageRate:
    case DataThroughput:
    case SignalStrength:
        updateTimeRange();
        break;
    case MessageTypes:
        updateMessageTypesChart();
        break;
    case LayerDistribution:
        updateLayerDistributionChart();
        break;
    }
}

void ChartWidget::updateTimeRange()
{
    if (!m_lineSeries || m_dataPoints.isEmpty()) {
        return;
    }
    
    m_lineSeries->clear();
    
    for (const auto &point : m_dataPoints) {
        m_lineSeries->append(point.first.toMSecsSinceEpoch(), point.second);
    }
    
    // Update axis ranges
    if (m_axisX && !m_dataPoints.isEmpty()) {
        QDateTime minTime = m_dataPoints.first().first;
        QDateTime maxTime = m_dataPoints.last().first;
        m_axisX->setRange(minTime, maxTime);
    }
    
    if (m_axisY && m_maxValue > m_minValue) {
        qreal margin = (m_maxValue - m_minValue) * 0.1;
        m_axisY->setRange(m_minValue - margin, m_maxValue + margin);
    }
}

void ChartWidget::updateMessageTypesChart()
{
    if (!m_barSeries || m_messageTypeCounts.isEmpty()) {
        return;
    }
    
    m_barSeries->clear();
    m_categoriesAxis->clear();
    
    auto *barSet = new QBarSet("Messages");
    barSet->setColor(QColor(33, 150, 243));
    
    int index = 0;
    for (auto it = m_messageTypeCounts.begin(); it != m_messageTypeCounts.end(); ++it) {
        QString shortName = it.key().split(' ').last(); // Use last word for shorter labels
        m_categoriesAxis->append(shortName, index);
        *barSet << it.value();
        index++;
    }
    
    m_barSeries->append(barSet);
    
    if (m_axisY) {
        int maxCount = 0;
        for (int count : m_messageTypeCounts.values()) {
            maxCount = qMax(maxCount, count);
        }
        m_axisY->setRange(0, maxCount * 1.1);
    }
}

void ChartWidget::updateLayerDistributionChart()
{
    if (!m_barSeries || m_layerCounts.isEmpty()) {
        return;
    }
    
    m_barSeries->clear();
    m_categoriesAxis->clear();
    
    auto *barSet = new QBarSet("Layer Messages");
    
    // Define colors for different layers
    QList<QColor> layerColors = {
        QColor(244, 67, 54),   // Red - NAS
        QColor(33, 150, 243),  // Blue - RRC
        QColor(255, 152, 0),   // Orange - PDCP
        QColor(76, 175, 80),   // Green - RLC
        QColor(156, 39, 176),  // Purple - MAC
        QColor(96, 125, 139)   // Blue Grey - Physical
    };
    
    int index = 0;
    for (auto it = m_layerCounts.begin(); it != m_layerCounts.end(); ++it) {
        m_categoriesAxis->append(it.key(), index);
        *barSet << it.value();
        if (index < layerColors.size()) {
            barSet->setColor(layerColors[index]);
        }
        index++;
    }
    
    m_barSeries->append(barSet);
    
    if (m_axisY) {
        int maxCount = 0;
        for (int count : m_layerCounts.values()) {
            maxCount = qMax(maxCount, count);
        }
        m_axisY->setRange(0, maxCount * 1.1);
    }
}

void ChartWidget::exportChart()
{
    QString fileName = QFileDialog::getSaveFileName(this, 
        "Export Chart", 
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/chart.png",
        "PNG files (*.png);;PDF files (*.pdf);;SVG files (*.svg)");
    
    if (!fileName.isEmpty()) {
        QPixmap pixmap = m_chartView->grab();
        pixmap.save(fileName);
    }
}