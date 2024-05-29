/****************************************************************************
** Meta object code from reading C++ file 'slowdata.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../SlowData/slowdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slowdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SlowData_t {
    uint offsetsAndSizes[14];
    char stringdata0[9];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[17];
    char stringdata5[16];
    char stringdata6[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SlowData_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_SlowData_t qt_meta_stringdata_SlowData = {
    {
        QT_MOC_LITERAL(0, 8),  // "SlowData"
        QT_MOC_LITERAL(9, 21),  // "socket_conn_info_Slot"
        QT_MOC_LITERAL(31, 0),  // ""
        QT_MOC_LITERAL(32, 4),  // "info"
        QT_MOC_LITERAL(37, 16),  // "dataExchangeSlot"
        QT_MOC_LITERAL(54, 15),  // "MBP_Data_Struct"
        QT_MOC_LITERAL(70, 4)   // "data"
    },
    "SlowData",
    "socket_conn_info_Slot",
    "",
    "info",
    "dataExchangeSlot",
    "MBP_Data_Struct",
    "data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlowData[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x08,    1 /* Private */,
       4,    1,   29,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void SlowData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SlowData *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->socket_conn_info_Slot((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->dataExchangeSlot((*reinterpret_cast< std::add_pointer_t<MBP_Data_Struct>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject SlowData::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SlowData.offsetsAndSizes,
    qt_meta_data_SlowData,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_SlowData_t
, QtPrivate::TypeAndForceComplete<SlowData, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<MBP_Data_Struct, std::false_type>


>,
    nullptr
} };


const QMetaObject *SlowData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlowData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SlowData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SlowData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
