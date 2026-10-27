#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QIcon>
#include <QTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QFileInfo>
#include <QDir>
#include "AppController.h"

int main(int argc, char *argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication app(argc, argv);

    QApplication::setApplicationName(QStringLiteral("music-focus"));
    QApplication::setApplicationDisplayName(QStringLiteral("Music Focus"));
    QApplication::setOrganizationName(QStringLiteral("KDE"));
    QApplication::setOrganizationDomain(QStringLiteral("kde.org"));

    QString desktopFileName = QStringLiteral("org.kde.musicfocus");
    const QString gioDesktop = qEnvironmentVariable("GIO_LAUNCHED_DESKTOP_FILE");
    if (!gioDesktop.isEmpty()) {
        QFileInfo fi(gioDesktop);
        desktopFileName = fi.completeBaseName();
    } else {
        const QString appImage = qEnvironmentVariable("APPIMAGE");
        if (!appImage.isEmpty()) {
            QFileInfo fi(appImage);
            QString candidate = QDir::homePath() + QStringLiteral("/.local/share/applications/") + fi.completeBaseName() + QStringLiteral(".desktop");
            if (QFile::exists(candidate)) {
                desktopFileName = fi.completeBaseName();
            }
        }
    }
    QApplication::setDesktopFileName(desktopFileName);

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Minimal Native KDE Study Music Player"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption screenshotOption(QStringLiteral("screenshot"),
        QStringLiteral("Capture rendered window to <file> and exit."),
        QStringLiteral("file"));
    parser.addOption(screenshotOption);

    QCommandLineOption openSettingsOption(QStringLiteral("open-settings"),
        QStringLiteral("Open settings dialog on launch."));
    parser.addOption(openSettingsOption);

    QCommandLineOption settingsTabOption(QStringLiteral("settings-tab"),
        QStringLiteral("Index of tab to open in settings (0=Library, 1=Appearance, 2=Playback, 3=Behavior)."),
        QStringLiteral("index"), QStringLiteral("0"));
    parser.addOption(settingsTabOption);

    QCommandLineOption scrollSettingsOption(QStringLiteral("scroll-settings"),
        QStringLiteral("Scroll position ratio in settings (0.0 to 1.0)."),
        QStringLiteral("ratio"), QStringLiteral("0.0"));
    parser.addOption(scrollSettingsOption);

    QCommandLineOption editLayoutOption(QStringLiteral("edit-layout"),
        QStringLiteral("Open in desktop layout grid editing mode."));
    parser.addOption(editLayoutOption);

    QCommandLineOption testRecoveryOption(QStringLiteral("test-recovery"),
        QStringLiteral("Simulate unavailable music directory for recovery screen."));
    parser.addOption(testRecoveryOption);

    parser.process(app);

    QString screenshotPath = parser.value(screenshotOption);

    // Ensure Breeze icon theme is used for dark ambient environment
    if (QIcon::themeName().isEmpty() || QIcon::themeName() == QStringLiteral("hicolor")) {
        QIcon::setThemeName(QStringLiteral("breeze-dark"));
    }

    // Set application icon from bundled multi-resolution assets
    QIcon appIcon;
    const QVector<int> iconSizes = {16, 24, 32, 48, 64, 128, 256, 512};
    for (int size : iconSizes) {
        QString path = QStringLiteral(":/icons/hicolor/%1x%1/apps/music-focus.png").arg(size);
        appIcon.addFile(path, QSize(size, size));
    }
    if (appIcon.isNull()) {
        appIcon = QIcon(QStringLiteral(":/icons/music-focus.png"));
    }
    if (!appIcon.isNull()) {
        QApplication::setWindowIcon(appIcon);
    }

    // Use Breeze style if available
    if (QQuickStyle::name().isEmpty()) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.breeze"));
    }

    AppController appController;

    QQmlApplicationEngine engine;

    // Expose controller and sub-services to QML
    engine.rootContext()->setContextProperty(QStringLiteral("appController"), &appController);
    engine.rootContext()->setContextProperty(QStringLiteral("settings"), appController.settings());
    engine.rootContext()->setContextProperty(QStringLiteral("trackModel"), appController.trackModel());
    engine.rootContext()->setContextProperty(QStringLiteral("audioPlayer"), appController.audioPlayer());
    engine.rootContext()->setContextProperty(QStringLiteral("playlistManager"), appController.playlistManager());
    engine.rootContext()->setContextProperty(QStringLiteral("wallpaperManager"), appController.wallpaperManager());
    engine.rootContext()->setContextProperty(QStringLiteral("libraryScanner"), appController.libraryScanner());
    engine.rootContext()->setContextProperty(QStringLiteral("todoManager"), appController.todoManager());
    engine.rootContext()->setContextProperty(QStringLiteral("layoutController"), appController.layoutController());
    engine.rootContext()->setContextProperty(QStringLiteral("contrastManager"), appController.contrastManager());
    engine.rootContext()->setContextProperty(QStringLiteral("pomodoroManager"), appController.pomodoroManager());

    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url, appIcon](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            } else if (obj && url == objUrl) {
                if (auto *window = qobject_cast<QQuickWindow*>(obj)) {
                    window->setIcon(appIcon);
                }
            }
        },
        Qt::QueuedConnection);

    engine.load(url);

    // Apply window icon directly to any root objects already loaded
    for (auto *obj : engine.rootObjects()) {
        if (auto *window = qobject_cast<QQuickWindow*>(obj)) {
            window->setIcon(appIcon);
        }
    }

    // Run initial scan / directory check after QML is loaded
    QTimer::singleShot(50, &appController, &AppController::initialize);

    if (parser.isSet(openSettingsOption)) {
        int tabIdx = parser.value(settingsTabOption).toInt();
        QTimer::singleShot(250, [&engine, tabIdx]() {
            const auto rootObjects = engine.rootObjects();
            if (!rootObjects.isEmpty()) {
                QMetaObject::invokeMethod(rootObjects.first(), "openSettings", Q_ARG(QVariant, tabIdx));
            }
        });
    }

    if (parser.isSet(scrollSettingsOption)) {
        double ratio = parser.value(scrollSettingsOption).toDouble();
        QTimer::singleShot(600, [&engine, ratio]() {
            const auto rootObjects = engine.rootObjects();
            if (!rootObjects.isEmpty()) {
                QMetaObject::invokeMethod(rootObjects.first(), "scrollSettings", Q_ARG(QVariant, ratio));
            }
        });
    }

    if (parser.isSet(editLayoutOption)) {
        QTimer::singleShot(200, [&appController]() {
            if (appController.layoutController()) {
                appController.layoutController()->setEditing(true);
            }
        });
    }

    if (parser.isSet(testRecoveryOption)) {
        appController.settings()->setOnboardingCompleted(true);
        appController.settings()->setMusicDirectory(QStringLiteral("/media/usb_unmounted_missing_directory"));
    }

    if (!screenshotPath.isEmpty()) {
        QTimer::singleShot(1200, [&engine, screenshotPath, &app]() {
            const auto rootObjects = engine.rootObjects();
            if (!rootObjects.isEmpty()) {
                if (auto *window = qobject_cast<QQuickWindow*>(rootObjects.first())) {
                    QImage img = window->grabWindow();
                    img.save(screenshotPath);
                }
            }
            app.quit();
        });
    }

    return app.exec();
}
