/****************************************************************************
** Meta object code from reading C++ file 'slowdata.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../SlowData/slowdata.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slowdata.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSSlowDataENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSlowDataENDCLASS = QtMocHelpers::stringData(
    "SlowData",
    "newSlowDataReceivedSignal",
    "",
    "QHash<QString,MBP_Data_Struct>",
    "data",
    "socket_conn_info_Slot",
    "info",
    "dataExchangeSlot",
    "MBP_Data_Struct"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSlowDataENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[9];
    char stringdata1[26];
    char stringdata2[1];
    char stringdata3[31];
    char stringdata4[5];
    char stringdata5[22];
    char stringdata6[5];
    char stringdata7[17];
    char stringdata8[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSlowDataENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSlowDataENDCLASS_t qt_meta_stringdata_CLASSSlowDataENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "SlowData"
        QT_MOC_LITERAL(9, 25),  // "newSlowDataReceivedSignal"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 30),  // "QHash<QString,MBP_Data_Struct>"
        QT_MOC_LITERAL(67, 4),  // "data"
        QT_MOC_LITERAL(72, 21),  // "socket_conn_info_Slot"
        QT_MOC_LITERAL(94, 4),  // "info"
        QT_MOC_LITERAL(99, 16),  // "dataExchangeSlot"
        QT_MOC_LITERAL(116, 15)   // "MBP_Data_Struct"
    },
    "SlowData",
    "newSlowDataReceivedSignal",
    "",
    "QHash<QString,MBP_Data_Struct>",
    "data",
    "socket_conn_info_Slot",
    "info",
    "dataExchangeSlot",
    "MBP_Data_Struct"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSlowDataENDCLASS[] = {

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
    QMetaType::Void, 0x80000000 | 8,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject SlowData::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSSlowDataENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSlowDataENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSlowDataENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SlowData, std::true_type>,
        // method 'newSlowDataReceivedSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHash<QString,MBP_Data_Struct> &, std::false_type>,
        // method 'socket_conn_info_Slot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'dataExchangeSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<MBP_Data_Struct, std::false_type>
    >,
    nullptr
} };

void SlowData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SlowData *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newSlowDataReceivedSignal((*reinterpret_cast< std::add_pointer_t<QHash<QString,MBP_Data_Struct>>>(_a[1]))); break;
        case 1: _t->socket_conn_info_Slot((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->dataExchangeSlot((*reinterpret_cast< std::add_pointer_t<MBP_Data_Struct>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SlowData::*)(const QHash<QString,MBP_Data_Struct> & );
            if (_t _q_method = &SlowData::newSlowDataReceivedSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *SlowData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlowData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSlowDataENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SlowData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SlowData::newSlowDataReceivedSignal(const QHash<QString,MBP_Data_Struct> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
