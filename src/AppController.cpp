#include "AppController.h"
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QEvent>
#include <QFileDialog>

AppController::AppController(QObject *parent)
    : QObject(parent)
    , m_settings(new SettingsManager(this))
    , m_trackModel(new TrackModel(this))
    , m_audioPlayer(new AudioPlayer(this))
    , m_playlistManager(new PlaylistManager(m_trackModel, m_audioPlayer, m_settings, this))
    , m_wallpaperManager(new WallpaperManager(m_settings, this))
    , m_libraryScanner(new LibraryScanner(this))
    , m_todoManager(new TodoManager(this))
    , m_layoutController(new LayoutController(m_settings, this))
    , m_contrastManager(new ContrastManager(m_settings, m_wallpaperManager, this))
    , m_pomodoroManager(new PomodoroManager(m_settings, this))
{
    connect(m_libraryScanner, &LibraryScanner::scanCompleted, this, &AppController::onScanCompleted);
    connect(m_audioPlayer, &AudioPlayer::errorOccurred, this, &AppController::onAudioError);
    connect(m_settings, &SettingsManager::musicDirectoryChanged, this, &AppController::appStateChanged);
    connect(m_settings, &SettingsManager::onboardingCompletedChanged, this, &AppController::appStateChanged);
}

bool AppController::isFirstLaunchSetup() const
{
    return !m_settings->onboardingCompleted();
}

bool AppController::isRecoveryRequired() const
{
    return m_settings->onboardingCompleted() && !m_settings->hasValidDirectory();
}

void AppController::initialize()
{
    // Apply initial volume
    if (m_audioPlayer && m_settings) {
        m_audioPlayer->setVolume(m_settings->volume());
    }

    // Set up centralized inactivity controller
    if (qApp) {
        qApp->installEventFilter(this);
    }

    m_inactivityTimer.setSingleShot(true);
    connect(&m_inactivityTimer, &QTimer::timeout, this, [this]() {
        if (m_userActive) {
            m_userActive = false;
            emit userActiveChanged(false);
        }
    });

    connect(m_settings, &SettingsManager::inactivityEnabledChanged, this, [this]() {
        reportActivity();
    });

    connect(m_settings, &SettingsManager::inactivityTimeoutSecondsChanged, this, [this]() {
        reportActivity();
    });

    reportActivity();

    emit appStateChanged();

    if (isFirstLaunchSetup()) {
        emit initialSetupRequired();
    } else if (isRecoveryRequired()) {
        emit notificationRequested(QStringLiteral("Configured music directory is unavailable. Please reconnect or choose a folder."), true);
    } else {
        m_libraryScanner->scanDirectory(m_settings->musicDirectory(), m_trackModel);
        m_wallpaperManager->updateConfiguration();
    }
}

void AppController::reportActivity()
{
    if (!m_settings->inactivityEnabled()) {
        if (!m_userActive) {
            m_userActive = true;
            emit userActiveChanged(true);
        }
        m_inactivityTimer.stop();
        return;
    }

    if (!m_userActive) {
        m_userActive = true;
        emit userActiveChanged(true);
    }
    m_inactivityTimer.start(m_settings->inactivityTimeoutSeconds() * 1000);
}

void AppController::setUserActive(bool active)
{
    if (m_userActive != active) {
        m_userActive = active;
        emit userActiveChanged(m_userActive);
    }
    if (active) {
        if (m_settings->inactivityEnabled()) {
            m_inactivityTimer.start(m_settings->inactivityTimeoutSeconds() * 1000);
        }
    } else {
        m_inactivityTimer.stop();
    }
}

bool AppController::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::Wheel:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
        reportActivity();
        break;
    default:
        break;
    }
    return QObject::eventFilter(watched, event);
}

void AppController::selectMusicDirectory(const QString &dirPath)
{
    QString localPath = dirPath;
    if (localPath.startsWith(QLatin1String("file://"))) {
        localPath = QUrl(dirPath).toLocalFile();
    }
    localPath = QDir::cleanPath(localPath);

    QDir dir(localPath);
    if (!dir.exists()) {
        emit notificationRequested(QStringLiteral("Directory does not exist: ") + localPath, true);
        return;
    }

    // Safely create wallpaper folders without overwriting
    QDir().mkpath(localPath + QStringLiteral("/Wallpapers/Still Wallpapers"));
    QDir().mkpath(localPath + QStringLiteral("/Wallpapers/Video Wallpapers"));

    m_settings->setMusicDirectory(localPath);
    m_wallpaperManager->updateConfiguration();
    m_libraryScanner->scanDirectory(localPath, m_trackModel);
    emit appStateChanged();
}

