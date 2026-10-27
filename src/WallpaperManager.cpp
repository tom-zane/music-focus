#include "WallpaperManager.h"
#include <QFileInfo>
#include <QCollator>
#include <QRandomGenerator>
#include <QFile>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <QThreadPool>
#include <QProcess>
#include <algorithm>
#include <QDebug>

WallpaperManager::WallpaperManager(SettingsManager *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
    m_rotationTimer.setSingleShot(false);
    connect(&m_rotationTimer, &QTimer::timeout, this, &WallpaperManager::onRotationTimeout);

    if (m_settings) {
        connect(m_settings, &SettingsManager::musicDirectoryChanged, this, &WallpaperManager::updateConfiguration);
        connect(m_settings, &SettingsManager::wallpaperModeChanged, this, &WallpaperManager::updateConfiguration);
        connect(m_settings, &SettingsManager::rotateIntervalMinutesChanged, this, &WallpaperManager::updateConfiguration);
        connect(m_settings, &SettingsManager::stillAutoRotateChanged, this, &WallpaperManager::updateConfiguration);
        connect(m_settings, &SettingsManager::fixedWallpaperChanged, this, &WallpaperManager::updateConfiguration);
        connect(m_settings, &SettingsManager::videoWallpaperChanged, this, &WallpaperManager::updateConfiguration);
    }

    updateConfiguration();
}

QUrl WallpaperManager::currentWallpaperUrl() const
{
    return m_currentWallpaperUrl;
}

bool WallpaperManager::hasWallpaper() const
{
    return !m_currentWallpaperUrl.isEmpty();
}

QString WallpaperManager::currentWallpaperName() const
{
    if (m_currentIndex >= 0 && m_currentIndex < m_wallpaperFiles.size()) {
        return m_wallpaperFiles.at(m_currentIndex);
    }
    return QString();
}

QStringList WallpaperManager::availableWallpapers() const
{
    return m_wallpaperFiles;
}

int WallpaperManager::wallpapersCount() const
{
    return m_wallpaperFiles.size();
}

QUrl WallpaperManager::currentVideoUrl() const
{
    return m_currentVideoUrl;
}

bool WallpaperManager::hasVideo() const
{
    return !m_currentVideoUrl.isEmpty();
}

QString WallpaperManager::currentVideoName() const
{
    if (m_currentVideoIndex >= 0 && m_currentVideoIndex < m_videoFiles.size()) {
        return m_videoFiles.at(m_currentVideoIndex);
    }
    return QString();
}

QStringList WallpaperManager::availableVideos() const
{
    return m_videoFiles;
}

int WallpaperManager::videosCount() const
{
    return m_videoFiles.size();
}

QString WallpaperManager::stillWallpapersPath() const
{
    return m_stillWallpapersDir;
}

QString WallpaperManager::videoWallpapersPath() const
{
    return m_videoWallpapersDir;
}

bool WallpaperManager::directoryError() const
{
    return m_directoryError;
}

QString WallpaperManager::directoryErrorMessage() const
{
    return m_directoryErrorMessage;
}

void WallpaperManager::rescan()
{
    ensureDirectoriesAndMigrate();
    scanWallpapers();
    scanVideos();
    applyCurrentMode();
}

void WallpaperManager::nextWallpaper()
{
    if (m_wallpaperFiles.isEmpty()) {
        return;
    }
    m_currentIndex = (m_currentIndex + 1) % m_wallpaperFiles.size();
    QString fileName = m_wallpaperFiles.at(m_currentIndex);
    QString dirPath = m_stillWallpapersDir.isEmpty() ? (m_settings->musicDirectory() + QStringLiteral("/wallpapers")) : m_stillWallpapersDir;
    m_currentWallpaperUrl = QUrl::fromLocalFile(dirPath + QStringLiteral("/") + fileName);
    if (m_settings) {
        m_settings->setFixedWallpaper(fileName);
    }
    emit currentWallpaperUrlChanged();
}

