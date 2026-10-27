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

        // 1. Keyboard Shortcuts Card
        SettingsCard {
            title: "Keyboard Shortcuts"
            description: "Global hotkeys active whenever the main application window is focused"
            iconSource: "qrc:/icons/lucide/help-circle.svg"

            GridLayout {
                Layout.fillWidth: true
                columns: 2
                rowSpacing: 8
                columnSpacing: 16

                Repeater {
                    model: [
                        { key: "Space", action: "Toggle Play / Pause" },
                        { key: "Left / Right", action: "Previous / Next track" },
                        { key: "Shift + Left/Right", action: "Seek backward / forward 5s" },
                        { key: "Up / Down", action: "Adjust volume ±5%" },
                        { key: "M", action: "Toggle mute audio" },
                        { key: "R", action: "Cycle repeat (Off / All / One)" },
                        { key: "T", action: "Toggle Study Tasks visibility" },
                        { key: "P", action: "Toggle Pomodoro Timer visibility" },
                        { key: "E", action: "Toggle Desktop Grid editing" },
                        { key: "W / Shift+W", action: "Next / Previous wallpaper" },
                        { key: "H / Tab", action: "Reveal faded/hidden controls" },
                        { key: "Ctrl + , / S", action: "Open Settings dialog" },
                        { key: "F11 / Esc", action: "Toggle Fullscreen / Close modal" }
                    ]

                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 34
                        radius: 6
                        color: Qt.rgba(1, 1, 1, 0.03)
                        border.color: Qt.rgba(1, 1, 1, 0.06)
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            spacing: 10

                            // Styled Kbd Badge
                            Rectangle {
                                implicitWidth: Math.max(50, keyText.implicitWidth + 12)
                                implicitHeight: 20
                                radius: 4
                                color: Qt.rgba(0.54, 0.71, 0.98, 0.16)
                                border.color: Qt.rgba(0.54, 0.71, 0.98, 0.35)
                                border.width: 1

                                Text {
                                    id: keyText
                                    anchors.centerIn: parent
                                    text: modelData.key
                                    font.pixelSize: 10
                                    font.weight: Font.Bold
                                    font.family: "monospace"
                                    color: "#89b4fa"
                                }
                            }

                            Text {
                                Layout.fillWidth: true
                                text: modelData.action
                                font.pixelSize: 11
                                color: "#cdd6f4"
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }

        // 2. Folder Structure and File Formats Card
        SettingsCard {
            title: "Music Folder & Wallpaper Structure"
            description: "How study audio files and wallpaper media are organized on disk"
            iconSource: "qrc:/icons/lucide/folder-open.svg"

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: dirTreeText.implicitHeight + 20
                radius: 8
                color: Qt.rgba(0, 0, 0, 0.35)
                border.color: Qt.rgba(1, 1, 1, 0.08)
                border.width: 1

                Text {
                    id: dirTreeText
                    anchors.fill: parent
                    anchors.margins: 12
                    font.family: "monospace"
                    font.pixelSize: 11
                    lineHeight: 1.35
                    color: "#bac2de"
                    text: "Your Music Folder/\n├── Track 1.mp3\n├── Album/\n│   └── Track 2.flac\n└── Wallpapers/\n    ├── Still Wallpapers/\n    │   ├── calm-forest.jpg\n    │   └── starry-night.png\n    └── Video Wallpapers/\n        ├── cozy-fireplace.mp4\n        └── rain-on-window.webm"
                }
            }

            // Supported formats chips
            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Rectangle {
                    implicitWidth: audioFormats.implicitWidth + 16
                    implicitHeight: 24
                    radius: 4
                    color: Qt.rgba(1, 1, 1, 0.05)
                    border.color: Qt.rgba(1, 1, 1, 0.10)
                    border.width: 1
                    Text {
                        id: audioFormats
                        anchors.centerIn: parent
                        text: "Audio: MP3, FLAC, OGG, WAV"
                        font.pixelSize: 10
                        color: "#a6adc8"
                    }
                }

                Rectangle {
                    implicitWidth: imageFormats.implicitWidth + 16
                    implicitHeight: 24
                    radius: 4
                    color: Qt.rgba(1, 1, 1, 0.05)
                    border.color: Qt.rgba(1, 1, 1, 0.10)
                    border.width: 1
                    Text {
                        id: imageFormats
                        anchors.centerIn: parent
                        text: "Images: JPG, PNG, WEBP, BMP"
                        font.pixelSize: 10
                        color: "#a6adc8"
                    }
                }

                Rectangle {
                    implicitWidth: videoFormats.implicitWidth + 16
                    implicitHeight: 24
                    radius: 4
                    color: Qt.rgba(1, 1, 1, 0.05)
                    border.color: Qt.rgba(1, 1, 1, 0.10)
                    border.width: 1
                    Text {
                        id: videoFormats
                        anchors.centerIn: parent
                        text: "Videos: MP4, MKV, WEBM, MOV, AVI"
                        font.pixelSize: 10
                        color: "#a6adc8"
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 8 }
    }
}
