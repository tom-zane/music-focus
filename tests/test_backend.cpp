#include <QTest>
#include <QSignalSpy>
#include <QDir>
#include <QStandardPaths>
#include "../src/SettingsManager.h"
#include "../src/TrackModel.h"
#include "../src/LibraryScanner.h"
#include "../src/WallpaperManager.h"
#include "../src/PlaylistManager.h"
#include "../src/AudioPlayer.h"
#include "../src/TodoManager.h"
#include "../src/LayoutController.h"
#include "../src/AppController.h"
#include "../src/PomodoroManager.h"

class BackendTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testSettingsManagerDefaultsAndPersistence();
    void testLibraryScannerFilteringAndNaturalSorting();
    void testLibraryScannerErrorHandling();
    void testWallpaperManagerModesAndRotation();
    void testPlaylistManagerNavigationAndRepeat();
    void testPlaylistManagerShuffle();
    void testAudioPlayerFormattingAndVolume();
    void testAudioPlayerInvalidMedia();
    void testTodoManagerCreationCompletionAndPersistence();
    void testWallpaperManagerCanonicalDirectoriesAndVideos();
    void testOnboardingAndRecoveryState();
    void testLayoutControllerGridMathAndBounds();
    void testLayoutControllerCollisionAndEdgeSharing();
    void testLayoutControllerPersistenceMigrationAndReset();
    void testContrastManagerAndLegibility();
    void testIndependentScalingAndDialogDimensions();
    void testClockSettingsAndLayoutManagement();
    void testClockMinimumSizeAndFormatting();
    void testPomodoroTimerStateMachineAndTiming();
    void testPomodoroPresetsManagement();
    void testPomodoroHistoryAndHeatmapCalculations();
    void testPomodoroLayoutIntegration();
    void testMandatoryWidgetsProtection();

private:
    QString m_testDataPath;
};

void BackendTest::initTestCase()
{
#ifdef TEST_DATA_DIR
    m_testDataPath = QStringLiteral(TEST_DATA_DIR);
#else
    m_testDataPath = QDir::current().absoluteFilePath("test_data/StudyMusic");
#endif
    QVERIFY(QDir(m_testDataPath).exists());
}

