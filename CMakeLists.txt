cmake_minimum_required(VERSION 3.20)

project(music-focus VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickControls2
    Multimedia
    Qml
    Test
)

find_package(KF6Kirigami REQUIRED)
find_package(KF6CoreAddons REQUIRED)

set(SOURCES
    src/main.cpp
    src/SettingsManager.h
    src/SettingsManager.cpp
    src/TrackModel.h
    src/TrackModel.cpp
    src/AudioPlayer.h
    src/AudioPlayer.cpp
    src/LibraryScanner.h
    src/LibraryScanner.cpp
    src/WallpaperManager.h
    src/WallpaperManager.cpp
    src/PlaylistManager.h
    src/PlaylistManager.cpp
    src/AppController.h
    src/AppController.cpp
    src/TodoManager.h
    src/TodoManager.cpp
    src/LayoutController.h
    src/LayoutController.cpp
    src/ContrastManager.h
    src/ContrastManager.cpp
    src/PomodoroManager.h
    src/PomodoroManager.cpp
    resources.qrc
)

add_executable(music-focus ${SOURCES})

target_link_libraries(music-focus PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia
    KF6::Kirigami
    KF6::CoreAddons
)

# Install targets
install(TARGETS music-focus
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES resources/org.kde.musicfocus.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

install(FILES resources/icons/music-focus.svg
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
    RENAME org.kde.musicfocus.svg
)

foreach(SIZE 16 24 32 48 64 128 256 512)
    install(FILES resources/icons/hicolor/${SIZE}x${SIZE}/apps/music-focus.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${SIZE}x${SIZE}/apps
        RENAME org.kde.musicfocus.png
    )
endforeach()

# Testing target
enable_testing()
add_executable(test_backend
    tests/test_backend.cpp
    src/SettingsManager.cpp
    src/TrackModel.cpp
    src/AudioPlayer.cpp
    src/LibraryScanner.cpp
    src/WallpaperManager.cpp
    src/PlaylistManager.cpp
    src/TodoManager.cpp
    src/LayoutController.cpp
    src/ContrastManager.cpp
    src/PomodoroManager.cpp
    src/AppController.cpp
)

target_link_libraries(test_backend PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Test
    KF6::CoreAddons
)

target_compile_definitions(test_backend PRIVATE
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test_data/StudyMusic"
)

add_test(NAME BackendTest COMMAND test_backend)

