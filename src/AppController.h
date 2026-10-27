#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include "SettingsManager.h"
#include "TrackModel.h"
#include "AudioPlayer.h"
#include "PlaylistManager.h"
#include "WallpaperManager.h"
#include "LibraryScanner.h"
#include "TodoManager.h"
#include "LayoutController.h"
#include "ContrastManager.h"
#include "PomodoroManager.h"

class AppController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool userActive READ isUserActive NOTIFY userActiveChanged)
    Q_PROPERTY(bool isFirstLaunchSetup READ isFirstLaunchSetup NOTIFY appStateChanged)
    Q_PROPERTY(bool isRecoveryRequired READ isRecoveryRequired NOTIFY appStateChanged)
    Q_PROPERTY(SettingsManager* settings READ settings CONSTANT)
    Q_PROPERTY(TrackModel* trackModel READ trackModel CONSTANT)
    Q_PROPERTY(AudioPlayer* audioPlayer READ audioPlayer CONSTANT)
    Q_PROPERTY(PlaylistManager* playlistManager READ playlistManager CONSTANT)
    Q_PROPERTY(WallpaperManager* wallpaperManager READ wallpaperManager CONSTANT)
    Q_PROPERTY(LibraryScanner* libraryScanner READ libraryScanner CONSTANT)
    Q_PROPERTY(TodoManager* todoManager READ todoManager CONSTANT)
    Q_PROPERTY(LayoutController* layoutController READ layoutController CONSTANT)
    Q_PROPERTY(ContrastManager* contrastManager READ contrastManager CONSTANT)
    Q_PROPERTY(PomodoroManager* pomodoroManager READ pomodoroManager CONSTANT)

public:
    explicit AppController(QObject *parent = nullptr);

    bool isUserActive() const { return m_userActive; }
    bool isFirstLaunchSetup() const;
    bool isRecoveryRequired() const;

    SettingsManager* settings() const { return m_settings; }
    TrackModel* trackModel() const { return m_trackModel; }
    AudioPlayer* audioPlayer() const { return m_audioPlayer; }
    PlaylistManager* playlistManager() const { return m_playlistManager; }
    WallpaperManager* wallpaperManager() const { return m_wallpaperManager; }
    LibraryScanner* libraryScanner() const { return m_libraryScanner; }
    TodoManager* todoManager() const { return m_todoManager; }
    LayoutController* layoutController() const { return m_layoutController; }
    ContrastManager* contrastManager() const { return m_contrastManager; }
    PomodoroManager* pomodoroManager() const { return m_pomodoroManager; }

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void reportActivity();
    Q_INVOKABLE void setUserActive(bool active);
    Q_INVOKABLE void selectMusicDirectory(const QString &dirPath);
    Q_INVOKABLE bool promptForMusicDirectory();
    Q_INVOKABLE QVariantMap validateDirectory(const QString &dirPath);
    Q_INVOKABLE bool completeOnboarding(const QString &dirPath);
    Q_INVOKABLE bool retryDirectoryAccess();
    Q_INVOKABLE void rescanLibrary();
    Q_INVOKABLE void rescanWallpapers();

    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void userActiveChanged(bool active);
    void appStateChanged();
    void initialSetupRequired();
    void notificationRequested(const QString &message, bool isError);

private slots:
    void onScanCompleted(int count, const QString &error);
    void onAudioError(const QString &message);

private:
    SettingsManager *m_settings;
    TrackModel *m_trackModel;
    AudioPlayer *m_audioPlayer;
    PlaylistManager *m_playlistManager;
    WallpaperManager *m_wallpaperManager;
    LibraryScanner *m_libraryScanner;
    TodoManager *m_todoManager;
    LayoutController *m_layoutController;
    ContrastManager *m_contrastManager;
    PomodoroManager *m_pomodoroManager;

    bool m_isInitialScan{true};
    bool m_userActive{true};
    QTimer m_inactivityTimer;
};
