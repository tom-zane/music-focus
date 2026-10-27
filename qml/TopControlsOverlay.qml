import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Item {
    id: root

    readonly property real uiScale: (settings && settings.topControlsScale) ? settings.topControlsScale : 1.0

    implicitWidth: controlsLayout.implicitWidth + Math.round(16 * root.uiScale)
    implicitHeight: controlsLayout.implicitHeight + Math.round(12 * root.uiScale)

    signal settingsClicked()
    signal fullscreenToggled()
    signal todoToggled()
    signal layoutEditToggled()

    property bool isFullscreen: false

    RowLayout {
        id: controlsLayout
        anchors.centerIn: parent
        spacing: Math.round(8 * root.uiScale)

        // --- Wallpaper Navigation Arrow Buttons ---
        RowLayout {
            spacing: Math.round(6 * root.uiScale)
            visible: settings.wallpaperMode === 1 || settings.wallpaperMode === 2 || settings.wallpaperMode === 0

            // Previous Wallpaper Button
            ToolButton {
                id: prevWallpaperBtn
                implicitWidth: Math.round(38 * root.uiScale)
                implicitHeight: Math.round(38 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: {
                    if (settings.wallpaperMode === 2) return "Previous Video Wallpaper (Shift+W)";
                    if (settings.wallpaperMode === 0) return "Regenerate Gradient (Shift+W)";
                    return "Previous Wallpaper (Shift+W)";
                }
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: Math.round(10 * root.uiScale)
                    color: prevWallpaperBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(0, 0, 0, 0.15)) : (contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity) : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity))
                    border.color: prevWallpaperBtn.activeFocus ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12))
                    border.width: 1

                    FrostedBackground {
                        targetItem: parent
                        radius: parent.radius
                    }

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }

                contentItem: Item {
                    implicitWidth: Math.round(18 * root.uiScale)
                    implicitHeight: Math.round(18 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/chevron-left.svg"
                        isMask: true
                        width: Math.round(18 * root.uiScale)
                        height: Math.round(18 * root.uiScale)
                        color: prevWallpaperBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                    }
                }

                onClicked: {
                    if (settings.wallpaperMode === 2) {
                        wallpaperManager.previousVideo();
                    } else if (settings.wallpaperMode === 0) {
                        wallpaperManager.regenerateGradientSeed();
                    } else {
                        wallpaperManager.previousWallpaper();
                    }
                }
            }

            // Next Wallpaper Button
            ToolButton {
                id: nextWallpaperBtn
                implicitWidth: Math.round(38 * root.uiScale)
                implicitHeight: Math.round(38 * root.uiScale)
                focusPolicy: Qt.TabFocus
                ToolTip.text: {
                    if (settings.wallpaperMode === 2) return "Next Video Wallpaper (W)";
                    if (settings.wallpaperMode === 0) return "Regenerate Gradient (W)";
                    return "Next Wallpaper (W)";
                }
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: Math.round(10 * root.uiScale)
                    color: nextWallpaperBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(0, 0, 0, 0.15)) : (contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity) : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity))
                    border.color: nextWallpaperBtn.activeFocus ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12))
                    border.width: 1

                    FrostedBackground {
                        targetItem: parent
                        radius: parent.radius
                    }

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }

                contentItem: Item {
                    implicitWidth: Math.round(18 * root.uiScale)
                    implicitHeight: Math.round(18 * root.uiScale)
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/chevron-right.svg"
                        isMask: true
                        width: Math.round(18 * root.uiScale)
                        height: Math.round(18 * root.uiScale)
                        color: nextWallpaperBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                    }
                }

                onClicked: {
                    if (settings.wallpaperMode === 2) {
                        wallpaperManager.nextVideo();
                    } else if (settings.wallpaperMode === 0) {
                        wallpaperManager.regenerateGradientSeed();
                    } else {
                        wallpaperManager.nextWallpaper();
                    }
                }
            }
        }

        Item { implicitWidth: Math.round(2 * root.uiScale) }

        // Todo Toggle Button
        ToolButton {
            id: todoBtn
            visible: settings.todoEnabled
            implicitWidth: Math.round(38 * root.uiScale)
            implicitHeight: Math.round(38 * root.uiScale)
            focusPolicy: Qt.TabFocus
            ToolTip.text: settings.todoVisible ? "Hide Todo List (T)" : "Show Todo List (T)"
            ToolTip.visible: hovered

            background: Rectangle {
                radius: Math.round(10 * root.uiScale)
                color: settings.todoVisible ? Qt.rgba(0.54, 0.71, 0.98, 0.28) : (todoBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(0, 0, 0, 0.15)) : (contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity) : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity)))
                border.color: todoBtn.activeFocus ? contrastManager.accentColor : (settings.todoVisible ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12)))
                border.width: 1

                FrostedBackground {
                    targetItem: parent
                    radius: parent.radius
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }

            contentItem: Item {
                implicitWidth: Math.round(18 * root.uiScale)
                implicitHeight: Math.round(18 * root.uiScale)
                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "qrc:/icons/lucide/list-todo.svg"
                    isMask: true
                    width: Math.round(18 * root.uiScale)
                    height: Math.round(18 * root.uiScale)
                    color: settings.todoVisible ? contrastManager.accentColor : (todoBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor)
                }
            }

            onClicked: {
                settings.todoVisible = !settings.todoVisible;
                root.todoToggled();
            }
        }

        // Fullscreen Toggle Button
        ToolButton {
            id: fsBtn
            implicitWidth: Math.round(38 * root.uiScale)
            implicitHeight: Math.round(38 * root.uiScale)
            focusPolicy: Qt.TabFocus
            ToolTip.text: root.isFullscreen ? "Exit Fullscreen (Esc / F11)" : "Fullscreen (F11)"
            ToolTip.visible: hovered

            background: Rectangle {
                radius: Math.round(10 * root.uiScale)
                color: fsBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(0, 0, 0, 0.15)) : (contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity) : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity))
                border.color: fsBtn.activeFocus ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12))
                border.width: 1

                FrostedBackground {
                    targetItem: parent
                    radius: parent.radius
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }

            contentItem: Item {
                implicitWidth: Math.round(18 * root.uiScale)
                implicitHeight: Math.round(18 * root.uiScale)
                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: root.isFullscreen ? "qrc:/icons/lucide/minimize-2.svg" : "qrc:/icons/lucide/maximize-2.svg"
                    isMask: true
                    width: Math.round(18 * root.uiScale)
                    height: Math.round(18 * root.uiScale)
                    color: fsBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                }
            }

            onClicked: root.fullscreenToggled()
        }

        // Layout Grid Editing Button
        ToolButton {
            id: editLayoutBtn
            implicitWidth: Math.round(38 * root.uiScale)
            implicitHeight: Math.round(38 * root.uiScale)
            focusPolicy: Qt.TabFocus
            ToolTip.text: layoutController.editing ? "Done Editing Layout (E)" : "Customize Desktop Layout Grid (E)"
            ToolTip.visible: hovered

            background: Rectangle {
                radius: Math.round(10 * root.uiScale)
                color: layoutController.editing ? contrastManager.accentColor : (editLayoutBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(0, 0, 0, 0.15)) : (contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity) : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity)))
                border.color: layoutController.editing ? (contrastManager.isDark ? "#ffffff" : "#000000") : (editLayoutBtn.activeFocus ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12)))
                border.width: 1

                FrostedBackground {
                    targetItem: parent
                    radius: parent.radius
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }

            contentItem: Item {
                implicitWidth: Math.round(18 * root.uiScale)
                implicitHeight: Math.round(18 * root.uiScale)
                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "qrc:/icons/lucide/layout-grid.svg"
                    isMask: true
                    width: Math.round(18 * root.uiScale)
                    height: Math.round(18 * root.uiScale)
                    color: layoutController.editing ? (contrastManager.isDark ? "#11111b" : "#ffffff") : (editLayoutBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor)
                }
            }

            onClicked: root.layoutEditToggled()
        }

        // Settings Button
        ToolButton {
            id: settingsBtn
            implicitWidth: Math.round(38 * root.uiScale)
            implicitHeight: Math.round(38 * root.uiScale)
            focusPolicy: Qt.TabFocus
            ToolTip.text: "Settings (Ctrl+, / S)"
            ToolTip.visible: hovered

            background: Rectangle {
                radius: Math.round(10 * root.uiScale)
                color: settingsBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(0, 0, 0, 0.15)) : (contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity) : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity))
                border.color: settingsBtn.activeFocus ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12))
                border.width: 1

                FrostedBackground {
                    targetItem: parent
                    radius: parent.radius
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }

            contentItem: Item {
                implicitWidth: Math.round(18 * root.uiScale)
                implicitHeight: Math.round(18 * root.uiScale)
                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: "qrc:/icons/lucide/settings.svg"
                    isMask: true
                    width: Math.round(18 * root.uiScale)
                    height: Math.round(18 * root.uiScale)
                    color: settingsBtn.hovered ? contrastManager.primaryTextColor : contrastManager.iconColor
                }
            }

            onClicked: root.settingsClicked()
        }
    }
}
