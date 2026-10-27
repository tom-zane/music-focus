/****************************************************************************
** Meta object code from reading C++ file 'AppController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/AppController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AppController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13AppControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto AppController::qt_create_metaobjectdata<qt_meta_tag_ZN13AppControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AppController",
        "userActiveChanged",
        "",
        "active",
        "appStateChanged",
        "initialSetupRequired",
        "notificationRequested",
        "message",
        "isError",
        "onScanCompleted",
        "count",
        "error",
        "onAudioError",
        "initialize",
        "reportActivity",
        "setUserActive",
        "selectMusicDirectory",
        "dirPath",
        "promptForMusicDirectory",
        "validateDirectory",
        "QVariantMap",
        "completeOnboarding",
        "retryDirectoryAccess",
        "rescanLibrary",
        "rescanWallpapers",
        "userActive",
        "isFirstLaunchSetup",
        "isRecoveryRequired",
        "settings",
        "SettingsManager*",
        "trackModel",
        "TrackModel*",
        "audioPlayer",
        "AudioPlayer*",
        "playlistManager",
        "PlaylistManager*",
        "wallpaperManager",
        "WallpaperManager*",
        "libraryScanner",
        "LibraryScanner*",
        "todoManager",
        "TodoManager*",
        "layoutController",
        "LayoutController*",
        "contrastManager",
        "ContrastManager*"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'userActiveChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'appStateChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'initialSetupRequired'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'notificationRequested'
        QtMocHelpers::SignalData<void(const QString &, bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 7 }, { QMetaType::Bool, 8 },
        }}),
        // Slot 'onScanCompleted'
        QtMocHelpers::SlotData<void(int, const QString &)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::QString, 11 },
        }}),
        // Slot 'onAudioError'
        QtMocHelpers::SlotData<void(const QString &)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 7 },
        }}),
        // Method 'initialize'
        QtMocHelpers::MethodData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reportActivity'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setUserActive'
        QtMocHelpers::MethodData<void(bool)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Method 'selectMusicDirectory'
        QtMocHelpers::MethodData<void(const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 },
        }}),
        // Method 'promptForMusicDirectory'
        QtMocHelpers::MethodData<bool()>(18, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'validateDirectory'
        QtMocHelpers::MethodData<QVariantMap(const QString &)>(19, 2, QMC::AccessPublic, 0x80000000 | 20, {{
            { QMetaType::QString, 17 },
        }}),
        // Method 'completeOnboarding'
        QtMocHelpers::MethodData<bool(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 17 },
        }}),
        // Method 'retryDirectoryAccess'
        QtMocHelpers::MethodData<bool()>(22, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'rescanLibrary'
        QtMocHelpers::MethodData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'rescanWallpapers'
        QtMocHelpers::MethodData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'userActive'
        QtMocHelpers::PropertyData<bool>(25, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'isFirstLaunchSetup'
        QtMocHelpers::PropertyData<bool>(26, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'isRecoveryRequired'
        QtMocHelpers::PropertyData<bool>(27, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'settings'
        QtMocHelpers::PropertyData<SettingsManager*>(28, 0x80000000 | 29, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'trackModel'
        QtMocHelpers::PropertyData<TrackModel*>(30, 0x80000000 | 31, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'audioPlayer'
        QtMocHelpers::PropertyData<AudioPlayer*>(32, 0x80000000 | 33, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'playlistManager'
        QtMocHelpers::PropertyData<PlaylistManager*>(34, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'wallpaperManager'
        QtMocHelpers::PropertyData<WallpaperManager*>(36, 0x80000000 | 37, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'libraryScanner'
        QtMocHelpers::PropertyData<LibraryScanner*>(38, 0x80000000 | 39, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'todoManager'
        QtMocHelpers::PropertyData<TodoManager*>(40, 0x80000000 | 41, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'layoutController'
        QtMocHelpers::PropertyData<LayoutController*>(42, 0x80000000 | 43, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
        // property 'contrastManager'
        QtMocHelpers::PropertyData<ContrastManager*>(44, 0x80000000 | 45, QMC::DefaultPropertyFlags | QMC::EnumOrFlag | QMC::Constant),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AppController, qt_meta_tag_ZN13AppControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AppController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13AppControllerE_t>.metaTypes,
    nullptr
} };

void AppController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AppController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->userActiveChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->appStateChanged(); break;
        case 2: _t->initialSetupRequired(); break;
        case 3: _t->notificationRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->onScanCompleted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->onAudioError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->initialize(); break;
        case 7: _t->reportActivity(); break;
        case 8: _t->setUserActive((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 9: _t->selectMusicDirectory((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: { bool _r = _t->promptForMusicDirectory();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { QVariantMap _r = _t->validateDirectory((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 12: { bool _r = _t->completeOnboarding((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->retryDirectoryAccess();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->rescanLibrary(); break;
        case 15: _t->rescanWallpapers(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(bool )>(_a, &AppController::userActiveChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::appStateChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)()>(_a, &AppController::initialSetupRequired, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AppController::*)(const QString & , bool )>(_a, &AppController::notificationRequested, 3))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AudioPlayer* >(); break;
        case 11:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ContrastManager* >(); break;
        case 10:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LayoutController* >(); break;
        case 8:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< LibraryScanner* >(); break;
        case 6:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PlaylistManager* >(); break;
        case 3:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SettingsManager* >(); break;
        case 9:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TodoManager* >(); break;
        case 4:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< TrackModel* >(); break;
        case 7:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< WallpaperManager* >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->isUserActive(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isFirstLaunchSetup(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isRecoveryRequired(); break;
        case 3: *reinterpret_cast<SettingsManager**>(_v) = _t->settings(); break;
        case 4: *reinterpret_cast<TrackModel**>(_v) = _t->trackModel(); break;
        case 5: *reinterpret_cast<AudioPlayer**>(_v) = _t->audioPlayer(); break;
        case 6: *reinterpret_cast<PlaylistManager**>(_v) = _t->playlistManager(); break;
        case 7: *reinterpret_cast<WallpaperManager**>(_v) = _t->wallpaperManager(); break;
        case 8: *reinterpret_cast<LibraryScanner**>(_v) = _t->libraryScanner(); break;
        case 9: *reinterpret_cast<TodoManager**>(_v) = _t->todoManager(); break;
        case 10: *reinterpret_cast<LayoutController**>(_v) = _t->layoutController(); break;
        case 11: *reinterpret_cast<ContrastManager**>(_v) = _t->contrastManager(); break;
        default: break;
        }
    }
}

const QMetaObject *AppController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AppControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AppController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void AppController::userActiveChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void AppController::appStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AppController::initialSetupRequired()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AppController::notificationRequested(const QString & _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP
