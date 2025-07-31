#include "../include/advancedfilter.h"
#include <QtWidgets/QMessageBox>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QRegularExpression>
#include <QtCore/QDir>

AdvancedFilterDialog::AdvancedFilterDialog(QWidget *parent)
    : QDialog(parent)
    , m_mainLayout(new QVBoxLayout(this))
{
    setWindowTitle("Advanced Message Filter");
    setModal(true);
    resize(800, 600);
    
    setupUI();
    
    // 连接信号槽
    connect(m_regexCheckBox, &QCheckBox::toggled, this, &AdvancedFilterDialog::onRegexToggled);
    connect(m_useTimeFilterCheckBox, &QCheckBox::toggled, this, &AdvancedFilterDialog::onTimeFilterToggled);
    connect(m_useSizeFilterCheckBox, &QCheckBox::toggled, this, &AdvancedFilterDialog::onSizeFilterToggled);
    connect(m_enableCustomFilterCheckBox, &QCheckBox::toggled, this, &AdvancedFilterDialog::onCustomFilterToggled);
    
    connect(m_contentPatternEdit, &QLineEdit::textChanged, this, &AdvancedFilterDialog::validateRegex);
    
    // 按钮连接
    connect(m_previewBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onPreviewFilter);
    connect(m_applyBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onApplyFilter);
    connect(m_resetBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onResetFilter);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    
    // 预设管理
    connect(m_savePresetBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onSavePreset);
    connect(m_loadPresetBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onLoadPreset);
    
    // 快速过滤器
    connect(m_errorMessagesBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onQuickFilterSelected);
    connect(m_handoverEventsBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onQuickFilterSelected);
    connect(m_rrcConnectionBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onQuickFilterSelected);
    connect(m_dataTransfersBtn, &QPushButton::clicked, this, &AdvancedFilterDialog::onQuickFilterSelected);
}

AdvancedFilterDialog::~AdvancedFilterDialog() = default;

void AdvancedFilterDialog::setupUI()
{
    // 创建所有UI组件
    setupMessageTypeFilters();
    setupContentFilters();
    setupTimeFilters();
    setupSizeFilters();
    setupLayerFilters();
    setupAdvancedFilters();
    setupDisplayOptions();
    setupPresets();
    
    // 创建主要区域
    auto *scrollArea = new QScrollArea(this);
    auto *scrollWidget = new QWidget();
    auto *scrollLayout = new QVBoxLayout(scrollWidget);
    
    scrollLayout->addWidget(m_messageTypeGroup);
    scrollLayout->addWidget(m_contentGroup);
    scrollLayout->addWidget(m_timeGroup);
    scrollLayout->addWidget(m_sizeGroup);
    scrollLayout->addWidget(m_layerGroup);
    scrollLayout->addWidget(m_advancedGroup);
    scrollLayout->addWidget(m_displayGroup);
    scrollLayout->addWidget(m_presetsGroup);
    scrollLayout->addWidget(m_quickFiltersGroup);
    
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    
    // 预览区域
    m_previewTextEdit = new QTextEdit(this);
    m_previewTextEdit->setMaximumHeight(150);
    m_previewTextEdit->setPlaceholderText("Filter preview will appear here...");
    
    // 控制按钮
    m_buttonLayout = new QHBoxLayout();
    m_previewBtn = new QPushButton("Preview", this);
    m_applyBtn = new QPushButton("Apply Filter", this);
    m_resetBtn = new QPushButton("Reset", this);
    m_cancelBtn = new QPushButton("Cancel", this);
    
    m_applyBtn->setDefault(true);
    m_applyBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    m_buttonLayout->addWidget(m_previewBtn);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_resetBtn);
    m_buttonLayout->addWidget(m_cancelBtn);
    m_buttonLayout->addWidget(m_applyBtn);
    
    // 添加到主布局
    m_mainLayout->addWidget(scrollArea, 1);
    m_mainLayout->addWidget(new QLabel("Preview:"));
    m_mainLayout->addWidget(m_previewTextEdit);
    m_mainLayout->addLayout(m_buttonLayout);
}

