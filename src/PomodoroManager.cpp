#include "PomodoroManager.h"
#include "SettingsManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QUuid>
#include <QUrl>
#include <QtMath>
#include <algorithm>

QVariantMap PomodoroPreset::toMap() const
{
    QVariantMap map;
    map[QStringLiteral("id")] = id;
    map[QStringLiteral("name")] = name;
    map[QStringLiteral("focusDurationMinutes")] = focusDurationMinutes;
    map[QStringLiteral("breakDurationMinutes")] = breakDurationMinutes;
    map[QStringLiteral("isDefault")] = isDefault;
    return map;
}

PomodoroPreset PomodoroPreset::fromMap(const QVariantMap &map)
{
    PomodoroPreset p;
    p.id = map.value(QStringLiteral("id")).toString();
    p.name = map.value(QStringLiteral("name")).toString();
    p.focusDurationMinutes = qBound(1, map.value(QStringLiteral("focusDurationMinutes"), 25).toInt(), 180);
    p.breakDurationMinutes = qBound(1, map.value(QStringLiteral("breakDurationMinutes"), 5).toInt(), 60);
    p.isDefault = map.value(QStringLiteral("isDefault"), false).toBool();
    return p;
}

QVariantMap FocusSessionRecord::toMap() const
{
    QVariantMap map;
    map[QStringLiteral("id")] = id;
    map[QStringLiteral("sessionType")] = sessionType;
    map[QStringLiteral("startTime")] = startTime.toString(Qt::ISODate);
    map[QStringLiteral("endTime")] = endTime.toString(Qt::ISODate);
    map[QStringLiteral("durationSeconds")] = durationSeconds;
    map[QStringLiteral("targetDurationSeconds")] = targetDurationSeconds;
    map[QStringLiteral("completed")] = completed;
    return map;
}

FocusSessionRecord FocusSessionRecord::fromMap(const QVariantMap &map)
{
    FocusSessionRecord r;
    r.id = map.value(QStringLiteral("id")).toString();
    r.sessionType = map.value(QStringLiteral("sessionType"), QStringLiteral("focus")).toString();
    r.startTime = QDateTime::fromString(map.value(QStringLiteral("startTime")).toString(), Qt::ISODate);
    r.endTime = QDateTime::fromString(map.value(QStringLiteral("endTime")).toString(), Qt::ISODate);
    r.durationSeconds = map.value(QStringLiteral("durationSeconds"), 0).toInt();
    r.targetDurationSeconds = map.value(QStringLiteral("targetDurationSeconds"), 0).toInt();
    r.completed = map.value(QStringLiteral("completed"), true).toBool();
    return r;
}

PomodoroManager::PomodoroManager(SettingsManager *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/KDE");
    QDir().mkpath(configDir);
    m_historyStoragePath = configDir + QStringLiteral("/MusicFocus_pomodoro_history.json");

    loadPresets();
    loadHistory();

    // Default to the first preset (Classic: 25/5)
    prepareCurrentPhase();

    m_tickTimer.setInterval(100);
    connect(&m_tickTimer, &QTimer::timeout, this, &PomodoroManager::onTickTimer);

    if (m_settings) {
        connect(m_settings, &SettingsManager::clock24HourChanged, this, &PomodoroManager::historyChanged);
    }
}

qreal PomodoroManager::progress() const
{
    if (m_targetDurationSeconds <= 0) return 0.0;
    qreal prog = 1.0 - (static_cast<qreal>(m_remainingSeconds) / static_cast<qreal>(m_targetDurationSeconds));
    return qBound(0.0, prog, 1.0);
}

QString PomodoroManager::formattedTime() const
{
    int mins = m_remainingSeconds / 60;
    int secs = m_remainingSeconds % 60;
    return QStringLiteral("%1:%2")
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0'));
}

QString PomodoroManager::currentPresetName() const
{
    if (m_activePresetIndex >= 0 && m_activePresetIndex < m_presets.size()) {
        return m_presets[m_activePresetIndex].name;
    }
    return QStringLiteral("Custom");
}

QVariantList PomodoroManager::presetsVariant() const
{
    QVariantList list;
    for (const auto &p : m_presets) {
        list.append(p.toMap());
    }
    return list;
}

