import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import org.kde.kirigami as Kirigami

Item {
    id: root

    property string selectedPath: settings.musicDirectory || ""
    property var validationInfo: appController.validateDirectory(selectedPath)

    function updateValidation() {
        validationInfo = appController.validateDirectory(selectedPath);
    }

    Component.onCompleted: {
        updateValidation();
    }

    // Ambient wallpaper or gradient backdrop
    WallpaperView {
        anchors.fill: parent
    }

    // Dark frosted scrim to ensure pristine readability
    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0.08, 0.09, 0.14, 0.88)

        FrostedBackground {
            targetItem: parent
            radius: 0
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Choose Your Music Folder"
        currentFolder: selectedPath.length > 0 ? ("file://" + selectedPath) : ""
        onAccepted: {
            var path = selectedFolder.toString();
            if (path.indexOf("file://") === 0) {
                // Remove file:// prefix
                path = decodeURIComponent(path.substring(7));
            }
            root.selectedPath = path;
            root.updateValidation();
        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        anchors.margins: 20
        contentWidth: availableWidth
        clip: true

        ColumnLayout {
            width: Math.min(760, scrollView.availableWidth)
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            Item { Layout.preferredHeight: 10 }

            // --- HEADER ---
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 16

                Image {
                    source: "qrc:/icons/music-focus.png"
                    Layout.preferredWidth: 64
                    Layout.preferredHeight: 64
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                }

                ColumnLayout {
                    spacing: 4
                    Text {
                        text: "Welcome to Music Focus"
                        font.pixelSize: 24
                        font.weight: Font.Bold
                        color: "#ffffff"
                    }
                    Text {
                        text: "Distraction-free study music player with ambient wallpapers & floating widgets"
                        font.pixelSize: 13
                        color: "#a6adc8"
                    }
                }
            }

            // --- CARD 1: MUSIC FOLDER SELECTION ---
            Rectangle {
                Layout.fillWidth: true
                radius: 12
                color: Qt.rgba(0.12, 0.13, 0.19, 0.85)
                border.color: Qt.rgba(1, 1, 1, 0.14)
                border.width: 1
                implicitHeight: card1Layout.implicitHeight + 32

                FrostedBackground {
                    targetItem: parent
                    radius: 12
                }

                ColumnLayout {
                    id: card1Layout
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 14

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/folder-open.svg"
                            implicitWidth: 20
                            implicitHeight: 20
                            color: "#89b4fa"
                        }

                        Text {
                            text: "1. Select Your Music Folder"
                            font.pixelSize: 15
                            font.weight: Font.DemiBold
                            color: "#cdd6f4"
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "Music Focus plays audio files from your designated music directory. Supported formats include MP3, FLAC, OGG, and WAV. Files in subdirectories can also be organized into your playlists."
                        font.pixelSize: 12
                        color: "#bac2de"
                        wrapMode: Text.WordWrap
                        lineHeight: 1.3
                    }

                    // Directory selector strip
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 40
                            radius: 8
                            color: Qt.rgba(0.06, 0.07, 0.10, 0.8)
                            border.color: root.validationInfo && root.validationInfo.valid ? "#a6e3a1" : (root.selectedPath.length > 0 ? "#f38ba8" : Qt.rgba(1, 1, 1, 0.18))
                            border.width: 1

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 12
                                anchors.rightMargin: 12
                                spacing: 8

                                Text {
                                    id: pathText
                                    Layout.fillWidth: true
                                    text: root.selectedPath.length > 0 ? root.selectedPath : "No music folder chosen yet..."
                                    font.pixelSize: 12
                                    font.family: "monospace"
                                    color: root.selectedPath.length > 0 ? "#ffffff" : "#6c7086"
                                    elide: Text.ElideMiddle
                                }
                            }
                        }

                        Button {
                            id: chooseFolderBtn
                            text: "Choose Music Folder"
                            icon.source: "qrc:/icons/lucide/folder-open.svg"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                            implicitHeight: 40
                            highlighted: root.selectedPath.length === 0
                            onClicked: {
                                folderDialog.open();
                            }
                        }
                    }

                    // Live validation status banner
                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: statusLayout.implicitHeight + 16
                        radius: 8
                        visible: root.selectedPath.length > 0
                        color: {
                            if (!root.validationInfo || !root.validationInfo.valid) return Qt.rgba(0.95, 0.35, 0.45, 0.15);
                            return Qt.rgba(0.45, 0.85, 0.55, 0.15);
                        }
                        border.color: {
                            if (!root.validationInfo || !root.validationInfo.valid) return Qt.rgba(0.95, 0.35, 0.45, 0.5);
                            return Qt.rgba(0.45, 0.85, 0.55, 0.5);
                        }
                        border.width: 1

                        RowLayout {
                            id: statusLayout
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 12
                            spacing: 10

                            Kirigami.Icon {
                                source: (root.validationInfo && root.validationInfo.valid) ? "qrc:/icons/lucide/check.svg" : "qrc:/icons/lucide/triangle-alert.svg"
                                implicitWidth: 18
                                implicitHeight: 18
                                color: (root.validationInfo && root.validationInfo.valid) ? "#a6e3a1" : "#f38ba8"
                            }

                            Text {
                                Layout.fillWidth: true
                                text: root.validationInfo ? root.validationInfo.message : ""
                                font.pixelSize: 12
                                font.weight: Font.Medium
                                color: (root.validationInfo && root.validationInfo.valid) ? "#a6e3a1" : "#f38ba8"
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                }
            }

            // --- CARD 2: WALLPAPERS STRUCTURE GUIDE ---
            Rectangle {
                Layout.fillWidth: true
                radius: 12
                color: Qt.rgba(0.12, 0.13, 0.19, 0.85)
                border.color: Qt.rgba(1, 1, 1, 0.14)
                border.width: 1
                implicitHeight: card2Layout.implicitHeight + 32

                FrostedBackground {
                    targetItem: parent
                    radius: 12
                }

                ColumnLayout {
                    id: card2Layout
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 14

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/image.svg"
                            implicitWidth: 20
                            implicitHeight: 20
                            color: "#cba6f7"
                        }

                        Text {
                            text: "2. Ambient Wallpapers Organization"
                            font.pixelSize: 15
                            font.weight: Font.DemiBold
                            color: "#cdd6f4"
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        text: "Place your favorite background photos and video loops inside dedicated wallpaper subdirectories inside your music folder:"
                        font.pixelSize: 12
                        color: "#bac2de"
                        wrapMode: Text.WordWrap
                    }

                    // Directory Tree View
                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: treeText.implicitHeight + 20
                        radius: 8
                        color: Qt.rgba(0.06, 0.07, 0.10, 0.9)
                        border.color: Qt.rgba(1, 1, 1, 0.10)
                        border.width: 1

                        Text {
                            id: treeText
                            anchors.fill: parent
                            anchors.margins: 12
                            font.family: "monospace"
                            font.pixelSize: 12
                            lineHeight: 1.35
                            color: "#cdd6f4"
                            text: "Your Music Folder/\n├── Song 1.mp3\n├── Album/\n│   └── Song 2.flac\n└── Wallpapers/\n    ├── Still Wallpapers/\n    │   ├── wallpaper1.jpg\n    │   └── wallpaper2.png\n    └── Video Wallpapers/\n        ├── wallpaper1.mp4\n        └── wallpaper2.webm"
                        }
                    }

                    ColumnLayout {
                        spacing: 6
                        Layout.fillWidth: true

                        RowLayout {
                            spacing: 8
                            Kirigami.Icon {
                                source: "qrc:/icons/lucide/circle-alert.svg"
                                implicitWidth: 14
                                implicitHeight: 14
                                color: "#89b4fa"
                            }
                            Text {
                                Layout.fillWidth: true
                                text: "These wallpaper folders are created automatically when missing. Existing files and folders are never deleted or overwritten."
                                font.pixelSize: 11
                                color: "#a6adc8"
                                wrapMode: Text.WordWrap
                            }
                        }

                        RowLayout {
                            spacing: 8
                            Kirigami.Icon {
                                source: "qrc:/icons/lucide/disc.svg"
                                implicitWidth: 14
                                implicitHeight: 14
                                color: "#f9e2af"
                            }
                            Text {
                                Layout.fillWidth: true
                                text: "Supported still image formats: JPG, JPEG, PNG, WEBP, BMP.\nSupported video formats: MP4, MKV, WEBM, MOV, AVI."
                                font.pixelSize: 11
                                color: "#a6adc8"
                                wrapMode: Text.WordWrap
                            }
                        }

                        RowLayout {
                            spacing: 8
                            Kirigami.Icon {
                                source: "qrc:/icons/lucide/settings.svg"
                                implicitWidth: 14
                                implicitHeight: 14
                                color: "#b4befe"
                            }
                            Text {
                                Layout.fillWidth: true
                                text: "Wallpapers can be added anytime and rescanned through Settings or via shortcut W."
                                font.pixelSize: 11
                                color: "#a6adc8"
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                }
            }

            // --- BOTTOM ACTIONS ---
            RowLayout {
                Layout.fillWidth: true
                spacing: 16

                Item { Layout.fillWidth: true }

                Button {
                    id: continueBtn
                    text: "Continue to Player"
                    icon.source: "qrc:/icons/lucide/chevron-right.svg"
                    font.pixelSize: 13
                    font.weight: Font.Bold
                    implicitHeight: 44
                    implicitWidth: 200
                    enabled: root.validationInfo && root.validationInfo.valid
                    highlighted: enabled
                    onClicked: {
                        if (root.selectedPath.length > 0) {
                            appController.completeOnboarding(root.selectedPath);
                        }
                    }
                }
            }

            Item { Layout.preferredHeight: 20 }
        }
    }
}