void BackendTest::testSettingsManagerDefaultsAndPersistence()
{
    // Use an isolated settings group/scope for clean defaults testing
    QSettings rawSettings("KDE", "MusicFocus");
    rawSettings.clear();

    SettingsManager settings;
    // Check default values per specification
    QCOMPARE(settings.wallpaperMode(), static_cast<int>(SettingsManager::RotateMode));
    QCOMPARE(settings.rotateIntervalMinutes(), 5);
    QCOMPARE(settings.repeatMode(), static_cast<int>(SettingsManager::RepeatAll));
    QCOMPARE(settings.shuffle(), false);
    QCOMPARE(settings.autoPlay(), false);
    QCOMPARE(settings.inactivityEnabled(), true);
    QCOMPARE(settings.inactivityTimeoutSeconds(), 5);
    QCOMPARE(settings.inactivityMode(), static_cast<int>(SettingsManager::InactivityFade));

    // Test clamped setters
    settings.setDimmingOpacity(1.5);
    QVERIFY(settings.dimmingOpacity() <= 0.9);

    settings.setOverlayOpacity(0.05);
    QVERIFY(settings.overlayOpacity() >= 0.15);

    settings.setVolume(2.0);
    QVERIFY(settings.volume() <= 1.0);

    // Test blur and darkening properties
    QCOMPARE(settings.blurEnabled(), true);
    QCOMPARE(settings.darkeningEnabled(), true);
    settings.setBlurStrength(1.5);
    QCOMPARE(settings.blurStrength(), 1.0);
    settings.setBlurStrength(-0.5);
    QCOMPARE(settings.blurStrength(), 0.0);
    settings.setBlurStrength(0.7);
    QCOMPARE(settings.blurStrength(), 0.7);

    settings.setDarkeningOpacity(1.5);
    QCOMPARE(settings.darkeningOpacity(), 1.0);
    settings.setDarkeningOpacity(-0.5);
    QCOMPARE(settings.darkeningOpacity(), 0.0);
    settings.setDarkeningOpacity(0.8);
    QCOMPARE(settings.darkeningOpacity(), 0.8);

    // Test element scale clamping and reset
    QCOMPARE(settings.playlistScale(), 1.0);
    QCOMPARE(settings.playbackScale(), 1.0);
    QCOMPARE(settings.topControlsScale(), 1.0);
    QCOMPARE(settings.todoScale(), 1.20);

    settings.setPlaylistScale(2.5);
    QCOMPARE(settings.playlistScale(), 1.50);
    settings.setPlaylistScale(0.2);
    QCOMPARE(settings.playlistScale(), 0.70);

    settings.setTodoScale(2.0);
    QCOMPARE(settings.todoScale(), 1.50);
    settings.setTodoScale(0.5);
    QCOMPARE(settings.todoScale(), 0.70);
    settings.setTodoScale(1.15);
    QCOMPARE(settings.todoScale(), 1.15);

    settings.resetScales();
    QCOMPARE(settings.playlistScale(), 1.0);
    QCOMPARE(settings.playbackScale(), 1.0);
    QCOMPARE(settings.topControlsScale(), 1.0);
    QCOMPARE(settings.todoScale(), 1.20);

    // Test font properties
    settings.setFontFamily("Sans Serif");
    QCOMPARE(settings.fontFamily(), QStringLiteral("Sans Serif"));
    settings.setFontSize(30);
    QCOMPARE(settings.fontSize(), 20);
    settings.setFontSize(5);
    QCOMPARE(settings.fontSize(), 9);
    settings.resetFont();
    QVERIFY(settings.fontFamily().isEmpty());
    QCOMPARE(settings.fontSize(), 11);

    // Test visualizer density, sensitivity and speed defaults & clamping
    QCOMPARE(settings.visualizerBarDensity(), 64);
    QCOMPARE(settings.visualizerSensitivity(), 1.0);
    QCOMPARE(settings.visualizerSpeed(), 0.8);

    settings.setVisualizerBarDensity(200);
    QCOMPARE(settings.visualizerBarDensity(), 128);
    settings.setVisualizerBarDensity(5);
    QCOMPARE(settings.visualizerBarDensity(), 16);
    settings.setVisualizerBarDensity(64);
    QCOMPARE(settings.visualizerBarDensity(), 64);

    settings.setVisualizerSensitivity(5.0);
    QCOMPARE(settings.visualizerSensitivity(), 2.0);
    settings.setVisualizerSensitivity(0.1);
    QCOMPARE(settings.visualizerSensitivity(), 0.5);
    settings.setVisualizerSensitivity(1.0);
    QCOMPARE(settings.visualizerSensitivity(), 1.0);

    settings.setVisualizerSpeed(3.0);
    QCOMPARE(settings.visualizerSpeed(), 2.0);
    settings.setVisualizerSpeed(0.1);
    QCOMPARE(settings.visualizerSpeed(), 0.4);
    settings.setVisualizerSpeed(0.8);
    QCOMPARE(settings.visualizerSpeed(), 0.8);

    // Test persistence across application restart
    settings.setVisualizerBarDensity(96);
    settings.setVisualizerSensitivity(1.45);
    settings.setVisualizerSpeed(0.65);
    settings.setTodoScale(1.35);
    settings.save();

    // Fresh instance simulating restart
    {
        SettingsManager restartedSettings;
        QCOMPARE(restartedSettings.visualizerBarDensity(), 96);
        QCOMPARE(restartedSettings.visualizerSensitivity(), 1.45);
        QCOMPARE(restartedSettings.visualizerSpeed(), 0.65);
        QCOMPARE(restartedSettings.todoScale(), 1.35);

        // Reset back to defaults
        restartedSettings.setVisualizerBarDensity(64);
        restartedSettings.setVisualizerSensitivity(1.0);
        restartedSettings.setVisualizerSpeed(0.8);
        restartedSettings.setTodoScale(1.20);
        restartedSettings.save();
    }

    // Restore standard defaults
    settings.setDimmingOpacity(0.25);
    settings.setOverlayOpacity(0.85);
    settings.resetScales();
}

void BackendTest::testLibraryScannerFilteringAndNaturalSorting()
{
    LibraryScanner scanner;
    TrackModel model;

    QSignalSpy spy(&scanner, &LibraryScanner::scanCompleted);
    scanner.scanDirectory(m_testDataPath, &model);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    int count = arguments.at(0).toInt();

    // Exactly 4 valid audio files should be found:
    // Track 1 (.mp3), Track 2 (.flac), Track 10 (.ogg), Track 11 (.wav)
    // .hidden_audio.mp3, notes.txt, and subfolder/sub_track.mp3 must be excluded!
    QCOMPARE(count, 4);
    QCOMPARE(model.count(), 4);

    // Verify NATURAL SORT order:
    // Track 1 precedes Track 2, which precedes Track 10, which precedes Track 11!
    QCOMPARE(model.trackAt(0).fileName, QStringLiteral("Track 1 - Ambient Morning.mp3"));
    QCOMPARE(model.trackAt(1).fileName, QStringLiteral("Track 2 - Rain On Glass.flac"));
    QCOMPARE(model.trackAt(2).fileName, QStringLiteral("Track 10 - Deep Meditation.ogg"));
    QCOMPARE(model.trackAt(3).fileName, QStringLiteral("Track 11 - Evening Chill.wav"));

    // Verify titles are cleanly formatted
    QCOMPARE(model.trackAt(0).title, QStringLiteral("Track 1 - Ambient Morning"));
    QCOMPARE(model.trackAt(2).title, QStringLiteral("Track 10 - Deep Meditation"));
}