void PomodoroManager::setActivePresetIndex(int index)
{
    if (index < 0 || index >= m_presets.size()) {
        return;
    }

    if (m_activePresetIndex == index) {
        return;
    }

    if (m_timerState == Running || m_timerState == Paused) {
        // Queue pending preset for next phase to avoid corrupting active session
        m_pendingPresetIndex = index;
        return;
    }

    m_activePresetIndex = index;
    m_pendingPresetIndex = -1;
    const auto &preset = m_presets[m_activePresetIndex];
    m_focusDurationMinutes = preset.focusDurationMinutes;
    m_breakDurationMinutes = preset.breakDurationMinutes;

    if (m_settings) {
        m_settings->setPomodoroFocusDuration(m_focusDurationMinutes);
        m_settings->setPomodoroBreakDuration(m_breakDurationMinutes);
    }

    prepareCurrentPhase();

    emit activePresetIndexChanged();
    emit durationChanged();
    emit tick();
}

void PomodoroManager::selectPreset(int index)
{
    setActivePresetIndex(index);
}

bool PomodoroManager::addCustomPreset(const QString &name, int focusMinutes, int breakMinutes)
{
    QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) return false;
    focusMinutes = qBound(1, focusMinutes, 180);
    breakMinutes = qBound(1, breakMinutes, 60);

    PomodoroPreset p;
    p.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    p.name = trimmed;
    p.focusDurationMinutes = focusMinutes;
    p.breakDurationMinutes = breakMinutes;
    p.isDefault = false;

    m_presets.append(p);
    savePresets();
    emit presetsChanged();

    // Select the newly created preset
    setActivePresetIndex(m_presets.size() - 1);
    return true;
}

bool PomodoroManager::updateCustomPreset(int index, const QString &name, int focusMinutes, int breakMinutes)
{
    if (index < 0 || index >= m_presets.size()) return false;
    if (m_presets[index].isDefault) return false; // Default presets are protected

    QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) return false;

    m_presets[index].name = trimmed;
    m_presets[index].focusDurationMinutes = qBound(1, focusMinutes, 180);
    m_presets[index].breakDurationMinutes = qBound(1, breakMinutes, 60);

    savePresets();
    emit presetsChanged();

    if (m_activePresetIndex == index) {
        m_focusDurationMinutes = m_presets[index].focusDurationMinutes;
        m_breakDurationMinutes = m_presets[index].breakDurationMinutes;
        prepareCurrentPhase();
        emit durationChanged();
        emit tick();
    }
    return true;
}

bool PomodoroManager::deleteCustomPreset(int index)
{
    if (index < 0 || index >= m_presets.size()) return false;
    if (m_presets[index].isDefault) return false;

    m_presets.removeAt(index);
    if (m_activePresetIndex >= m_presets.size()) {
        m_activePresetIndex = 0;
    }
    savePresets();
    emit presetsChanged();
    emit activePresetIndexChanged();

    const auto &p = m_presets[m_activePresetIndex];
    m_focusDurationMinutes = p.focusDurationMinutes;
    m_breakDurationMinutes = p.breakDurationMinutes;
    prepareCurrentPhase();
    emit durationChanged();
    emit tick();
    return true;
}

void PomodoroManager::restoreDefaultPresets()
{
    initializeDefaultPresets();
    m_activePresetIndex = 0;
    m_pendingPresetIndex = -1;
    savePresets();

    const auto &p = m_presets[0];
    m_focusDurationMinutes = p.focusDurationMinutes;
    m_breakDurationMinutes = p.breakDurationMinutes;
    prepareCurrentPhase();

    emit presetsChanged();
    emit activePresetIndexChanged();
    emit durationChanged();
    emit tick();
}

void PomodoroManager::start()
{
    if (m_timerState == Running) {
        return;
    }

    if (m_timerState == Stopped) {
        m_sessionStartTime = QDateTime::currentDateTime();
        m_elapsedBeforePauseSeconds = 0;
        m_remainingSeconds = m_targetDurationSeconds;
    }

    m_phaseStartSteadyTime = std::chrono::steady_clock::now();
    m_timerState = Running;
    m_tickTimer.start();

    emit timerStateChanged(m_timerState);
    emit tick();
}

