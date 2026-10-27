import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

ScrollView {
    id: root
    clip: true
    contentWidth: availableWidth
    ScrollBar.vertical.policy: ScrollBar.AsNeeded

    ColumnLayout {
        width: root.availableWidth - 12
        spacing: 16

        // Top Action Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Text {
                    text: "Desktop Widgets Catalogue"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    color: "#89b4fa"
                }
                Text {
                    text: "Configure floating overlays on your study workspace. Core widgets remain persistent."
                    font.pixelSize: 11
                    color: "#9399b2"
                }
            }

            SettingsButton {
                isPrimary: true
                text: layoutController.editing ? "Done Editing Layout" : "Edit Layout Grid (E)"
                iconSource: "qrc:/icons/lucide/layout-grid.svg"
                onClicked: {
                    settingsDialog.close();
                    layoutController.startEditing();
                }
            }
        }

        // =====================================================================
        // 1. STUDY TASKS (TODO WIDGET) — STRUCTURED INTO 3 SPECIFIED SECTIONS
        // =====================================================================
        SettingsCard {
            title: "Study Tasks (Todo Widget)"
            description: "Translucent checklist for study goals, homework, and assignment tracking"
            iconSource: "qrc:/icons/lucide/list-todo.svg"

            // 1. Widget Enablement and Visibility
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Rectangle {
                    implicitWidth: todoStatusBadge.implicitWidth + 14
                    implicitHeight: 22
                    radius: 4
                    color: layoutController.widgetVisible("todo") ? Qt.rgba(0.65, 0.89, 0.63, 0.18) : Qt.rgba(1, 1, 1, 0.08)
                    border.color: layoutController.widgetVisible("todo") ? "#a6e3a1" : Qt.rgba(1, 1, 1, 0.12)
                    border.width: 1

                    Text {
                        id: todoStatusBadge
                        anchors.centerIn: parent
                        text: layoutController.widgetVisible("todo") ? "ACTIVE ON DESKTOP" : "AVAILABLE"
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        color: layoutController.widgetVisible("todo") ? "#a6e3a1" : "#a6adc8"
                    }
                }

                // Keyboard Shortcut Badge
                Rectangle {
                    implicitWidth: 32
                    implicitHeight: 22
                    radius: 4
                    color: Qt.rgba(0.54, 0.71, 0.98, 0.15)
                    border.color: Qt.rgba(0.54, 0.71, 0.98, 0.3)
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: "T"
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        font.family: "monospace"
                        color: "#89b4fa"
                    }
                }

                Text {
                    text: "Press 'T' to toggle visibility anytime"
                    font.pixelSize: 11
                    color: "#9399b2"
                    Layout.fillWidth: true
                }

                SettingsButton {
                    visible: layoutController.widgetVisible("todo")
                    text: "Position"
                    iconSource: "qrc:/icons/lucide/move.svg"
                    onClicked: {
                        settingsDialog.close();
                        layoutController.startEditing();
                        layoutController.selectedWidgetId = "todo";
                    }
                }

                SettingsButton {
                    visible: layoutController.widgetVisible("todo")
                    isDestructive: true
                    text: "Remove"
                    iconSource: "qrc:/icons/lucide/trash-2.svg"
                    onClicked: layoutController.removeWidget("todo")
                }

                SettingsButton {
                    visible: !layoutController.widgetVisible("todo")
                    isPrimary: true
                    text: "Add to Desktop"
                    iconSource: "qrc:/icons/lucide/plus.svg"
                    onClicked: layoutController.addWidget("todo")
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
                visible: layoutController.widgetVisible("todo")
            }

            // 2. Widget Size and Appearance
            SettingsSlider {
                visible: layoutController.widgetVisible("todo")
                title: "Widget Scale"
                description: "Proportional scale of the task list container and text"
                from: 0.70
                to: 1.50
                stepSize: 0.05
                value: settings.todoScale
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.todoScale = Math.round(v * 100) / 100; }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
                visible: layoutController.widgetVisible("todo")
            }

            // 3. Task Management and Storage Information
            RowLayout {
                visible: layoutController.widgetVisible("todo")
                Layout.fillWidth: true
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    Text {
                        text: "Task Storage: " + todoManager.count + " total items (" + todoManager.activeCount + " active)"
                        font.pixelSize: 12
                        font.weight: Font.Medium
                        color: "#cdd6f4"
                    }
                    Text {
                        text: "Tasks are safely preserved in local configuration across app restarts."
                        font.pixelSize: 10
                        color: "#9399b2"
                    }
                }

                SettingsButton {
                    text: "Clear Completed"
                    iconSource: "qrc:/icons/lucide/trash-2.svg"
                    enabled: (todoManager.count - todoManager.activeCount) > 0
                    onClicked: todoManager.clearCompleted()
                }
            }
        }

        // =====================================================================
        // 2. FOCUS CLOCK (OPTIONAL DESKTOP WIDGET)
        // =====================================================================
        SettingsCard {
            title: "Focus Clock"
            description: "Digital clock and date widget with adaptive contrast"
            iconSource: "qrc:/icons/lucide/clock.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Rectangle {
                    implicitWidth: clockStatusBadge.implicitWidth + 14
                    implicitHeight: 22
                    radius: 4
                    color: layoutController.widgetVisible("clock") ? Qt.rgba(0.65, 0.89, 0.63, 0.18) : Qt.rgba(1, 1, 1, 0.08)
                    border.color: layoutController.widgetVisible("clock") ? "#a6e3a1" : Qt.rgba(1, 1, 1, 0.12)
                    border.width: 1

                    Text {
                        id: clockStatusBadge
                        anchors.centerIn: parent
                        text: layoutController.widgetVisible("clock") ? "ACTIVE ON DESKTOP" : "AVAILABLE"
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        color: layoutController.widgetVisible("clock") ? "#a6e3a1" : "#a6adc8"
                    }
                }

                Item { Layout.fillWidth: true }

                SettingsButton {
                    visible: layoutController.widgetVisible("clock")
                    text: "Position"
                    iconSource: "qrc:/icons/lucide/move.svg"
                    onClicked: {
                        settingsDialog.close();
                        layoutController.startEditing();
                        layoutController.selectedWidgetId = "clock";
                    }
                }

                SettingsButton {
                    visible: layoutController.widgetVisible("clock")
                    isDestructive: true
                    text: "Remove"
                    iconSource: "qrc:/icons/lucide/trash-2.svg"
                    onClicked: layoutController.removeWidget("clock")
                }

                SettingsButton {
                    visible: !layoutController.widgetVisible("clock")
                    isPrimary: true
                    text: "Add to Desktop"
                    iconSource: "qrc:/icons/lucide/plus.svg"
                    onClicked: layoutController.addWidget("clock")
                }
            }

            // Clock Options (when active)
            RowLayout {
                visible: layoutController.widgetVisible("clock")
                Layout.fillWidth: true
                spacing: 16

                SettingsToggle {
                    Layout.fillWidth: true
                    title: "24-Hour Format"
                    checked: settings.clock24Hour
                    onToggled: function(c) { settings.clock24Hour = c; }
                }

                SettingsToggle {
                    Layout.fillWidth: true
                    title: "Show Date"
                    checked: settings.clockShowDate
                    onToggled: function(c) { settings.clockShowDate = c; }
                }

                SettingsToggle {
                    Layout.fillWidth: true
                    title: "Show Seconds"
                    checked: settings.clockShowSeconds
                    onToggled: function(c) { settings.clockShowSeconds = c; }
                }
            }
        }

        // =====================================================================
        // 3. POMODORO TIMER (FLOATING DESKTOP WIDGET)
        // =====================================================================
        SettingsCard {
            title: "Pomodoro Timer"
            description: "Floating study timer with focus & break phases and subtle breathing glow"
            iconSource: "qrc:/icons/lucide/timer.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Rectangle {
                    implicitWidth: pomodoroStatusBadge.implicitWidth + 14
                    implicitHeight: 22
                    radius: 4
                    color: layoutController.widgetVisible("pomodoro") ? Qt.rgba(0.65, 0.89, 0.63, 0.18) : Qt.rgba(1, 1, 1, 0.08)
                    border.color: layoutController.widgetVisible("pomodoro") ? "#a6e3a1" : Qt.rgba(1, 1, 1, 0.12)
                    border.width: 1

                    Text {
                        id: pomodoroStatusBadge
                        anchors.centerIn: parent
                        text: layoutController.widgetVisible("pomodoro") ? "ACTIVE ON DESKTOP" : "AVAILABLE"
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        color: layoutController.widgetVisible("pomodoro") ? "#a6e3a1" : "#a6adc8"
                    }
                }

                // Keyboard Shortcut Badge
                Rectangle {
                    implicitWidth: 32
                    implicitHeight: 22
                    radius: 4
                    color: Qt.rgba(0.54, 0.71, 0.98, 0.15)
                    border.color: Qt.rgba(0.54, 0.71, 0.98, 0.3)
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: "P"
                        font.pixelSize: 10
                        font.weight: Font.Bold
                        font.family: "monospace"
                        color: "#89b4fa"
                    }
                }

                Text {
                    text: "Press 'P' to toggle visibility anytime"
                    font.pixelSize: 11
                    color: "#9399b2"
                    Layout.fillWidth: true
                }

                SettingsButton {
                    visible: layoutController.widgetVisible("pomodoro")
                    text: "Position"
                    iconSource: "qrc:/icons/lucide/move.svg"
                    onClicked: {
                        settingsDialog.close();
                        layoutController.startEditing();
                        layoutController.selectedWidgetId = "pomodoro";
                    }
                }

                SettingsButton {
                    visible: layoutController.widgetVisible("pomodoro")
                    isDestructive: true
                    text: "Remove"
                    iconSource: "qrc:/icons/lucide/trash-2.svg"
                    onClicked: layoutController.removeWidget("pomodoro")
                }

                SettingsButton {
                    visible: !layoutController.widgetVisible("pomodoro")
                    isPrimary: true
                    text: "Add to Desktop"
                    iconSource: "qrc:/icons/lucide/plus.svg"
                    onClicked: layoutController.addWidget("pomodoro")
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
                visible: layoutController.widgetVisible("pomodoro")
            }

            // Pomodoro Scale Slider
            SettingsSlider {
                visible: layoutController.widgetVisible("pomodoro")
                title: "Widget Scale"
                description: "Proportional scale of the floating Pomodoro widget"
                from: 0.70
                to: 1.50
                stepSize: 0.05
                value: settings.pomodoroScale
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.pomodoroScale = Math.round(v * 100) / 100; }
            }
        }

        // =====================================================================
        // 3. MANDATORY CORE WIDGETS (MUSIC CONTROL & PLAYLIST)
        // =====================================================================
        SettingsCard {
            title: "Core Desktop Panels"
            description: "Essential study music playback and queue panels"
            iconSource: "qrc:/icons/lucide/disc.svg"

            // Playback Strip Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    RowLayout {
                        spacing: 8
                        Text {
                            text: "Music Playback Strip"
                            font.pixelSize: 13
                            font.weight: Font.DemiBold
                            color: "#ffffff"
                        }
                        Rectangle {
                            implicitWidth: 70
                            implicitHeight: 18
                            radius: 3
                            color: Qt.rgba(0.54, 0.71, 0.98, 0.15)
                            border.color: "#89b4fa"
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "CORE PANEL"
                                font.pixelSize: 9
                                font.weight: Font.Bold
                                color: "#89b4fa"
                            }
                        }
                    }
                    Text {
                        text: "Floating scrub bar with track position, volume, shuffle, and repeat."
                        font.pixelSize: 10
                        color: "#9399b2"
                    }
                }

                SettingsButton {
                    text: "Position in Grid"
                    iconSource: "qrc:/icons/lucide/move.svg"
                    onClicked: {
                        settingsDialog.close();
                        layoutController.startEditing();
                        layoutController.selectedWidgetId = "playback";
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            // Playlist Row
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    RowLayout {
                        spacing: 8
                        Text {
                            text: "Tracks Playlist"
                            font.pixelSize: 13
                            font.weight: Font.DemiBold
                            color: "#ffffff"
                        }
                        Rectangle {
                            implicitWidth: 70
                            implicitHeight: 18
                            radius: 3
                            color: Qt.rgba(0.54, 0.71, 0.98, 0.15)
                            border.color: "#89b4fa"
                            border.width: 1
                            Text {
                                anchors.centerIn: parent
                                text: "CORE PANEL"
                                font.pixelSize: 9
                                font.weight: Font.Bold
                                color: "#89b4fa"
                            }
                        }
                    }
                    Text {
                        text: "Interactive track list with search, smooth scrolling, and instant switching."
                        font.pixelSize: 10
                        color: "#9399b2"
                    }
                }

                SettingsButton {
                    text: "Position in Grid"
                    iconSource: "qrc:/icons/lucide/move.svg"
                    onClicked: {
                        settingsDialog.close();
                        layoutController.startEditing();
                        layoutController.selectedWidgetId = "playlist";
                    }
                }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 8 }
    }
}