void AdvancedFilterDialog::setupMessageTypeFilters()
{
    m_messageTypeGroup = new QGroupBox("Message Type Filters", this);
    auto *layout = new QHBoxLayout(m_messageTypeGroup);
    
    // Include list
    auto *includeLayout = new QVBoxLayout();
    includeLayout->addWidget(new QLabel("Include Types:"));
    m_includeTypesList = new QListWidget();
    m_includeTypesList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_addIncludeBtn = new QPushButton("Add");
    m_removeIncludeBtn = new QPushButton("Remove");
    
    includeLayout->addWidget(m_includeTypesList);
    auto *includeBtnLayout = new QHBoxLayout();
    includeBtnLayout->addWidget(m_addIncludeBtn);
    includeBtnLayout->addWidget(m_removeIncludeBtn);
    includeLayout->addLayout(includeBtnLayout);
    
    // Exclude list
    auto *excludeLayout = new QVBoxLayout();
    excludeLayout->addWidget(new QLabel("Exclude Types:"));
    m_excludeTypesList = new QListWidget();
    m_excludeTypesList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_addExcludeBtn = new QPushButton("Add");
    m_removeExcludeBtn = new QPushButton("Remove");
    
    excludeLayout->addWidget(m_excludeTypesList);
    auto *excludeBtnLayout = new QHBoxLayout();
    excludeBtnLayout->addWidget(m_addExcludeBtn);
    excludeBtnLayout->addWidget(m_removeExcludeBtn);
    excludeLayout->addLayout(excludeBtnLayout);
    
    layout->addLayout(includeLayout);
    layout->addLayout(excludeLayout);
    
    // 预填充常见消息类型
    const QStringList commonTypes = {
        "NR5G L2 UL TB", "NR5G L2 UL BSR", "NR5G MAC DCI Info",
        "NR5G RRC OTA Packet", "NR5G NAS MM5G State", "NR5G PDCP DL Data Pdu"
    };
    
    for (const QString &type : commonTypes) {
        m_includeTypesList->addItem(type);
        m_excludeTypesList->addItem(type);
    }
}

void AdvancedFilterDialog::setupContentFilters()
{
    m_contentGroup = new QGroupBox("Content Filters", this);
    auto *layout = new QGridLayout(m_contentGroup);
    
    layout->addWidget(new QLabel("Pattern:"), 0, 0);
    m_contentPatternEdit = new QLineEdit();
    m_contentPatternEdit->setPlaceholderText("Enter search pattern...");
    layout->addWidget(m_contentPatternEdit, 0, 1, 1, 2);
    
    m_regexCheckBox = new QCheckBox("Use Regular Expression");
    m_caseSensitiveCheckBox = new QCheckBox("Case Sensitive");
    layout->addWidget(m_regexCheckBox, 1, 0);
    layout->addWidget(m_caseSensitiveCheckBox, 1, 1);
    
    m_regexStatusLabel = new QLabel();
    m_regexStatusLabel->setStyleSheet("color: red;");
    layout->addWidget(m_regexStatusLabel, 2, 0, 1, 3);
}

void AdvancedFilterDialog::setupTimeFilters()
{
    m_timeGroup = new QGroupBox("Time Filters", this);
    auto *layout = new QGridLayout(m_timeGroup);
    
    m_useTimeFilterCheckBox = new QCheckBox("Enable Time Filter");
    layout->addWidget(m_useTimeFilterCheckBox, 0, 0, 1, 2);
    
    layout->addWidget(new QLabel("Start Time:"), 1, 0);
    m_startTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime().addDays(-1));
    m_startTimeEdit->setCalendarPopup(true);
    m_startTimeEdit->setEnabled(false);
    layout->addWidget(m_startTimeEdit, 1, 1);
    
    layout->addWidget(new QLabel("End Time:"), 2, 0);
    m_endTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    m_endTimeEdit->setCalendarPopup(true);
    m_endTimeEdit->setEnabled(false);
    layout->addWidget(m_endTimeEdit, 2, 1);
}

