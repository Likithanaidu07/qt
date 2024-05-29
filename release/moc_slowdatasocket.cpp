/****************************************************************************
** Meta object code from reading C++ file 'slowdatasocket.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../SlowData/slowdatasocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slowdatasocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SlowDataSocket_t {
    uint offsetsAndSizes[16];
    char stringdata0[15];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[5];
    char stringdata5[24];
    char stringdata6[5];
    char stringdata7[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_SlowDataSocket_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_SlowDataSocket_t qt_meta_stringdata_SlowDataSocket = {
    {
        QT_MOC_LITERAL(0, 14),  // "SlowDataSocket"
        QT_MOC_LITERAL(15, 10),  // "dataSignal"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 15),  // "MBP_Data_Struct"
        QT_MOC_LITERAL(43, 4),  // "data"
        QT_MOC_LITERAL(48, 23),  // "socket_conn_info_Signal"
        QT_MOC_LITERAL(72, 4),  // "info"
        QT_MOC_LITERAL(77, 19)   // "received_dataSignal"
    },
    "SlowDataSocket",
    "dataSignal",
    "",
    "MBP_Data_Struct",
    "data",
    "socket_conn_info_Signal",
    "info",
    "received_dataSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SlowDataSocket[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       5,    1,   35,    2, 0x06,    3 /* Public */,
       7,    1,   38,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QStringList,    4,

       0        // eod
};

void SlowDataSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SlowDataSocket *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dataSignal((*reinterpret_cast< std::add_pointer_t<MBP_Data_Struct>>(_a[1]))); break;
        case 1: _t->socket_conn_info_Signal((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->received_dataSignal((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SlowDataSocket::*)(MBP_Data_Struct );
            if (_t _q_method = &SlowDataSocket::dataSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SlowDataSocket::*)(QString );
            if (_t _q_method = &SlowDataSocket::socket_conn_info_Signal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SlowDataSocket::*)(QStringList );
            if (_t _q_method = &SlowDataSocket::received_dataSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject SlowDataSocket::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_SlowDataSocket.offsetsAndSizes,
    qt_meta_data_SlowDataSocket,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_SlowDataSocket_t
, QtPrivate::TypeAndForceComplete<SlowDataSocket, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<MBP_Data_Struct, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QStringList, std::false_type>



>,
    nullptr
} };


const QMetaObject *SlowDataSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SlowDataSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SlowDataSocket.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SlowDataSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void SlowDataSocket::dataSignal(MBP_Data_Struct _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SlowDataSocket::socket_conn_info_Signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SlowDataSocket::received_dataSignal(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