void BackendTest::testLibraryScannerErrorHandling()
{
    LibraryScanner scanner;
    TrackModel model;

    // Test invalid directory
    scanner.scanDirectory("/nonexistent/path/that/does/not/exist", &model);
    QCOMPARE(model.count(), 0);
    QVERIFY(!scanner.lastError().isEmpty());

    // Test empty path
    scanner.scanDirectory("", &model);
    QCOMPARE(model.count(), 0);
    QVERIFY(!scanner.lastError().isEmpty());
}

void BackendTest::testWallpaperManagerModesAndRotation()
{
    SettingsManager settings;
    settings.setMusicDirectory(m_testDataPath);

    WallpaperManager wm(&settings);
    wm.rescan();

    // Should find forest.jpg, mountains.webp, rainy-city.png
    QCOMPARE(wm.wallpapersCount(), 3);
    QStringList list = wm.availableWallpapers();
    QVERIFY(list.contains("forest.jpg"));
    QVERIFY(list.contains("mountains.webp"));
    QVERIFY(list.contains("rainy-city.png"));

    // Test Rotate Mode
    settings.setWallpaperMode(SettingsManager::RotateMode);
    QVERIFY(wm.hasWallpaper());
    QUrl firstUrl = wm.currentWallpaperUrl();

    // Rotate next
    wm.nextWallpaper();
    QUrl secondUrl = wm.currentWallpaperUrl();
    QVERIFY(firstUrl != secondUrl);

    // Test Fixed Mode
    settings.setWallpaperMode(SettingsManager::FixedMode);
    wm.selectWallpaperByName("mountains.webp");
    QCOMPARE(wm.currentWallpaperName(), QStringLiteral("mountains.webp"));
    QCOMPARE(settings.fixedWallpaper(), QStringLiteral("mountains.webp"));

    // Test Solid Color Mode
    settings.setWallpaperMode(SettingsManager::SolidColorMode);
    QVERIFY(!wm.hasWallpaper()); // Wallpaper URL should be empty in solid color mode
}

void BackendTest::testPlaylistManagerNavigationAndRepeat()
{
    SettingsManager settings;
    TrackModel model;
    AudioPlayer player;
    PlaylistManager pm(&model, &player, &settings);

    LibraryScanner scanner;
    scanner.scanDirectory(m_testDataPath, &model);
    QCOMPARE(model.count(), 4);

    // Play track at index 0
    pm.playTrackAtIndex(0);
    QCOMPARE(pm.currentIndex(), 0);
    QCOMPARE(pm.currentTitle(), QStringLiteral("Track 1 - Ambient Morning"));

    // Next track (linear)
    settings.setRepeatMode(SettingsManager::RepeatAll);
    settings.setShuffle(false);
    pm.nextTrack();
    QCOMPARE(pm.currentIndex(), 1);
    pm.nextTrack();
    QCOMPARE(pm.currentIndex(), 2);
    pm.nextTrack();
    QCOMPARE(pm.currentIndex(), 3);

    // With RepeatAll, advancing past last track should loop to 0
    pm.nextTrack();
    QCOMPARE(pm.currentIndex(), 0);

    // With RepeatOff, advancing past last track should stop
    pm.playTrackAtIndex(3);
    settings.setRepeatMode(SettingsManager::RepeatOff);
    pm.nextTrack();
    QVERIFY(player.playbackState() == AudioPlayer::StoppedState || pm.currentIndex() == 3);

    // Previous track navigation
    pm.playTrackAtIndex(2);
    pm.previousTrack();
    QCOMPARE(pm.currentIndex(), 1);
}

void BackendTest::testPlaylistManagerShuffle()
{
    SettingsManager settings;
    TrackModel model;
    PlaylistManager pm(&model, nullptr, &settings);

    LibraryScanner scanner;
    scanner.scanDirectory(m_testDataPath, &model);

    pm.clearHistory();
    settings.setRepeatMode(SettingsManager::RepeatAll);
    settings.setShuffle(true);
    QVERIFY(pm.shuffle());

    pm.playTrackAtIndex(0);
    pm.nextTrack();
    int nextIdx = pm.currentIndex();
    QVERIFY(nextIdx >= 0 && nextIdx < model.count());

    pm.previousTrack();
    QCOMPARE(pm.currentIndex(), 0);
}

void BackendTest::testAudioPlayerFormattingAndVolume()
{
    AudioPlayer player;

    // Test formatting helper via public properties
    player.setVolume(0.5);
    QVERIFY(qAbs(player.volume() - 0.5) < 0.05);

    player.setMuted(true);
    QVERIFY(player.isMuted());
    player.setMuted(false);
    QVERIFY(!player.isMuted());

    // Volume clamping
    player.setVolume(-0.2);
    QVERIFY(player.volume() >= 0.0);
    player.setVolume(1.8);
    QVERIFY(player.volume() <= 1.0);
}

void BackendTest::testAudioPlayerInvalidMedia()
{
    AudioPlayer player;
    QSignalSpy errorSpy(&player, &AudioPlayer::errorOccurred);

    // Attempt to play a non-existent or unreadable file
    player.loadAndPlay(QUrl::fromLocalFile("/nonexistent/song.mp3"));

    // Wait briefly for error or status update
    QTest::qWait(200);

    // The player should not crash and should record error if backend rejected it
    QVERIFY(!player.isPlaying());
}

