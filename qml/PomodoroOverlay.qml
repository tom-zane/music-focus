import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    readonly property bool isFocus: pomodoroManager ? pomodoroManager.sessionType === 0 : true
    readonly property bool isRunning: pomodoroManager ? pomodoroManager.isRunning : false
    readonly property bool isPaused: pomodoroManager ? pomodoroManager.isPaused : false
    readonly property real uiScale: (settings && settings.pomodoroScale) ? settings.pomodoroScale : 1.0

    readonly property bool breathingEnabled: settings ? settings.pomodoroBreathingEnabled : true
    readonly property real breathingSpeed: settings ? settings.pomodoroBreathingSpeed : 1.0
    readonly property real breathingIntensity: settings ? settings.pomodoroBreathingIntensity : 0.7

    property real breathFactor: 0.0

    // Subtle, gentle breathing animation during running focus phase
    SequentialAnimation {
        id: breathAnim
        running: root.isFocus && root.isRunning && root.breathingEnabled
        loops: Animation.Infinite

        NumberAnimation {
            target: root
            property: "breathFactor"
            from: 0.0
            to: 1.0
            duration: Math.max(1200, Math.round(2800 / Math.max(0.2, root.breathingSpeed)))
            easing.type: Easing.InOutSine
        }
        NumberAnimation {
            target: root
            property: "breathFactor"
            from: 1.0
            to: 0.0
            duration: Math.max(1200, Math.round(2800 / Math.max(0.2, root.breathingSpeed)))
            easing.type: Easing.InOutSine
        }
    }

    onIsRunningChanged: {
        if (!isRunning) {
            breathFactor = 0.0;
        }
    }

    readonly property real effectiveBreath: breathingEnabled ? (breathFactor * breathingIntensity) : 0.0

    // Subtle green-tinted background for Focus; subtle blue-tinted background for Break
    color: {
        var baseOpacity = settings ? settings.overlayOpacity : 0.85;
        if (contrastManager.isDark) {
            if (isFocus) {
                var g = 0.14 + 0.09 * effectiveBreath;
                var r = 0.07 + 0.03 * effectiveBreath;
                var b = 0.09 + 0.03 * effectiveBreath;
                return Qt.rgba(r, g, b, baseOpacity);
            } else {
                return Qt.rgba(0.07, 0.12, 0.22, baseOpacity);
            }
        } else {
            if (isFocus) {
                return Qt.rgba(0.91, 0.97, 0.93, baseOpacity);
            } else {
                return Qt.rgba(0.91, 0.95, 0.99, baseOpacity);
            }
        }
    }

    radius: Math.max(10, Math.min(18, height * 0.12))
    border.color: {
        if (isFocus) {
            return isRunning
                ? (contrastManager.isDark ? Qt.rgba(0.55, 0.88, 0.65, 0.45 + 0.30 * effectiveBreath) : "#16a34a")
                : (contrastManager.isDark ? Qt.rgba(0.45, 0.75, 0.55, 0.25) : Qt.rgba(0, 0, 0, 0.12));
        } else {
            return isRunning
                ? (contrastManager.isDark ? Qt.rgba(0.50, 0.75, 0.98, 0.50) : "#2563eb")
                : (contrastManager.isDark ? Qt.rgba(0.40, 0.65, 0.90, 0.25) : Qt.rgba(0, 0, 0, 0.12));
        }
    }
    border.width: isRunning ? 1.5 : 1
    clip: true

    FrostedBackground {
        targetItem: root
        radius: root.radius
    }

    // Dynamic padding
    readonly property real padding: Math.max(6, Math.min(width * 0.04, height * 0.06, 16))
    readonly property real availWidth: Math.max(1, width - padding * 2)
    readonly property real availHeight: Math.max(1, height - padding * 2)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: root.padding
        spacing: Math.max(4, Math.min(12, root.availHeight * 0.05))

        // --- TOP HEADER: PHASE BADGE & PRESET SELECTOR ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            // Session Phase Badge (FOCUS vs BREAK)
            Rectangle {
                implicitWidth: phaseLabel.implicitWidth + 16
                implicitHeight: Math.max(20, Math.min(26, root.availHeight * 0.18))
                radius: height / 2
                color: root.isFocus
                    ? (contrastManager.isDark ? Qt.rgba(0.65, 0.89, 0.63, 0.22) : Qt.rgba(0.09, 0.64, 0.29, 0.15))
                    : (contrastManager.isDark ? Qt.rgba(0.54, 0.71, 0.98, 0.22) : Qt.rgba(0.15, 0.39, 0.92, 0.15))
                border.color: root.isFocus
                    ? (contrastManager.isDark ? "#a6e3a1" : "#16a34a")
                    : (contrastManager.isDark ? "#89b4fa" : "#2563eb")
                border.width: 1

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 5

                    Rectangle {
                        implicitWidth: 6
                        implicitHeight: 6
                        radius: 3
                        color: root.isFocus
                            ? (contrastManager.isDark ? "#a6e3a1" : "#16a34a")
                            : (contrastManager.isDark ? "#89b4fa" : "#2563eb")
                    }

                    Text {
                        id: phaseLabel
                        text: (pomodoroManager ? pomodoroManager.sessionTypeName.toUpperCase() : "FOCUS")
                        font.pixelSize: Math.max(9, Math.min(11, root.availHeight * 0.10))
                        font.weight: Font.Bold
                        color: root.isFocus
                            ? (contrastManager.isDark ? "#a6e3a1" : "#15803d")
                            : (contrastManager.isDark ? "#89b4fa" : "#1d4ed8")
                    }
                }
            }

            Item { Layout.fillWidth: true }

            // Preset Selector Pill Button
            Rectangle {
                id: presetButton
                implicitWidth: presetRow.implicitWidth + 14
                implicitHeight: Math.max(20, Math.min(26, root.availHeight * 0.18))
                radius: height / 2
                color: presetHover.hovered ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(1, 1, 1, 0.05)
                border.color: presetHover.hovered ? Qt.rgba(1, 1, 1, 0.25) : Qt.rgba(1, 1, 1, 0.10)
                border.width: 1

                HoverHandler { id: presetHover }
                TapHandler {
                    onTapped: presetMenu.open()
                }

                ToolTip.text: "Select timer preset"
                ToolTip.visible: presetHover.hovered

                RowLayout {
                    id: presetRow
                    anchors.centerIn: parent
                    spacing: 4

                    Text {
                        text: pomodoroManager ? pomodoroManager.currentPresetName : "Classic"
                        font.pixelSize: Math.max(9, Math.min(10, root.availHeight * 0.09))
                        font.weight: Font.Medium
                        color: contrastManager.primaryTextColor
                        elide: Text.ElideRight
                        Layout.maximumWidth: Math.max(40, root.availWidth * 0.40)
                    }

                    Kirigami.Icon {
                        source: "qrc:/icons/lucide/chevron-down.svg"
                        isMask: true
                        implicitWidth: 10
                        implicitHeight: 10
                        color: contrastManager.secondaryTextColor
                    }
                }

                Menu {
                    id: presetMenu
                    y: presetButton.height + 4

                    Repeater {
                        model: pomodoroManager ? pomodoroManager.presets : []
                        MenuItem {
                            text: modelData.name + " (" + modelData.focusDurationMinutes + "m / " + modelData.breakDurationMinutes + "m)"
                            font.pixelSize: 11
                            checked: pomodoroManager && pomodoroManager.activePresetIndex === index
                            checkable: true
                            onTriggered: {
                                if (pomodoroManager) {
                                    pomodoroManager.selectPreset(index);
                                }
                            }
                        }
                    }
                }
            }
        }

        // --- SUBTLE PROGRESS BAR ---
        Rectangle {
            Layout.fillWidth: true
            height: 2
            radius: 1
            color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(0, 0, 0, 0.08)

            Rectangle {
                width: parent.width * (pomodoroManager ? pomodoroManager.progress : 0.0)
                height: parent.height
                radius: parent.radius
                color: root.isFocus
                    ? (contrastManager.isDark ? "#a6e3a1" : "#16a34a")
                    : (contrastManager.isDark ? "#89b4fa" : "#2563eb")
                Behavior on width {
                    NumberAnimation { duration: 150 }
                }
            }
        }

        Item { Layout.fillHeight: true }

        // --- PROMINENT COUNTDOWN DISPLAY ---
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Math.max(28, root.availHeight * 0.42)

            Text {
                id: countdownText
                anchors.centerIn: parent
                text: pomodoroManager ? pomodoroManager.formattedTime : "25:00"
                font.weight: Font.Bold
                font.family: "monospace"
                font.pixelSize: {
                    var wFit = root.availWidth / 3.4;
                    var hFit = parent.height * 0.92;
                    return Math.max(16, Math.min(wFit, hFit, 68));
                }
                color: contrastManager.primaryTextColor
                fontSizeMode: Text.Fit
                minimumPixelSize: 12
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item { Layout.fillHeight: true }

        // --- COMPACT CONTROLS: RESET, PLAY/PAUSE, SKIP ---
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: Math.max(8, Math.min(16, root.availWidth * 0.06))

            // 1. Reset Button
            Button {
                id: resetBtn
                implicitWidth: Math.max(30, Math.min(36, root.availHeight * 0.22))
                implicitHeight: implicitWidth
                focusPolicy: Qt.NoFocus
                ToolTip.text: "Reset session"
                ToolTip.visible: hovered
                onClicked: {
                    if (pomodoroManager) pomodoroManager.reset();
                }

                background: Rectangle {
                    radius: parent.height / 2
                    color: resetBtn.down
                        ? Qt.rgba(1, 1, 1, 0.18)
                        : (resetBtn.hovered ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.04))
                    border.color: resetBtn.hovered ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(1, 1, 1, 0.08)
                    border.width: 1
                }

                contentItem: Item {
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/rotate-ccw.svg"
                        isMask: true
                        implicitWidth: 15
                        implicitHeight: 15
                        color: resetBtn.hovered ? contrastManager.primaryTextColor : contrastManager.secondaryTextColor
                    }
                }
            }

            // 2. Play / Pause Button (Primary focal button)
            Button {
                id: playPauseBtn
                implicitWidth: Math.max(38, Math.min(46, root.availHeight * 0.26))
                implicitHeight: implicitWidth
                focusPolicy: Qt.NoFocus
                ToolTip.text: root.isRunning ? "Pause timer" : "Start timer"
                ToolTip.visible: hovered
                onClicked: {
                    if (pomodoroManager) pomodoroManager.togglePlayPause();
                }

                background: Rectangle {
                    radius: parent.height / 2
                    color: playPauseBtn.down
                        ? (root.isFocus ? (contrastManager.isDark ? "#84d67d" : "#14532d") : (contrastManager.isDark ? "#6ea4f7" : "#1e40af"))
                        : (playPauseBtn.hovered
                            ? (root.isFocus ? (contrastManager.isDark ? "#a6e3a1" : "#16a34a") : (contrastManager.isDark ? "#89b4fa" : "#2563eb"))
                            : (root.isFocus ? (contrastManager.isDark ? Qt.rgba(0.65, 0.89, 0.63, 0.35) : "#16a34a") : (contrastManager.isDark ? Qt.rgba(0.54, 0.71, 0.98, 0.35) : "#2563eb")))
                    border.color: contrastManager.isDark ? "#ffffff" : Qt.rgba(0, 0, 0, 0.15)
                    border.width: 1

                    Behavior on color {
                        ColorAnimation { duration: 120 }
                    }
                }

                contentItem: Item {
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: root.isRunning ? "qrc:/icons/lucide/pause.svg" : "qrc:/icons/lucide/play.svg"
                        isMask: true
                        implicitWidth: 18
                        implicitHeight: 18
                        color: playPauseBtn.hovered
                            ? "#11111b"
                            : (contrastManager.isDark ? contrastManager.primaryTextColor : "#ffffff")
                    }
                }
            }

            // 3. Skip Button
            Button {
                id: skipBtn
                implicitWidth: Math.max(30, Math.min(36, root.availHeight * 0.22))
                implicitHeight: implicitWidth
                focusPolicy: Qt.NoFocus
                ToolTip.text: "Skip to next phase"
                ToolTip.visible: hovered
                onClicked: {
                    if (pomodoroManager) pomodoroManager.skip();
                }

                background: Rectangle {
                    radius: parent.height / 2
                    color: skipBtn.down
                        ? Qt.rgba(1, 1, 1, 0.18)
                        : (skipBtn.hovered ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.04))
                    border.color: skipBtn.hovered ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(1, 1, 1, 0.08)
                    border.width: 1
                }

                contentItem: Item {
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/skip-forward.svg"
                        isMask: true
                        implicitWidth: 15
                        implicitHeight: 15
                        color: skipBtn.hovered ? contrastManager.primaryTextColor : contrastManager.secondaryTextColor
                    }
                }
            }
        }
    }
}
