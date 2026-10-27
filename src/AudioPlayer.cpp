#include "AudioPlayer.h"
#include <QTime>
#include <QDebug>
#include <QAudioBufferOutput>
#include <QAudioBuffer>
#include <QTimer>
#include <cmath>
#include <algorithm>
#include <vector>

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
{
    m_player.setAudioOutput(&m_audioOutput);
    m_audioOutput.setVolume(0.75);

    // Initialize 64 spectrum frequency bins for high-density audio visualization
    m_spectrum.resize(64);
    m_spectrum.fill(0.0);

    // Connect real audio buffer output from Qt Multimedia
    m_audioBufferOutput = new QAudioBufferOutput(this);
    m_player.setAudioBufferOutput(m_audioBufferOutput);
    connect(m_audioBufferOutput, &QAudioBufferOutput::audioBufferReceived, this, &AudioPlayer::onAudioBufferReceived);

    // 30fps animation and decay timer
    m_decayTimer = new QTimer(this);
    m_decayTimer->setInterval(33);
    connect(m_decayTimer, &QTimer::timeout, this, &AudioPlayer::onSpectrumDecayTick);
    m_decayTimer->start();

    connect(&m_player, &QMediaPlayer::playbackStateChanged, this, &AudioPlayer::onPlayerPlaybackStateChanged);
    connect(&m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioPlayer::onPlayerMediaStatusChanged);
    connect(&m_player, &QMediaPlayer::positionChanged, this, &AudioPlayer::onPlayerPositionChanged);
    connect(&m_player, &QMediaPlayer::durationChanged, this, &AudioPlayer::onPlayerDurationChanged);
    connect(&m_player, &QMediaPlayer::errorOccurred, this, &AudioPlayer::onPlayerErrorOccurred);
}

AudioPlayer::~AudioPlayer()
{
    m_player.stop();
}

int AudioPlayer::playbackState() const
{
    switch (m_player.playbackState()) {
    case QMediaPlayer::PlayingState:
        return PlayingState;
    case QMediaPlayer::PausedState:
        return PausedState;
    case QMediaPlayer::StoppedState:
    default:
        return StoppedState;
    }
}

bool AudioPlayer::isPlaying() const
{
    return m_player.playbackState() == QMediaPlayer::PlayingState;
}

qint64 AudioPlayer::position() const
{
    return m_player.position();
}

qint64 AudioPlayer::duration() const
{
    return m_player.duration();
}

qreal AudioPlayer::volume() const
{
    return m_audioOutput.volume();
}

bool AudioPlayer::isMuted() const
{
    return m_audioOutput.isMuted();
}

QString AudioPlayer::formattedPosition() const
{
    return formatTime(m_player.position());
}

QString AudioPlayer::formattedDuration() const
{
    return formatTime(m_player.duration());
}

bool AudioPlayer::hasError() const
{
    return m_hasError;
}

QString AudioPlayer::errorMessage() const
{
    return m_errorMessage;
}

QUrl AudioPlayer::currentSource() const
{
    return m_player.source();
}

QList<qreal> AudioPlayer::spectrum() const
{
    return m_spectrum;
}

qreal AudioPlayer::currentEnergy() const
{
    return m_currentEnergy;
}

bool AudioPlayer::hasGenuineSpectrum() const
{
    return m_genuineSpectrumActive;
}

void AudioPlayer::play()
{
    clearError();
    m_player.play();
}

void AudioPlayer::pause()
{
    m_player.pause();
}

void AudioPlayer::togglePlayPause()
{
    if (isPlaying()) {
        pause();
    } else {
        play();
    }
}

void AudioPlayer::stop()
{
    m_player.stop();
}

void AudioPlayer::seek(qint64 positionMs)
{
    clearError();
    if (m_player.isSeekable()) {
        m_player.setPosition(positionMs);
    } else {
        m_pendingSeekMs = positionMs;
    }
}

void AudioPlayer::setPosition(qint64 positionMs)
{
    seek(positionMs);
}

void AudioPlayer::setVolume(qreal vol)
{
    float clamped = static_cast<float>(qBound(0.0, vol, 1.0));
    if (!qFuzzyCompare(m_audioOutput.volume(), clamped)) {
        m_audioOutput.setVolume(clamped);
        emit volumeChanged();
    }
}

void AudioPlayer::setMuted(bool mute)
{
    if (m_audioOutput.isMuted() != mute) {
        m_audioOutput.setMuted(mute);
        emit mutedChanged();
    }
}

void AudioPlayer::loadAndPlay(const QUrl &source, qint64 startPositionMs)
{
    clearError();
    m_pendingSeekMs = startPositionMs > 0 ? startPositionMs : -1;
    m_player.setSource(source);
    emit currentSourceChanged();
    m_player.play();
}