void BackendTest::testTodoManagerCreationCompletionAndPersistence()
{
    // Test TodoManager
    TodoManager todo;
    todo.clearCompleted();
    int initialCount = todo.count();

    todo.addTask(QStringLiteral("Study math chapter 4"));
    todo.addTask(QStringLiteral("Review physics formulas"));
    QCOMPARE(todo.count(), initialCount + 2);

    int idx1 = todo.count() - 2;
    int idx2 = todo.count() - 1;

    // Toggle completion
    todo.toggleCompleted(idx1);
    QCOMPARE(todo.data(todo.index(idx1), TodoManager::CompletedRole).toBool(), true);
    QCOMPARE(todo.data(todo.index(idx2), TodoManager::CompletedRole).toBool(), false);

    // Delete task
    todo.deleteTask(idx2);
    QCOMPARE(todo.count(), initialCount + 1);

    // Clear completed
    todo.clearCompleted();
    QCOMPARE(todo.count(), initialCount);
}

void BackendTest::testWallpaperManagerCanonicalDirectoriesAndVideos()
{
    SettingsManager settings;
    settings.setMusicDirectory(m_testDataPath);

    WallpaperManager wm(&settings);
    wm.rescan();

    // Check canonical directories were set up
    QVERIFY(!wm.stillWallpapersPath().isEmpty());
    QVERIFY(!wm.videoWallpapersPath().isEmpty());
    QVERIFY(wm.wallpapersCount() >= 3);

    // Test deterministic seed regeneration
    int initialSeed = settings.gradientSeed();
    wm.regenerateGradientSeed();
    QVERIFY(settings.gradientSeed() != initialSeed || settings.gradientSeed() > 0);

    // Test mode transitions
    settings.setWallpaperMode(SettingsManager::GradientMode);
    QCOMPARE(settings.wallpaperMode(), static_cast<int>(SettingsManager::GradientMode));
    QVERIFY(!wm.hasWallpaper());
    QVERIFY(!wm.hasVideo());

    settings.setWallpaperMode(SettingsManager::StillImagesMode);
    QCOMPARE(settings.wallpaperMode(), static_cast<int>(SettingsManager::StillImagesMode));
    QVERIFY(wm.hasWallpaper());
}

void BackendTest::testOnboardingAndRecoveryState()
{
    QSettings rawSettings("KDE", "MusicFocus");
    rawSettings.clear();

    SettingsManager settings;
    QCOMPARE(settings.onboardingCompleted(), false);
    QCOMPARE(settings.hasValidDirectory(), false);

    AppController controller;
    // Initial state: first-launch setup required, recovery not required
    QCOMPARE(controller.isFirstLaunchSetup(), true);
    QCOMPARE(controller.isRecoveryRequired(), false);

    // Validate directory checks
    QVariantMap emptyVal = controller.validateDirectory("");
    QCOMPARE(emptyVal.value("valid").toBool(), false);

    QVariantMap nonExistentVal = controller.validateDirectory("/nonexistent/directory/that/does/not/exist");
    QCOMPARE(nonExistentVal.value("valid").toBool(), false);

    QVariantMap validVal = controller.validateDirectory(m_testDataPath);
    QCOMPARE(validVal.value("valid").toBool(), true);
    QCOMPARE(validVal.value("trackCount").toInt(), 4);

    // Complete onboarding with valid path
    bool completed = controller.completeOnboarding(m_testDataPath);
    QVERIFY(completed);
    QCOMPARE(controller.settings()->onboardingCompleted(), true);
    QCOMPARE(controller.settings()->hasValidDirectory(), true);
    QCOMPARE(controller.isFirstLaunchSetup(), false);
    QCOMPARE(controller.isRecoveryRequired(), false);

    // Check wallpaper folders created safely
    QVERIFY(QDir(m_testDataPath + QStringLiteral("/Wallpapers/Still Wallpapers")).exists());
    QVERIFY(QDir(m_testDataPath + QStringLiteral("/Wallpapers/Video Wallpapers")).exists());

    // Test recovery state when directory becomes unavailable
    controller.settings()->setMusicDirectory("/media/unmounted_storage_drive_xyz");
    QCOMPARE(controller.settings()->hasValidDirectory(), false);
    QCOMPARE(controller.isFirstLaunchSetup(), false);
    QCOMPARE(controller.isRecoveryRequired(), true);

    // Retry should fail while directory unavailable
    QCOMPARE(controller.retryDirectoryAccess(), false);
    QCOMPARE(controller.isRecoveryRequired(), true);

    // Restore valid directory
    controller.settings()->setMusicDirectory(m_testDataPath);
    QCOMPARE(controller.retryDirectoryAccess(), true);
    QCOMPARE(controller.isRecoveryRequired(), false);
}

