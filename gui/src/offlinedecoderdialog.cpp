#include "../include/offlinedecoderdialog.h"
#include <QtCore/QStandardPaths>
#include <QtCore/QTimer>

OfflineDecoderDialog::OfflineDecoderDialog(QWidget *parent)
    : QDialog(parent)
    , m_mainLayout(nullptr)
    , m_inputGroup(nullptr)
    , m_inputFileEdit(nullptr)
    , m_browseInputButton(nullptr)
    , m_outputGroup(nullptr)
    , m_outputFileEdit(nullptr)
    , m_browseOutputButton(nullptr)
    , m_actionGroup(nullptr)
    , m_startButton(nullptr)
    , m_cancelButton(nullptr)
    , m_closeButton(nullptr)
    , m_progressGroup(nullptr)
    , m_progressBar(nullptr)
    , m_outputDisplay(nullptr)
    , m_statusLabel(nullptr)
    , m_decoderProcess(nullptr)
    , m_isProcessing(false)
{
    setupUI();
    setWindowTitle("Offline Decoder");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumSize(600, 500);
    resize(700, 600);
}

OfflineDecoderDialog::~OfflineDecoderDialog()
{
    if (m_decoderProcess && m_decoderProcess->state() != QProcess::NotRunning) {
        m_decoderProcess->kill();
        m_decoderProcess->waitForFinished(3000);
    }
}

void OfflineDecoderDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // File input section
    m_inputGroup = new QGroupBox("Input File", this);
    auto *inputLayout = new QGridLayout(m_inputGroup);
    
    inputLayout->addWidget(new QLabel("Binary file to decode:"), 0, 0);
    m_inputFileEdit = new QLineEdit(this);
    m_inputFileEdit->setPlaceholderText("Enter file path or browse for binary file...");
    inputLayout->addWidget(m_inputFileEdit, 1, 0, 1, 2);
    
    m_browseInputButton = new QPushButton("📁 Browse...", this);
    inputLayout->addWidget(m_browseInputButton, 1, 2);
    
    connect(m_browseInputButton, &QPushButton::clicked, this, &OfflineDecoderDialog::browseInputFile);
    connect(m_inputFileEdit, &QLineEdit::textChanged, this, &OfflineDecoderDialog::onInputPathChanged);
    
    m_mainLayout->addWidget(m_inputGroup);
    
    // Output specification section
    m_outputGroup = new QGroupBox("Output Specification", this);
    auto *outputLayout = new QGridLayout(m_outputGroup);
    
    outputLayout->addWidget(new QLabel("Output file path:"), 0, 0);
    m_outputFileEdit = new QLineEdit(this);
    m_outputFileEdit->setPlaceholderText("Enter output file path or browse...");
    outputLayout->addWidget(m_outputFileEdit, 1, 0, 1, 2);
    
    m_browseOutputButton = new QPushButton("📁 Browse...", this);
    outputLayout->addWidget(m_browseOutputButton, 1, 2);
    
    connect(m_browseOutputButton, &QPushButton::clicked, this, &OfflineDecoderDialog::browseOutputFile);
    connect(m_outputFileEdit, &QLineEdit::textChanged, this, &OfflineDecoderDialog::onOutputPathChanged);
    
    m_mainLayout->addWidget(m_outputGroup);
    
    // Action section
    m_actionGroup = new QGroupBox("Actions", this);
    auto *actionLayout = new QHBoxLayout(m_actionGroup);
    
    m_startButton = new QPushButton("🚀 Start Decoder", this);
    m_startButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px 16px; }");
    m_startButton->setEnabled(false);
    
    m_cancelButton = new QPushButton("⏹️ Cancel", this);
    m_cancelButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; padding: 8px 16px; }");
    m_cancelButton->setEnabled(false);
    
    m_closeButton = new QPushButton("❌ Close", this);
    m_closeButton->setStyleSheet("QPushButton { padding: 8px 16px; }");
    
    connect(m_startButton, &QPushButton::clicked, this, &OfflineDecoderDialog::startDecoder);
    connect(m_cancelButton, &QPushButton::clicked, [this]() {
        if (m_decoderProcess && m_decoderProcess->state() != QProcess::NotRunning) {
            m_decoderProcess->kill();
        }
    });
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::close);
    
    actionLayout->addWidget(m_startButton);
    actionLayout->addWidget(m_cancelButton);
    actionLayout->addStretch();
    actionLayout->addWidget(m_closeButton);
    
    m_mainLayout->addWidget(m_actionGroup);
    
    // Progress and output section
    m_progressGroup = new QGroupBox("Decoder Output", this);
    auto *progressLayout = new QVBoxLayout(m_progressGroup);
    
    m_statusLabel = new QLabel("Ready", this);
    m_statusLabel->setStyleSheet("font-weight: bold; color: #666;");
    progressLayout->addWidget(m_statusLabel);
    
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    progressLayout->addWidget(m_progressBar);
    
    m_outputDisplay = new QTextEdit(this);
    m_outputDisplay->setReadOnly(true);
    m_outputDisplay->setFont(QFont("Courier", 9));
    m_outputDisplay->setMinimumHeight(200);
    m_outputDisplay->setPlaceholderText("Command output will appear here...");
    progressLayout->addWidget(m_outputDisplay);
    
    m_mainLayout->addWidget(m_progressGroup);
    
    // Initialize process
    m_decoderProcess = new QProcess(this);
    connect(m_decoderProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &OfflineDecoderDialog::onProcessFinished);
    connect(m_decoderProcess, &QProcess::errorOccurred,
            this, &OfflineDecoderDialog::onProcessError);
    connect(m_decoderProcess, &QProcess::readyReadStandardOutput,
            this, &OfflineDecoderDialog::onProcessOutput);
    connect(m_decoderProcess, &QProcess::readyReadStandardError,
            this, &OfflineDecoderDialog::onProcessOutput);
}

