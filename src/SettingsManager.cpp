#include "SettingsManager.h"
#include <QFileInfo>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QFont>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent)
    , m_settings("KDE", "MusicFocus")
{
    loadSettings();
}

void SettingsManager::loadSettings()
{
    m_musicDirectory = m_settings.value("library/musicDirectory", QString()).toString();
    if (m_settings.contains("library/onboardingCompleted")) {
        m_onboardingCompleted = m_settings.value("library/onboardingCompleted", false).toBool();
    } else {
        m_onboardingCompleted = !m_musicDirectory.trimmed().isEmpty() && QDir(m_musicDirectory).exists();
    }
    
    // Migrate legacy wallpaperMode (old 0: Still, old 1: SolidColor) to new (0: Gradient, 1: Still, 2: Video)
    bool migrated = m_settings.value("appearance/wallpaperModeMigratedV2", false).toBool();
    if (!migrated && m_settings.contains("appearance/wallpaperMode")) {
        int oldVal = m_settings.value("appearance/wallpaperMode").toInt();
        if (oldVal == 0) {
            m_wallpaperMode = StillImagesMode; // 1
        } else if (oldVal == 1) {
            m_wallpaperMode = GradientMode; // 0
        } else {
            m_wallpaperMode = oldVal;
        }
        m_settings.setValue("appearance/wallpaperMode", m_wallpaperMode);
        m_settings.setValue("appearance/wallpaperModeMigratedV2", true);
        m_settings.sync();
    } else {
        m_wallpaperMode = m_settings.value("appearance/wallpaperMode", StillImagesMode).toInt();
    }

    m_stillAutoRotate = m_settings.value("appearance/stillAutoRotate", true).toBool();
    m_rotateIntervalMinutes = m_settings.value("appearance/rotateIntervalMinutes", 5).toInt();
    m_fixedWallpaper = m_settings.value("appearance/fixedWallpaper", QString()).toString();
    m_solidColor = m_settings.value("appearance/solidColor", QColor(24, 25, 38)).value<QColor>();
    m_gradientSeed = m_settings.value("appearance/gradientSeed", 42).toInt();
    m_gradientStyle = m_settings.value("appearance/gradientStyle", 0).toInt();
    m_videoWallpaper = m_settings.value("appearance/videoWallpaper", QString()).toString();

    m_dimmingOpacity = m_settings.value("appearance/dimmingOpacity", 0.25).toReal();
    m_overlayOpacity = m_settings.value("appearance/overlayOpacity", 0.85).toReal();
    m_blurEnabled = m_settings.value("appearance/blurEnabled", true).toBool();
    m_blurStrength = qBound(0.0, m_settings.value("appearance/blurStrength", 0.65).toReal(), 1.0);
    m_darkeningEnabled = m_settings.value("appearance/darkeningEnabled", true).toBool();
    m_darkeningOpacity = qBound(0.0, m_settings.value("appearance/darkeningOpacity", 0.75).toReal(), 1.0);

    m_playlistScale = qBound(0.70, m_settings.value("appearance/playlistScale", 1.0).toReal(), 1.50);
    m_playbackScale = qBound(0.70, m_settings.value("appearance/playbackStripScale",
                      m_settings.value("appearance/playbackScale", 1.0)).toReal(), 1.50);
    m_topControlsScale = qBound(0.70, m_settings.value("appearance/topControlsScale", 1.0).toReal(), 1.50);
    m_todoScale = qBound(0.70, m_settings.value("appearance/todoScale", 1.20).toReal(), 1.50);
    m_settingsDialogWidth = qBound(720, m_settings.value("appearance/settingsDialogWidth", 960).toInt(), 1600);
    m_settingsDialogHeight = qBound(500, m_settings.value("appearance/settingsDialogHeight", 680).toInt(), 1200);
    m_contrastMode = qBound(0, m_settings.value("appearance/contrastMode", ContrastAuto).toInt(), 2);
    m_fontFamily = m_settings.value("appearance/fontFamily", QString()).toString();
    m_fontSize = qBound(9, m_settings.value("appearance/fontSize", 11).toInt(), 20);

    if (!m_fontFamily.trimmed().isEmpty()) {
        QFont f = QGuiApplication::font();
        f.setFamily(m_fontFamily);
        f.setPointSize(m_fontSize);
        QGuiApplication::setFont(f);
    }

    m_volume = m_settings.value("playback/volume", 0.75).toReal();
    m_shuffle = m_settings.value("playback/shuffle", false).toBool();
    m_repeatMode = m_settings.value("playback/repeatMode", RepeatAll).toInt();
    m_autoPlay = m_settings.value("playback/autoPlay", false).toBool();

    m_rememberLastTrack = m_settings.value("behavior/rememberLastTrack", true).toBool();
    m_lastTrackPath = m_settings.value("behavior/lastTrackPath", QString()).toString();
    m_resumePlaybackPosition = m_settings.value("behavior/resumePlaybackPosition", false).toBool();
    m_lastPositionMs = m_settings.value("behavior/lastPositionMs", 0).toLongLong();

    m_inactivityEnabled = m_settings.value("behavior/inactivityEnabled",
                          m_settings.value("behavior/autoHideOverlays", true)).toBool();
    m_inactivityTimeoutSeconds = m_settings.value("behavior/inactivityTimeoutSeconds",
                                 m_settings.value("behavior/autoHideTimeoutSeconds", 5)).toInt();
    m_inactivityMode = m_settings.value("behavior/inactivityMode", InactivityFade).toInt();
    m_inactivityFadeOpacity = qBound(0.02, m_settings.value("behavior/inactivityFadeOpacity", 0.15).toReal(), 0.80);

    m_visualizerEnabled = m_settings.value("visualizer/enabled", true).toBool();
    m_visualizerTheme = m_settings.value("visualizer/theme", ClassicBars).toInt();
    m_visualizerOpacity = m_settings.value("visualizer/opacity", 0.65).toReal();
    m_visualizerPosition = m_settings.value("visualizer/position", PositionBottom).toInt();
    m_visualizerHeight = m_settings.value("visualizer/height", 120).toInt();
    m_visualizerBarDensity = qBound(16, m_settings.value("visualizer/barDensity", 64).toInt(), 128);
    m_visualizerSensitivity = qBound(0.50, m_settings.value("visualizer/sensitivity", 1.0).toReal(), 2.00);
    m_visualizerSpeed = qBound(0.40, m_settings.value("visualizer/speed", 0.8).toReal(), 2.00);

    m_todoEnabled = m_settings.value("todo/enabled", true).toBool();
    m_todoVisible = m_settings.value("todo/visible", false).toBool();

    m_clockEnabled = m_settings.value("clock/enabled", true).toBool();
    m_clockVisible = m_settings.value("clock/visible", false).toBool();
    m_clockShowSeconds = m_settings.value("clock/showSeconds", false).toBool();
    m_clockShowDate = m_settings.value("clock/showDate", true).toBool();
    m_clock24Hour = m_settings.value("clock/is24Hour", true).toBool();

    m_pomodoroEnabled = m_settings.value("pomodoro/enabled", true).toBool();
    m_pomodoroVisible = m_settings.value("pomodoro/visible", false).toBool();
    m_pomodoroFocusDuration = m_settings.value("pomodoro/focusDuration", 25).toInt();
    m_pomodoroBreakDuration = m_settings.value("pomodoro/breakDuration", 5).toInt();
    m_pomodoroBreathingEnabled = m_settings.value("pomodoro/breathingEnabled", true).toBool();
    m_pomodoroBreathingSpeed = m_settings.value("pomodoro/breathingSpeed", 1.0).toReal();
    m_pomodoroBreathingIntensity = m_settings.value("pomodoro/breathingIntensity", 0.7).toReal();
    m_pomodoroSoundEnabled = m_settings.value("pomodoro/soundEnabled", true).toBool();
    m_pomodoroSoundStyle = m_settings.value("pomodoro/soundStyle", 0).toInt();
    m_pomodoroSoundVolume = m_settings.value("pomodoro/soundVolume", 0.8).toReal();
    m_pomodoroAutoStartBreaks = m_settings.value("pomodoro/autoStartBreaks", false).toBool();
    m_pomodoroAutoStartFocus = m_settings.value("pomodoro/autoStartFocus", false).toBool();
    m_pomodoroScale = qBound(0.70, m_settings.value("pomodoro/scale", 1.0).toReal(), 1.50);
}