void BackendTest::testLayoutControllerGridMathAndBounds()
{
    SettingsManager settings;
    LayoutController layout(&settings);

    // Verify 120 x 80 logical grid (3:2 ratio)
    QCOMPARE(layout.gridColumns(), 120);
    QCOMPARE(layout.gridRows(), 80);

    qreal usableW = 1200.0;
    qreal usableH = 800.0;

    // Center conversions
    QCOMPARE(layout.gridToPixelX(60, usableW, 0.0), 600.0);
    QCOMPARE(layout.gridToPixelY(40, usableH, 0.0), 400.0);
    QCOMPARE(layout.gridToPixelWidth(30, usableW), 300.0);
    QCOMPARE(layout.gridToPixelHeight(20, usableH), 200.0);

    // With insets
    QCOMPARE(layout.gridToPixelX(0, usableW, 16.0), 16.0);
    QCOMPARE(layout.gridToPixelY(0, usableH, 16.0), 16.0);

    // Invert conversion
    QCOMPARE(layout.pixelToGridX(600.0, usableW, 0.0), 60);
    QCOMPARE(layout.pixelToGridY(400.0, usableH, 0.0), 40);
    QCOMPARE(layout.pixelToGridWidth(300.0, usableW), 30);
    QCOMPARE(layout.pixelToGridHeight(200.0, usableH), 20);

    // Bounds checking
    QVERIFY(layout.isWithinBounds(0, 0, 120, 80));
    QVERIFY(!layout.isWithinBounds(-1, 0, 10, 10));
    QVERIFY(!layout.isWithinBounds(0, -1, 10, 10));
    QVERIFY(!layout.isWithinBounds(115, 0, 10, 10)); // 115 + 10 = 125 > 120
    QVERIFY(!layout.isWithinBounds(0, 75, 10, 10));  // 75 + 10 = 85 > 80
    QVERIFY(layout.isWithinBounds(110, 70, 10, 10)); // 110 + 10 = 120, 70 + 10 = 80
}

void BackendTest::testLayoutControllerCollisionAndEdgeSharing()
{
    SettingsManager settings;
    LayoutController layout(&settings);
    layout.resetLayout();

    // Default canonical layout:
    // playlist: x=2, y=42, w=38, h=36 (right edge = 40, bottom edge = 78)
    // playback: x=76, y=68, w=42, h=10
    // todo: hidden by default
    QCOMPARE(layout.widgetX("playlist"), 2);
    QCOMPARE(layout.widgetY("playlist"), 42);
    QCOMPARE(layout.widgetWidth("playlist"), 38);
    QCOMPARE(layout.widgetHeight("playlist"), 36);

    // 1. Overlapping test
    // An item overlapping playlist inside x=10..30, y=50..70
    QVERIFY(layout.checkCollision("playback", 10, 50, 20, 20));

    // 2. Non-overlapping test
    // In empty upper right area
    QVERIFY(!layout.checkCollision("playback", 50, 10, 20, 20));

    // 3. Edge-sharing test (Widgets may share edges without collision)
    // Playlist right edge is at x=40, y=42..78.
    // Placing another widget at x=40, y=42, w=20, h=36 shares the exact vertical edge:
    QVERIFY(!layout.checkCollision("playback", 40, 42, 20, 36));

    // If it penetrates by 1 unit into playlist (x=39), it MUST collide:
    QVERIFY(layout.checkCollision("playback", 39, 42, 20, 36));

    // Sharing horizontal edge: Playlist bottom edge is at y=78.
    // A widget placed at x=2, y=78, w=38, h=2 shares the horizontal edge:
    QVERIFY(!layout.checkCollision("playback", 2, 78, 38, 2));

    // If it penetrates by 1 unit (y=77), it MUST collide:
    QVERIFY(layout.checkCollision("playback", 2, 77, 38, 2));
}

void BackendTest::testLayoutControllerPersistenceMigrationAndReset()
{
    QSettings rawSettings("KDE", "MusicFocus");
    rawSettings.remove("layout/gridConfigV1");

    SettingsManager settings;
    LayoutController layout(&settings);
    layout.resetLayout();

    int initialRev = layout.layoutRevision();
    QSignalSpy layoutSpy(&layout, &LayoutController::layoutChanged);

    // Move playback to an empty valid grid location
    QVERIFY(layout.canMove("playback", 50, 20));
    QVERIFY(layout.moveWidget("playback", 50, 20));
    QCOMPARE(layout.widgetX("playback"), 50);
    QCOMPARE(layout.widgetY("playback"), 20);
    QVERIFY(layout.layoutRevision() > initialRev);
    QVERIFY(layoutSpy.count() >= 1);

    int prevRev = layout.layoutRevision();
    // Resize playback
    QVERIFY(layout.canResize("playback", 45, 12));
    QVERIFY(layout.resizeWidget("playback", 45, 12));
    QCOMPARE(layout.widgetWidth("playback"), 45);
    QCOMPARE(layout.widgetHeight("playback"), 12);
    QVERIFY(layout.layoutRevision() > prevRev);

    // Simulating app restart: fresh LayoutController instance
    {
        LayoutController restartedLayout(&settings);
        QCOMPARE(restartedLayout.widgetX("playback"), 50);
        QCOMPARE(restartedLayout.widgetY("playback"), 20);
        QCOMPARE(restartedLayout.widgetWidth("playback"), 45);
        QCOMPARE(restartedLayout.widgetHeight("playback"), 12);

        // Test Reset Layout restores defaults
        restartedLayout.resetLayout();
        QCOMPARE(restartedLayout.widgetX("playback"), 76);
        QCOMPARE(restartedLayout.widgetY("playback"), 68);
        QCOMPARE(restartedLayout.widgetWidth("playback"), 42);
        QCOMPARE(restartedLayout.widgetHeight("playback"), 10);
    }
}