void OfflineDecoderDialog::browseInputFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        "Select Binary File to Decode",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Binary files (*.dat *.bin *.log);;All files (*)");
    
    if (!fileName.isEmpty()) {
        m_inputFileEdit->setText(fileName);
    }
}

void OfflineDecoderDialog::browseOutputFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Select Output File",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Text files (*.txt);;All files (*)");
    
    if (!fileName.isEmpty()) {
        m_outputFileEdit->setText(fileName);
    }
}

void OfflineDecoderDialog::onInputPathChanged()
{
    validateInputs();
}

void OfflineDecoderDialog::onOutputPathChanged()
{
    validateInputs();
}

void OfflineDecoderDialog::validateInputs()
{
    bool inputValid = false;
    bool outputValid = false;
    
    // Validate input file
    QString inputPath = m_inputFileEdit->text().trimmed();
    if (!inputPath.isEmpty()) {
        QFileInfo inputInfo(inputPath);
        inputValid = inputInfo.exists() && inputInfo.isFile() && inputInfo.isReadable();
    }
    
    // Validate output path
    QString outputPath = m_outputFileEdit->text().trimmed();
    if (!outputPath.isEmpty()) {
        QFileInfo outputInfo(outputPath);
        QDir outputDir = outputInfo.dir();
        outputValid = outputDir.exists() && !outputInfo.fileName().isEmpty();
    }
    
    m_startButton->setEnabled(inputValid && outputValid && !m_isProcessing);
    
    // Update UI feedback
    if (!inputPath.isEmpty() && !inputValid) {
        m_inputFileEdit->setStyleSheet("QLineEdit { border: 2px solid red; }");
    } else {
        m_inputFileEdit->setStyleSheet("");
    }
    
    if (!outputPath.isEmpty() && !outputValid) {
        m_outputFileEdit->setStyleSheet("QLineEdit { border: 2px solid red; }");
    } else {
        m_outputFileEdit->setStyleSheet("");
    }
}

