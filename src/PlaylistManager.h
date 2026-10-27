#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include "TrackModel.h"
#include "AudioPlayer.h"
#include "SettingsManager.h"

class PlaylistManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentTrackChanged)
    Q_PROPERTY(QString currentFileName READ currentFileName NOTIFY currentTrackChanged)
    Q_PROPERTY(QString currentFilePath READ currentFilePath NOTIFY currentTrackChanged)
    Q_PROPERTY(bool hasCurrentTrack READ hasCurrentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(bool shuffle READ shuffle WRITE setShuffle NOTIFY shuffleChanged)
    Q_PROPERTY(int repeatMode READ repeatMode WRITE setRepeatMode NOTIFY repeatModeChanged)

public:
    explicit PlaylistManager(TrackModel *model, AudioPlayer *player, SettingsManager *settings, QObject *parent = nullptr);

    int currentIndex() const;
    QString currentTitle() const;
    QString currentFileName() const;
    QString currentFilePath() const;
    bool hasCurrentTrack() const;

    bool shuffle() const;
    void setShuffle(bool enabled);

    int repeatMode() const;
    void setRepeatMode(int mode);

    Q_INVOKABLE void playTrackAtIndex(int index);
    Q_INVOKABLE void playTrackByPath(const QString &filePath);
    Q_INVOKABLE void nextTrack();
    Q_INVOKABLE void previousTrack();
    Q_INVOKABLE void toggleShuffle();
    Q_INVOKABLE void cycleRepeatMode();
    Q_INVOKABLE void clearHistory();
    Q_INVOKABLE void restoreSavedTrack(bool autoPlay);

signals:
    void currentIndexChanged();
    void currentTrackChanged();
    void shuffleChanged();
    void repeatModeChanged();

private slots:
    void onTrackEnded();
    void onModelCountChanged();
    void onPlayerPositionChanged();

private:
    TrackModel *m_model;
    AudioPlayer *m_player;
    SettingsManager *m_settings;

    int m_currentIndex{-1};
    TrackItem m_currentTrack;

    QList<int> m_history;
    QList<int> m_upcomingShuffle;

    void rebuildShuffleList();
    void selectAndPlayIndex(int index, bool autoplay = true, qint64 startPositionMs = 0);
};
