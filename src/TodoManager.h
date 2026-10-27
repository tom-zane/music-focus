#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QStandardPaths>

struct TodoItem {
    QString id;
    QString text;
    bool completed{false};
};

class TodoManager : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int activeCount READ activeCount NOTIFY countChanged)

public:
    enum TodoRoles {
        IdRole = Qt::UserRole + 1,
        TextRole,
        CompletedRole
    };
    Q_ENUM(TodoRoles)

    explicit TodoManager(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const;
    int activeCount() const;

    Q_INVOKABLE void addTask(const QString &text);
    Q_INVOKABLE void toggleCompleted(int index);
    Q_INVOKABLE void deleteTask(int index);
    Q_INVOKABLE void clearCompleted();

signals:
    void countChanged();

private:
    QList<TodoItem> m_items;
    QString m_storagePath;

    void load();
    void save();
};