bool OfflineDecoderDialog::validateFilePaths()
{
    QString inputPath = m_inputFileEdit->text().trimmed();
    QString outputPath = m_outputFileEdit->text().trimmed();
    
    if (inputPath.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please specify an input file.");
        return false;
    }
    
    if (outputPath.isEmpty()) {
        QMessageBox::warning(this, "Output Error", "Please specify an output file.");
        return false;
    }
    
    QFileInfo inputInfo(inputPath);
    if (!inputInfo.exists()) {
        QMessageBox::warning(this, "Input Error", "Input file does not exist:\n" + inputPath);
        return false;
    }
    
    if (!inputInfo.isFile()) {
        QMessageBox::warning(this, "Input Error", "Input path is not a file:\n" + inputPath);
        return false;
    }
    
    if (!inputInfo.isReadable()) {
        QMessageBox::warning(this, "Input Error", "Input file is not readable:\n" + inputPath);
        return false;
    }
    
    QFileInfo outputInfo(outputPath);
    QDir outputDir = outputInfo.dir();
    if (!outputDir.exists()) {
        QMessageBox::warning(this, "Output Error", "Output directory does not exist:\n" + outputDir.path());
        return false;
    }
    
    if (outputInfo.exists() && !outputInfo.isWritable()) {
        QMessageBox::warning(this, "Output Error", "Output file is not writable:\n" + outputPath);
        return false;
    }
    
    return true;
}

void OfflineDecoderDialog::startDecoder()
{
    if (!validateFilePaths()) {
        return;
    }
    
    QString inputPath = m_inputFileEdit->text().trimmed();
    QString outputPath = m_outputFileEdit->text().trimmed();
    
    // Check if decoder executable exists
    QString decoderPath = "./decoder";
    QFileInfo decoderInfo(decoderPath);
    if (!decoderInfo.exists()) {
        QMessageBox::critical(this, "Decoder Error", 
            "Decoder executable not found at: " + decoderPath + "\n\n"
            "Please ensure the decoder executable is in the current directory.");
        return;
    }
    
    if (!decoderInfo.isExecutable()) {
        QMessageBox::critical(this, "Decoder Error", 
            "Decoder file is not executable: " + decoderPath + "\n\n"
            "Please check file permissions.");
        return;
    }
    
    // Confirm if output file exists
    QFileInfo outputInfo(outputPath);
    if (outputInfo.exists()) {
        int ret = QMessageBox::question(this, "File Exists", 
            "Output file already exists:\n" + outputPath + "\n\nOverwrite?",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret != QMessageBox::Yes) {
            return;
        }
    }
    
    // Start the decoding process
    m_isProcessing = true;
    showProgress(true);
    updateButtonStates();
    
    m_outputDisplay->clear();
    m_statusLabel->setText("Starting decoder...");
    m_statusLabel->setStyleSheet("font-weight: bold; color: #FF9800;");
    
    // Prepare command: sudo ./decoder -i [input_file_path] -o [output_file_path]
    QStringList arguments;
    arguments << decoderPath << "-i" << inputPath << "-o" << outputPath;
    
    QString command = QString("sudo %1").arg(arguments.join(" "));
    m_outputDisplay->append("Executing command:");
    m_outputDisplay->append(command);
    m_outputDisplay->append("");
    
    // Start the process with sudo
    m_decoderProcess->start("sudo", arguments);
    
    if (!m_decoderProcess->waitForStarted(5000)) {
        onProcessError(QProcess::FailedToStart);
        return;
    }
    
    m_statusLabel->setText("Decoder running...");
}