void AdvancedFilterDialog::setupSizeFilters()
{
    m_sizeGroup = new QGroupBox("Size Filters", this);
    auto *layout = new QGridLayout(m_sizeGroup);
    
    m_useSizeFilterCheckBox = new QCheckBox("Enable Size Filter");
    layout->addWidget(m_useSizeFilterCheckBox, 0, 0, 1, 2);
    
    layout->addWidget(new QLabel("Min Size (bytes):"), 1, 0);
    m_minSizeSpinBox = new QSpinBox();
    m_minSizeSpinBox->setRange(0, 10000);
    m_minSizeSpinBox->setEnabled(false);
    layout->addWidget(m_minSizeSpinBox, 1, 1);
    
    layout->addWidget(new QLabel("Max Size (bytes):"), 2, 0);
    m_maxSizeSpinBox = new QSpinBox();
    m_maxSizeSpinBox->setRange(0, 10000);
    m_maxSizeSpinBox->setValue(1000);
    m_maxSizeSpinBox->setEnabled(false);
    layout->addWidget(m_maxSizeSpinBox, 2, 1);
}

void AdvancedFilterDialog::setupLayerFilters()
{
    m_layerGroup = new QGroupBox("Protocol Layer Filters", this);
    auto *layout = new QGridLayout(m_layerGroup);
    
    m_nasCheckBox = new QCheckBox("NAS (Non-Access Stratum)");
    m_rrcCheckBox = new QCheckBox("RRC (Radio Resource Control)");
    m_pdcpCheckBox = new QCheckBox("PDCP (Packet Data Convergence)");
    m_rlcCheckBox = new QCheckBox("RLC (Radio Link Control)");
    m_macCheckBox = new QCheckBox("MAC (Medium Access Control)");
    m_physicalCheckBox = new QCheckBox("Physical Layer");
    
    layout->addWidget(m_nasCheckBox, 0, 0);
    layout->addWidget(m_rrcCheckBox, 0, 1);
    layout->addWidget(m_pdcpCheckBox, 1, 0);
    layout->addWidget(m_rlcCheckBox, 1, 1);
    layout->addWidget(m_macCheckBox, 2, 0);
    layout->addWidget(m_physicalCheckBox, 2, 1);
}

void AdvancedFilterDialog::setupAdvancedFilters()
{
    m_advancedGroup = new QGroupBox("Advanced Filters", this);
    auto *layout = new QVBoxLayout(m_advancedGroup);
    
    m_enableCustomFilterCheckBox = new QCheckBox("Enable Custom Filter Script");
    layout->addWidget(m_enableCustomFilterCheckBox);
    
    m_customFilterEdit = new QTextEdit();
    m_customFilterEdit->setMaximumHeight(100);
    m_customFilterEdit->setEnabled(false);
    m_customFilterEdit->setPlaceholderText("Enter custom filter script (JavaScript-like syntax)...");
    layout->addWidget(m_customFilterEdit);
    
    m_customFilterHelpLabel = new QLabel(
        "Example: message.contains('Error') && timestamp > '2023-01-01' && size > 100");
    m_customFilterHelpLabel->setStyleSheet("color: gray; font-size: 10px;");
    layout->addWidget(m_customFilterHelpLabel);
}

void AdvancedFilterDialog::setupDisplayOptions()
{
    m_displayGroup = new QGroupBox("Display Options", this);
    auto *layout = new QGridLayout(m_displayGroup);
    
    m_highlightMatchesCheckBox = new QCheckBox("Highlight Matches");
    layout->addWidget(m_highlightMatchesCheckBox, 0, 0);
    
    layout->addWidget(new QLabel("Highlight Color:"), 0, 1);
    m_highlightColorCombo = new QComboBox();
    m_highlightColorCombo->addItems({"Yellow", "Green", "Blue", "Red", "Orange"});
    layout->addWidget(m_highlightColorCombo, 0, 2);
    
    m_showOnlyMatchesCheckBox = new QCheckBox("Show Only Matches");
    layout->addWidget(m_showOnlyMatchesCheckBox, 1, 0);
    
    layout->addWidget(new QLabel("Max Results:"), 1, 1);
    m_maxResultsSpinBox = new QSpinBox();
    m_maxResultsSpinBox->setRange(1, 10000);
    m_maxResultsSpinBox->setValue(1000);
    layout->addWidget(m_maxResultsSpinBox, 1, 2);
}

