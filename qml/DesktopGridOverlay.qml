import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int columns: layoutController.gridColumns
    property int rows: layoutController.gridRows
    property real insetLeft: 16
    property real insetTop: 16
    property real insetRight: 16
    property real insetBottom: 16

    readonly property real usableWidth: Math.max(1, width - insetLeft - insetRight)
    readonly property real usableHeight: Math.max(1, height - insetTop - insetBottom)

    visible: layoutController.editing

    // Semi-transparent backdrop to slightly mute wallpaper while editing
    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(0.05, 0.06, 0.10, 0.45)
    }

    // High-performance single Canvas rendering responsive grid lines & dots
    Canvas {
        id: gridCanvas
        anchors.fill: parent
        renderTarget: Canvas.FramebufferObject
        renderStrategy: Canvas.Threaded

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            var startX = root.insetLeft;
            var startY = root.insetTop;
            var uW = root.usableWidth;
            var uH = root.usableHeight;
            var cols = root.columns;
            var rws = root.rows;

            // Draw outer usable layout boundary
            ctx.strokeStyle = "rgba(137, 180, 250, 0.35)";
            ctx.lineWidth = 1;
            ctx.strokeRect(startX, startY, uW, uH);

            // Draw major grid guides every 10 columns and 10 rows
            ctx.strokeStyle = "rgba(137, 180, 250, 0.12)";
            ctx.beginPath();

            for (var c = 10; c < cols; c += 10) {
                var x = Math.round(startX + (c * uW) / cols);
                ctx.moveTo(x + 0.5, startY);
                ctx.lineTo(x + 0.5, startY + uH);
            }

            for (var r = 10; r < rws; r += 10) {
                var y = Math.round(startY + (r * uH) / rws);
                ctx.moveTo(startX, y + 0.5);
                ctx.lineTo(startX + uW, y + 0.5);
            }
            ctx.stroke();

            // Draw subtle intersection dots for minor grid cells (every 5 units)
            ctx.fillStyle = "rgba(205, 214, 244, 0.18)";
            for (var dc = 5; dc < cols; dc += 5) {
                var dotX = Math.round(startX + (dc * uW) / cols);
                for (var dr = 5; dr < rws; dr += 5) {
                    var dotY = Math.round(startY + (dr * uH) / rws);
                    ctx.fillRect(dotX - 1, dotY - 1, 2, 2);
                }
            }
        }

        Connections {
            target: root
            function onWidthChanged() { gridCanvas.requestPaint(); }
            function onHeightChanged() { gridCanvas.requestPaint(); }
        }

        Connections {
            target: layoutController
            function onEditingChanged() {
                if (layoutController.editing) {
                    gridCanvas.requestPaint();
                }
            }
        }
    }
}
