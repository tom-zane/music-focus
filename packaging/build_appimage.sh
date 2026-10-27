#!/usr/bin/env bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${PROJECT_DIR}/build-release"
APPDIR="${PROJECT_DIR}/packaging/AppDir"
TOOLS_DIR="${PROJECT_DIR}/packaging/tools"
OUTPUT_APPIMAGE="${PROJECT_DIR}/MusicFocus-x86_64.AppImage"

echo "=== 1. Cleaning All Old Build Code, Cache & Artifacts ==="
rm -rf "${BUILD_DIR}" "${APPDIR}" "${OUTPUT_APPIMAGE}"

echo "=== 2. Building Release Binary ==="
cmake -B "${BUILD_DIR}" -S "${PROJECT_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" -j"$(nproc)"

echo "=== 3. Preparing AppDir ==="
mkdir -p "${APPDIR}/usr/bin" \
         "${APPDIR}/usr/lib" \
         "${APPDIR}/usr/plugins" \
         "${APPDIR}/usr/qml/org/kde" \
         "${APPDIR}/usr/share/applications" \
         "${APPDIR}/usr/share/icons/hicolor/scalable/apps"

for size in 16 24 32 48 64 128 256 512; do
    mkdir -p "${APPDIR}/usr/share/icons/hicolor/${size}x${size}/apps"
done

# Copy binary
cp -a "${BUILD_DIR}/music-focus" "${APPDIR}/usr/bin/music-focus"

# Copy desktop and all icon resolutions
cp -a "${PROJECT_DIR}/resources/org.kde.musicfocus.desktop" "${APPDIR}/usr/share/applications/org.kde.musicfocus.desktop"
cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${APPDIR}/usr/share/icons/hicolor/scalable/apps/org.kde.musicfocus.svg"
cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${APPDIR}/usr/share/icons/hicolor/scalable/apps/music-focus.svg"

for size in 16 24 32 48 64 128 256 512; do
    cp -a "${PROJECT_DIR}/resources/icons/hicolor/${size}x${size}/apps/music-focus.png" "${APPDIR}/usr/share/icons/hicolor/${size}x${size}/apps/org.kde.musicfocus.png"
    cp -a "${PROJECT_DIR}/resources/icons/hicolor/${size}x${size}/apps/music-focus.png" "${APPDIR}/usr/share/icons/hicolor/${size}x${size}/apps/music-focus.png"
done

ln -sf usr/share/applications/org.kde.musicfocus.desktop "${APPDIR}/org.kde.musicfocus.desktop"
ln -sf usr/share/icons/hicolor/256x256/apps/org.kde.musicfocus.png "${APPDIR}/org.kde.musicfocus.png"
ln -sf usr/share/icons/hicolor/256x256/apps/org.kde.musicfocus.png "${APPDIR}/music-focus.png"
ln -sf org.kde.musicfocus.png "${APPDIR}/.DirIcon"

echo "=== 4. Bundling Qt6 & KF6 Libraries ==="
cp -a /lib64/libQt6Core.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Gui.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Widgets.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Quick*.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Qml.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6QmlModels.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6QmlMeta.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6QmlWorkerScript.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Network.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6OpenGL.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Multimedia.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6MultimediaQuick.so.6* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libQt6SpatialAudio.so.6* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libavcodec.so* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libavformat.so* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libavutil.so* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libswresample.so* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libswscale.so* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libQt6DBus.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libQt6Wayland* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libQt6WlShellIntegration* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libQt6XcbQpa* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libQt6Svg.so.6* "${APPDIR}/usr/lib/" 2>/dev/null || true
cp -a /lib64/libKirigami*.so.6* "${APPDIR}/usr/lib/"
cp -a /lib64/libKF6*.so.6* "${APPDIR}/usr/lib/" 2>/dev/null || true

echo "=== 5. Bundling Qt Plugins & QML Modules ==="
cp -r /usr/lib64/qt6/plugins/platforms "${APPDIR}/usr/plugins/"
mkdir -p "${APPDIR}/usr/plugins/platformthemes"
cp -a /usr/lib64/qt6/plugins/platformthemes/libqxdgdesktopportal.so "${APPDIR}/usr/plugins/platformthemes/"
cp -a /usr/lib64/qt6/plugins/platformthemes/libqgtk3.so "${APPDIR}/usr/plugins/platformthemes/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/imageformats "${APPDIR}/usr/plugins/"
cp -r /usr/lib64/qt6/plugins/iconengines "${APPDIR}/usr/plugins/"
cp -r /usr/lib64/qt6/plugins/kf6 "${APPDIR}/usr/plugins/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/multimedia "${APPDIR}/usr/plugins/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/wayland-shell-integration "${APPDIR}/usr/plugins/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/wayland-graphics-integration-client "${APPDIR}/usr/plugins/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/wayland-decoration-client "${APPDIR}/usr/plugins/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/xcbglintegrations "${APPDIR}/usr/plugins/" 2>/dev/null || true
cp -r /usr/lib64/qt6/plugins/tls "${APPDIR}/usr/plugins/" 2>/dev/null || true

