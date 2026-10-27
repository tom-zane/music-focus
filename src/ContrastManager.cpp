#include "ContrastManager.h"
#include <QImageReader>
#include <QFileInfo>
#include <cmath>

static inline qreal srgbToLinear(qreal c)
{
    return (c <= 0.04045) ? (c / 12.92) : std::pow((c + 0.055) / 1.055, 2.4);
}

ContrastManager::ContrastManager(SettingsManager *settings, WallpaperManager *wallpaperManager, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
    , m_wallpaperManager(wallpaperManager)
{
    m_luminanceGrid = QImage(64, 36, QImage::Format_RGB888);
    m_luminanceGrid.fill(QColor(24, 25, 38));

    if (m_wallpaperManager) {
        connect(m_wallpaperManager, &WallpaperManager::currentWallpaperUrlChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_wallpaperManager, &WallpaperManager::currentVideoUrlChanged,
                this, &ContrastManager::updateLuminanceMap);
    }

    if (m_settings) {
        connect(m_settings, &SettingsManager::wallpaperModeChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_settings, &SettingsManager::dimmingOpacityChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_settings, &SettingsManager::darkeningOpacityChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_settings, &SettingsManager::darkeningEnabledChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_settings, &SettingsManager::gradientSeedChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_settings, &SettingsManager::gradientStyleChanged,
                this, &ContrastManager::updateLuminanceMap);
        connect(m_settings, &SettingsManager::contrastModeChanged,
                this, [this]() {
                    emit contrastModeChanged();
                    updateLuminanceMap();
                });
    }

    updateLuminanceMap();
}

int ContrastManager::contrastMode() const
{
    return m_settings ? m_settings->contrastMode() : SettingsManager::ContrastAuto;
}

void ContrastManager::setContrastMode(int mode)
{
    if (m_settings) {
        m_settings->setContrastMode(mode);
    }
}

qreal ContrastManager::calculateRelativeLuminance(const QColor &color)
{
    qreal r = srgbToLinear(color.redF());
    qreal g = srgbToLinear(color.greenF());
    qreal b = srgbToLinear(color.blueF());
    return 0.2126 * r + 0.7152 * g + 0.0722 * b;
}

qreal ContrastManager::calculateContrast(qreal lum1, qreal lum2)
{
    qreal lmax = qMax(lum1, lum2);
    qreal lmin = qMin(lum1, lum2);
    return (lmax + 0.05) / (lmin + 0.05);
}

qreal ContrastManager::contrastRatio(const QColor &fg, const QColor &bg) const
{
    return calculateContrast(calculateRelativeLuminance(fg), calculateRelativeLuminance(bg));
}

void ContrastManager::buildStillLuminanceMap(const QString &filePath)
{
    if (filePath.isEmpty() || !QFileInfo::exists(filePath)) {
        m_luminanceGrid.fill(QColor(24, 25, 38));
        return;
    }

    QImageReader reader(filePath);
    reader.setScaledSize(QSize(64, 36));
    QImage scaled = reader.read();
    if (!scaled.isNull()) {
        m_luminanceGrid = scaled.convertToFormat(QImage::Format_RGB888);
    } else {
        m_luminanceGrid.fill(QColor(24, 25, 38));
    }
}

void ContrastManager::buildGradientLuminanceMap()
{
    const QVector<QVector<QColor>> palettes = {
        { QColor(QStringLiteral("#181926")), QColor(QStringLiteral("#25273e")), QColor(QStringLiteral("#353754")), QColor(QStringLiteral("#494d6e")) },
        { QColor(QStringLiteral("#141c19")), QColor(QStringLiteral("#1d2d27")), QColor(QStringLiteral("#2b4038")), QColor(QStringLiteral("#3c574c")) },
        { QColor(QStringLiteral("#16192b")), QColor(QStringLiteral("#1f2742")), QColor(QStringLiteral("#2e3b5e")), QColor(QStringLiteral("#41537d")) },
        { QColor(QStringLiteral("#1c1723")), QColor(QStringLiteral("#2a2135")), QColor(QStringLiteral("#3c2e4c")), QColor(QStringLiteral("#523f66")) },
        { QColor(QStringLiteral("#20171e")), QColor(QStringLiteral("#31202e")), QColor(QStringLiteral("#442b3f")), QColor(QStringLiteral("#5c3a55")) },
        { QColor(QStringLiteral("#1e1917")), QColor(QStringLiteral("#2c2320")), QColor(QStringLiteral("#3d302c")), QColor(QStringLiteral("#52403b")) }
    };

    int seed = m_settings ? m_settings->gradientSeed() : 42;
    int style = m_settings ? m_settings->gradientStyle() : 0;
    int pIdx = std::abs(seed) % palettes.size();
    const auto &p = palettes[pIdx];

    for (int y = 0; y < 36; ++y) {
        qreal vy = static_cast<qreal>(y) / 35.0;
        for (int x = 0; x < 64; ++x) {
            qreal vx = static_cast<qreal>(x) / 63.0;
            QColor c;
            if (style == 0) { // Linear Pastel Vertical
                if (vy < 0.4) {
                    qreal t = vy / 0.4;
                    c = QColor::fromRgbF(
                        p[0].redF() * (1 - t) + p[1].redF() * t,
                        p[0].greenF() * (1 - t) + p[1].greenF() * t,
                        p[0].blueF() * (1 - t) + p[1].blueF() * t);
                } else if (vy < 0.8) {
                    qreal t = (vy - 0.4) / 0.4;
                    c = QColor::fromRgbF(
                        p[1].redF() * (1 - t) + p[2].redF() * t,
                        p[1].greenF() * (1 - t) + p[2].greenF() * t,
                        p[1].blueF() * (1 - t) + p[2].blueF() * t);
                } else {
                    qreal t = (vy - 0.8) / 0.2;
                    c = QColor::fromRgbF(
                        p[2].redF() * (1 - t) + p[3].redF() * t,
                        p[2].greenF() * (1 - t) + p[3].greenF() * t,
                        p[2].blueF() * (1 - t) + p[3].blueF() * t);
                }
            } else if (style == 1) { // Radial Pastel
                qreal dx = vx - 0.5;
                qreal dy = vy - 0.5;
                qreal dist = qBound(0.0, std::sqrt(dx * dx + dy * dy) * 1.5, 1.0);
                c = QColor::fromRgbF(
                    p[3].redF() * (1 - dist) + p[0].redF() * dist,
                    p[3].greenF() * (1 - dist) + p[0].greenF() * dist,
                    p[3].blueF() * (1 - dist) + p[0].blueF() * dist);
            } else { // Horizontal / Angled / Mesh
                qreal t = (vx + vy) * 0.5;
                c = QColor::fromRgbF(
                    p[0].redF() * (1 - t) + p[3].redF() * t,
                    p[0].greenF() * (1 - t) + p[3].greenF() * t,
                    p[0].blueF() * (1 - t) + p[3].blueF() * t);
            }
            m_luminanceGrid.setPixelColor(x, y, c);
        }
    }
}

void ContrastManager::buildVideoLuminanceMap(const QString &videoPath)
{
    if (m_wallpaperManager && !videoPath.isEmpty()) {
        QFileInfo fi(videoPath);
        QUrl thumbUrl = m_wallpaperManager->videoThumbnailUrl(fi.fileName());
        if (thumbUrl.isLocalFile() && QFileInfo::exists(thumbUrl.toLocalFile())) {
            buildStillLuminanceMap(thumbUrl.toLocalFile());
            return;
        }
    }
    // Ambient dark default for video wallpaper
    m_luminanceGrid.fill(QColor(18, 19, 28));
}

void ContrastManager::updateLuminanceMap()
{
    int mode = m_settings ? m_settings->wallpaperMode() : SettingsManager::StillImagesMode;

    if (mode == SettingsManager::StillImagesMode) {
        QString localPath;
        if (m_wallpaperManager && m_wallpaperManager->hasWallpaper()) {
            localPath = m_wallpaperManager->currentWallpaperUrl().toLocalFile();
        }
        buildStillLuminanceMap(localPath);
    } else if (mode == SettingsManager::GradientMode) {
        buildGradientLuminanceMap();
    } else if (mode == SettingsManager::VideoMode) {
        QString localPath;
        if (m_wallpaperManager && m_wallpaperManager->hasVideo()) {
            localPath = m_wallpaperManager->currentVideoUrl().toLocalFile();
        }
        buildVideoLuminanceMap(localPath);
    }

    qreal globalLum = relativeLuminanceForRect(0.0, 0.0, 1.0, 1.0);
    // Hysteresis threshold to prevent flickering
    if (m_globalDark) {
        if (globalLum > 0.50) {
            m_globalDark = false;
        }
    } else {
        if (globalLum < 0.42) {
            m_globalDark = true;
        }
    }

    emit colorsChanged();
}

QColor ContrastManager::sampleRawRegion(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    qreal x0 = qBound(0.0, normX, 1.0);
    qreal y0 = qBound(0.0, normY, 1.0);
    qreal w0 = qBound(0.01, normW, 1.0 - x0);
    qreal h0 = qBound(0.01, normH, 1.0 - y0);

    int startX = qBound(0, static_cast<int>(x0 * 64.0), 63);
    int endX = qBound(0, static_cast<int>((x0 + w0) * 64.0), 64);
    int startY = qBound(0, static_cast<int>(y0 * 36.0), 35);
    int endY = qBound(0, static_cast<int>((y0 + h0) * 36.0), 36);

    if (endX <= startX) endX = qMin(64, startX + 1);
    if (endY <= startY) endY = qMin(36, startY + 1);

    quint64 totalR = 0, totalG = 0, totalB = 0;
    int count = 0;

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            QColor c = m_luminanceGrid.pixelColor(x, y);
            totalR += c.red();
            totalG += c.green();
            totalB += c.blue();
            ++count;
        }
    }

    if (count == 0) return QColor(24, 25, 38);

    return QColor(totalR / count, totalG / count, totalB / count);
}

