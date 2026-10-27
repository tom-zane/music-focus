#include "LibraryScanner.h"
#include <QDir>
#include <QFileInfo>
#include <QCollator>
#include <algorithm>

LibraryScanner::LibraryScanner(QObject *parent)
    : QObject(parent)
{
}

bool LibraryScanner::isScanning() const
{
    return m_isScanning;
}

QString LibraryScanner::lastError() const
{
    return m_lastError;
}

int LibraryScanner::trackCount() const
{
    return m_trackCount;
}

void LibraryScanner::scanDirectory(const QString &dirPath, TrackModel *model)
{
    m_isScanning = true;
    emit isScanningChanged();
    m_lastError.clear();

    if (dirPath.trimmed().isEmpty()) {
        m_lastError = QStringLiteral("No music directory specified. Please select a folder.");
        m_trackCount = 0;
        if (model) {
            model->clear();
        }
        m_isScanning = false;
        emit isScanningChanged();
        emit scanCompleted(0, m_lastError);
        return;
    }

    QDir dir(dirPath);
    if (!dir.exists()) {
        m_lastError = QStringLiteral("Directory does not exist or is unavailable: ") + dirPath;
        m_trackCount = 0;
        if (model) {
            model->clear();
        }
        m_isScanning = false;
        emit isScanningChanged();
        emit scanCompleted(0, m_lastError);
        return;
    }

    QStringList nameFilters;
    nameFilters << QStringLiteral("*.mp3")
                << QStringLiteral("*.flac")
                << QStringLiteral("*.ogg")
                << QStringLiteral("*.wav");

    // Only immediate contents: Files only, no subdirectories, readable
    QFileInfoList fileInfos = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);

    // Natural sort: Track 2 precedes Track 10
    QCollator collator;
    collator.setNumericMode(true);
    collator.setCaseSensitivity(Qt::CaseInsensitive);

    std::sort(fileInfos.begin(), fileInfos.end(), [&collator](const QFileInfo &a, const QFileInfo &b) {
        return collator.compare(a.fileName(), b.fileName()) < 0;
    });

    QList<TrackItem> tracks;
    tracks.reserve(fileInfos.size());

    for (const QFileInfo &info : fileInfos) {
        if (info.isHidden() || info.fileName().startsWith(QLatin1Char('.'))) {
            continue;
        }

        TrackItem item;
        item.fileName = info.fileName();
        item.title = info.completeBaseName();
        item.filePath = info.absoluteFilePath();
        item.fileUrl = QUrl::fromLocalFile(info.absoluteFilePath());
        tracks.append(item);
    }

    m_trackCount = tracks.size();
    if (model) {
        model->setTracks(tracks);
    }

    if (tracks.isEmpty()) {
        m_lastError = QStringLiteral("No supported audio files (.mp3, .flac, .ogg, .wav) found in this folder.");
    }

    m_isScanning = false;
    emit isScanningChanged();
    emit scanCompleted(m_trackCount, m_lastError);
}
