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

        // 1. Core Playback Persistence Card
        SettingsCard {
            title: "Playback Persistence & Startup"
            description: "Session restoration preferences when launching Music Focus"
            iconSource: "qrc:/icons/lucide/play.svg"

            SettingsToggle {
                title: "Auto-play on startup"
                description: "Automatically start playing the restored track upon application launch"
                checked: settings.autoPlay
                onToggled: function(c) { settings.autoPlay = c; }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            SettingsToggle {
                title: "Remember last played track"
                description: "Restores your current playlist selection between application sessions"
                checked: settings.rememberLastTrack
                onToggled: function(c) { settings.rememberLastTrack = c; }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            SettingsToggle {
                title: "Resume exact playback position"
                description: "Seeks to the exact second where audio was last paused"
                checked: settings.resumePlaybackPosition
                enabled: settings.rememberLastTrack
                onToggled: function(c) { settings.resumePlaybackPosition = c; }
            }
        }

        // 2. Playback Controls & Hotkeys Summary Card
        SettingsCard {
            title: "Desktop Playback Controls"
            description: "Primary controls available on the floating playback strip and via keyboard shortcuts"
            iconSource: "qrc:/icons/lucide/disc.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                // Volume Card
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 70
                    radius: 8
                    color: Qt.rgba(1, 1, 1, 0.03)
                    border.color: Qt.rgba(1, 1, 1, 0.08)
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 4

                        RowLayout {
                            spacing: 6
                            Kirigami.Icon {
                                source: "qrc:/icons/actions/audio-volume-high.svg"
                                isMask: true
                                implicitWidth: 16
                                implicitHeight: 16
                                color: "#89b4fa"
                            }
                            Text {
                                text: "Volume & Mute"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: "#cdd6f4"
                            }
                        }
                        Text {
                            text: "Up/Down arrows or strip slider. 'M' toggles mute."
                            font.pixelSize: 10
                            color: "#9399b2"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }
                }

                // Shuffle Card
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 70
                    radius: 8
                    color: Qt.rgba(1, 1, 1, 0.03)
                    border.color: Qt.rgba(1, 1, 1, 0.08)
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 4

                        RowLayout {
                            spacing: 6
                            Kirigami.Icon {
                                source: "qrc:/icons/actions/media-playlist-shuffle.svg"
                                isMask: true
                                implicitWidth: 16
                                implicitHeight: 16
                                color: "#89b4fa"
                            }
                            Text {
                                text: "Shuffle Mode"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: "#cdd6f4"
                            }
                        }
                        Text {
                            text: "Non-repeating randomized track sequence."
                            font.pixelSize: 10
                            color: "#9399b2"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }
                }

                // Repeat Card
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 70
                    radius: 8
                    color: Qt.rgba(1, 1, 1, 0.03)
                    border.color: Qt.rgba(1, 1, 1, 0.08)
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 4

                        RowLayout {
                            spacing: 6
                            Kirigami.Icon {
                                source: "qrc:/icons/actions/media-playlist-repeat.svg"
                                isMask: true
                                implicitWidth: 16
                                implicitHeight: 16
                                color: "#89b4fa"
                            }
                            Text {
                                text: "Repeat Modes"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: "#cdd6f4"
                            }
                        }
                        Text {
                            text: "Press 'R' to cycle: Off, Repeat All, Repeat One."
                            font.pixelSize: 10
                            color: "#9399b2"
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 8 }
    }
}
