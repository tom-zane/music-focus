#include "LayoutController.h"
#include "SettingsManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>
#include <algorithm>

QVariantMap WidgetGridItem::toMap() const
{
    QVariantMap map;
    map[QStringLiteral("id")] = id;
    map[QStringLiteral("name")] = name;
    map[QStringLiteral("x")] = x;
    map[QStringLiteral("y")] = y;
    map[QStringLiteral("w")] = w;
    map[QStringLiteral("h")] = h;
    map[QStringLiteral("minW")] = minW;
    map[QStringLiteral("minH")] = minH;
    map[QStringLiteral("maxW")] = maxW;
    map[QStringLiteral("maxH")] = maxH;
    map[QStringLiteral("visible")] = visible;
    map[QStringLiteral("preserveAspectRatio")] = preserveAspectRatio;
    return map;
}

WidgetGridItem WidgetGridItem::fromMap(const QVariantMap &map, const WidgetGridItem &defaultItem)
{
    WidgetGridItem item = defaultItem;
    item.id = map.value(QStringLiteral("id"), defaultItem.id).toString();
    item.name = map.value(QStringLiteral("name"), defaultItem.name).toString();
    item.x = qBound(0, map.value(QStringLiteral("x"), defaultItem.x).toInt(), 120 - defaultItem.minW);
    item.y = qBound(0, map.value(QStringLiteral("y"), defaultItem.y).toInt(), 80 - defaultItem.minH);
    item.w = qBound(defaultItem.minW, map.value(QStringLiteral("w"), defaultItem.w).toInt(), defaultItem.maxW);
    item.h = qBound(defaultItem.minH, map.value(QStringLiteral("h"), defaultItem.h).toInt(), defaultItem.maxH);
    item.visible = map.value(QStringLiteral("visible"), defaultItem.visible).toBool();
    item.preserveAspectRatio = map.value(QStringLiteral("preserveAspectRatio"), defaultItem.preserveAspectRatio).toBool();
    return item;
}

LayoutController::LayoutController(SettingsManager *settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
    initializeDefaults();
    loadLayout();

    if (m_settings) {
        // Sync visibility of todo widget with settings
        setWidgetVisible(QStringLiteral("todo"), m_settings->todoVisible());
        connect(m_settings, &SettingsManager::todoVisibleChanged, this, [this]() {
            if (m_widgets.contains(QStringLiteral("todo"))) {
                if (m_widgets[QStringLiteral("todo")].visible != m_settings->todoVisible()) {
                    m_widgets[QStringLiteral("todo")].visible = m_settings->todoVisible();
                    emit widgetChanged(QStringLiteral("todo"));
                    emit layoutChanged();
                }
            }
        });

        // Sync visibility of clock widget with settings
        setWidgetVisible(QStringLiteral("clock"), m_settings->clockVisible());
        connect(m_settings, &SettingsManager::clockVisibleChanged, this, [this]() {
            if (m_widgets.contains(QStringLiteral("clock"))) {
                if (m_widgets[QStringLiteral("clock")].visible != m_settings->clockVisible()) {
                    m_widgets[QStringLiteral("clock")].visible = m_settings->clockVisible();
                    emit widgetChanged(QStringLiteral("clock"));
                    emit layoutChanged();
                }
            }
        });

        // Sync visibility of pomodoro widget with settings
        setWidgetVisible(QStringLiteral("pomodoro"), m_settings->pomodoroVisible());
        connect(m_settings, &SettingsManager::pomodoroVisibleChanged, this, [this]() {
            if (m_widgets.contains(QStringLiteral("pomodoro"))) {
                if (m_widgets[QStringLiteral("pomodoro")].visible != m_settings->pomodoroVisible()) {
                    m_widgets[QStringLiteral("pomodoro")].visible = m_settings->pomodoroVisible();
                    emit widgetChanged(QStringLiteral("pomodoro"));
                    emit layoutChanged();
                }
            }
        });
    }
}

