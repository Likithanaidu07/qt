/****************************************************************************
** Meta object code from reading C++ file 'slowdataindices.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../SlowDataIndices/slowdataindices.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slowdataindices.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS = QtMocHelpers::stringData(
    "SlowDataIndices",
    "indicesDataSignal",
    "",
    "Indices_Data_Struct",
    "data",
    "socket_conn_info_Slot",
    "info",
    "dataExchangeSlot"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[16];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[5];
    char stringdata5[22];
    char stringdata6[5];
    char stringdata7[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS_t qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "SlowDataIndices"
        QT_MOC_LITERAL(16, 17),  // "indicesDataSignal"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 19),  // "Indices_Data_Struct"
        QT_MOC_LITERAL(55, 4),  // "data"
        QT_MOC_LITERAL(60, 21),  // "socket_conn_info_Slot"
        QT_MOC_LITERAL(82, 4),  // "info"
        QT_MOC_LITERAL(87, 16)   // "dataExchangeSlot"
    },
    "SlowDataIndices",
    "indicesDataSignal",
    "",
    "Indices_Data_Struct",
    "data",
    "socket_conn_info_Slot",
    "info",
    "dataExchangeSlot"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSlowDataIndicesENDCLASS[] = {

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
       5,    1,   35,    2, 0x08,    3 /* Private */,
       7,    1,   38,    2, 0x08,    5 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject SlowDataIndices::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSlowDataIndicesENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SlowDataIndices, std::true_type>,
        // method 'indicesDataSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Indices_Data_Struct, std::false_type>,
        // method 'socket_conn_info_Slot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'dataExchangeSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Indices_Data_Struct, std::false_type>
    >,
    nullptr
} };

void SlowDataIndices::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SlowDataIndices *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->indicesDataSignal((*reinterpret_cast< std::add_pointer_t<Indices_Data_Struct>>(_a[1]))); break;
        case 1: _t->socket_conn_info_Slot((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->dataExchangeSlot((*reinterpret_cast< std::add_pointer_t<Indices_Data_Struct>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SlowDataIndices::*)(Indices_Data_Struct );
            if (_t _q_method = &SlowDataIndices::indicesDataSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *SlowDataIndices::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlowDataIndices::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSlowDataIndicesENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SlowDataIndices::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void SlowDataIndices::indicesDataSignal(Indices_Data_Struct _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
