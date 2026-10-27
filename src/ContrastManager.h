#pragma once

#include <QObject>
#include <QColor>
#include <QImage>
#include <QUrl>
#include <QRectF>
#include <QVector>
#include "SettingsManager.h"
#include "WallpaperManager.h"

class ContrastManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int contrastMode READ contrastMode WRITE setContrastMode NOTIFY contrastModeChanged)
    Q_PROPERTY(bool isDarkBackground READ isDarkBackground NOTIFY colorsChanged)
    Q_PROPERTY(bool isDark READ isDarkBackground NOTIFY colorsChanged)
    Q_PROPERTY(QColor primaryTextColor READ primaryTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor secondaryTextColor READ secondaryTextColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor iconColor READ iconColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor accentColor READ accentColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor panelBackgroundColor READ panelBackgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor panelBg READ panelBackgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor panelBorderColor READ panelBorderColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor panelBorder READ panelBorderColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonBackgroundColor READ buttonBackgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonBg READ buttonBackgroundColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonBorderColor READ buttonBorderColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonBorder READ buttonBorderColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonHoverColor READ buttonHoverColor NOTIFY colorsChanged)

public:
    explicit ContrastManager(SettingsManager *settings, WallpaperManager *wallpaperManager, QObject *parent = nullptr);

    int contrastMode() const;
    void setContrastMode(int mode);

    bool isDarkBackground() const;
    QColor primaryTextColor() const;
    QColor secondaryTextColor() const;
    QColor iconColor() const;
    QColor accentColor() const;
    QColor panelBackgroundColor() const;
    QColor panelBorderColor() const;
    QColor buttonBackgroundColor() const;
    QColor buttonBorderColor() const;
    QColor buttonHoverColor() const;

    Q_INVOKABLE bool isDarkForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE QColor primaryTextForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE QColor secondaryTextForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE QColor iconColorForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE QColor accentColorForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE QColor buttonBgForRect(qreal normX, qreal normY, qreal normW, qreal normH, bool hovered = false) const;
    Q_INVOKABLE QColor buttonBorderForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE QColor panelBgForRect(qreal normX, qreal normY, qreal normW, qreal normH, qreal opacity = -1.0) const;
    Q_INVOKABLE QColor panelBorderForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE qreal relativeLuminanceForRect(qreal normX, qreal normY, qreal normW, qreal normH) const;
    Q_INVOKABLE qreal contrastRatio(const QColor &fg, const QColor &bg) const;

    static qreal calculateRelativeLuminance(const QColor &color);
    static qreal calculateContrast(qreal lum1, qreal lum2);

public slots:
    void updateLuminanceMap();

signals:
    void contrastModeChanged();
    void colorsChanged();

private:
    SettingsManager *m_settings{nullptr};
    WallpaperManager *m_wallpaperManager{nullptr};

    QImage m_luminanceGrid;
    mutable bool m_globalDark{true};

    void buildStillLuminanceMap(const QString &filePath);
    void buildGradientLuminanceMap();
    void buildVideoLuminanceMap(const QString &videoPath);
    QColor sampleRawRegion(qreal normX, qreal normY, qreal normW, qreal normH) const;
    QColor computeEffectiveColor(const QColor &rawSample, bool hasDarkeningPanel) const;
};