void AudioPlayer::setSourceWithoutPlaying(const QUrl &source, qint64 startPositionMs)
{
    clearError();
    m_pendingSeekMs = startPositionMs > 0 ? startPositionMs : -1;
    m_player.setSource(source);
    emit currentSourceChanged();
}

void AudioPlayer::clearError()
{
    if (m_hasError) {
        m_hasError = false;
        m_errorMessage.clear();
        emit errorStateChanged();
    }
}

void AudioPlayer::onPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState)
{
    emit playbackStateChanged();
}

void AudioPlayer::onPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::BufferedMedia) {
        if (m_pendingSeekMs >= 0 && m_player.isSeekable()) {
            m_player.setPosition(m_pendingSeekMs);
            m_pendingSeekMs = -1;
        }
    } else if (status == QMediaPlayer::EndOfMedia) {
        emit trackEnded();
    }
}

void AudioPlayer::onPlayerPositionChanged(qint64)
{
    emit positionChanged();
}

void AudioPlayer::onPlayerDurationChanged(qint64)
{
    emit durationChanged();
}

void AudioPlayer::onPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    if (error != QMediaPlayer::NoError) {
        m_hasError = true;
        m_errorMessage = errorString.isEmpty() ? QStringLiteral("Unable to decode or play media file.") : errorString;
        qWarning() << "AudioPlayer error:" << error << m_errorMessage;
        emit errorStateChanged();
        emit errorOccurred(m_errorMessage);
    }
}