QVariantMap AppController::validateDirectory(const QString &dirPath)
{
    QVariantMap res;
    QString path = dirPath;
    if (path.startsWith(QLatin1String("file://"))) {
        path = QUrl(dirPath).toLocalFile();
    }
    path = QDir::cleanPath(path);

    res[QStringLiteral("path")] = path;
    if (path.isEmpty()) {
        res[QStringLiteral("valid")] = false;
        res[QStringLiteral("exists")] = false;
        res[QStringLiteral("accessible")] = false;
        res[QStringLiteral("trackCount")] = 0;
        res[QStringLiteral("message")] = QStringLiteral("Please select a music folder.");
        return res;
    }

    QDir dir(path);
    if (!dir.exists()) {
        res[QStringLiteral("valid")] = false;
        res[QStringLiteral("exists")] = false;
        res[QStringLiteral("accessible")] = false;
        res[QStringLiteral("trackCount")] = 0;
        res[QStringLiteral("message")] = QStringLiteral("Directory does not exist or is inaccessible.");
        return res;
    }

    QFileInfo fi(path);
    if (!fi.isReadable()) {
        res[QStringLiteral("valid")] = false;
        res[QStringLiteral("exists")] = true;
        res[QStringLiteral("accessible")] = false;
        res[QStringLiteral("trackCount")] = 0;
        res[QStringLiteral("message")] = QStringLiteral("Permission denied: cannot read directory.");
        return res;
    }

    // Audio format check
    QStringList audioFilters = {
        QStringLiteral("*.mp3"),
        QStringLiteral("*.flac"),
        QStringLiteral("*.ogg"),
        QStringLiteral("*.wav")
    };
    QFileInfoList audioFiles = dir.entryInfoList(audioFilters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    int trackCount = audioFiles.size();

    res[QStringLiteral("valid")] = true;
    res[QStringLiteral("exists")] = true;
    res[QStringLiteral("accessible")] = true;
    res[QStringLiteral("trackCount")] = trackCount;

    if (trackCount > 0) {
        res[QStringLiteral("message")] = QStringLiteral("Valid directory: %1 audio tracks found.").arg(trackCount);
    } else {
        res[QStringLiteral("message")] = QStringLiteral("Valid directory: currently 0 audio tracks (you can add songs anytime).");
    }

    QDir stillDir(path + QStringLiteral("/Wallpapers/Still Wallpapers"));
    QDir videoDir(path + QStringLiteral("/Wallpapers/Video Wallpapers"));
    res[QStringLiteral("hasStillDir")] = stillDir.exists();
    res[QStringLiteral("hasVideoDir")] = videoDir.exists();

    return res;
}

bool AppController::completeOnboarding(const QString &dirPath)
{
    QString path = dirPath;
    if (path.startsWith(QLatin1String("file://"))) {
        path = QUrl(dirPath).toLocalFile();
    }
    path = QDir::cleanPath(path);

    QDir dir(path);
    if (!dir.exists()) {
        emit notificationRequested(QStringLiteral("Directory does not exist: ") + path, true);
        return false;
    }

    // Create wallpaper directories safely without overwriting anything
    QDir().mkpath(path + QStringLiteral("/Wallpapers/Still Wallpapers"));
    QDir().mkpath(path + QStringLiteral("/Wallpapers/Video Wallpapers"));

    m_settings->setMusicDirectory(path);
    m_settings->setOnboardingCompleted(true);
    m_settings->save();

    m_wallpaperManager->updateConfiguration();
    m_libraryScanner->scanDirectory(path, m_trackModel);

    emit appStateChanged();
    emit notificationRequested(QStringLiteral("Setup complete! Enjoy your study sessions."), false);
    return true;
}

bool AppController::retryDirectoryAccess()
{
    if (m_settings->hasValidDirectory()) {
        m_wallpaperManager->updateConfiguration();
        m_libraryScanner->scanDirectory(m_settings->musicDirectory(), m_trackModel);
        emit appStateChanged();
        emit notificationRequested(QStringLiteral("Connected to music directory successfully."), false);
        return true;
    }
    emit notificationRequested(QStringLiteral("Directory is still unavailable: ") + m_settings->musicDirectory(), true);
    return false;
}

bool AppController::promptForMusicDirectory()
{
    QString initialDir = m_settings->hasValidDirectory() ? m_settings->musicDirectory() : QDir::homePath();

    QFileDialog dialog(nullptr, tr("Select Study Music Folder or Any Audio File"), initialDir);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, false);
    dialog.setOption(QFileDialog::DontResolveSymlinks, true);
    dialog.setNameFilters({
        QStringLiteral("Study Music & Audio Files (*.mp3 *.flac *.ogg *.wav *.m4a *.aac *.opus)"),
        QStringLiteral("All Files (*)")
    });

    if (dialog.exec() == QDialog::Accepted) {
        const QStringList selected = dialog.selectedFiles();
        if (!selected.isEmpty()) {
            const QString chosen = selected.first();
            QFileInfo fi(chosen);
            QString dirPath = fi.isDir() ? fi.absoluteFilePath() : fi.absolutePath();
            selectMusicDirectory(dirPath);
            return true;
        }
    }
    return false;
}

void AppController::rescanLibrary()
{
    if (m_settings->hasValidDirectory()) {
        m_libraryScanner->scanDirectory(m_settings->musicDirectory(), m_trackModel);
    } else {
        emit notificationRequested(QStringLiteral("No valid music directory configured."), true);
    }
}

void AppController::rescanWallpapers()
{
    if (m_wallpaperManager) {
        m_wallpaperManager->rescan();
        int count = m_wallpaperManager->wallpapersCount();
        emit notificationRequested(QStringLiteral("Wallpapers rescanned: %1 images found.").arg(count), false);
    }
}

void AppController::onScanCompleted(int count, const QString &error)
{
    if (!error.isEmpty()) {
        emit notificationRequested(error, count == 0);
    } else {
        emit notificationRequested(QStringLiteral("Loaded %1 audio tracks.").arg(count), false);
    }

    if (m_isInitialScan) {
        m_isInitialScan = false;
        m_playlistManager->restoreSavedTrack(m_settings->autoPlay());
    }
}

void AppController::onAudioError(const QString &message)
{
    emit notificationRequested(message, true);
}
