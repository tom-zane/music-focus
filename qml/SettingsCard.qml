import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    default property alias content: contentLayout.data
    property string title: ""
    property string description: ""
    property string iconSource: ""
    property var headerAction: null

    Layout.fillWidth: true
    implicitHeight: mainCol.implicitHeight + 28
    radius: 10
    color: Qt.rgba(1, 1, 1, 0.035)
    border.color: Qt.rgba(1, 1, 1, 0.08)
    border.width: 1

    ColumnLayout {
        id: mainCol
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 14
        spacing: 12

        // Optional Card Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            visible: root.title.length > 0

            Rectangle {
                visible: root.iconSource.length > 0
                implicitWidth: 28
                implicitHeight: 28
                radius: 6
                color: Qt.rgba(0.54, 0.71, 0.98, 0.12)
                Layout.alignment: Qt.AlignVCenter

                Kirigami.Icon {
                    anchors.centerIn: parent
                    source: root.iconSource
                    isMask: true
                    implicitWidth: 16
                    implicitHeight: 16
                    color: "#89b4fa"
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Layout.alignment: Qt.AlignVCenter

                Text {
                    text: root.title
                    font.pixelSize: 13
                    font.weight: Font.DemiBold
                    color: "#cdd6f4"
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

            Loader {
                sourceComponent: root.headerAction
                visible: root.headerAction !== null
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Qt.rgba(1, 1, 1, 0.06)
            visible: root.title.length > 0 && contentLayout.children.length > 0
        }

        // Card Content Slot
        ColumnLayout {
            id: contentLayout
            Layout.fillWidth: true
            spacing: 10
        }
    }
}
