/****************************************************************************
** Meta object code from reading C++ file 'table_portfolios_model.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PortFolio/table_portfolios_model.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_portfolios_model.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS = QtMocHelpers::stringData(
    "Table_Portfolios_Model",
    "editCompleted",
    "",
    "text",
    "QModelIndex",
    "idx",
    "edit_Started",
    "row",
    "col",
    "updateDBOnDataChanged",
    "index",
    "onItemChanged",
    "selectionChangedSlot",
    "currentIdx",
    "updateModelDataList",
    "QList<PortfolioObject*>",
    "portfolio_data_list",
    "clearTableFlg",
    "editCancelled"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS_t {
    uint offsetsAndSizes[38];
    char stringdata0[23];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[12];
    char stringdata5[4];
    char stringdata6[13];
    char stringdata7[4];
    char stringdata8[4];
    char stringdata9[22];
    char stringdata10[6];
    char stringdata11[14];
    char stringdata12[21];
    char stringdata13[11];
    char stringdata14[20];
    char stringdata15[24];
    char stringdata16[20];
    char stringdata17[14];
    char stringdata18[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS_t qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS = {
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
        QT_MOC_LITERAL(80, 21),  // "updateDBOnDataChanged"
        QT_MOC_LITERAL(102, 5),  // "index"
        QT_MOC_LITERAL(108, 13),  // "onItemChanged"
        QT_MOC_LITERAL(122, 20),  // "selectionChangedSlot"
        QT_MOC_LITERAL(143, 10),  // "currentIdx"
        QT_MOC_LITERAL(154, 19),  // "updateModelDataList"
        QT_MOC_LITERAL(174, 23),  // "QList<PortfolioObject*>"
        QT_MOC_LITERAL(198, 19),  // "portfolio_data_list"
        QT_MOC_LITERAL(218, 13),  // "clearTableFlg"
        QT_MOC_LITERAL(232, 13)   // "editCancelled"
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
    "updateDBOnDataChanged",
    "index",
    "onItemChanged",
    "selectionChangedSlot",
    "currentIdx",
    "updateModelDataList",
    "QList<PortfolioObject*>",
    "portfolio_data_list",
    "clearTableFlg",
    "editCancelled"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSTable_Portfolios_ModelENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   56,    2, 0x106,    1 /* Public | MethodIsConst  */,
       6,    2,   61,    2, 0x106,    4 /* Public | MethodIsConst  */,
       9,    1,   66,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,   69,    2, 0x0a,    9 /* Public */,
      12,    1,   72,    2, 0x0a,   11 /* Public */,
      14,    2,   75,    2, 0x0a,   13 /* Public */,
      18,    1,   80,    2, 0x0a,   16 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, 0x80000000 | 4,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,   10,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, 0x80000000 | 15, QMetaType::Bool,   16,   17,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject Table_Portfolios_Model::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractTableModel::staticMetaObject>(),
    qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSTable_Portfolios_ModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Table_Portfolios_Model, std::true_type>,
        // method 'editCompleted'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>,
        // method 'edit_Started'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateDBOnDataChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QModelIndex, std::false_type>,
        // method 'onItemChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'selectionChangedSlot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateModelDataList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QList<PortfolioObject*>, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'editCancelled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void Table_Portfolios_Model::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Table_Portfolios_Model *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->editCompleted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 1: _t->edit_Started((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->updateDBOnDataChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 3: _t->onItemChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 4: _t->selectionChangedSlot((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->updateModelDataList((*reinterpret_cast< std::add_pointer_t<QList<PortfolioObject*>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 6: _t->editCancelled((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
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
            using _t = void (Table_Portfolios_Model::*)(QModelIndex );
            if (_t _q_method = &Table_Portfolios_Model::updateDBOnDataChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *Table_Portfolios_Model::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Table_Portfolios_Model::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSTable_Portfolios_ModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int Table_Portfolios_Model::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
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
void Table_Portfolios_Model::updateDBOnDataChanged(QModelIndex _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
