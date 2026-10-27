#pragma once

#include <QAbstractListModel>
#include <QUrl>
#include <QString>
#include <QList>

struct TrackItem {
    QString title;
    QString fileName;
    QString filePath;
    QUrl fileUrl;
};

class TrackModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum TrackRoles {
        TitleRole = Qt::UserRole + 1,
        FileNameRole,
        FilePathRole,
        FileUrlRole,
        IndexRole
    };
    Q_ENUM(TrackRoles)

    explicit TrackModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
    void setTracks(const QList<TrackItem> &tracks);
    void clear();

    TrackItem trackAt(int index) const;
    int indexOfPath(const QString &filePath) const;

signals:
    void countChanged();

private:
    QList<TrackItem> m_tracks;
};
