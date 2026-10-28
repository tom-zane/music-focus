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

        // 1. Enablement & Visualizer Style Card
        SettingsCard {
            title: "Visualizer Style & Placement"
            description: "Ambient audio reactivity rendered between wallpaper and UI controls"
            iconSource: "qrc:/icons/lucide/activity.svg"

            SettingsToggle {
                title: "Enable Audio Visualizer"
                description: "Renders real-time harmonic waveforms and reactive frequency bars"
                checked: settings.visualizerEnabled
                onToggled: function(c) { settings.visualizerEnabled = c; }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.06)
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                enabled: settings.visualizerEnabled

                Text {
                    text: "Theme Style:"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: "#cdd6f4"
                    Layout.preferredWidth: 120
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: [
                        "Classic Equalizer Bars (Windows Media Player style)",
                        "Soft Rounded Bars (Sleek pastel vertical capsules)",
                        "Minimal Waveform (Clean oscillating harmonic spline)",
                        "Subtle Ambient Waves (Dual harmonic ribbons)"
                    ]
                    currentIndex: settings.visualizerTheme
                    focusPolicy: Qt.TabFocus
                    onActivated: settings.visualizerTheme = index
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                enabled: settings.visualizerEnabled

                Text {
                    text: "Vertical Position:"
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: "#cdd6f4"
                    Layout.preferredWidth: 120
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: [
                        "Bottom (Above playback strip)",
                        "Center (Screen midpoint)",
                        "Top (Subtle header)"
                    ]
                    currentIndex: settings.visualizerPosition
                    focusPolicy: Qt.TabFocus
                    onActivated: settings.visualizerPosition = index
                }
            }
        }

        // 2. Geometry, Opacity & Density Card
        SettingsCard {
            title: "Dimensions & Density"
            description: "Size, bar density, and visual opacity of the reactive graphic"
            iconSource: "qrc:/icons/lucide/sliders-horizontal.svg"
            enabled: settings.visualizerEnabled

            SettingsSlider {
                title: "Visualizer Height"
                description: "Vertical space allocated to peaks and waves"
                from: 60
                to: 240
                stepSize: 10
                value: settings.visualizerHeight
                valueText: Math.round(value) + " px"
                onMoved: function(v) { settings.visualizerHeight = Math.round(v); }
            }

            SettingsSlider {
                title: "Visualizer Opacity"
                description: "Subtle translucency to prevent distracting from study"
                from: 0.15
                to: 1.00
                stepSize: 0.05
                value: settings.visualizerOpacity
                valueText: Math.round(value * 100) + "%"
                onMoved: function(v) { settings.visualizerOpacity = Math.round(v * 100) / 100; }
            }

            SettingsSlider {
                title: "Bar Density"
                description: "Number of frequency spectrum bands displayed across width"
                from: 16
                to: 128
                stepSize: 1
                value: settings.visualizerBarDensity
                valueText: Math.round(value) + " bars"
                onMoved: function(v) { settings.visualizerBarDensity = Math.round(v); }
            }
        }

        // 3. Animation Sensitivity & Speed Card
        SettingsCard {
            title: "Dynamics & Responsiveness"
            description: "Reaction sensitivity and smoothing speed for audio beats"
            iconSource: "qrc:/icons/lucide/timer.svg"
            enabled: settings.visualizerEnabled

            SettingsSlider {
                title: "Sensitivity"
                description: "Multiplier applied to incoming audio amplitude"
                from: 0.50
                to: 2.00
                stepSize: 0.05
                value: settings.visualizerSensitivity
                valueText: value.toFixed(2) + "×"
                onMoved: function(v) { settings.visualizerSensitivity = Math.round(v * 100) / 100; }
            }

            SettingsSlider {
                title: "Movement Speed"
                description: "Animation tempo and spring decay duration"
                from: 0.40
                to: 2.00
                stepSize: 0.05
                value: settings.visualizerSpeed
                valueText: {
                    var s = settings.visualizerSpeed;
                    var lbl = (s <= 0.60) ? "Slow" : (s <= 1.05 ? "Relaxed" : "Fast");
                    return s.toFixed(2) + "× (" + lbl + ")";
                }
                onMoved: function(v) { settings.visualizerSpeed = Math.round(v * 100) / 100; }
            }
        }

        // 4. Audio Spectrum Status (Visually Secondary)
        SettingsInfoBox {
            isSuccess: audioPlayer.hasGenuineSpectrum
            iconSource: audioPlayer.hasGenuineSpectrum ? "qrc:/icons/lucide/check.svg" : "qrc:/icons/lucide/circle-alert.svg"
            text: audioPlayer.hasGenuineSpectrum ? "Genuine FFT audio buffer frequency analysis active (QAudioBufferOutput)" : "Harmonic ambient spectrum model active (backend fallback)"
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 8 }
    }
}