void LayoutController::initializeDefaults()
{
    // Canonical default arrangement on a 120 x 80 logical grid (3:2 ratio)
    // 1. Tracks playlist (bottom-left)
    WidgetGridItem playlist;
    playlist.id = QStringLiteral("playlist");
    playlist.name = QStringLiteral("Tracks List");
    playlist.x = 2;
    playlist.y = 42;
    playlist.w = 38;
    playlist.h = 36;
    playlist.minW = 24;
    playlist.minH = 18;
    playlist.maxW = 60;
    playlist.maxH = 70;
    playlist.visible = true;
    playlist.preserveAspectRatio = false;

    // 2. Playback controls strip (bottom-right)
    WidgetGridItem playback;
    playback.id = QStringLiteral("playback");
    playback.name = QStringLiteral("Playback Controls");
    playback.x = 76;
    playback.y = 68;
    playback.w = 42;
    playback.h = 10;
    playback.minW = 32;
    playback.minH = 8;
    playback.maxW = 80;
    playback.maxH = 14;
    playback.visible = true;
    playback.preserveAspectRatio = false;

    // 3. Floating study tasks / todo widget (top-left)
    WidgetGridItem todo;
    todo.id = QStringLiteral("todo");
    todo.name = QStringLiteral("Study Tasks");
    todo.x = 2;
    todo.y = 2;
    todo.w = 38;
    todo.h = 34;
    todo.minW = 24;
    todo.minH = 18;
    todo.maxW = 60;
    todo.maxH = 65;
    todo.visible = m_settings ? m_settings->todoVisible() : false;
    todo.preserveAspectRatio = false;

    // 4. Focus Clock widget (top-center)
    WidgetGridItem clock;
    clock.id = QStringLiteral("clock");
    clock.name = QStringLiteral("Focus Clock");
    clock.x = 45;
    clock.y = 4;
    clock.w = 30;
    clock.h = 12;
    clock.minW = 4;
    clock.minH = 2;
    clock.maxW = 60;
    clock.maxH = 35;
    clock.visible = m_settings ? m_settings->clockVisible() : false;
    clock.preserveAspectRatio = false;

    // 5. Pomodoro Timer widget
    WidgetGridItem pomodoro;
    pomodoro.id = QStringLiteral("pomodoro");
    pomodoro.name = QStringLiteral("Pomodoro Timer");
    pomodoro.x = 45;
    pomodoro.y = 18;
    pomodoro.w = 30;
    pomodoro.h = 16;
    pomodoro.minW = 10;
    pomodoro.minH = 6;
    pomodoro.maxW = 60;
    pomodoro.maxH = 40;
    pomodoro.visible = m_settings ? m_settings->pomodoroVisible() : false;
    pomodoro.preserveAspectRatio = false;

    m_defaultWidgets.clear();
    m_defaultWidgets.insert(playlist.id, playlist);
    m_defaultWidgets.insert(playback.id, playback);
    m_defaultWidgets.insert(todo.id, todo);
    m_defaultWidgets.insert(clock.id, clock);
    m_defaultWidgets.insert(pomodoro.id, pomodoro);

    m_widgets = m_defaultWidgets;
}

void LayoutController::setEditing(bool editing)
{
    if (m_editing != editing) {
        m_editing = editing;
        if (!m_editing) {
            m_selectedWidgetId.clear();
            emit selectedWidgetIdChanged(m_selectedWidgetId);
        }
        emit editingChanged(m_editing);
    }
}

void LayoutController::setSelectedWidgetId(const QString &id)
{
    if (m_selectedWidgetId != id) {
        m_selectedWidgetId = id;
        emit selectedWidgetIdChanged(m_selectedWidgetId);
    }
}

qreal LayoutController::gridToPixelX(int gx, qreal usableWidth, qreal insetLeft) const
{
    return insetLeft + (static_cast<qreal>(gx) * usableWidth) / static_cast<qreal>(gridColumns());
}

qreal LayoutController::gridToPixelY(int gy, qreal usableHeight, qreal insetTop) const
{
    return insetTop + (static_cast<qreal>(gy) * usableHeight) / static_cast<qreal>(gridRows());
}

qreal LayoutController::gridToPixelWidth(int gw, qreal usableWidth) const
{
    return (static_cast<qreal>(gw) * usableWidth) / static_cast<qreal>(gridColumns());
}

qreal LayoutController::gridToPixelHeight(int gh, qreal usableHeight) const
{
    return (static_cast<qreal>(gh) * usableHeight) / static_cast<qreal>(gridRows());
}