void PomodoroManager::pause()
{
    if (m_timerState != Running) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    int currentSegSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - m_phaseStartSteadyTime).count();
    m_elapsedBeforePauseSeconds += currentSegSeconds;

    m_timerState = Paused;
    m_tickTimer.stop();

    emit timerStateChanged(m_timerState);
    emit tick();
}

void PomodoroManager::togglePlayPause()
{
    if (m_timerState == Running) {
        pause();
    } else {
        start();
    }
}

void PomodoroManager::skip()
{
    // End current phase immediately without awarding completed time
    m_timerState = Stopped;
    m_tickTimer.stop();
    m_elapsedBeforePauseSeconds = 0;

    // Switch phase cleanly
    m_sessionType = (m_sessionType == FocusSession) ? BreakSession : FocusSession;
    applyPendingPresetIfNeeded();
    prepareCurrentPhase();

    emit sessionTypeChanged(m_sessionType);
    emit timerStateChanged(m_timerState);
    emit durationChanged();
    emit tick();
}

void PomodoroManager::reset()
{
    // Reset current phase to full duration without logging completion
    m_timerState = Stopped;
    m_tickTimer.stop();
    m_elapsedBeforePauseSeconds = 0;

    applyPendingPresetIfNeeded();
    prepareCurrentPhase();

    emit timerStateChanged(m_timerState);
    emit tick();
}

void PomodoroManager::prepareCurrentPhase()
{
    if (m_sessionType == FocusSession) {
        m_targetDurationSeconds = m_focusDurationMinutes * 60;
    } else {
        m_targetDurationSeconds = m_breakDurationMinutes * 60;
    }
    m_remainingSeconds = m_targetDurationSeconds;
    m_elapsedBeforePauseSeconds = 0;
}

void PomodoroManager::applyPendingPresetIfNeeded()
{
    if (m_pendingPresetIndex >= 0 && m_pendingPresetIndex < m_presets.size()) {
        m_activePresetIndex = m_pendingPresetIndex;
        m_pendingPresetIndex = -1;
        const auto &preset = m_presets[m_activePresetIndex];
        m_focusDurationMinutes = preset.focusDurationMinutes;
        m_breakDurationMinutes = preset.breakDurationMinutes;
        if (m_settings) {
            m_settings->setPomodoroFocusDuration(m_focusDurationMinutes);
            m_settings->setPomodoroBreakDuration(m_breakDurationMinutes);
        }
        emit activePresetIndexChanged();
        emit durationChanged();
    }
}

void PomodoroManager::onTickTimer()
{
    if (m_timerState != Running) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    int currentSegSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - m_phaseStartSteadyTime).count();
    int totalElapsed = m_elapsedBeforePauseSeconds + currentSegSeconds;

    int remaining = m_targetDurationSeconds - totalElapsed;
    if (remaining <= 0) {
        m_remainingSeconds = 0;
        emit tick();
        handlePhaseCompleted();
    } else {
        if (m_remainingSeconds != remaining) {
            m_remainingSeconds = remaining;
            emit tick();
        }
    }
}

void PomodoroManager::handlePhaseCompleted()
{
    if (m_completingPhase) return;
    m_completingPhase = true;

    m_timerState = Stopped;
    m_tickTimer.stop();
    m_elapsedBeforePauseSeconds = 0;

    int completedDuration = m_targetDurationSeconds;
    int completedPhase = m_sessionType;

    // Log Focus phase if completed
    if (completedPhase == FocusSession) {
        FocusSessionRecord record;
        record.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        record.sessionType = QStringLiteral("focus");
        record.startTime = m_sessionStartTime.isValid() ? m_sessionStartTime : QDateTime::currentDateTime().addSecs(-completedDuration);
        record.endTime = QDateTime::currentDateTime();
        record.durationSeconds = completedDuration;
        record.targetDurationSeconds = completedDuration;
        record.completed = true;

        addHistoryRecord(record);
    }

    // Play completion sound
    playCompletionSound();

    emit sessionCompleted(completedPhase, completedDuration);

    // Transition to next phase
    m_sessionType = (completedPhase == FocusSession) ? BreakSession : FocusSession;
    applyPendingPresetIfNeeded();
    prepareCurrentPhase();

    emit sessionTypeChanged(m_sessionType);
    emit timerStateChanged(m_timerState);
    emit durationChanged();
    emit tick();

    m_completingPhase = false;

    // Auto-start next phase if configured
    if (m_sessionType == BreakSession && m_settings && m_settings->pomodoroAutoStartBreaks()) {
        start();
    } else if (m_sessionType == FocusSession && m_settings && m_settings->pomodoroAutoStartFocus()) {
        start();
    }
}

