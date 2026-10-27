import QtQuick
import QtQuick.Controls

Item {
    id: visualizerRoot

    readonly property bool isEnabled: settings.visualizerEnabled
    readonly property int theme: settings.visualizerTheme
    readonly property real userOpacity: settings.visualizerOpacity
    readonly property int vizPosition: settings.visualizerPosition
    readonly property int vizHeight: settings.visualizerHeight
    readonly property int barDensity: settings.visualizerBarDensity
    readonly property real sensitivity: settings.visualizerSensitivity
    readonly property real speedFactor: settings.visualizerSpeed

    width: Math.min(parent ? (parent.width - 96) : 680, 840)
    height: vizHeight

    anchors.horizontalCenter: parent.horizontalCenter

    // Configurable vertical positioning
    y: {
        if (!parent) return 70;
        if (vizPosition === 2) {
            // Top
            return 70;
        } else if (vizPosition === 1) {
            // Center
            return (parent.height - height) / 2;
        } else {
            // Bottom (Comfortably floating above bottom playback overlay)
            return parent.height - height - 110;
        }
    }

    Behavior on y {
        NumberAnimation { duration: 250; easing.type: Easing.OutCubic }
    }

    visible: isEnabled && opacity > 0.01
    opacity: isEnabled ? (audioPlayer.isPlaying ? userOpacity : (hasResidualEnergy ? userOpacity * 0.5 : 0.0)) : 0.0

    Behavior on opacity {
        NumberAnimation { duration: 350; easing.type: Easing.InOutQuad }
    }

    // Smoothed state arrays for high-density audio visualization
    property var smoothedLevels: []
    property var peakLevels: []
    property var peakHoldFrames: []
    property bool hasResidualEnergy: false

    function initArrays() {
        var count = Math.max(16, Math.min(128, visualizerRoot.barDensity));
        var s = [];
        var p = [];
        var h = [];
        for (var i = 0; i < count; ++i) {
            s.push(0.0);
            p.push(0.0);
            h.push(0);
        }
        smoothedLevels = s;
        peakLevels = p;
        peakHoldFrames = h;
    }

    onBarDensityChanged: initArrays()
    Component.onCompleted: initArrays()

    // 50 FPS stable animation timer that only runs when visualizer is visible and active
    Timer {
        id: animTimer
        interval: 20
        running: visualizerRoot.isEnabled && visualizerRoot.visible && (audioPlayer.isPlaying || visualizerRoot.hasResidualEnergy)
        repeat: true
        onTriggered: {
            visualizerRoot.updatePhysics();
            canvasViz.requestPaint();
        }
    }

    function updatePhysics() {
        var count = Math.max(16, Math.min(128, visualizerRoot.barDensity));
        if (smoothedLevels.length !== count) {
            initArrays();
        }

        var spec = audioPlayer.spectrum;
        var specLen = (spec && spec.length) ? spec.length : 0;
        var isPlaying = audioPlayer.isPlaying;

        var s = smoothedLevels.slice();
        var p = peakLevels.slice();
        var h = peakHoldFrames.slice();

        var speed = Math.max(0.40, Math.min(2.0, visualizerRoot.speedFactor));
        var sens = Math.max(0.50, Math.min(2.0, visualizerRoot.sensitivity));

        // Attack is calm, release is over 3.5x slower for peaceful relaxation
        var attackRate = Math.min(0.60, 0.22 * speed);
        var releaseRate = Math.min(0.25, 0.06 * speed);

        // 1. Resample target spectrum across the dense bars
        var rawTargets = [];
        for (var i = 0; i < count; ++i) {
            if (!isPlaying || specLen === 0) {
                rawTargets.push(0.0);
                continue;
            }
            var normX = count > 1 ? (i / (count - 1)) : 0;
            var specIdx = normX * (specLen - 1);
            var idx0 = Math.floor(specIdx);
            var idx1 = Math.min(specLen - 1, idx0 + 1);
            var frac = specIdx - idx0;
            var val = (spec[idx0] || 0.0) * (1.0 - frac) + (spec[idx1] || 0.0) * frac;

            // Apply sensitivity with soft-knee compression to avoid harsh clipping
            var scaled = val * sens;
            var compressed = Math.pow(Math.min(scaled, 1.5), 0.85);
            rawTargets.push(Math.min(1.0, Math.max(0.0, compressed)));
        }

        // 2. Spatial smoothing across neighbors for organic cohesion
        var spatialTargets = [];
        for (var j = 0; j < count; ++j) {
            var prev = (j > 0) ? rawTargets[j - 1] : rawTargets[j];
            var curr = rawTargets[j];
            var next = (j < count - 1) ? rawTargets[j + 1] : rawTargets[j];
            spatialTargets.push(0.20 * prev + 0.60 * curr + 0.20 * next);
        }

        // 3. Temporal smoothing and peak hold
        var totalEnergy = 0.0;
        for (var k = 0; k < count; ++k) {
            var target = spatialTargets[k];
            if (target > s[k]) {
                s[k] += (target - s[k]) * attackRate;
            } else {
                s[k] += (target - s[k]) * releaseRate;
            }
            if (s[k] < 0.003) s[k] = 0.0;

            // Peak tracking with gravity
            if (s[k] >= p[k]) {
                p[k] = s[k];
                h[k] = Math.max(4, Math.round(10 / speed)); // Hold peak briefly
            } else if (h[k] > 0) {
                h[k]--;
            } else {
                p[k] = Math.max(0.0, p[k] - 0.015 * speed);
            }

            totalEnergy += s[k];
        }

        smoothedLevels = s;
        peakLevels = p;
        peakHoldFrames = h;
        hasResidualEnergy = (totalEnergy > 0.01);
    }

    // Single unified hardware-accelerated Canvas for all visualizer themes
    Canvas {
        id: canvasViz
        anchors.fill: parent

        readonly property real dpr: Screen.devicePixelRatio || 1.0
        canvasSize: Qt.size(Math.max(1, Math.ceil(width * dpr)), Math.max(1, Math.ceil(height * dpr)))

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.scale(dpr, dpr);
            ctx.clearRect(0, 0, width, height);

            var levels = visualizerRoot.smoothedLevels;
            var peaks = visualizerRoot.peakLevels;
            var count = levels.length;
            if (count === 0) return;

            var w = width;
            var h = height;
            var curTheme = visualizerRoot.theme;

            if (curTheme === 0) {
                // =============================================================
                // THEME 0: Classic Equalizer Bars (Segmented LED blocks + peaks)
                // =============================================================
                var gap = Math.max(1, Math.min(3, Math.floor(w / (count * 4))));
                var barW = (w - (count - 1) * gap) / count;
                if (barW < 1) barW = 1;

                var blockGap = 2;
                var blockCount = Math.max(8, Math.min(22, Math.floor(h / 7)));
                var blockH = Math.max(2, (h - (blockCount - 1) * blockGap) / blockCount);

                for (var b = 0; b < count; ++b) {
                    var barX = b * (barW + gap);
                    var lvl = levels[b];
                    var pk = peaks[b];

                    // Render LED segmented blocks
                    for (var blk = 0; blk < blockCount; ++blk) {
                        var blockY = h - (blk + 1) * (blockH + blockGap);
                        var threshold = (blk + 1) / blockCount;
                        var isLit = (lvl >= threshold);

                        if (isLit) {
                            var normHeight = blk / blockCount;
                            if (normHeight < 0.35) {
                                ctx.fillStyle = "#a6e3a1"; // Mint green (bass)
                            } else if (normHeight < 0.65) {
                                ctx.fillStyle = "#89b4fa"; // Sapphire blue (mids)
                            } else if (normHeight < 0.85) {
                                ctx.fillStyle = "#cba6f7"; // Mauve (mid-high)
                            } else {
                                ctx.fillStyle = "#f5c2e7"; // Pink (peak)
                            }
                            ctx.globalAlpha = 0.92;
                        } else {
                            ctx.fillStyle = "rgba(255, 255, 255, 0.05)";
                            ctx.globalAlpha = 0.40;
                        }

                        ctx.fillRect(barX, blockY, barW, blockH);
                    }

                    // Floating peak indicator bar
                    if (pk > 0.05) {
                        var peakY = Math.max(0, Math.min(h - 3, h * (1.0 - pk)));
                        ctx.fillStyle = "#f5e0dc"; // Pastel peach
                        ctx.globalAlpha = 0.95;
                        ctx.fillRect(barX, peakY, barW, 2);
                    }
                }
                ctx.globalAlpha = 1.0;

            } else if (curTheme === 1) {
                // =============================================================
                // THEME 1: Soft Rounded Bars (Modern sleek pastel capsules)
                // =============================================================
                var gap1 = Math.max(1, Math.min(4, Math.floor(w / (count * 3.5))));
                var barW1 = (w - (count - 1) * gap1) / count;
                if (barW1 < 1) barW1 = 1;
                var rad = Math.min(barW1 / 2, 4);

                for (var i = 0; i < count; ++i) {
                    var barX1 = i * (barW1 + gap1);
                    var barH1 = Math.max(4, h * levels[i]);
                    var barY1 = h - barH1;

                    var grad = ctx.createLinearGradient(0, barY1, 0, h);
                    grad.addColorStop(0.0, "#b4befe");
                    grad.addColorStop(0.5, "#89b4fa");
                    grad.addColorStop(1.0, "rgba(137, 180, 250, 0.25)");

                    ctx.fillStyle = grad;
                    ctx.beginPath();
                    if (ctx.roundRect) {
                        ctx.roundRect(barX1, barY1, barW1, barH1, [rad, rad, 0, 0]);
                    } else {
                        ctx.rect(barX1, barY1, barW1, barH1);
                    }
                    ctx.fill();
                }

            } else if (curTheme === 2) {
                // =============================================================
                // THEME 2: Minimal Waveform (Clean oscillating harmonic spline)
                // =============================================================
                ctx.save();
                ctx.beginPath();
                var midY = h / 2;
                var step = w / (count - 1);

                for (var m = 0; m < count; ++m) {
                    var x = m * step;
                    var mag = levels[m] * (h * 0.45);
                    var y = (m % 2 === 0) ? (midY - mag) : (midY + mag);
                    if (m === 0) {
                        ctx.moveTo(x, y);
                    } else {
                        var prevX = (m - 1) * step;
                        var cX = (prevX + x) / 2;
                        ctx.quadraticCurveTo(prevX, y, cX, y);
                    }
                }

                ctx.strokeStyle = "#89b4fa";
                ctx.lineWidth = 2.5;
                ctx.lineCap = "round";
                ctx.lineJoin = "round";
                ctx.shadowColor = "rgba(137, 180, 250, 0.4)";
                ctx.shadowBlur = 6;
                ctx.stroke();
                ctx.restore();

            } else if (curTheme === 3) {
                // =============================================================
                // THEME 3: Subtle Ambient Waves (Dual harmonic ribbons)
                // =============================================================
                ctx.save();
                var energy = audioPlayer.currentEnergy || 0.0;
                var baseAmp = (h * 0.35) * (0.25 + energy * 0.75);

                // Wave 1: Soft Lavender ribbon
                ctx.beginPath();
                ctx.moveTo(0, h * 0.5);
                for (var x1 = 0; x1 <= w; x1 += 6) {
                    var p1 = x1 / w;
                    var idxW1 = Math.min(count - 1, Math.floor(p1 * count));
                    var localMag1 = levels[idxW1];
                    var y1 = (h * 0.5) + Math.sin(p1 * Math.PI * 4 + energy * 2.5) * (baseAmp * (0.35 + localMag1 * 0.65));
                    ctx.lineTo(x1, y1);
                }
                ctx.strokeStyle = "rgba(203, 166, 247, 0.65)";
                ctx.lineWidth = 2.8;
                ctx.stroke();

                // Wave 2: Soft Blue ribbon
                ctx.beginPath();
                ctx.moveTo(0, h * 0.55);
                for (var x2 = 0; x2 <= w; x2 += 6) {
                    var p2 = x2 / w;
                    var idxW2 = Math.min(count - 1, Math.floor((1.0 - p2) * count));
                    var localMag2 = levels[idxW2];
                    var y2 = (h * 0.55) + Math.cos(p2 * Math.PI * 3.5 - energy * 2.0) * (baseAmp * 0.85 * (0.35 + localMag2 * 0.65));
                    ctx.lineTo(x2, y2);
                }
                ctx.strokeStyle = "rgba(137, 180, 250, 0.55)";
                ctx.lineWidth = 2.0;
                ctx.stroke();

                ctx.restore();
            }
        }
    }
}