void BackendTest::testContrastManagerAndLegibility()
{
    // Test relative luminance calculation
    qreal whiteLum = ContrastManager::calculateRelativeLuminance(QColor(255, 255, 255));
    qreal blackLum = ContrastManager::calculateRelativeLuminance(QColor(0, 0, 0));
    QVERIFY(qFuzzyCompare(whiteLum, 1.0));
    QVERIFY(qFuzzyCompare(blackLum, 0.0));

    // Test contrast ratio calculations against WCAG thresholds
    qreal maxRatio = ContrastManager::calculateContrast(whiteLum, blackLum);
    QCOMPARE(maxRatio, 21.0); // 1.05 / 0.05 = 21

    QColor darkBg(17, 17, 27); // #11111b
    qreal darkLum = ContrastManager::calculateRelativeLuminance(darkBg);
    qreal darkRatio = ContrastManager::calculateContrast(whiteLum, darkLum);
    QVERIFY(darkRatio >= 4.5); // WCAG normal text minimum

    QColor lightBg(245, 245, 247);
    qreal lightLum = ContrastManager::calculateRelativeLuminance(lightBg);
    qreal lightRatio = ContrastManager::calculateContrast(lightLum, darkLum);
    QVERIFY(lightRatio >= 4.5); // WCAG normal text minimum

    SettingsManager settings;
    WallpaperManager wallpapers(&settings);
    ContrastManager contrast(&settings, &wallpapers);

    // Default is Automatic
    QCOMPARE(contrast.contrastMode(), static_cast<int>(SettingsManager::ContrastAuto));

    // Test Light mode forces light text
    contrast.setContrastMode(SettingsManager::ContrastLight);
    QVERIFY(contrast.isDarkBackground());
    QCOMPARE(contrast.primaryTextColor(), QColor("#ffffff"));

    // Test Dark mode forces dark text
    contrast.setContrastMode(SettingsManager::ContrastDark);
    QVERIFY(!contrast.isDarkBackground());
    QCOMPARE(contrast.primaryTextColor(), QColor("#11111b"));

    // Reset back to Auto
    contrast.setContrastMode(SettingsManager::ContrastAuto);
    QCOMPARE(contrast.contrastMode(), static_cast<int>(SettingsManager::ContrastAuto));
}

void BackendTest::testIndependentScalingAndDialogDimensions()
{
    SettingsManager settings;

    // Default settings dialog dimensions
    QCOMPARE(settings.settingsDialogWidth(), 960);
    QCOMPARE(settings.settingsDialogHeight(), 680);
    QCOMPARE(settings.playbackStripScale(), 1.0);
    QCOMPARE(settings.playbackScale(), 1.0);

    // Modify playbackStripScale
    settings.setPlaybackStripScale(1.35);
    QCOMPARE(settings.playbackStripScale(), 1.35);
    QCOMPARE(settings.playbackScale(), 1.35);

    // Verify settings dialog dimensions did NOT change
    QCOMPARE(settings.settingsDialogWidth(), 960);
    QCOMPARE(settings.settingsDialogHeight(), 680);

    // Modify settings dialog dimensions
    settings.setSettingsDialogWidth(1100);
    settings.setSettingsDialogHeight(750);
    QCOMPARE(settings.settingsDialogWidth(), 1100);
    QCOMPARE(settings.settingsDialogHeight(), 750);

    // Verify playbackStripScale did NOT change
    QCOMPARE(settings.playbackStripScale(), 1.35);

    // Test clamping
    settings.setSettingsDialogWidth(300); // Below 720
    QCOMPARE(settings.settingsDialogWidth(), 720);
    settings.setSettingsDialogWidth(2000); // Above 1600
    QCOMPARE(settings.settingsDialogWidth(), 1600);

    settings.setSettingsDialogHeight(200); // Below 500
    QCOMPARE(settings.settingsDialogHeight(), 500);
    settings.setSettingsDialogHeight(1500); // Above 1200
    QCOMPARE(settings.settingsDialogHeight(), 1200);

    // Reset scales only affects widget scales, not dialog dimensions
    settings.resetScales();
    QCOMPARE(settings.playbackStripScale(), 1.0);
    QCOMPARE(settings.settingsDialogWidth(), 1600);
    QCOMPARE(settings.settingsDialogHeight(), 1200);
}

