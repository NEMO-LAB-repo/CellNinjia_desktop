#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QLegend>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <memory>

QT_CHARTS_USE_NAMESPACE

class ChartWidget : public QWidget
{
    Q_OBJECT

public:
    enum ChartType {
        MessageRate,        // Messages per second over time
        DataThroughput,    // Data throughput over time
        MessageTypes,      // Distribution of message types
        SignalStrength,    // Signal strength over time
        LayerDistribution  // Protocol layer distribution
    };

    explicit ChartWidget(ChartType type, QWidget *parent = nullptr);
    ~ChartWidget();

    void addDataPoint(qreal value, const QDateTime &timestamp = QDateTime::currentDateTime());
    void addMessageType(const QString &messageType, int count);
    void setSignalStrength(qreal strength, const QDateTime &timestamp = QDateTime::currentDateTime());
    void updateLayerDistribution(const QMap<QString, int> &layerCounts);
    void clearData();
    void setMaxDataPoints(int maxPoints);

public slots:
    void updateChart();
    void exportChart();

private:
    void setupChart();
    void setupMessageRateChart();
    void setupDataThroughputChart();
    void setupMessageTypesChart();
    void setupSignalStrengthChart();
    void setupLayerDistributionChart();
    void updateTimeRange();

    ChartType m_chartType;
    QChart *m_chart;
    QChartView *m_chartView;
    QTimer *m_updateTimer;
    
    // Line charts data
    QLineSeries *m_lineSeries;
    QAreaSeries *m_areaSeries;
    QValueAxis *m_axisY;
    QDateTimeAxis *m_axisX;
    
    // Bar chart data
    QBarSeries *m_barSeries;
    QCategoryAxis *m_categoriesAxis;
    
    // Data storage
    QList<QPair<QDateTime, qreal>> m_dataPoints;
    QMap<QString, int> m_messageTypeCounts;
    QMap<QString, int> m_layerCounts;
    
    int m_maxDataPoints;
    qreal m_maxValue;
    qreal m_minValue;
};

#endif // CHARTWIDGET_H