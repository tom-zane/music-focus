import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

ScrollView {
    id: root
    clip: true
    contentWidth: availableWidth
    ScrollBar.vertical.policy: ScrollBar.AsNeeded

    property int currentYear: new Date().getFullYear()
    property int currentMonth: new Date().getMonth() + 1
    property string selectedDateString: Qt.formatDate(new Date(), "yyyy-MM-dd")

    property var currentMonthData: pomodoroManager ? pomodoroManager.monthData(currentYear, currentMonth) : ({})
    property var selectedDaySessions: pomodoroManager ? pomodoroManager.sessionsForDate(selectedDateString) : []

    function updateMonth() {
        if (pomodoroManager) {
            currentMonthData = pomodoroManager.monthData(currentYear, currentMonth);
            selectedDaySessions = pomodoroManager.sessionsForDate(selectedDateString);
        }
    }

    Connections {
        target: pomodoroManager
        function onHistoryChanged() {
            root.updateMonth();
        }
    }

    ColumnLayout {
        width: root.availableWidth - 12
        spacing: 16

        // Top Action Bar
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Text {
                    text: "Focus History & Analytics"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    color: "#89b4fa"
                }
                Text {
                    text: "Accurate historical records of your completed study intervals"
                    font.pixelSize: 11
                    color: "#9399b2"
                }
            }
        }

        // =====================================================================
        // SECTION A: TODAY'S FOCUS
        // =====================================================================
        SettingsCard {
            title: "Today's Focus"
            description: "Actual completed study time recorded for today"
            iconSource: "qrc:/icons/lucide/timer.svg"

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 90
                radius: 10
                color: Qt.rgba(0.65, 0.89, 0.63, 0.12)
                border.color: contrastManager.isDark ? "#a6e3a1" : "#16a34a"
                border.width: 1.5

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 20

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: "TODAY"
                            font.pixelSize: 10
                            font.weight: Font.Bold
                            color: contrastManager.isDark ? "#a6e3a1" : "#16a34a"
                        }

                        Text {
                            id: todayTimeText
                            text: (pomodoroManager ? pomodoroManager.todayFormattedFocusTime : "0m") + " focused"
                            font.pixelSize: 22
                            font.weight: Font.Bold
                            color: "#ffffff"
                        }

                        Text {
                            text: (pomodoroManager ? pomodoroManager.todaySessionCount : 0) + " completed sessions"
                            font.pixelSize: 12
                            font.weight: Font.Medium
                            color: "#cdd6f4"
                        }
                    }

                    // Progress ring or compact visual stat badge
                    Rectangle {
                        implicitWidth: 54
                        implicitHeight: 54
                        radius: 27
                        color: Qt.rgba(1, 1, 1, 0.06)
                        border.color: Qt.rgba(1, 1, 1, 0.12)
                        border.width: 1

                        Kirigami.Icon {
                            anchors.centerIn: parent
                            source: "qrc:/icons/lucide/check.svg"
                            isMask: true
                            implicitWidth: 24
                            implicitHeight: 24
                            color: (pomodoroManager && pomodoroManager.todaySessionCount > 0) ? "#a6e3a1" : "#6c7086"
                        }
                    }
                }
            }
        }

        // =====================================================================
        // SECTION B: LAST 7 DAYS
        // =====================================================================
        SettingsCard {
            title: "Last 7 Days"
            description: "Completed sessions and intervals over the past week"
            iconSource: "qrc:/icons/lucide/activity.svg"

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Repeater {
                    model: pomodoroManager ? pomodoroManager.last7DaysData() : []

                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: dayCol.implicitHeight + 16
                        radius: 8
                        color: Qt.rgba(1, 1, 1, 0.03)
                        border.color: Qt.rgba(1, 1, 1, 0.08)
                        border.width: 1

                        ColumnLayout {
                            id: dayCol
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 6

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                Text {
                                    text: modelData.dayLabel
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    color: "#ffffff"
                                }

                                Item { Layout.fillWidth: true }

                                Text {
                                    text: modelData.sessionCount > 0
                                        ? (modelData.formattedFocusTime + " (" + modelData.sessionCount + " session" + (modelData.sessionCount > 1 ? "s" : "") + ")")
                                        : "No focus sessions"
                                    font.pixelSize: 11
                                    font.weight: modelData.sessionCount > 0 ? Font.Medium : Font.Normal
                                    color: modelData.sessionCount > 0 ? "#a6e3a1" : "#6c7086"
                                }
                            }

                            // Compact horizontal session chips
                            Flow {
                                Layout.fillWidth: true
                                spacing: 6
                                visible: modelData.sessions && modelData.sessions.length > 0

                                Repeater {
                                    model: modelData.sessions || []

                                    Rectangle {
                                        implicitWidth: sessionChipRow.implicitWidth + 12
                                        implicitHeight: 22
                                        radius: 4
                                        color: Qt.rgba(0.65, 0.89, 0.63, 0.15)
                                        border.color: Qt.rgba(0.65, 0.89, 0.63, 0.35)
                                        border.width: 1

                                        RowLayout {
                                            id: sessionChipRow
                                            anchors.centerIn: parent
                                            spacing: 4

                                            Text {
                                                text: modelData.timeRange
                                                font.pixelSize: 10
                                                font.family: "monospace"
                                                color: "#cdd6f4"
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // =====================================================================
        // SECTION C: MONTHLY CALENDAR AND HEATMAP
        // =====================================================================
        SettingsCard {
            title: "Monthly Focus Calendar & Heatmap"
            description: "Visual activity heatmap of study volume across calendar months"
            iconSource: "qrc:/icons/lucide/layout-grid.svg"

            // Calendar Navigation Header
            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    ToolTip.text: "Previous Month"
                    ToolTip.visible: hovered
                    onClicked: {
                        if (root.currentMonth === 1) {
                            root.currentMonth = 12;
                            root.currentYear -= 1;
                        } else {
                            root.currentMonth -= 1;
                        }
                        root.updateMonth();
                    }
                    contentItem: Kirigami.Icon {
                        source: "qrc:/icons/lucide/chevron-left.svg"
                        isMask: true
                        implicitWidth: 16
                        implicitHeight: 16
                        color: "#ffffff"
                    }
                }

                // Clickable Month & Year Heading opening instant selector
                Rectangle {
                    implicitWidth: headingRow.implicitWidth + 16
                    implicitHeight: 32
                    radius: 6
                    color: monthPickerHover.hovered ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(1, 1, 1, 0.05)
                    border.color: monthPickerHover.hovered ? "#89b4fa" : Qt.rgba(1, 1, 1, 0.12)
                    border.width: 1

                    HoverHandler { id: monthPickerHover }
                    TapHandler {
                        onTapped: monthYearDialog.open()
                    }

                    ToolTip.text: "Click to jump to any month and year"
                    ToolTip.visible: monthPickerHover.hovered

                    RowLayout {
                        id: headingRow
                        anchors.centerIn: parent
                        spacing: 6

                        Text {
                            text: (root.currentMonthData.monthName ? root.currentMonthData.monthName : "October") + " " + root.currentYear
                            font.pixelSize: 13
                            font.weight: Font.Bold
                            color: "#ffffff"
                        }

                        Kirigami.Icon {
                            source: "qrc:/icons/lucide/chevron-down.svg"
                            isMask: true
                            implicitWidth: 12
                            implicitHeight: 12
                            color: "#89b4fa"
                        }
                    }
                }

                ToolButton {
                    implicitWidth: 32
                    implicitHeight: 32
                    ToolTip.text: "Next Month"
                    ToolTip.visible: hovered
                    onClicked: {
                        if (root.currentMonth === 12) {
                            root.currentMonth = 1;
                            root.currentYear += 1;
                        } else {
                            root.currentMonth += 1;
                        }
                        root.updateMonth();
                    }
                    contentItem: Kirigami.Icon {
                        source: "qrc:/icons/lucide/chevron-right.svg"
                        isMask: true
                        implicitWidth: 16
                        implicitHeight: 16
                        color: "#ffffff"
                    }
                }

                Item { Layout.fillWidth: true }

                SettingsButton {
                    text: "Jump to Today"
                    iconSource: "qrc:/icons/lucide/rotate-ccw.svg"
                    onClicked: {
                        var now = new Date();
                        root.currentYear = now.getFullYear();
                        root.currentMonth = now.getMonth() + 1;
                        root.selectedDateString = Qt.formatDate(now, "yyyy-MM-dd");
                        root.updateMonth();
                    }
                }
            }

            // Month & Year Picker Dialog
            Dialog {
                id: monthYearDialog
                title: "Select Month and Year"
                modal: true
                anchors.centerIn: Overlay.overlay
                standardButtons: Dialog.Ok | Dialog.Cancel

                ColumnLayout {
                    spacing: 12
                    RowLayout {
                        spacing: 8
                        Text { text: "Year:"; color: "#ffffff"; font.pixelSize: 12 }
                        SpinBox {
                            id: yearPickerSpin
                            from: 2020
                            to: 2040
                            value: root.currentYear
                            editable: true
                        }
                    }

                    GridLayout {
                        columns: 4
                        rowSpacing: 6
                        columnSpacing: 6

                        Repeater {
                            model: ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"]
                            Button {
                                text: modelData
                                highlighted: (index + 1) === root.currentMonth
                                onClicked: {
                                    root.currentMonth = index + 1;
                                    root.currentYear = yearPickerSpin.value;
                                    root.updateMonth();
                                    monthYearDialog.close();
                                }
                            }
                        }
                    }
                }

                onAccepted: {
                    root.currentYear = yearPickerSpin.value;
                    root.updateMonth();
                }
            }

            // 7-Column Weekday Header
            RowLayout {
                Layout.fillWidth: true
                spacing: 4

                Repeater {
                    model: ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"]
                    Item {
                        Layout.fillWidth: true
                        height: 24

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            font.pixelSize: 10
                            font.weight: Font.DemiBold
                            color: "#9399b2"
                        }
                    }
                }
            }

            // Calendar Heatmap Grid
            GridLayout {
                id: calendarGrid
                Layout.fillWidth: true
                columns: 7
                rowSpacing: 4
                columnSpacing: 4

                // Leading empty cells before 1st of month
                Repeater {
                    model: (root.currentMonthData.firstDayOfWeek ? root.currentMonthData.firstDayOfWeek - 1 : 0)

                    Rectangle {
                        Layout.fillWidth: true
                        implicitHeight: 46
                        radius: 6
                        color: "transparent"
                    }
                }

                // Month Day Cells
                Repeater {
                    model: root.currentMonthData.days || []

                    Rectangle {
                        id: dayCell
                        Layout.fillWidth: true
                        implicitHeight: 46
                        radius: 6
                        readonly property bool isSelected: modelData.dateString === root.selectedDateString

                        // Heatmap fill colors matching specification scale
                        color: {
                            var intVal = modelData.heatmapIntensity;
                            if (intVal === 5) return "#22c55e"; // 10h+
                            if (intVal === 4) return Qt.rgba(0.20, 0.70, 0.35, 0.85); // 8h
                            if (intVal === 3) return Qt.rgba(0.20, 0.60, 0.30, 0.65); // 6h
                            if (intVal === 2) return Qt.rgba(0.20, 0.50, 0.28, 0.45); // 4h
                            if (intVal === 1) return Qt.rgba(0.20, 0.45, 0.25, 0.28); // 2h
                            return Qt.rgba(1, 1, 1, 0.03); // < 2h
                        }

                        border.color: isSelected
                            ? "#89b4fa"
                            : (modelData.isToday ? "#a6e3a1" : (cellHover.hovered ? Qt.rgba(1, 1, 1, 0.25) : Qt.rgba(1, 1, 1, 0.08)))
                        border.width: isSelected ? 2 : (modelData.isToday ? 1.5 : 1)

                        HoverHandler { id: cellHover }
                        TapHandler {
                            onTapped: {
                                root.selectedDateString = modelData.dateString;
                                if (pomodoroManager) {
                                    root.selectedDaySessions = pomodoroManager.sessionsForDate(root.selectedDateString);
                                }
                            }
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 4
                            spacing: 2

                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    text: modelData.day.toString()
                                    font.pixelSize: 10
                                    font.weight: modelData.isToday ? Font.Bold : Font.Normal
                                    color: (modelData.heatmapIntensity >= 4) ? "#11111b" : "#ffffff"
                                }
                                Item { Layout.fillWidth: true }
                                Rectangle {
                                    visible: modelData.isToday
                                    implicitWidth: 4
                                    implicitHeight: 4
                                    radius: 2
                                    color: (modelData.heatmapIntensity >= 4) ? "#11111b" : "#a6e3a1"
                                }
                            }

                            Item { Layout.fillHeight: true }

                            Text {
                                text: modelData.formattedFocusTime ? modelData.formattedFocusTime : ""
                                font.pixelSize: 9
                                font.weight: Font.DemiBold
                                color: (modelData.heatmapIntensity >= 4) ? "#11111b" : "#cdd6f4"
                                elide: Text.ElideRight
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }

            // Heatmap Legend
            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 6

                Text {
                    text: "Intensity:"
                    font.pixelSize: 10
                    color: "#9399b2"
                }

                Repeater {
                    model: [
                        { label: "< 2h", color: Qt.rgba(1, 1, 1, 0.06) },
                        { label: "2h", color: Qt.rgba(0.20, 0.45, 0.25, 0.28) },
                        { label: "4h", color: Qt.rgba(0.20, 0.50, 0.28, 0.45) },
                        { label: "6h", color: Qt.rgba(0.20, 0.60, 0.30, 0.65) },
                        { label: "8h", color: Qt.rgba(0.20, 0.70, 0.35, 0.85) },
                        { label: "10h+", color: "#22c55e" }
                    ]

                    RowLayout {
                        spacing: 3
                        Rectangle {
                            implicitWidth: 12
                            implicitHeight: 12
                            radius: 2
                            color: modelData.color
                            border.color: Qt.rgba(1, 1, 1, 0.15)
                            border.width: 1
                        }
                        Text {
                            text: modelData.label
                            font.pixelSize: 9
                            color: "#9399b2"
                        }
                    }
                }
            }

            // Selected Day Detail Panel
            Rectangle {
                Layout.fillWidth: true
                implicitHeight: detailCol.implicitHeight + 20
                radius: 8
                color: Qt.rgba(0, 0, 0, 0.25)
                border.color: Qt.rgba(0.54, 0.71, 0.98, 0.25)
                border.width: 1

                ColumnLayout {
                    id: detailCol
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Text {
                            text: "Sessions on " + root.selectedDateString
                            font.pixelSize: 12
                            font.weight: Font.Bold
                            color: "#89b4fa"
                        }
                        Item { Layout.fillWidth: true }
                        Text {
                            text: root.selectedDaySessions.length + " completed session" + (root.selectedDaySessions.length === 1 ? "" : "s")
                            font.pixelSize: 11
                            color: "#cdd6f4"
                        }
                    }

                    // Empty state if no sessions
                    Text {
                        visible: root.selectedDaySessions.length === 0
                        text: "No completed focus sessions recorded on this date."
                        font.pixelSize: 11
                        color: "#9399b2"
                        Layout.topMargin: 4
                    }

                    // Session list for selected date
                    Repeater {
                        model: root.selectedDaySessions

                        Rectangle {
                            Layout.fillWidth: true
                            implicitHeight: 32
                            radius: 6
                            color: Qt.rgba(1, 1, 1, 0.04)
                            border.color: Qt.rgba(1, 1, 1, 0.08)
                            border.width: 1

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                spacing: 10

                                Kirigami.Icon {
                                    source: "qrc:/icons/lucide/check.svg"
                                    isMask: true
                                    implicitWidth: 14
                                    implicitHeight: 14
                                    color: "#a6e3a1"
                                }

                                Text {
                                    text: "Focus Session " + (index + 1)
                                    font.pixelSize: 11
                                    font.weight: Font.DemiBold
                                    color: "#ffffff"
                                }

                                Item { Layout.fillWidth: true }

                                Text {
                                    text: modelData.timeRange
                                    font.pixelSize: 11
                                    font.family: "monospace"
                                    color: "#89b4fa"
                                }

                                Rectangle {
                                    implicitWidth: 1
                                    implicitHeight: 14
                                    color: Qt.rgba(1, 1, 1, 0.15)
                                }

                                Text {
                                    text: modelData.duration
                                    font.pixelSize: 11
                                    font.weight: Font.Medium
                                    color: "#a6e3a1"
                                }
                            }
                        }
                    }
                }
            }
        }

        // Bottom spacer
        Item { Layout.preferredHeight: 12 }
    }
}
