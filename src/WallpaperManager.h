#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QTimer>
#include <QDir>
#include <QSet>
#include "SettingsManager.h"

class WallpaperManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl currentWallpaperUrl READ currentWallpaperUrl NOTIFY currentWallpaperUrlChanged)
    Q_PROPERTY(bool hasWallpaper READ hasWallpaper NOTIFY currentWallpaperUrlChanged)
    Q_PROPERTY(QString currentWallpaperName READ currentWallpaperName NOTIFY currentWallpaperUrlChanged)
    Q_PROPERTY(QStringList availableWallpapers READ availableWallpapers NOTIFY availableWallpapersChanged)
    Q_PROPERTY(int wallpapersCount READ wallpapersCount NOTIFY availableWallpapersChanged)

    Q_PROPERTY(QUrl currentVideoUrl READ currentVideoUrl NOTIFY currentVideoUrlChanged)
    Q_PROPERTY(bool hasVideo READ hasVideo NOTIFY currentVideoUrlChanged)
    Q_PROPERTY(QString currentVideoName READ currentVideoName NOTIFY currentVideoUrlChanged)
    Q_PROPERTY(QStringList availableVideos READ availableVideos NOTIFY availableVideosChanged)
    Q_PROPERTY(int videosCount READ videosCount NOTIFY availableVideosChanged)

    Q_PROPERTY(QString stillWallpapersPath READ stillWallpapersPath NOTIFY directoriesChanged)
    Q_PROPERTY(QString videoWallpapersPath READ videoWallpapersPath NOTIFY directoriesChanged)
    Q_PROPERTY(bool directoryError READ directoryError NOTIFY directoryErrorChanged)
    Q_PROPERTY(QString directoryErrorMessage READ directoryErrorMessage NOTIFY directoryErrorChanged)

public:
    explicit WallpaperManager(SettingsManager *settings, QObject *parent = nullptr);

    QUrl currentWallpaperUrl() const;
    bool hasWallpaper() const;
    QString currentWallpaperName() const;
    QStringList availableWallpapers() const;
    int wallpapersCount() const;

    QUrl currentVideoUrl() const;
    bool hasVideo() const;
    QString currentVideoName() const;
    QStringList availableVideos() const;
    int videosCount() const;

    QString stillWallpapersPath() const;
    QString videoWallpapersPath() const;
    bool directoryError() const;
    QString directoryErrorMessage() const;

    Q_INVOKABLE void rescan();
    Q_INVOKABLE void nextWallpaper();
    Q_INVOKABLE void previousWallpaper();
    Q_INVOKABLE void selectWallpaperByName(const QString &fileName);

    Q_INVOKABLE void nextVideo();
    Q_INVOKABLE void previousVideo();
    Q_INVOKABLE void selectVideoByName(const QString &fileName);
    Q_INVOKABLE void regenerateGradientSeed();

    Q_INVOKABLE QUrl wallpaperUrlFor(const QString &fileName) const;
    Q_INVOKABLE QUrl videoUrlFor(const QString &fileName) const;
    Q_INVOKABLE QUrl videoThumbnailUrl(const QString &fileName);

public slots:
    void updateConfiguration();

signals:
    void currentWallpaperUrlChanged();
    void availableWallpapersChanged();
    void currentVideoUrlChanged();
    void availableVideosChanged();
    void directoriesChanged();
    void directoryErrorChanged();
    void videoThumbnailReady(const QString &fileName, const QUrl &thumbnailUrl);

private slots:
    void onRotationTimeout();

private:
    SettingsManager *m_settings;
    QTimer m_rotationTimer;

    QStringList m_wallpaperFiles;
    int m_currentIndex{-1};
    QUrl m_currentWallpaperUrl;

    QStringList m_videoFiles;
    int m_currentVideoIndex{-1};
    QUrl m_currentVideoUrl;

    QString m_stillWallpapersDir;
    QString m_videoWallpapersDir;
    bool m_directoryError{false};
    QString m_directoryErrorMessage;
    QSet<QString> m_pendingThumbnails;

    void ensureDirectoriesAndMigrate();
    void scanWallpapers();
    void scanVideos();
    void applyCurrentMode();
    void chooseNextRotatedWallpaper();
};