void WallpaperManager::previousWallpaper()
{
    if (m_wallpaperFiles.isEmpty()) {
        return;
    }
    m_currentIndex = (m_currentIndex - 1 + m_wallpaperFiles.size()) % m_wallpaperFiles.size();
    QString fileName = m_wallpaperFiles.at(m_currentIndex);
    QString dirPath = m_stillWallpapersDir.isEmpty() ? (m_settings->musicDirectory() + QStringLiteral("/wallpapers")) : m_stillWallpapersDir;
    m_currentWallpaperUrl = QUrl::fromLocalFile(dirPath + QStringLiteral("/") + fileName);
    if (m_settings) {
        m_settings->setFixedWallpaper(fileName);
    }
    emit currentWallpaperUrlChanged();
}

void WallpaperManager::selectWallpaperByName(const QString &fileName)
{
    int index = m_wallpaperFiles.indexOf(fileName);
    if (index >= 0) {
        m_currentIndex = index;
        QString dirPath = m_stillWallpapersDir.isEmpty() ? (m_settings->musicDirectory() + QStringLiteral("/wallpapers")) : m_stillWallpapersDir;
        m_currentWallpaperUrl = QUrl::fromLocalFile(dirPath + QStringLiteral("/") + fileName);
        if (m_settings) {
            m_settings->setFixedWallpaper(fileName);
        }
        emit currentWallpaperUrlChanged();
    }
}

void WallpaperManager::nextVideo()
{
    if (m_videoFiles.isEmpty()) return;
    m_currentVideoIndex = (m_currentVideoIndex + 1) % m_videoFiles.size();
    QString fileName = m_videoFiles.at(m_currentVideoIndex);
    m_currentVideoUrl = QUrl::fromLocalFile(m_videoWallpapersDir + QStringLiteral("/") + fileName);
    if (m_settings) {
        m_settings->setVideoWallpaper(fileName);
    }
    emit currentVideoUrlChanged();
}

void WallpaperManager::previousVideo()
{
    if (m_videoFiles.isEmpty()) return;
    m_currentVideoIndex = (m_currentVideoIndex - 1 + m_videoFiles.size()) % m_videoFiles.size();
    QString fileName = m_videoFiles.at(m_currentVideoIndex);
    m_currentVideoUrl = QUrl::fromLocalFile(m_videoWallpapersDir + QStringLiteral("/") + fileName);
    if (m_settings) {
        m_settings->setVideoWallpaper(fileName);
    }
    emit currentVideoUrlChanged();
}

void WallpaperManager::selectVideoByName(const QString &fileName)
{
    int index = m_videoFiles.indexOf(fileName);
    if (index >= 0) {
        m_currentVideoIndex = index;
        m_currentVideoUrl = QUrl::fromLocalFile(m_videoWallpapersDir + QStringLiteral("/") + fileName);
        if (m_settings) {
            m_settings->setVideoWallpaper(fileName);
        }
        emit currentVideoUrlChanged();
    }
}

void WallpaperManager::regenerateGradientSeed()
{
    if (m_settings) {
        int newSeed = QRandomGenerator::global()->bounded(10, 999999);
        m_settings->setGradientSeed(newSeed);
    }
}

QUrl WallpaperManager::wallpaperUrlFor(const QString &fileName) const
{
    if (fileName.isEmpty()) return QUrl();
    QString dirPath = m_stillWallpapersDir.isEmpty() ? (m_settings ? m_settings->musicDirectory() + QStringLiteral("/wallpapers") : QString()) : m_stillWallpapersDir;
    return QUrl::fromLocalFile(dirPath + QStringLiteral("/") + fileName);
}

QUrl WallpaperManager::videoUrlFor(const QString &fileName) const
{
    if (fileName.isEmpty()) return QUrl();
    return QUrl::fromLocalFile(m_videoWallpapersDir + QStringLiteral("/") + fileName);
}

