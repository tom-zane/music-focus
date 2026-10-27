#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include <QVariantMap>
#include <QVariantList>
#include <QSettings>
#include <QPoint>

class SettingsManager;

struct WidgetGridItem {
    QString id;
    QString name;
    int x{0};
    int y{0};
    int w{10};
    int h{10};
    int minW{8};
    int minH{6};
    int maxW{120};
    int maxH{80};
    bool visible{true};
    bool preserveAspectRatio{false};

    QVariantMap toMap() const;
    static WidgetGridItem fromMap(const QVariantMap &map, const WidgetGridItem &defaultItem);
};

class LayoutController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int gridColumns READ gridColumns CONSTANT)
    Q_PROPERTY(int gridRows READ gridRows CONSTANT)
    Q_PROPERTY(int layoutRevision READ layoutRevision NOTIFY layoutChanged)
    Q_PROPERTY(bool editing READ isEditing WRITE setEditing NOTIFY editingChanged)
    Q_PROPERTY(QString selectedWidgetId READ selectedWidgetId WRITE setSelectedWidgetId NOTIFY selectedWidgetIdChanged)

public:
    explicit LayoutController(SettingsManager *settings, QObject *parent = nullptr);

    int gridColumns() const { return 120; }
    int gridRows() const { return 80; }
    int layoutRevision() const { return m_layoutRevision; }

    bool isEditing() const { return m_editing; }
    void setEditing(bool editing);

    QString selectedWidgetId() const { return m_selectedWidgetId; }
    void setSelectedWidgetId(const QString &id);

    // Coordinate conversion utilities
    Q_INVOKABLE qreal gridToPixelX(int gx, qreal usableWidth, qreal insetLeft = 0.0) const;
    Q_INVOKABLE qreal gridToPixelY(int gy, qreal usableHeight, qreal insetTop = 0.0) const;
    Q_INVOKABLE qreal gridToPixelWidth(int gw, qreal usableWidth) const;
    Q_INVOKABLE qreal gridToPixelHeight(int gh, qreal usableHeight) const;

    Q_INVOKABLE int pixelToGridX(qreal px, qreal usableWidth, qreal insetLeft = 0.0) const;
    Q_INVOKABLE int pixelToGridY(qreal py, qreal usableHeight, qreal insetTop = 0.0) const;
    Q_INVOKABLE int pixelToGridWidth(qreal pw, qreal usableWidth) const;
    Q_INVOKABLE int pixelToGridHeight(qreal ph, qreal usableHeight) const;

    // Widget layout queries
    Q_INVOKABLE QVariantMap widgetLayout(const QString &id) const;
    Q_INVOKABLE QVariantList allWidgets() const;
    Q_INVOKABLE bool isWidgetRegistered(const QString &id) const;

    // Individual property accessors for fast QML bindings
    Q_INVOKABLE int widgetX(const QString &id) const;
    Q_INVOKABLE int widgetY(const QString &id) const;
    Q_INVOKABLE int widgetWidth(const QString &id) const;
    Q_INVOKABLE int widgetHeight(const QString &id) const;
    Q_INVOKABLE int widgetMinWidth(const QString &id) const;
    Q_INVOKABLE int widgetMinHeight(const QString &id) const;
    Q_INVOKABLE int widgetMaxWidth(const QString &id) const;
    Q_INVOKABLE int widgetMaxHeight(const QString &id) const;
    Q_INVOKABLE bool widgetVisible(const QString &id) const;

    // Movement, resizing, collision and bounds validation
    Q_INVOKABLE bool checkCollision(const QString &id, int testX, int testY, int testW, int testH) const;
    Q_INVOKABLE bool isWithinBounds(int testX, int testY, int testW, int testH) const;
    Q_INVOKABLE bool canMove(const QString &id, int newX, int newY) const;
    Q_INVOKABLE bool canResize(const QString &id, int newW, int newH) const;

    Q_INVOKABLE bool moveWidget(const QString &id, int newX, int newY);
    Q_INVOKABLE bool resizeWidget(const QString &id, int newW, int newH);
    Q_INVOKABLE void setWidgetVisible(const QString &id, bool visible);
    Q_INVOKABLE bool isMandatoryWidget(const QString &id) const;
    Q_INVOKABLE bool addWidget(const QString &id);
    Q_INVOKABLE bool removeWidget(const QString &id);
    Q_INVOKABLE QPoint findAvailablePosition(int w, int h) const;

    // Edit mode toggles and layout resets
    Q_INVOKABLE void toggleEditing();
    Q_INVOKABLE void startEditing();
    Q_INVOKABLE void stopEditing();
    Q_INVOKABLE void resetLayout();
    Q_INVOKABLE void saveLayout();
    Q_INVOKABLE void loadLayout();

    // Responsive screen size adjustment
    Q_INVOKABLE void validateAndAdjustForScreen(int contentWidth, int contentHeight);

signals:
    void editingChanged(bool editing);
    void selectedWidgetIdChanged(const QString &id);
    void layoutChanged();
    void widgetChanged(const QString &id);

private:
    SettingsManager *m_settings;
    int m_layoutRevision{0};
    bool m_editing{false};
    QString m_selectedWidgetId;
    QMap<QString, WidgetGridItem> m_widgets;
    QMap<QString, WidgetGridItem> m_defaultWidgets;

    void initializeDefaults();
};
