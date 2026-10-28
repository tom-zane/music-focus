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

    signal navigateToWallpapers()

    ColumnLayout {
        width: root.availableWidth
        spacing: 16

        // 1. Interface Transparency & Background Effects Card
        SettingsCard {
            title: "Interface Transparency & Background Effects"
            description: "Customize glassmorphism, background blur, and overlay translucency"
            iconSource: "qrc:/icons/lucide/palette.svg"

            SettingsSlider {
                title: "Overlay Opacity"
                description: "Base opacity for floating widgets and control panels"
                from: 0.15
                to: 1.00
                stepSize: 0.05
                value: settings.overlayOpacity
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.overlayOpacity = Math.round(v * 100) / 100; }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            SettingsToggle {
                title: "Background Blur"
                description: "Applies hardware-accelerated blur behind interface panels"
                checked: settings.blurEnabled
                onToggled: function(c) { settings.blurEnabled = c; }
            }

            SettingsSlider {
                title: "Blur Strength"
                description: "Radius and intensity of the background blur effect"
                from: 0.10
                to: 1.00
                stepSize: 0.05
                value: settings.blurStrength
                valueText: Math.round(value * 100) + "%"
                enabled: settings.blurEnabled
                onMoved: function(v) { settings.blurStrength = Math.round(v * 100) / 100; }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            SettingsToggle {
                title: "Background Darkening"
                description: "Adds a dark translucent tint to boost readability over bright wallpapers"
                checked: settings.darkeningEnabled
                onToggled: function(c) { settings.darkeningEnabled = c; }
            }

            SettingsSlider {
                title: "Darkening Strength"
                description: "Opacity of the dark background underlay"
                from: 0.10
                to: 1.00
                stepSize: 0.05
                value: settings.darkeningOpacity
                valueText: Math.round(value * 100) + "%"
                enabled: settings.darkeningEnabled
                onMoved: function(v) { settings.darkeningOpacity = Math.round(v * 100) / 100; }
            }
        }

        // 2. Adaptive Text & Icon Contrast Card
        SettingsCard {
            title: "Adaptive Text & Icon Contrast"
            description: "Automatically sample wallpaper colors to maintain WCAG-compliant legibility"
            iconSource: "qrc:/icons/lucide/sparkles.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Repeater {
                    model: [
                        { text: "Automatic", desc: "Adaptive background sampling", value: 0, icon: "qrc:/icons/lucide/sparkles.svg" },
                        { text: "Light Foregrounds", desc: "Crisp white text & icons", value: 1, icon: "qrc:/icons/lucide/palette.svg" },
                        { text: "Dark Foregrounds", desc: "Dark text for light walls", value: 2, icon: "qrc:/icons/lucide/palette.svg" }
                    ]

                    delegate: Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 52
                        radius: 8
                        color: settings.contrastMode === modelData.value ? Qt.rgba(0.54, 0.71, 0.98, 0.18) : (modeHover.hovered ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.03))
                        border.color: settings.contrastMode === modelData.value ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                        border.width: settings.contrastMode === modelData.value ? 1.5 : 1

                        HoverHandler { id: modeHover }
                        TapHandler { onTapped: settings.contrastMode = modelData.value }

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 8

                            Kirigami.Icon {
                                source: modelData.icon
                                isMask: true
                                implicitWidth: 18
                                implicitHeight: 18
                                color: settings.contrastMode === modelData.value ? "#89b4fa" : "#a6adc8"
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 1
                                Text {
                                    text: modelData.text
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    color: settings.contrastMode === modelData.value ? "#ffffff" : "#cdd6f4"
                                }
                                Text {
                                    text: modelData.desc
                                    font.pixelSize: 10
                                    color: "#9399b2"
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }
        }

        // 3. UI Element Scales Card
        SettingsCard {
            title: "Interface Scaling"
            description: "Independently adjust proportions of playback, playlist, and top controls"
            iconSource: "qrc:/icons/lucide/scaling.svg"

            SettingsSlider {
                title: "Playback Strip Scale"
                from: 0.70
                to: 1.50
                stepSize: 0.05
                value: settings.playbackStripScale
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.playbackStripScale = Math.round(v * 100) / 100; }
            }

            SettingsSlider {
                title: "Playlist Overlay Scale"
                from: 0.70
                to: 1.50
                stepSize: 0.05
                value: settings.playlistScale
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.playlistScale = Math.round(v * 100) / 100; }
            }

            SettingsSlider {
                title: "Top Controls Scale"
                from: 0.70
                to: 1.50
                stepSize: 0.05
                value: settings.topControlsScale
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.topControlsScale = Math.round(v * 100) / 100; }
            }

            RowLayout {
                Layout.fillWidth: true
                SettingsButton {
                    text: "Reset All Scales to Defaults"
                    iconSource: "qrc:/icons/lucide/rotate-ccw.svg"
                    onClicked: settings.resetScales()
                }
                Item { Layout.fillWidth: true }
            }
        }

        // 4. Typography Card
        SettingsCard {
            title: "Typography"
            description: "System font family and base sizing for text elements"
            iconSource: "qrc:/icons/lucide/pencil.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Font Family:"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: "#cdd6f4"
                    Layout.preferredWidth: 100
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: ["System Default"].concat(settings.availableFontFamilies)
                    currentIndex: {
                        if (!settings.fontFamily || settings.fontFamily === "") return 0;
                        var idx = settings.availableFontFamilies.indexOf(settings.fontFamily);
                        return (idx >= 0) ? (idx + 1) : 0;
                    }
                    focusPolicy: Qt.TabFocus
                    onActivated: {
                        if (index === 0) settings.fontFamily = "";
                        else settings.fontFamily = settings.availableFontFamilies[index - 1];
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: "Font Size:"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: "#cdd6f4"
                    Layout.preferredWidth: 100
                }

                SpinBox {
                    from: 9
                    to: 20
                    value: settings.fontSize
                    focusPolicy: Qt.TabFocus
                    onValueModified: settings.fontSize = value
                }

                SettingsButton {
                    text: "Reset Font"
                    iconSource: "qrc:/icons/lucide/rotate-ccw.svg"
                    onClicked: settings.resetFont()
                }

                Item { Layout.fillWidth: true }
            }
        }

        // 5. Quick link to Wallpapers tab
        SettingsCard {
            title: "Wallpaper Engine Reference"
            description: "Active wallpaper rendering mode and gallery management"
            iconSource: "qrc:/icons/lucide/image.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Text {
                    text: {
                        if (settings.wallpaperMode === 1) return "Current engine: Picture (Still photo carousel)";
                        if (settings.wallpaperMode === 0) return "Current engine: Generated (Pastel study gradient)";
                        return "Current engine: Video (Ambient looping video)";
                    }
                    font.pixelSize: 12
                    color: "#cdd6f4"
                    Layout.fillWidth: true
                }

                SettingsButton {
                    text: "Manage Wallpapers in Wallpapers Tab →"
                    iconSource: "qrc:/icons/lucide/image.svg"
                    onClicked: root.navigateToWallpapers()
                }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 8 }
    }
}
