import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import org.kde.kirigami as Kirigami

Item {
    id: root

    property string missingPath: settings.musicDirectory || ""
    property string candidatePath: ""
    property var validationInfo: candidatePath.length > 0 ? appController.validateDirectory(candidatePath) : null

    // Ambient wallpaper or gradient backdrop
    WallpaperView {
        anchors.fill: parent
    }

    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0.08, 0.09, 0.14, 0.90)

        FrostedBackground {
            targetItem: parent
            radius: 0
        }
    }

    FolderDialog {
        id: chooseFolderDialog
        title: "Choose New Music Folder"
        currentFolder: candidatePath.length > 0 ? ("file://" + candidatePath) : ""
        onAccepted: {
            var path = selectedFolder.toString();
            if (path.indexOf("file://") === 0) {
                path = decodeURIComponent(path.substring(7));
            }
            root.candidatePath = path;
            root.validationInfo = appController.validateDirectory(path);
        }
    }

    ColumnLayout {
        width: Math.min(680, parent.width - 48)
        anchors.centerIn: parent
        spacing: 24

        // Warning Header
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 16

            Rectangle {
                implicitWidth: 56
                implicitHeight: 56
                radius: 28
                color: Qt.rgba(0.95, 0.40, 0.45, 0.20)
                border.color: "#f38ba8"
                border.width: 1

                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "qrc:/icons/lucide/triangle-alert.svg"
                    implicitWidth: 28
                    implicitHeight: 28
                    color: "#f38ba8"
                }
            }

            ColumnLayout {
                spacing: 4
                Text {
                    text: "Music Directory Unavailable"
                    font.pixelSize: 22
                    font.weight: Font.Bold
                    color: "#ffffff"
                }
                Text {
                    text: "Your previously configured music folder cannot be reached."
                    font.pixelSize: 13
                    color: "#bac2de"
                }
            }
        }

        // Details Card
        Rectangle {
            Layout.fillWidth: true
            radius: 12
            color: Qt.rgba(0.12, 0.13, 0.19, 0.90)
            border.color: Qt.rgba(1, 1, 1, 0.14)
            border.width: 1
            implicitHeight: detailsLayout.implicitHeight + 36

            FrostedBackground {
                targetItem: parent
                radius: 12
            }

            ColumnLayout {
                id: detailsLayout
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                Text {
                    Layout.fillWidth: true
                    text: "The directory below may be on a disconnected external storage drive, a moved folder, or unmounted network share. Your playback settings, playlists, and study tasks remain completely safe."
                    font.pixelSize: 12
                    color: "#a6adc8"
                    wrapMode: Text.WordWrap
                    lineHeight: 1.3
                }

                // Previously Saved Path Box
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Configured Music Path:"
                        font.pixelSize: 11
                        font.weight: Font.DemiBold
                        color: "#89b4fa"
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 38
                        radius: 6
                        color: Qt.rgba(0.06, 0.07, 0.10, 0.9)
                        border.color: "#f38ba8"
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 12
                            spacing: 8

                            Kirigami.Icon {
                                source: "qrc:/icons/lucide/folder-open.svg"
                                implicitWidth: 16
                                implicitHeight: 16
                                color: "#f38ba8"
                            }

                            Text {
                                Layout.fillWidth: true
                                text: root.missingPath.length > 0 ? root.missingPath : "None configured"
                                font.pixelSize: 12
                                font.family: "monospace"
                                color: "#ffffff"
                                elide: Text.ElideMiddle
                            }
                        }
                    }
                }

                // Candidate path display if user selected one
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6
                    visible: root.candidatePath.length > 0

                    Text {
                        text: "New Selection:"
                        font.pixelSize: 11
                        font.weight: Font.DemiBold
                        color: "#a6e3a1"
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 38
                        radius: 6
                        color: Qt.rgba(0.06, 0.07, 0.10, 0.9)
                        border.color: (root.validationInfo && root.validationInfo.valid) ? "#a6e3a1" : "#f38ba8"
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 12
                            anchors.rightMargin: 12
                            spacing: 8

                            Kirigami.Icon {
                                source: (root.validationInfo && root.validationInfo.valid) ? "qrc:/icons/lucide/check.svg" : "qrc:/icons/lucide/triangle-alert.svg"
                                implicitWidth: 16
                                implicitHeight: 16
                                color: (root.validationInfo && root.validationInfo.valid) ? "#a6e3a1" : "#f38ba8"
                            }

                            Text {
                                Layout.fillWidth: true
                                text: root.candidatePath
                                font.pixelSize: 12
                                font.family: "monospace"
                                color: "#ffffff"
                                elide: Text.ElideMiddle
                            }
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        text: root.validationInfo ? root.validationInfo.message : ""
                        font.pixelSize: 11
                        color: (root.validationInfo && root.validationInfo.valid) ? "#a6e3a1" : "#f38ba8"
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }

        // Action Buttons Row
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Button {
                text: "Retry Access"
                icon.source: "qrc:/icons/lucide/rotate-cw.svg"
                font.pixelSize: 12
                font.weight: Font.DemiBold
                implicitHeight: 40
                onClicked: {
                    if (!appController.retryDirectoryAccess()) {
                        // stays on recovery screen
                    }
                }
            }

            Button {
                text: "Select Another Folder"
                icon.source: "qrc:/icons/lucide/folder-open.svg"
                font.pixelSize: 12
                font.weight: Font.DemiBold
                implicitHeight: 40
                onClicked: {
                    chooseFolderDialog.open();
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Use New Folder"
                icon.source: "qrc:/icons/lucide/chevron-right.svg"
                font.pixelSize: 12
                font.weight: Font.Bold
                implicitHeight: 40
                highlighted: true
                visible: root.candidatePath.length > 0 && root.validationInfo && root.validationInfo.valid
                onClicked: {
                    appController.selectMusicDirectory(root.candidatePath);
                }
            }
        }
    }
}