int LayoutController::pixelToGridX(qreal px, qreal usableWidth, qreal insetLeft) const
{
    if (usableWidth <= 0.0) return 0;
    qreal rel = px - insetLeft;
    int gx = qRound((rel * static_cast<qreal>(gridColumns())) / usableWidth);
    return qBound(0, gx, gridColumns());
}

int LayoutController::pixelToGridY(qreal py, qreal usableHeight, qreal insetTop) const
{
    if (usableHeight <= 0.0) return 0;
    qreal rel = py - insetTop;
    int gy = qRound((rel * static_cast<qreal>(gridRows())) / usableHeight);
    return qBound(0, gy, gridRows());
}

int LayoutController::pixelToGridWidth(qreal pw, qreal usableWidth) const
{
    if (usableWidth <= 0.0) return 1;
    int gw = qRound((pw * static_cast<qreal>(gridColumns())) / usableWidth);
    return qBound(1, gw, gridColumns());
}

int LayoutController::pixelToGridHeight(qreal ph, qreal usableHeight) const
{
    if (usableHeight <= 0.0) return 1;
    int gh = qRound((ph * static_cast<qreal>(gridRows())) / usableHeight);
    return qBound(1, gh, gridRows());
}

QVariantMap LayoutController::widgetLayout(const QString &id) const
{
    if (m_widgets.contains(id)) {
        return m_widgets.value(id).toMap();
    }
    return QVariantMap();
}

QVariantList LayoutController::allWidgets() const
{
    QVariantList list;
    for (auto it = m_widgets.constBegin(); it != m_widgets.constEnd(); ++it) {
        list.append(it.value().toMap());
    }
    return list;
}

bool LayoutController::isWidgetRegistered(const QString &id) const
{
    return m_widgets.contains(id);
}

int LayoutController::widgetX(const QString &id) const
{
    return m_widgets.value(id).x;
}

int LayoutController::widgetY(const QString &id) const
{
    return m_widgets.value(id).y;
}

int LayoutController::widgetWidth(const QString &id) const
{
    return m_widgets.value(id).w;
}

int LayoutController::widgetHeight(const QString &id) const
{
    return m_widgets.value(id).h;
}

int LayoutController::widgetMinWidth(const QString &id) const
{
    return m_widgets.value(id).minW;
}

int LayoutController::widgetMinHeight(const QString &id) const
{
    return m_widgets.value(id).minH;
}

int LayoutController::widgetMaxWidth(const QString &id) const
{
    return m_widgets.value(id).maxW;
}

int LayoutController::widgetMaxHeight(const QString &id) const
{
    return m_widgets.value(id).maxH;
}

bool LayoutController::widgetVisible(const QString &id) const
{
    return m_widgets.value(id).visible;
}

bool LayoutController::isWithinBounds(int testX, int testY, int testW, int testH) const
{
    return testX >= 0 && testY >= 0 && (testX + testW) <= gridColumns() && (testY + testH) <= gridRows();
}

bool LayoutController::checkCollision(const QString &id, int testX, int testY, int testW, int testH) const
{
    for (auto it = m_widgets.constBegin(); it != m_widgets.constEnd(); ++it) {
        if (it.key() == id) {
            continue;
        }
        const WidgetGridItem &other = it.value();
        if (!other.visible) {
            continue;
        }

        // Axis-Aligned Bounding Box collision detection:
        // Widgets may share edges (e.g. testX + testW == other.x), but must not occupy overlapping area.
        bool overlap = (testX < other.x + other.w) &&
                       (testX + testW > other.x) &&
                       (testY < other.y + other.h) &&
                       (testY + testH > other.y);

        if (overlap) {
            return true;
        }
    }
    return false;
}

bool LayoutController::canMove(const QString &id, int newX, int newY) const
{
    if (!m_widgets.contains(id)) return false;
    const WidgetGridItem &w = m_widgets.value(id);
    if (!isWithinBounds(newX, newY, w.w, w.h)) {
        return false;
    }
    return !checkCollision(id, newX, newY, w.w, w.h);
}

bool LayoutController::canResize(const QString &id, int newW, int newH) const
{
    if (!m_widgets.contains(id)) return false;
    const WidgetGridItem &w = m_widgets.value(id);
    if (newW < w.minW || newW > w.maxW || newH < w.minH || newH > w.maxH) {
        return false;
    }
    if (!isWithinBounds(w.x, w.y, newW, newH)) {
        return false;
    }
    return !checkCollision(id, w.x, w.y, newW, newH);
}

