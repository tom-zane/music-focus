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
# Locate Qt and system library directories dynamically
QT_LIBS=""
QT_PLUGINS=""
QT_QML=""
if command -v qtpaths6 &>/dev/null; then
    QT_LIBS="$(qtpaths6 --query QT_INSTALL_LIBS 2>/dev/null || true)"
    QT_PLUGINS="$(qtpaths6 --query QT_INSTALL_PLUGINS 2>/dev/null || true)"
    QT_QML="$(qtpaths6 --query QT_INSTALL_QML 2>/dev/null || true)"
fi

SEARCH_LIB_DIRS=("/usr/lib/x86_64-linux-gnu" "/lib64" "/usr/lib64" "/usr/lib")
if [ -z "${QT_LIBS}" ] || [ ! -d "${QT_LIBS}" ]; then
    for d in "${SEARCH_LIB_DIRS[@]}"; do
        if [ -f "${d}/libQt6Core.so.6" ] || compgen -G "${d}/libQt6Core.so.6*" > /dev/null; then
            QT_LIBS="${d}"
            break
        fi
    done
fi

if [ -z "${QT_PLUGINS}" ] || [ ! -d "${QT_PLUGINS}" ]; then
    for d in "/usr/lib/x86_64-linux-gnu/qt6/plugins" "/usr/lib64/qt6/plugins" "/usr/lib/qt6/plugins"; do
        if [ -d "${d}" ]; then
            QT_PLUGINS="${d}"
            break
        fi
    done
fi

if [ -z "${QT_QML}" ] || [ ! -d "${QT_QML}" ]; then
    for d in "/usr/lib/x86_64-linux-gnu/qt6/qml" "/usr/lib64/qt6/qml" "/usr/lib/qt6/qml"; do
        if [ -d "${d}" ]; then
            QT_QML="${d}"
            break
        fi
    done
fi

echo "Resolved Qt paths: LIBS=${QT_LIBS}, PLUGINS=${QT_PLUGINS}, QML=${QT_QML}"

copy_lib() {
    local pattern="$1"
    local found=0
    for dir in "${QT_LIBS}" "${SEARCH_LIB_DIRS[@]}"; do
        if [ -d "${dir}" ]; then
            for f in "${dir}"/${pattern}; do
                if [ -e "${f}" ]; then
                    cp -a "${f}" "${APPDIR}/usr/lib/" 2>/dev/null || true
                    found=1
                fi
            done
        fi
        [ ${found} -eq 1 ] && break
    done
}

copy_lib "libQt6Core.so.6*"
copy_lib "libQt6Gui.so.6*"
copy_lib "libQt6Widgets.so.6*"
copy_lib "libQt6Quick*.so.6*"
copy_lib "libQt6Qml*.so.6*"
copy_lib "libQt6Network.so.6*"
copy_lib "libQt6OpenGL.so.6*"
copy_lib "libQt6Multimedia*.so.6*"
copy_lib "libQt6SpatialAudio.so.6*"
copy_lib "libQt6DBus.so.6*"
copy_lib "libQt6Wayland*.so.6*"
copy_lib "libQt6WlShellIntegration*.so.6*"
copy_lib "libQt6XcbQpa*.so.6*"
copy_lib "libQt6Svg.so.6*"
copy_lib "libKirigami*.so.6*"
copy_lib "libKF6*.so.6*"
copy_lib "libavcodec.so*"
copy_lib "libavformat.so*"
copy_lib "libavutil.so*"
copy_lib "libswresample.so*"
copy_lib "libswscale.so*"

echo "=== 5. Bundling Qt Plugins & QML Modules ==="
if [ -n "${QT_PLUGINS}" ] && [ -d "${QT_PLUGINS}" ]; then
    if [ -d "${QT_PLUGINS}/platforms" ]; then
        cp -r "${QT_PLUGINS}/platforms" "${APPDIR}/usr/plugins/"
    fi
    mkdir -p "${APPDIR}/usr/plugins/platformthemes"
    for f in "${QT_PLUGINS}/platformthemes"/libqxdgdesktopportal.so "${QT_PLUGINS}/platformthemes"/libqgtk3.so; do
        if [ -f "${f}" ]; then
            cp -a "${f}" "${APPDIR}/usr/plugins/platformthemes/"
        fi
    done
    for plugin_group in imageformats iconengines kf6 multimedia wayland-shell-integration wayland-graphics-integration-client wayland-decoration-client xcbglintegrations tls; do
        if [ -d "${QT_PLUGINS}/${plugin_group}" ]; then
            cp -r "${QT_PLUGINS}/${plugin_group}" "${APPDIR}/usr/plugins/"
        fi
    done
fi

if [ -n "${QT_QML}" ] && [ -d "${QT_QML}" ]; then
    if [ -d "${QT_QML}/QtQuick" ]; then
        cp -r "${QT_QML}/QtQuick" "${APPDIR}/usr/qml/"
    fi
    if [ -d "${QT_QML}/QtMultimedia" ]; then
        cp -r "${QT_QML}/QtMultimedia" "${APPDIR}/usr/qml/"
    fi
    if [ -d "${QT_QML}/org/kde" ]; then
        mkdir -p "${APPDIR}/usr/qml/org/kde"
        cp -r "${QT_QML}/org/kde/"* "${APPDIR}/usr/qml/org/kde/" 2>/dev/null || true
    fi
fi

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
    echo "Downloading appimagetool..."
    curl -sL -o "${TOOLS_DIR}/appimagetool" https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
    chmod +x "${TOOLS_DIR}/appimagetool"
fi

export APPIMAGE_EXTRACT_AND_RUN=1
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