void PomodoroManager::playCompletionSound()
{
    if (m_settings && !m_settings->pomodoroSoundEnabled()) {
        return;
    }
    int style = m_settings ? m_settings->pomodoroSoundStyle() : 0;
    qreal vol = m_settings ? m_settings->pomodoroSoundVolume() : 0.8;
    previewSound(style, vol);
}

void PomodoroManager::previewSound(int soundStyle, qreal volume)
{
    QString soundPath = ensureSoundFile(soundStyle);
    if (soundPath.isEmpty() || !QFile::exists(soundPath)) {
        return;
    }

    m_soundEffect.stop();
    m_soundEffect.setSource(QUrl::fromLocalFile(soundPath));
    m_soundEffect.setVolume(qBound(0.0, volume, 1.0));
    m_soundEffect.play();
}

QString PomodoroManager::ensureSoundFile(int soundStyle)
{
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/sounds");
    QDir().mkpath(dirPath);

    QString fileName;
    switch (soundStyle) {
    case 1: fileName = QStringLiteral("chime.wav"); break;
    case 2: fileName = QStringLiteral("digital.wav"); break;
    case 3: fileName = QStringLiteral("bell.wav"); break;
    case 0:
    default: fileName = QStringLiteral("three_beeps.wav"); break;
    }

    QString fullPath = dirPath + QStringLiteral("/") + fileName;
    if (QFile::exists(fullPath)) {
        return fullPath;
    }

    // Generate synthesized audio file
    const int sampleRate = 44100;
    QByteArray pcmData;

    auto appendTone = [&](double freq, int durationMs, double maxAmp, bool applyEnvelope) {
        int totalSamples = (sampleRate * durationMs) / 1000;
        int rampSamples = applyEnvelope ? std::min(totalSamples / 4, sampleRate * 5 / 1000) : 0; // 5ms ramp

        for (int i = 0; i < totalSamples; ++i) {
            double t = static_cast<double>(i) / sampleRate;
            double sample = std::sin(2.0 * M_PI * freq * t);

            // Envelope to avoid digital clicks/pops
            double env = 1.0;
            if (applyEnvelope) {
                if (i < rampSamples) {
                    env = static_cast<double>(i) / rampSamples;
                } else if (i > totalSamples - rampSamples) {
                    env = static_cast<double>(totalSamples - i) / rampSamples;
                }
            }

            int16_t val = static_cast<int16_t>(sample * maxAmp * env * 32767.0);
            pcmData.append(reinterpret_cast<const char*>(&val), sizeof(int16_t));
        }
    };

    auto appendSilence = [&](int durationMs) {
        int totalSamples = (sampleRate * durationMs) / 1000;
        pcmData.append(totalSamples * sizeof(int16_t), '\0');
    };

    if (soundStyle == 0) {
        // Default: Three clearly separated short beeps (880Hz, 120ms beep + 75ms silence)
        for (int i = 0; i < 3; ++i) {
            appendTone(880.0, 120, 0.75, true);
            if (i < 2) {
                appendSilence(75);
            }
        }
    } else if (soundStyle == 1) {
        // Harmonic Chime (C5 523Hz, E5 659Hz, G5 784Hz) with gentle exponential decay
        int durationMs = 1200;
        int totalSamples = (sampleRate * durationMs) / 1000;
        for (int i = 0; i < totalSamples; ++i) {
            double t = static_cast<double>(i) / sampleRate;
            double decay = std::exp(-3.2 * t);
            double s1 = std::sin(2.0 * M_PI * 523.25 * t) * 0.45;
            double s2 = std::sin(2.0 * M_PI * 659.25 * t) * 0.35;
            double s3 = std::sin(2.0 * M_PI * 783.99 * t) * 0.20;
            double sample = (s1 + s2 + s3) * decay;
            int16_t val = static_cast<int16_t>(sample * 32767.0);
            pcmData.append(reinterpret_cast<const char*>(&val), sizeof(int16_t));
        }
    } else if (soundStyle == 2) {
        // Digital Pulse (high double chirp: 1046Hz 100ms, silence 50ms, 1318Hz 140ms)
        appendTone(1046.5, 100, 0.70, true);
        appendSilence(50);
        appendTone(1318.5, 140, 0.70, true);
    } else {
        // Soft Bell (440Hz warm meditation bell chime with decay)
        int durationMs = 1100;
        int totalSamples = (sampleRate * durationMs) / 1000;
        for (int i = 0; i < totalSamples; ++i) {
            double t = static_cast<double>(i) / sampleRate;
            double decay = std::exp(-2.8 * t);
            double sample = std::sin(2.0 * M_PI * 440.0 * t) * decay * 0.85;
            int16_t val = static_cast<int16_t>(sample * 32767.0);
            pcmData.append(reinterpret_cast<const char*>(&val), sizeof(int16_t));
        }
    }

    // Write WAV header and PCM data
    QFile file(fullPath);
    if (!file.open(QIODevice::WriteOnly)) {
        return QString();
    }

    uint32_t dataSize = static_cast<uint32_t>(pcmData.size());
    uint32_t chunkSize = 36 + dataSize;
    uint32_t subchunk1Size = 16;
    uint16_t audioFormat = 1; // PCM
    uint16_t numChannels = 1; // Mono
    uint32_t sRate = sampleRate;
    uint32_t byteRate = sampleRate * 1 * sizeof(int16_t);
    uint16_t blockAlign = 1 * sizeof(int16_t);
    uint16_t bitsPerSample = 16;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&chunkSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    file.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
    file.write(pcmData);
    file.close();

    return fullPath;
}