void SettingsManager::save()
{
    m_settings.setValue("library/musicDirectory", m_musicDirectory);
    m_settings.setValue("library/onboardingCompleted", m_onboardingCompleted);
    m_settings.setValue("appearance/wallpaperMode", m_wallpaperMode);
    m_settings.setValue("appearance/stillAutoRotate", m_stillAutoRotate);
    m_settings.setValue("appearance/rotateIntervalMinutes", m_rotateIntervalMinutes);
    m_settings.setValue("appearance/fixedWallpaper", m_fixedWallpaper);
    m_settings.setValue("appearance/solidColor", m_solidColor);
    m_settings.setValue("appearance/gradientSeed", m_gradientSeed);
    m_settings.setValue("appearance/gradientStyle", m_gradientStyle);
    m_settings.setValue("appearance/videoWallpaper", m_videoWallpaper);
    m_settings.setValue("appearance/dimmingOpacity", m_dimmingOpacity);
    m_settings.setValue("appearance/overlayOpacity", m_overlayOpacity);
    m_settings.setValue("appearance/blurEnabled", m_blurEnabled);
    m_settings.setValue("appearance/blurStrength", m_blurStrength);
    m_settings.setValue("appearance/darkeningEnabled", m_darkeningEnabled);
    m_settings.setValue("appearance/darkeningOpacity", m_darkeningOpacity);
    m_settings.setValue("appearance/playlistScale", m_playlistScale);
    m_settings.setValue("appearance/playbackScale", m_playbackScale);
    m_settings.setValue("appearance/topControlsScale", m_topControlsScale);
    m_settings.setValue("appearance/todoScale", m_todoScale);
    m_settings.setValue("appearance/fontFamily", m_fontFamily);
    m_settings.setValue("appearance/fontSize", m_fontSize);
    m_settings.setValue("behavior/inactivityFadeOpacity", m_inactivityFadeOpacity);

    m_settings.setValue("playback/volume", m_volume);
    m_settings.setValue("playback/shuffle", m_shuffle);
    m_settings.setValue("playback/repeatMode", m_repeatMode);
    m_settings.setValue("playback/autoPlay", m_autoPlay);

    m_settings.setValue("behavior/rememberLastTrack", m_rememberLastTrack);
    m_settings.setValue("behavior/lastTrackPath", m_lastTrackPath);
    m_settings.setValue("behavior/resumePlaybackPosition", m_resumePlaybackPosition);
    m_settings.setValue("behavior/lastPositionMs", m_lastPositionMs);
    m_settings.setValue("behavior/inactivityEnabled", m_inactivityEnabled);
    m_settings.setValue("behavior/autoHideOverlays", m_inactivityEnabled);
    m_settings.setValue("behavior/inactivityTimeoutSeconds", m_inactivityTimeoutSeconds);
    m_settings.setValue("behavior/autoHideTimeoutSeconds", m_inactivityTimeoutSeconds);
    m_settings.setValue("behavior/inactivityMode", m_inactivityMode);

    m_settings.setValue("visualizer/enabled", m_visualizerEnabled);
    m_settings.setValue("visualizer/theme", m_visualizerTheme);
    m_settings.setValue("visualizer/opacity", m_visualizerOpacity);
    m_settings.setValue("visualizer/position", m_visualizerPosition);
    m_settings.setValue("visualizer/height", m_visualizerHeight);
    m_settings.setValue("visualizer/barDensity", m_visualizerBarDensity);
    m_settings.setValue("visualizer/sensitivity", m_visualizerSensitivity);
    m_settings.setValue("visualizer/speed", m_visualizerSpeed);

    m_settings.setValue("todo/enabled", m_todoEnabled);
    m_settings.setValue("todo/visible", m_todoVisible);
    m_settings.setValue("clock/enabled", m_clockEnabled);
    m_settings.setValue("clock/visible", m_clockVisible);
    m_settings.setValue("clock/showSeconds", m_clockShowSeconds);
    m_settings.setValue("clock/showDate", m_clockShowDate);
    m_settings.setValue("clock/is24Hour", m_clock24Hour);

    m_settings.setValue("pomodoro/enabled", m_pomodoroEnabled);
    m_settings.setValue("pomodoro/visible", m_pomodoroVisible);
    m_settings.setValue("pomodoro/focusDuration", m_pomodoroFocusDuration);
    m_settings.setValue("pomodoro/breakDuration", m_pomodoroBreakDuration);
    m_settings.setValue("pomodoro/breathingEnabled", m_pomodoroBreathingEnabled);
    m_settings.setValue("pomodoro/breathingSpeed", m_pomodoroBreathingSpeed);
    m_settings.setValue("pomodoro/breathingIntensity", m_pomodoroBreathingIntensity);
    m_settings.setValue("pomodoro/soundEnabled", m_pomodoroSoundEnabled);
    m_settings.setValue("pomodoro/soundStyle", m_pomodoroSoundStyle);
    m_settings.setValue("pomodoro/soundVolume", m_pomodoroSoundVolume);
    m_settings.setValue("pomodoro/autoStartBreaks", m_pomodoroAutoStartBreaks);
    m_settings.setValue("pomodoro/autoStartFocus", m_pomodoroAutoStartFocus);
    m_settings.setValue("pomodoro/scale", m_pomodoroScale);

    m_settings.sync();
}

