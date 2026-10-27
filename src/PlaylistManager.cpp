#include "PlaylistManager.h"
#include <QRandomGenerator>
#include <algorithm>
#include <numeric>

PlaylistManager::PlaylistManager(TrackModel *model, AudioPlayer *player, SettingsManager *settings, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_player(player)
    , m_settings(settings)
{
    if (m_settings) {
        connect(m_settings, &SettingsManager::shuffleChanged, this, [this]() {
            if (shuffle()) {
                rebuildShuffleList();
            } else {
                m_upcomingShuffle.clear();
            }
            emit shuffleChanged();
        });
        connect(m_settings, &SettingsManager::repeatModeChanged, this, &PlaylistManager::repeatModeChanged);
    }

    if (m_player) {
        connect(m_player, &AudioPlayer::trackEnded, this, &PlaylistManager::onTrackEnded);
        connect(m_player, &AudioPlayer::positionChanged, this, &PlaylistManager::onPlayerPositionChanged);
    }

    if (m_model) {
        connect(m_model, &TrackModel::countChanged, this, &PlaylistManager::onModelCountChanged);
    }
}

int PlaylistManager::currentIndex() const
{
    return m_currentIndex;
}

QString PlaylistManager::currentTitle() const
{
    return m_currentTrack.title;
}

QString PlaylistManager::currentFileName() const
{
    return m_currentTrack.fileName;
}

QString PlaylistManager::currentFilePath() const
{
    return m_currentTrack.filePath;
}

bool PlaylistManager::hasCurrentTrack() const
{
    return m_currentIndex >= 0 && !m_currentTrack.filePath.isEmpty();
}

bool PlaylistManager::shuffle() const
{
    return m_settings ? m_settings->shuffle() : false;
}

void PlaylistManager::setShuffle(bool enabled)
{
    if (m_settings && m_settings->shuffle() != enabled) {
        m_settings->setShuffle(enabled);
        if (enabled) {
            rebuildShuffleList();
        } else {
            m_upcomingShuffle.clear();
        }
    }
}

int PlaylistManager::repeatMode() const
{
    return m_settings ? m_settings->repeatMode() : SettingsManager::RepeatAll;
}

void PlaylistManager::setRepeatMode(int mode)
{
    if (m_settings && m_settings->repeatMode() != mode) {
        m_settings->setRepeatMode(mode);
    }
}

void PlaylistManager::playTrackAtIndex(int index)
{
    if (!m_model || index < 0 || index >= m_model->count()) {
        return;
    }

    if (m_currentIndex >= 0 && m_currentIndex != index) {
        m_history.append(m_currentIndex);
        if (m_history.size() > 50) {
            m_history.removeFirst();
        }
    }

    selectAndPlayIndex(index, true, 0);
}

void PlaylistManager::playTrackByPath(const QString &filePath)
{
    if (!m_model) {
        return;
    }
    int idx = m_model->indexOfPath(filePath);
    if (idx >= 0) {
        playTrackAtIndex(idx);
    }
}

void PlaylistManager::nextTrack()
{
    if (!m_model || m_model->count() == 0) {
        return;
    }

    int count = m_model->count();
    bool isShuffle = shuffle();
    int rMode = repeatMode();

    if (isShuffle) {
        if (m_upcomingShuffle.isEmpty()) {
            if (rMode == SettingsManager::RepeatOff) {
                if (m_player) {
                    m_player->stop();
                }
                return;
            }
            rebuildShuffleList();
        }

        if (!m_upcomingShuffle.isEmpty()) {
            int nextIdx = m_upcomingShuffle.takeFirst();
            if (m_currentIndex >= 0) {
                m_history.append(m_currentIndex);
                if (m_history.size() > 50) {
                    m_history.removeFirst();
                }
            }
            selectAndPlayIndex(nextIdx, true, 0);
            return;
        }
    }

    // Linear playback
    int nextIdx = m_currentIndex + 1;
    if (nextIdx >= count) {
        if (rMode == SettingsManager::RepeatOff) {
            if (m_player) {
                m_player->stop();
            }
            return;
        }
        nextIdx = 0;
    }

    if (m_currentIndex >= 0) {
        m_history.append(m_currentIndex);
        if (m_history.size() > 50) {
            m_history.removeFirst();
        }
    }
    selectAndPlayIndex(nextIdx, true, 0);
}

void PlaylistManager::previousTrack()
{
    if (!m_model || m_model->count() == 0) {
        return;
    }

    // If currently playing past 3 seconds, seek to start of track first
    if (m_player && m_player->isPlaying() && m_player->position() > 3000) {
        m_player->seek(0);
        return;
    }

    if (shuffle() && !m_history.isEmpty()) {
        int prevIdx = m_history.takeLast();
        if (prevIdx >= 0 && prevIdx < m_model->count()) {
            selectAndPlayIndex(prevIdx, true, 0);
            return;
        }
    }

    // Fall back to previous linear index
    int prevIdx = m_currentIndex - 1;
    if (prevIdx < 0) {
        prevIdx = m_model->count() - 1;
    }
    selectAndPlayIndex(prevIdx, true, 0);
}

void PlaylistManager::toggleShuffle()
{
    setShuffle(!shuffle());
}

void PlaylistManager::cycleRepeatMode()
{
    int current = repeatMode();
    int next = (current + 1) % 3;
    setRepeatMode(next);
}

void PlaylistManager::clearHistory()
{
    m_history.clear();
}

void PlaylistManager::restoreSavedTrack(bool autoPlay)
{
    if (!m_model || !m_settings || m_model->count() == 0) {
        return;
    }

    int targetIndex = 0;
    qint64 startPos = 0;

    if (m_settings->rememberLastTrack()) {
        QString savedPath = m_settings->lastTrackPath();
        int foundIdx = m_model->indexOfPath(savedPath);
        if (foundIdx >= 0) {
            targetIndex = foundIdx;
            if (m_settings->resumePlaybackPosition()) {
                startPos = m_settings->lastPositionMs();
            }
        }
    }

    selectAndPlayIndex(targetIndex, autoPlay, startPos);
}

void PlaylistManager::onTrackEnded()
{
    if (repeatMode() == SettingsManager::RepeatOne) {
        if (m_player) {
            m_player->seek(0);
            m_player->play();
        }
        return;
    }

    nextTrack();
}

void PlaylistManager::onModelCountChanged()
{
    if (!m_model || m_model->count() == 0) {
        m_currentIndex = -1;
        m_currentTrack = TrackItem();
        m_history.clear();
        m_upcomingShuffle.clear();
        emit currentIndexChanged();
        emit currentTrackChanged();
        return;
    }

    if (m_currentIndex >= m_model->count()) {
        m_currentIndex = -1;
        m_currentTrack = TrackItem();
        emit currentIndexChanged();
        emit currentTrackChanged();
    } else if (m_currentIndex >= 0) {
        // Keep track item updated
        m_currentTrack = m_model->trackAt(m_currentIndex);
        emit currentTrackChanged();
    }

    if (shuffle()) {
        rebuildShuffleList();
    }
}

void PlaylistManager::onPlayerPositionChanged()
{
    if (m_settings && m_settings->resumePlaybackPosition() && m_player) {
        qint64 pos = m_player->position();
        // Update persisted position roughly every 3 seconds to avoid disk churn
        if (qAbs(pos - m_settings->lastPositionMs()) >= 3000) {
            m_settings->setLastPositionMs(pos);
        }
    }
}

void PlaylistManager::rebuildShuffleList()
{
    m_upcomingShuffle.clear();
    if (!m_model || m_model->count() <= 1) {
        return;
    }

    int count = m_model->count();
    m_upcomingShuffle.resize(count);
    std::iota(m_upcomingShuffle.begin(), m_upcomingShuffle.end(), 0);

    auto rng = QRandomGenerator::global();
    for (int i = count - 1; i > 0; --i) {
        int j = rng->bounded(i + 1);
        std::swap(m_upcomingShuffle[i], m_upcomingShuffle[j]);
    }

    // Remove current index if present so we don't immediately repeat
    if (m_currentIndex >= 0) {
        m_upcomingShuffle.removeAll(m_currentIndex);
    }
}

void PlaylistManager::selectAndPlayIndex(int index, bool autoplay, qint64 startPositionMs)
{
    if (!m_model || index < 0 || index >= m_model->count()) {
        return;
    }

    m_currentIndex = index;
    m_currentTrack = m_model->trackAt(index);

    if (shuffle()) {
        m_upcomingShuffle.removeAll(index);
    }

    if (m_settings && m_settings->rememberLastTrack()) {
        m_settings->setLastTrackPath(m_currentTrack.filePath);
    }

    emit currentIndexChanged();
    emit currentTrackChanged();

    if (m_player) {
        if (autoplay) {
            m_player->loadAndPlay(m_currentTrack.fileUrl, startPositionMs);
        } else {
            m_player->setSourceWithoutPlaying(m_currentTrack.fileUrl, startPositionMs);
        }
    }
}
