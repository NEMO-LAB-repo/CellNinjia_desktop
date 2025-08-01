#ifndef OFFLINEDECODERDIALOG_H
#define OFFLINEDECODERDIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>

class OfflineDecoderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OfflineDecoderDialog(QWidget *parent = nullptr);
    ~OfflineDecoderDialog();

private slots:
    void browseInputFile();
    void browseOutputFile();
    void onInputPathChanged();
    void onOutputPathChanged();
    void startDecoder();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void onProcessOutput();

private:
    void setupUI();
    void validateInputs();
    void updateButtonStates();
    bool validateFilePaths();
    void showProgress(bool show);
    void resetDialog();

    // UI Components
    QVBoxLayout *m_mainLayout;
    
    // File input section
    QGroupBox *m_inputGroup;
    QLineEdit *m_inputFileEdit;
    QPushButton *m_browseInputButton;
    
    // Output specification section
    QGroupBox *m_outputGroup;
    QLineEdit *m_outputFileEdit;
    QPushButton *m_browseOutputButton;
    
    // Action section
    QGroupBox *m_actionGroup;
    QPushButton *m_startButton;
    QPushButton *m_cancelButton;
    QPushButton *m_closeButton;
    
    // Progress and output section
    QGroupBox *m_progressGroup;
    QProgressBar *m_progressBar;
    QTextEdit *m_outputDisplay;
    QLabel *m_statusLabel;
    
    // Backend
    QProcess *m_decoderProcess;
    bool m_isProcessing;
};

#endif // OFFLINEDECODERDIALOG_H