QString SettingsManager::musicDirectory() const
{
    return m_musicDirectory;
}

void SettingsManager::setMusicDirectory(const QString &dir)
{
    if (m_musicDirectory != dir) {
        m_musicDirectory = dir;
        m_settings.setValue("library/musicDirectory", m_musicDirectory);
        m_settings.sync();
        emit musicDirectoryChanged();
    }
}

bool SettingsManager::hasValidDirectory() const
{
    return !m_musicDirectory.trimmed().isEmpty() && QDir(m_musicDirectory).exists();
}

bool SettingsManager::onboardingCompleted() const
{
    return m_onboardingCompleted;
}

void SettingsManager::setOnboardingCompleted(bool completed)
{
    if (m_onboardingCompleted != completed) {
        m_onboardingCompleted = completed;
        m_settings.setValue("library/onboardingCompleted", m_onboardingCompleted);
        m_settings.sync();
        emit onboardingCompletedChanged();
    }
}

int SettingsManager::wallpaperMode() const
{
    return m_wallpaperMode;
}

void SettingsManager::setWallpaperMode(int mode)
{
    if (m_wallpaperMode != mode) {
        m_wallpaperMode = mode;
        m_settings.setValue("appearance/wallpaperMode", m_wallpaperMode);
        m_settings.sync();
        emit wallpaperModeChanged();
    }
}

int SettingsManager::rotateIntervalMinutes() const
{
    return m_rotateIntervalMinutes;
}

void SettingsManager::setRotateIntervalMinutes(int minutes)
{
    if (m_rotateIntervalMinutes != minutes) {
        m_rotateIntervalMinutes = qMax(1, minutes);
        m_settings.setValue("appearance/rotateIntervalMinutes", m_rotateIntervalMinutes);
        m_settings.sync();
        emit rotateIntervalMinutesChanged();
    }
}

QString SettingsManager::fixedWallpaper() const
{
    return m_fixedWallpaper;
}

void SettingsManager::setFixedWallpaper(const QString &wallpaper)
{
    if (m_fixedWallpaper != wallpaper) {
        m_fixedWallpaper = wallpaper;
        m_settings.setValue("appearance/fixedWallpaper", m_fixedWallpaper);
        m_settings.sync();
        emit fixedWallpaperChanged();
    }
}

QColor SettingsManager::solidColor() const
{
    return m_solidColor;
}

void SettingsManager::setSolidColor(const QColor &color)
{
    if (m_solidColor != color) {
        m_solidColor = color;
        m_settings.setValue("appearance/solidColor", m_solidColor);
        m_settings.sync();
        emit solidColorChanged();
    }
}

qreal SettingsManager::dimmingOpacity() const
{
    return m_dimmingOpacity;
}

