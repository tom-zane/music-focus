import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    readonly property real uiScale: (settings && settings.playbackStripScale) ? settings.playbackStripScale : (settings && settings.playbackScale ? settings.playbackScale : 1.0)

    implicitWidth: Math.min(Math.round(420 * uiScale), parent ? Math.max(320, parent.width * 0.55) : 420)
    implicitHeight: Math.round(82 * uiScale)

    color: "transparent"
    radius: Math.round(14 * Math.min(1.2, uiScale))
    border.color: activeFocus ? contrastManager.accentColor : contrastManager.buttonBorder
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
        id: mainLayout
        anchors.fill: parent
        anchors.leftMargin: Math.round(18 * root.uiScale)
        anchors.rightMargin: Math.round(18 * root.uiScale)
        anchors.topMargin: Math.round(10 * root.uiScale)
        anchors.bottomMargin: Math.round(10 * root.uiScale)
        spacing: Math.round(6 * root.uiScale)

        // Middle Row: Seek Slider + Time indicators
        RowLayout {
            Layout.fillWidth: true
            spacing: Math.round(8 * root.uiScale)

            Text {
                text: audioPlayer.formattedPosition
                font.pixelSize: Math.max(9, Math.round(10 * root.uiScale))
                font.family: "monospace"
                color: contrastManager.secondaryTextColor
            }

            Slider {
                id: seekSlider
                Layout.fillWidth: true
                from: 0
                to: Math.max(1, audioPlayer.duration)
                enabled: playlistManager.hasCurrentTrack && audioPlayer.duration > 0

                // Prevent fighting while dragging
                value: pressed ? value : audioPlayer.position

                onMoved: {
                    audioPlayer.seek(value)
                }

                background: Rectangle {
                    x: seekSlider.leftPadding
                    y: seekSlider.topPadding + seekSlider.availableHeight / 2 - height / 2
                    implicitWidth: 180
                    implicitHeight: Math.max(5, Math.round(6 * root.uiScale))
                    width: seekSlider.availableWidth
                    height: implicitHeight
                    radius: height / 2
                    color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.16) : Qt.rgba(0, 0, 0, 0.14)

                    Rectangle {
                        width: seekSlider.visualPosition * parent.width
                        height: parent.height
                        color: contrastManager.accentColor
                        radius: height / 2
                    }
                }

                handle: Rectangle {
                    x: seekSlider.leftPadding + seekSlider.visualPosition * (seekSlider.availableWidth - width)
                    y: seekSlider.topPadding + seekSlider.availableHeight / 2 - height / 2
                    implicitWidth: Math.round((seekSlider.hovered || seekSlider.pressed ? 14 : 11) * root.uiScale)
                    implicitHeight: Math.round((seekSlider.hovered || seekSlider.pressed ? 14 : 11) * root.uiScale)
                    radius: implicitWidth / 2
                    color: contrastManager.primaryTextColor
                    border.color: contrastManager.accentColor
                    border.width: 1.5

                    Behavior on implicitWidth {
                        NumberAnimation { duration: 150 }
                    }
                }
            }

            Text {
                text: audioPlayer.formattedDuration
                font.pixelSize: Math.max(9, Math.round(10 * root.uiScale))
                font.family: "monospace"
                color: contrastManager.secondaryTextColor
            }
        }

        // Bottom Row: Controls
        RowLayout {
            Layout.fillWidth: true
            spacing: Math.round(8 * root.uiScale)

            // Shuffle Button
            ToolButton {
                id: shuffleBtn
                implicitWidth: Math.round(34 * root.uiScale)
                implicitHeight: Math.round(34 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: "Shuffle: " + (playlistManager.shuffle ? "On" : "Off")
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: 8
                    color: playlistManager.shuffle ? Qt.rgba(0.54, 0.71, 0.98, 0.22) : (shuffleBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent")
                    border.color: shuffleBtn.activeFocus ? contrastManager.accentColor : "transparent"
                    border.width: 1
                }

                contentItem: Item {
                    implicitWidth: Math.round(16 * root.uiScale)
                    implicitHeight: Math.round(16 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/shuffle.svg"
                        isMask: true
                        width: Math.round(16 * root.uiScale)
                        height: Math.round(16 * root.uiScale)
                        color: playlistManager.shuffle ? contrastManager.accentColor : (shuffleBtn.hovered ? contrastManager.primaryTextColor : contrastManager.secondaryTextColor)
                    }
                }

                onClicked: playlistManager.toggleShuffle()
            }

            // Previous Button
            ToolButton {
                id: prevBtn
                implicitWidth: Math.round(34 * root.uiScale)
                implicitHeight: Math.round(34 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: "Previous Track (Left Arrow)"
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: 8
                    color: prevBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent"
                    border.color: prevBtn.activeFocus ? contrastManager.accentColor : "transparent"
                    border.width: 1
                }

                contentItem: Item {
                    implicitWidth: Math.round(16 * root.uiScale)
                    implicitHeight: Math.round(16 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/skip-back.svg"
                        isMask: true
                        width: Math.round(16 * root.uiScale)
                        height: Math.round(16 * root.uiScale)
                        color: prevBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                    }
                }

                onClicked: playlistManager.previousTrack()
            }

            // Play / Pause Button (Prominent, circular, 42x42 with 18x18 icon)
            Button {
                id: playPauseBtn
                implicitWidth: Math.round(42 * root.uiScale)
                implicitHeight: Math.round(42 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: audioPlayer.isPlaying ? "Pause (Space)" : "Play (Space)"
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: width / 2
                    color: playPauseBtn.hovered ? (contrastManager.isDark ? "#b4befe" : "#2563eb") : contrastManager.accentColor
                    border.color: playPauseBtn.activeFocus ? (contrastManager.isDark ? "#ffffff" : "#000000") : Qt.rgba(1, 1, 1, 0.25)
                    border.width: playPauseBtn.activeFocus ? 2 : 1

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }

                contentItem: Item {
                    implicitWidth: Math.round(18 * root.uiScale)
                    implicitHeight: Math.round(18 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        anchors.horizontalCenterOffset: audioPlayer.isPlaying ? 0 : 1
                        source: audioPlayer.isPlaying ? "qrc:/icons/lucide/pause.svg" : "qrc:/icons/lucide/play.svg"
                        isMask: true
                        width: Math.round(18 * root.uiScale)
                        height: Math.round(18 * root.uiScale)
                        color: contrastManager.isDark ? "#181926" : "#ffffff"
                    }
                }

                onClicked: audioPlayer.togglePlayPause()
            }

            // Next Button
            ToolButton {
                id: nextBtn
                implicitWidth: Math.round(34 * root.uiScale)
                implicitHeight: Math.round(34 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: "Next Track (Right Arrow)"
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: 8
                    color: nextBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent"
                    border.color: nextBtn.activeFocus ? contrastManager.accentColor : "transparent"
                    border.width: 1
                }

                contentItem: Item {
                    implicitWidth: Math.round(16 * root.uiScale)
                    implicitHeight: Math.round(16 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/skip-forward.svg"
                        isMask: true
                        width: Math.round(16 * root.uiScale)
                        height: Math.round(16 * root.uiScale)
                        color: nextBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                    }
                }

                onClicked: playlistManager.nextTrack()
            }

            // Repeat Button
            ToolButton {
                id: repeatBtn
                implicitWidth: Math.round(34 * root.uiScale)
                implicitHeight: Math.round(34 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: {
                    if (playlistManager.repeatMode === 0) return "Repeat: Off";
                    if (playlistManager.repeatMode === 1) return "Repeat: All";
                    return "Repeat: One";
                }
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: 8
                    color: playlistManager.repeatMode !== 0 ? Qt.rgba(0.54, 0.71, 0.98, 0.22) : (repeatBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent")
                    border.color: repeatBtn.activeFocus ? contrastManager.accentColor : "transparent"
                    border.width: 1
                }

                contentItem: Item {
                    implicitWidth: Math.round(16 * root.uiScale)
                    implicitHeight: Math.round(16 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: playlistManager.repeatMode === 2 ? "qrc:/icons/lucide/repeat-1.svg" : "qrc:/icons/lucide/repeat.svg"
                        isMask: true
                        width: Math.round(16 * root.uiScale)
                        height: Math.round(16 * root.uiScale)
                        color: {
                            if (playlistManager.repeatMode === 0) return (repeatBtn.hovered ? contrastManager.primaryTextColor : contrastManager.secondaryTextColor);
                            return contrastManager.accentColor;
                        }
                    }
                }

                onClicked: playlistManager.cycleRepeatMode()
            }

            Item { Layout.fillWidth: true }

            // Volume Section: Volume Icon + Refined Volume Slider
            RowLayout {
                spacing: Math.round(6 * root.uiScale)

                ToolButton {
                    id: muteBtn
                    implicitWidth: Math.round(32 * root.uiScale)
                    implicitHeight: Math.round(32 * root.uiScale)
                    focusPolicy: Qt.TabFocus
                    ToolTip.text: audioPlayer.muted ? "Unmute" : "Mute"
                    ToolTip.visible: hovered

                    background: Rectangle {
                        radius: 6
                        color: muteBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent"
                        border.color: muteBtn.activeFocus ? contrastManager.accentColor : "transparent"
                        border.width: 1
                    }

                    contentItem: Item {
                        implicitWidth: Math.round(15 * root.uiScale)
                        implicitHeight: Math.round(15 * root.uiScale)
                        Kirigami.Icon {
                            anchors.centerIn: parent
                            source: audioPlayer.muted || audioPlayer.volume === 0 ? "qrc:/icons/lucide/volume-x.svg" : "qrc:/icons/lucide/volume-2.svg"
                            isMask: true
                            width: Math.round(15 * root.uiScale)
                            height: Math.round(15 * root.uiScale)
                            color: muteBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                        }
                    }

                    onClicked: audioPlayer.setMuted(!audioPlayer.muted)
                }

                Slider {
                    id: volSlider
                    implicitWidth: Math.round(90 * root.uiScale)
                    focusPolicy: Qt.TabFocus
                    from: 0.0
                    to: 1.0
                    value: audioPlayer.volume

                    onMoved: {
                        if (audioPlayer.muted) {
                            audioPlayer.setMuted(false);
                        }
                        audioPlayer.setVolume(value);
                        settings.volume = value;
                    }

                    background: Rectangle {
                        x: volSlider.leftPadding
                        y: volSlider.topPadding + volSlider.availableHeight / 2 - height / 2
                        width: volSlider.availableWidth
                        height: Math.max(3, Math.round(4 * root.uiScale))
                        radius: height / 2
                        color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.16) : Qt.rgba(0, 0, 0, 0.14)

                        Rectangle {
                            width: volSlider.visualPosition * parent.width
                            height: parent.height
                            color: contrastManager.accentColor
                            radius: height / 2
                        }
                    }

                    handle: Rectangle {
                        x: volSlider.leftPadding + volSlider.visualPosition * (volSlider.availableWidth - width)
                        y: volSlider.topPadding + volSlider.availableHeight / 2 - height / 2
                        implicitWidth: Math.round(11 * root.uiScale)
                        implicitHeight: Math.round(11 * root.uiScale)
                        radius: width / 2
                        color: contrastManager.primaryTextColor
                        border.color: contrastManager.accentColor
                        border.width: 1
                    }
                }
            }
        }
    }
}
