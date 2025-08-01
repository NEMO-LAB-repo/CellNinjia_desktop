#ifndef ADVANCEDFILTER_H
#define ADVANCEDFILTER_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QInputDialog>
#include <QtCore/QRegularExpression>
#include <QtCore/QDateTime>

class AdvancedFilterDialog : public QDialog
{
    Q_OBJECT

public:
    struct FilterCriteria {
        // Message Type Filters
        QStringList includeMessageTypes;
        QStringList excludeMessageTypes;
        
        // Content Filters
        QString contentPattern;
        bool useRegex;
        bool caseSensitive;
        
        // Time Filters
        QDateTime startTime;
        QDateTime endTime;
        bool useTimeFilter;
        
        // Size Filters
        int minMessageSize;
        int maxMessageSize;
        bool useSizeFilter;
        
        // Layer Filters
        QStringList protocolLayers;
        
        // Advanced Filters
        QString customFilter;
        bool enableCustomFilter;
        
        // Display Options
        bool highlightMatches;
        QString highlightColor;
        bool showOnlyMatches;
        int maxResults;
    };

    explicit AdvancedFilterDialog(QWidget *parent = nullptr);
    ~AdvancedFilterDialog();

    FilterCriteria getFilterCriteria() const;
    void setFilterCriteria(const FilterCriteria &criteria);
    
    static FilterCriteria getQuickFilter(const QString &filterType);

signals:
    void filterApplied(const FilterCriteria &criteria);
    void previewRequested(const FilterCriteria &criteria);
    void presetSaved(const QString &name);
    void presetLoaded(const QString &name);

private slots:
    void onApplyFilter();
    void onPreviewFilter();
    void onResetFilter();
    void onSavePreset();
    void onLoadPreset();
    void onQuickFilterSelected();
    void onRegexToggled(bool enabled);
    void onTimeFilterToggled(bool enabled);
    void onSizeFilterToggled(bool enabled);
    void onCustomFilterToggled(bool enabled);

private:
    void setupUI();
    void setupMessageTypeFilters();
    void setupContentFilters();
    void setupTimeFilters();
    void setupSizeFilters();
    void setupLayerFilters();
    void setupAdvancedFilters();
    void setupDisplayOptions();
    void setupPresets();
    
    void validateRegex();
    void updatePreview();
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    
    // Message Type Filters
    QGroupBox *m_messageTypeGroup;
    QListWidget *m_includeTypesList;
    QListWidget *m_excludeTypesList;
    QPushButton *m_addIncludeBtn;
    QPushButton *m_addExcludeBtn;
    QPushButton *m_removeIncludeBtn;
    QPushButton *m_removeExcludeBtn;
    
    // Content Filters
    QGroupBox *m_contentGroup;
    QLineEdit *m_contentPatternEdit;
    QCheckBox *m_regexCheckBox;
    QCheckBox *m_caseSensitiveCheckBox;
    QLabel *m_regexStatusLabel;
    
    // Time Filters
    QGroupBox *m_timeGroup;
    QCheckBox *m_useTimeFilterCheckBox;
    QDateTimeEdit *m_startTimeEdit;
    QDateTimeEdit *m_endTimeEdit;
    
    // Size Filters
    QGroupBox *m_sizeGroup;
    QCheckBox *m_useSizeFilterCheckBox;
    QSpinBox *m_minSizeSpinBox;
    QSpinBox *m_maxSizeSpinBox;
    
    // Layer Filters
    QGroupBox *m_layerGroup;
    QCheckBox *m_nasCheckBox;
    QCheckBox *m_rrcCheckBox;
    QCheckBox *m_pdcpCheckBox;
    QCheckBox *m_rlcCheckBox;
    QCheckBox *m_macCheckBox;
    QCheckBox *m_physicalCheckBox;
    
    // Advanced Filters
    QGroupBox *m_advancedGroup;
    QCheckBox *m_enableCustomFilterCheckBox;
    QTextEdit *m_customFilterEdit;
    QLabel *m_customFilterHelpLabel;
    
    // Display Options
    QGroupBox *m_displayGroup;
    QCheckBox *m_highlightMatchesCheckBox;
    QComboBox *m_highlightColorCombo;
    QCheckBox *m_showOnlyMatchesCheckBox;
    QSpinBox *m_maxResultsSpinBox;
    
    // Presets
    QGroupBox *m_presetsGroup;
    QComboBox *m_presetsCombo;
    QPushButton *m_savePresetBtn;
    QPushButton *m_loadPresetBtn;
    QPushButton *m_deletePresetBtn;
    
    // Quick Filters
    QGroupBox *m_quickFiltersGroup;
    QPushButton *m_errorMessagesBtn;
    QPushButton *m_handoverEventsBtn;
    QPushButton *m_rrcConnectionBtn;
    QPushButton *m_dataTransfersBtn;
    
    // Control Buttons
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_previewBtn;
    QPushButton *m_applyBtn;
    QPushButton *m_resetBtn;
    QPushButton *m_cancelBtn;
    
    // Preview
    QTextEdit *m_previewTextEdit;
    
    FilterCriteria m_currentCriteria;
};

class MessageFilter : public QObject
{
    Q_OBJECT

public:
    explicit MessageFilter(QObject *parent = nullptr);
    
    bool matchesFilter(const QString &message, const QString &messageType, 
                      const QDateTime &timestamp, int messageSize,
                      const AdvancedFilterDialog::FilterCriteria &criteria);
    
    QStringList filterMessages(const QStringList &messages, 
                              const QStringList &messageTypes,
                              const QList<QDateTime> &timestamps,
                              const QList<int> &messageSizes,
                              const AdvancedFilterDialog::FilterCriteria &criteria);

private:
    bool matchesMessageType(const QString &messageType, const AdvancedFilterDialog::FilterCriteria &criteria);
    bool matchesContent(const QString &message, const AdvancedFilterDialog::FilterCriteria &criteria);
    bool matchesTimeRange(const QDateTime &timestamp, const AdvancedFilterDialog::FilterCriteria &criteria);
    bool matchesSizeRange(int messageSize, const AdvancedFilterDialog::FilterCriteria &criteria);
    bool matchesLayer(const QString &messageType, const AdvancedFilterDialog::FilterCriteria &criteria);
    bool matchesCustomFilter(const QString &message, const QString &messageType, 
                           const QDateTime &timestamp, int messageSize,
                           const AdvancedFilterDialog::FilterCriteria &criteria);
    
    QString getProtocolLayer(const QString &messageType);
};

#endif // ADVANCEDFILTER_H