void AdvancedFilterDialog::setupPresets()
{
    m_presetsGroup = new QGroupBox("Presets", this);
    auto *layout = new QHBoxLayout(m_presetsGroup);
    
    m_presetsCombo = new QComboBox();
    m_presetsCombo->addItems({"<Select Preset>", "Error Messages", "Handover Events", "RRC Connection", "Data Transfers"});
    layout->addWidget(m_presetsCombo);
    
    m_savePresetBtn = new QPushButton("Save");
    m_loadPresetBtn = new QPushButton("Load");
    m_deletePresetBtn = new QPushButton("Delete");
    
    layout->addWidget(m_savePresetBtn);
    layout->addWidget(m_loadPresetBtn);
    layout->addWidget(m_deletePresetBtn);
    
    // Quick filters
    m_quickFiltersGroup = new QGroupBox("Quick Filters", this);
    auto *quickLayout = new QHBoxLayout(m_quickFiltersGroup);
    
    m_errorMessagesBtn = new QPushButton("Error Messages");
    m_handoverEventsBtn = new QPushButton("Handover Events");
    m_rrcConnectionBtn = new QPushButton("RRC Connection");
    m_dataTransfersBtn = new QPushButton("Data Transfers");
    
    quickLayout->addWidget(m_errorMessagesBtn);
    quickLayout->addWidget(m_handoverEventsBtn);
    quickLayout->addWidget(m_rrcConnectionBtn);
    quickLayout->addWidget(m_dataTransfersBtn);
}

AdvancedFilterDialog::FilterCriteria AdvancedFilterDialog::getFilterCriteria() const
{
    FilterCriteria criteria;
    
    // Message types
    for (int i = 0; i < m_includeTypesList->count(); ++i) {
        if (m_includeTypesList->item(i)->isSelected()) {
            criteria.includeMessageTypes << m_includeTypesList->item(i)->text();
        }
    }
    
    for (int i = 0; i < m_excludeTypesList->count(); ++i) {
        if (m_excludeTypesList->item(i)->isSelected()) {
            criteria.excludeMessageTypes << m_excludeTypesList->item(i)->text();
        }
    }
    
    // Content filters
    criteria.contentPattern = m_contentPatternEdit->text();
    criteria.useRegex = m_regexCheckBox->isChecked();
    criteria.caseSensitive = m_caseSensitiveCheckBox->isChecked();
    
    // Time filters
    criteria.useTimeFilter = m_useTimeFilterCheckBox->isChecked();
    criteria.startTime = m_startTimeEdit->dateTime();
    criteria.endTime = m_endTimeEdit->dateTime();
    
    // Size filters
    criteria.useSizeFilter = m_useSizeFilterCheckBox->isChecked();
    criteria.minMessageSize = m_minSizeSpinBox->value();
    criteria.maxMessageSize = m_maxSizeSpinBox->value();
    
    // Layer filters
    if (m_nasCheckBox->isChecked()) criteria.protocolLayers << "NAS";
    if (m_rrcCheckBox->isChecked()) criteria.protocolLayers << "RRC";
    if (m_pdcpCheckBox->isChecked()) criteria.protocolLayers << "PDCP";
    if (m_rlcCheckBox->isChecked()) criteria.protocolLayers << "RLC";
    if (m_macCheckBox->isChecked()) criteria.protocolLayers << "MAC";
    if (m_physicalCheckBox->isChecked()) criteria.protocolLayers << "Physical";
    
    // Advanced filters
    criteria.enableCustomFilter = m_enableCustomFilterCheckBox->isChecked();
    criteria.customFilter = m_customFilterEdit->toPlainText();
    
    // Display options
    criteria.highlightMatches = m_highlightMatchesCheckBox->isChecked();
    criteria.highlightColor = m_highlightColorCombo->currentText();
    criteria.showOnlyMatches = m_showOnlyMatchesCheckBox->isChecked();
    criteria.maxResults = m_maxResultsSpinBox->value();
    
    return criteria;
}

