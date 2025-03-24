/****************************************************************************
** Meta object code from reading C++ file 'table_portfolios_custom.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PortFolio/table_portfolios_custom.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_portfolios_custom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.3. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASStable_portfolios_customENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASStable_portfolios_customENDCLASS = QtMocHelpers::stringData(
    "table_portfolios_custom",
    "selectionChangedSignal",
    "",
    "currentRow",
    "handleTabKeyPressFromEditableCell",
    "nav_direction",
    "direction",
    "mousePressEvent",
    "QMouseEvent*",
    "event"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASStable_portfolios_customENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[24];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[34];
    char stringdata5[14];
    char stringdata6[10];
    char stringdata7[16];
    char stringdata8[13];
    char stringdata9[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASStable_portfolios_customENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASStable_portfolios_customENDCLASS_t qt_meta_stringdata_CLASStable_portfolios_customENDCLASS = {
    {
        QT_MOC_LITERAL(0, 23),  // "table_portfolios_custom"
        QT_MOC_LITERAL(24, 22),  // "selectionChangedSignal"
        QT_MOC_LITERAL(47, 0),  // ""
        QT_MOC_LITERAL(48, 10),  // "currentRow"
        QT_MOC_LITERAL(59, 33),  // "handleTabKeyPressFromEditable..."
        QT_MOC_LITERAL(93, 13),  // "nav_direction"
        QT_MOC_LITERAL(107, 9),  // "direction"
        QT_MOC_LITERAL(117, 15),  // "mousePressEvent"
        QT_MOC_LITERAL(133, 12),  // "QMouseEvent*"
        QT_MOC_LITERAL(146, 5)   // "event"
    },
    "table_portfolios_custom",
    "selectionChangedSignal",
    "",
    "currentRow",
    "handleTabKeyPressFromEditableCell",
    "nav_direction",
    "direction",
    "mousePressEvent",
    "QMouseEvent*",
    "event"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASStable_portfolios_customENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   35,    2, 0x0a,    3 /* Public */,
       7,    1,   38,    2, 0x0a,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

Q_CONSTINIT const QMetaObject table_portfolios_custom::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableView::staticMetaObject>(),
    qt_meta_stringdata_CLASStable_portfolios_customENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASStable_portfolios_customENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASStable_portfolios_customENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<table_portfolios_custom, std::true_type>,
        // method 'selectionChangedSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleTabKeyPressFromEditableCell'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<nav_direction, std::false_type>,
        // method 'mousePressEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>
    >,
    nullptr
} };

void table_portfolios_custom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<table_portfolios_custom *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->selectionChangedSignal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->handleTabKeyPressFromEditableCell((*reinterpret_cast< std::add_pointer_t<nav_direction>>(_a[1]))); break;
        case 2: _t->mousePressEvent((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (table_portfolios_custom::*)(int );
            if (_t _q_method = &table_portfolios_custom::selectionChangedSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *table_portfolios_custom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *table_portfolios_custom::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASStable_portfolios_customENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QTableView::qt_metacast(_clname);
}

int table_portfolios_custom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void table_portfolios_custom::selectionChangedSignal(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