void PomodoroManager::loadPresets()
{
    QSettings rawSettings(QStringLiteral("KDE"), QStringLiteral("MusicFocus"));
    if (rawSettings.contains(QStringLiteral("pomodoro/presets"))) {
        QByteArray data = rawSettings.value(QStringLiteral("pomodoro/presets")).toByteArray();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            m_presets.clear();
            const auto arr = doc.array();
            for (const auto &v : arr) {
                m_presets.append(PomodoroPreset::fromMap(v.toObject().toVariantMap()));
            }
        }
    }

    if (m_presets.isEmpty()) {
        initializeDefaultPresets();
        savePresets();
    }

    m_activePresetIndex = qBound(0, rawSettings.value(QStringLiteral("pomodoro/activePresetIndex"), 0).toInt(), m_presets.size() - 1);
    const auto &p = m_presets[m_activePresetIndex];
    m_focusDurationMinutes = p.focusDurationMinutes;
    m_breakDurationMinutes = p.breakDurationMinutes;
}

void PomodoroManager::savePresets()
{
    QSettings rawSettings(QStringLiteral("KDE"), QStringLiteral("MusicFocus"));
    QJsonArray arr;
    for (const auto &p : m_presets) {
        arr.append(QJsonObject::fromVariantMap(p.toMap()));
    }
    rawSettings.setValue(QStringLiteral("pomodoro/presets"), QJsonDocument(arr).toJson(QJsonDocument::Compact));
    rawSettings.setValue(QStringLiteral("pomodoro/activePresetIndex"), m_activePresetIndex);
    rawSettings.sync();
}

void PomodoroManager::initializeDefaultPresets()
{
    m_presets.clear();

    PomodoroPreset classic;
    classic.id = QStringLiteral("classic");
    classic.name = QStringLiteral("Classic (25 / 5)");
    classic.focusDurationMinutes = 25;
    classic.breakDurationMinutes = 5;
    classic.isDefault = true;

    PomodoroPreset extended;
    extended.id = QStringLiteral("extended");
    extended.name = QStringLiteral("Extended (50 / 10)");
    extended.focusDurationMinutes = 50;
    extended.breakDurationMinutes = 10;
    extended.isDefault = true;

    PomodoroPreset sprint;
    sprint.id = QStringLiteral("sprint");
    sprint.name = QStringLiteral("Short Sprint (15 / 3)");
    sprint.focusDurationMinutes = 15;
    sprint.breakDurationMinutes = 3;
    sprint.isDefault = true;

    m_presets.append(classic);
    m_presets.append(extended);
    m_presets.append(sprint);
}

