#pragma once

#include <QObject>
#include <QSettings>
#include <QColor>
#include <QDir>
#include <QUrl>

class SettingsManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString musicDirectory READ musicDirectory WRITE setMusicDirectory NOTIFY musicDirectoryChanged)
    Q_PROPERTY(bool hasValidDirectory READ hasValidDirectory NOTIFY musicDirectoryChanged)
    Q_PROPERTY(bool onboardingCompleted READ onboardingCompleted WRITE setOnboardingCompleted NOTIFY onboardingCompletedChanged)
    Q_PROPERTY(int wallpaperMode READ wallpaperMode WRITE setWallpaperMode NOTIFY wallpaperModeChanged)
    Q_PROPERTY(bool stillAutoRotate READ stillAutoRotate WRITE setStillAutoRotate NOTIFY stillAutoRotateChanged)
    Q_PROPERTY(int rotateIntervalMinutes READ rotateIntervalMinutes WRITE setRotateIntervalMinutes NOTIFY rotateIntervalMinutesChanged)
    Q_PROPERTY(QString fixedWallpaper READ fixedWallpaper WRITE setFixedWallpaper NOTIFY fixedWallpaperChanged)
    Q_PROPERTY(QColor solidColor READ solidColor WRITE setSolidColor NOTIFY solidColorChanged)
    Q_PROPERTY(int gradientSeed READ gradientSeed WRITE setGradientSeed NOTIFY gradientSeedChanged)
    Q_PROPERTY(int gradientStyle READ gradientStyle WRITE setGradientStyle NOTIFY gradientStyleChanged)
    Q_PROPERTY(QString videoWallpaper READ videoWallpaper WRITE setVideoWallpaper NOTIFY videoWallpaperChanged)
    Q_PROPERTY(qreal dimmingOpacity READ dimmingOpacity WRITE setDimmingOpacity NOTIFY dimmingOpacityChanged)
    Q_PROPERTY(qreal overlayOpacity READ overlayOpacity WRITE setOverlayOpacity NOTIFY overlayOpacityChanged)
    Q_PROPERTY(bool blurEnabled READ blurEnabled WRITE setBlurEnabled NOTIFY blurEnabledChanged)
    Q_PROPERTY(qreal blurStrength READ blurStrength WRITE setBlurStrength NOTIFY blurStrengthChanged)
    Q_PROPERTY(bool darkeningEnabled READ darkeningEnabled WRITE setDarkeningEnabled NOTIFY darkeningEnabledChanged)
    Q_PROPERTY(qreal darkeningOpacity READ darkeningOpacity WRITE setDarkeningOpacity NOTIFY darkeningOpacityChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(int repeatMode READ repeatMode WRITE setRepeatMode NOTIFY repeatModeChanged)
    Q_PROPERTY(bool autoPlay READ autoPlay WRITE setAutoPlay NOTIFY autoPlayChanged)
    Q_PROPERTY(bool rememberLastTrack READ rememberLastTrack WRITE setRememberLastTrack NOTIFY rememberLastTrackChanged)
    Q_PROPERTY(QString lastTrackPath READ lastTrackPath WRITE setLastTrackPath NOTIFY lastTrackPathChanged)
    Q_PROPERTY(bool resumePlaybackPosition READ resumePlaybackPosition WRITE setResumePlaybackPosition NOTIFY resumePlaybackPositionChanged)
    Q_PROPERTY(qint64 lastPositionMs READ lastPositionMs WRITE setLastPositionMs NOTIFY lastPositionMsChanged)
    Q_PROPERTY(bool inactivityEnabled READ inactivityEnabled WRITE setInactivityEnabled NOTIFY inactivityEnabledChanged)
    Q_PROPERTY(bool autoHideOverlays READ inactivityEnabled WRITE setInactivityEnabled NOTIFY inactivityEnabledChanged)
    Q_PROPERTY(int inactivityTimeoutSeconds READ inactivityTimeoutSeconds WRITE setInactivityTimeoutSeconds NOTIFY inactivityTimeoutSecondsChanged)
    Q_PROPERTY(int autoHideTimeoutSeconds READ inactivityTimeoutSeconds WRITE setInactivityTimeoutSeconds NOTIFY inactivityTimeoutSecondsChanged)
    Q_PROPERTY(int inactivityMode READ inactivityMode WRITE setInactivityMode NOTIFY inactivityModeChanged)
    Q_PROPERTY(qreal inactivityFadeOpacity READ inactivityFadeOpacity WRITE setInactivityFadeOpacity NOTIFY inactivityFadeOpacityChanged)
    Q_PROPERTY(bool visualizerEnabled READ visualizerEnabled WRITE setVisualizerEnabled NOTIFY visualizerEnabledChanged)
    Q_PROPERTY(int visualizerTheme READ visualizerTheme WRITE setVisualizerTheme NOTIFY visualizerThemeChanged)
    Q_PROPERTY(qreal visualizerOpacity READ visualizerOpacity WRITE setVisualizerOpacity NOTIFY visualizerOpacityChanged)
    Q_PROPERTY(int visualizerPosition READ visualizerPosition WRITE setVisualizerPosition NOTIFY visualizerPositionChanged)
    Q_PROPERTY(int visualizerHeight READ visualizerHeight WRITE setVisualizerHeight NOTIFY visualizerHeightChanged)
    Q_PROPERTY(int visualizerBarDensity READ visualizerBarDensity WRITE setVisualizerBarDensity NOTIFY visualizerBarDensityChanged)
    Q_PROPERTY(qreal visualizerSensitivity READ visualizerSensitivity WRITE setVisualizerSensitivity NOTIFY visualizerSensitivityChanged)
    Q_PROPERTY(qreal visualizerSpeed READ visualizerSpeed WRITE setVisualizerSpeed NOTIFY visualizerSpeedChanged)
    Q_PROPERTY(bool todoEnabled READ todoEnabled WRITE setTodoEnabled NOTIFY todoEnabledChanged)
    Q_PROPERTY(bool todoVisible READ todoVisible WRITE setTodoVisible NOTIFY todoVisibleChanged)
    Q_PROPERTY(qreal todoScale READ todoScale WRITE setTodoScale NOTIFY todoScaleChanged)
    Q_PROPERTY(bool clockEnabled READ clockEnabled WRITE setClockEnabled NOTIFY clockEnabledChanged)
    Q_PROPERTY(bool clockVisible READ clockVisible WRITE setClockVisible NOTIFY clockVisibleChanged)
    Q_PROPERTY(bool clockShowSeconds READ clockShowSeconds WRITE setClockShowSeconds NOTIFY clockShowSecondsChanged)
    Q_PROPERTY(bool clockShowDate READ clockShowDate WRITE setClockShowDate NOTIFY clockShowDateChanged)
    Q_PROPERTY(bool clock24Hour READ clock24Hour WRITE setClock24Hour NOTIFY clock24HourChanged)
    Q_PROPERTY(bool pomodoroEnabled READ pomodoroEnabled WRITE setPomodoroEnabled NOTIFY pomodoroEnabledChanged)
    Q_PROPERTY(bool pomodoroVisible READ pomodoroVisible WRITE setPomodoroVisible NOTIFY pomodoroVisibleChanged)
    Q_PROPERTY(int pomodoroFocusDuration READ pomodoroFocusDuration WRITE setPomodoroFocusDuration NOTIFY pomodoroFocusDurationChanged)
    Q_PROPERTY(int pomodoroBreakDuration READ pomodoroBreakDuration WRITE setPomodoroBreakDuration NOTIFY pomodoroBreakDurationChanged)
    Q_PROPERTY(bool pomodoroBreathingEnabled READ pomodoroBreathingEnabled WRITE setPomodoroBreathingEnabled NOTIFY pomodoroBreathingEnabledChanged)
    Q_PROPERTY(qreal pomodoroBreathingSpeed READ pomodoroBreathingSpeed WRITE setPomodoroBreathingSpeed NOTIFY pomodoroBreathingSpeedChanged)
    Q_PROPERTY(qreal pomodoroBreathingIntensity READ pomodoroBreathingIntensity WRITE setPomodoroBreathingIntensity NOTIFY pomodoroBreathingIntensityChanged)
    Q_PROPERTY(bool pomodoroSoundEnabled READ pomodoroSoundEnabled WRITE setPomodoroSoundEnabled NOTIFY pomodoroSoundEnabledChanged)
    Q_PROPERTY(int pomodoroSoundStyle READ pomodoroSoundStyle WRITE setPomodoroSoundStyle NOTIFY pomodoroSoundStyleChanged)
    Q_PROPERTY(qreal pomodoroSoundVolume READ pomodoroSoundVolume WRITE setPomodoroSoundVolume NOTIFY pomodoroSoundVolumeChanged)
    Q_PROPERTY(bool pomodoroAutoStartBreaks READ pomodoroAutoStartBreaks WRITE setPomodoroAutoStartBreaks NOTIFY pomodoroAutoStartBreaksChanged)
    Q_PROPERTY(bool pomodoroAutoStartFocus READ pomodoroAutoStartFocus WRITE setPomodoroAutoStartFocus NOTIFY pomodoroAutoStartFocusChanged)
    Q_PROPERTY(qreal pomodoroScale READ pomodoroScale WRITE setPomodoroScale NOTIFY pomodoroScaleChanged)
    Q_PROPERTY(qreal playlistScale READ playlistScale WRITE setPlaylistScale NOTIFY playlistScaleChanged)
    Q_PROPERTY(qreal playbackScale READ playbackScale WRITE setPlaybackScale NOTIFY playbackScaleChanged)
    Q_PROPERTY(qreal playbackStripScale READ playbackStripScale WRITE setPlaybackStripScale NOTIFY playbackStripScaleChanged)
    Q_PROPERTY(int settingsDialogWidth READ settingsDialogWidth WRITE setSettingsDialogWidth NOTIFY settingsDialogWidthChanged)
    Q_PROPERTY(int settingsDialogHeight READ settingsDialogHeight WRITE setSettingsDialogHeight NOTIFY settingsDialogHeightChanged)
    Q_PROPERTY(int contrastMode READ contrastMode WRITE setContrastMode NOTIFY contrastModeChanged)
    Q_PROPERTY(qreal topControlsScale READ topControlsScale WRITE setTopControlsScale NOTIFY topControlsScaleChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(QStringList availableFontFamilies READ availableFontFamilies CONSTANT)

public:
    enum WallpaperMode {
        GradientMode = 0,
        StillImagesMode = 1,
        VideoMode = 2,
        // Compatibility aliases
        RotateMode = 1,
        FixedMode = 1,
        SolidColorMode = 0
    };
    Q_ENUM(WallpaperMode)

    enum InactivityMode {
        InactivityFade = 0,
        InactivityHide = 1
    };
    Q_ENUM(InactivityMode)

    enum ContrastMode {
        ContrastAuto = 0,
        ContrastLight = 1,
        ContrastDark = 2
    };
    Q_ENUM(ContrastMode)

    enum VisualizerTheme {
        ClassicBars = 0,
        SoftRoundedBars = 1,
        MinimalWaveform = 2,
        AmbientWaves = 3
    };
    Q_ENUM(VisualizerTheme)

    enum VisualizerPosition {
        PositionBottom = 0,
        PositionCenter = 1,
        PositionTop = 2
    };
    Q_ENUM(VisualizerPosition)

    enum GradientStyle {
        LinearPastel = 0,
        RadialPastel = 1,
        AngledPastel = 2,
        SoftMesh = 3
    };
    Q_ENUM(GradientStyle)

    enum RepeatMode {
        RepeatOff = 0,
        RepeatAll = 1,
        RepeatOne = 2
    };
    Q_ENUM(RepeatMode)

    explicit SettingsManager(QObject *parent = nullptr);

    QString musicDirectory() const;
    void setMusicDirectory(const QString &dir);
    bool hasValidDirectory() const;
    bool onboardingCompleted() const;
    void setOnboardingCompleted(bool completed);

    int wallpaperMode() const;
    void setWallpaperMode(int mode);

    int rotateIntervalMinutes() const;
    void setRotateIntervalMinutes(int minutes);

    QString fixedWallpaper() const;
    void setFixedWallpaper(const QString &wallpaper);

    QColor solidColor() const;
    void setSolidColor(const QColor &color);

    qreal dimmingOpacity() const;
    void setDimmingOpacity(qreal opacity);

    qreal overlayOpacity() const;
    void setOverlayOpacity(qreal opacity);

    bool blurEnabled() const;
    void setBlurEnabled(bool enabled);

    qreal blurStrength() const;
    void setBlurStrength(qreal strength);

    bool darkeningEnabled() const;
    void setDarkeningEnabled(bool enabled);

    qreal darkeningOpacity() const;
    void setDarkeningOpacity(qreal opacity);

    qreal volume() const;
    void setVolume(qreal vol);

    bool shuffle() const;
    void setShuffle(bool shuf);

    int repeatMode() const;
    void setRepeatMode(int mode);

    bool autoPlay() const;
    void setAutoPlay(bool ap);

    bool rememberLastTrack() const;
    void setRememberLastTrack(bool remember);

    QString lastTrackPath() const;
    void setLastTrackPath(const QString &path);

    bool resumePlaybackPosition() const;
    void setResumePlaybackPosition(bool resume);

    qint64 lastPositionMs() const;
    void setLastPositionMs(qint64 pos);

    bool stillAutoRotate() const;
    void setStillAutoRotate(bool rotate);

    int gradientSeed() const;
    void setGradientSeed(int seed);

    int gradientStyle() const;
    void setGradientStyle(int style);

    QString videoWallpaper() const;
    void setVideoWallpaper(const QString &video);


    bool inactivityEnabled() const;
    void setInactivityEnabled(bool enabled);
    bool autoHideOverlays() const { return inactivityEnabled(); }
    void setAutoHideOverlays(bool b) { setInactivityEnabled(b); }

    int inactivityTimeoutSeconds() const;
    void setInactivityTimeoutSeconds(int seconds);
    int autoHideTimeoutSeconds() const { return inactivityTimeoutSeconds(); }
    void setAutoHideTimeoutSeconds(int s) { setInactivityTimeoutSeconds(s); }

    int inactivityMode() const;
    void setInactivityMode(int mode);

    qreal inactivityFadeOpacity() const;
    void setInactivityFadeOpacity(qreal opacity);

    bool visualizerEnabled() const;
    void setVisualizerEnabled(bool enabled);

    int visualizerTheme() const;
    void setVisualizerTheme(int theme);

    qreal visualizerOpacity() const;
    void setVisualizerOpacity(qreal opacity);

    int visualizerPosition() const;
    void setVisualizerPosition(int pos);

    int visualizerHeight() const;
    void setVisualizerHeight(int height);

    int visualizerBarDensity() const;
    void setVisualizerBarDensity(int density);

    qreal visualizerSensitivity() const;
    void setVisualizerSensitivity(qreal sensitivity);

    qreal visualizerSpeed() const;
    void setVisualizerSpeed(qreal speed);

    bool todoEnabled() const;
    void setTodoEnabled(bool enabled);

    bool todoVisible() const;
    void setTodoVisible(bool visible);

    qreal todoScale() const;
    void setTodoScale(qreal scale);

    bool clockEnabled() const;
    void setClockEnabled(bool enabled);

    bool clockVisible() const;
    void setClockVisible(bool visible);

    bool clockShowSeconds() const;
    void setClockShowSeconds(bool show);

    bool clockShowDate() const;
    void setClockShowDate(bool show);

    bool clock24Hour() const;
    void setClock24Hour(bool is24);

    bool pomodoroEnabled() const;
    void setPomodoroEnabled(bool enabled);

    bool pomodoroVisible() const;
    void setPomodoroVisible(bool visible);

    int pomodoroFocusDuration() const;
    void setPomodoroFocusDuration(int minutes);

    int pomodoroBreakDuration() const;
    void setPomodoroBreakDuration(int minutes);

    bool pomodoroBreathingEnabled() const;
    void setPomodoroBreathingEnabled(bool enabled);

    qreal pomodoroBreathingSpeed() const;
    void setPomodoroBreathingSpeed(qreal speed);

    qreal pomodoroBreathingIntensity() const;
    void setPomodoroBreathingIntensity(qreal intensity);

    bool pomodoroSoundEnabled() const;
    void setPomodoroSoundEnabled(bool enabled);

    int pomodoroSoundStyle() const;
    void setPomodoroSoundStyle(int style);

    qreal pomodoroSoundVolume() const;
    void setPomodoroSoundVolume(qreal volume);

    bool pomodoroAutoStartBreaks() const;
    void setPomodoroAutoStartBreaks(bool autoStart);

    bool pomodoroAutoStartFocus() const;
    void setPomodoroAutoStartFocus(bool autoStart);

    qreal pomodoroScale() const;
    void setPomodoroScale(qreal scale);

    qreal playlistScale() const;
    void setPlaylistScale(qreal scale);

    qreal playbackScale() const;
    void setPlaybackScale(qreal scale);

    qreal playbackStripScale() const;
    void setPlaybackStripScale(qreal scale);

    int settingsDialogWidth() const;
    void setSettingsDialogWidth(int width);

    int settingsDialogHeight() const;
    void setSettingsDialogHeight(int height);

    int contrastMode() const;
    void setContrastMode(int mode);

    qreal topControlsScale() const;
    void setTopControlsScale(qreal scale);

    QString fontFamily() const;
    void setFontFamily(const QString &family);

    int fontSize() const;
    void setFontSize(int size);

    QStringList availableFontFamilies() const;

    Q_INVOKABLE void resetScales();
    Q_INVOKABLE void resetFont();
    Q_INVOKABLE void save();

signals:
    void musicDirectoryChanged();
    void onboardingCompletedChanged();
    void wallpaperModeChanged();
    void stillAutoRotateChanged();
    void rotateIntervalMinutesChanged();
    void fixedWallpaperChanged();
    void solidColorChanged();
    void gradientSeedChanged();
    void gradientStyleChanged();
    void videoWallpaperChanged();
    void dimmingOpacityChanged();
    void overlayOpacityChanged();
    void blurEnabledChanged();
    void blurStrengthChanged();
    void darkeningEnabledChanged();
    void darkeningOpacityChanged();
    void volumeChanged();
    void shuffleChanged();
    void repeatModeChanged();
    void autoPlayChanged();
    void rememberLastTrackChanged();
    void lastTrackPathChanged();
    void resumePlaybackPositionChanged();
    void lastPositionMsChanged();
    void inactivityEnabledChanged();
    void inactivityTimeoutSecondsChanged();
    void inactivityModeChanged();
    void inactivityFadeOpacityChanged();
    void visualizerEnabledChanged();
    void visualizerThemeChanged();
    void visualizerOpacityChanged();
    void visualizerPositionChanged();
    void visualizerHeightChanged();
    void visualizerBarDensityChanged();
    void visualizerSensitivityChanged();
    void visualizerSpeedChanged();
    void todoEnabledChanged();
    void todoVisibleChanged();
    void todoScaleChanged();
    void clockEnabledChanged();
    void clockVisibleChanged();
    void clockShowSecondsChanged();
    void clockShowDateChanged();
    void clock24HourChanged();
    void pomodoroEnabledChanged();
    void pomodoroVisibleChanged();
    void pomodoroFocusDurationChanged();
    void pomodoroBreakDurationChanged();
    void pomodoroBreathingEnabledChanged();
    void pomodoroBreathingSpeedChanged();
    void pomodoroBreathingIntensityChanged();
    void pomodoroSoundEnabledChanged();
    void pomodoroSoundStyleChanged();
    void pomodoroSoundVolumeChanged();
    void pomodoroAutoStartBreaksChanged();
    void pomodoroAutoStartFocusChanged();
    void pomodoroScaleChanged();
    void playlistScaleChanged();
    void playbackScaleChanged();
    void playbackStripScaleChanged();
    void settingsDialogWidthChanged();
    void settingsDialogHeightChanged();
    void contrastModeChanged();
    void topControlsScaleChanged();
    void fontFamilyChanged();
    void fontSizeChanged();

private:
    QSettings m_settings;

    QString m_musicDirectory;
    bool m_onboardingCompleted{false};
    int m_wallpaperMode{StillImagesMode};
    bool m_stillAutoRotate{true};
    int m_rotateIntervalMinutes{5};
    QString m_fixedWallpaper;
    QColor m_solidColor{QColor(24, 25, 38)};
    int m_gradientSeed{42};
    int m_gradientStyle{0};
    QString m_videoWallpaper;
    qreal m_dimmingOpacity{0.25};
    qreal m_overlayOpacity{0.85};
    bool m_blurEnabled{true};
    qreal m_blurStrength{0.65};
    bool m_darkeningEnabled{true};
    qreal m_darkeningOpacity{0.75};
    qreal m_volume{0.75};
    bool m_shuffle{false};
    int m_repeatMode{RepeatAll};
    bool m_autoPlay{false};
    bool m_rememberLastTrack{true};
    QString m_lastTrackPath;
    bool m_resumePlaybackPosition{false};
    qint64 m_lastPositionMs{0};
    bool m_inactivityEnabled{true};
    int m_inactivityTimeoutSeconds{5};
    int m_inactivityMode{InactivityFade};
    qreal m_inactivityFadeOpacity{0.15};
    bool m_visualizerEnabled{true};
    int m_visualizerTheme{ClassicBars};
    qreal m_visualizerOpacity{0.65};
    int m_visualizerPosition{PositionBottom};
    int m_visualizerHeight{120};
    int m_visualizerBarDensity{64};
    qreal m_visualizerSensitivity{1.0};
    qreal m_visualizerSpeed{0.8};
    bool m_todoEnabled{true};
    bool m_todoVisible{false};
    qreal m_todoScale{1.20};
    bool m_clockEnabled{true};
    bool m_clockVisible{false};
    bool m_clockShowSeconds{false};
    bool m_clockShowDate{true};
    bool m_clock24Hour{true};
    bool m_pomodoroEnabled{true};
    bool m_pomodoroVisible{false};
    int m_pomodoroFocusDuration{25};
    int m_pomodoroBreakDuration{5};
    bool m_pomodoroBreathingEnabled{true};
    qreal m_pomodoroBreathingSpeed{1.0};
    qreal m_pomodoroBreathingIntensity{0.7};
    bool m_pomodoroSoundEnabled{true};
    int m_pomodoroSoundStyle{0};
    qreal m_pomodoroSoundVolume{0.8};
    bool m_pomodoroAutoStartBreaks{false};
    bool m_pomodoroAutoStartFocus{false};
    qreal m_pomodoroScale{1.0};
    qreal m_playlistScale{1.0};
    qreal m_playbackScale{1.0};
    qreal m_topControlsScale{1.0};
    int m_settingsDialogWidth{960};
    int m_settingsDialogHeight{680};
    int m_contrastMode{ContrastAuto};
    QString m_fontFamily;
    int m_fontSize{11};

    void loadSettings();
};