bool LayoutController::moveWidget(const QString &id, int newX, int newY)
{
    if (!canMove(id, newX, newY)) {
        return false;
    }

    m_widgets[id].x = newX;
    m_widgets[id].y = newY;
    ++m_layoutRevision;
    emit widgetChanged(id);
    emit layoutChanged();
    saveLayout();
    return true;
}

bool LayoutController::resizeWidget(const QString &id, int newW, int newH)
{
    if (!canResize(id, newW, newH)) {
        return false;
    }

    m_widgets[id].w = newW;
    m_widgets[id].h = newH;
    ++m_layoutRevision;
    emit widgetChanged(id);
    emit layoutChanged();
    saveLayout();
    return true;
}

void LayoutController::setWidgetVisible(const QString &id, bool visible)
{
    if (isMandatoryWidget(id) && !visible) {
        // Mandatory widgets (Music Control, Playlist) can never be removed or hidden
        return;
    }

    if (m_widgets.contains(id)) {
        if (m_widgets[id].visible != visible) {
            m_widgets[id].visible = visible;
            ++m_layoutRevision;
            emit widgetChanged(id);
            emit layoutChanged();
            saveLayout();

            if (m_settings) {
                if (id == QStringLiteral("todo") && m_settings->todoVisible() != visible) {
                    m_settings->setTodoVisible(visible);
                } else if (id == QStringLiteral("clock") && m_settings->clockVisible() != visible) {
                    m_settings->setClockVisible(visible);
                } else if (id == QStringLiteral("pomodoro") && m_settings->pomodoroVisible() != visible) {
                    m_settings->setPomodoroVisible(visible);
                }
            }
        }
    }
}

bool LayoutController::isMandatoryWidget(const QString &id) const
{
    return (id == QStringLiteral("playback") || id == QStringLiteral("playlist"));
}

bool LayoutController::addWidget(const QString &id)
{
    if (!m_widgets.contains(id)) {
        return false;
    }
    WidgetGridItem &w = m_widgets[id];
    if (w.visible) {
        return true; // Already on desktop
    }

    // Check if current position collides with visible widgets or is out of bounds
    if (checkCollision(id, w.x, w.y, w.w, w.h) || !isWithinBounds(w.x, w.y, w.w, w.h)) {
        QPoint pos = findAvailablePosition(w.w, w.h);
        w.x = pos.x();
        w.y = pos.y();
    }
    w.visible = true;
    ++m_layoutRevision;
    emit widgetChanged(id);
    emit layoutChanged();
    saveLayout();

    if (m_settings) {
        if (id == QStringLiteral("todo")) m_settings->setTodoVisible(true);
        if (id == QStringLiteral("clock")) m_settings->setClockVisible(true);
        if (id == QStringLiteral("pomodoro")) m_settings->setPomodoroVisible(true);
    }
    return true;
}

bool LayoutController::removeWidget(const QString &id)
{
    if (isMandatoryWidget(id)) {
        return false; // Mandatory widgets cannot be removed
    }
    if (!m_widgets.contains(id)) {
        return false;
    }
    setWidgetVisible(id, false);
    return true;
}

QPoint LayoutController::findAvailablePosition(int w, int h) const
{
    for (int gy = 2; gy <= gridRows() - h; gy += 2) {
        for (int gx = 2; gx <= gridColumns() - w; gx += 2) {
            if (!checkCollision(QString(), gx, gy, w, h)) {
                return QPoint(gx, gy);
            }
        }
    }
    return QPoint(2, 2);
}

void LayoutController::toggleEditing()
{
    setEditing(!m_editing);
}

void LayoutController::startEditing()
{
    setEditing(true);
}

void LayoutController::stopEditing()
{
    setEditing(false);
}

void LayoutController::resetLayout()
{
    m_widgets = m_defaultWidgets;
    if (m_settings) {
        m_widgets[QStringLiteral("todo")].visible = m_settings->todoVisible();
        m_widgets[QStringLiteral("clock")].visible = m_settings->clockVisible();
        m_widgets[QStringLiteral("pomodoro")].visible = m_settings->pomodoroVisible();
    }
    ++m_layoutRevision;
    saveLayout();
    emit layoutChanged();
    for (const auto &id : m_widgets.keys()) {
        emit widgetChanged(id);
    }
}