void SettingsManager::setDimmingOpacity(qreal opacity)
{
    qreal clamped = qBound(0.0, opacity, 0.9);
    if (!qFuzzyCompare(m_dimmingOpacity, clamped)) {
        m_dimmingOpacity = clamped;
        m_settings.setValue("appearance/dimmingOpacity", m_dimmingOpacity);
        m_settings.sync();
        emit dimmingOpacityChanged();
    }
}

qreal SettingsManager::overlayOpacity() const
{
    return m_overlayOpacity;
}

void SettingsManager::setOverlayOpacity(qreal opacity)
{
    qreal clamped = qBound(0.15, opacity, 1.0);
    if (!qFuzzyCompare(m_overlayOpacity, clamped)) {
        m_overlayOpacity = clamped;
        m_settings.setValue("appearance/overlayOpacity", m_overlayOpacity);
        m_settings.sync();
        emit overlayOpacityChanged();
    }
}

bool SettingsManager::blurEnabled() const
{
    return m_blurEnabled;
}

void SettingsManager::setBlurEnabled(bool enabled)
{
    if (m_blurEnabled != enabled) {
        m_blurEnabled = enabled;
        m_settings.setValue("appearance/blurEnabled", m_blurEnabled);
        m_settings.sync();
        emit blurEnabledChanged();
    }
}

qreal SettingsManager::blurStrength() const
{
    return m_blurStrength;
}

void SettingsManager::setBlurStrength(qreal strength)
{
    qreal clamped = qBound(0.0, strength, 1.0);
    if (!qFuzzyCompare(m_blurStrength, clamped)) {
        m_blurStrength = clamped;
        m_settings.setValue("appearance/blurStrength", m_blurStrength);
        m_settings.sync();
        emit blurStrengthChanged();
    }
}

bool SettingsManager::darkeningEnabled() const
{
    return m_darkeningEnabled;
}

void SettingsManager::setDarkeningEnabled(bool enabled)
{
    if (m_darkeningEnabled != enabled) {
        m_darkeningEnabled = enabled;
        m_settings.setValue("appearance/darkeningEnabled", m_darkeningEnabled);
        m_settings.sync();
        emit darkeningEnabledChanged();
    }
}

qreal SettingsManager::darkeningOpacity() const
{
    return m_darkeningOpacity;
}

void SettingsManager::setDarkeningOpacity(qreal opacity)
{
    qreal clamped = qBound(0.0, opacity, 1.0);
    if (!qFuzzyCompare(m_darkeningOpacity, clamped)) {
        m_darkeningOpacity = clamped;
        m_settings.setValue("appearance/darkeningOpacity", m_darkeningOpacity);
        m_settings.sync();
        emit darkeningOpacityChanged();
    }
}

qreal SettingsManager::volume() const
{
    return m_volume;
}

void SettingsManager::setVolume(qreal vol)
{
    qreal clamped = qBound(0.0, vol, 1.0);
    if (!qFuzzyCompare(m_volume, clamped)) {
        m_volume = clamped;
        m_settings.setValue("playback/volume", m_volume);
        m_settings.sync();
        emit volumeChanged();
    }
}

bool SettingsManager::shuffle() const
{
    return m_shuffle;
}

void SettingsManager::setShuffle(bool shuf)
{
    if (m_shuffle != shuf) {
        m_shuffle = shuf;
        m_settings.setValue("playback/shuffle", m_shuffle);
        m_settings.sync();
        emit shuffleChanged();
    }
}

int SettingsManager::repeatMode() const
{
    return m_repeatMode;
}

void SettingsManager::setRepeatMode(int mode)
{
    if (m_repeatMode != mode) {
        m_repeatMode = mode;
        m_settings.setValue("playback/repeatMode", m_repeatMode);
        m_settings.sync();
        emit repeatModeChanged();
    }
}

bool SettingsManager::autoPlay() const
{
    return m_autoPlay;
}

void SettingsManager::setAutoPlay(bool ap)
{
    if (m_autoPlay != ap) {
        m_autoPlay = ap;
        m_settings.setValue("playback/autoPlay", m_autoPlay);
        m_settings.sync();
        emit autoPlayChanged();
    }
}

bool SettingsManager::rememberLastTrack() const
{
    return m_rememberLastTrack;
}

void SettingsManager::setRememberLastTrack(bool remember)
{
    if (m_rememberLastTrack != remember) {
        m_rememberLastTrack = remember;
        m_settings.setValue("behavior/rememberLastTrack", m_rememberLastTrack);
        m_settings.sync();
        emit rememberLastTrackChanged();
    }
}

QString SettingsManager::lastTrackPath() const
{
    return m_lastTrackPath;
}

void SettingsManager::setLastTrackPath(const QString &path)
{
    if (m_lastTrackPath != path) {
        m_lastTrackPath = path;
        m_settings.setValue("behavior/lastTrackPath", m_lastTrackPath);
        m_settings.sync();
        emit lastTrackPathChanged();
    }
}

bool SettingsManager::resumePlaybackPosition() const
{
    return m_resumePlaybackPosition;
}

void SettingsManager::setResumePlaybackPosition(bool resume)
{
    if (m_resumePlaybackPosition != resume) {
        m_resumePlaybackPosition = resume;
        m_settings.setValue("behavior/resumePlaybackPosition", m_resumePlaybackPosition);
        m_settings.sync();
        emit resumePlaybackPositionChanged();
    }
}

qint64 SettingsManager::lastPositionMs() const
{
    return m_lastPositionMs;
}