QUrl WallpaperManager::videoThumbnailUrl(const QString &fileName)
{
    if (fileName.isEmpty() || m_videoWallpapersDir.isEmpty()) {
        return QUrl();
    }

    QString videoPath = m_videoWallpapersDir + QStringLiteral("/") + fileName;
    if (!QFile::exists(videoPath)) {
        return QUrl();
    }

    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QStringLiteral("/video_thumbnails");
    QDir().mkpath(cacheDir);

    QByteArray hash = QCryptographicHash::hash(videoPath.toUtf8(), QCryptographicHash::Sha1).toHex();
    QString thumbPath = cacheDir + QStringLiteral("/") + QString::fromLatin1(hash) + QStringLiteral(".jpg");

    if (QFile::exists(thumbPath)) {
        return QUrl::fromLocalFile(thumbPath);
    }

    if (m_pendingThumbnails.contains(fileName)) {
        return QUrl();
    }

    m_pendingThumbnails.insert(fileName);

    QThreadPool::globalInstance()->start([this, fileName, videoPath, thumbPath]() {
        QProcess proc;
        QStringList args;
        args << QStringLiteral("-y")
             << QStringLiteral("-ss") << QStringLiteral("00:00:01")
             << QStringLiteral("-i") << videoPath
             << QStringLiteral("-vframes") << QStringLiteral("1")
             << QStringLiteral("-vf") << QStringLiteral("scale=320:-1")
             << thumbPath;

        proc.start(QStringLiteral("ffmpeg"), args);
        bool ok = proc.waitForFinished(10000);

        QMetaObject::invokeMethod(this, [this, fileName, thumbPath, ok]() {
            m_pendingThumbnails.remove(fileName);
            if (ok && QFile::exists(thumbPath)) {
                emit videoThumbnailReady(fileName, QUrl::fromLocalFile(thumbPath));
            } else {
                emit videoThumbnailReady(fileName, QUrl());
            }
        });
    });

    return QUrl();
}

void WallpaperManager::updateConfiguration()
{
    ensureDirectoriesAndMigrate();
    scanWallpapers();
    scanVideos();
    applyCurrentMode();
}