void AdvancedFilterDialog::setFilterCriteria(const FilterCriteria &criteria)
{
    // 设置各个控件的值
    m_contentPatternEdit->setText(criteria.contentPattern);
    m_regexCheckBox->setChecked(criteria.useRegex);
    m_caseSensitiveCheckBox->setChecked(criteria.caseSensitive);
    
    m_useTimeFilterCheckBox->setChecked(criteria.useTimeFilter);
    m_startTimeEdit->setDateTime(criteria.startTime);
    m_endTimeEdit->setDateTime(criteria.endTime);
    
    m_useSizeFilterCheckBox->setChecked(criteria.useSizeFilter);
    m_minSizeSpinBox->setValue(criteria.minMessageSize);
    m_maxSizeSpinBox->setValue(criteria.maxMessageSize);
    
    // Layer filters
    m_nasCheckBox->setChecked(criteria.protocolLayers.contains("NAS"));
    m_rrcCheckBox->setChecked(criteria.protocolLayers.contains("RRC"));
    m_pdcpCheckBox->setChecked(criteria.protocolLayers.contains("PDCP"));
    m_rlcCheckBox->setChecked(criteria.protocolLayers.contains("RLC"));
    m_macCheckBox->setChecked(criteria.protocolLayers.contains("MAC"));
    m_physicalCheckBox->setChecked(criteria.protocolLayers.contains("Physical"));
    
    m_enableCustomFilterCheckBox->setChecked(criteria.enableCustomFilter);
    m_customFilterEdit->setPlainText(criteria.customFilter);
    
    m_highlightMatchesCheckBox->setChecked(criteria.highlightMatches);
    m_showOnlyMatchesCheckBox->setChecked(criteria.showOnlyMatches);
    m_maxResultsSpinBox->setValue(criteria.maxResults);
}

AdvancedFilterDialog::FilterCriteria AdvancedFilterDialog::getQuickFilter(const QString &filterType)
{
    FilterCriteria criteria;
    
    if (filterType == "Error Messages") {
        criteria.contentPattern = "Error|ERROR|Fail|FAIL|Exception";
        criteria.useRegex = true;
        criteria.caseSensitive = false;
    }
    else if (filterType == "Handover Events") {
        criteria.contentPattern = "Handover|HO|Mobility";
        criteria.useRegex = true;
        criteria.protocolLayers << "RRC" << "NAS";
    }
    else if (filterType == "RRC Connection") {
        criteria.contentPattern = "RRC.*Connection|Connection.*Setup|Connection.*Release";
        criteria.useRegex = true;
        criteria.protocolLayers << "RRC";
    }
    else if (filterType == "Data Transfers") {
        criteria.protocolLayers << "PDCP" << "RLC";
        criteria.includeMessageTypes << "NR5G PDCP DL Data Pdu" << "NR5G L2 DL DATA PDU";
    }
    
    return criteria;
}

void AdvancedFilterDialog::onApplyFilter()
{
    FilterCriteria criteria = getFilterCriteria();
    emit filterApplied(criteria);
    accept();
}

void AdvancedFilterDialog::onPreviewFilter()
{
    FilterCriteria criteria = getFilterCriteria();
    emit previewRequested(criteria);
    updatePreview();
}

void AdvancedFilterDialog::onResetFilter()
{
    // 重置所有控件到默认状态
    m_contentPatternEdit->clear();
    m_regexCheckBox->setChecked(false);
    m_caseSensitiveCheckBox->setChecked(false);
    
    m_useTimeFilterCheckBox->setChecked(false);
    m_useSizeFilterCheckBox->setChecked(false);
    
    m_nasCheckBox->setChecked(false);
    m_rrcCheckBox->setChecked(false);
    m_pdcpCheckBox->setChecked(false);
    m_rlcCheckBox->setChecked(false);
    m_macCheckBox->setChecked(false);
    m_physicalCheckBox->setChecked(false);
    
    m_enableCustomFilterCheckBox->setChecked(false);
    m_customFilterEdit->clear();
    
    m_highlightMatchesCheckBox->setChecked(false);
    m_showOnlyMatchesCheckBox->setChecked(false);
    m_maxResultsSpinBox->setValue(1000);
    
    m_includeTypesList->clearSelection();
    m_excludeTypesList->clearSelection();
}

void AdvancedFilterDialog::onSavePreset()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Save Preset", "Preset name:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        FilterCriteria criteria = getFilterCriteria();
        // 保存预设到设置文件
        QSettings settings;
        settings.beginGroup("FilterPresets");
        // 将criteria序列化保存...
        settings.endGroup();
        
        emit presetSaved(name);
    }
}

void AdvancedFilterDialog::onLoadPreset()
{
    QString presetName = m_presetsCombo->currentText();
    if (presetName != "<Select Preset>") {
        FilterCriteria criteria = getQuickFilter(presetName);
        setFilterCriteria(criteria);
        emit presetLoaded(presetName);
    }
}

void AdvancedFilterDialog::onQuickFilterSelected()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        FilterCriteria criteria = getQuickFilter(button->text());
        setFilterCriteria(criteria);
        updatePreview();
    }
}

