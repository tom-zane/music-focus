import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import org.kde.kirigami as Kirigami

Popup {
    id: root

    // Centralized design tokens
    QtObject {
        id: tokens
        readonly property int fontDialogTitle: 17
        readonly property int fontBody: 12
        readonly property int dialogPadding: 20
        readonly property int cornerRadius: 12
        readonly property int sidebarWidth: 205
        readonly property int buttonHeight: 34
    }

    // Independent dialog dimensions with bounds matching backend tests
    width: Math.min(settings ? settings.settingsDialogWidth : 960, (parent ? parent.width - 32 : 960))
    height: Math.min(settings ? settings.settingsDialogHeight : 680, (parent ? parent.height - 32 : 680))
    anchors.centerIn: Overlay.overlay
    modal: true
    focus: true
    dim: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        color: Qt.rgba(0.08, 0.09, 0.14, 0.96)
        radius: tokens.cornerRadius
        border.color: Qt.rgba(1, 1, 1, 0.12)
        border.width: 1

        FrostedBackground {
            targetItem: parent
            radius: parent.radius
        }
    }

    Overlay.modal: Rectangle {
        color: Qt.rgba(0, 0, 0, 0.55)
        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }

    onOpened: {
        window.notifyActivity();
    }

    function openTab(tabIndex) {
        if (tabIndex >= 0 && tabIndex < categoryModel.count) {
            categoryList.currentIndex = tabIndex;
        }
        open();
    }

    function setScrollPosition(ratio) {
        var page = contentStack.children[categoryList.currentIndex];
        if (page) {
            if (page.contentY !== undefined && page.contentHeight !== undefined) {
                var maxY = Math.max(0, page.contentHeight - page.height);
                page.contentY = ratio * maxY;
            } else if (page.ScrollBar && page.ScrollBar.vertical) {
                page.ScrollBar.vertical.position = ratio * (1.0 - page.ScrollBar.vertical.size);
            }
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Select Solid Background Color"
        selectedColor: settings ? settings.solidColor : "#181926"
        onAccepted: {
            if (settings) {
                settings.solidColor = selectedColor;
            }
        }
    }

    ListModel {
        id: categoryModel
        ListElement { name: "General"; iconName: "qrc:/icons/lucide/settings.svg"; desc: "Directory & Inactivity" }
        ListElement { name: "Playback"; iconName: "qrc:/icons/lucide/play.svg"; desc: "Startup & Persistence" }
        ListElement { name: "Appearance"; iconName: "qrc:/icons/lucide/palette.svg"; desc: "Transparency & Contrast" }
        ListElement { name: "Wallpapers"; iconName: "qrc:/icons/lucide/image.svg"; desc: "Backdrop & Dimming" }
        ListElement { name: "Visualizer"; iconName: "qrc:/icons/lucide/activity.svg"; desc: "Reactivity & Themes" }
        ListElement { name: "Widgets"; iconName: "qrc:/icons/lucide/layout-grid.svg"; desc: "Desktop Overlays & Tasks" }
        ListElement { name: "Pomodoro"; iconName: "qrc:/icons/lucide/timer.svg"; desc: "Timer Presets & Sounds" }
        ListElement { name: "History"; iconName: "qrc:/icons/lucide/activity.svg"; desc: "Focus Stats & Calendar" }
        ListElement { name: "Help & Shortcuts"; iconName: "qrc:/icons/lucide/help-circle.svg"; desc: "Hotkeys & Guide" }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: tokens.dialogPadding
        spacing: 12

        // Top Header Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
                implicitWidth: 32
                implicitHeight: 32
                radius: 8
                color: Qt.rgba(0.54, 0.71, 0.98, 0.15)
                border.color: Qt.rgba(0.54, 0.71, 0.98, 0.3)
                border.width: 1

                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "qrc:/icons/lucide/settings.svg"
                    isMask: true
                    implicitWidth: 18
                    implicitHeight: 18
                    color: "#89b4fa"
                }
            }

            ColumnLayout {
                spacing: 1
                Text {
                    text: "Settings"
                    font.pixelSize: tokens.fontDialogTitle
                    font.weight: Font.Bold
                    color: "#ffffff"
                }
                Text {
                    text: "Distraction-free ambient music configuration"
                    font.pixelSize: 11
                    color: "#9399b2"
                }
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                id: closeBtn
                implicitWidth: 32
                implicitHeight: 32
                focusPolicy: Qt.TabFocus
                ToolTip.text: "Close Settings (Esc)"
                ToolTip.visible: hovered
                onClicked: root.close()

                background: Rectangle {
                    radius: 16
                    color: closeBtn.hovered ? Qt.rgba(1, 1, 1, 0.14) : "transparent"
                    border.color: closeBtn.activeFocus ? "#89b4fa" : "transparent"
                    border.width: 1
                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                contentItem: Item {
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/x.svg"
                        isMask: true
                        implicitWidth: 16
                        implicitHeight: 16
                        color: closeBtn.hovered ? "#ffffff" : "#a6adc8"
                    }
                }
            }
        }

        // Horizontal Separator
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Qt.rgba(1, 1, 1, 0.08)
        }

        // Main Two-Column Layout (Sidebar + Content Stack)
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 16

            // Left Navigation Sidebar
            Rectangle {
                Layout.preferredWidth: tokens.sidebarWidth
                Layout.fillHeight: true
                color: Qt.rgba(0, 0, 0, 0.22)
                radius: 10
                border.color: Qt.rgba(1, 1, 1, 0.06)
                border.width: 1

                ListView {
                    id: categoryList
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 4
                    model: categoryModel
                    currentIndex: 0
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        width: categoryList.width
                        height: 40
                        radius: 8
                        readonly property bool isSelected: index === categoryList.currentIndex

                        color: {
                            if (isSelected) return Qt.rgba(0.54, 0.71, 0.98, 0.20);
                            if (navHover.hovered) return Qt.rgba(1, 1, 1, 0.06);
                            return "transparent";
                        }
                        border.color: isSelected ? "#89b4fa" : "transparent"
                        border.width: 1

                        HoverHandler { id: navHover }
                        TapHandler { onTapped: categoryList.currentIndex = index }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            spacing: 10

                            Kirigami.Icon {
                                source: model.iconName
                                isMask: true
                                implicitWidth: 16
                                implicitHeight: 16
                                color: isSelected ? "#89b4fa" : (navHover.hovered ? "#ffffff" : "#a6adc8")
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                Text {
                                    text: model.name
                                    font.pixelSize: 12
                                    font.weight: isSelected ? Font.DemiBold : Font.Normal
                                    color: isSelected ? "#ffffff" : (navHover.hovered ? "#ffffff" : "#cdd6f4")
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }

            // Right Content Area (StackLayout of the 7 Pages)
            StackLayout {
                id: contentStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: categoryList.currentIndex

                // 0: General
                SettingsGeneralTab {
                    id: generalTab
                }

                // 1: Playback
                SettingsPlaybackTab {
                    id: playbackTab
                }

                // 2: Appearance
                SettingsAppearanceTab {
                    id: appearanceTab
                    onNavigateToWallpapers: {
                        categoryList.currentIndex = 3;
                    }
                }

                // 3: Wallpapers (Preserving the Approved Carousel)
                SettingsWallpapersTab {
                    id: wallpapersTab
                }

                // 4: Visualizer
                SettingsVisualizerTab {
                    id: visualizerTab
                }

                // 5: Widgets
                SettingsWidgetsTab {
                    id: widgetsTab
                }

                // 6: Pomodoro
                SettingsPomodoroTab {
                    id: pomodoroTab
                }

                // 7: History
                SettingsHistoryTab {
                    id: historyTab
                }

                // 8: Help & Shortcuts
                SettingsHelpTab {
                    id: helpTab
                }
            }
        }
    }

    // Interactive resize grip handle at bottom-right corner
    MouseArea {
        id: resizeGrip
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: 24
        height: 24
        cursorShape: Qt.SizeFDiagCursor
        z: 1000
        hoverEnabled: true

        property point dragStart: Qt.point(0, 0)
        property int startW: 0
        property int startH: 0

        onPressed: function(mouse) {
            dragStart = Qt.point(mouse.x, mouse.y);
            startW = root.width;
            startH = root.height;
        }

        onPositionChanged: function(mouse) {
            if (pressed) {
                var dx = mouse.x - dragStart.x;
                var dy = mouse.y - dragStart.y;
                var maxW = (parent && parent.width) ? parent.width - 32 : 1600;
                var maxH = (parent && parent.height) ? parent.height - 32 : 1200;
                var targetW = Math.max(720, Math.min(maxW, startW + dx));
                var targetH = Math.max(500, Math.min(maxH, startH + dy));
                if (settings) {
                    settings.settingsDialogWidth = Math.round(targetW);
                    settings.settingsDialogHeight = Math.round(targetH);
                }
            }
        }

        Kirigami.Icon {
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 4
            implicitWidth: 14
            implicitHeight: 14
            source: "qrc:/icons/lucide/arrow-down-right.svg"
            isMask: true
            opacity: resizeGrip.containsMouse || resizeGrip.pressed ? 0.90 : 0.40
            color: resizeGrip.containsMouse || resizeGrip.pressed ? "#89b4fa" : "#a6adc8"
        }
    }
}
