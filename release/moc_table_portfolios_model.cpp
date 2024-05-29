/****************************************************************************
** Meta object code from reading C++ file 'table_portfolios_model.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../PortFolio/table_portfolios_model.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_portfolios_model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Table_Portfolios_Model_t {
    uint offsetsAndSizes[28];
    char stringdata0[23];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[12];
    char stringdata5[4];
    char stringdata6[13];
    char stringdata7[4];
    char stringdata8[4];
    char stringdata9[29];
    char stringdata10[9];
    char stringdata11[22];
    char stringdata12[6];
    char stringdata13[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_Table_Portfolios_Model_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_Table_Portfolios_Model_t qt_meta_stringdata_Table_Portfolios_Model = {
    {
        QT_MOC_LITERAL(0, 22),  // "Table_Portfolios_Model"
        QT_MOC_LITERAL(23, 13),  // "editCompleted"
        QT_MOC_LITERAL(37, 0),  // ""
        QT_MOC_LITERAL(38, 4),  // "text"
        QT_MOC_LITERAL(43, 11),  // "QModelIndex"
        QT_MOC_LITERAL(55, 3),  // "idx"
        QT_MOC_LITERAL(59, 12),  // "edit_Started"
        QT_MOC_LITERAL(72, 3),  // "row"
        QT_MOC_LITERAL(76, 3),  // "col"
        QT_MOC_LITERAL(80, 28),  // "resizePortFolioTableColWidth"
        QT_MOC_LITERAL(109, 8),  // "colWidth"
        QT_MOC_LITERAL(118, 21),  // "updateDBOnDataChanged"
        QT_MOC_LITERAL(140, 5),  // "index"
        QT_MOC_LITERAL(146, 13)   // "onItemChanged"
    },
    "Table_Portfolios_Model",
    "editCompleted",
    "",
    "text",
    "QModelIndex",
    "idx",
    "edit_Started",
    "row",
    "col",
    "resizePortFolioTableColWidth",
    "colWidth",
    "updateDBOnDataChanged",
    "index",
    "onItemChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Table_Portfolios_Model[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   44,    2, 0x106,    1 /* Public | MethodIsConst  */,
       6,    2,   49,    2, 0x106,    4 /* Public | MethodIsConst  */,
       9,    1,   54,    2, 0x06,    7 /* Public */,
      11,    1,   57,    2, 0x06,    9 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    1,   60,    2, 0x0a,   11 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 4,   12,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,   12,

       0        // eod
};

void Table_Portfolios_Model::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Table_Portfolios_Model *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->editCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 1: _t->edit_Started((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->resizePortFolioTableColWidth((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->updateDBOnDataChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 4: _t->onItemChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Table_Portfolios_Model::*)(QString , QModelIndex ) const;
            if (_t _q_method = &Table_Portfolios_Model::editCompleted; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Table_Portfolios_Model::*)(int , int ) const;
            if (_t _q_method = &Table_Portfolios_Model::edit_Started; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Table_Portfolios_Model::*)(int );
            if (_t _q_method = &Table_Portfolios_Model::resizePortFolioTableColWidth; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Table_Portfolios_Model::*)(QModelIndex );
            if (_t _q_method = &Table_Portfolios_Model::updateDBOnDataChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject Table_Portfolios_Model::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_Table_Portfolios_Model.offsetsAndSizes,
    qt_meta_data_Table_Portfolios_Model,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_Table_Portfolios_Model_t
, QtPrivate::TypeAndForceComplete<Table_Portfolios_Model, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>


>,
    nullptr
} };


const QMetaObject *Table_Portfolios_Model::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Table_Portfolios_Model::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Table_Portfolios_Model.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int Table_Portfolios_Model::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Table_Portfolios_Model::editCompleted(QString _t1, QModelIndex _t2)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(const_cast< Table_Portfolios_Model *>(this), &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Table_Portfolios_Model::edit_Started(int _t1, int _t2)const
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(const_cast< Table_Portfolios_Model *>(this), &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Table_Portfolios_Model::resizePortFolioTableColWidth(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Table_Portfolios_Model::updateDBOnDataChanged(QModelIndex _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
