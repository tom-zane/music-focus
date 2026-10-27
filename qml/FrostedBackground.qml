import QtQuick
import QtQuick.Effects

Item {
    id: frosted
    anchors.fill: parent
    z: -1

    property Item targetItem: parent
    property Item wallpaperSource: wallpaperView
    property real radius: (targetItem && targetItem.radius !== undefined) ? targetItem.radius : 10

    readonly property bool blurActive: settings.blurEnabled && (settings.blurStrength > 0.01)
    readonly property bool darkeningActive: settings.darkeningEnabled && (settings.darkeningOpacity > 0.01)

    visible: true

    // Synchronize source rectangle with exact current screen position on wallpaper
    function updateSourceRect() {
        if (!frosted.targetItem || !frosted.wallpaperSource) return;
        var p0 = frosted.targetItem.mapToItem(frosted.wallpaperSource, 0, 0);
        var rx = Math.max(0, p0.x);
        var ry = Math.max(0, p0.y);
        var rw = Math.max(1, frosted.targetItem.width);
        var rh = Math.max(1, frosted.targetItem.height);

        var curRect = bgSource.sourceRect;
        if (Math.abs(curRect.x - rx) > 0.5 || Math.abs(curRect.y - ry) > 0.5 ||
            Math.abs(curRect.width - rw) > 0.5 || Math.abs(curRect.height - rh) > 0.5) {
            bgSource.sourceRect = Qt.rect(rx, ry, rw, rh);
            bgSource.scheduleUpdate();
        }
    }

    // Dynamic ancestor tracking: hooks x, y, width, height, and visible changes up the item tree
    property var _hookedItems: []

    function cleanupHooks() {
        for (var i = 0; i < _hookedItems.length; ++i) {
            var itm = _hookedItems[i];
            if (itm) {
                try {
                    itm.xChanged.disconnect(updateSourceRect);
                    itm.yChanged.disconnect(updateSourceRect);
                    itm.widthChanged.disconnect(updateSourceRect);
                    itm.heightChanged.disconnect(updateSourceRect);
                    itm.visibleChanged.disconnect(updateSourceRect);
                } catch (e) {}
            }
        }
        _hookedItems = [];
    }

    function setupHooks() {
        cleanupHooks();
        var cur = frosted.targetItem;
        while (cur && cur !== frosted.wallpaperSource) {
            try {
                cur.xChanged.connect(updateSourceRect);
                cur.yChanged.connect(updateSourceRect);
                cur.widthChanged.connect(updateSourceRect);
                cur.heightChanged.connect(updateSourceRect);
                cur.visibleChanged.connect(updateSourceRect);
                _hookedItems.push(cur);
            } catch (e) {}
            cur = cur.parent;
        }
        updateSourceRect();
    }

    Component.onCompleted: setupHooks()
    Component.onDestruction: cleanupHooks()
    onTargetItemChanged: setupHooks()
    onWallpaperSourceChanged: setupHooks()
    onVisibleChanged: {
        if (visible) {
            updateSourceRect();
            bgSource.scheduleUpdate();
        }
    }

    Connections {
        target: frosted.wallpaperSource
        function onWidthChanged() { frosted.updateSourceRect(); }
        function onHeightChanged() { frosted.updateSourceRect(); }
    }

    Connections {
        target: wallpaperManager
        function onCurrentWallpaperUrlChanged() {
            frosted.updateSourceRect();
            bgSource.scheduleUpdate();
        }
        function onCurrentVideoUrlChanged() {
            frosted.updateSourceRect();
            bgSource.scheduleUpdate();
        }
    }

    Connections {
        target: settings
        function onWallpaperModeChanged() {
            frosted.updateSourceRect();
            bgSource.scheduleUpdate();
        }
        function onGradientSeedChanged() {
            frosted.updateSourceRect();
            bgSource.scheduleUpdate();
        }
        function onGradientStyleChanged() {
            frosted.updateSourceRect();
            bgSource.scheduleUpdate();
        }
        function onDimmingOpacityChanged() {
            bgSource.scheduleUpdate();
        }
        function onBlurStrengthChanged() {
            bgSource.scheduleUpdate();
        }
    }

    Connections {
        target: layoutController
        function onLayoutChanged() {
            frosted.updateSourceRect();
        }
        function onWidgetChanged(id) {
            frosted.updateSourceRect();
        }
    }

    // 1. Independent Blur Layer (conventional blur, only when enabled)
    Item {
        anchors.fill: parent
        visible: frosted.blurActive

        Rectangle {
            id: maskRect
            anchors.fill: parent
            radius: frosted.radius
            color: "#ffffff"
            antialiasing: true
            visible: false
            layer.enabled: true
        }

        ShaderEffectSource {
            id: bgSource
            sourceItem: frosted.wallpaperSource
            sourceRect: {
                if (!frosted.targetItem || !frosted.wallpaperSource) return Qt.rect(0, 0, 0, 0);
                var p0 = frosted.targetItem.mapToItem(frosted.wallpaperSource, 0, 0);
                return Qt.rect(Math.max(0, p0.x), Math.max(0, p0.y), Math.max(1, frosted.targetItem.width), Math.max(1, frosted.targetItem.height));
            }
            anchors.fill: parent
            live: frosted.blurActive
            hideSource: false
            visible: false
        }

        MultiEffect {
            anchors.fill: parent
            source: bgSource
            blurEnabled: true
            blur: Math.min(1.0, settings.blurStrength * 0.75)
            blurMax: 32
            autoPaddingEnabled: false
            maskEnabled: true
            maskSource: maskRect
        }
    }

    // 2. Reliable backing surface guaranteeing WCAG legibility
    Rectangle {
        anchors.fill: parent
        radius: frosted.radius
        color: contrastManager.isDark ? "#08090e" : "#f5f6f8"
        opacity: frosted.darkeningActive ? Math.max(settings.darkeningOpacity, contrastManager.isDark ? 0.35 : 0.45) : (contrastManager.isDark ? 0.35 : 0.45)

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
        Behavior on color {
            ColorAnimation { duration: 150 }
        }
    }
}
