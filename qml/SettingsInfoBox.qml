import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    property string text: ""
    property string iconSource: "qrc:/icons/lucide/info.svg"
    property bool isWarning: false
    property bool isSuccess: false

    Layout.fillWidth: true
    implicitHeight: Math.max(38, rowLayout.implicitHeight + 20)
    radius: 8

    color: {
        if (isWarning) return Qt.rgba(0.98, 0.89, 0.69, 0.08);
        if (isSuccess) return Qt.rgba(0.65, 0.89, 0.63, 0.08);
        return Qt.rgba(0.54, 0.71, 0.98, 0.08);
    }
    border.color: {
        if (isWarning) return Qt.rgba(0.98, 0.89, 0.69, 0.25);
        if (isSuccess) return Qt.rgba(0.65, 0.89, 0.63, 0.25);
        return Qt.rgba(0.54, 0.71, 0.98, 0.20);
    }
    border.width: 1

    RowLayout {
        id: rowLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        spacing: 10

        Kirigami.Icon {
            source: root.iconSource
            isMask: true
            implicitWidth: 16
            implicitHeight: 16
            Layout.alignment: Qt.AlignTop
            color: {
                if (root.isWarning) return "#f9e2af";
                if (root.isSuccess) return "#a6e3a1";
                return "#89b4fa";
            }
        }

        Text {
            text: root.text
            font.pixelSize: 11
            color: {
                if (root.isWarning) return "#f9e2af";
                if (root.isSuccess) return "#a6e3a1";
                return "#cdd6f4";
            }
            lineHeight: 1.3
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }
    }
}
