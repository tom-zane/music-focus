#include "TrackModel.h"

TrackModel::TrackModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TrackModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_tracks.size();
}

QVariant TrackModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_tracks.size()) {
        return QVariant();
    }

    const TrackItem &item = m_tracks.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
    case TitleRole:
        return item.title;
    case FileNameRole:
        return item.fileName;
    case FilePathRole:
        return item.filePath;
    case FileUrlRole:
        return item.fileUrl;
    case IndexRole:
        return index.row();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TrackModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    roles[FileUrlRole] = "fileUrl";
    roles[IndexRole] = "trackIndex";
    return roles;
}

int TrackModel::count() const
{
    return m_tracks.size();
}

void TrackModel::setTracks(const QList<TrackItem> &tracks)
{
    beginResetModel();
    m_tracks = tracks;
    endResetModel();
    emit countChanged();
}

void TrackModel::clear()
{
    beginResetModel();
    m_tracks.clear();
    endResetModel();
    emit countChanged();
}

TrackItem TrackModel::trackAt(int index) const
{
    if (index >= 0 && index < m_tracks.size()) {
        return m_tracks.at(index);
    }
    return TrackItem();
}

int TrackModel::indexOfPath(const QString &filePath) const
{
    for (int i = 0; i < m_tracks.size(); ++i) {
        if (m_tracks.at(i).filePath == filePath) {
            return i;
        }
    }
    return -1;
}
