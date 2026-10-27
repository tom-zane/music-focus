import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import org.kde.kirigami as Kirigami

Popup {
    id: root

    anchors.centerIn: parent
    width: Math.min(460, parent.width * 0.9)
    height: Math.min(320, parent.height * 0.8)
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    background: Rectangle {
        color: Qt.rgba(0.1, 0.11, 0.16, 0.96)
        radius: 14
        border.color: Qt.rgba(1, 1, 1, 0.18)
        border.width: 1

        FrostedBackground {
            targetItem: parent
            radius: 14
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Study Music Directory"
        onAccepted: {
            appController.selectMusicDirectory(selectedFolder.toString())
            root.close()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Image {
            Layout.alignment: Qt.AlignHCenter
            source: "qrc:/icons/music-focus.png"
            Layout.preferredWidth: 64
            Layout.preferredHeight: 64
            fillMode: Image.PreserveAspectFit
            smooth: true
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Welcome to Music Focus"
            font.pixelSize: 18
            font.weight: Font.Bold
            color: "#ffffff"
        }

        Text {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            text: "To get started, please select your study music folder.\nSupported files: MP3, FLAC, OGG, WAV.\nPlace wallpaper images in a 'wallpapers' subfolder."
            font.pixelSize: 12
            color: "#a6adc8"
            lineHeight: 1.4
            wrapMode: Text.WordWrap
        }

        Item { Layout.fillHeight: true }

        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Select Music Folder"
            icon.source: "qrc:/icons/lucide/folder-open.svg"
            font.pixelSize: 13
            font.weight: Font.DemiBold
            highlighted: true
            implicitHeight: 38
            implicitWidth: 200
            onClicked: {
                if (appController.promptForMusicDirectory()) {
                    root.close()
                }
            }
        }
    }
}
