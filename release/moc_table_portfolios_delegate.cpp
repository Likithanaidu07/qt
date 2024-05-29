/****************************************************************************
** Meta object code from reading C++ file 'table_portfolios_delegate.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../PortFolio/table_portfolios_delegate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_portfolios_delegate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Table_Portfolios_Delegate_t {
    uint offsetsAndSizes[16];
    char stringdata0[26];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[12];
    char stringdata5[4];
    char stringdata6[14];
    char stringdata7[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_Table_Portfolios_Delegate_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_Table_Portfolios_Delegate_t qt_meta_stringdata_Table_Portfolios_Delegate = {
    {
        QT_MOC_LITERAL(0, 25),  // "Table_Portfolios_Delegate"
        QT_MOC_LITERAL(26, 12),  // "editFinished"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 4),  // "text"
        QT_MOC_LITERAL(45, 11),  // "QModelIndex"
        QT_MOC_LITERAL(57, 3),  // "idx"
        QT_MOC_LITERAL(61, 13),  // "tabKeyPressed"
        QT_MOC_LITERAL(75, 13)   // "nav_direction"
    },
    "Table_Portfolios_Delegate",
    "editFinished",
    "",
    "text",
    "QModelIndex",
    "idx",
    "tabKeyPressed",
    "nav_direction"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Table_Portfolios_Delegate[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   26,    2, 0x106,    1 /* Public | MethodIsConst  */,
       6,    1,   31,    2, 0x06,    4 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void Table_Portfolios_Delegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Table_Portfolios_Delegate *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->editFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 1: _t->tabKeyPressed((*reinterpret_cast< std::add_pointer_t<nav_direction>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Table_Portfolios_Delegate::*)(QString , QModelIndex ) const;
            if (_t _q_method = &Table_Portfolios_Delegate::editFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Table_Portfolios_Delegate::*)(nav_direction );
            if (_t _q_method = &Table_Portfolios_Delegate::tabKeyPressed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Table_Portfolios_Delegate::staticMetaObject = { {
    QMetaObject::SuperData::link<QStyledItemDelegate::staticMetaObject>(),
    qt_meta_stringdata_Table_Portfolios_Delegate.offsetsAndSizes,
    qt_meta_data_Table_Portfolios_Delegate,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Table_Portfolios_Delegate_t
, QtPrivate::TypeAndForceComplete<Table_Portfolios_Delegate, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<nav_direction, std::false_type>



>,
    nullptr
} };


const QMetaObject *Table_Portfolios_Delegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Table_Portfolios_Delegate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Table_Portfolios_Delegate.stringdata0))
        return static_cast<void*>(this);
    return QStyledItemDelegate::qt_metacast(_clname);
}

int Table_Portfolios_Delegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStyledItemDelegate::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Table_Portfolios_Delegate::editFinished(QString _t1, QModelIndex _t2)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(const_cast< Table_Portfolios_Delegate *>(this), &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Table_Portfolios_Delegate::tabKeyPressed(nav_direction _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
