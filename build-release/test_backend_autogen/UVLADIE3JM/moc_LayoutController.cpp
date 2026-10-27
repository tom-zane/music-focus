/****************************************************************************
** Meta object code from reading C++ file 'LayoutController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/LayoutController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LayoutController.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16LayoutControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto LayoutController::qt_create_metaobjectdata<qt_meta_tag_ZN16LayoutControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LayoutController",
        "editingChanged",
        "",
        "editing",
        "selectedWidgetIdChanged",
        "id",
        "layoutChanged",
        "widgetChanged",
        "gridToPixelX",
        "gx",
        "usableWidth",
        "insetLeft",
        "gridToPixelY",
        "gy",
        "usableHeight",
        "insetTop",
        "gridToPixelWidth",
        "gw",
        "gridToPixelHeight",
        "gh",
        "pixelToGridX",
        "px",
        "pixelToGridY",
        "py",
        "pixelToGridWidth",
        "pw",
        "pixelToGridHeight",
        "ph",
        "widgetLayout",
        "QVariantMap",
        "allWidgets",
        "QVariantList",
        "isWidgetRegistered",
        "widgetX",
        "widgetY",
        "widgetWidth",
        "widgetHeight",
        "widgetMinWidth",
        "widgetMinHeight",
        "widgetMaxWidth",
        "widgetMaxHeight",
        "widgetVisible",
        "checkCollision",
        "testX",
        "testY",
        "testW",
        "testH",
        "isWithinBounds",
        "canMove",
        "newX",
        "newY",
        "canResize",
        "newW",
        "newH",
        "moveWidget",
        "resizeWidget",
        "setWidgetVisible",
        "visible",
        "isMandatoryWidget",
        "addWidget",
        "removeWidget",
        "findAvailablePosition",
        "QPoint",
        "w",
        "h",
        "toggleEditing",
        "startEditing",
        "stopEditing",
        "resetLayout",
        "saveLayout",
        "loadLayout",
        "validateAndAdjustForScreen",
        "contentWidth",
        "contentHeight",
        "gridColumns",
        "gridRows",
        "layoutRevision",
        "selectedWidgetId"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'editingChanged'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'selectedWidgetIdChanged'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'layoutChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'widgetChanged'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'gridToPixelX'
        QtMocHelpers::MethodData<qreal(int, qreal, qreal) const>(8, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { QMetaType::Int, 9 }, { QMetaType::QReal, 10 }, { QMetaType::QReal, 11 },
        }}),
        // Method 'gridToPixelX'
        QtMocHelpers::MethodData<qreal(int, qreal) const>(8, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::QReal, {{
            { QMetaType::Int, 9 }, { QMetaType::QReal, 10 },
        }}),
        // Method 'gridToPixelY'
        QtMocHelpers::MethodData<qreal(int, qreal, qreal) const>(12, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { QMetaType::Int, 13 }, { QMetaType::QReal, 14 }, { QMetaType::QReal, 15 },
        }}),
        // Method 'gridToPixelY'
        QtMocHelpers::MethodData<qreal(int, qreal) const>(12, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::QReal, {{
            { QMetaType::Int, 13 }, { QMetaType::QReal, 14 },
        }}),
        // Method 'gridToPixelWidth'
        QtMocHelpers::MethodData<qreal(int, qreal) const>(16, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { QMetaType::Int, 17 }, { QMetaType::QReal, 10 },
        }}),
        // Method 'gridToPixelHeight'
        QtMocHelpers::MethodData<qreal(int, qreal) const>(18, 2, QMC::AccessPublic, QMetaType::QReal, {{
            { QMetaType::Int, 19 }, { QMetaType::QReal, 14 },
        }}),
        // Method 'pixelToGridX'
        QtMocHelpers::MethodData<int(qreal, qreal, qreal) const>(20, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QReal, 21 }, { QMetaType::QReal, 10 }, { QMetaType::QReal, 11 },
        }}),
        // Method 'pixelToGridX'
        QtMocHelpers::MethodData<int(qreal, qreal) const>(20, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Int, {{
            { QMetaType::QReal, 21 }, { QMetaType::QReal, 10 },
        }}),
        // Method 'pixelToGridY'
        QtMocHelpers::MethodData<int(qreal, qreal, qreal) const>(22, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QReal, 23 }, { QMetaType::QReal, 14 }, { QMetaType::QReal, 15 },
        }}),
        // Method 'pixelToGridY'
        QtMocHelpers::MethodData<int(qreal, qreal) const>(22, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Int, {{
            { QMetaType::QReal, 23 }, { QMetaType::QReal, 14 },
        }}),
        // Method 'pixelToGridWidth'
        QtMocHelpers::MethodData<int(qreal, qreal) const>(24, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QReal, 25 }, { QMetaType::QReal, 10 },
        }}),
        // Method 'pixelToGridHeight'
        QtMocHelpers::MethodData<int(qreal, qreal) const>(26, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QReal, 27 }, { QMetaType::QReal, 14 },
        }}),
        // Method 'widgetLayout'
        QtMocHelpers::MethodData<QVariantMap(const QString &) const>(28, 2, QMC::AccessPublic, 0x80000000 | 29, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'allWidgets'
        QtMocHelpers::MethodData<QVariantList() const>(30, 2, QMC::AccessPublic, 0x80000000 | 31),
        // Method 'isWidgetRegistered'
        QtMocHelpers::MethodData<bool(const QString &) const>(32, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetX'
        QtMocHelpers::MethodData<int(const QString &) const>(33, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetY'
        QtMocHelpers::MethodData<int(const QString &) const>(34, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetWidth'
        QtMocHelpers::MethodData<int(const QString &) const>(35, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetHeight'
        QtMocHelpers::MethodData<int(const QString &) const>(36, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetMinWidth'
        QtMocHelpers::MethodData<int(const QString &) const>(37, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetMinHeight'
        QtMocHelpers::MethodData<int(const QString &) const>(38, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetMaxWidth'
        QtMocHelpers::MethodData<int(const QString &) const>(39, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetMaxHeight'
        QtMocHelpers::MethodData<int(const QString &) const>(40, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'widgetVisible'
        QtMocHelpers::MethodData<bool(const QString &) const>(41, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'checkCollision'
        QtMocHelpers::MethodData<bool(const QString &, int, int, int, int) const>(42, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::Int, 43 }, { QMetaType::Int, 44 }, { QMetaType::Int, 45 },
            { QMetaType::Int, 46 },
        }}),
        // Method 'isWithinBounds'
        QtMocHelpers::MethodData<bool(int, int, int, int) const>(47, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 43 }, { QMetaType::Int, 44 }, { QMetaType::Int, 45 }, { QMetaType::Int, 46 },
        }}),
        // Method 'canMove'
        QtMocHelpers::MethodData<bool(const QString &, int, int) const>(48, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::Int, 49 }, { QMetaType::Int, 50 },
        }}),
        // Method 'canResize'
        QtMocHelpers::MethodData<bool(const QString &, int, int) const>(51, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::Int, 52 }, { QMetaType::Int, 53 },
        }}),
        // Method 'moveWidget'
        QtMocHelpers::MethodData<bool(const QString &, int, int)>(54, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::Int, 49 }, { QMetaType::Int, 50 },
        }}),
        // Method 'resizeWidget'
        QtMocHelpers::MethodData<bool(const QString &, int, int)>(55, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::Int, 52 }, { QMetaType::Int, 53 },
        }}),
        // Method 'setWidgetVisible'
        QtMocHelpers::MethodData<void(const QString &, bool)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 }, { QMetaType::Bool, 57 },
        }}),
        // Method 'isMandatoryWidget'
        QtMocHelpers::MethodData<bool(const QString &) const>(58, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'addWidget'
        QtMocHelpers::MethodData<bool(const QString &)>(59, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'removeWidget'
        QtMocHelpers::MethodData<bool(const QString &)>(60, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'findAvailablePosition'
        QtMocHelpers::MethodData<QPoint(int, int) const>(61, 2, QMC::AccessPublic, 0x80000000 | 62, {{
            { QMetaType::Int, 63 }, { QMetaType::Int, 64 },
        }}),
        // Method 'toggleEditing'
        QtMocHelpers::MethodData<void()>(65, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startEditing'
        QtMocHelpers::MethodData<void()>(66, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'stopEditing'
        QtMocHelpers::MethodData<void()>(67, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'resetLayout'
        QtMocHelpers::MethodData<void()>(68, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'saveLayout'
        QtMocHelpers::MethodData<void()>(69, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'loadLayout'
        QtMocHelpers::MethodData<void()>(70, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'validateAndAdjustForScreen'
        QtMocHelpers::MethodData<void(int, int)>(71, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 72 }, { QMetaType::Int, 73 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'gridColumns'
        QtMocHelpers::PropertyData<int>(74, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'gridRows'
        QtMocHelpers::PropertyData<int>(75, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Constant),
        // property 'layoutRevision'
        QtMocHelpers::PropertyData<int>(76, QMetaType::Int, QMC::DefaultPropertyFlags, 2),
        // property 'editing'
        QtMocHelpers::PropertyData<bool>(3, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'selectedWidgetId'
        QtMocHelpers::PropertyData<QString>(77, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LayoutController, qt_meta_tag_ZN16LayoutControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LayoutController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16LayoutControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16LayoutControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN16LayoutControllerE_t>.metaTypes,
    nullptr
} };

void LayoutController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LayoutController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->editingChanged((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->selectedWidgetIdChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->layoutChanged(); break;
        case 3: _t->widgetChanged((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: { qreal _r = _t->gridToPixelX((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 5: { qreal _r = _t->gridToPixelX((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 6: { qreal _r = _t->gridToPixelY((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 7: { qreal _r = _t->gridToPixelY((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 8: { qreal _r = _t->gridToPixelWidth((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 9: { qreal _r = _t->gridToPixelHeight((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<qreal*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->pixelToGridX((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->pixelToGridX((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->pixelToGridY((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[3])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 13: { int _r = _t->pixelToGridY((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->pixelToGridWidth((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 15: { int _r = _t->pixelToGridHeight((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 16: { QVariantMap _r = _t->widgetLayout((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 17: { QVariantList _r = _t->allWidgets();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 18: { bool _r = _t->isWidgetRegistered((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 19: { int _r = _t->widgetX((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 20: { int _r = _t->widgetY((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 21: { int _r = _t->widgetWidth((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 22: { int _r = _t->widgetHeight((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 23: { int _r = _t->widgetMinWidth((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 24: { int _r = _t->widgetMinHeight((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 25: { int _r = _t->widgetMaxWidth((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 26: { int _r = _t->widgetMaxHeight((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 27: { bool _r = _t->widgetVisible((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 28: { bool _r = _t->checkCollision((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 29: { bool _r = _t->isWithinBounds((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 30: { bool _r = _t->canMove((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 31: { bool _r = _t->canResize((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 32: { bool _r = _t->moveWidget((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 33: { bool _r = _t->resizeWidget((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 34: _t->setWidgetVisible((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 35: { bool _r = _t->isMandatoryWidget((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 36: { bool _r = _t->addWidget((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 37: { bool _r = _t->removeWidget((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 38: { QPoint _r = _t->findAvailablePosition((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QPoint*>(_a[0]) = std::move(_r); }  break;
        case 39: _t->toggleEditing(); break;
        case 40: _t->startEditing(); break;
        case 41: _t->stopEditing(); break;
        case 42: _t->resetLayout(); break;
        case 43: _t->saveLayout(); break;
        case 44: _t->loadLayout(); break;
        case 45: _t->validateAndAdjustForScreen((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LayoutController::*)(bool )>(_a, &LayoutController::editingChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayoutController::*)(const QString & )>(_a, &LayoutController::selectedWidgetIdChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayoutController::*)()>(_a, &LayoutController::layoutChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (LayoutController::*)(const QString & )>(_a, &LayoutController::widgetChanged, 3))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->gridColumns(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->gridRows(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->layoutRevision(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->isEditing(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->selectedWidgetId(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 3: _t->setEditing(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setSelectedWidgetId(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *LayoutController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LayoutController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN16LayoutControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LayoutController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 46)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 46;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 46)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 46;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void LayoutController::editingChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void LayoutController::selectedWidgetIdChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void LayoutController::layoutChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LayoutController::widgetChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