void SettingsManager::setLastPositionMs(qint64 pos)
{
    if (m_lastPositionMs != pos) {
        m_lastPositionMs = pos;
        m_settings.setValue("behavior/lastPositionMs", m_lastPositionMs);
        m_settings.sync();
        emit lastPositionMsChanged();
    }
}

bool SettingsManager::stillAutoRotate() const
{
    return m_stillAutoRotate;
}

void SettingsManager::setStillAutoRotate(bool rotate)
{
    if (m_stillAutoRotate != rotate) {
        m_stillAutoRotate = rotate;
        m_settings.setValue("appearance/stillAutoRotate", m_stillAutoRotate);
        m_settings.sync();
        emit stillAutoRotateChanged();
    }
}

int SettingsManager::gradientSeed() const
{
    return m_gradientSeed;
}

void SettingsManager::setGradientSeed(int seed)
{
    if (m_gradientSeed != seed) {
        m_gradientSeed = seed;
        m_settings.setValue("appearance/gradientSeed", m_gradientSeed);
        m_settings.sync();
        emit gradientSeedChanged();
    }
}

int SettingsManager::gradientStyle() const
{
    return m_gradientStyle;
}

void SettingsManager::setGradientStyle(int style)
{
    if (m_gradientStyle != style) {
        m_gradientStyle = style;
        m_settings.setValue("appearance/gradientStyle", m_gradientStyle);
        m_settings.sync();
        emit gradientStyleChanged();
    }
}

QString SettingsManager::videoWallpaper() const
{
    return m_videoWallpaper;
}

void SettingsManager::setVideoWallpaper(const QString &video)
{
    if (m_videoWallpaper != video) {
        m_videoWallpaper = video;
        m_settings.setValue("appearance/videoWallpaper", m_videoWallpaper);
        m_settings.sync();
        emit videoWallpaperChanged();
    }
}



bool SettingsManager::inactivityEnabled() const
{
    return m_inactivityEnabled;
}

void SettingsManager::setInactivityEnabled(bool enabled)
{
    if (m_inactivityEnabled != enabled) {
        m_inactivityEnabled = enabled;
        m_settings.setValue("behavior/inactivityEnabled", m_inactivityEnabled);
        m_settings.setValue("behavior/autoHideOverlays", m_inactivityEnabled);
        m_settings.sync();
        emit inactivityEnabledChanged();
    }
}

int SettingsManager::inactivityTimeoutSeconds() const
{
    return m_inactivityTimeoutSeconds;
}

void SettingsManager::setInactivityTimeoutSeconds(int seconds)
{
    int val = qBound(1, seconds, 60);
    if (m_inactivityTimeoutSeconds != val) {
        m_inactivityTimeoutSeconds = val;
        m_settings.setValue("behavior/inactivityTimeoutSeconds", m_inactivityTimeoutSeconds);
        m_settings.setValue("behavior/autoHideTimeoutSeconds", m_inactivityTimeoutSeconds);
        m_settings.sync();
        emit inactivityTimeoutSecondsChanged();
    }
}

int SettingsManager::inactivityMode() const
{
    return m_inactivityMode;
}

void SettingsManager::setInactivityMode(int mode)
{
    if (m_inactivityMode != mode) {
        m_inactivityMode = mode;
        m_settings.setValue("behavior/inactivityMode", m_inactivityMode);
        m_settings.sync();
        emit inactivityModeChanged();
    }
}

bool SettingsManager::visualizerEnabled() const
{
    return m_visualizerEnabled;
}

void SettingsManager::setVisualizerEnabled(bool enabled)
{
    if (m_visualizerEnabled != enabled) {
        m_visualizerEnabled = enabled;
        m_settings.setValue("visualizer/enabled", m_visualizerEnabled);
        m_settings.sync();
        emit visualizerEnabledChanged();
    }
}

int SettingsManager::visualizerTheme() const
{
    return m_visualizerTheme;
}

void SettingsManager::setVisualizerTheme(int theme)
{
    if (m_visualizerTheme != theme) {
        m_visualizerTheme = theme;
        m_settings.setValue("visualizer/theme", m_visualizerTheme);
        m_settings.sync();
        emit visualizerThemeChanged();
    }
}

qreal SettingsManager::visualizerOpacity() const
{
    return m_visualizerOpacity;
}

void SettingsManager::setVisualizerOpacity(qreal opacity)
{
    qreal clamped = qBound(0.05, opacity, 1.0);
    if (!qFuzzyCompare(m_visualizerOpacity, clamped)) {
        m_visualizerOpacity = clamped;
        m_settings.setValue("visualizer/opacity", m_visualizerOpacity);
        m_settings.sync();
        emit visualizerOpacityChanged();
    }
}

int SettingsManager::visualizerPosition() const
{
    return m_visualizerPosition;
}

void SettingsManager::setVisualizerPosition(int pos)
{
    if (m_visualizerPosition != pos) {
        m_visualizerPosition = pos;
        m_settings.setValue("visualizer/position", m_visualizerPosition);
        m_settings.sync();
        emit visualizerPositionChanged();
    }
}

int SettingsManager::visualizerHeight() const
{
    return m_visualizerHeight;
}

void SettingsManager::setVisualizerHeight(int height)
{
    int clamped = qBound(40, height, 400);
    if (m_visualizerHeight != clamped) {
        m_visualizerHeight = clamped;
        m_settings.setValue("visualizer/height", m_visualizerHeight);
        m_settings.sync();
        emit visualizerHeightChanged();
    }
}

