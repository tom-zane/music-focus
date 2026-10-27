import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property string title: ""
    property string description: ""
    property real from: 0.0
    property real to: 1.0
    property real stepSize: 0.01
    property real value: 0.5
    property string valueText: ""
    property int valueWidth: 70
    signal moved(real value)

    Layout.fillWidth: true
    implicitHeight: Math.max(42, colLayout.implicitHeight)
    opacity: enabled ? 1.0 : 0.45

    ColumnLayout {
        id: colLayout
        anchors.fill: parent
        spacing: 4

        // Top Row: Label and Current Value
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Text {
                text: root.title
                font.pixelSize: 13
                font.weight: Font.Medium
                color: "#cdd6f4"
            }

            Item { Layout.fillWidth: true }

            Text {
                text: root.valueText !== "" ? root.valueText : root.value.toFixed(1)
                font.pixelSize: 12
                font.weight: Font.DemiBold
                font.family: "monospace"
                color: "#89b4fa"
                horizontalAlignment: Text.AlignRight
                Layout.preferredWidth: root.valueWidth
            }
        }

        Text {
            text: root.description
            font.pixelSize: 11
            color: "#9399b2"
            visible: root.description.length > 0
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        // Custom Slider Control
        Slider {
            id: slider
            Layout.fillWidth: true
            implicitHeight: 22
            from: root.from
            to: root.to
            stepSize: root.stepSize
            value: root.value
            focusPolicy: Qt.TabFocus

            onMoved: {
                root.value = slider.value;
                root.moved(slider.value);
            }

            // Sync external value changes to slider
            Binding on value {
                when: !slider.pressed
                value: root.value
            }

            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 5
                width: slider.availableWidth
                height: implicitHeight
                radius: 2.5
                color: Qt.rgba(1, 1, 1, 0.12)

                // Highlighted Progress Bar
                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    color: root.enabled ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.25)
                    radius: 2.5
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 16
                implicitHeight: 16
                radius: 8
                color: slider.pressed ? "#b4befe" : (slider.hovered ? "#ffffff" : "#cdd6f4")
                border.color: slider.pressed ? "#89b4fa" : Qt.rgba(0, 0, 0, 0.4)
                border.width: 1

                Behavior on color { ColorAnimation { duration: 100 } }
                Behavior on scale { NumberAnimation { duration: 100 } }
                scale: slider.pressed ? 1.15 : (slider.hovered ? 1.08 : 1.0)
            }
        }
    }
}
