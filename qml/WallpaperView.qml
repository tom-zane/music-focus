import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import QtMultimedia
import org.kde.kirigami as Kirigami

Item {
    id: root

    readonly property int mode: settings.wallpaperMode
    readonly property url wallpaperUrl: wallpaperManager.currentWallpaperUrl
    readonly property url videoUrl: wallpaperManager.currentVideoUrl
    readonly property color solidColor: settings.solidColor
    readonly property real dimmingOpacity: settings.dimmingOpacity
    readonly property int seed: settings.gradientSeed
    readonly property int style: settings.gradientStyle

    // --- 1. DETERMINISTIC GRADIENT GENERATOR LAYER (Mode 0: GradientMode) ---
    Item {
        id: gradientLayer
        anchors.fill: parent
        visible: root.mode === 0 || (!wallpaperManager.hasWallpaper && !wallpaperManager.hasVideo) || (root.mode === 2 && videoLayer.videoHasError)

        // Generate deterministic pastel palette from seed
        readonly property var pastelPalette: {
            var palettes = [
                // 0: Deep Lavender & Misty Indigo
                ["#181926", "#25273e", "#353754", "#494d6e"],
                // 1: Muted Forest & Sage Mint
                ["#141c19", "#1d2d27", "#2b4038", "#3c574c"],
                // 2: Nordic Twilight & Sapphire
                ["#16192b", "#1f2742", "#2e3b5e", "#41537d"],
                // 3: Dusky Mauve & Soft Plum
                ["#1c1723", "#2a2135", "#3c2e4c", "#523f66"],
                // 4: Dark Sakura & Soft Rose
                ["#20171e", "#31202e", "#442b3f", "#5c3a55"],
                // 5: Warm Espresso & Dusk Amber
                ["#1e1917", "#2c2320", "#3d302c", "#52403b"]
            ];
            var pIdx = Math.abs(root.seed) % palettes.length;
            return palettes[pIdx];
        }

        // Base rectangle with primary color
        Rectangle {
            anchors.fill: parent
            color: gradientLayer.pastelPalette[0]
        }

        // Style 0: Linear Pastel (Horizontal / Vertical smooth blend)
        Rectangle {
            anchors.fill: parent
            visible: root.style === 0
            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: gradientLayer.pastelPalette[0] }
                GradientStop { position: 0.4; color: gradientLayer.pastelPalette[1] }
                GradientStop { position: 0.8; color: gradientLayer.pastelPalette[2] }
                GradientStop { position: 1.0; color: gradientLayer.pastelPalette[3] }
            }
        }

        // Style 1: Radial Pastel (Centered ambient glow)
        Rectangle {
            anchors.fill: parent
            visible: root.style === 1
            gradient: Gradient {
                orientation: Gradient.Vertical
                GradientStop { position: 0.0; color: gradientLayer.pastelPalette[1] }
                GradientStop { position: 0.5; color: gradientLayer.pastelPalette[2] }
                GradientStop { position: 1.0; color: gradientLayer.pastelPalette[0] }
            }

            Rectangle {
                anchors.centerIn: parent
                width: Math.max(parent.width, parent.height) * 0.9
                height: width
                radius: width / 2
                opacity: 0.65
                gradient: Gradient {
                    GradientStop { position: 0.0; color: gradientLayer.pastelPalette[3] }
                    GradientStop { position: 0.5; color: gradientLayer.pastelPalette[2] }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }
        }

        // Style 2: Angled Pastel (Diagonal dynamic blend)
        Rectangle {
            anchors.fill: parent
            visible: root.style === 2
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: gradientLayer.pastelPalette[0] }
                GradientStop { position: 0.45; color: gradientLayer.pastelPalette[2] }
                GradientStop { position: 1.0; color: gradientLayer.pastelPalette[3] }
            }

            Rectangle {
                anchors.fill: parent
                opacity: 0.55
                gradient: Gradient {
                    orientation: Gradient.Vertical
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 1.0; color: gradientLayer.pastelPalette[1] }
                }
            }
        }

        // Style 3: Soft Mesh (Dual-accent ambient wash)
        Item {
            anchors.fill: parent
            visible: root.style === 3

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    orientation: Gradient.Vertical
                    GradientStop { position: 0.0; color: gradientLayer.pastelPalette[0] }
                    GradientStop { position: 1.0; color: gradientLayer.pastelPalette[1] }
                }
            }

            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width * 0.75
                height: parent.height * 0.75
                radius: Math.max(width, height) / 2
                opacity: 0.5
                gradient: Gradient {
                    GradientStop { position: 0.0; color: gradientLayer.pastelPalette[2] }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: parent.width * 0.8
                height: parent.height * 0.8
                radius: Math.max(width, height) / 2
                opacity: 0.45
                gradient: Gradient {
                    GradientStop { position: 0.0; color: gradientLayer.pastelPalette[3] }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }
        }
    }

    // --- 2. VIDEO WALLPAPER LAYER (Mode 2: VideoMode) ---
    Item {
        id: videoLayer
        anchors.fill: parent
        visible: root.mode === 2 && wallpaperManager.hasVideo && !videoHasError

        property bool videoHasError: false
        property string lastErrorMsg: ""

        MediaPlayer {
            id: videoPlayer
            videoOutput: videoOutput
            source: (root.mode === 2 && wallpaperManager.hasVideo) ? root.videoUrl : ""
            loops: MediaPlayer.Infinite

            onMediaStatusChanged: {
                if (mediaStatus === MediaPlayer.EndOfMedia && root.mode === 2) {
                    videoPlayer.setPosition(0);
                    videoPlayer.play();
                } else if (mediaStatus === MediaPlayer.LoadedMedia && root.mode === 2) {
                    videoPlayer.play();
                } else if (mediaStatus === MediaPlayer.InvalidMedia) {
                    videoLayer.videoHasError = true;
                    videoLayer.lastErrorMsg = "Invalid video media or unsupported codec";
                    console.warn("Video wallpaper invalid media:", root.videoUrl);
                }
            }

            onErrorOccurred: (error, errorString) => {
                videoLayer.videoHasError = true;
                videoLayer.lastErrorMsg = errorString;
                console.warn("Video wallpaper playback error:", error, errorString);
            }
        }

        VideoOutput {
            id: videoOutput
            anchors.fill: parent
            fillMode: VideoOutput.PreserveAspectCrop
        }

        Connections {
            target: root
            function onModeChanged() {
                if (root.mode === 2 && wallpaperManager.hasVideo) {
                    videoLayer.videoHasError = false;
                    videoPlayer.source = root.videoUrl;
                    videoPlayer.play();
                } else {
                    videoPlayer.stop();
                    videoPlayer.source = "";
                }
            }
            function onVideoUrlChanged() {
                videoLayer.videoHasError = false;
                videoPlayer.stop();
                if (root.mode === 2 && wallpaperManager.hasVideo) {
                    videoPlayer.source = root.videoUrl;
                    videoPlayer.play();
                } else {
                    videoPlayer.source = "";
                }
            }
        }

        Component.onCompleted: {
            if (root.mode === 2 && wallpaperManager.hasVideo && root.videoUrl.toString() !== "") {
                videoPlayer.play();
            }
        }
    }

    // Fallback notice overlay when video playback fails
    Rectangle {
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        visible: root.mode === 2 && videoLayer.videoHasError
        z: 5
        implicitWidth: fallbackNoticeRow.implicitWidth + 24
        implicitHeight: 32
        radius: 16
        color: Qt.rgba(0.12, 0.13, 0.18, 0.94)
        border.color: "#f38ba8"
        border.width: 1

        RowLayout {
            id: fallbackNoticeRow
            anchors.centerIn: parent
            spacing: 8

            Kirigami.Icon {
                source: "qrc:/icons/lucide/triangle-alert.svg"
                implicitWidth: 16
                implicitHeight: 16
                color: "#f38ba8"
            }

            Text {
                text: "Video format unsupported (" + (videoLayer.lastErrorMsg.length > 0 ? videoLayer.lastErrorMsg : "decoder error") + "). Displaying gradient fallback."
                font.pixelSize: 11
                color: "#ffffff"
            }
        }
    }

    // --- 3. STILL IMAGE WALLPAPER LAYER (Mode 1: StillImagesMode) ---
    Item {
        id: stillLayer
        anchors.fill: parent
        visible: root.mode === 1

        property bool activeIsA: false
        property bool fadeEnabled: false

        function applyWallpaper(crossfade) {
            if (root.mode !== 1 || !root.wallpaperUrl || root.wallpaperUrl.toString() === "") {
                imgA.opacity = 0.0;
                imgB.opacity = 0.0;
                return;
            }

            var currentImg = stillLayer.activeIsA ? imgA : imgB;
            var nextImg = stillLayer.activeIsA ? imgB : imgA;

            if (currentImg.source.toString() === root.wallpaperUrl.toString() && currentImg.opacity > 0.01) {
                return;
            }

            stillLayer.fadeEnabled = crossfade;
            nextImg.source = root.wallpaperUrl;
            nextImg.opacity = 1.0;
            currentImg.opacity = 0.0;
            stillLayer.activeIsA = !stillLayer.activeIsA;
            stillLayer.fadeEnabled = true;
        }

        Connections {
            target: root
            function onWallpaperUrlChanged() {
                stillLayer.applyWallpaper(true);
            }
            function onModeChanged() {
                if (root.mode === 1) {
                    stillLayer.applyWallpaper(false);
                }
            }
        }

        Connections {
            target: wallpaperManager
            function onCurrentWallpaperUrlChanged() {
                stillLayer.applyWallpaper(true);
            }
        }

        Component.onCompleted: {
            stillLayer.applyWallpaper(false);
        }

        Image {
            id: imgA
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            asynchronous: true
            cache: true
            mipmap: true
            smooth: true
            opacity: 0.0
            visible: opacity > 0.001

            Behavior on opacity {
                enabled: stillLayer.fadeEnabled
                NumberAnimation { duration: 600; easing.type: Easing.InOutQuad }
            }
        }

        Image {
            id: imgB
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            asynchronous: true
            cache: true
            mipmap: true
            smooth: true
            opacity: 0.0
            visible: opacity > 0.001

            Behavior on opacity {
                enabled: stillLayer.fadeEnabled
                NumberAnimation { duration: 600; easing.type: Easing.InOutQuad }
            }
        }
    }

    // --- 4. CONFIGURABLE DIMMING LAYER ---
    Rectangle {
        id: dimmingLayer
        anchors.fill: parent
        color: "#000000"
        opacity: root.dimmingOpacity

        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }
    }
}
