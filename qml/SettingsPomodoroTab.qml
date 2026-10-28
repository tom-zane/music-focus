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

    property int editIndex: -1

    ColumnLayout {
        width: root.availableWidth
        spacing: 16

        // Top Action Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            ColumnLayout {
                Layout.fillWidth: true
                Layout.minimumWidth: 100
                spacing: 2
                Text {
                    text: "Pomodoro System Configuration"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    color: "#89b4fa"
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                Text {
                    text: "Configure focus sessions, custom interval presets, completion sounds, and visual breathing"
                    font.pixelSize: 11
                    color: "#9399b2"
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }
            }

            SettingsButton {
                text: "Restore Defaults"
                iconSource: "qrc:/icons/lucide/rotate-ccw.svg"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                onClicked: {
                    if (pomodoroManager) {
                        pomodoroManager.restoreDefaultPresets();
                    }
                    if (settings) {
                        settings.pomodoroBreathingEnabled = true;
                        settings.pomodoroBreathingSpeed = 1.0;
                        settings.pomodoroBreathingIntensity = 0.7;
                        settings.pomodoroSoundEnabled = true;
                        settings.pomodoroSoundStyle = 0;
                        settings.pomodoroSoundVolume = 0.8;
                        settings.pomodoroAutoStartBreaks = false;
                        settings.pomodoroAutoStartFocus = false;
                    }
                }
            }
        }

        // =====================================================================
        // 1. TIMER PRESETS SECTION
        // =====================================================================
        SettingsCard {
            title: "Timer Presets & Durations"
            description: "Choose an established interval combination or create custom durations"
            iconSource: "qrc:/icons/lucide/timer.svg"

            // Active Preset Selector Cards
            GridLayout {
                Layout.fillWidth: true
                columns: Math.max(1, Math.min(3, Math.floor(root.availableWidth / 220)))
                rowSpacing: 8
                columnSpacing: 8

                Repeater {
                    model: pomodoroManager ? pomodoroManager.presets : []

                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 64
                        radius: 8
                        readonly property bool isSelected: pomodoroManager && pomodoroManager.activePresetIndex === index

                        color: isSelected
                            ? Qt.rgba(0.54, 0.71, 0.98, 0.18)
                            : (presetCardHover.hovered ? Qt.rgba(1, 1, 1, 0.06) : Qt.rgba(1, 1, 1, 0.03))
                        border.color: isSelected ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.10)
                        border.width: isSelected ? 1.5 : 1

                        HoverHandler { id: presetCardHover }
                        TapHandler {
                            onTapped: {
                                if (pomodoroManager) {
                                    pomodoroManager.selectPreset(index);
                                }
                            }
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 10

                            Kirigami.Icon {
                                source: isSelected ? "qrc:/icons/lucide/check.svg" : "qrc:/icons/lucide/timer.svg"
                                isMask: true
                                implicitWidth: 18
                                implicitHeight: 18
                                color: isSelected ? "#89b4fa" : "#a6adc8"
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2

                                Text {
                                    text: modelData.name
                                    font.pixelSize: 12
                                    font.weight: isSelected ? Font.Bold : Font.DemiBold
                                    color: isSelected ? "#ffffff" : "#cdd6f4"
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }

                                Text {
                                    text: modelData.focusDurationMinutes + "m Focus • " + modelData.breakDurationMinutes + "m Break"
                                    font.pixelSize: 10
                                    color: isSelected ? "#89b4fa" : "#9399b2"
                                }
                            }

                            // Edit / Rename button for custom presets
                            ToolButton {
                                visible: !modelData.isDefault
                                implicitWidth: 26
                                implicitHeight: 26
                                ToolTip.text: "Rename or edit preset"
                                ToolTip.visible: hovered
                                onClicked: {
                                    root.editIndex = index;
                                    editNameInput.text = modelData.name;
                                    editFocusSpin.value = modelData.focusDurationMinutes;
                                    editBreakSpin.value = modelData.breakDurationMinutes;
                                    editPresetBox.visible = true;
                                    customPresetBox.visible = false;
                                }
                                background: Rectangle {
                                    radius: 13
                                    color: parent.hovered ? Qt.rgba(0.54, 0.71, 0.98, 0.20) : "transparent"
                                }
                                contentItem: Kirigami.Icon {
                                    source: "qrc:/icons/lucide/pencil.svg"
                                    isMask: true
                                    color: parent.hovered ? "#89b4fa" : "#a6adc8"
                                    implicitWidth: 14
                                    implicitHeight: 14
                                }
                            }

                            // Delete button for custom presets
                            ToolButton {
                                visible: !modelData.isDefault
                                implicitWidth: 26
                                implicitHeight: 26
                                ToolTip.text: "Delete custom preset"
                                ToolTip.visible: hovered
                                onClicked: {
                                    if (pomodoroManager) {
                                        pomodoroManager.deleteCustomPreset(index);
                                    }
                                }
                                background: Rectangle {
                                    radius: 13
                                    color: parent.hovered ? Qt.rgba(0.95, 0.54, 0.66, 0.20) : "transparent"
                                }
                                contentItem: Kirigami.Icon {
                                    source: "qrc:/icons/lucide/trash-2.svg"
                                    isMask: true
                                    color: parent.hovered ? "#f38ba8" : "#6c7086"
                                    implicitWidth: 14
                                    implicitHeight: 14
                                }
                            }
                        }
                    }
                }
            }

            // Create Custom Preset Button & Drawer
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                SettingsButton {
                    text: customPresetBox.visible ? "Cancel New Preset" : "Add Custom Preset..."
                    iconSource: customPresetBox.visible ? "qrc:/icons/lucide/x.svg" : "qrc:/icons/lucide/plus.svg"
                    onClicked: {
                        customPresetBox.visible = !customPresetBox.visible;
                        if (customPresetBox.visible) {
                            editPresetBox.visible = false;
                        }
                    }
                }

                Item { Layout.fillWidth: true }
            }

            // Inline Custom Preset Edit Form
            Rectangle {
                id: editPresetBox
                visible: false
                Layout.fillWidth: true
                implicitHeight: editFormCol.implicitHeight + 20
                radius: 8
                color: Qt.rgba(0, 0, 0, 0.25)
                border.color: Qt.rgba(0.65, 0.89, 0.63, 0.4)
                border.width: 1

                ColumnLayout {
                    id: editFormCol
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 10

                    RowLayout {
                        Layout.fillWidth: true
                        Text {
                            text: "Rename / Edit Custom Preset"
                            font.pixelSize: 12
                            font.weight: Font.Bold
                            color: "#a6e3a1"
                        }
                        Item { Layout.fillWidth: true }
                        ToolButton {
                            implicitWidth: 22
                            implicitHeight: 22
                            onClicked: editPresetBox.visible = false
                            contentItem: Kirigami.Icon {
                                source: "qrc:/icons/lucide/x.svg"
                                isMask: true
                                color: "#9399b2"
                                implicitWidth: 12
                                implicitHeight: 12
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        TextField {
                            id: editNameInput
                            Layout.fillWidth: true
                            placeholderText: "Preset name"
                            font.pixelSize: 11
                            color: "#ffffff"
                            background: Rectangle {
                                radius: 6
                                color: Qt.rgba(1, 1, 1, 0.08)
                                border.color: editNameInput.activeFocus ? "#a6e3a1" : Qt.rgba(1, 1, 1, 0.15)
                                border.width: 1
                            }
                        }

                        RowLayout {
                            spacing: 4
                            Text { text: "Focus:"; font.pixelSize: 11; color: "#cdd6f4" }
                            SpinBox {
                                id: editFocusSpin
                                from: 1
                                to: 180
                                editable: true
                                implicitWidth: 90
                            }
                            Text { text: "min"; font.pixelSize: 11; color: "#9399b2" }
                        }

                        RowLayout {
                            spacing: 4
                            Text { text: "Break:"; font.pixelSize: 11; color: "#cdd6f4" }
                            SpinBox {
                                id: editBreakSpin
                                from: 1
                                to: 60
                                editable: true
                                implicitWidth: 85
                            }
                            Text { text: "min"; font.pixelSize: 11; color: "#9399b2" }
                        }

                        SettingsButton {
                            isPrimary: true
                            text: "Update"
                            iconSource: "qrc:/icons/lucide/check.svg"
                            onClicked: {
                                var name = editNameInput.text.trim();
                                if (name.length > 0 && pomodoroManager && root.editIndex >= 0) {
                                    pomodoroManager.updateCustomPreset(root.editIndex, name, editFocusSpin.value, editBreakSpin.value);
                                    editPresetBox.visible = false;
                                }
                            }
                        }
                    }
                }
            }

            // Inline Custom Preset Creation Form
            Rectangle {
                id: customPresetBox
                visible: false
                Layout.fillWidth: true
                implicitHeight: customFormCol.implicitHeight + 20
                radius: 8
                color: Qt.rgba(0, 0, 0, 0.25)
                border.color: Qt.rgba(0.54, 0.71, 0.98, 0.3)
                border.width: 1

                ColumnLayout {
                    id: customFormCol
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 10

                    Text {
                        text: "Create Custom Preset"
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        color: "#ffffff"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        TextField {
                            id: presetNameInput
                            Layout.fillWidth: true
                            placeholderText: "Preset name (e.g. Deep Work 90)"
                            font.pixelSize: 11
                            color: "#ffffff"
                            background: Rectangle {
                                radius: 6
                                color: Qt.rgba(1, 1, 1, 0.08)
                                border.color: presetNameInput.activeFocus ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.15)
                                border.width: 1
                            }
                        }

                        RowLayout {
                            spacing: 4
                            Text {
                                text: "Focus:"
                                font.pixelSize: 11
                                color: "#cdd6f4"
                            }
                            SpinBox {
                                id: customFocusSpin
                                from: 1
                                to: 180
                                value: 45
                                editable: true
                                implicitWidth: 90
                            }
                            Text {
                                text: "min"
                                font.pixelSize: 11
                                color: "#9399b2"
                            }
                        }

                        RowLayout {
                            spacing: 4
                            Text {
                                text: "Break:"
                                font.pixelSize: 11
                                color: "#cdd6f4"
                            }
                            SpinBox {
                                id: customBreakSpin
                                from: 1
                                to: 60
                                value: 15
                                editable: true
                                implicitWidth: 85
                            }
                            Text {
                                text: "min"
                                font.pixelSize: 11
                                color: "#9399b2"
                            }
                        }

                        SettingsButton {
                            isPrimary: true
                            text: "Save Preset"
                            iconSource: "qrc:/icons/lucide/check.svg"
                            onClicked: {
                                var name = presetNameInput.text.trim();
                                if (name.length > 0 && pomodoroManager) {
                                    pomodoroManager.addCustomPreset(name, customFocusSpin.value, customBreakSpin.value);
                                    presetNameInput.text = "";
                                    customPresetBox.visible = false;
                                }
                            }
                        }
                    }
                }
            }

            // Helpful note regarding live session protection
            SettingsInfoBox {
                text: "Changing a preset while a session is running applies automatically to the subsequent phase to prevent disrupting your focus."
                iconSource: "qrc:/icons/lucide/info.svg"
            }
        }

        // =====================================================================
        // 2. APPEARANCE AND BREATHING ANIMATION
        // =====================================================================
        SettingsCard {
            title: "Appearance & Breathing Animation"
            description: "Subtle ambient animations for the translucent floating timer"
            iconSource: "qrc:/icons/lucide/palette.svg"

            SettingsToggle {
                title: "Breathing Glow Animation"
                description: "Subtly increases and decreases green glow during running focus sessions"
                checked: settings ? settings.pomodoroBreathingEnabled : true
                onToggled: function(c) {
                    if (settings) settings.pomodoroBreathingEnabled = c;
                }
            }

            SettingsSlider {
                title: "Breathing Speed"
                description: "Speed cycle of the gentle breathing luminescence"
                from: 0.5
                to: 2.0
                stepSize: 0.25
                value: settings ? settings.pomodoroBreathingSpeed : 1.0
                valueText: value <= 0.75 ? "Relaxed (Slow)" : (value >= 1.5 ? "Brisk (Fast)" : "Normal")
                enabled: settings ? settings.pomodoroBreathingEnabled : true
                onMoved: function(v) {
                    if (settings) settings.pomodoroBreathingSpeed = Math.round(v * 100) / 100;
                }
            }

            SettingsSlider {
                title: "Breathing Intensity"
                description: "Brightness depth of the breathing glow"
                from: 0.2
                to: 1.0
                stepSize: 0.1
                value: settings ? settings.pomodoroBreathingIntensity : 0.7
                valueText: Math.round(value * 100) + "%"
                enabled: settings ? settings.pomodoroBreathingEnabled : true
                onMoved: function(v) {
                    if (settings) settings.pomodoroBreathingIntensity = Math.round(v * 100) / 100;
                }
            }
        }

        // =====================================================================
        // 3. COMPLETION SOUND ALERTS
        // =====================================================================
        SettingsCard {
            title: "Completion Sound"
            description: "Configurable non-intrusive sound played once when a session completes"
            iconSource: "qrc:/icons/lucide/volume-2.svg"

            SettingsToggle {
                title: "Enable Completion Sound"
                description: "Play an audible notification when focus or break countdown reaches zero"
                checked: settings ? settings.pomodoroSoundEnabled : true
                onToggled: function(c) {
                    if (settings) settings.pomodoroSoundEnabled = c;
                }
            }

            // Sound Style Selector
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                enabled: settings ? settings.pomodoroSoundEnabled : true
                opacity: (settings && settings.pomodoroSoundEnabled) ? 1.0 : 0.45

                ColumnLayout {
                    spacing: 2
                    Text {
                        text: "Sound Style"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        color: "#ffffff"
                    }
                    Text {
                        text: "Default: sequence of three short separated beeps"
                        font.pixelSize: 11
                        color: "#9399b2"
                    }
                }

                Item { Layout.fillWidth: true }

                ComboBox {
                    id: soundStyleCombo
                    model: ["Three Beeps (Default)", "Gentle Chime", "Digital Pulse", "Soft Bell"]
                    currentIndex: settings ? settings.pomodoroSoundStyle : 0
                    implicitWidth: 200
                    Layout.alignment: Qt.AlignVCenter
                    onActivated: function(index) {
                        if (settings) settings.pomodoroSoundStyle = index;
                    }
                }

                SettingsButton {
                    text: "Preview"
                    iconSource: "qrc:/icons/lucide/volume-2.svg"
                    Layout.alignment: Qt.AlignVCenter
                    onClicked: {
                        if (pomodoroManager) {
                            pomodoroManager.previewSound(
                                soundStyleCombo.currentIndex,
                                settings ? settings.pomodoroSoundVolume : 0.8
                            );
                        }
                    }
                }
            }

            SettingsSlider {
                title: "Alert Volume"
                description: "Volume level of the completion sound without affecting music playback"
                from: 0.0
                to: 1.0
                stepSize: 0.05
                value: settings ? settings.pomodoroSoundVolume : 0.8
                valueText: Math.round(value * 100) + "%"
                enabled: settings ? settings.pomodoroSoundEnabled : true
                onMoved: function(v) {
                    if (settings) settings.pomodoroSoundVolume = Math.round(v * 100) / 100;
                }
            }
        }

        // =====================================================================
        // 4. TIMER BEHAVIOR & AUTOMATION
        // =====================================================================
        SettingsCard {
            title: "Timer Behavior & Transitions"
            description: "Automatic phase advancing between focus intervals and breaks"
            iconSource: "qrc:/icons/lucide/repeat.svg"

            SettingsToggle {
                title: "Auto-Start Breaks"
                description: "Immediately start break countdown when a focus session completes"
                checked: settings ? settings.pomodoroAutoStartBreaks : false
                onToggled: function(c) {
                    if (settings) settings.pomodoroAutoStartBreaks = c;
                }
            }

            SettingsToggle {
                title: "Auto-Start Next Focus"
                description: "Immediately start next focus interval when a break finishes"
                checked: settings ? settings.pomodoroAutoStartFocus : false
                onToggled: function(c) {
                    if (settings) settings.pomodoroAutoStartFocus = c;
                }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 12 }
    }
}