QColor ContrastManager::computeEffectiveColor(const QColor &rawSample, bool hasDarkeningPanel) const
{
    qreal dimming = m_settings ? m_settings->dimmingOpacity() : 0.0;
    dimming = qBound(0.0, dimming, 1.0);

    // Wallpaper dimmed by black:
    qreal r = rawSample.redF() * (1.0 - dimming);
    qreal g = rawSample.greenF() * (1.0 - dimming);
    qreal b = rawSample.blueF() * (1.0 - dimming);

    if (hasDarkeningPanel && m_settings && m_settings->darkeningEnabled()) {
        qreal darkOp = qBound(0.0, m_settings->darkeningOpacity(), 1.0);
        // Blended with dark panel tint #08090e:
        qreal dr = 8.0 / 255.0;
        qreal dg = 9.0 / 255.0;
        qreal db = 14.0 / 255.0;
        r = r * (1.0 - darkOp) + dr * darkOp;
        g = g * (1.0 - darkOp) + dg * darkOp;
        b = b * (1.0 - darkOp) + db * darkOp;
    }

    return QColor::fromRgbF(qBound(0.0, r, 1.0), qBound(0.0, g, 1.0), qBound(0.0, b, 1.0));
}

qreal ContrastManager::relativeLuminanceForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    QColor raw = sampleRawRegion(normX, normY, normW, normH);
    QColor eff = computeEffectiveColor(raw, false);
    return calculateRelativeLuminance(eff);
}