int SettingsManager::visualizerBarDensity() const
{
    return m_visualizerBarDensity;
}

void SettingsManager::setVisualizerBarDensity(int density)
{
    int clamped = qBound(16, density, 128);
    if (m_visualizerBarDensity != clamped) {
        m_visualizerBarDensity = clamped;
        m_settings.setValue("visualizer/barDensity", m_visualizerBarDensity);
        m_settings.sync();
        emit visualizerBarDensityChanged();
    }
}

qreal SettingsManager::visualizerSensitivity() const
{
    return m_visualizerSensitivity;
}

void SettingsManager::setVisualizerSensitivity(qreal sensitivity)
{
    qreal clamped = qBound(0.50, sensitivity, 2.00);
    if (!qFuzzyCompare(m_visualizerSensitivity, clamped)) {
        m_visualizerSensitivity = clamped;
        m_settings.setValue("visualizer/sensitivity", m_visualizerSensitivity);
        m_settings.sync();
        emit visualizerSensitivityChanged();
    }
}

qreal SettingsManager::visualizerSpeed() const
{
    return m_visualizerSpeed;
}

void SettingsManager::setVisualizerSpeed(qreal speed)
{
    qreal clamped = qBound(0.40, speed, 2.00);
    if (!qFuzzyCompare(m_visualizerSpeed, clamped)) {
        m_visualizerSpeed = clamped;
        m_settings.setValue("visualizer/speed", m_visualizerSpeed);
        m_settings.sync();
        emit visualizerSpeedChanged();
    }
}

qreal SettingsManager::inactivityFadeOpacity() const
{
    return m_inactivityFadeOpacity;
}

void SettingsManager::setInactivityFadeOpacity(qreal opacity)
{
    qreal clamped = qBound(0.02, opacity, 0.80);
    if (!qFuzzyCompare(m_inactivityFadeOpacity, clamped)) {
        m_inactivityFadeOpacity = clamped;
        m_settings.setValue("behavior/inactivityFadeOpacity", m_inactivityFadeOpacity);
        m_settings.sync();
        emit inactivityFadeOpacityChanged();
    }
}

bool SettingsManager::todoEnabled() const
{
    return m_todoEnabled;
}

void SettingsManager::setTodoEnabled(bool enabled)
{
    if (m_todoEnabled != enabled) {
        m_todoEnabled = enabled;
        m_settings.setValue("todo/enabled", m_todoEnabled);
        m_settings.sync();
        emit todoEnabledChanged();
    }
}

bool SettingsManager::todoVisible() const
{
    return m_todoVisible;
}

void SettingsManager::setTodoVisible(bool visible)
{
    if (m_todoVisible != visible) {
        m_todoVisible = visible;
        m_settings.setValue("todo/visible", m_todoVisible);
        m_settings.sync();
        emit todoVisibleChanged();
    }
}

qreal SettingsManager::todoScale() const
{
    return m_todoScale;
}

void SettingsManager::setTodoScale(qreal scale)
{
    qreal clamped = qBound(0.70, scale, 1.50);
    if (!qFuzzyCompare(m_todoScale, clamped)) {
        m_todoScale = clamped;
        m_settings.setValue("appearance/todoScale", m_todoScale);
        m_settings.sync();
        emit todoScaleChanged();
    }
}

bool SettingsManager::clockEnabled() const
{
    return m_clockEnabled;
}

void SettingsManager::setClockEnabled(bool enabled)
{
    if (m_clockEnabled != enabled) {
        m_clockEnabled = enabled;
        m_settings.setValue("clock/enabled", m_clockEnabled);
        m_settings.sync();
        emit clockEnabledChanged();
    }
}

bool SettingsManager::clockVisible() const
{
    return m_clockVisible;
}

void SettingsManager::setClockVisible(bool visible)
{
    if (m_clockVisible != visible) {
        m_clockVisible = visible;
        m_settings.setValue("clock/visible", m_clockVisible);
        m_settings.sync();
        emit clockVisibleChanged();
    }
}

bool SettingsManager::clockShowSeconds() const
{
    return m_clockShowSeconds;
}

void SettingsManager::setClockShowSeconds(bool show)
{
    if (m_clockShowSeconds != show) {
        m_clockShowSeconds = show;
        m_settings.setValue("clock/showSeconds", m_clockShowSeconds);
        m_settings.sync();
        emit clockShowSecondsChanged();
    }
}

bool SettingsManager::clockShowDate() const
{
    return m_clockShowDate;
}

void SettingsManager::setClockShowDate(bool show)
{
    if (m_clockShowDate != show) {
        m_clockShowDate = show;
        m_settings.setValue("clock/showDate", m_clockShowDate);
        m_settings.sync();
        emit clockShowDateChanged();
    }
}

bool SettingsManager::clock24Hour() const
{
    return m_clock24Hour;
}

void SettingsManager::setClock24Hour(bool is24)
{
    if (m_clock24Hour != is24) {
        m_clock24Hour = is24;
        m_settings.setValue("clock/is24Hour", m_clock24Hour);
        m_settings.sync();
        emit clock24HourChanged();
    }
}

bool SettingsManager::pomodoroEnabled() const
{
    return m_pomodoroEnabled;
}

void SettingsManager::setPomodoroEnabled(bool enabled)
{
    if (m_pomodoroEnabled != enabled) {
        m_pomodoroEnabled = enabled;
        m_settings.setValue("pomodoro/enabled", m_pomodoroEnabled);
        m_settings.sync();
        emit pomodoroEnabledChanged();
    }
}

