import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    readonly property real uiScale: (settings && settings.playlistScale) ? settings.playlistScale : 1.0

    implicitWidth: Math.min(Math.round(310 * uiScale), parent ? Math.max(240, parent.width * 0.44) : 310)
    implicitHeight: Math.min(Math.round(310 * uiScale), parent ? Math.max(160, parent.height * 0.46) : 310)

    color: "transparent"
    radius: Math.round(14 * Math.min(1.2, uiScale))
    border.color: contrastManager.buttonBorder
    border.width: 1

    Behavior on implicitWidth {
        NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
    }
    Behavior on implicitHeight {
        NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
    }

    FrostedBackground {
        targetItem: root
        radius: root.radius
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.round(14 * root.uiScale)
        spacing: Math.round(8 * root.uiScale)

        // Header Row
        RowLayout {
            Layout.fillWidth: true
            spacing: Math.round(8 * root.uiScale)

            Kirigami.Icon {
                source: "qrc:/icons/lucide/list-music.svg"
                isMask: true
                implicitWidth: Math.round(18 * root.uiScale)
                implicitHeight: Math.round(18 * root.uiScale)
                color: contrastManager.accentColor
            }

            Text {
                text: "Tracks"
                font.pixelSize: Math.round(13 * root.uiScale)
                font.weight: Font.DemiBold
                color: contrastManager.primaryTextColor
            }

            Rectangle {
                implicitWidth: countText.implicitWidth + Math.round(12 * root.uiScale)
                implicitHeight: Math.round(18 * root.uiScale)
                radius: height / 2
                color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(0, 0, 0, 0.08)

                Text {
                    id: countText
                    anchors.centerIn: parent
                    text: trackModel.count.toString()
                    font.pixelSize: Math.round(10 * root.uiScale)
                    font.weight: Font.Medium
                    color: contrastManager.secondaryTextColor
                }
            }

            Item { Layout.fillWidth: true }

            // Rescan button
            ToolButton {
                id: rescanBtn
                implicitWidth: Math.round(32 * root.uiScale)
                implicitHeight: Math.round(32 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: "Rescan tracks"
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: 6
                    color: rescanBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent"
                    border.color: rescanBtn.activeFocus ? contrastManager.accentColor : "transparent"
                    border.width: 1
                }

                contentItem: Item {
                    implicitWidth: Math.round(15 * root.uiScale)
                    implicitHeight: Math.round(15 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/rotate-cw.svg"
                        isMask: true
                        width: Math.round(15 * root.uiScale)
                        height: Math.round(15 * root.uiScale)
                        color: rescanBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                    }
                }

                onClicked: appController.rescanLibrary()
            }
        }

        // Divider
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.10)
        }

        // Empty state
        Text {
            visible: trackModel.count === 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "No audio tracks found.\nSelect a folder in Settings."
            font.pixelSize: 12
            color: contrastManager.secondaryTextColor
            lineHeight: 1.4
        }

        // Track List
        ListView {
            id: trackListView
            visible: trackModel.count > 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: trackModel
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {
                active: trackListView.moving || trackListView.flicking
                width: 4
                contentItem: Rectangle {
                    radius: 2
                    color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.25) : Qt.rgba(0, 0, 0, 0.25)
                }
            }

            delegate: Rectangle {
                id: trackDelegate
                width: trackListView.width
                height: Math.round(34 * root.uiScale)
                radius: Math.round(6 * root.uiScale)

                readonly property bool isCurrent: index === playlistManager.currentIndex

                color: {
                    if (isCurrent) {
                        return Qt.rgba(0.54, 0.71, 0.98, contrastManager.isDark ? 0.22 : 0.30);
                    }
                    if (rowMouseArea.containsMouse) {
                        return contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.07);
                    }
                    return "transparent";
                }

                MouseArea {
                    id: rowMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: playlistManager.playTrackAtIndex(index)
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: Math.round(8 * root.uiScale)
                    anchors.rightMargin: Math.round(8 * root.uiScale)
                    spacing: Math.round(8 * root.uiScale)

                    // Playing indicator / Track number
                    Item {
                        implicitWidth: Math.round(16 * root.uiScale)
                        implicitHeight: Math.round(16 * root.uiScale)

                        Kirigami.Icon {
                            anchors.centerIn: parent
                            visible: trackDelegate.isCurrent
                            source: audioPlayer.isPlaying ? "qrc:/icons/lucide/play.svg" : "qrc:/icons/lucide/pause.svg"
                            isMask: true
                            implicitWidth: Math.round(12 * root.uiScale)
                            implicitHeight: Math.round(12 * root.uiScale)
                            color: contrastManager.accentColor
                        }

                        Text {
                            anchors.centerIn: parent
                            visible: !trackDelegate.isCurrent
                            text: (index + 1).toString()
                            font.pixelSize: Math.round(11 * root.uiScale)
                            color: contrastManager.secondaryTextColor
                        }
                    }

                    // Track title (elided, full row clickable)
                    Text {
                        id: titleText
                        Layout.fillWidth: true
                        text: model.title
                        font.pixelSize: Math.max(9, Math.round(12 * root.uiScale))
                        font.weight: trackDelegate.isCurrent ? Font.DemiBold : Font.Normal
                        color: trackDelegate.isCurrent ? contrastManager.accentColor : (rowMouseArea.containsMouse ? contrastManager.primaryTextColor : contrastManager.iconColor)
                        elide: Text.ElideRight
                    }
                }
            }
        }
    }
}
