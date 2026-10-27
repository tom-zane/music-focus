#pragma once

#include <QObject>
#include <QString>
#include <QList>
#include "TrackModel.h"

class LibraryScanner : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY scanCompleted)
    Q_PROPERTY(int trackCount READ trackCount NOTIFY scanCompleted)

public:
    explicit LibraryScanner(QObject *parent = nullptr);

    bool isScanning() const;
    QString lastError() const;
    int trackCount() const;

    Q_INVOKABLE void scanDirectory(const QString &dirPath, TrackModel *model);

signals:
    void isScanningChanged();
    void scanCompleted(int count, const QString &error);

private:
    bool m_isScanning{false};
    QString m_lastError;
    int m_trackCount{0};
};