bool SettingsManager::pomodoroVisible() const
{
    return m_pomodoroVisible;
}

void SettingsManager::setPomodoroVisible(bool visible)
{
    if (m_pomodoroVisible != visible) {
        m_pomodoroVisible = visible;
        m_settings.setValue("pomodoro/visible", m_pomodoroVisible);
        m_settings.sync();
        emit pomodoroVisibleChanged();
    }
}

int SettingsManager::pomodoroFocusDuration() const
{
    return m_pomodoroFocusDuration;
}

void SettingsManager::setPomodoroFocusDuration(int minutes)
{
    minutes = qBound(1, minutes, 180);
    if (m_pomodoroFocusDuration != minutes) {
        m_pomodoroFocusDuration = minutes;
        m_settings.setValue("pomodoro/focusDuration", m_pomodoroFocusDuration);
        m_settings.sync();
        emit pomodoroFocusDurationChanged();
    }
}

int SettingsManager::pomodoroBreakDuration() const
{
    return m_pomodoroBreakDuration;
}

void SettingsManager::setPomodoroBreakDuration(int minutes)
{
    minutes = qBound(1, minutes, 60);
    if (m_pomodoroBreakDuration != minutes) {
        m_pomodoroBreakDuration = minutes;
        m_settings.setValue("pomodoro/breakDuration", m_pomodoroBreakDuration);
        m_settings.sync();
        emit pomodoroBreakDurationChanged();
    }
}

bool SettingsManager::pomodoroBreathingEnabled() const
{
    return m_pomodoroBreathingEnabled;
}

void SettingsManager::setPomodoroBreathingEnabled(bool enabled)
{
    if (m_pomodoroBreathingEnabled != enabled) {
        m_pomodoroBreathingEnabled = enabled;
        m_settings.setValue("pomodoro/breathingEnabled", m_pomodoroBreathingEnabled);
        m_settings.sync();
        emit pomodoroBreathingEnabledChanged();
    }
}

qreal SettingsManager::pomodoroBreathingSpeed() const
{
    return m_pomodoroBreathingSpeed;
}

void SettingsManager::setPomodoroBreathingSpeed(qreal speed)
{
    speed = qBound(0.2, speed, 3.0);
    if (!qFuzzyCompare(m_pomodoroBreathingSpeed, speed)) {
        m_pomodoroBreathingSpeed = speed;
        m_settings.setValue("pomodoro/breathingSpeed", m_pomodoroBreathingSpeed);
        m_settings.sync();
        emit pomodoroBreathingSpeedChanged();
    }
}

qreal SettingsManager::pomodoroBreathingIntensity() const
{
    return m_pomodoroBreathingIntensity;
}

void SettingsManager::setPomodoroBreathingIntensity(qreal intensity)
{
    intensity = qBound(0.1, intensity, 1.0);
    if (!qFuzzyCompare(m_pomodoroBreathingIntensity, intensity)) {
        m_pomodoroBreathingIntensity = intensity;
        m_settings.setValue("pomodoro/breathingIntensity", m_pomodoroBreathingIntensity);
        m_settings.sync();
        emit pomodoroBreathingIntensityChanged();
    }
}

bool SettingsManager::pomodoroSoundEnabled() const
{
    return m_pomodoroSoundEnabled;
}

void SettingsManager::setPomodoroSoundEnabled(bool enabled)
{
    if (m_pomodoroSoundEnabled != enabled) {
        m_pomodoroSoundEnabled = enabled;
        m_settings.setValue("pomodoro/soundEnabled", m_pomodoroSoundEnabled);
        m_settings.sync();
        emit pomodoroSoundEnabledChanged();
    }
}

int SettingsManager::pomodoroSoundStyle() const
{
    return m_pomodoroSoundStyle;
}

void SettingsManager::setPomodoroSoundStyle(int style)
{
    style = qBound(0, style, 3);
    if (m_pomodoroSoundStyle != style) {
        m_pomodoroSoundStyle = style;
        m_settings.setValue("pomodoro/soundStyle", m_pomodoroSoundStyle);
        m_settings.sync();
        emit pomodoroSoundStyleChanged();
    }
}

qreal SettingsManager::pomodoroSoundVolume() const
{
    return m_pomodoroSoundVolume;
}

void SettingsManager::setPomodoroSoundVolume(qreal volume)
{
    volume = qBound(0.0, volume, 1.0);
    if (!qFuzzyCompare(m_pomodoroSoundVolume, volume)) {
        m_pomodoroSoundVolume = volume;
        m_settings.setValue("pomodoro/soundVolume", m_pomodoroSoundVolume);
        m_settings.sync();
        emit pomodoroSoundVolumeChanged();
    }
}

bool SettingsManager::pomodoroAutoStartBreaks() const
{
    return m_pomodoroAutoStartBreaks;
}

void SettingsManager::setPomodoroAutoStartBreaks(bool autoStart)
{
    if (m_pomodoroAutoStartBreaks != autoStart) {
        m_pomodoroAutoStartBreaks = autoStart;
        m_settings.setValue("pomodoro/autoStartBreaks", m_pomodoroAutoStartBreaks);
        m_settings.sync();
        emit pomodoroAutoStartBreaksChanged();
    }
}

bool SettingsManager::pomodoroAutoStartFocus() const
{
    return m_pomodoroAutoStartFocus;
}

void SettingsManager::setPomodoroAutoStartFocus(bool autoStart)
{
    if (m_pomodoroAutoStartFocus != autoStart) {
        m_pomodoroAutoStartFocus = autoStart;
        m_settings.setValue("pomodoro/autoStartFocus", m_pomodoroAutoStartFocus);
        m_settings.sync();
        emit pomodoroAutoStartFocusChanged();
    }
}

