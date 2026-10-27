import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Item {
    id: root

    required property string widgetId
    property string widgetTitle: ""

    property real insetLeft: 16
    property real insetTop: 16
    property real insetRight: 16
    property real insetBottom: 16

    readonly property real usableWidth: Math.max(1, parent ? (parent.width - insetLeft - insetRight) : 1000)
    readonly property real usableHeight: Math.max(1, parent ? (parent.height - insetTop - insetBottom) : 600)

    // Grid coordinates synced reactively from LayoutController
    property int gridX: layoutController.widgetX(widgetId)
    property int gridY: layoutController.widgetY(widgetId)
    property int gridW: layoutController.widgetWidth(widgetId)
    property int gridH: layoutController.widgetHeight(widgetId)
    property bool widgetVisible: layoutController.widgetVisible(widgetId)

    readonly property bool isSelected: layoutController.editing && layoutController.selectedWidgetId === widgetId
    readonly property bool isEditing: layoutController.editing

    function syncFromLayout() {
        gridX = layoutController.widgetX(widgetId);
        gridY = layoutController.widgetY(widgetId);
        gridW = layoutController.widgetWidth(widgetId);
        gridH = layoutController.widgetHeight(widgetId);
        widgetVisible = layoutController.widgetVisible(widgetId);
    }

    Component.onCompleted: syncFromLayout()

    Connections {
        target: layoutController
        function onWidgetChanged(id) {
            if (id === root.widgetId) {
                root.syncFromLayout();
            }
        }
        function onLayoutChanged() {
            root.syncFromLayout();
        }
    }

    // Rendered pixel coordinates (normalized, resolution-independent)
    x: Math.round(layoutController.gridToPixelX(gridX, usableWidth, insetLeft))
    y: Math.round(layoutController.gridToPixelY(gridY, usableHeight, insetTop))
    width: Math.max(1, Math.round(layoutController.gridToPixelWidth(gridW, usableWidth)))
    height: Math.max(1, Math.round(layoutController.gridToPixelHeight(gridH, usableHeight)))

    visible: widgetVisible

    default property alias content: contentPlaceholder.data

    // Child container holding the actual widget
    Item {
        id: contentPlaceholder
        anchors.fill: parent
        clip: false
    }

    // --- EDITING MODE OVERLAY & HANDLES ---
    Item {
        id: editOverlay
        anchors.fill: parent
        visible: root.isEditing
        z: root.isSelected ? 100 : 10

        // Tap on widget in edit mode selects it
        MouseArea {
            anchors.fill: parent
            enabled: root.isEditing && !dragArea.drag.active && !resizeArea.drag.active
            onClicked: {
                layoutController.selectedWidgetId = root.widgetId;
            }
        }

        // Occupied Grid Area Outline
        Rectangle {
            anchors.fill: parent
            color: root.isSelected ? Qt.rgba(0.54, 0.71, 0.98, 0.12) : Qt.rgba(0.54, 0.71, 0.98, 0.04)
            border.color: root.isSelected ? "#89b4fa" : Qt.rgba(0.54, 0.71, 0.98, 0.45)
            border.width: root.isSelected ? 2 : 1
            radius: 8

            // Top-left Info Badge showing title and grid dimensions
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: -10
                implicitWidth: badgeRow.implicitWidth + 14
                implicitHeight: 22
                radius: 4
                color: root.isSelected ? "#89b4fa" : Qt.rgba(0.12, 0.13, 0.18, 0.95)
                border.color: root.isSelected ? "#ffffff" : Qt.rgba(1, 1, 1, 0.25)
                border.width: 1

                RowLayout {
                    id: badgeRow
                    anchors.centerIn: parent
                    spacing: 4

                    Text {
                        text: (root.widgetTitle.length > 0 ? root.widgetTitle : root.widgetId) + " • (" + root.gridX + ", " + root.gridY + ") [" + root.gridW + "×" + root.gridH + "]"
                        font.pixelSize: 10
                        font.weight: Font.DemiBold
                        color: root.isSelected ? "#11111b" : "#cdd6f4"
                    }
                }
            }
        }

        // --- DRAG ANCHOR (TOP-RIGHT) ---
        Rectangle {
            id: dragAnchor
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: -12
            anchors.topMargin: -12
            implicitWidth: 28
            implicitHeight: 28
            radius: 14
            color: dragArea.drag.active ? "#a6e3a1" : (dragArea.containsMouse ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(0.12, 0.13, 0.18, 0.95) : Qt.rgba(0.95, 0.96, 0.98, 0.95)))
            border.color: dragArea.drag.active ? "#ffffff" : (dragArea.containsMouse ? "#ffffff" : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.40) : Qt.rgba(0, 0, 0, 0.30)))
            border.width: 1.5
            z: 20

            Kirigami.Icon {
                anchors.centerIn: parent
                source: "qrc:/icons/lucide/move.svg"
                isMask: true
                implicitWidth: 16
                implicitHeight: 16
                color: (dragArea.drag.active || dragArea.containsMouse) ? "#11111b" : contrastManager.primaryTextColor
            }

            ToolTip.text: "Drag anchor to move widget (snaps to grid)"
            ToolTip.visible: dragArea.containsMouse && !dragArea.drag.active

            MouseArea {
                id: dragArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.SizeAllCursor

                property real pressMouseParentX: 0
                property real pressMouseParentY: 0
                property real pressRootX: 0
                property real pressRootY: 0
                property int currentProposedGridX: root.gridX
                property int currentProposedGridY: root.gridY
                property bool canDrop: true

                drag.target: dummyDragTarget
                drag.axis: Drag.XAndYAxis

                Item {
                    id: dummyDragTarget
                }

                onPressed: (mouse) => {
                    layoutController.selectedWidgetId = root.widgetId;
                    var parentPt = mapToItem(root.parent, mouse.x, mouse.y);
                    pressMouseParentX = parentPt.x;
                    pressMouseParentY = parentPt.y;
                    pressRootX = root.x;
                    pressRootY = root.y;
                    currentProposedGridX = root.gridX;
                    currentProposedGridY = root.gridY;
                    canDrop = true;
                    movePreviewGhost.visible = true;
                }

                onPositionChanged: (mouse) => {
                    if (drag.active) {
                        var curParentPt = mapToItem(root.parent, mouse.x, mouse.y);
                        var deltaX = curParentPt.x - pressMouseParentX;
                        var deltaY = curParentPt.y - pressMouseParentY;
                        var targetX = pressRootX + deltaX;
                        var targetY = pressRootY + deltaY;

                        var proposedGx = layoutController.pixelToGridX(targetX, root.usableWidth, root.insetLeft);
                        var proposedGy = layoutController.pixelToGridY(targetY, root.usableHeight, root.insetTop);

                        // Clamp to screen bounds
                        proposedGx = Math.max(0, Math.min(layoutController.gridColumns - root.gridW, proposedGx));
                        proposedGy = Math.max(0, Math.min(layoutController.gridRows - root.gridH, proposedGy));

                        currentProposedGridX = proposedGx;
                        currentProposedGridY = proposedGy;

                        canDrop = layoutController.canMove(root.widgetId, proposedGx, proposedGy);

                        // Continuous position update during drag for live background synchronization
                        if (canDrop && (proposedGx !== root.gridX || proposedGy !== root.gridY)) {
                            var moved = layoutController.moveWidget(root.widgetId, proposedGx, proposedGy);
                            if (moved) {
                                root.syncFromLayout();
                            }
                        }
                    }
                }

                onReleased: {
                    movePreviewGhost.visible = false;
                    if (canDrop) {
                        var changed = layoutController.moveWidget(root.widgetId, currentProposedGridX, currentProposedGridY);
                        if (changed) {
                            root.syncFromLayout();
                        }
                    }
                }
            }
        }

        // --- RESIZE HANDLE (BOTTOM-RIGHT) ---
        Rectangle {
            id: resizeHandle
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: -10
            anchors.bottomMargin: -10
            implicitWidth: 26
            implicitHeight: 26
            radius: 6
            color: resizeArea.drag.active ? "#a6e3a1" : (resizeArea.containsMouse ? contrastManager.accentColor : (contrastManager.isDark ? Qt.rgba(0.12, 0.13, 0.18, 0.95) : Qt.rgba(0.95, 0.96, 0.98, 0.95)))
            border.color: resizeArea.drag.active ? "#ffffff" : (resizeArea.containsMouse ? "#ffffff" : (contrastManager.isDark ? Qt.rgba(1, 1, 1, 0.40) : Qt.rgba(0, 0, 0, 0.30)))
            border.width: 1.5
            z: 20

            Kirigami.Icon {
                anchors.centerIn: parent
                source: "qrc:/icons/lucide/scaling.svg"
                isMask: true
                implicitWidth: 15
                implicitHeight: 15
                color: (resizeArea.drag.active || resizeArea.containsMouse) ? "#11111b" : contrastManager.primaryTextColor
            }

            ToolTip.text: "Drag handle to resize width and height (snaps to grid)"
            ToolTip.visible: resizeArea.containsMouse && !resizeArea.drag.active

            MouseArea {
                id: resizeArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.SizeFDiagCursor

                property real pressMouseParentX: 0
                property real pressMouseParentY: 0
                property real pressRootW: root.width
                property real pressRootH: root.height
                property int currentProposedGridW: root.gridW
                property int currentProposedGridH: root.gridH
                property bool canResizeDrop: true

                drag.target: dummyResizeTarget
                drag.axis: Drag.XAndYAxis

                Item {
                    id: dummyResizeTarget
                }

                onPressed: (mouse) => {
                    layoutController.selectedWidgetId = root.widgetId;
                    var parentPt = mapToItem(root.parent, mouse.x, mouse.y);
                    pressMouseParentX = parentPt.x;
                    pressMouseParentY = parentPt.y;
                    pressRootW = root.width;
                    pressRootH = root.height;
                    currentProposedGridW = root.gridW;
                    currentProposedGridH = root.gridH;
                    canResizeDrop = true;
                    resizePreviewGhost.visible = true;
                }

                onPositionChanged: (mouse) => {
                    if (drag.active) {
                        var curParentPt = mapToItem(root.parent, mouse.x, mouse.y);
                        var deltaW = curParentPt.x - pressMouseParentX;
                        var deltaH = curParentPt.y - pressMouseParentY;

                        var proposedPixelW = Math.max(10, pressRootW + deltaW);
                        var proposedPixelH = Math.max(10, pressRootH + deltaH);

                        var proposedGw = layoutController.pixelToGridWidth(proposedPixelW, root.usableWidth);
                        var proposedGh = layoutController.pixelToGridHeight(proposedPixelH, root.usableHeight);

                        var minW = layoutController.widgetMinWidth(root.widgetId);
                        var minH = layoutController.widgetMinHeight(root.widgetId);
                        var maxW = layoutController.widgetMaxWidth(root.widgetId);
                        var maxH = layoutController.widgetMaxHeight(root.widgetId);

                        proposedGw = Math.max(minW, Math.min(maxW, proposedGw));
                        proposedGh = Math.max(minH, Math.min(maxH, proposedGh));

                        // Clamp to boundary
                        proposedGw = Math.min(layoutController.gridColumns - root.gridX, proposedGw);
                        proposedGh = Math.min(layoutController.gridRows - root.gridY, proposedGh);

                        currentProposedGridW = proposedGw;
                        currentProposedGridH = proposedGh;

                        canResizeDrop = layoutController.canResize(root.widgetId, proposedGw, proposedGh);

                        // Continuous dimension update during resize for live background synchronization
                        if (canResizeDrop && (proposedGw !== root.gridW || proposedGh !== root.gridH)) {
                            var resized = layoutController.resizeWidget(root.widgetId, proposedGw, proposedGh);
                            if (resized) {
                                root.syncFromLayout();
                            }
                        }
                    }
                }

                onReleased: {
                    resizePreviewGhost.visible = false;
                    if (canResizeDrop) {
                        var changed = layoutController.resizeWidget(root.widgetId, currentProposedGridW, currentProposedGridH);
                        if (changed) {
                            root.syncFromLayout();
                        }
                    }
                }
            }
        }
    }

    // --- IMMEDIATE VISUAL PREVIEW: MOVE GHOST ---
    Rectangle {
        id: movePreviewGhost
        visible: false
        parent: root.parent // Mount in parent coordinate space
        z: 999

        x: Math.round(layoutController.gridToPixelX(dragArea.currentProposedGridX, root.usableWidth, root.insetLeft))
        y: Math.round(layoutController.gridToPixelY(dragArea.currentProposedGridY, root.usableHeight, root.insetTop))
        width: root.width
        height: root.height
        radius: 8

        color: dragArea.canDrop ? Qt.rgba(0.65, 0.89, 0.63, 0.25) : Qt.rgba(0.95, 0.54, 0.66, 0.35)
        border.color: dragArea.canDrop ? "#a6e3a1" : "#f38ba8"
        border.width: 2

        Rectangle {
            anchors.centerIn: parent
            implicitWidth: ghostLabel.implicitWidth + 16
            implicitHeight: 26
            radius: 13
            color: dragArea.canDrop ? "#a6e3a1" : "#f38ba8"

            Text {
                id: ghostLabel
                anchors.centerIn: parent
                text: dragArea.canDrop ? ("Move to (" + dragArea.currentProposedGridX + ", " + dragArea.currentProposedGridY + ")") : "Collision / Blocked"
                font.pixelSize: 11
                font.weight: Font.Bold
                color: "#11111b"
            }
        }
    }

    // --- IMMEDIATE VISUAL PREVIEW: RESIZE GHOST ---
    Rectangle {
        id: resizePreviewGhost
        visible: false
        parent: root.parent
        z: 999

        x: root.x
        y: root.y
        width: Math.round(layoutController.gridToPixelWidth(resizeArea.currentProposedGridW, root.usableWidth))
        height: Math.round(layoutController.gridToPixelHeight(resizeArea.currentProposedGridH, root.usableHeight))
        radius: 8

        color: resizeArea.canResizeDrop ? Qt.rgba(0.65, 0.89, 0.63, 0.25) : Qt.rgba(0.95, 0.54, 0.66, 0.35)
        border.color: resizeArea.canResizeDrop ? "#a6e3a1" : "#f38ba8"
        border.width: 2

        Rectangle {
            anchors.centerIn: parent
            implicitWidth: resizeLabel.implicitWidth + 16
            implicitHeight: 26
            radius: 13
            color: resizeArea.canResizeDrop ? "#a6e3a1" : "#f38ba8"

            Text {
                id: resizeLabel
                anchors.centerIn: parent
                text: resizeArea.canResizeDrop ? ("Resize [" + resizeArea.currentProposedGridW + " × " + resizeArea.currentProposedGridH + "]") : "Collision / Blocked"
                font.pixelSize: 11
                font.weight: Font.Bold
                color: "#11111b"
            }
        }
    }
}
