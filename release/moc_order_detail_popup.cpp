/****************************************************************************
** Meta object code from reading C++ file 'order_detail_popup.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../order_detail_popup.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'order_detail_popup.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS = QtMocHelpers::stringData(
    "OrderDetail_Popup",
    "dataLoaded",
    "",
    "QHash<QString,QString>",
    "OrderDetailsData",
    "updateUI"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[18];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[17];
    char stringdata5[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS_t qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS = {
    {
        QT_MOC_LITERAL(0, 17),  // "OrderDetail_Popup"
        QT_MOC_LITERAL(18, 10),  // "dataLoaded"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 22),  // "QHash<QString,QString>"
        QT_MOC_LITERAL(53, 16),  // "OrderDetailsData"
        QT_MOC_LITERAL(70, 8)   // "updateUI"
    },
    "OrderDetail_Popup",
    "dataLoaded",
    "",
    "QHash<QString,QString>",
    "OrderDetailsData",
    "updateUI"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSOrderDetail_PopupENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   29,    2, 0x0a,    3 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject OrderDetail_Popup::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSOrderDetail_PopupENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<OrderDetail_Popup, std::true_type>,
        // method 'dataLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<QString,QString>, std::false_type>,
        // method 'updateUI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QHash<QString,QString>, std::false_type>
    >,
    nullptr
} };

void OrderDetail_Popup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrderDetail_Popup *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dataLoaded((*reinterpret_cast< std::add_pointer_t<QHash<QString,QString>>>(_a[1]))); break;
        case 1: _t->updateUI((*reinterpret_cast< std::add_pointer_t<QHash<QString,QString>>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrderDetail_Popup::*)(QHash<QString,QString> );
            if (_t _q_method = &OrderDetail_Popup::dataLoaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *OrderDetail_Popup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderDetail_Popup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSOrderDetail_PopupENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int OrderDetail_Popup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void OrderDetail_Popup::dataLoaded(QHash<QString,QString> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
