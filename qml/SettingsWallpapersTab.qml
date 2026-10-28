import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

ScrollView {
    id: root
    clip: true
    contentWidth: availableWidth
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    rightPadding: (ScrollBar.vertical && ScrollBar.vertical.visible) ? (ScrollBar.vertical.width + 12) : 12
    leftPadding: 6
    topPadding: 4
    bottomPadding: 16

    ColumnLayout {
        width: root.availableWidth
        spacing: 16

        // 1. Wallpaper Engine Mode Selector Cards
        SettingsCard {
            title: "Active Wallpaper Engine"
            description: "Choose your study backdrop rendering system"
            iconSource: "qrc:/icons/lucide/image.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                // Card 1: Picture
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 64
                    radius: 8
                    color: (settings.wallpaperMode === 1) ? Qt.rgba(0.54, 0.71, 0.98, 0.18) : (picWallHover.hovered ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.03))
                    border.color: (settings.wallpaperMode === 1) ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                    border.width: (settings.wallpaperMode === 1) ? 1.5 : 1

                    HoverHandler { id: picWallHover }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: settings.wallpaperMode = 1
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/image.svg"
                            isMask: true
                            implicitWidth: 22
                            implicitHeight: 22
                            color: (settings.wallpaperMode === 1) ? "#89b4fa" : "#a6adc8"
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1
                            RowLayout {
                                spacing: 6
                                Text {
                                    text: "Picture"
                                    font.pixelSize: 13
                                    font.weight: Font.Bold
                                    color: (settings.wallpaperMode === 1) ? "#89b4fa" : "#ffffff"
                                }
                                Rectangle {
                                    visible: settings.wallpaperMode === 1
                                    implicitWidth: 40
                                    implicitHeight: 15
                                    radius: 3
                                    color: "#89b4fa"
                                    Text {
                                        anchors.centerIn: parent
                                        text: "ACTIVE"
                                        font.pixelSize: 8
                                        font.weight: Font.Bold
                                        color: "#11111b"
                                    }
                                }
                            }
                            Text {
                                text: "Still photo collection"
                                font.pixelSize: 10
                                color: "#9399b2"
                            }
                        }
                    }
                }

                // Card 2: Generated
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 64
                    radius: 8
                    color: (settings.wallpaperMode === 0) ? Qt.rgba(0.54, 0.71, 0.98, 0.18) : (genWallHover.hovered ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.03))
                    border.color: (settings.wallpaperMode === 0) ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                    border.width: (settings.wallpaperMode === 0) ? 1.5 : 1

                    HoverHandler { id: genWallHover }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: settings.wallpaperMode = 0
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/sparkles.svg"
                            isMask: true
                            implicitWidth: 22
                            implicitHeight: 22
                            color: (settings.wallpaperMode === 0) ? "#89b4fa" : "#a6adc8"
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1
                            RowLayout {
                                spacing: 6
                                Text {
                                    text: "Generated"
                                    font.pixelSize: 13
                                    font.weight: Font.Bold
                                    color: (settings.wallpaperMode === 0) ? "#89b4fa" : "#ffffff"
                                }
                                Rectangle {
                                    visible: settings.wallpaperMode === 0
                                    implicitWidth: 40
                                    implicitHeight: 15
                                    radius: 3
                                    color: "#89b4fa"
                                    Text {
                                        anchors.centerIn: parent
                                        text: "ACTIVE"
                                        font.pixelSize: 8
                                        font.weight: Font.Bold
                                        color: "#11111b"
                                    }
                                }
                            }
                            Text {
                                text: "Pastel study gradients"
                                font.pixelSize: 10
                                color: "#9399b2"
                            }
                        }
                    }
                }

                // Card 3: Video
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 64
                    radius: 8
                    color: (settings.wallpaperMode === 2) ? Qt.rgba(0.54, 0.71, 0.98, 0.18) : (vidWallHover.hovered ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.03))
                    border.color: (settings.wallpaperMode === 2) ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                    border.width: (settings.wallpaperMode === 2) ? 1.5 : 1

                    HoverHandler { id: vidWallHover }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: settings.wallpaperMode = 2
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/video.svg"
                            isMask: true
                            implicitWidth: 22
                            implicitHeight: 22
                            color: (settings.wallpaperMode === 2) ? "#89b4fa" : "#a6adc8"
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1
                            RowLayout {
                                spacing: 6
                                Text {
                                    text: "Video"
                                    font.pixelSize: 13
                                    font.weight: Font.Bold
                                    color: (settings.wallpaperMode === 2) ? "#89b4fa" : "#ffffff"
                                }
                                Rectangle {
                                    visible: settings.wallpaperMode === 2
                                    implicitWidth: 40
                                    implicitHeight: 15
                                    radius: 3
                                    color: "#89b4fa"
                                    Text {
                                        anchors.centerIn: parent
                                        text: "ACTIVE"
                                        font.pixelSize: 8
                                        font.weight: Font.Bold
                                        color: "#11111b"
                                    }
                                }
                            }
                            Text {
                                text: "Ambient looping video"
                                font.pixelSize: 10
                                color: "#9399b2"
                            }
                        }
                    }
                }
            }
        }

        // =====================================================================
        // MODE 1: PICTURE MODE (PRESERVED CAROUSEL)
        // =====================================================================
        SettingsCard {
            visible: settings.wallpaperMode === 1
            title: "Still Wallpapers Gallery"
            description: "Browse and select from your still photo collection"
            iconSource: "qrc:/icons/lucide/image.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Rectangle {
                    implicitWidth: stillCountBadge.implicitWidth + 12
                    implicitHeight: 20
                    radius: 10
                    color: Qt.rgba(1, 1, 1, 0.08)
                    Text {
                        id: stillCountBadge
                        anchors.centerIn: parent
                        text: wallpaperManager.wallpapersCount + " images available"
                        font.pixelSize: 11
                        font.weight: Font.Medium
                        color: "#cdd6f4"
                    }
                }

                Item { Layout.fillWidth: true }

                SettingsButton {
                    text: "Rescan"
                    iconSource: "qrc:/icons/lucide/rotate-cw.svg"
                    onClicked: appController.rescanWallpapers()
                }
            }

            // --- THE APPROVED WALLPAPER CAROUSEL COMPONENT ---
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                visible: wallpaperManager.wallpapersCount > 0

                // Left Chevron Button
                ToolButton {
                    id: stillPrevBtn
                    Layout.alignment: Qt.AlignVCenter
                    implicitWidth: 38
                    implicitHeight: 38
                    enabled: stillGalleryView.contentX > 0.5
                    focusPolicy: Qt.TabFocus
                    ToolTip.text: "Scroll left"
                    ToolTip.visible: hovered

                    background: Rectangle {
                        radius: 19
                        color: stillPrevBtn.enabled ? (stillPrevBtn.hovered ? Qt.rgba(1, 1, 1, 0.18) : Qt.rgba(1, 1, 1, 0.08)) : Qt.rgba(1, 1, 1, 0.02)
                        border.color: stillPrevBtn.enabled ? Qt.rgba(1, 1, 1, 0.15) : "transparent"
                        border.width: 1
                    }

                    contentItem: Item {
                        implicitWidth: 18
                        implicitHeight: 18
                        Kirigami.Icon {
                            anchors.centerIn: parent
                            source: "qrc:/icons/lucide/chevron-left.svg"
                            isMask: true
                            width: 18
                            height: 18
                            color: stillPrevBtn.enabled ? (stillPrevBtn.hovered ? "#ffffff" : "#cdd6f4") : "#45475a"
                        }
                    }

                    onClicked: {
                        stillGalleryView.contentX = Math.max(0, stillGalleryView.contentX - 182);
                    }
                }

                // Cards ListView
                ListView {
                    id: stillGalleryView
                    Layout.fillWidth: true
                    implicitHeight: 124
                    orientation: ListView.Horizontal
                    spacing: 12
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    model: wallpaperManager.availableWallpapers

                    delegate: Rectangle {
                        id: stillCard
                        width: 170
                        height: 120
                        radius: 8
                        clip: true
                        readonly property bool isSelected: (settings.wallpaperMode === 1) && (modelData === wallpaperManager.currentWallpaperName)
                        color: Qt.rgba(0.12, 0.13, 0.18, 0.95)
                        border.color: isSelected ? "#89b4fa" : (cardHover.hovered ? Qt.rgba(1, 1, 1, 0.35) : Qt.rgba(1, 1, 1, 0.12))
                        border.width: isSelected ? 2 : 1

                        Image {
                            anchors.fill: parent
                            anchors.margins: stillCard.isSelected ? 2 : 1
                            source: wallpaperManager.wallpaperUrlFor(modelData)
                            fillMode: Image.PreserveAspectCrop
                            asynchronous: true
                            cache: true
                        }

                        // Dark gradient scrim at bottom
                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 38
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "transparent" }
                                GradientStop { position: 1.0; color: Qt.rgba(0.05, 0.06, 0.09, 0.92) }
                            }
                        }

                        // Filename label
                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.margins: 6
                            text: modelData
                            font.pixelSize: 10
                            font.weight: stillCard.isSelected ? Font.Bold : Font.Normal
                            color: stillCard.isSelected ? "#89b4fa" : "#ffffff"
                            elide: Text.ElideMiddle
                        }

                        // Selected Active indicator badge
                        Rectangle {
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.margins: 6
                            visible: stillCard.isSelected
                            implicitWidth: activeLabel.implicitWidth + 8
                            implicitHeight: 18
                            radius: 4
                            color: "#89b4fa"

                            Text {
                                id: activeLabel
                                anchors.centerIn: parent
                                text: "ACTIVE"
                                font.pixelSize: 9
                                font.weight: Font.Bold
                                color: "#11111b"
                            }
                        }

                        HoverHandler { id: cardHover; cursorShape: Qt.PointingHandCursor }
                        TapHandler {
                            onTapped: {
                                settings.wallpaperMode = 1;
                                wallpaperManager.selectWallpaperByName(modelData);
                            }
                        }
                    }
                }

                // Right Chevron Button
                ToolButton {
                    id: stillNextBtn
                    Layout.alignment: Qt.AlignVCenter
                    implicitWidth: 38
                    implicitHeight: 38
                    enabled: (stillGalleryView.contentWidth - stillGalleryView.contentX - stillGalleryView.width) > 0.5
                    focusPolicy: Qt.TabFocus
                    ToolTip.text: "Scroll right"
                    ToolTip.visible: hovered

                    background: Rectangle {
                        radius: 19
                        color: stillNextBtn.enabled ? (stillNextBtn.hovered ? Qt.rgba(1, 1, 1, 0.18) : Qt.rgba(1, 1, 1, 0.08)) : Qt.rgba(1, 1, 1, 0.02)
                        border.color: stillNextBtn.enabled ? Qt.rgba(1, 1, 1, 0.15) : "transparent"
                        border.width: 1
                    }

                    contentItem: Item {
                        implicitWidth: 18
                        implicitHeight: 18
                        Kirigami.Icon {
                            anchors.centerIn: parent
                            source: "qrc:/icons/lucide/chevron-right.svg"
                            isMask: true
                            width: 18
                            height: 18
                            color: stillNextBtn.enabled ? (stillNextBtn.hovered ? "#ffffff" : "#cdd6f4") : "#45475a"
                        }
                    }

                    onClicked: {
                        var maxScroll = Math.max(0, stillGalleryView.contentWidth - stillGalleryView.width);
                        stillGalleryView.contentX = Math.min(maxScroll, stillGalleryView.contentX + 182);
                    }
                }
            }

            // Visible Horizontal Scrollbar Below Cards
            ScrollBar {
                id: stillHScrollBar
                Layout.fillWidth: true
                Layout.leftMargin: 40
                Layout.rightMargin: 40
                orientation: Qt.Horizontal
                visible: (wallpaperManager.wallpapersCount > 0) && (stillGalleryView.contentWidth > stillGalleryView.width)
                policy: ScrollBar.AlwaysOn
                size: Math.min(1.0, stillGalleryView.width / Math.max(1, stillGalleryView.contentWidth))
                position: Math.max(0, stillGalleryView.contentX / Math.max(1, stillGalleryView.contentWidth))

                contentItem: Rectangle {
                    implicitHeight: 6
                    radius: 3
                    color: stillHScrollBar.pressed ? "#89b4fa" : (stillHScrollBar.hovered ? "#b4befe" : Qt.rgba(1, 1, 1, 0.35))
                }
                background: Rectangle {
                    implicitHeight: 6
                    radius: 3
                    color: Qt.rgba(1, 1, 1, 0.08)
                }

                Connections {
                    target: stillGalleryView
                    function onContentXChanged() {
                        if (!stillHScrollBar.pressed) {
                            var maxW = Math.max(1, stillGalleryView.contentWidth);
                            stillHScrollBar.position = Math.max(0, Math.min(1.0 - stillHScrollBar.size, stillGalleryView.contentX / maxW));
                        }
                    }
                    function onContentWidthChanged() {
                        if (!stillHScrollBar.pressed) {
                            var maxW = Math.max(1, stillGalleryView.contentWidth);
                            stillHScrollBar.position = Math.max(0, Math.min(1.0 - stillHScrollBar.size, stillGalleryView.contentX / maxW));
                        }
                    }
                }

                onPositionChanged: {
                    if (pressed) {
                        stillGalleryView.contentX = position * stillGalleryView.contentWidth;
                    }
                }
            }

            // Empty State Card
            SettingsInfoBox {
                visible: wallpaperManager.wallpapersCount === 0
                isWarning: true
                text: "No images found in Wallpapers/Still Wallpapers/. Add JPG, PNG, WEBP, or BMP images to your music folder to populate this gallery."
                iconSource: "qrc:/icons/lucide/image-off.svg"
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            // Rotation Controls
            SettingsToggle {
                title: "Automatic rotation"
                description: "Smoothly cycle through still wallpapers without immediate repeats"
                checked: settings.stillAutoRotate
                onToggled: function(c) { settings.stillAutoRotate = c; }
            }

            SettingsSlider {
                title: "Rotate Interval"
                description: "Time between automatic wallpaper transitions"
                from: 1
                to: 60
                stepSize: 1
                value: settings.rotateIntervalMinutes
                valueText: Math.round(value) + " min"
                enabled: settings.stillAutoRotate
                onMoved: function(v) { settings.rotateIntervalMinutes = Math.round(v); }
            }
        }

        // =====================================================================
        // MODE 0: GENERATED PASTEL GRADIENTS
        // =====================================================================
        SettingsCard {
            visible: settings.wallpaperMode === 0
            title: "Deterministic Gradient Configuration"
            description: "Mathematical color meshes and harmonic gradients generated in real time"
            iconSource: "qrc:/icons/lucide/sparkles.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Gradient Style:"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: "#cdd6f4"
                    Layout.preferredWidth: 120
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: ["Linear Pastel", "Radial Pastel", "Angled Pastel", "Soft Mesh"]
                    currentIndex: settings.gradientStyle
                    focusPolicy: Qt.TabFocus
                    onActivated: settings.gradientStyle = index
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Gradient Seed:"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: "#cdd6f4"
                    Layout.preferredWidth: 120
                }

                Text {
                    text: settings.gradientSeed.toString()
                    font.pixelSize: 13
                    font.weight: Font.Bold
                    font.family: "monospace"
                    color: "#89b4fa"
                    Layout.fillWidth: true
                }

                SettingsButton {
                    text: "Regenerate Seed"
                    iconSource: "qrc:/icons/lucide/rotate-cw.svg"
                    onClicked: wallpaperManager.regenerateGradientSeed()
                }
            }

            SettingsInfoBox {
                text: "Deterministic seeds ensure the exact same gradient colors are reproduced across restarts and window resizes."
                iconSource: "qrc:/icons/lucide/info.svg"
            }
        }

        // =====================================================================
        // MODE 2: VIDEO WALLPAPERS (PRESERVED CAROUSEL)
        // =====================================================================
        SettingsCard {
            visible: settings.wallpaperMode === 2
            title: "Video Wallpapers Gallery"
            description: "Ambient looping video backgrounds muted for focus"
            iconSource: "qrc:/icons/lucide/video.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Rectangle {
                    implicitWidth: videoCountBadge.implicitWidth + 12
                    implicitHeight: 20
                    radius: 10
                    color: Qt.rgba(1, 1, 1, 0.08)
                    Text {
                        id: videoCountBadge
                        anchors.centerIn: parent
                        text: wallpaperManager.videosCount + " videos available"
                        font.pixelSize: 11
                        font.weight: Font.Medium
                        color: "#cdd6f4"
                    }
                }

                Item { Layout.fillWidth: true }

                SettingsButton {
                    text: "Rescan"
                    iconSource: "qrc:/icons/lucide/rotate-cw.svg"
                    onClicked: appController.rescanWallpapers()
                }
            }

            // --- VIDEO CAROUSEL COMPONENT ---
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                visible: wallpaperManager.videosCount > 0

                // Left Chevron Button
                ToolButton {
                    id: vidPrevBtn
                    Layout.alignment: Qt.AlignVCenter
                    implicitWidth: 38
                    implicitHeight: 38
                    enabled: videoGalleryView.contentX > 0.5
                    focusPolicy: Qt.TabFocus
                    ToolTip.text: "Scroll left"
                    ToolTip.visible: hovered

                    background: Rectangle {
                        radius: 19
                        color: vidPrevBtn.enabled ? (vidPrevBtn.hovered ? Qt.rgba(1, 1, 1, 0.18) : Qt.rgba(1, 1, 1, 0.08)) : Qt.rgba(1, 1, 1, 0.02)
                        border.color: vidPrevBtn.enabled ? Qt.rgba(1, 1, 1, 0.15) : "transparent"
                        border.width: 1
                    }

                    contentItem: Item {
                        implicitWidth: 18
                        implicitHeight: 18
                        Kirigami.Icon {
                            anchors.centerIn: parent
                            source: "qrc:/icons/lucide/chevron-left.svg"
                            isMask: true
                            width: 18
                            height: 18
                            color: vidPrevBtn.enabled ? (vidPrevBtn.hovered ? "#ffffff" : "#cdd6f4") : "#45475a"
                        }
                    }

                    onClicked: {
                        videoGalleryView.contentX = Math.max(0, videoGalleryView.contentX - 182);
                    }
                }

                // Video Cards ListView
                ListView {
                    id: videoGalleryView
                    Layout.fillWidth: true
                    implicitHeight: 124
                    orientation: ListView.Horizontal
                    spacing: 12
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    model: wallpaperManager.availableVideos

                    delegate: Rectangle {
                        id: videoCard
                        width: 170
                        height: 120
                        radius: 8
                        clip: true
                        readonly property bool isSelected: (settings.wallpaperMode === 2) && (modelData === wallpaperManager.currentVideoName)
                        color: Qt.rgba(0.12, 0.13, 0.18, 0.95)
                        border.color: isSelected ? "#a6e3a1" : (vCardHover.hovered ? Qt.rgba(1, 1, 1, 0.35) : Qt.rgba(1, 1, 1, 0.12))
                        border.width: isSelected ? 2 : 1

                        Image {
                            id: vThumbImg
                            anchors.fill: parent
                            anchors.margins: videoCard.isSelected ? 2 : 1
                            source: wallpaperManager.videoThumbnailUrl(modelData)
                            fillMode: Image.PreserveAspectCrop
                            asynchronous: true
                            cache: true
                            visible: status === Image.Ready

                            Connections {
                                target: wallpaperManager
                                function onVideoThumbnailReady(fn, url) {
                                    if (fn === modelData && url.toString() !== "") {
                                        vThumbImg.source = url;
                                    }
                                }
                            }
                        }

                        // Fallback when thumbnail is generating
                        Item {
                            anchors.fill: parent
                            visible: vThumbImg.status !== Image.Ready
                            Rectangle {
                                anchors.fill: parent
                                color: Qt.rgba(0.16, 0.17, 0.24, 0.96)
                            }
                            Kirigami.Icon {
                                anchors.centerIn: parent
                                source: "qrc:/icons/lucide/video.svg"
                                implicitWidth: 32
                                implicitHeight: 32
                                color: "#89b4fa"
                            }
                        }

                        // Dark gradient scrim at bottom
                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 38
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "transparent" }
                                GradientStop { position: 1.0; color: Qt.rgba(0.05, 0.06, 0.09, 0.92) }
                            }
                        }

                        // Filename label
                        Text {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.margins: 6
                            text: modelData
                            font.pixelSize: 10
                            font.weight: videoCard.isSelected ? Font.Bold : Font.Normal
                            color: videoCard.isSelected ? "#a6e3a1" : "#ffffff"
                            elide: Text.ElideMiddle
                        }

                        // Selected Active indicator badge
                        Rectangle {
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.margins: 6
                            visible: videoCard.isSelected
                            implicitWidth: vActiveLabel.implicitWidth + 8
                            implicitHeight: 18
                            radius: 4
                            color: "#a6e3a1"

                            Text {
                                id: vActiveLabel
                                anchors.centerIn: parent
                                text: "ACTIVE"
                                font.pixelSize: 9
                                font.weight: Font.Bold
                                color: "#11111b"
                            }
                        }

                        HoverHandler { id: vCardHover; cursorShape: Qt.PointingHandCursor }
                        TapHandler {
                            onTapped: {
                                settings.wallpaperMode = 2;
                                wallpaperManager.selectVideoByName(modelData);
                            }
                        }
                    }
                }

                // Right Chevron Button
                ToolButton {
                    id: vidNextBtn
                    Layout.alignment: Qt.AlignVCenter
                    implicitWidth: 38
                    implicitHeight: 38
                    enabled: (videoGalleryView.contentWidth - videoGalleryView.contentX - videoGalleryView.width) > 0.5
                    focusPolicy: Qt.TabFocus
                    ToolTip.text: "Scroll right"
                    ToolTip.visible: hovered

                    background: Rectangle {
                        radius: 19
                        color: vidNextBtn.enabled ? (vidNextBtn.hovered ? Qt.rgba(1, 1, 1, 0.18) : Qt.rgba(1, 1, 1, 0.08)) : Qt.rgba(1, 1, 1, 0.02)
                        border.color: vidNextBtn.enabled ? Qt.rgba(1, 1, 1, 0.15) : "transparent"
                        border.width: 1
                    }

                    contentItem: Item {
                        implicitWidth: 18
                        implicitHeight: 18
                        Kirigami.Icon {
                            anchors.centerIn: parent
                            source: "qrc:/icons/lucide/chevron-right.svg"
                            isMask: true
                            width: 18
                            height: 18
                            color: vidNextBtn.enabled ? (vidNextBtn.hovered ? "#ffffff" : "#cdd6f4") : "#45475a"
                        }
                    }

                    onClicked: {
                        var maxScroll = Math.max(0, videoGalleryView.contentWidth - videoGalleryView.width);
                        videoGalleryView.contentX = Math.min(maxScroll, videoGalleryView.contentX + 182);
                    }
                }
            }

            // Visible Horizontal Scrollbar Below Video Cards
            ScrollBar {
                id: vidHScrollBar
                Layout.fillWidth: true
                Layout.leftMargin: 40
                Layout.rightMargin: 40
                orientation: Qt.Horizontal
                visible: (wallpaperManager.videosCount > 0) && (videoGalleryView.contentWidth > videoGalleryView.width)
                policy: ScrollBar.AlwaysOn
                size: Math.min(1.0, videoGalleryView.width / Math.max(1, videoGalleryView.contentWidth))
                position: Math.max(0, videoGalleryView.contentX / Math.max(1, videoGalleryView.contentWidth))

                contentItem: Rectangle {
                    implicitHeight: 6
                    radius: 3
                    color: vidHScrollBar.pressed ? "#89b4fa" : (vidHScrollBar.hovered ? "#b4befe" : Qt.rgba(1, 1, 1, 0.35))
                }
                background: Rectangle {
                    implicitHeight: 6
                    radius: 3
                    color: Qt.rgba(1, 1, 1, 0.08)
                }

                Connections {
                    target: videoGalleryView
                    function onContentXChanged() {
                        if (!vidHScrollBar.pressed) {
                            var maxW = Math.max(1, videoGalleryView.contentWidth);
                            vidHScrollBar.position = Math.max(0, Math.min(1.0 - vidHScrollBar.size, videoGalleryView.contentX / maxW));
                        }
                    }
                    function onContentWidthChanged() {
                        if (!vidHScrollBar.pressed) {
                            var maxW = Math.max(1, videoGalleryView.contentWidth);
                            vidHScrollBar.position = Math.max(0, Math.min(1.0 - vidHScrollBar.size, videoGalleryView.contentX / maxW));
                        }
                    }
                }

                onPositionChanged: {
                    if (pressed) {
                        videoGalleryView.contentX = position * videoGalleryView.contentWidth;
                    }
                }
            }

            // Empty State Card
            SettingsInfoBox {
                visible: wallpaperManager.videosCount === 0
                isWarning: true
                text: "No videos found in Wallpapers/Video Wallpapers/. Place MP4, MKV, WEBM, MOV, or AVI files in your music folder's Video Wallpapers directory."
                iconSource: "qrc:/icons/lucide/video-off.svg"
            }

            SettingsInfoBox {
                text: "Video wallpapers loop continuously and remain permanently muted in the background for distraction-free study."
                iconSource: "qrc:/icons/lucide/info.svg"
            }
        }

        // =====================================================================
        // COMMON: WALLPAPER DIMMING
        // =====================================================================
        SettingsCard {
            title: "Wallpaper Dimming"
            description: "Dims the active wallpaper underneath widgets to boost contrast and focus"
            iconSource: "qrc:/icons/lucide/sliders-horizontal.svg"

            SettingsSlider {
                title: "Dimming Level"
                description: "Translucent dark layer applied over the active wallpaper"
                from: 0.00
                to: 0.90
                stepSize: 0.05
                value: settings.dimmingOpacity
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.dimmingOpacity = Math.round(v * 100) / 100; }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 8 }
    }
}
