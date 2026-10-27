/****************************************************************************
** Meta object code from reading C++ file 'AudioPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/AudioPlayer.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioPlayer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11AudioPlayerE_t {};
} // unnamed namespace

template <> constexpr inline auto AudioPlayer::qt_create_metaobjectdata<qt_meta_tag_ZN11AudioPlayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AudioPlayer",
        "playbackStateChanged",
        "",
        "positionChanged",
        "durationChanged",
        "volumeChanged",
        "mutedChanged",
        "currentSourceChanged",
        "errorStateChanged",
        "trackEnded",
        "errorOccurred",
        "message",
        "spectrumChanged",
        "genuineSpectrumChanged",
        "onPlayerPlaybackStateChanged",
        "QMediaPlayer::PlaybackState",
        "state",
        "onPlayerMediaStatusChanged",
        "QMediaPlayer::MediaStatus",
        "status",
        "onPlayerPositionChanged",
        "position",
        "onPlayerDurationChanged",
        "duration",
        "onPlayerErrorOccurred",
        "QMediaPlayer::Error",
        "error",
        "errorString",
        "onAudioBufferReceived",
        "QAudioBuffer",
        "buffer",
        "onSpectrumDecayTick",
        "play",
        "pause",
        "togglePlayPause",
        "stop",
        "seek",
        "positionMs",
        "setPosition",
        "setVolume",
        "vol",
        "setMuted",
        "mute",
        "loadAndPlay",
        "QUrl",
        "source",
        "startPositionMs",
        "setSourceWithoutPlaying",
        "clearError",
        "playbackState",
        "isPlaying",
        "volume",
        "muted",
        "formattedPosition",
        "formattedDuration",
        "hasError",
        "errorMessage",
        "currentSource",
        "spectrum",
        "QList<qreal>",
        "currentEnergy",
        "hasGenuineSpectrum",
        "PlaybackState",
        "StoppedState",
        "PlayingState",
        "PausedState"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'playbackStateChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'positionChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'durationChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'volumeChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'mutedChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentSourceChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'errorStateChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'trackEnded'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Signal 'spectrumChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'genuineSpectrumChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onPlayerPlaybackStateChanged'
        QtMocHelpers::SlotData<void(QMediaPlayer::PlaybackState)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
        // Slot 'onPlayerMediaStatusChanged'
        QtMocHelpers::SlotData<void(QMediaPlayer::MediaStatus)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'onPlayerPositionChanged'
        QtMocHelpers::SlotData<void(qint64)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 21 },
        }}),
        // Slot 'onPlayerDurationChanged'
        QtMocHelpers::SlotData<void(qint64)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 23 },
        }}),
        // Slot 'onPlayerErrorOccurred'
        QtMocHelpers::SlotData<void(QMediaPlayer::Error, const QString &)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 25, 26 }, { QMetaType::QString, 27 },
        }}),
        // Slot 'onAudioBufferReceived'
        QtMocHelpers::SlotData<void(const QAudioBuffer &)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 29, 30 },
        }}),
        // Slot 'onSpectrumDecayTick'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'play'
        QtMocHelpers::MethodData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pause'
        QtMocHelpers::MethodData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'togglePlayPause'
        QtMocHelpers::MethodData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stop'
        QtMocHelpers::MethodData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'seek'
        QtMocHelpers::MethodData<void(qint64)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 37 },
        }}),
        // Method 'setPosition'
        QtMocHelpers::MethodData<void(qint64)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 37 },
        }}),
        // Method 'setVolume'
        QtMocHelpers::MethodData<void(qreal)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QReal, 40 },
        }}),
        // Method 'setMuted'
        QtMocHelpers::MethodData<void(bool)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 42 },
        }}),
        // Method 'loadAndPlay'
        QtMocHelpers::MethodData<void(const QUrl &, qint64)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 44, 45 }, { QMetaType::LongLong, 46 },
        }}),
        // Method 'loadAndPlay'
        QtMocHelpers::MethodData<void(const QUrl &)>(43, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 44, 45 },
        }}),
        // Method 'setSourceWithoutPlaying'
        QtMocHelpers::MethodData<void(const QUrl &, qint64)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 44, 45 }, { QMetaType::LongLong, 46 },
        }}),
        // Method 'setSourceWithoutPlaying'
        QtMocHelpers::MethodData<void(const QUrl &)>(47, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { 0x80000000 | 44, 45 },
        }}),
        // Method 'clearError'
        QtMocHelpers::MethodData<void()>(48, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'playbackState'
        QtMocHelpers::PropertyData<int>(49, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'isPlaying'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'position'
        QtMocHelpers::PropertyData<qint64>(21, QMetaType::LongLong, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'duration'
        QtMocHelpers::PropertyData<qint64>(23, QMetaType::LongLong, QMC::DefaultPropertyFlags, 2),
        // property 'volume'
        QtMocHelpers::PropertyData<qreal>(51, QMetaType::QReal, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'muted'
        QtMocHelpers::PropertyData<bool>(52, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'formattedPosition'
        QtMocHelpers::PropertyData<QString>(53, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'formattedDuration'
        QtMocHelpers::PropertyData<QString>(54, QMetaType::QString, QMC::DefaultPropertyFlags, 2),
        // property 'hasError'
        QtMocHelpers::PropertyData<bool>(55, QMetaType::Bool, QMC::DefaultPropertyFlags, 6),
        // property 'errorMessage'
        QtMocHelpers::PropertyData<QString>(56, QMetaType::QString, QMC::DefaultPropertyFlags, 6),
        // property 'currentSource'
        QtMocHelpers::PropertyData<QUrl>(57, 0x80000000 | 44, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 5),
        // property 'spectrum'
        QtMocHelpers::PropertyData<QList<qreal>>(58, 0x80000000 | 59, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 9),
        // property 'currentEnergy'
        QtMocHelpers::PropertyData<qreal>(60, QMetaType::QReal, QMC::DefaultPropertyFlags, 9),
        // property 'hasGenuineSpectrum'
        QtMocHelpers::PropertyData<bool>(61, QMetaType::Bool, QMC::DefaultPropertyFlags, 10),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'PlaybackState'
        QtMocHelpers::EnumData<enum PlaybackState>(62, 62, QMC::EnumFlags{}).add({
            {   63, PlaybackState::StoppedState },
            {   64, PlaybackState::PlayingState },
            {   65, PlaybackState::PausedState },
        }),
    };
    return QtMocHelpers::metaObjectData<AudioPlayer, qt_meta_tag_ZN11AudioPlayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AudioPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11AudioPlayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11AudioPlayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11AudioPlayerE_t>.metaTypes,
    nullptr
} };

void AudioPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AudioPlayer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->playbackStateChanged(); break;
        case 1: _t->positionChanged(); break;
        case 2: _t->durationChanged(); break;
        case 3: _t->volumeChanged(); break;
        case 4: _t->mutedChanged(); break;
        case 5: _t->currentSourceChanged(); break;
        case 6: _t->errorStateChanged(); break;
        case 7: _t->trackEnded(); break;
        case 8: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->spectrumChanged(); break;
        case 10: _t->genuineSpectrumChanged(); break;
        case 11: _t->onPlayerPlaybackStateChanged((*reinterpret_cast<std::add_pointer_t<QMediaPlayer::PlaybackState>>(_a[1]))); break;
        case 12: _t->onPlayerMediaStatusChanged((*reinterpret_cast<std::add_pointer_t<QMediaPlayer::MediaStatus>>(_a[1]))); break;
        case 13: _t->onPlayerPositionChanged((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 14: _t->onPlayerDurationChanged((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 15: _t->onPlayerErrorOccurred((*reinterpret_cast<std::add_pointer_t<QMediaPlayer::Error>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 16: _t->onAudioBufferReceived((*reinterpret_cast<std::add_pointer_t<QAudioBuffer>>(_a[1]))); break;
        case 17: _t->onSpectrumDecayTick(); break;
        case 18: _t->play(); break;
        case 19: _t->pause(); break;
        case 20: _t->togglePlayPause(); break;
        case 21: _t->stop(); break;
        case 22: _t->seek((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 23: _t->setPosition((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 24: _t->setVolume((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1]))); break;
        case 25: _t->setMuted((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 26: _t->loadAndPlay((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2]))); break;
        case 27: _t->loadAndPlay((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 28: _t->setSourceWithoutPlaying((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2]))); break;
        case 29: _t->setSourceWithoutPlaying((*reinterpret_cast<std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 30: _t->clearError(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::playbackStateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::positionChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::durationChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::volumeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::mutedChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::currentSourceChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::errorStateChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::trackEnded, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)(const QString & )>(_a, &AudioPlayer::errorOccurred, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::spectrumChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioPlayer::*)()>(_a, &AudioPlayer::genuineSpectrumChanged, 10))
            return;
    }
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<qreal> >(); break;
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->playbackState(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isPlaying(); break;
        case 2: *reinterpret_cast<qint64*>(_v) = _t->position(); break;
        case 3: *reinterpret_cast<qint64*>(_v) = _t->duration(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->volume(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->isMuted(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->formattedPosition(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->formattedDuration(); break;
        case 8: *reinterpret_cast<bool*>(_v) = _t->hasError(); break;
        case 9: *reinterpret_cast<QString*>(_v) = _t->errorMessage(); break;
        case 10: *reinterpret_cast<QUrl*>(_v) = _t->currentSource(); break;
        case 11: *reinterpret_cast<QList<qreal>*>(_v) = _t->spectrum(); break;
        case 12: *reinterpret_cast<qreal*>(_v) = _t->currentEnergy(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->hasGenuineSpectrum(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 2: _t->setPosition(*reinterpret_cast<qint64*>(_v)); break;
        case 4: _t->setVolume(*reinterpret_cast<qreal*>(_v)); break;
        case 5: _t->setMuted(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AudioPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11AudioPlayerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AudioPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 31;
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
void AudioPlayer::playbackStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AudioPlayer::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AudioPlayer::durationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AudioPlayer::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AudioPlayer::mutedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void AudioPlayer::currentSourceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void AudioPlayer::errorStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void AudioPlayer::trackEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void AudioPlayer::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void AudioPlayer::spectrumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void AudioPlayer::genuineSpectrumChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
