/****************************************************************************
** Meta object code from reading C++ file 'switchbutton.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../SwitchButton/switchbutton.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'switchbutton.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSSwitchButtonENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSwitchButtonENDCLASS = QtMocHelpers::stringData(
    "SwitchButton",
    "valueChanged",
    "",
    "newvalue"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSwitchButtonENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[13];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSwitchButtonENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSwitchButtonENDCLASS_t qt_meta_stringdata_CLASSSwitchButtonENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "SwitchButton"
        QT_MOC_LITERAL(13, 12),  // "valueChanged"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 8)   // "newvalue"
    },
    "SwitchButton",
    "valueChanged",
    "",
    "newvalue"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSwitchButtonENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject SwitchButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSwitchButtonENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSwitchButtonENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSwitchButtonENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SwitchButton, std::true_type>,
        // method 'valueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void SwitchButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SwitchButton *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SwitchButton::*)(bool );
            if (_t _q_method = &SwitchButton::valueChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *SwitchButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwitchButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSwitchButtonENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SwitchButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SwitchButton::valueChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS = QtMocHelpers::stringData(
    "SwitchButton::SwitchBackground"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS_t qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS = {
    {
        QT_MOC_LITERAL(0, 30)   // "SwitchButton::SwitchBackground"
    },
    "SwitchButton::SwitchBackground"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject SwitchButton::SwitchBackground::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SwitchBackground, std::true_type>
    >,
    nullptr
} };

void SwitchButton::SwitchBackground::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *SwitchButton::SwitchBackground::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwitchButton::SwitchBackground::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchBackgroundENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SwitchButton::SwitchBackground::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS = QtMocHelpers::stringData(
    "SwitchButton::SwitchCircle"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS_t qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS = {
    {
        QT_MOC_LITERAL(0, 26)   // "SwitchButton::SwitchCircle"
    },
    "SwitchButton::SwitchCircle"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSwitchButtonSCOPESwitchCircleENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject SwitchButton::SwitchCircle::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSwitchButtonSCOPESwitchCircleENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SwitchCircle, std::true_type>
    >,
    nullptr
} };

void SwitchButton::SwitchCircle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *SwitchButton::SwitchCircle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwitchButton::SwitchCircle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSwitchButtonSCOPESwitchCircleENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SwitchButton::SwitchCircle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
