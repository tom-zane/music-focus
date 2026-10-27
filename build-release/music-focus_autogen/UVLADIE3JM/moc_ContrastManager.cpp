/****************************************************************************
** Meta object code from reading C++ file 'ContrastManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/ContrastManager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ContrastManager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15ContrastManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ContrastManager::qt_create_metaobjectdata<qt_meta_tag_ZN15ContrastManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ContrastManager",
        "contrastModeChanged",
        "",
        "colorsChanged",
        "updateLuminanceMap",
        "isDarkForRect",
        "normX",
        "normY",
        "normW",
        "normH",
        "primaryTextForRect",
        "QColor",
        "secondaryTextForRect",
        "iconColorForRect",
        "accentColorForRect",
        "buttonBgForRect",
        "hovered",
        "buttonBorderForRect",
        "panelBgForRect",
        "opacity",
        "panelBorderForRect",
        "relativeLuminanceForRect",
        "contrastRatio",
        "fg",
        "bg",
        "contrastMode",
        "isDarkBackground",
        "isDark",
        "primaryTextColor",
        "secondaryTextColor",
        "iconColor",
        "accentColor",
        "panelBackgroundColor",
        "panelBg",
        "panelBorderColor",
        "panelBorder",
        "buttonBackgroundColor",
        "buttonBg",
        "buttonBorderColor",
        "buttonBorder",
        "buttonHoverColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'contrastModeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'colorsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateLuminanceMap'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'isDarkForRect'
        QtMocHelpers::MethodData<bool(qreal, qreal, qreal, qreal) const>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'primaryTextForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(10, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'secondaryTextForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(12, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'iconColorForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(13, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'accentColorForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(14, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'buttonBgForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal, bool) const>(15, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
            { QMetaType::Bool, 16 },
        }}),
        // Method 'buttonBgForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(15, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'buttonBorderForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(17, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'panelBgForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal, qreal) const>(18, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
            { QMetaType::QReal, 19 },
        }}),
        // Method 'panelBgForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(18, 2, QMC::AccessPublic | QMC::MethodCloned, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'panelBorderForRect'
        QtMocHelpers::MethodData<QColor(qreal, qreal, qreal, qreal) const>(20, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'relativeLuminanceForRect'
        QtMocHelpers::MethodData<qreal(qreal, qreal, qreal, qreal) const>(21, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { QMetaType::QReal, 6 }, { QMetaType::QReal, 7 }, { QMetaType::QReal, 8 }, { QMetaType::QReal, 9 },
        }}),
        // Method 'contrastRatio'
        QtMocHelpers::MethodData<qreal(const QColor &, const QColor &) const>(22, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { 0x80000000 | 11, 23 }, { 0x80000000 | 11, 24 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'contrastMode'
        QtMocHelpers::PropertyData<int>(25, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'isDarkBackground'
        QtMocHelpers::PropertyData<bool>(26, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'isDark'
        QtMocHelpers::PropertyData<bool>(27, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'primaryTextColor'
        QtMocHelpers::PropertyData<QColor>(28, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'secondaryTextColor'
        QtMocHelpers::PropertyData<QColor>(29, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'iconColor'
        QtMocHelpers::PropertyData<QColor>(30, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'accentColor'
        QtMocHelpers::PropertyData<QColor>(31, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'panelBackgroundColor'
        QtMocHelpers::PropertyData<QColor>(32, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'panelBg'
        QtMocHelpers::PropertyData<QColor>(33, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'panelBorderColor'
        QtMocHelpers::PropertyData<QColor>(34, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'panelBorder'
        QtMocHelpers::PropertyData<QColor>(35, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'buttonBackgroundColor'
        QtMocHelpers::PropertyData<QColor>(36, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'buttonBg'
        QtMocHelpers::PropertyData<QColor>(37, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'buttonBorderColor'
        QtMocHelpers::PropertyData<QColor>(38, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'buttonBorder'
        QtMocHelpers::PropertyData<QColor>(39, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'buttonHoverColor'
        QtMocHelpers::PropertyData<QColor>(40, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ContrastManager, qt_meta_tag_ZN15ContrastManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ContrastManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ContrastManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ContrastManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15ContrastManagerE_t>.metaTypes,
    nullptr
} };

void ContrastManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ContrastManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->contrastModeChanged(); break;
        case 1: _t->colorsChanged(); break;
        case 2: _t->updateLuminanceMap(); break;
        case 3: { bool _r = _t->isDarkForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { QColor _r = _t->primaryTextForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 5: { QColor _r = _t->secondaryTextForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 6: { QColor _r = _t->iconColorForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 7: { QColor _r = _t->accentColorForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 8: { QColor _r = _t->buttonBgForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[5])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 9: { QColor _r = _t->buttonBgForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 10: { QColor _r = _t->buttonBorderForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 11: { QColor _r = _t->panelBgForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[5])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 12: { QColor _r = _t->panelBgForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 13: { QColor _r = _t->panelBorderForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<QColor*>(_a[0]) = std::move(_r); }  break;
        case 14: { qreal _r = _t->relativeLuminanceForRect((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[4])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 15: { qreal _r = _t->contrastRatio((*reinterpret_cast<std::add_pointer_t<QColor>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QColor>>(_a[2])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ContrastManager::*)()>(_a, &ContrastManager::contrastModeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ContrastManager::*)()>(_a, &ContrastManager::colorsChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->contrastMode(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isDarkBackground(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isDarkBackground(); break;
        case 3: *reinterpret_cast<QColor*>(_v) = _t->primaryTextColor(); break;
        case 4: *reinterpret_cast<QColor*>(_v) = _t->secondaryTextColor(); break;
        case 5: *reinterpret_cast<QColor*>(_v) = _t->iconColor(); break;
        case 6: *reinterpret_cast<QColor*>(_v) = _t->accentColor(); break;
        case 7: *reinterpret_cast<QColor*>(_v) = _t->panelBackgroundColor(); break;
        case 8: *reinterpret_cast<QColor*>(_v) = _t->panelBackgroundColor(); break;
        case 9: *reinterpret_cast<QColor*>(_v) = _t->panelBorderColor(); break;
        case 10: *reinterpret_cast<QColor*>(_v) = _t->panelBorderColor(); break;
        case 11: *reinterpret_cast<QColor*>(_v) = _t->buttonBackgroundColor(); break;
        case 12: *reinterpret_cast<QColor*>(_v) = _t->buttonBackgroundColor(); break;
        case 13: *reinterpret_cast<QColor*>(_v) = _t->buttonBorderColor(); break;
        case 14: *reinterpret_cast<QColor*>(_v) = _t->buttonBorderColor(); break;
        case 15: *reinterpret_cast<QColor*>(_v) = _t->buttonHoverColor(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setContrastMode(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ContrastManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContrastManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ContrastManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ContrastManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void ContrastManager::contrastModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ContrastManager::colorsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