void AdvancedFilterDialog::onRegexToggled(bool enabled)
{
    if (enabled) {
        validateRegex();
        m_regexStatusLabel->setVisible(true);
    } else {
        m_regexStatusLabel->setVisible(false);
    }
}

void AdvancedFilterDialog::onTimeFilterToggled(bool enabled)
{
    m_startTimeEdit->setEnabled(enabled);
    m_endTimeEdit->setEnabled(enabled);
}

void AdvancedFilterDialog::onSizeFilterToggled(bool enabled)
{
    m_minSizeSpinBox->setEnabled(enabled);
    m_maxSizeSpinBox->setEnabled(enabled);
}

void AdvancedFilterDialog::onCustomFilterToggled(bool enabled)
{
    m_customFilterEdit->setEnabled(enabled);
    m_customFilterHelpLabel->setVisible(enabled);
}

void AdvancedFilterDialog::validateRegex()
{
    if (!m_regexCheckBox->isChecked()) {
        return;
    }
    
    QString pattern = m_contentPatternEdit->text();
    if (pattern.isEmpty()) {
        m_regexStatusLabel->clear();
        return;
    }
    
    QRegularExpression regex(pattern);
    if (regex.isValid()) {
        m_regexStatusLabel->setText("✓ Valid regular expression");
        m_regexStatusLabel->setStyleSheet("color: green;");
    } else {
        m_regexStatusLabel->setText("✗ Invalid regular expression: " + regex.errorString());
        m_regexStatusLabel->setStyleSheet("color: red;");
    }
}

void AdvancedFilterDialog::updatePreview()
{
    FilterCriteria criteria = getFilterCriteria();
    QString preview = "Filter Summary:\n";
    
    if (!criteria.contentPattern.isEmpty()) {
        preview += QString("• Content: %1 %2\n")
                  .arg(criteria.contentPattern)
                  .arg(criteria.useRegex ? "(regex)" : "(literal)");
    }
    
    if (criteria.useTimeFilter) {
        preview += QString("• Time: %1 to %2\n")
                  .arg(criteria.startTime.toString())
                  .arg(criteria.endTime.toString());
    }
    
    if (criteria.useSizeFilter) {
        preview += QString("• Size: %1 to %2 bytes\n")
                  .arg(criteria.minMessageSize)
                  .arg(criteria.maxMessageSize);
    }
    
    if (!criteria.protocolLayers.isEmpty()) {
        preview += QString("• Layers: %1\n").arg(criteria.protocolLayers.join(", "));
    }
    
    if (!criteria.includeMessageTypes.isEmpty()) {
        preview += QString("• Include: %1\n").arg(criteria.includeMessageTypes.join(", "));
    }
    
    if (!criteria.excludeMessageTypes.isEmpty()) {
        preview += QString("• Exclude: %1\n").arg(criteria.excludeMessageTypes.join(", "));
    }
    
    preview += QString("• Max results: %1").arg(criteria.maxResults);
    
    m_previewTextEdit->setPlainText(preview);
}

// MessageFilter Implementation
MessageFilter::MessageFilter(QObject *parent) : QObject(parent) {}

bool MessageFilter::matchesFilter(const QString &message, const QString &messageType, 
                                 const QDateTime &timestamp, int messageSize,
                                 const AdvancedFilterDialog::FilterCriteria &criteria)
{
    // 检查消息类型包含/排除
    if (!matchesMessageType(messageType, criteria)) {
        return false;
    }
    
    // 检查内容匹配
    if (!matchesContent(message, criteria)) {
        return false;
    }
    
    // 检查时间范围
    if (!matchesTimeRange(timestamp, criteria)) {
        return false;
    }
    
    // 检查消息大小
    if (!matchesSizeRange(messageSize, criteria)) {
        return false;
    }
    
    // 检查协议层
    if (!matchesLayer(messageType, criteria)) {
        return false;
    }
    
    // 检查自定义过滤器
    if (!matchesCustomFilter(message, messageType, timestamp, messageSize, criteria)) {
        return false;
    }
    
    return true;
}