void WallpaperManager::ensureDirectoriesAndMigrate()
{
    if (!m_settings || m_settings->musicDirectory().trimmed().isEmpty()) {
        m_directoryError = false;
        m_directoryErrorMessage.clear();
        emit directoryErrorChanged();
        return;
    }

    QString musicDir = m_settings->musicDirectory();
    QDir base(musicDir);
    if (!base.exists()) {
        m_directoryError = true;
        m_directoryErrorMessage = QStringLiteral("Configured music directory does not exist: ") + musicDir;
        emit directoryErrorChanged();
        return;
    }

    // Canonical structure:
    // MusicDirectory/Wallpapers/Still Wallpapers/
    // MusicDirectory/Wallpapers/Video Wallpapers/
    m_stillWallpapersDir = musicDir + QStringLiteral("/Wallpapers/Still Wallpapers");
    m_videoWallpapersDir = musicDir + QStringLiteral("/Wallpapers/Video Wallpapers");

    QDir dir;
    bool sOk = dir.mkpath(m_stillWallpapersDir);
    bool vOk = dir.mkpath(m_videoWallpapersDir);

    if (!sOk || !vOk) {
        m_directoryError = true;
        m_directoryErrorMessage = QStringLiteral("Cannot create wallpaper folders in music directory (read-only or permission denied).");
    } else {
        m_directoryError = false;
        m_directoryErrorMessage.clear();

        // Safe migration: Check if legacy MusicDirectory/wallpapers exists
        QDir legacyWallpapers(musicDir + QStringLiteral("/wallpapers"));
        if (legacyWallpapers.exists()) {
            QStringList filters;
            filters << QStringLiteral("*.jpg") << QStringLiteral("*.jpeg")
                    << QStringLiteral("*.png") << QStringLiteral("*.webp")
                    << QStringLiteral("*.bmp");
            QFileInfoList list = legacyWallpapers.entryInfoList(filters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
            for (const QFileInfo &info : list) {
                QString destPath = m_stillWallpapersDir + QStringLiteral("/") + info.fileName();
                if (!QFile::exists(destPath)) {
                    QFile::copy(info.absoluteFilePath(), destPath);
                }
            }
        }
    }

    emit directoriesChanged();
    emit directoryErrorChanged();
}

void WallpaperManager::scanWallpapers()
{
    m_wallpaperFiles.clear();

    if (!m_settings || m_settings->musicDirectory().trimmed().isEmpty()) {
        emit availableWallpapersChanged();
        return;
    }

    QStringList filters;
    filters << QStringLiteral("*.jpg")
            << QStringLiteral("*.jpeg")
            << QStringLiteral("*.png")
            << QStringLiteral("*.webp")
            << QStringLiteral("*.bmp");

    QFileInfoList list;
    QDir stillDir(m_stillWallpapersDir);
    if (stillDir.exists()) {
        list = stillDir.entryInfoList(filters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    }

    // Fallback to legacy wallpapers directory if Still Wallpapers is empty
    if (list.isEmpty()) {
        QDir legacyDir(m_settings->musicDirectory() + QStringLiteral("/wallpapers"));
        if (legacyDir.exists()) {
            list = legacyDir.entryInfoList(filters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
            if (!list.isEmpty()) {
                m_stillWallpapersDir = legacyDir.absolutePath();
            }
        }
    }

    // Fallback to musicDirectory itself if both subfolders are empty
    if (list.isEmpty()) {
        QDir musicDir(m_settings->musicDirectory());
        if (musicDir.exists()) {
            list = musicDir.entryInfoList(filters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
            if (!list.isEmpty()) {
                m_stillWallpapersDir = musicDir.absolutePath();
            }
        }
    }

    QCollator collator;
    collator.setNumericMode(true);
    collator.setCaseSensitivity(Qt::CaseInsensitive);

    std::sort(list.begin(), list.end(), [&collator](const QFileInfo &a, const QFileInfo &b) {
        return collator.compare(a.fileName(), b.fileName()) < 0;
    });

    for (const QFileInfo &info : list) {
        if (!info.isHidden() && !info.fileName().startsWith(QLatin1Char('.'))) {
            m_wallpaperFiles.append(info.fileName());
        }
    }

    emit availableWallpapersChanged();
}

void WallpaperManager::scanVideos()
{
    m_videoFiles.clear();

    if (!m_settings || m_settings->musicDirectory().trimmed().isEmpty()) {
        emit availableVideosChanged();
        return;
    }

    QDir videoDir(m_videoWallpapersDir);
    if (!videoDir.exists()) {
        emit availableVideosChanged();
        return;
    }

    QStringList filters;
    filters << QStringLiteral("*.mp4")
            << QStringLiteral("*.mkv")
            << QStringLiteral("*.webm")
            << QStringLiteral("*.mov")
            << QStringLiteral("*.avi");

    QFileInfoList list = videoDir.entryInfoList(filters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);

    QCollator collator;
    collator.setNumericMode(true);
    collator.setCaseSensitivity(Qt::CaseInsensitive);

    std::sort(list.begin(), list.end(), [&collator](const QFileInfo &a, const QFileInfo &b) {
        return collator.compare(a.fileName(), b.fileName()) < 0;
    });

    for (const QFileInfo &info : list) {
        if (!info.isHidden() && !info.fileName().startsWith(QLatin1Char('.'))) {
            m_videoFiles.append(info.fileName());
        }
    }

    emit availableVideosChanged();
}

void WallpaperManager::applyCurrentMode()
{
    if (!m_settings) {
        return;
    }

    int mode = m_settings->wallpaperMode();

    if (mode == SettingsManager::GradientMode) {
        // Mode 0: Deterministic gradient
        m_rotationTimer.stop();
        m_currentIndex = -1;
        m_currentWallpaperUrl = QUrl();
        m_currentVideoIndex = -1;
        m_currentVideoUrl = QUrl();
        emit currentWallpaperUrlChanged();
        emit currentVideoUrlChanged();
        return;
    }

    if (mode == SettingsManager::VideoMode) {
        // Mode 2: Video wallpapers
        m_rotationTimer.stop();
        m_currentIndex = -1;
        m_currentWallpaperUrl = QUrl();
        emit currentWallpaperUrlChanged();

        if (m_videoFiles.isEmpty()) {
            m_currentVideoIndex = -1;
            m_currentVideoUrl = QUrl();
            emit currentVideoUrlChanged();
            return;
        }

        QString selected = m_settings->videoWallpaper();
        int found = m_videoFiles.indexOf(selected);
        if (found >= 0) {
            m_currentVideoIndex = found;
        } else {
            m_currentVideoIndex = 0;
            m_settings->setVideoWallpaper(m_videoFiles.first());
        }
        m_currentVideoUrl = QUrl::fromLocalFile(m_videoWallpapersDir + QStringLiteral("/") + m_videoFiles.at(m_currentVideoIndex));
        emit currentVideoUrlChanged();
        return;
    }

    // Mode 1: Still Images mode
    m_currentVideoIndex = -1;
    m_currentVideoUrl = QUrl();
    emit currentVideoUrlChanged();

    if (m_wallpaperFiles.isEmpty()) {
        m_rotationTimer.stop();
        m_currentIndex = -1;
        m_currentWallpaperUrl = QUrl();
        emit currentWallpaperUrlChanged();
        return;
    }

    QString dirPath = m_stillWallpapersDir.isEmpty() ? (m_settings->musicDirectory() + QStringLiteral("/wallpapers")) : m_stillWallpapersDir;

    if (!m_settings->stillAutoRotate()) {
        m_rotationTimer.stop();
        QString fixedName = m_settings->fixedWallpaper();
        int foundIndex = m_wallpaperFiles.indexOf(fixedName);
        if (foundIndex >= 0) {
            m_currentIndex = foundIndex;
        } else {
            m_currentIndex = 0;
            m_settings->setFixedWallpaper(m_wallpaperFiles.first());
        }
        m_currentWallpaperUrl = QUrl::fromLocalFile(dirPath + QStringLiteral("/") + m_wallpaperFiles.at(m_currentIndex));
        emit currentWallpaperUrlChanged();
        return;
    }

    // Still Auto-Rotate Mode
    int intervalMs = qMax(1, m_settings->rotateIntervalMinutes()) * 60 * 1000;
    m_rotationTimer.setInterval(intervalMs);
    m_rotationTimer.start();

    if (m_currentIndex < 0 || m_currentIndex >= m_wallpaperFiles.size()) {
        QString fixedName = m_settings->fixedWallpaper();
        int idx = m_wallpaperFiles.indexOf(fixedName);
        m_currentIndex = (idx >= 0) ? idx : 0;
    }
    m_currentWallpaperUrl = QUrl::fromLocalFile(dirPath + QStringLiteral("/") + m_wallpaperFiles.at(m_currentIndex));
    emit currentWallpaperUrlChanged();
}

void WallpaperManager::chooseNextRotatedWallpaper()
{
    if (m_wallpaperFiles.isEmpty()) {
        m_currentWallpaperUrl = QUrl();
        m_currentIndex = -1;
        emit currentWallpaperUrlChanged();
        return;
    }

    if (m_wallpaperFiles.size() == 1) {
        m_currentIndex = 0;
    } else {
        // Pick next or random non-matching
        int nextIndex = m_currentIndex;
        while (nextIndex == m_currentIndex) {
            nextIndex = QRandomGenerator::global()->bounded(m_wallpaperFiles.size());
        }
        m_currentIndex = nextIndex;
    }

    QString dirPath = m_stillWallpapersDir.isEmpty() ? (m_settings->musicDirectory() + QStringLiteral("/wallpapers")) : m_stillWallpapersDir;
    m_currentWallpaperUrl = QUrl::fromLocalFile(dirPath + QStringLiteral("/") + m_wallpaperFiles.at(m_currentIndex));
    if (m_settings) {
        m_settings->setFixedWallpaper(m_wallpaperFiles.at(m_currentIndex));
    }
    emit currentWallpaperUrlChanged();
}

void WallpaperManager::onRotationTimeout()
{
    if (m_settings && m_settings->wallpaperMode() == SettingsManager::StillImagesMode && m_settings->stillAutoRotate()) {
        chooseNextRotatedWallpaper();
    }
}