void PomodoroManager::loadHistory()
{
    m_historyRecords.clear();
    QFile file(m_historyStoragePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isArray()) {
        const auto arr = doc.array();
        for (const auto &v : arr) {
            FocusSessionRecord r = FocusSessionRecord::fromMap(v.toObject().toVariantMap());
            if (r.completed && r.sessionType == QStringLiteral("focus") && r.durationSeconds > 0) {
                m_historyRecords.append(r);
            }
        }
    }
}

void PomodoroManager::saveHistory()
{
    QJsonArray arr;
    for (const auto &r : m_historyRecords) {
        arr.append(QJsonObject::fromVariantMap(r.toMap()));
    }

    QFile file(m_historyStoragePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
        file.close();
    }
}

void PomodoroManager::addHistoryRecord(const FocusSessionRecord &record)
{
    m_historyRecords.append(record);
    saveHistory();
    emit historyChanged();
}

void PomodoroManager::clearHistory()
{
    m_historyRecords.clear();
    saveHistory();
    emit historyChanged();
}

int PomodoroManager::todayFocusSeconds() const
{
    QDate today = QDate::currentDate();
    int total = 0;
    for (const auto &r : m_historyRecords) {
        // Cross-midnight sessions attributed consistently to local start date
        if (r.startTime.toLocalTime().date() == today) {
            total += r.durationSeconds;
        }
    }
    return total;
}

int PomodoroManager::todaySessionCount() const
{
    QDate today = QDate::currentDate();
    int count = 0;
    for (const auto &r : m_historyRecords) {
        if (r.startTime.toLocalTime().date() == today) {
            count++;
        }
    }
    return count;
}

QString PomodoroManager::todayFormattedFocusTime() const
{
    return formatDuration(todayFocusSeconds());
}

QString PomodoroManager::formatDuration(int totalSeconds)
{
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;

    if (hours > 0 && minutes > 0) {
        return QStringLiteral("%1h %2m").arg(hours).arg(minutes);
    } else if (hours > 0) {
        return QStringLiteral("%1h").arg(hours);
    } else if (minutes > 0) {
        return QStringLiteral("%1m").arg(minutes);
    } else if (totalSeconds > 0) {
        return QStringLiteral("%1s").arg(totalSeconds);
    }
    return QStringLiteral("0m");
}

QString PomodoroManager::formatTimestamp(const QDateTime &dt) const
{
    if (!dt.isValid()) return QString();
    bool is24 = m_settings ? m_settings->clock24Hour() : true;
    return dt.toLocalTime().toString(is24 ? QStringLiteral("hh:mm") : QStringLiteral("hh:mm AP"));
}

QVariantList PomodoroManager::last7DaysData() const
{
    QVariantList result;
    QDate today = QDate::currentDate();

    // Map sessions to local start dates
    QMap<QDate, QList<FocusSessionRecord>> dailyMap;
    for (const auto &r : m_historyRecords) {
        QDate d = r.startTime.toLocalTime().date();
        dailyMap[d].append(r);
    }

    for (int i = 6; i >= 0; --i) {
        QDate date = today.addDays(-i);
        QVariantMap dayMap;
        dayMap[QStringLiteral("date")] = date.toString(Qt::ISODate);

        QString label;
        if (date == today) {
            label = QStringLiteral("Today, %1").arg(date.toString(QStringLiteral("d MMM")));
        } else if (date == today.addDays(-1)) {
            label = QStringLiteral("Yesterday, %1").arg(date.toString(QStringLiteral("d MMM")));
        } else {
            label = date.toString(QStringLiteral("dddd, d MMM"));
        }
        dayMap[QStringLiteral("dayLabel")] = label;

        int focusSecs = 0;
        int sessionCount = 0;
        QVariantList sessionList;

        if (dailyMap.contains(date)) {
            const auto &records = dailyMap[date];
            sessionCount = records.size();
            for (const auto &r : records) {
                focusSecs += r.durationSeconds;
                QVariantMap sm;
                sm[QStringLiteral("id")] = r.id;
                sm[QStringLiteral("startTime")] = formatTimestamp(r.startTime);
                sm[QStringLiteral("endTime")] = formatTimestamp(r.endTime);
                sm[QStringLiteral("timeRange")] = QStringLiteral("%1 – %2")
                    .arg(formatTimestamp(r.startTime))
                    .arg(formatTimestamp(r.endTime));
                sm[QStringLiteral("duration")] = formatDuration(r.durationSeconds);
                sessionList.append(sm);
            }
        }

        dayMap[QStringLiteral("focusSeconds")] = focusSecs;
        dayMap[QStringLiteral("formattedFocusTime")] = formatDuration(focusSecs);
        dayMap[QStringLiteral("sessionCount")] = sessionCount;
        dayMap[QStringLiteral("sessions")] = sessionList;

        result.append(dayMap);
    }

    return result;
}

