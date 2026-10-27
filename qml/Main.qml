import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import org.kde.kirigami as Kirigami

ApplicationWindow {
    id: window

    title: {
        var base = "Music Focus";
        if (playlistManager.hasCurrentTrack) {
            return base + " — " + playlistManager.currentTitle;
        }
        return base;
    }

    width: 1100
    height: 700
    minimumWidth: 720
    minimumHeight: 480
    visible: true
    color: "#181926"

    font.family: settings.fontFamily.length > 0 ? settings.fontFamily : ""
    font.pointSize: settings.fontSize > 0 ? settings.fontSize : 11

    // Track fullscreen state cleanly
    property bool isFullscreen: visibility === Window.FullScreen

    function toggleFullscreen() {
        if (isFullscreen) {
            window.showNormal();
        } else {
            window.showFullScreen();
        }
    }

    function openSettings(tabIndex) {
        if (typeof tabIndex === "number" && tabIndex >= 0) {
            settingsDialog.openTab(tabIndex);
        } else {
            settingsDialog.open();
        }
    }

    function scrollSettings(ratio) {
        settingsDialog.setScrollPosition(ratio);
    }

    function isTyping() {
        var item = window.activeFocusItem;
        if (!item) return false;
        var s = item.toString();
        return s.indexOf("QQuickTextInput") !== -1 ||
               s.indexOf("QQuickTextField") !== -1 ||
               s.indexOf("QQuickTextEdit") !== -1 ||
               item.hasOwnProperty("echoMode") ||
               item.hasOwnProperty("cursorPosition");
    }

    // Centralized inactivity-based overlay opacity:
    readonly property bool overlaysActive: !settings.inactivityEnabled || appController.userActive || settingsDialog.opened || setupDialog.opened
    readonly property real overlayTargetOpacity: overlaysActive ? 1.0 : (settings.inactivityMode === 0 ? settings.inactivityFadeOpacity : 0.00)

    function notifyActivity() {
        appController.reportActivity();
    }

    // Mouse tracker over full window for activity
    MouseArea {
        id: activityTracker
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton
        onPositionChanged: window.notifyActivity()
        onWheel: window.notifyActivity()
    }

    // --- 1. FIRST-LAUNCH ONBOARDING WELCOME SCREEN ---
    WelcomeSetupView {
        id: welcomeView
        anchors.fill: parent
        visible: appController.isFirstLaunchSetup
        z: 50
    }

    // --- 2. RECOVERY SCREEN (WHEN SAVED DIRECTORY BECOMES UNAVAILABLE) ---
    RecoveryView {
        id: recoveryView
        anchors.fill: parent
        visible: !appController.isFirstLaunchSetup && appController.isRecoveryRequired
        z: 40
    }

    // --- 3. MAIN PLAYER DESKTOP VIEW ---
    Item {
        id: mainPlayerView
        anchors.fill: parent
        visible: !appController.isFirstLaunchSetup && !appController.isRecoveryRequired

        // --- FULL-WINDOW WALLPAPER BACKGROUND ---
        WallpaperView {
            id: wallpaperView
            anchors.fill: parent
        }

        // --- AUDIO VISUALIZER (Above Wallpaper, Below Interactive Controls) ---
        AudioVisualizer {
            id: audioVisualizer
        }

        // --- DESKTOP GRID OVERLAY (Active during layout editing) ---
        DesktopGridOverlay {
            id: desktopGridOverlay
            anchors.fill: parent
            z: 80
        }

        // --- FLOATING LAYOUT EDITING TOOLBAR ---
        Rectangle {
            id: editToolbar
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            implicitWidth: editToolbarRow.implicitWidth + 24
            implicitHeight: 42
            radius: 21
            color: Qt.rgba(0.12, 0.13, 0.18, 0.96)
            border.color: "#89b4fa"
            border.width: 1.5
            visible: layoutController.editing
            z: 200

            FrostedBackground {
                targetItem: parent
                radius: parent.radius
            }

            RowLayout {
                id: editToolbarRow
                anchors.centerIn: parent
                spacing: 12

                Kirigami.Icon {
                    source: "qrc:/icons/lucide/move.svg"
                    implicitWidth: 16
                    implicitHeight: 16
                    color: "#89b4fa"
                }

                Text {
                    text: "Layout Grid Editing"
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    color: "#ffffff"
                }

                Text {
                    text: "• Drag top-right to move • Drag bottom-right to resize"
                    font.pixelSize: 11
                    color: "#a6adc8"
                }

                Button {
                    text: "Reset Layout"
                    font.pixelSize: 11
                    font.weight: Font.DemiBold
                    implicitHeight: 28
                    onClicked: layoutController.resetLayout()
                }

                Button {
                    text: "Done"
                    font.pixelSize: 11
                    font.weight: Font.Bold
                    highlighted: true
                    implicitHeight: 28
                    onClicked: layoutController.stopEditing()
                }
            }
        }

        // --- TRANSLUCENT TODO WIDGET (Wrapped in responsive Desktop Grid layout) ---
        WidgetWrapper {
            id: todoWrapper
            widgetId: "todo"
            widgetTitle: "Study Tasks"
            visible: settings.todoEnabled && settings.todoVisible
            z: 90

            TodoOverlay {
                anchors.fill: parent
                opacity: window.overlaysActive || layoutController.editing ? 1.0 : (settings.inactivityMode === 0 ? settings.inactivityFadeOpacity : 0.00)

                Behavior on opacity {
                    NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
                }
            }
        }

        // --- FOCUS CLOCK WIDGET (Wrapped in responsive Desktop Grid layout) ---
        WidgetWrapper {
            id: clockWrapper
            widgetId: "clock"
            widgetTitle: "Focus Clock"
            visible: settings.clockEnabled && layoutController.widgetVisible("clock")
            z: 88

            ClockOverlay {
                anchors.fill: parent
                opacity: window.overlaysActive || layoutController.editing ? 1.0 : (settings.inactivityMode === 0 ? settings.inactivityFadeOpacity : 0.00)

                Behavior on opacity {
                    NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
                }
            }
        }

        // --- POMODORO TIMER WIDGET (Wrapped in responsive Desktop Grid layout) ---
        WidgetWrapper {
            id: pomodoroWrapper
            widgetId: "pomodoro"
            widgetTitle: "Pomodoro Timer"
            visible: settings.pomodoroEnabled && layoutController.widgetVisible("pomodoro")
            z: 89

            PomodoroOverlay {
                anchors.fill: parent
                opacity: window.overlaysActive || layoutController.editing ? 1.0 : (settings.inactivityMode === 0 ? settings.inactivityFadeOpacity : 0.00)

                Behavior on opacity {
                    NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
                }
            }
        }

        // --- OVERLAY CONTAINER (Fades or hides based on inactivity mode) ---
        Item {
            id: overlayContainer
            anchors.fill: parent
            opacity: layoutController.editing ? 1.0 : window.overlayTargetOpacity

            Behavior on opacity {
                NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
            }

            // --- PLAYLIST WIDGET (Tracks list wrapped in Desktop Grid) ---
            WidgetWrapper {
                id: playlistWrapper
                widgetId: "playlist"
                widgetTitle: "Tracks List"

                PlaylistOverlay {
                    anchors.fill: parent
                }
            }

            // --- PLAYBACK WIDGET (Floating playback strip wrapped in Desktop Grid) ---
            WidgetWrapper {
                id: playbackWrapper
                widgetId: "playback"
                widgetTitle: "Playback Controls"

                PlaybackOverlay {
                    anchors.fill: parent
                }
            }

            // --- TOP-RIGHT CONTROLS ---
            TopControlsOverlay {
                id: topControls
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: Math.max(16, Math.round(20 * (settings ? settings.topControlsScale : 1.0)))
                anchors.rightMargin: Math.max(20, Math.round(24 * (settings ? settings.topControlsScale : 1.0)))
                isFullscreen: window.isFullscreen

                onFullscreenToggled: window.toggleFullscreen()
                onSettingsClicked: {
                    window.notifyActivity();
                    settingsDialog.open();
                }
                onTodoToggled: {
                    window.notifyActivity();
                }
                onLayoutEditToggled: {
                    window.notifyActivity();
                    layoutController.toggleEditing();
                }
            }
        }
    }

    // --- NOTIFICATION TOAST ---
    Rectangle {
        id: toast
        anchors.top: parent.top
        anchors.topMargin: layoutController.editing ? 74 : 24
        anchors.horizontalCenter: parent.horizontalCenter

        Behavior on anchors.topMargin {
            NumberAnimation { duration: 200 }
        }
        implicitWidth: Math.min(500, toastText.implicitWidth + 36)
        implicitHeight: 36
        radius: 18
        color: toastIsError ? Qt.rgba(0.95, 0.4, 0.45, 0.92) : Qt.rgba(0.12, 0.13, 0.18, 0.92)
        border.color: Qt.rgba(1, 1, 1, 0.2)
        border.width: 1
        visible: opacity > 0.01
        opacity: 0.0

        property bool toastIsError: false

        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }

        RowLayout {
            anchors.centerIn: parent
            spacing: 8

            Kirigami.Icon {
                source: toast.toastIsError ? "qrc:/icons/lucide/triangle-alert.svg" : "qrc:/icons/lucide/info.svg"
                implicitWidth: 16
                implicitHeight: 16
                color: "#ffffff"
            }

            Text {
                id: toastText
                font.pixelSize: 12
                font.weight: Font.Medium
                color: "#ffffff"
                elide: Text.ElideRight
            }
        }

        Timer {
            id: toastTimer
            interval: 3500
            onTriggered: toast.opacity = 0.0
        }

        function showMessage(msg, isErr) {
            toastText.text = msg;
            toastIsError = isErr;
            toast.opacity = 1.0;
            toastTimer.restart();
        }
    }

    // --- SETTINGS DIALOG ---
    SettingsDialog {
        id: settingsDialog
    }

    // --- INITIAL SETUP DIALOG ---
    InitialSetupDialog {
        id: setupDialog
    }

    // Listen to Controller signals
    Connections {
        target: appController

        function onNotificationRequested(message, isError) {
            toast.showMessage(message, isError);
        }
    }

    // =========================================================================
    // GLOBAL KEYBOARD SHORTCUTS
    // =========================================================================

    // Play / Pause
    Shortcut {
        sequence: "Space"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            audioPlayer.togglePlayPause();
        }
    }

    // Previous Track
    Shortcut {
        sequence: "Left"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            playlistManager.previousTrack();
        }
    }

    // Next Track
    Shortcut {
        sequence: "Right"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            playlistManager.nextTrack();
        }
    }

    // Seek Backward 5s
    Shortcut {
        sequence: "Shift+Left"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            audioPlayer.seek(Math.max(0, audioPlayer.position - 5000));
        }
    }

    // Seek Forward 5s
    Shortcut {
        sequence: "Shift+Right"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            audioPlayer.seek(Math.min(audioPlayer.duration, audioPlayer.position + 5000));
        }
    }

    // Fullscreen Toggle
    Shortcut {
        sequence: "F11"
        onActivated: {
            window.notifyActivity();
            window.toggleFullscreen();
        }
    }

    // Exit Fullscreen / Close Dialogs / Exit Edit Mode
    Shortcut {
        sequence: "Escape"
        onActivated: {
            if (layoutController.editing) {
                layoutController.stopEditing();
            } else if (settingsDialog.opened) {
                settingsDialog.close();
            } else if (setupDialog.opened) {
                setupDialog.close();
            } else if (window.isFullscreen) {
                window.showNormal();
            }
        }
    }

    // Toggle Desktop Grid Layout Editing
    Shortcut {
        sequence: "E"
        enabled: !window.isTyping() && !appController.isFirstLaunchSetup && !appController.isRecoveryRequired
        onActivated: {
            window.notifyActivity();
            layoutController.toggleEditing();
        }
    }

    // Volume Up
    Shortcut {
        sequence: "Up"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            var newVol = Math.min(1.0, audioPlayer.volume + 0.05);
            audioPlayer.setVolume(newVol);
            settings.volume = newVol;
        }
    }

    // Volume Down
    Shortcut {
        sequence: "Down"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            var newVol = Math.max(0.0, audioPlayer.volume - 0.05);
            audioPlayer.setVolume(newVol);
            settings.volume = newVol;
        }
    }

    // Toggle Mute
    Shortcut {
        sequence: "M"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            audioPlayer.setMuted(!audioPlayer.muted);
        }
    }

    // Shuffle Toggle
    Shortcut {
        sequence: "S"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            settingsDialog.open();
        }
    }

    // Open Settings (Ctrl+,)
    Shortcut {
        sequence: "Ctrl+,"
        onActivated: {
            window.notifyActivity();
            settingsDialog.open();
        }
    }

    // Toggle Todo Widget
    Shortcut {
        sequence: "T"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            settings.todoVisible = !settings.todoVisible;
        }
    }

    // Toggle Pomodoro Widget
    Shortcut {
        sequence: "P"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            settings.pomodoroVisible = !settings.pomodoroVisible;
        }
    }

    // Next Wallpaper / Video
    Shortcut {
        sequence: "W"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            if (settings.wallpaperMode === 2) {
                wallpaperManager.nextVideo();
            } else if (settings.wallpaperMode === 0) {
                wallpaperManager.regenerateGradientSeed();
            } else {
                wallpaperManager.nextWallpaper();
            }
        }
    }

    // Previous Wallpaper / Video
    Shortcut {
        sequence: "Shift+W"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            if (settings.wallpaperMode === 2) {
                wallpaperManager.previousVideo();
            } else if (settings.wallpaperMode === 0) {
                wallpaperManager.regenerateGradientSeed();
            } else {
                wallpaperManager.previousWallpaper();
            }
        }
    }

    // Reveal Controls immediately
    Shortcut {
        sequence: "H"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
        }
    }

    // Repeat Mode Cycle
    Shortcut {
        sequence: "R"
        enabled: !window.isTyping()
        onActivated: {
            window.notifyActivity();
            playlistManager.cycleRepeatMode();
        }
    }
}
