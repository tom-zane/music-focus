/****************************************************************************
** Meta object code from reading C++ file 'WallpaperManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/WallpaperManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WallpaperManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16WallpaperManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto WallpaperManager::qt_create_metaobjectdata<qt_meta_tag_ZN16WallpaperManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "WallpaperManager",
        "currentWallpaperUrlChanged",
        "",
        "availableWallpapersChanged",
        "currentVideoUrlChanged",
        "availableVideosChanged",
        "directoriesChanged",
        "directoryErrorChanged",
        "videoThumbnailReady",
        "fileName",
        "QUrl",
        "thumbnailUrl",
        "updateConfiguration",
        "onRotationTimeout",
        "rescan",
        "nextWallpaper",
        "previousWallpaper",
        "selectWallpaperByName",
        "nextVideo",
        "previousVideo",
        "selectVideoByName",
        "regenerateGradientSeed",
        "wallpaperUrlFor",
        "videoUrlFor",
        "videoThumbnailUrl",
        "currentWallpaperUrl",
        "hasWallpaper",
        "currentWallpaperName",
        "availableWallpapers",
        "wallpapersCount",
        "currentVideoUrl",
        "hasVideo",
        "currentVideoName",
        "availableVideos",
        "videosCount",
        "stillWallpapersPath",
        "videoWallpapersPath",
        "directoryError",
        "directoryErrorMessage"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentWallpaperUrlChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'availableWallpapersChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentVideoUrlChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'availableVideosChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'directoriesChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'directoryErrorChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'videoThumbnailReady'
        QtMocHelpers::SignalData<void(const QString &, const QUrl &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { 0x80000000 | 10, 11 },
        }}),
        // Slot 'updateConfiguration'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onRotationTimeout'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'rescan'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'nextWallpaper'
        QtMocHelpers::MethodData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'previousWallpaper'
        QtMocHelpers::MethodData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'selectWallpaperByName'
        QtMocHelpers::MethodData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'nextVideo'
        QtMocHelpers::MethodData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'previousVideo'
        QtMocHelpers::MethodData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'selectVideoByName'
        QtMocHelpers::MethodData<void(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'regenerateGradientSeed'
        QtMocHelpers::MethodData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'wallpaperUrlFor'
        QtMocHelpers::MethodData<QUrl(const QString &) const>(22, 2, QMC::AccessPublic, 0x80000000 | 10, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'videoUrlFor'
        QtMocHelpers::MethodData<QUrl(const QString &) const>(23, 2, QMC::AccessPublic, 0x80000000 | 10, {{
            { QMetaType::QString, 9 },
        }}),
        // Method 'videoThumbnailUrl'
        QtMocHelpers::MethodData<QUrl(const QString &)>(24, 2, QMC::AccessPublic, 0x80000000 | 10, {{
            { QMetaType::QString, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentWallpaperUrl'
        QtMocHelpers::PropertyData<QUrl>(25, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'hasWallpaper'
        QtMocHelpers::PropertyData<bool>(26, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'currentWallpaperName'
        QtMocHelpers::PropertyData<QString>(27, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'availableWallpapers'
        QtMocHelpers::PropertyData<QStringList>(28, QMetaType::QStringList, QMC::DefaultPropertyFlags, 1),
        // property 'wallpapersCount'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'currentVideoUrl'
        QtMocHelpers::PropertyData<QUrl>(30, 0x80000000 | 10, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 2),
        // property 'hasVideo'
        QtMocHelpers::PropertyData<bool>(31, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
        // property 'currentVideoName'
        QtMocHelpers::PropertyData<QString>(32, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'availableVideos'
        QtMocHelpers::PropertyData<QStringList>(33, QMetaType::QStringList, QMC::DefaultPropertyFlags, 3),
        // property 'videosCount'
        QtMocHelpers::PropertyData<int>(34, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
        // property 'stillWallpapersPath'
        QtMocHelpers::PropertyData<QString>(35, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'videoWallpapersPath'
        QtMocHelpers::PropertyData<QString>(36, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'directoryError'
        QtMocHelpers::PropertyData<bool>(37, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'directoryErrorMessage'
        QtMocHelpers::PropertyData<QString>(38, QMetaType::QString, QMC::DefaultPropertyFlags, 5),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<WallpaperManager, qt_meta_tag_ZN16WallpaperManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject WallpaperManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16WallpaperManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16WallpaperManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16WallpaperManagerE_t>.metaTypes,
    nullptr
} };

void WallpaperManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<WallpaperManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentWallpaperUrlChanged(); break;
        case 1: _t->availableWallpapersChanged(); break;
        case 2: _t->currentVideoUrlChanged(); break;
        case 3: _t->availableVideosChanged(); break;
        case 4: _t->directoriesChanged(); break;
        case 5: _t->directoryErrorChanged(); break;
        case 6: _t->videoThumbnailReady((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[2]))); break;
        case 7: _t->updateConfiguration(); break;
        case 8: _t->onRotationTimeout(); break;
        case 9: _t->rescan(); break;
        case 10: _t->nextWallpaper(); break;
        case 11: _t->previousWallpaper(); break;
        case 12: _t->selectWallpaperByName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->nextVideo(); break;
        case 14: _t->previousVideo(); break;
        case 15: _t->selectVideoByName((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->regenerateGradientSeed(); break;
        case 17: { QUrl _r = _t->wallpaperUrlFor((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QUrl*>(_a[0]) = std::move(_r); }  break;
        case 18: { QUrl _r = _t->videoUrlFor((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QUrl*>(_a[0]) = std::move(_r); }  break;
        case 19: { QUrl _r = _t->videoThumbnailUrl((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QUrl*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)()>(_a, &WallpaperManager::currentWallpaperUrlChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)()>(_a, &WallpaperManager::availableWallpapersChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)()>(_a, &WallpaperManager::currentVideoUrlChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)()>(_a, &WallpaperManager::availableVideosChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)()>(_a, &WallpaperManager::directoriesChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)()>(_a, &WallpaperManager::directoryErrorChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (WallpaperManager::*)(const QString & , const QUrl & )>(_a, &WallpaperManager::videoThumbnailReady, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QUrl*>(_v) = _t->currentWallpaperUrl(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->hasWallpaper(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->currentWallpaperName(); break;
        case 3: *reinterpret_cast<QStringList*>(_v) = _t->availableWallpapers(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->wallpapersCount(); break;
        case 5: *reinterpret_cast<QUrl*>(_v) = _t->currentVideoUrl(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->hasVideo(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->currentVideoName(); break;
        case 8: *reinterpret_cast<QStringList*>(_v) = _t->availableVideos(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->videosCount(); break;
        case 10: *reinterpret_cast<QString*>(_v) = _t->stillWallpapersPath(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->videoWallpapersPath(); break;
        case 12: *reinterpret_cast<bool*>(_v) = _t->directoryError(); break;
        case 13: *reinterpret_cast<QString*>(_v) = _t->directoryErrorMessage(); break;
        default: break;
        }
    }
}

const QMetaObject *WallpaperManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WallpaperManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16WallpaperManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WallpaperManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void WallpaperManager::currentWallpaperUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WallpaperManager::availableWallpapersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WallpaperManager::currentVideoUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void WallpaperManager::availableVideosChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void WallpaperManager::directoriesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void WallpaperManager::directoryErrorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void WallpaperManager::videoThumbnailReady(const QString & _t1, const QUrl & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}
QT_WARNING_POP
