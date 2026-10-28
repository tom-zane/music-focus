import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property date currentTime: new Date()

    Timer {
        id: clockTimer
        interval: 1000
        running: true
        repeat: true
        onTriggered: root.currentTime = new Date()
    }

    Connections {
        target: settings
        function onClock24HourChanged() {
            root.currentTime = new Date();
        }
        function onClockShowSecondsChanged() {
            root.currentTime = new Date();
        }
    }

    readonly property bool showSeconds: settings ? settings.clockShowSeconds : false
    readonly property bool showDate: settings ? settings.clockShowDate : true
    readonly property bool is24Hour: settings ? settings.clock24Hour : true

    // Check if widget is in compact mode (below 16x8 grid units or compact pixel size)
    readonly property int currentGridW: layoutController ? layoutController.widgetWidth("clock")
                                                         : ((parent && parent.parent && parent.parent.gridW !== undefined) ? parent.parent.gridW : 30)
    readonly property int currentGridH: layoutController ? layoutController.widgetHeight("clock")
                                                         : ((parent && parent.parent && parent.parent.gridH !== undefined) ? parent.parent.gridH : 12)

    readonly property bool isCompact: currentGridW < 16 || currentGridH < 8 || width < 150 || height < 55

    // In compact mode, show strictly only the primary time (no seconds, no dates, no labels)
    readonly property bool effectiveShowSeconds: !isCompact && showSeconds
    readonly property bool effectiveShowDate: !isCompact && showDate

    // Dynamic padding scales with widget size (minimal in compact mode to preserve every pixel)
    readonly property real padding: isCompact ? Math.max(1, Math.min(width * 0.03, height * 0.05, 4))
                                              : Math.max(6, Math.min(width * 0.05, height * 0.08, 16))

    readonly property real availWidth: Math.max(1, width - padding * 2)
    readonly property real availHeight: Math.max(1, height - padding * 2)

    // Format time:
    // 12-hour format: "09:46 PM" (or "09:46:00 PM" if seconds shown at large size)
    // 24-hour format: "21:46" (or "21:46:00" if seconds shown at large size)
    readonly property string timeString: {
        var fmt = is24Hour ? (effectiveShowSeconds ? "hh:mm:ss" : "hh:mm")
                           : (effectiveShowSeconds ? "hh:mm:ss AP" : "hh:mm AP");
        return Qt.formatTime(currentTime, fmt);
    }

    readonly property string dateString: {
        if (availWidth < 140) {
            return Qt.formatDate(currentTime, "MMM d");
        } else if (availWidth < 220) {
            return Qt.formatDate(currentTime, "ddd, MMM d");
        } else {
            return Qt.formatDate(currentTime, "dddd, MMMM d");
        }
    }

    readonly property real radius: bgPanel.radius

    // Outer frosted glass container
    Rectangle {
        id: bgPanel
        anchors.fill: parent
        radius: Math.min(14, Math.floor(Math.min(root.width, root.height) / 2))
        color: contrastManager.isDark ? Qt.rgba(0.08, 0.09, 0.14, settings.overlayOpacity)
                                      : Qt.rgba(0.95, 0.96, 0.98, settings.overlayOpacity)
        border.color: contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.14) : Qt.rgba(0, 0, 0, 0.12)
        border.width: 1
        clip: true

        FrostedBackground {
            targetItem: parent
            radius: parent.radius
        }

        // --- 1. COMPACT PILL-SHAPED LAYOUT (Below 16x8 grid units, e.g. down to 4x2) ---
        Item {
            anchors.fill: parent
            anchors.margins: root.padding
            visible: root.isCompact

            Text {
                id: compactTimeText
                anchors.centerIn: parent
                width: parent.width
                height: parent.height
                text: root.timeString
                font.weight: Font.Bold
                font.pixelSize: {
                    var chars = root.is24Hour ? 5.2 : 8.2;
                    var wFit = root.availWidth / (chars * 0.58);
                    var hFit = root.availHeight * 0.80;
                    return Math.max(5, Math.min(hFit, wFit, 48));
                }
                color: contrastManager.primaryTextColor
                fontSizeMode: Text.Fit
                minimumPixelSize: 5
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        // --- 2. STANDARD / LARGER CARD LAYOUT (16x8 grid units and above) ---
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: root.padding
            spacing: Math.max(1, Math.min(6, root.availHeight * 0.04))
            visible: !root.isCompact

            Item { Layout.fillHeight: true }

            Text {
                id: standardTimeText
                text: root.timeString
                font.weight: Font.Bold
                font.pixelSize: {
                    var maxChars = (root.effectiveShowSeconds ? 8.6 : 5.4) + (root.is24Hour ? 0 : 3.4);
                    var widthBased = root.availWidth / (maxChars * 0.60);
                    var heightFactor = (root.effectiveShowDate && root.availHeight >= 45) ? 0.48 : 0.66;
                    return Math.max(10, Math.min(root.availHeight * heightFactor, widthBased, 76));
                }
                color: contrastManager.primaryTextColor
                fontSizeMode: Text.Fit
                minimumPixelSize: 8
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: root.availWidth
            }

            Text {
                id: standardDateText
                text: root.dateString
                visible: root.effectiveShowDate && (root.availHeight >= 42)
                font.pixelSize: Math.max(9, Math.min(standardTimeText.font.pixelSize * 0.34, root.availHeight * 0.22, 18))
                font.weight: Font.Medium
                color: contrastManager.secondaryTextColor
                fontSizeMode: Text.Fit
                minimumPixelSize: 8
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: root.availWidth
            }

            Item { Layout.fillHeight: true }
        }
    }
}
