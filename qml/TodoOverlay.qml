import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Rectangle {
    id: root

    readonly property real uiScale: (settings && settings.todoScale) ? settings.todoScale : 1.20

    // Base dimensions for proportional scaling
    readonly property int baseWidth: 290
    readonly property int baseHeight: 260

    implicitWidth: {
        let target = Math.round(baseWidth * uiScale);
        if (parent) {
            let maxW = Math.max(200, parent.width - 48);
            return Math.min(target, maxW);
        }
        return target;
    }
    implicitHeight: {
        let target = Math.round(baseHeight * uiScale);
        if (parent) {
            let maxH = Math.max(160, parent.height - 48);
            return Math.min(target, maxH);
        }
        return target;
    }

    color: "transparent"
    radius: Math.round(14 * Math.min(1.2, uiScale))
    border.color: contrastManager.buttonBorder
    border.width: 1

    Behavior on implicitWidth {
        NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
    }
    Behavior on implicitHeight {
        NumberAnimation { duration: 150; easing.type: Easing.OutQuad }
    }

    FrostedBackground {
        targetItem: root
        radius: root.radius
    }

    function submitTask() {
        let raw = taskInput.text;
        let trimmed = raw.trim();
        if (trimmed.length > 0) {
            taskInput.text = "";
            todoManager.addTask(trimmed);
            taskInput.focus = false;
            if (window && window.contentItem) {
                window.contentItem.forceActiveFocus();
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Math.max(10, Math.round(14 * root.uiScale))
        spacing: Math.max(6, Math.round(8 * root.uiScale))

        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: Math.max(6, Math.round(8 * root.uiScale))

            Kirigami.Icon {
                source: "qrc:/icons/lucide/list-todo.svg"
                isMask: true
                implicitWidth: Math.max(16, Math.round(18 * root.uiScale))
                implicitHeight: Math.max(16, Math.round(18 * root.uiScale))
                color: contrastManager.isDark ? "#a6e3a1" : "#16a34a"
            }

            Text {
                text: "Focus Tasks"
                font.pixelSize: Math.max(11, Math.round(13 * root.uiScale))
                font.weight: Font.DemiBold
                color: contrastManager.primaryTextColor
            }

            Rectangle {
                Layout.preferredWidth: countText.implicitWidth + Math.max(10, Math.round(12 * root.uiScale))
                Layout.preferredHeight: Math.max(16, Math.round(18 * root.uiScale))
                implicitWidth: Layout.preferredWidth
                implicitHeight: Layout.preferredHeight
                radius: height / 2
                color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(0, 0, 0, 0.08)

                Text {
                    id: countText
                    anchors.centerIn: parent
                    text: todoManager.activeCount.toString()
                    font.pixelSize: Math.max(9, Math.round(10 * root.uiScale))
                    font.weight: Font.Medium
                    color: contrastManager.secondaryTextColor
                }
            }

            Item { Layout.fillWidth: true }

            // Clear Completed Button
            ToolButton {
                id: clearBtn
                visible: todoManager.count > todoManager.activeCount
                Layout.preferredWidth: Math.max(26, Math.round(28 * root.uiScale))
                Layout.preferredHeight: Math.max(26, Math.round(28 * root.uiScale))
                implicitWidth: Layout.preferredWidth
                implicitHeight: Layout.preferredHeight
                focusPolicy: Qt.TabFocus
                ToolTip.text: "Clear completed tasks"
                ToolTip.visible: hovered

                background: Rectangle {
                    radius: 6
                    color: clearBtn.pressed ? Qt.rgba(1, 1, 1, 0.20) : (clearBtn.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.10)) : "transparent")
                    border.color: clearBtn.activeFocus ? contrastManager.accentColor : "transparent"
                    border.width: 1
                }

                contentItem: Item {
                    implicitWidth: Math.max(13, Math.round(15 * root.uiScale))
                    implicitHeight: Math.max(13, Math.round(15 * root.uiScale))
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/trash-2.svg"
                        isMask: true
                        width: Math.max(13, Math.round(15 * root.uiScale))
                        height: Math.max(13, Math.round(15 * root.uiScale))
                        color: clearBtn.hovered ? "#f38ba8" : contrastManager.secondaryTextColor
                    }
                }

                onClicked: todoManager.clearCompleted()
            }
        }

        // Add Task Input Row
        RowLayout {
            Layout.fillWidth: true
            spacing: Math.max(6, Math.round(8 * root.uiScale))

            Rectangle {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                implicitHeight: Math.max(30, Math.round(34 * root.uiScale))
                radius: Math.max(5, Math.round(7 * root.uiScale))
                color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(0, 0, 0, 0.06)
                border.color: taskInput.activeFocus ? contrastManager.accentColor : contrastManager.buttonBorder
                border.width: 1

                TextField {
                    id: taskInput
                    anchors.fill: parent
                    anchors.leftMargin: Math.max(8, Math.round(10 * root.uiScale))
                    anchors.rightMargin: Math.max(8, Math.round(10 * root.uiScale))
                    placeholderText: "Add a focus task..."
                    placeholderTextColor: contrastManager.secondaryTextColor
                    font.pixelSize: Math.max(10, Math.round(11 * root.uiScale))
                    color: contrastManager.primaryTextColor
                    background: null
                    verticalAlignment: TextInput.AlignVCenter
                    selectByMouse: true

                    onAccepted: root.submitTask()
                }
            }

            AbstractButton {
                id: addBtn
                Layout.alignment: Qt.AlignVCenter
                padding: 0
                Layout.preferredWidth: Math.max(32, Math.round(34 * root.uiScale))
                Layout.preferredHeight: Math.max(32, Math.round(34 * root.uiScale))
                implicitWidth: Layout.preferredWidth
                implicitHeight: Layout.preferredHeight
                focusPolicy: Qt.TabFocus
                ToolTip.text: taskInput.text.trim().length > 0 ? "Add task (Enter)" : "Open task input"
                ToolTip.visible: hovered

                background: Rectangle {
                    anchors.fill: parent
                    radius: Math.max(5, Math.round(7 * root.uiScale))
                    color: {
                        if (!addBtn.enabled) return Qt.rgba(1, 1, 1, 0.05);
                        if (addBtn.pressed) return contrastManager.isDark ? "#b4befe" : "#2563eb";
                        if (addBtn.hovered) return contrastManager.accentColor;
                        return taskInput.text.trim().length > 0 ? Qt.rgba(0.54, 0.71, 0.98, 0.40) : Qt.rgba(0.54, 0.71, 0.98, 0.22);
                    }
                    border.color: addBtn.activeFocus ? contrastManager.accentColor : (addBtn.hovered ? contrastManager.accentColor : contrastManager.buttonBorder)
                    border.width: addBtn.activeFocus ? 1.5 : 1

                    Behavior on color {
                        ColorAnimation { duration: 120 }
                    }
                }

                contentItem: Item {
                    implicitWidth: Math.max(15, Math.round(16 * root.uiScale))
                    implicitHeight: Math.max(15, Math.round(16 * root.uiScale))
                    Kirigami.Icon {
                        anchors.centerIn: parent
                        source: "qrc:/icons/lucide/plus.svg"
                        isMask: true
                        width: Math.max(15, Math.round(16 * root.uiScale))
                        height: Math.max(15, Math.round(16 * root.uiScale))
                        color: {
                            if (!addBtn.enabled) return contrastManager.secondaryTextColor;
                            if (addBtn.pressed || addBtn.hovered) return contrastManager.isDark ? "#11111b" : "#ffffff";
                            return contrastManager.primaryTextColor;
                        }
                    }
                }

                onClicked: root.submitTask()
            }
        }

        // Divider
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(0, 0, 0, 0.10)
        }

        // Empty state
        Text {
            visible: todoManager.count === 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "No tasks yet.\nAdd a task to stay focused!"
            font.pixelSize: Math.max(9, Math.round(11 * root.uiScale))
            color: contrastManager.secondaryTextColor
            lineHeight: 1.4
        }

        // Tasks List
        ListView {
            id: taskListView
            visible: todoManager.count > 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: todoManager
            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {
                active: taskListView.moving || taskListView.flicking
                width: Math.max(3, Math.round(4 * root.uiScale))
                contentItem: Rectangle {
                    radius: 2
                    color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.25) : Qt.rgba(0, 0, 0, 0.25)
                }
            }

            delegate: Rectangle {
                id: taskDelegate
                width: taskListView.width
                height: Math.max(30, Math.round(34 * root.uiScale))
                radius: Math.max(4, Math.round(6 * root.uiScale))
                color: itemHover.hovered ? (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(0, 0, 0, 0.06)) : "transparent"

                HoverHandler {
                    id: itemHover
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: Math.max(6, Math.round(8 * root.uiScale))
                    anchors.rightMargin: Math.max(6, Math.round(8 * root.uiScale))
                    spacing: Math.max(6, Math.round(8 * root.uiScale))

                    // Custom Checkbox
                    Rectangle {
                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: Math.max(18, Math.round(20 * root.uiScale))
                        Layout.preferredHeight: Math.max(18, Math.round(20 * root.uiScale))
                        radius: 5
                        color: model.completed ? (contrastManager.isDark ? "#a6e3a1" : "#16a34a") : "transparent"
                        border.color: model.completed ? (contrastManager.isDark ? "#a6e3a1" : "#16a34a") : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.35) : Qt.rgba(0, 0, 0, 0.35))
                        border.width: 1.5

                        TapHandler {
                            onTapped: todoManager.toggleCompleted(index)
                        }

                        Kirigami.Icon {
                            anchors.centerIn: parent
                            visible: model.completed
                            source: "qrc:/icons/lucide/check.svg"
                            isMask: true
                            implicitWidth: Math.max(11, Math.round(13 * root.uiScale))
                            implicitHeight: Math.max(11, Math.round(13 * root.uiScale))
                            color: "#181926"
                        }
                    }

                    // Task Text (strikes through on completion, preserves normal colors)
                    Text {
                        id: taskText
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        verticalAlignment: Text.AlignVCenter
                        text: model.text
                        font.pixelSize: Math.max(10, Math.round(11 * root.uiScale))
                        font.strikeout: model.completed
                        color: model.completed ? contrastManager.secondaryTextColor : contrastManager.primaryTextColor
                        elide: Text.ElideRight

                        TapHandler {
                            onTapped: todoManager.toggleCompleted(index)
                        }
                    }

                    // Delete Task Button
                    AbstractButton {
                        id: delBtn
                        Layout.alignment: Qt.AlignVCenter
                        padding: 0
                        visible: itemHover.hovered || delBtn.activeFocus
                        Layout.preferredWidth: Math.max(26, Math.round(28 * root.uiScale))
                        Layout.preferredHeight: Math.max(26, Math.round(28 * root.uiScale))
                        implicitWidth: Layout.preferredWidth
                        implicitHeight: Layout.preferredHeight
                        focusPolicy: Qt.TabFocus
                        ToolTip.text: "Delete task"
                        ToolTip.visible: hovered

                        background: Rectangle {
                            anchors.fill: parent
                            radius: 5
                            color: delBtn.pressed ? Qt.rgba(0.95, 0.4, 0.45, 0.45) : (delBtn.hovered ? Qt.rgba(0.95, 0.4, 0.45, 0.25) : "transparent")
                            border.color: delBtn.activeFocus ? "#f38ba8" : "transparent"
                            border.width: 1
                        }

                        contentItem: Item {
                            implicitWidth: Math.max(13, Math.round(14 * root.uiScale))
                            implicitHeight: Math.max(13, Math.round(14 * root.uiScale))
                            Kirigami.Icon {
                                anchors.centerIn: parent
                                source: "qrc:/icons/lucide/trash-2.svg"
                                isMask: true
                                width: Math.max(13, Math.round(14 * root.uiScale))
                                height: Math.max(13, Math.round(14 * root.uiScale))
                                color: delBtn.hovered ? "#f38ba8" : (delBtn.pressed ? "#eba0ac" : contrastManager.secondaryTextColor)
                            }
                        }

                        onClicked: todoManager.deleteTask(index)
                    }
                }
            }
        }
    }
}