void OfflineDecoderDialog::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_isProcessing = false;
    showProgress(false);
    updateButtonStates();
    
    QString statusText;
    QString statusColor;
    
    if (exitStatus == QProcess::CrashExit) {
        statusText = "Decoder crashed!";
        statusColor = "color: #f44336;";
        m_outputDisplay->append("\n--- PROCESS CRASHED ---");
    } else if (exitCode == 0) {
        statusText = "Decoder completed successfully!";
        statusColor = "color: #4CAF50;";
        m_outputDisplay->append("\n--- DECODING COMPLETED SUCCESSFULLY ---");
        
        QString outputPath = m_outputFileEdit->text().trimmed();
        QFileInfo outputInfo(outputPath);
        if (outputInfo.exists()) {
            m_outputDisplay->append(QString("Output file created: %1").arg(outputPath));
            m_outputDisplay->append(QString("File size: %1 bytes").arg(outputInfo.size()));
        }
    } else {
        statusText = QString("Decoder failed with exit code: %1").arg(exitCode);
        statusColor = "color: #f44336;";
        m_outputDisplay->append(QString("\n--- DECODING FAILED (Exit Code: %1) ---").arg(exitCode));
    }
    
    m_statusLabel->setText(statusText);
    m_statusLabel->setStyleSheet("font-weight: bold; " + statusColor);
    
    // Read any remaining output
    if (m_decoderProcess->bytesAvailable() > 0) {
        onProcessOutput();
    }
}

void OfflineDecoderDialog::onProcessError(QProcess::ProcessError error)
{
    m_isProcessing = false;
    showProgress(false);
    updateButtonStates();
    
    QString errorText;
    switch (error) {
        case QProcess::FailedToStart:
            errorText = "Failed to start decoder process. Check if sudo is available and decoder executable exists.";
            break;
        case QProcess::Crashed:
            errorText = "Decoder process crashed during execution.";
            break;
        case QProcess::Timedout:
            errorText = "Decoder process timed out.";
            break;
        case QProcess::WriteError:
            errorText = "Error writing to decoder process.";
            break;
        case QProcess::ReadError:
            errorText = "Error reading from decoder process.";
            break;
        default:
            errorText = "Unknown error occurred during decoding.";
            break;
    }
    
    m_statusLabel->setText("Error: " + errorText);
    m_statusLabel->setStyleSheet("font-weight: bold; color: #f44336;");
    
    m_outputDisplay->append("\n--- ERROR ---");
    m_outputDisplay->append(errorText);
    
    QMessageBox::critical(this, "Decoder Error", errorText);
}

void OfflineDecoderDialog::onProcessOutput()
{
    if (!m_decoderProcess) return;
    
    QByteArray stdOut = m_decoderProcess->readAllStandardOutput();
    QByteArray stdErr = m_decoderProcess->readAllStandardError();
    
    if (!stdOut.isEmpty()) {
        m_outputDisplay->append(QString::fromUtf8(stdOut));
    }
    
    if (!stdErr.isEmpty()) {
        m_outputDisplay->append("STDERR: " + QString::fromUtf8(stdErr));
    }
    
    // Auto-scroll to bottom
    QTextCursor cursor = m_outputDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_outputDisplay->setTextCursor(cursor);
}

void OfflineDecoderDialog::showProgress(bool show)
{
    m_progressBar->setVisible(show);
    if (show) {
        m_progressBar->setRange(0, 0); // Indeterminate progress
    }
}

void OfflineDecoderDialog::updateButtonStates()
{
    m_startButton->setEnabled(!m_isProcessing && !m_inputFileEdit->text().trimmed().isEmpty() && 
                             !m_outputFileEdit->text().trimmed().isEmpty());
    m_cancelButton->setEnabled(m_isProcessing);
    m_browseInputButton->setEnabled(!m_isProcessing);
    m_browseOutputButton->setEnabled(!m_isProcessing);
    m_inputFileEdit->setEnabled(!m_isProcessing);
    m_outputFileEdit->setEnabled(!m_isProcessing);
}

void OfflineDecoderDialog::resetDialog()
{
    m_inputFileEdit->clear();
    m_outputFileEdit->clear();
    m_outputDisplay->clear();
    m_statusLabel->setText("Ready");
    m_statusLabel->setStyleSheet("font-weight: bold; color: #666;");
    showProgress(false);
    updateButtonStates();
}