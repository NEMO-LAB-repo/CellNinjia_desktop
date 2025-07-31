#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <memory>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMenuBar;
QT_END_NAMESPACE

class CellNinjiaCore;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToDevice();
    void disconnectFromDevice();
    void startMonitoring();
    void stopMonitoring();
    void selectOutputFile();
    void onModeChanged();
    void onMessageTypeChanged();
    void updateConnectionStatus();
    void displayReceivedData(const QString &data);
    void updateStatistics();

private:
    void setupUI();
    void setupMenuBar();
    void setupConnectionPanel();
    void setupMessageConfigPanel();
    void setupMonitoringPanel();
    void setupStatusPanel();
    void loadMessageTypes();
    void updateButtonStates();

    // UI Components
    QWidget *m_centralWidget;
    QTabWidget *m_tabWidget;
    
    // Connection Tab
    QWidget *m_connectionTab;
    QComboBox *m_portCombo;
    QComboBox *m_baudCombo;
    QPushButton *m_connectButton;
    QPushButton *m_disconnectButton;
    QLabel *m_connectionStatus;
    QProgressBar *m_connectionProgress;
    
    // Configuration Tab
    QWidget *m_configTab;
    QComboBox *m_modeCombo;
    QListWidget *m_messageTypeList;
    QLineEdit *m_outputFileEdit;
    QPushButton *m_browseButton;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    
    // Monitoring Tab
    QWidget *m_monitorTab;
    QTextEdit *m_logDisplay;
    QTableWidget *m_statisticsTable;
    QLabel *m_messagesReceived;
    QLabel *m_dataTransferred;
    QLabel *m_monitoringTime;
    
    // Menu and Status
    QMenuBar *m_menuBar;
    QStatusBar *m_statusBar;
    QTimer *m_updateTimer;
    
    // Backend integration
    std::unique_ptr<CellNinjiaCore> m_core;
    bool m_isConnected;
    bool m_isMonitoring;
    QString m_currentOutputFile;
};

#endif // MAINWINDOW_H