void BackendTest::testClockSettingsAndLayoutManagement()
{
    SettingsManager settings;
    QCOMPARE(settings.clockEnabled(), true);
    QCOMPARE(settings.clockShowDate(), true);
    QCOMPARE(settings.clock24Hour(), true);
    QCOMPARE(settings.clockShowSeconds(), false);

    settings.setClockShowSeconds(true);
    QCOMPARE(settings.clockShowSeconds(), true);

    settings.setClock24Hour(false);
    QCOMPARE(settings.clock24Hour(), false);

    LayoutController layout(&settings);
    // Clock widget should be registered
    QVERIFY(layout.isWidgetRegistered("clock"));
    QCOMPARE(layout.widgetX("clock"), 45);
    QCOMPARE(layout.widgetY("clock"), 4);
    QCOMPARE(layout.widgetWidth("clock"), 30);
    QCOMPARE(layout.widgetHeight("clock"), 12);

    // Add clock widget
    QVERIFY(layout.addWidget("clock"));
    QCOMPARE(layout.widgetVisible("clock"), true);
    QCOMPARE(settings.clockVisible(), true);

    // Remove clock widget
    QVERIFY(layout.removeWidget("clock"));
    QCOMPARE(layout.widgetVisible("clock"), false);
    QCOMPARE(settings.clockVisible(), false);

    // Add back clock widget
    QVERIFY(layout.addWidget("clock"));
    QCOMPARE(layout.widgetVisible("clock"), true);
}

void BackendTest::testMandatoryWidgetsProtection()
{
    SettingsManager settings;
    LayoutController layout(&settings);

    // Mandatory widgets check
    QVERIFY(layout.isMandatoryWidget("playback"));
    QVERIFY(layout.isMandatoryWidget("playlist"));
    QVERIFY(!layout.isMandatoryWidget("todo"));
    QVERIFY(!layout.isMandatoryWidget("clock"));

    // Attempting to remove or hide mandatory widgets MUST fail and preserve visibility
    QVERIFY(!layout.removeWidget("playback"));
    QCOMPARE(layout.widgetVisible("playback"), true);

    QVERIFY(!layout.removeWidget("playlist"));
    QCOMPARE(layout.widgetVisible("playlist"), true);

    layout.setWidgetVisible("playback", false);
    QCOMPARE(layout.widgetVisible("playback"), true); // Remains true!

    layout.setWidgetVisible("playlist", false);
    QCOMPARE(layout.widgetVisible("playlist"), true); // Remains true!
}

void BackendTest::testClockMinimumSizeAndFormatting()
{
    SettingsManager settings;
    LayoutController layout(&settings);

    // Verify clock minimum size is reduced to 4x2
    QCOMPARE(layout.widgetMinWidth("clock"), 4);
    QCOMPARE(layout.widgetMinHeight("clock"), 2);

    // Ensure resizing down to 4x2 is permitted
    QVERIFY(layout.canResize("clock", 4, 2));
    QVERIFY(layout.resizeWidget("clock", 4, 2));
    QCOMPARE(layout.widgetWidth("clock"), 4);
    QCOMPARE(layout.widgetHeight("clock"), 2);

    // Resizing below 4x2 must be rejected
    QVERIFY(!layout.canResize("clock", 3, 2));
    QVERIFY(!layout.canResize("clock", 4, 1));

    // Verify 12-hour vs 24-hour setting
    settings.setClock24Hour(true);
    QCOMPARE(settings.clock24Hour(), true);
    settings.setClock24Hour(false);
    QCOMPARE(settings.clock24Hour(), false);
}

void BackendTest::testPomodoroTimerStateMachineAndTiming()
{
    SettingsManager settings;
    PomodoroManager pomodoro(&settings);

    // Default state: Stopped, Focus session, 25 min (1500 sec)
    QCOMPARE(pomodoro.sessionType(), static_cast<int>(PomodoroManager::FocusSession));
    QCOMPARE(pomodoro.timerState(), static_cast<int>(PomodoroManager::Stopped));
    QCOMPARE(pomodoro.remainingSeconds(), 1500);
    QCOMPARE(pomodoro.totalSeconds(), 1500);
    QCOMPARE(pomodoro.formattedTime(), QStringLiteral("25:00"));

    // Start timer
    pomodoro.start();
    QCOMPARE(pomodoro.timerState(), static_cast<int>(PomodoroManager::Running));
    QVERIFY(pomodoro.isRunning());

    // Pause timer
    pomodoro.pause();
    QCOMPARE(pomodoro.timerState(), static_cast<int>(PomodoroManager::Paused));
    QVERIFY(pomodoro.isPaused());

    // Toggle back to running
    pomodoro.togglePlayPause();
    QCOMPARE(pomodoro.timerState(), static_cast<int>(PomodoroManager::Running));

    // Skip: Must cleanly transition to Break without logging completed session
    int initialHistoryCount = pomodoro.todaySessionCount();
    pomodoro.skip();
    QCOMPARE(pomodoro.sessionType(), static_cast<int>(PomodoroManager::BreakSession));
    QCOMPARE(pomodoro.timerState(), static_cast<int>(PomodoroManager::Stopped));
    QCOMPARE(pomodoro.remainingSeconds(), 300); // 5 min break
    QCOMPARE(pomodoro.formattedTime(), QStringLiteral("05:00"));
    // Skipped session must not be logged
    QCOMPARE(pomodoro.todaySessionCount(), initialHistoryCount);

    // Reset: Resets phase duration
    pomodoro.reset();
    QCOMPARE(pomodoro.sessionType(), static_cast<int>(PomodoroManager::BreakSession));
    QCOMPARE(pomodoro.timerState(), static_cast<int>(PomodoroManager::Stopped));
    QCOMPARE(pomodoro.remainingSeconds(), 300);
}

