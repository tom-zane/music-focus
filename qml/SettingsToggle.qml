import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Item {
    id: root

    property string title: ""
    property string description: ""
    property string iconSource: ""
    property bool checked: false
    property bool interactive: true
    signal toggled(bool checked)

    Layout.fillWidth: true
    implicitHeight: Math.max(40, rowLayout.implicitHeight + 8)
    opacity: enabled ? 1.0 : 0.45

    MouseArea {
        anchors.fill: parent
        cursorShape: root.interactive && root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        hoverEnabled: true
        id: hoverArea
        onClicked: {
            if (root.interactive && root.enabled) {
                root.checked = !root.checked;
                root.toggled(root.checked);
            }
        }
    }

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        anchors.leftMargin: 4
        anchors.rightMargin: 4
        spacing: 12

        Rectangle {
            visible: root.iconSource.length > 0
            implicitWidth: 26
            implicitHeight: 26
            radius: 5
            color: root.checked ? Qt.rgba(0.54, 0.71, 0.98, 0.15) : Qt.rgba(1, 1, 1, 0.05)

            Kirigami.Icon {
                anchors.centerIn: parent
                source: root.iconSource
                isMask: true
                implicitWidth: 14
                implicitHeight: 14
                color: root.checked ? "#89b4fa" : "#a6adc8"
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: root.title
                font.pixelSize: 13
                font.weight: Font.Medium
                color: root.checked ? "#ffffff" : "#cdd6f4"
            }

            Text {
                text: root.description
                font.pixelSize: 11
                color: "#9399b2"
                visible: root.description.length > 0
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
            }
        }

        // Custom Sleek Switch
        Rectangle {
            id: switchTrack
            implicitWidth: 40
            implicitHeight: 22
            radius: 11
            color: root.checked ? "#89b4fa" : (hoverArea.containsMouse ? Qt.rgba(1, 1, 1, 0.22) : Qt.rgba(1, 1, 1, 0.14))
            border.color: root.checked ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.20)
            border.width: 1

            Behavior on color { ColorAnimation { duration: 150 } }

            Rectangle {
                id: switchThumb
                width: 16
                height: 16
                radius: 8
                y: 3
                x: root.checked ? switchTrack.width - width - 3 : 3
                color: root.checked ? "#11111b" : "#cdd6f4"

                Behavior on x { NumberAnimation { duration: 140; easing.type: Easing.OutQuad } }
                Behavior on color { ColorAnimation { duration: 140 } }
            }
        }
    }
}
