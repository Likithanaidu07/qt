/****************************************************************************
** Meta object code from reading C++ file 'convert_to_algo_bfly_bid.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Convert_to_Algo/convert_to_algo_bfly_bid.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'convert_to_algo_bfly_bid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_add_algo_btfly_bid_t {
    uint offsetsAndSizes[16];
    char stringdata0[19];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[13];
    char stringdata5[12];
    char stringdata6[22];
    char stringdata7[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_add_algo_btfly_bid_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_add_algo_btfly_bid_t qt_meta_stringdata_add_algo_btfly_bid = {
    {
        QT_MOC_LITERAL(0, 18),  // "add_algo_btfly_bid"
        QT_MOC_LITERAL(19, 13),  // "slotStartHide"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 11),  // "slotEndHide"
        QT_MOC_LITERAL(46, 12),  // "itemSelected"
        QT_MOC_LITERAL(59, 11),  // "QModelIndex"
        QT_MOC_LITERAL(71, 21),  // "itemSelectedEndStrike"
        QT_MOC_LITERAL(93, 5)   // "index"
    },
    "add_algo_btfly_bid",
    "slotStartHide",
    "",
    "slotEndHide",
    "itemSelected",
    "QModelIndex",
    "itemSelectedEndStrike",
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_add_algo_btfly_bid[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x08,    1 /* Private */,
       3,    1,   41,    2, 0x08,    3 /* Private */,
       4,    1,   44,    2, 0x0a,    5 /* Public */,
       6,    1,   47,    2, 0x0a,    7 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, 0x80000000 | 5,    7,

       0        // eod
};

void add_algo_btfly_bid::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<add_algo_btfly_bid *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->slotStartHide((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->slotEndHide((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->itemSelected((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 3: _t->itemSelectedEndStrike((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject add_algo_btfly_bid::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_add_algo_btfly_bid.offsetsAndSizes,
    qt_meta_data_add_algo_btfly_bid,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_add_algo_btfly_bid_t
, QtPrivate::TypeAndForceComplete<add_algo_btfly_bid, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>


>,
    nullptr
} };


const QMetaObject *add_algo_btfly_bid::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *add_algo_btfly_bid::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_add_algo_btfly_bid.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int add_algo_btfly_bid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
