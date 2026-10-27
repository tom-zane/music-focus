#include "TodoManager.h"
#include <QUuid>
#include <QDateTime>

TodoManager::TodoManager(QObject *parent)
    : QAbstractListModel(parent)
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QStringLiteral("/KDE");
    QDir().mkpath(configDir);
    m_storagePath = configDir + QStringLiteral("/MusicFocus_todos.json");
    load();
}

int TodoManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_items.size();
}

QVariant TodoManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return QVariant();
    }

    const TodoItem &item = m_items.at(index.row());
    switch (role) {
    case IdRole:
        return item.id;
    case TextRole:
        return item.text;
    case CompletedRole:
        return item.completed;
    default:
        return QVariant();
    }
}

bool TodoManager::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size()) {
        return false;
    }

    TodoItem &item = m_items[index.row()];
    if (role == CompletedRole) {
        bool comp = value.toBool();
        if (item.completed != comp) {
            item.completed = comp;
            save();
            emit dataChanged(index, index, {CompletedRole});
            emit countChanged();
            return true;
        }
    } else if (role == TextRole) {
        QString txt = value.toString().trimmed();
        if (!txt.isEmpty() && item.text != txt) {
            item.text = txt;
            save();
            emit dataChanged(index, index, {TextRole});
            return true;
        }
    }
    return false;
}

QHash<int, QByteArray> TodoManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TextRole] = "text";
    roles[CompletedRole] = "completed";
    return roles;
}

int TodoManager::count() const
{
    return m_items.size();
}

int TodoManager::activeCount() const
{
    int active = 0;
    for (const auto &it : m_items) {
        if (!it.completed) {
            active++;
        }
    }
    return active;
}

void TodoManager::addTask(const QString &text)
{
    QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    TodoItem item;
    item.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    item.text = trimmed;
    item.completed = false;
    m_items.append(item);
    endInsertRows();

    save();
    emit countChanged();
}

void TodoManager::toggleCompleted(int index)
{
    if (index >= 0 && index < m_items.size()) {
        m_items[index].completed = !m_items[index].completed;
        save();
        QModelIndex modelIdx = createIndex(index, 0);
        emit dataChanged(modelIdx, modelIdx, {CompletedRole});
        emit countChanged();
    }
}

void TodoManager::deleteTask(int index)
{
    if (index >= 0 && index < m_items.size()) {
        beginRemoveRows(QModelIndex(), index, index);
        m_items.removeAt(index);
        endRemoveRows();

        save();
        emit countChanged();
    }
}

void TodoManager::clearCompleted()
{
    for (int i = m_items.size() - 1; i >= 0; --i) {
        if (m_items.at(i).completed) {
            deleteTask(i);
        }
    }
}

void TodoManager::load()
{
    m_items.clear();
    QFile file(m_storagePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        return;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue &val : array) {
        if (val.isObject()) {
            QJsonObject obj = val.toObject();
            TodoItem item;
            item.id = obj.value(QStringLiteral("id")).toString();
            item.text = obj.value(QStringLiteral("text")).toString();
            item.completed = obj.value(QStringLiteral("completed")).toBool(false);
            if (!item.text.isEmpty()) {
                m_items.append(item);
            }
        }
    }
}

void TodoManager::save()
{
    QJsonArray array;
    for (const TodoItem &item : m_items) {
        QJsonObject obj;
        obj.insert(QStringLiteral("id"), item.id);
        obj.insert(QStringLiteral("text"), item.text);
        obj.insert(QStringLiteral("completed"), item.completed);
        array.append(obj);
    }

    QFile file(m_storagePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(array).toJson(QJsonDocument::Indented));
        file.close();
    }
}