QString AudioPlayer::formatTime(qint64 ms)
{
    if (ms <= 0) {
        return QStringLiteral("00:00");
    }

    qint64 totalSeconds = ms / 1000;
    qint64 seconds = totalSeconds % 60;
    qint64 minutes = (totalSeconds / 60) % 60;
    qint64 hours = totalSeconds / 3600;

    if (hours > 0) {
        return QStringLiteral("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));
    }

    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void AudioPlayer::onAudioBufferReceived(const QAudioBuffer &buffer)
{
    if (m_player.playbackState() != QMediaPlayer::PlayingState) {
        return;
    }

    int frames = buffer.frameCount();
    if (frames < 32) return;

    auto format = buffer.format();
    int channels = format.channelCount();
    int sampleRate = format.sampleRate();
    if (sampleRate <= 0) sampleRate = 44100;

    int N = std::min(frames, 384);

    // 64 logarithmically spaced frequency bins (50 Hz to 10,000 Hz for prime musical response)
    static const int NUM_BINS = 64;
    static float centerFreqs[NUM_BINS];
    static float bandGains[NUM_BINS];
    static bool tablesInitialized = false;
    if (!tablesInitialized) {
        for (int b = 0; b < NUM_BINS; ++b) {
            float p = static_cast<float>(b) / static_cast<float>(NUM_BINS - 1);
            centerFreqs[b] = 50.0f * std::pow(10000.0f / 50.0f, p);
            // Perceptual equal-loudness curve tailored for acoustic and ambient study music
            bandGains[b] = 1.35f + 1.25f * std::exp(-p * 3.5f) + 1.85f * (p * p);
        }
        tablesInitialized = true;
    }

    std::vector<float> samples(N);
    if (format.sampleFormat() == QAudioFormat::Float) {
        const float *data = buffer.constData<float>();
        for (int i = 0; i < N; ++i) {
            float val = 0.0f;
            for (int c = 0; c < channels; ++c) {
                val += data[i * channels + c];
            }
            samples[i] = val / channels;
        }
    } else if (format.sampleFormat() == QAudioFormat::Int16) {
        const qint16 *data = buffer.constData<qint16>();
        for (int i = 0; i < N; ++i) {
            float val = 0.0f;
            for (int c = 0; c < channels; ++c) {
                val += static_cast<float>(data[i * channels + c]) / 32768.0f;
            }
            samples[i] = val / channels;
        }
    } else if (format.sampleFormat() == QAudioFormat::Int32) {
        const qint32 *data = buffer.constData<qint32>();
        for (int i = 0; i < N; ++i) {
            float val = 0.0f;
            for (int c = 0; c < channels; ++c) {
                val += static_cast<float>(data[i * channels + c]) / 2147483648.0f;
            }
            samples[i] = val / channels;
        }
    } else {
        return;
    }

    if (!m_genuineSpectrumActive) {
        m_genuineSpectrumActive = true;
        emit genuineSpectrumChanged();
    }
    m_buffersSinceLastCheck = 10;

    std::vector<float> rawMags(NUM_BINS, 0.0f);
    float totalEnergy = 0.0f;

    for (int b = 0; b < NUM_BINS; ++b) {
        float f = centerFreqs[b];
        float k = (f * N) / static_cast<float>(sampleRate);
        float omega = 2.0f * 3.1415926535f * k / static_cast<float>(N);

        float real = 0.0f;
        float imag = 0.0f;
        for (int n = 0; n < N; ++n) {
            // Hann windowing to suppress spectral leakage
            float w = 0.5f * (1.0f - std::cos(2.0f * 3.1415926535f * n / static_cast<float>(N)));
            float s = samples[n] * w;
            real += s * std::cos(omega * n);
            imag += s * std::sin(omega * n);
        }

        float mag = std::sqrt(real * real + imag * imag) / (N * 0.22f);
        rawMags[b] = std::clamp(mag * bandGains[b], 0.0f, 1.0f);
    }

    // Spatial neighbor smoothing across frequency bands (prevents isolated noisy spikes)
    std::vector<float> spatiallySmoothed(NUM_BINS);
    for (int b = 0; b < NUM_BINS; ++b) {
        float prev = (b > 0) ? rawMags[b - 1] : rawMags[b];
        float curr = rawMags[b];
        float next = (b < NUM_BINS - 1) ? rawMags[b + 1] : rawMags[b];
        spatiallySmoothed[b] = 0.22f * prev + 0.56f * curr + 0.22f * next;
    }

    // Temporal smoothing: attack vs release (release noticeably slower than attack)
    for (int b = 0; b < NUM_BINS; ++b) {
        float target = spatiallySmoothed[b];
        if (target > m_spectrum[b]) {
            // Smooth attack: 0.35 dampens transient peak harshness
            m_spectrum[b] = m_spectrum[b] * 0.65f + target * 0.35f;
        } else {
            // Slower release: 0.12 ensures smooth, gradual decay
            m_spectrum[b] = m_spectrum[b] * 0.88f + target * 0.12f;
        }

        if (b < 16) {
            totalEnergy += m_spectrum[b];
        }
    }

    m_currentEnergy = totalEnergy / 16.0f;
    emit spectrumChanged();
}

void AudioPlayer::onSpectrumDecayTick()
{
    static const int NUM_BINS = 64;

    if (m_buffersSinceLastCheck > 0) {
        m_buffersSinceLastCheck--;
    } else if (m_genuineSpectrumActive && m_player.playbackState() != QMediaPlayer::PlayingState) {
        m_genuineSpectrumActive = false;
        emit genuineSpectrumChanged();
    }

    bool changed = false;
    if (m_player.playbackState() == QMediaPlayer::PlayingState) {
        if (!m_genuineSpectrumActive) {
            // Ambient organic harmonic wave model fallback for relaxing study atmosphere
            qint64 t = m_player.position();
            float basePhase = t * 0.0016f;
            float totalEnergy = 0.0f;
            for (int b = 0; b < NUM_BINS; ++b) {
                float norm = static_cast<float>(b) / static_cast<float>(NUM_BINS - 1);
                float w1 = std::sin(basePhase * 1.10f + norm * 3.8f);
                float w2 = std::cos(basePhase * 0.70f - norm * 5.2f);
                float w3 = std::sin(basePhase * 1.95f + norm * 2.2f);
                float composite = (w1 * 0.45f + w2 * 0.35f + w3 * 0.20f + 1.0f) * 0.5f;
                // Natural acoustic warmth roll-off in higher frequencies
                float rollOff = std::exp(-norm * 1.3f) * 0.72f + 0.28f;
                float target = std::clamp(composite * rollOff * 0.70f, 0.02f, 0.90f);
                m_spectrum[b] = m_spectrum[b] * 0.82f + target * 0.18f;
                if (b < 16) totalEnergy += m_spectrum[b];
            }
            m_currentEnergy = totalEnergy / 16.0f;
            changed = true;
        } else {
            for (int b = 0; b < NUM_BINS; ++b) {
                if (m_spectrum[b] > 0.001) {
                    m_spectrum[b] *= 0.92;
                    if (m_spectrum[b] < 0.004) m_spectrum[b] = 0.0;
                    changed = true;
                }
            }
            m_currentEnergy *= 0.92;
        }
    } else {
        // Paused or Stopped: smoothly decay to zero
        for (int b = 0; b < NUM_BINS; ++b) {
            if (m_spectrum[b] > 0.001) {
                m_spectrum[b] *= 0.85;
                if (m_spectrum[b] < 0.004) m_spectrum[b] = 0.0;
                changed = true;
            }
        }
        if (m_currentEnergy > 0.001) {
            m_currentEnergy *= 0.85;
            changed = true;
        }
    }

    if (changed) {
        emit spectrumChanged();
    }
}
