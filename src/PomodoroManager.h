#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QTimer>
#include <QSoundEffect>
#include <chrono>

class SettingsManager;

struct PomodoroPreset {
    QString id;
    QString name;
    int focusDurationMinutes{25};
    int breakDurationMinutes{5};
    bool isDefault{false};

    QVariantMap toMap() const;
    static PomodoroPreset fromMap(const QVariantMap &map);
};

struct FocusSessionRecord {
    QString id;
    QString sessionType; // "focus"
    QDateTime startTime;
    QDateTime endTime;
    int durationSeconds{0};
    int targetDurationSeconds{0};
    bool completed{true};

    QVariantMap toMap() const;
    static FocusSessionRecord fromMap(const QVariantMap &map);
};

class PomodoroManager : public QObject
{
    Q_OBJECT

    // Phase & Timer State
    Q_PROPERTY(int sessionType READ sessionType NOTIFY sessionTypeChanged)
    Q_PROPERTY(QString sessionTypeName READ sessionTypeName NOTIFY sessionTypeChanged)
    Q_PROPERTY(int timerState READ timerState NOTIFY timerStateChanged)
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY timerStateChanged)
    Q_PROPERTY(bool isPaused READ isPaused NOTIFY timerStateChanged)
    Q_PROPERTY(bool isStopped READ isStopped NOTIFY timerStateChanged)

    // Countdown Time Values
    Q_PROPERTY(int remainingSeconds READ remainingSeconds NOTIFY tick)
    Q_PROPERTY(int totalSeconds READ totalSeconds NOTIFY durationChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY tick)
    Q_PROPERTY(QString formattedTime READ formattedTime NOTIFY tick)

    // Presets
    Q_PROPERTY(int activePresetIndex READ activePresetIndex WRITE setActivePresetIndex NOTIFY activePresetIndexChanged)
    Q_PROPERTY(QString currentPresetName READ currentPresetName NOTIFY activePresetIndexChanged)
    Q_PROPERTY(int focusDurationMinutes READ focusDurationMinutes NOTIFY durationChanged)
    Q_PROPERTY(int breakDurationMinutes READ breakDurationMinutes NOTIFY durationChanged)
    Q_PROPERTY(QVariantList presets READ presetsVariant NOTIFY presetsChanged)

    // Today's Focus Metrics
    Q_PROPERTY(int todayFocusSeconds READ todayFocusSeconds NOTIFY historyChanged)
    Q_PROPERTY(int todaySessionCount READ todaySessionCount NOTIFY historyChanged)
    Q_PROPERTY(QString todayFormattedFocusTime READ todayFormattedFocusTime NOTIFY historyChanged)

public:
    enum SessionType {
        FocusSession = 0,
        BreakSession = 1
    };
    Q_ENUM(SessionType)

    enum TimerState {
        Stopped = 0,
        Running = 1,
        Paused = 2
    };
    Q_ENUM(TimerState)

    explicit PomodoroManager(SettingsManager *settings, QObject *parent = nullptr);
    ~PomodoroManager() override = default;

    int sessionType() const { return m_sessionType; }
    QString sessionTypeName() const { return m_sessionType == FocusSession ? QStringLiteral("Focus") : QStringLiteral("Break"); }
    int timerState() const { return m_timerState; }
    bool isRunning() const { return m_timerState == Running; }
    bool isPaused() const { return m_timerState == Paused; }
    bool isStopped() const { return m_timerState == Stopped; }

    int remainingSeconds() const { return m_remainingSeconds; }
    int totalSeconds() const { return m_targetDurationSeconds; }
    qreal progress() const;
    QString formattedTime() const;

    int activePresetIndex() const { return m_activePresetIndex; }
    void setActivePresetIndex(int index);
    QString currentPresetName() const;
    int focusDurationMinutes() const { return m_focusDurationMinutes; }
    int breakDurationMinutes() const { return m_breakDurationMinutes; }
    QVariantList presetsVariant() const;

    // Today's summary metrics
    int todayFocusSeconds() const;
    int todaySessionCount() const;
    QString todayFormattedFocusTime() const;

    // Timer Controls
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePlayPause();
    Q_INVOKABLE void skip();
    Q_INVOKABLE void reset();

    // Preset management
    Q_INVOKABLE void selectPreset(int index);
    Q_INVOKABLE bool addCustomPreset(const QString &name, int focusMinutes, int breakMinutes);
    Q_INVOKABLE bool updateCustomPreset(int index, const QString &name, int focusMinutes, int breakMinutes);
    Q_INVOKABLE bool deleteCustomPreset(int index);
    Q_INVOKABLE void restoreDefaultPresets();

    // Sound Controls
    Q_INVOKABLE void playCompletionSound();
    Q_INVOKABLE void previewSound(int soundStyle, qreal volume);

    // History Queries
    Q_INVOKABLE QVariantList last7DaysData() const;
    Q_INVOKABLE QVariantMap monthData(int year, int month) const;
    Q_INVOKABLE QVariantList sessionsForDate(const QString &dateString) const;
    Q_INVOKABLE void clearHistory();

signals:
    void sessionTypeChanged(int sessionType);
    void timerStateChanged(int timerState);
    void tick();
    void durationChanged();
    void activePresetIndexChanged();
    void presetsChanged();
    void historyChanged();
    void sessionCompleted(int sessionType, int durationSeconds);

private slots:
    void onTickTimer();

private:
    SettingsManager *m_settings;
    QTimer m_tickTimer;
    QSoundEffect m_soundEffect;

    SessionType m_sessionType{FocusSession};
    TimerState m_timerState{Stopped};

    int m_focusDurationMinutes{25};
    int m_breakDurationMinutes{5};
    int m_targetDurationSeconds{1500};
    int m_remainingSeconds{1500};
    int m_elapsedBeforePauseSeconds{0};

    // Steady clock anchors for bulletproof monotonic elapsed time
    std::chrono::steady_clock::time_point m_phaseStartSteadyTime;
    QDateTime m_sessionStartTime;

    int m_activePresetIndex{0};
    int m_pendingPresetIndex{-1};
    QList<PomodoroPreset> m_presets;

    QString m_historyStoragePath;
    QList<FocusSessionRecord> m_historyRecords;
    bool m_completingPhase{false};

    void prepareCurrentPhase();
    void handlePhaseCompleted();
    void applyPendingPresetIfNeeded();

    void loadPresets();
    void savePresets();
    void initializeDefaultPresets();

    void loadHistory();
    void saveHistory();
    void addHistoryRecord(const FocusSessionRecord &record);

    QString ensureSoundFile(int soundStyle);
    static QString formatDuration(int totalSeconds);
    QString formatTimestamp(const QDateTime &dt) const;
};