qreal SettingsManager::pomodoroScale() const
{
    return m_pomodoroScale;
}

void SettingsManager::setPomodoroScale(qreal scale)
{
    qreal clamped = qBound(0.70, scale, 1.50);
    if (!qFuzzyCompare(m_pomodoroScale, clamped)) {
        m_pomodoroScale = clamped;
        m_settings.setValue("pomodoro/scale", m_pomodoroScale);
        m_settings.sync();
        emit pomodoroScaleChanged();
    }
}

qreal SettingsManager::playlistScale() const
{
    return m_playlistScale;
}

void SettingsManager::setPlaylistScale(qreal scale)
{
    qreal clamped = qBound(0.70, scale, 1.50);
    if (!qFuzzyCompare(m_playlistScale, clamped)) {
        m_playlistScale = clamped;
        m_settings.setValue("appearance/playlistScale", m_playlistScale);
        m_settings.sync();
        emit playlistScaleChanged();
    }
}

qreal SettingsManager::playbackScale() const
{
    return m_playbackScale;
}

void SettingsManager::setPlaybackScale(qreal scale)
{
    qreal clamped = qBound(0.70, scale, 1.50);
    if (!qFuzzyCompare(m_playbackScale, clamped)) {
        m_playbackScale = clamped;
        m_settings.setValue("appearance/playbackScale", m_playbackScale);
        m_settings.setValue("appearance/playbackStripScale", m_playbackScale);
        m_settings.sync();
        emit playbackScaleChanged();
        emit playbackStripScaleChanged();
    }
}

qreal SettingsManager::playbackStripScale() const
{
    return m_playbackScale;
}

void SettingsManager::setPlaybackStripScale(qreal scale)
{
    setPlaybackScale(scale);
}

int SettingsManager::settingsDialogWidth() const
{
    return m_settingsDialogWidth;
}

void SettingsManager::setSettingsDialogWidth(int width)
{
    int clamped = qBound(720, width, 1600);
    if (m_settingsDialogWidth != clamped) {
        m_settingsDialogWidth = clamped;
        m_settings.setValue("appearance/settingsDialogWidth", m_settingsDialogWidth);
        m_settings.sync();
        emit settingsDialogWidthChanged();
    }
}

int SettingsManager::settingsDialogHeight() const
{
    return m_settingsDialogHeight;
}

void SettingsManager::setSettingsDialogHeight(int height)
{
    int clamped = qBound(500, height, 1200);
    if (m_settingsDialogHeight != clamped) {
        m_settingsDialogHeight = clamped;
        m_settings.setValue("appearance/settingsDialogHeight", m_settingsDialogHeight);
        m_settings.sync();
        emit settingsDialogHeightChanged();
    }
}

int SettingsManager::contrastMode() const
{
    return m_contrastMode;
}

void SettingsManager::setContrastMode(int mode)
{
    int clamped = qBound(0, mode, 2);
    if (m_contrastMode != clamped) {
        m_contrastMode = clamped;
        m_settings.setValue("appearance/contrastMode", m_contrastMode);
        m_settings.sync();
        emit contrastModeChanged();
    }
}

qreal SettingsManager::topControlsScale() const
{
    return m_topControlsScale;
}

void SettingsManager::setTopControlsScale(qreal scale)
{
    qreal clamped = qBound(0.70, scale, 1.50);
    if (!qFuzzyCompare(m_topControlsScale, clamped)) {
        m_topControlsScale = clamped;
        m_settings.setValue("appearance/topControlsScale", m_topControlsScale);
        m_settings.sync();
        emit topControlsScaleChanged();
    }
}

QString SettingsManager::fontFamily() const
{
    return m_fontFamily;
}

void SettingsManager::setFontFamily(const QString &family)
{
    if (m_fontFamily != family) {
        m_fontFamily = family;
        m_settings.setValue("appearance/fontFamily", m_fontFamily);
        m_settings.sync();

        QFont f = QGuiApplication::font();
        if (!m_fontFamily.trimmed().isEmpty()) {
            f.setFamily(m_fontFamily);
        }
        f.setPointSize(m_fontSize);
        QGuiApplication::setFont(f);

        emit fontFamilyChanged();
    }
}

int SettingsManager::fontSize() const
{
    return m_fontSize;
}

void SettingsManager::setFontSize(int size)
{
    int clamped = qBound(9, size, 20);
    if (m_fontSize != clamped) {
        m_fontSize = clamped;
        m_settings.setValue("appearance/fontSize", m_fontSize);
        m_settings.sync();

        QFont f = QGuiApplication::font();
        if (!m_fontFamily.trimmed().isEmpty()) {
            f.setFamily(m_fontFamily);
        }
        f.setPointSize(m_fontSize);
        QGuiApplication::setFont(f);

        emit fontSizeChanged();
    }
}

QStringList SettingsManager::availableFontFamilies() const
{
    QStringList families = QFontDatabase::families();
    families.removeDuplicates();
    families.sort(Qt::CaseInsensitive);
    return families;
}

void SettingsManager::resetScales()
{
    setPlaylistScale(1.0);
    setPlaybackScale(1.0);
    setTopControlsScale(1.0);
    setTodoScale(1.20);
    setPomodoroScale(1.0);
}

void SettingsManager::resetFont()
{
    setFontFamily(QString());
    setFontSize(11);
}

