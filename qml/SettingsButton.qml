import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Button {
    id: root

    property string iconSource: ""
    property bool isPrimary: false
    property bool isDestructive: false

    implicitHeight: 32
    implicitWidth: Math.max(90, rowLayout.implicitWidth + 24)
    focusPolicy: Qt.TabFocus

    background: Rectangle {
        radius: 6
        color: {
            if (!root.enabled) return Qt.rgba(1, 1, 1, 0.02);
            if (root.isPrimary) {
                return root.down ? "#74c7ec" : (root.hovered ? "#b4befe" : "#89b4fa");
            }
            if (root.isDestructive) {
                return root.down ? Qt.rgba(0.95, 0.54, 0.66, 0.3) : (root.hovered ? Qt.rgba(0.95, 0.54, 0.66, 0.22) : Qt.rgba(0.95, 0.54, 0.66, 0.12));
            }
            return root.down ? Qt.rgba(1, 1, 1, 0.16) : (root.hovered ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.05));
        }

        border.color: {
            if (!root.enabled) return Qt.rgba(1, 1, 1, 0.05);
            if (root.isPrimary) return "#89b4fa";
            if (root.isDestructive) return Qt.rgba(0.95, 0.54, 0.66, 0.35);
            if (root.activeFocus) return "#89b4fa";
            return root.hovered ? Qt.rgba(1, 1, 1, 0.20) : Qt.rgba(1, 1, 1, 0.10);
        }
        border.width: 1

        Behavior on color { ColorAnimation { duration: 120 } }
        Behavior on border.color { ColorAnimation { duration: 120 } }
    }

    contentItem: RowLayout {
        id: rowLayout
        spacing: 6
        anchors.centerIn: parent

        Kirigami.Icon {
            visible: root.iconSource.length > 0
            source: root.iconSource
            isMask: true
            implicitWidth: 15
            implicitHeight: 15
            color: {
                if (!root.enabled) return "#6c7086";
                if (root.isPrimary) return "#11111b";
                if (root.isDestructive) return "#f38ba8";
                return root.hovered ? "#ffffff" : "#cdd6f4";
            }
        }

        Text {
            text: root.text
            font.pixelSize: 12
            font.weight: root.isPrimary ? Font.Bold : Font.Medium
            color: {
                if (!root.enabled) return "#6c7086";
                if (root.isPrimary) return "#11111b";
                if (root.isDestructive) return "#f38ba8";
                return root.hovered ? "#ffffff" : "#cdd6f4";
            }
        }
    }
}
