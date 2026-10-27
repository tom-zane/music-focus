#pragma once

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QString>

class AudioPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int playbackState READ playbackState NOTIFY playbackStateChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playbackStateChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)
    Q_PROPERTY(QString formattedPosition READ formattedPosition NOTIFY positionChanged)
    Q_PROPERTY(QString formattedDuration READ formattedDuration NOTIFY durationChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY errorStateChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorStateChanged)
    Q_PROPERTY(QUrl currentSource READ currentSource NOTIFY currentSourceChanged)
    Q_PROPERTY(QList<qreal> spectrum READ spectrum NOTIFY spectrumChanged)
    Q_PROPERTY(qreal currentEnergy READ currentEnergy NOTIFY spectrumChanged)
    Q_PROPERTY(bool hasGenuineSpectrum READ hasGenuineSpectrum NOTIFY genuineSpectrumChanged)

public:
    enum PlaybackState {
        StoppedState = 0,
        PlayingState = 1,
        PausedState = 2
    };
    Q_ENUM(PlaybackState)

    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer() override;

    int playbackState() const;
    bool isPlaying() const;
    qint64 position() const;
    qint64 duration() const;
    qreal volume() const;
    bool isMuted() const;
    QString formattedPosition() const;
    QString formattedDuration() const;
    bool hasError() const;
    QString errorMessage() const;
    QUrl currentSource() const;

    QList<qreal> spectrum() const;
    qreal currentEnergy() const;
    bool hasGenuineSpectrum() const;

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePlayPause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void seek(qint64 positionMs);
    Q_INVOKABLE void setPosition(qint64 positionMs);
    Q_INVOKABLE void setVolume(qreal vol);
    Q_INVOKABLE void setMuted(bool mute);
    Q_INVOKABLE void loadAndPlay(const QUrl &source, qint64 startPositionMs = 0);
    Q_INVOKABLE void setSourceWithoutPlaying(const QUrl &source, qint64 startPositionMs = 0);
    Q_INVOKABLE void clearError();

signals:
    void playbackStateChanged();
    void positionChanged();
    void durationChanged();
    void volumeChanged();
    void mutedChanged();
    void currentSourceChanged();
    void errorStateChanged();
    void trackEnded();
    void errorOccurred(const QString &message);
    void spectrumChanged();
    void genuineSpectrumChanged();

private slots:
    void onPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState state);
    void onPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayerPositionChanged(qint64 position);
    void onPlayerDurationChanged(qint64 duration);
    void onPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
    void onAudioBufferReceived(const class QAudioBuffer &buffer);
    void onSpectrumDecayTick();

private:
    QMediaPlayer m_player;
    QAudioOutput m_audioOutput;
    class QAudioBufferOutput *m_audioBufferOutput{nullptr};
    class QTimer *m_decayTimer{nullptr};

    QList<qreal> m_spectrum;
    qreal m_currentEnergy{0.0};
    bool m_genuineSpectrumActive{false};
    int m_buffersSinceLastCheck{0};

    bool m_hasError{false};
    QString m_errorMessage;
    qint64 m_pendingSeekMs{-1};

    static QString formatTime(qint64 ms);
};
