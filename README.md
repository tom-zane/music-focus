# Music Focus

<p align="center">
  <img src="icon.png" alt="Music Focus Logo" width="128" height="128">
</p>

<p align="center">
  <strong>Distraction-free ambient music player, Pomodoro timer, and desktop productivity suite.</strong>
</p>

<p align="center">
  <a href="LICENSE"><img src="https://img.shields.io/badge/License-GPLv3-blue.svg" alt="License: GPL v3"></a>
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg" alt="C++20">
  <img src="https://img.shields.io/badge/Qt-6.4+-41CD52.svg?logo=qt" alt="Qt 6">
  <img src="https://img.shields.io/badge/KDE-KF6-1D99F3.svg?logo=kde" alt="KDE Frameworks 6">
  <img src="https://img.shields.io/badge/Platform-Linux-orange.svg?logo=linux" alt="Linux">
</p>

---

## Overview

**Music Focus** is a native Linux desktop application crafted for deep work, studying, and focused coding sessions. Built with modern C++20, Qt 6, QML, and KDE Frameworks 6, it combines a distraction-free audio player, an adaptable Pomodoro timer, task tracking, and ambient desktop widgets into an elegant frosted-glass interface.

---

## Features

- ⏱️ **Adaptive Pomodoro Timer**
  - Configurable focus, short break, and long break intervals.
  - Visual breathing glow indicator and audio chimes for session transitions.
  - Responsive layout: expands to full controls or shrinks down to an ultra-compact status bar for minimal screen footprint.

- 🎵 **Integrated Ambient Music Player**
  - Native multi-format audio playback (FLAC, MP3, OGG, WAV).
  - Background directory scanner and automatic playlist indexing.
  - Shuffle, repeat, track seeking, and media shortcut support.

- 📊 **Focus Analytics & Heatmap**
  - Session history and daily focus time tracking.
  - Interactive activity heatmaps to visualize consistency over weeks and months.

- 🖥️ **Customizable 120×80 Desktop Grid**
  - Freely move and resize widgets (Timer, Player, Clock, Todo List).
  - Snap-to-grid layout editing with persisted coordinate layouts.
  - Graceful responsive scaling with strict minimum boundary constraints.

- 🖼️ **Dynamic Wallpaper Engine**
  - Still wallpapers, seamless looping video backgrounds, and procedural animated gradients.
  - Smart contrast manager dynamically adjusting text and icon contrast against dynamic backgrounds.

- ✅ **Study Todo Checklist**
  - On-screen task management designed for single-session focus goals.
  - Persistent state between sessions.

- 🌊 **Audio Visualizer**
  - Real-time reactive waveform and frequency spectrum visualizer.

- 🎨 **Modern Frosted Glass Aesthetic**
  - Deep acrylic blur, smooth Kirigami animations, and dark/light system theme harmony.

---

## Tech Stack & Architecture

- **Language:** C++20
- **Framework:** Qt 6 (Core, Gui, Widgets, Quick, QuickControls2, Multimedia, Qml)
- **Desktop Integration:** KDE Frameworks 6 (KF6 CoreAddons, Kirigami)
- **Build System:** CMake 3.20+
- **Testing:** CTest / QtTest suite covering settings persistence, layouts, playlist queueing, and timer state machines

---

## Building from Source

### Prerequisites

Ensure you have a modern C++ compiler (`gcc` >= 11 or `clang` >= 13), CMake, Qt 6, and KDE Frameworks 6 installed.

#### Ubuntu / Debian (24.04+)
```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    extra-cmake-modules \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-multimedia-dev \
    libkf6coreaddons-dev \
    libkf6kirigami-dev \
    qml6-module-qtquick-layouts \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-effects \
    qml6-module-qtmultimedia \
    qml6-module-org-kde-kirigami
```

#### Arch Linux
```bash
sudo pacman -S --needed \
    base-devel \
    cmake \
    extra-cmake-modules \
    qt6-base \
    qt6-declarative \
    qt6-multimedia \
    kcoreaddons \
    kirigami
```

#### Fedora
```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    extra-cmake-modules \
    qt6-qtbase-devel \
    qt6-qtdeclarative-devel \
    qt6-qtmultimedia-devel \
    kf6-kcoreaddons-devel \
    kf6-kirigami-devel
```

### Compilation

```bash
# Clone the repository
git clone https://github.com/tom-zane/music-focus.git
cd music-focus

# Configure CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build executable
cmake --build build -j$(nproc)
```

### Running Tests

Execute the automated backend test suite:
```bash
ctest --test-dir build --output-on-failure
```

### Running the Application

```bash
./build/music-focus
```

---

## Packaging

### AppImage

Music Focus includes a complete standalone AppImage build script:

```bash
./packaging/build_appimage.sh
```

The script compiles the application in release mode, collects all Qt 6 and KF6 shared libraries and QML plugins into an AppDir, downloads `appimagetool` if necessary, and produces a portable `MusicFocus-x86_64.AppImage`.

### Continuous Integration

The repository includes a GitHub Actions workflow (`.github/workflows/build-appimage.yml`) that:
- Triggers on every push and pull request to `master`.
- Compiles the application and executes the unit test suite.
- Assembles and verifies the AppImage bundle.
- Automatically publishes release assets when a version tag (`v*`) is pushed.

---

## License

This project is licensed under the GNU General Public License v3.0 — see the [LICENSE](LICENSE) file for details.