void LayoutController::saveLayout()
{
    QJsonObject rootObj;
    rootObj[QStringLiteral("version")] = 1;
    rootObj[QStringLiteral("gridColumns")] = gridColumns();
    rootObj[QStringLiteral("gridRows")] = gridRows();

    QJsonObject widgetsObj;
    for (auto it = m_widgets.constBegin(); it != m_widgets.constEnd(); ++it) {
        QJsonObject wObj;
        wObj[QStringLiteral("x")] = it.value().x;
        wObj[QStringLiteral("y")] = it.value().y;
        wObj[QStringLiteral("w")] = it.value().w;
        wObj[QStringLiteral("h")] = it.value().h;
        wObj[QStringLiteral("visible")] = it.value().visible;
        widgetsObj[it.key()] = wObj;
    }
    rootObj[QStringLiteral("widgets")] = widgetsObj;

    QJsonDocument doc(rootObj);
    QSettings rawSettings("KDE", "MusicFocus");
    rawSettings.setValue("layout/gridConfigV1", QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    rawSettings.sync();
}

void LayoutController::loadLayout()
{
    QSettings rawSettings("KDE", "MusicFocus");
    QString jsonStr = rawSettings.value("layout/gridConfigV1").toString();
    if (jsonStr.trimmed().isEmpty()) {
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    int savedCols = root.value(QStringLiteral("gridColumns")).toInt(120);
    int savedRows = root.value(QStringLiteral("gridRows")).toInt(80);

    QJsonObject widgetsObj = root.value(QStringLiteral("widgets")).toObject();
    for (auto it = m_defaultWidgets.constBegin(); it != m_defaultWidgets.constEnd(); ++it) {
        const QString &id = it.key();
        if (widgetsObj.contains(id)) {
            QJsonObject wObj = widgetsObj.value(id).toObject();
            WidgetGridItem item = it.value();

            int sx = wObj.value(QStringLiteral("x")).toInt(item.x);
            int sy = wObj.value(QStringLiteral("y")).toInt(item.y);
            int sw = wObj.value(QStringLiteral("w")).toInt(item.w);
            int sh = wObj.value(QStringLiteral("h")).toInt(item.h);

            // Migration support: scale if saved from different grid dimensions
            if (savedCols != gridColumns() && savedCols > 0) {
                sx = qRound((static_cast<qreal>(sx) * gridColumns()) / savedCols);
                sw = qRound((static_cast<qreal>(sw) * gridColumns()) / savedCols);
            }
            if (savedRows != gridRows() && savedRows > 0) {
                sy = qRound((static_cast<qreal>(sy) * gridRows()) / savedRows);
                sh = qRound((static_cast<qreal>(sh) * gridRows()) / savedRows);
            }

            item.w = qBound(item.minW, sw, item.maxW);
            item.h = qBound(item.minH, sh, item.maxH);
            item.x = qBound(0, sx, gridColumns() - item.w);
            item.y = qBound(0, sy, gridRows() - item.h);

            if (wObj.contains(QStringLiteral("visible"))) {
                item.visible = wObj.value(QStringLiteral("visible")).toBool(item.visible);
            }

            m_widgets[id] = item;
        }
    }

    // Validate that loaded layout has no collisions; if collision occurs, restore default
    for (auto it = m_widgets.constBegin(); it != m_widgets.constEnd(); ++it) {
        if (!it.value().visible) continue;
        if (checkCollision(it.key(), it.value().x, it.value().y, it.value().w, it.value().h)) {
            m_widgets = m_defaultWidgets;
            break;
        }
    }
}

void LayoutController::validateAndAdjustForScreen(int contentWidth, int contentHeight)
{
    Q_UNUSED(contentWidth);
    Q_UNUSED(contentHeight);

    // Keep all widgets strictly inside bounds
    for (auto it = m_widgets.begin(); it != m_widgets.end(); ++it) {
        WidgetGridItem &w = it.value();
        if (w.x + w.w > gridColumns()) {
            w.x = qMax(0, gridColumns() - w.w);
        }
        if (w.y + w.h > gridRows()) {
            w.y = qMax(0, gridRows() - w.h);
        }
    }
}