cp -r /usr/lib64/qt6/qml/QtQuick "${APPDIR}/usr/qml/"
cp -r /usr/lib64/qt6/qml/QtMultimedia "${APPDIR}/usr/qml/"
cp -r /usr/lib64/qt6/qml/org/kde/* "${APPDIR}/usr/qml/org/kde/" 2>/dev/null || true

# Create qt.conf
cat > "${APPDIR}/usr/bin/qt.conf" << 'EOF'
[Paths]
Prefix = ..
Plugins = plugins
Qml2Imports = qml
QmlImports = qml
EOF

# Create AppRun entrypoint
cat > "${APPDIR}/AppRun" << 'EOF'
#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")")"
export PATH="${HERE}/usr/bin:/usr/libexec/kf6:${PATH}"
export LD_LIBRARY_PATH="${HERE}/usr/lib:${LD_LIBRARY_PATH}"
export QT_QPA_PLATFORMTHEME="xdgdesktopportal"
export QT_PLUGIN_PATH="${HERE}/usr/plugins:/usr/lib64/qt6/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}"
export QML_IMPORT_PATH="${HERE}/usr/qml"
export QML2_IMPORT_PATH="${HERE}/usr/qml"
export XDG_DATA_DIRS="${HERE}/usr/share:/usr/local/share:/usr/share${XDG_DATA_DIRS:+:$XDG_DATA_DIRS}"
exec "${HERE}/usr/bin/music-focus" "$@"
EOF
chmod +x "${APPDIR}/AppRun"

# Update user local icons cache so desktop compositor finds the new icon
for size in 16 24 32 48 64 128 256 512; do
    mkdir -p "${HOME}/.local/share/icons/hicolor/${size}x${size}/apps"
    cp -a "${PROJECT_DIR}/resources/icons/hicolor/${size}x${size}/apps/music-focus.png" "${HOME}/.local/share/icons/hicolor/${size}x${size}/apps/org.kde.musicfocus.png"
    cp -a "${PROJECT_DIR}/resources/icons/hicolor/${size}x${size}/apps/music-focus.png" "${HOME}/.local/share/icons/hicolor/${size}x${size}/apps/music-focus.png"
done
mkdir -p "${HOME}/.local/share/icons/hicolor/scalable/apps"
cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${HOME}/.local/share/icons/hicolor/scalable/apps/org.kde.musicfocus.svg"
cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${HOME}/.local/share/icons/hicolor/scalable/apps/music-focus.svg"
gtk-update-icon-cache "${HOME}/.local/share/icons/hicolor" 2>/dev/null || true

if [ -d "${HOME}/.local/share/icons/kora/apps/scalable" ]; then
    cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${HOME}/.local/share/icons/kora/apps/scalable/org.kde.musicfocus.svg"
    cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${HOME}/.local/share/icons/kora/apps/scalable/music-focus.svg"
    cp -a "${PROJECT_DIR}/resources/icons/music-focus.svg" "${HOME}/.local/share/icons/kora/apps/scalable/music_focus.svg"
    gtk-update-icon-cache "${HOME}/.local/share/icons/kora" 2>/dev/null || true
fi
kbuildsycoca6 --noincremental 2>/dev/null || true

echo "=== 6. Packaging into AppImage ==="
mkdir -p "${TOOLS_DIR}"
if [ ! -f "${TOOLS_DIR}/appimagetool" ]; then
    curl -sL -o "${TOOLS_DIR}/appimagetool" https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
    chmod +x "${TOOLS_DIR}/appimagetool"
fi

ARCH=x86_64 "${TOOLS_DIR}/appimagetool" "${APPDIR}" "${OUTPUT_APPIMAGE}"
echo "AppImage created successfully at ${OUTPUT_APPIMAGE}"

if [ -d "${HOME}/AppImages" ]; then
    cp -f "${OUTPUT_APPIMAGE}" "${HOME}/AppImages/music_focus.appimage"
    chmod +x "${HOME}/AppImages/music_focus.appimage"
    echo "Copied updated AppImage to ${HOME}/AppImages/music_focus.appimage"

    if [ -f "${HOME}/AppImages/music_focus.appimage_0.appimage" ]; then
        cp -f "${OUTPUT_APPIMAGE}" "${HOME}/AppImages/music_focus.appimage_0.appimage"
        chmod +x "${HOME}/AppImages/music_focus.appimage_0.appimage"
        echo "Copied updated AppImage to ${HOME}/AppImages/music_focus.appimage_0.appimage"
    fi

    if [ -d "${HOME}/AppImages/.icons" ]; then
        cp -f "${PROJECT_DIR}/resources/icons/hicolor/256x256/apps/music-focus.png" "${HOME}/AppImages/.icons/music_focus.appimage_0" 2>/dev/null || true
    fi
fi