bool MessageFilter::matchesMessageType(const QString &messageType, const AdvancedFilterDialog::FilterCriteria &criteria)
{
    // 如果有排除列表，检查是否在排除列表中
    if (!criteria.excludeMessageTypes.isEmpty() && criteria.excludeMessageTypes.contains(messageType)) {
        return false;
    }
    
    // 如果有包含列表，检查是否在包含列表中
    if (!criteria.includeMessageTypes.isEmpty() && !criteria.includeMessageTypes.contains(messageType)) {
        return false;
    }
    
    return true;
}

bool MessageFilter::matchesContent(const QString &message, const AdvancedFilterDialog::FilterCriteria &criteria)
{
    if (criteria.contentPattern.isEmpty()) {
        return true;
    }
    
    if (criteria.useRegex) {
        QRegularExpression regex(criteria.contentPattern);
        if (!criteria.caseSensitive) {
            regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        }
        return regex.match(message).hasMatch();
    } else {
        Qt::CaseSensitivity cs = criteria.caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
        return message.contains(criteria.contentPattern, cs);
    }
}

bool MessageFilter::matchesTimeRange(const QDateTime &timestamp, const AdvancedFilterDialog::FilterCriteria &criteria)
{
    if (!criteria.useTimeFilter) {
        return true;
    }
    
    return timestamp >= criteria.startTime && timestamp <= criteria.endTime;
}

bool MessageFilter::matchesSizeRange(int messageSize, const AdvancedFilterDialog::FilterCriteria &criteria)
{
    if (!criteria.useSizeFilter) {
        return true;
    }
    
    return messageSize >= criteria.minMessageSize && messageSize <= criteria.maxMessageSize;
}

bool MessageFilter::matchesLayer(const QString &messageType, const AdvancedFilterDialog::FilterCriteria &criteria)
{
    if (criteria.protocolLayers.isEmpty()) {
        return true;
    }
    
    QString layer = getProtocolLayer(messageType);
    return criteria.protocolLayers.contains(layer);
}

bool MessageFilter::matchesCustomFilter(const QString &message, const QString &messageType, 
                                       const QDateTime &timestamp, int messageSize,
                                       const AdvancedFilterDialog::FilterCriteria &criteria)
{
    if (!criteria.enableCustomFilter || criteria.customFilter.isEmpty()) {
        return true;
    }
    
    // 简单的脚本执行器 - 在实际实现中可能需要更复杂的脚本引擎
    QString script = criteria.customFilter;
    
    // 替换变量
    script.replace("message", QString("\"%1\"").arg(message));
    script.replace("messageType", QString("\"%1\"").arg(messageType));
    script.replace("size", QString::number(messageSize));
    script.replace("timestamp", QString("\"%1\"").arg(timestamp.toString()));
    
    // 这里可以使用QJSEngine来执行JavaScript
    // 简化版本：检查一些常见模式
    if (script.contains("message.contains(") && script.contains("Error")) {
        return message.contains("Error");
    }
    
    return true; // 默认通过
}

QString MessageFilter::getProtocolLayer(const QString &messageType)  
{
    if (messageType.contains("NAS", Qt::CaseInsensitive)) return "NAS";
    if (messageType.contains("RRC", Qt::CaseInsensitive)) return "RRC";
    if (messageType.contains("PDCP", Qt::CaseInsensitive)) return "PDCP";
    if (messageType.contains("RLC", Qt::CaseInsensitive)) return "RLC";
    if (messageType.contains("MAC", Qt::CaseInsensitive)) return "MAC";
    if (messageType.contains("LL1", Qt::CaseInsensitive) || messageType.contains("ML1", Qt::CaseInsensitive)) return "Physical";
    return "Unknown";
}

QStringList MessageFilter::filterMessages(const QStringList &messages, 
                                         const QStringList &messageTypes,
                                         const QList<QDateTime> &timestamps,
                                         const QList<int> &messageSizes,
                                         const AdvancedFilterDialog::FilterCriteria &criteria)
{
    QStringList filteredMessages;
    
    for (int i = 0; i < messages.size() && i < messageTypes.size() && 
         i < timestamps.size() && i < messageSizes.size(); ++i) {
        
        if (matchesFilter(messages[i], messageTypes[i], timestamps[i], messageSizes[i], criteria)) {
            filteredMessages.append(messages[i]);
            
            if (filteredMessages.size() >= criteria.maxResults) {
                break;
            }
        }
    }
    
    return filteredMessages;
}