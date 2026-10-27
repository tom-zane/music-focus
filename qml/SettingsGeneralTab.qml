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

        // 1. Music Library Section
        SettingsCard {
            title: "Music Library"
            description: "Directory containing your local audio files and wallpaper collections"
            iconSource: "qrc:/icons/lucide/folder-open.svg"

            // Directory Path Bar & Browse
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 38
                radius: 6
                color: Qt.rgba(0, 0, 0, 0.25)
                border.color: Qt.rgba(1, 1, 1, 0.10)
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 6
                    spacing: 8

                    Kirigami.Icon {
                        source: "qrc:/icons/lucide/folder-open.svg"
                        isMask: true
                        implicitWidth: 16
                        implicitHeight: 16
                        color: settings.hasValidDirectory ? "#89b4fa" : "#f38ba8"
                    }

                    Text {
                        Layout.fillWidth: true
                        text: settings.musicDirectory ? settings.musicDirectory : "No folder selected"
                        font.pixelSize: 12
                        font.family: "monospace"
                        color: settings.musicDirectory ? "#ffffff" : "#6c7086"
                        elide: Text.ElideMiddle
                    }

                    SettingsButton {
                        text: "Browse..."
                        iconSource: "qrc:/icons/lucide/folder-open.svg"
                        onClicked: appController.promptForMusicDirectory()
                    }
                }
            }

            // Rescan Actions
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                SettingsButton {
                    text: "Rescan Music Library"
                    iconSource: "qrc:/icons/lucide/rotate-cw.svg"
                    onClicked: appController.rescanLibrary()
                }

                SettingsButton {
                    text: "Rescan Wallpapers"
                    iconSource: "qrc:/icons/lucide/rotate-cw.svg"
                    onClicked: appController.rescanWallpapers()
                }

                Item { Layout.fillWidth: true }
            }

            // Helpful Folder Structure Tip
            SettingsInfoBox {
                text: "Wallpapers are automatically loaded from subfolders inside this folder:\n• Wallpapers/Still Wallpapers/ (JPG, PNG, WEBP)\n• Wallpapers/Video Wallpapers/ (MP4, WEBM, MKV)"
                iconSource: "qrc:/icons/lucide/info.svg"
            }
        }

        // 2. Automatic Inactivity Fade / Hide Section
        SettingsCard {
            title: "Automatic Inactivity Fade / Hide"
            description: "Automatically reduce visual distraction when mouse and keyboard are idle"
            iconSource: "qrc:/icons/lucide/eye.svg"

            SettingsToggle {
                title: "Enable Inactivity Timer"
                description: "Fades or hides on-screen controls after user inactivity"
                checked: settings.inactivityEnabled
                onToggled: function(c) { settings.inactivityEnabled = c; }
            }

            // Mode Selector: Fade vs Hide
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                enabled: settings.inactivityEnabled
                opacity: settings.inactivityEnabled ? 1.0 : 0.45

                // Fade Segment
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 46
                    radius: 8
                    color: settings.inactivityMode === 0 ? Qt.rgba(0.54, 0.71, 0.98, 0.16) : (fadeHover.hovered ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.03))
                    border.color: settings.inactivityMode === 0 ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                    border.width: settings.inactivityMode === 0 ? 1.5 : 1

                    HoverHandler { id: fadeHover }
                    TapHandler { onTapped: settings.inactivityMode = 0 }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/eye.svg"
                            isMask: true
                            implicitWidth: 18
                            implicitHeight: 18
                            color: settings.inactivityMode === 0 ? "#89b4fa" : "#a6adc8"
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1

                            Text {
                                text: "Fade"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: settings.inactivityMode === 0 ? "#ffffff" : "#cdd6f4"
                            }
                            Text {
                                text: "Controls fade to translucent opacity"
                                font.pixelSize: 10
                                color: "#9399b2"
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }
                    }
                }

                // Hide Segment
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 46
                    radius: 8
                    color: settings.inactivityMode === 1 ? Qt.rgba(0.54, 0.71, 0.98, 0.16) : (hideHover.hovered ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.03))
                    border.color: settings.inactivityMode === 1 ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                    border.width: settings.inactivityMode === 1 ? 1.5 : 1

                    HoverHandler { id: hideHover }
                    TapHandler { onTapped: settings.inactivityMode = 1 }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/eye-off.svg"
                            isMask: true
                            implicitWidth: 18
                            implicitHeight: 18
                            color: settings.inactivityMode === 1 ? "#89b4fa" : "#a6adc8"
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1

                            Text {
                                text: "Hide"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: settings.inactivityMode === 1 ? "#ffffff" : "#cdd6f4"
                            }
                            Text {
                                text: "Controls become fully invisible (0%)"
                                font.pixelSize: 10
                                color: "#9399b2"
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }

            // Inactivity Timeout Slider
            SettingsSlider {
                title: "Inactivity Timeout"
                description: "Seconds of keyboard/mouse inactivity before fading starts"
                from: 2
                to: 30
                stepSize: 1
                value: settings.inactivityTimeoutSeconds
                valueText: Math.round(value) + " seconds"
                enabled: settings.inactivityEnabled
                onMoved: function(v) { settings.inactivityTimeoutSeconds = Math.round(v); }
            }

            // Target Opacity Slider (Only for Fade mode)
            SettingsSlider {
                title: "Target Opacity"
                description: "Translucent opacity maintained while idle"
                from: 0.05
                to: 0.50
                stepSize: 0.05
                value: settings.inactivityFadeOpacity
                valueText: Math.round(value * 100) + "%"
                visible: settings.inactivityMode === 0
                enabled: settings.inactivityEnabled && settings.inactivityMode === 0
                onMoved: function(v) { settings.inactivityFadeOpacity = Math.round(v * 100) / 100; }
            }
        }

        // 3. Desktop Grid Layout Section
        SettingsCard {
            title: "Desktop Grid Layout"
            description: "Freely reposition and resize floating widgets on the study workspace"
            iconSource: "qrc:/icons/lucide/layout-grid.svg"

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                SettingsButton {
                    isPrimary: true
                    text: "Customize Desktop Grid Layout"
                    iconSource: "qrc:/icons/lucide/layout-grid.svg"
                    onClicked: {
                        settingsDialog.close();
                        layoutController.startEditing();
                    }
                }

                SettingsButton {
                    text: "Reset Layout to Default"
                    iconSource: "qrc:/icons/lucide/rotate-ccw.svg"
                    onClicked: layoutController.resetLayout()
                }

                Item { Layout.fillWidth: true }
            }
        }

        // Bottom padding spacer
        Item { Layout.preferredHeight: 8 }
    }
}