bool ContrastManager::isDarkForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    int mode = contrastMode();
    if (mode == SettingsManager::ContrastLight) return true;  // force light text
    if (mode == SettingsManager::ContrastDark) return false;  // force dark text

    QColor raw = sampleRawRegion(normX, normY, normW, normH);
    QColor eff = computeEffectiveColor(raw, false);
    qreal lum = calculateRelativeLuminance(eff);

    // Hysteresis check
    if (m_globalDark) {
        return lum <= 0.50;
    } else {
        return lum < 0.42;
    }
}

bool ContrastManager::isDarkBackground() const
{
    int mode = contrastMode();
    if (mode == SettingsManager::ContrastLight) return true;
    if (mode == SettingsManager::ContrastDark) return false;
    return m_globalDark;
}

QColor ContrastManager::primaryTextColor() const
{
    return isDarkBackground() ? QColor(QStringLiteral("#ffffff")) : QColor(QStringLiteral("#11111b"));
}

QColor ContrastManager::secondaryTextColor() const
{
    return isDarkBackground() ? QColor(QStringLiteral("#a6adc8")) : QColor(QStringLiteral("#45475a"));
}

QColor ContrastManager::iconColor() const
{
    return isDarkBackground() ? QColor(QStringLiteral("#cdd6f4")) : QColor(QStringLiteral("#181825"));
}