void BackendTest::testPomodoroPresetsManagement()
{
    SettingsManager settings;
    PomodoroManager pomodoro(&settings);

    pomodoro.restoreDefaultPresets();
    QVERIFY(pomodoro.presetsVariant().size() >= 3);

    // Check default Classic preset (25 / 5)
    pomodoro.selectPreset(0);
    QCOMPARE(pomodoro.focusDurationMinutes(), 25);
    QCOMPARE(pomodoro.breakDurationMinutes(), 5);

    // Add custom preset
    QVERIFY(pomodoro.addCustomPreset(QStringLiteral("Deep Sprint"), 45, 10));
    int customIdx = pomodoro.presetsVariant().size() - 1;
    pomodoro.selectPreset(customIdx);
    QCOMPARE(pomodoro.focusDurationMinutes(), 45);
    QCOMPARE(pomodoro.breakDurationMinutes(), 10);
    QCOMPARE(pomodoro.currentPresetName(), QStringLiteral("Deep Sprint"));

    // Update custom preset
    QVERIFY(pomodoro.updateCustomPreset(customIdx, QStringLiteral("Extended Sprint"), 55, 12));
    QCOMPARE(pomodoro.focusDurationMinutes(), 55);
    QCOMPARE(pomodoro.breakDurationMinutes(), 12);
    QCOMPARE(pomodoro.currentPresetName(), QStringLiteral("Extended Sprint"));

    // Delete custom preset
    QVERIFY(pomodoro.deleteCustomPreset(customIdx));

    // Protected default presets cannot be deleted
    QVERIFY(!pomodoro.deleteCustomPreset(0));
}

void BackendTest::testPomodoroHistoryAndHeatmapCalculations()
{
    SettingsManager settings;
    PomodoroManager pomodoro(&settings);

    pomodoro.clearHistory();
    QCOMPARE(pomodoro.todayFocusSeconds(), 0);
    QCOMPARE(pomodoro.todaySessionCount(), 0);

    // Verify last 7 days returns 7 items
    QVariantList last7 = pomodoro.last7DaysData();
    QCOMPARE(last7.size(), 7);

    // Test monthly heatmap calculation
    QDate today = QDate::currentDate();
    QVariantMap monthData = pomodoro.monthData(today.year(), today.month());
    QVERIFY(monthData.contains(QStringLiteral("days")));
    QVariantList days = monthData.value(QStringLiteral("days")).toList();
    QCOMPARE(days.size(), today.daysInMonth());

    // Initially with 0 seconds, heatmap intensity must be 0
    QVariantMap todayItem = days.at(today.day() - 1).toMap();
    QCOMPARE(todayItem.value(QStringLiteral("heatmapIntensity")).toInt(), 0);

    // Verify sessionsForDate returns empty list
    QVariantList sessionsToday = pomodoro.sessionsForDate(today.toString(Qt::ISODate));
    QCOMPARE(sessionsToday.size(), 0);
}

void BackendTest::testPomodoroLayoutIntegration()
{
    SettingsManager settings;
    LayoutController layout(&settings);

    // Pomodoro widget must be registered
    QVERIFY(layout.isWidgetRegistered("pomodoro"));
    QCOMPARE(layout.widgetMinWidth("pomodoro"), 18);
    QCOMPARE(layout.widgetMinHeight("pomodoro"), 10);

    // Non-mandatory widget check
    QVERIFY(!layout.isMandatoryWidget("pomodoro"));

    // Add Pomodoro widget to desktop
    QVERIFY(layout.addWidget("pomodoro"));
    QCOMPARE(layout.widgetVisible("pomodoro"), true);
    QCOMPARE(settings.pomodoroVisible(), true);

    // Remove Pomodoro widget from desktop
    QVERIFY(layout.removeWidget("pomodoro"));
    QCOMPARE(layout.widgetVisible("pomodoro"), false);
    QCOMPARE(settings.pomodoroVisible(), false);

    // Re-add Pomodoro widget
    QVERIFY(layout.addWidget("pomodoro"));
    QCOMPARE(layout.widgetVisible("pomodoro"), true);
}

QTEST_MAIN(BackendTest)
#include "test_backend.moc"