QVariantMap PomodoroManager::monthData(int year, int month) const
{
    QVariantMap res;
    if (year <= 0 || month < 1 || month > 12) {
        QDate now = QDate::currentDate();
        year = now.year();
        month = now.month();
    }

    QDate firstOfMonth(year, month, 1);
    int daysInMonth = firstOfMonth.daysInMonth();
    int firstDayOfWeek = firstOfMonth.dayOfWeek(); // 1 = Monday, 7 = Sunday
    QDate today = QDate::currentDate();

    res[QStringLiteral("year")] = year;
    res[QStringLiteral("month")] = month;
    res[QStringLiteral("monthName")] = firstOfMonth.toString(QStringLiteral("MMMM"));
    res[QStringLiteral("daysInMonth")] = daysInMonth;
    res[QStringLiteral("firstDayOfWeek")] = firstDayOfWeek;

    QMap<int, int> daySecondsMap;
    QMap<int, int> dayCountMap;
    for (const auto &r : m_historyRecords) {
        QDate d = r.startTime.toLocalTime().date();
        if (d.year() == year && d.month() == month) {
            daySecondsMap[d.day()] += r.durationSeconds;
            dayCountMap[d.day()] += 1;
        }
    }

    QVariantList daysList;
    for (int day = 1; day <= daysInMonth; ++day) {
        QDate date(year, month, day);
        QVariantMap dm;
        dm[QStringLiteral("day")] = day;
        dm[QStringLiteral("dateString")] = date.toString(Qt::ISODate);

        int secs = daySecondsMap.value(day, 0);
        int count = dayCountMap.value(day, 0);
        dm[QStringLiteral("focusSeconds")] = secs;
        dm[QStringLiteral("sessionCount")] = count;
        dm[QStringLiteral("formattedFocusTime")] = secs > 0 ? formatDuration(secs) : QString();
        dm[QStringLiteral("isToday")] = (date == today);

        // Heatmap scale:
        // Less than 2 hours: 0
        // 2 hours (2h <= t < 4h): 1
        // 4 hours (4h <= t < 6h): 2
        // 6 hours (6h <= t < 8h): 3
        // 8 hours (8h <= t < 10h): 4
        // 10 hours or more: 5
        int intensity = 0;
        int hours = secs / 3600;
        if (hours >= 10) intensity = 5;
        else if (hours >= 8) intensity = 4;
        else if (hours >= 6) intensity = 3;
        else if (hours >= 4) intensity = 2;
        else if (hours >= 2) intensity = 1;
        else intensity = 0;

        dm[QStringLiteral("heatmapIntensity")] = intensity;
        daysList.append(dm);
    }

    res[QStringLiteral("days")] = daysList;
    return res;
}

QVariantList PomodoroManager::sessionsForDate(const QString &dateString) const
{
    QVariantList list;
    QDate targetDate = QDate::fromString(dateString, Qt::ISODate);
    if (!targetDate.isValid()) {
        return list;
    }

    for (const auto &r : m_historyRecords) {
        if (r.startTime.toLocalTime().date() == targetDate) {
            QVariantMap sm;
            sm[QStringLiteral("id")] = r.id;
            sm[QStringLiteral("startTime")] = formatTimestamp(r.startTime);
            sm[QStringLiteral("endTime")] = formatTimestamp(r.endTime);
            sm[QStringLiteral("timeRange")] = QStringLiteral("%1 – %2")
                .arg(formatTimestamp(r.startTime))
                .arg(formatTimestamp(r.endTime));
            sm[QStringLiteral("duration")] = formatDuration(r.durationSeconds);
            sm[QStringLiteral("durationSeconds")] = r.durationSeconds;
            list.append(sm);
        }
    }

    return list;
}
