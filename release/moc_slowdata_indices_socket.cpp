/****************************************************************************
** Meta object code from reading C++ file 'slowdata_indices_socket.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../SlowDataIndices/slowdata_indices_socket.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slowdata_indices_socket.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS = QtMocHelpers::stringData(
    "Slowdata_Indices_Socket",
    "dataSignal",
    "",
    "Indices_Data_Struct",
    "data",
    "socket_conn_info_Signal",
    "info",
    "received_dataSignal"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS_t {
    uint offsetsAndSizes[16];
    char stringdata0[24];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[5];
    char stringdata5[24];
    char stringdata6[5];
    char stringdata7[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS_t qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS = {
    {
        QT_MOC_LITERAL(0, 23),  // "Slowdata_Indices_Socket"
        QT_MOC_LITERAL(24, 10),  // "dataSignal"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 19),  // "Indices_Data_Struct"
        QT_MOC_LITERAL(56, 4),  // "data"
        QT_MOC_LITERAL(61, 23),  // "socket_conn_info_Signal"
        QT_MOC_LITERAL(85, 4),  // "info"
        QT_MOC_LITERAL(90, 19)   // "received_dataSignal"
    },
    "Slowdata_Indices_Socket",
    "dataSignal",
    "",
    "Indices_Data_Struct",
    "data",
    "socket_conn_info_Signal",
    "info",
    "received_dataSignal"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSlowdata_Indices_SocketENDCLASS[] = {

 // content:
      12,       // revision
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

Q_CONSTINIT const QMetaObject Slowdata_Indices_Socket::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSlowdata_Indices_SocketENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Slowdata_Indices_Socket, std::true_type>,
        // method 'dataSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Indices_Data_Struct, std::false_type>,
        // method 'socket_conn_info_Signal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'received_dataSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>
    >,
    nullptr
} };

void Slowdata_Indices_Socket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Slowdata_Indices_Socket *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dataSignal((*reinterpret_cast< std::add_pointer_t<Indices_Data_Struct>>(_a[1]))); break;
        case 1: _t->socket_conn_info_Signal((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->received_dataSignal((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Slowdata_Indices_Socket::*)(Indices_Data_Struct );
            if (_t _q_method = &Slowdata_Indices_Socket::dataSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Slowdata_Indices_Socket::*)(QString );
            if (_t _q_method = &Slowdata_Indices_Socket::socket_conn_info_Signal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Slowdata_Indices_Socket::*)(QStringList );
            if (_t _q_method = &Slowdata_Indices_Socket::received_dataSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *Slowdata_Indices_Socket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Slowdata_Indices_Socket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSlowdata_Indices_SocketENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Slowdata_Indices_Socket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Slowdata_Indices_Socket::dataSignal(Indices_Data_Struct _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Slowdata_Indices_Socket::socket_conn_info_Signal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Slowdata_Indices_Socket::received_dataSignal(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