QColor ContrastManager::accentColor() const
{
    return isDarkBackground() ? QColor(QStringLiteral("#89b4fa")) : QColor(QStringLiteral("#1e66f5"));
}

QColor ContrastManager::panelBackgroundColor() const
{
    qreal op = m_settings ? m_settings->overlayOpacity() : 0.85;
    return isDarkBackground() ? QColor(20, 23, 36, static_cast<int>(255 * op))
                              : QColor(245, 245, 247, static_cast<int>(255 * op));
}

QColor ContrastManager::panelBorderColor() const
{
    return isDarkBackground() ? QColor(255, 255, 255, 30) : QColor(0, 0, 0, 38);
}

QColor ContrastManager::buttonBackgroundColor() const
{
    qreal op = m_settings ? m_settings->overlayOpacity() : 0.85;
    return isDarkBackground() ? QColor(20, 23, 36, static_cast<int>(255 * op))
                              : QColor(255, 255, 255, static_cast<int>(255 * qMin(1.0, op + 0.1)));
}

QColor ContrastManager::buttonBorderColor() const
{
    return isDarkBackground() ? QColor(255, 255, 255, 30) : QColor(0, 0, 0, 38);
}

QColor ContrastManager::buttonHoverColor() const
{
    return isDarkBackground() ? QColor(255, 255, 255, 46) : QColor(230, 230, 238, 255);
}

QColor ContrastManager::primaryTextForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    return isDarkForRect(normX, normY, normW, normH) ? QColor(QStringLiteral("#ffffff")) : QColor(QStringLiteral("#11111b"));
}

QColor ContrastManager::secondaryTextForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    return isDarkForRect(normX, normY, normW, normH) ? QColor(QStringLiteral("#a6adc8")) : QColor(QStringLiteral("#45475a"));
}

QColor ContrastManager::iconColorForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    return isDarkForRect(normX, normY, normW, normH) ? QColor(QStringLiteral("#cdd6f4")) : QColor(QStringLiteral("#181825"));
}

QColor ContrastManager::accentColorForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    return isDarkForRect(normX, normY, normW, normH) ? QColor(QStringLiteral("#89b4fa")) : QColor(QStringLiteral("#1e66f5"));
}

QColor ContrastManager::buttonBgForRect(qreal normX, qreal normY, qreal normW, qreal normH, bool hovered) const
{
    bool dark = isDarkForRect(normX, normY, normW, normH);
    qreal op = m_settings ? m_settings->overlayOpacity() : 0.85;
    if (dark) {
        return hovered ? QColor(255, 255, 255, 46) : QColor(20, 23, 36, static_cast<int>(255 * op));
    } else {
        return hovered ? QColor(230, 230, 238, 255) : QColor(255, 255, 255, static_cast<int>(255 * qMin(1.0, op + 0.1)));
    }
}

QColor ContrastManager::buttonBorderForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    bool dark = isDarkForRect(normX, normY, normW, normH);
    return dark ? QColor(255, 255, 255, 30) : QColor(0, 0, 0, 38);
}

QColor ContrastManager::panelBgForRect(qreal normX, qreal normY, qreal normW, qreal normH, qreal opacity) const
{
    bool dark = isDarkForRect(normX, normY, normW, normH);
    qreal op = (opacity >= 0.0) ? opacity : (m_settings ? m_settings->overlayOpacity() : 0.85);
    return dark ? QColor(20, 23, 36, static_cast<int>(255 * op)) : QColor(245, 245, 247, static_cast<int>(255 * op));
}

QColor ContrastManager::panelBorderForRect(qreal normX, qreal normY, qreal normW, qreal normH) const
{
    bool dark = isDarkForRect(normX, normY, normW, normH);
    return dark ? QColor(255, 255, 255, 30) : QColor(0, 0, 0, 38);
}
