/****************************************************************************
** Meta object code from reading C++ file 'loginwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../loginwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginwindow.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSloginwindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSloginwindowENDCLASS = QtMocHelpers::stringData(
    "loginwindow",
    "loginStatus",
    "",
    "userInfo",
    "userData",
    "on_pushButtonlogin_clicked",
    "on_close_clicked",
    "on_minimize_clicked",
    "mousePressEvent",
    "QMouseEvent*",
    "event",
    "mouseMoveEvent",
    "keyPressEvent",
    "QKeyEvent*",
    "mouseReleaseEvent",
    "on_lineEditUsername_textEdited",
    "arg1",
    "on_lineEdit_Password_textEdited"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSloginwindowENDCLASS_t {
    uint offsetsAndSizes[36];
    char stringdata0[12];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[9];
    char stringdata5[27];
    char stringdata6[17];
    char stringdata7[20];
    char stringdata8[16];
    char stringdata9[13];
    char stringdata10[6];
    char stringdata11[15];
    char stringdata12[14];
    char stringdata13[11];
    char stringdata14[18];
    char stringdata15[31];
    char stringdata16[5];
    char stringdata17[32];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSloginwindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSloginwindowENDCLASS_t qt_meta_stringdata_CLASSloginwindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "loginwindow"
        QT_MOC_LITERAL(12, 11),  // "loginStatus"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 8),  // "userInfo"
        QT_MOC_LITERAL(34, 8),  // "userData"
        QT_MOC_LITERAL(43, 26),  // "on_pushButtonlogin_clicked"
        QT_MOC_LITERAL(70, 16),  // "on_close_clicked"
        QT_MOC_LITERAL(87, 19),  // "on_minimize_clicked"
        QT_MOC_LITERAL(107, 15),  // "mousePressEvent"
        QT_MOC_LITERAL(123, 12),  // "QMouseEvent*"
        QT_MOC_LITERAL(136, 5),  // "event"
        QT_MOC_LITERAL(142, 14),  // "mouseMoveEvent"
        QT_MOC_LITERAL(157, 13),  // "keyPressEvent"
        QT_MOC_LITERAL(171, 10),  // "QKeyEvent*"
        QT_MOC_LITERAL(182, 17),  // "mouseReleaseEvent"
        QT_MOC_LITERAL(200, 30),  // "on_lineEditUsername_textEdited"
        QT_MOC_LITERAL(231, 4),  // "arg1"
        QT_MOC_LITERAL(236, 31)   // "on_lineEdit_Password_textEdited"
    },
    "loginwindow",
    "loginStatus",
    "",
    "userInfo",
    "userData",
    "on_pushButtonlogin_clicked",
    "on_close_clicked",
    "on_minimize_clicked",
    "mousePressEvent",
    "QMouseEvent*",
    "event",
    "mouseMoveEvent",
    "keyPressEvent",
    "QKeyEvent*",
    "mouseReleaseEvent",
    "on_lineEditUsername_textEdited",
    "arg1",
    "on_lineEdit_Password_textEdited"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSloginwindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   74,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   77,    2, 0x08,    3 /* Private */,
       6,    0,   78,    2, 0x08,    4 /* Private */,
       7,    0,   79,    2, 0x08,    5 /* Private */,
       8,    1,   80,    2, 0x08,    6 /* Private */,
      11,    1,   83,    2, 0x08,    8 /* Private */,
      12,    1,   86,    2, 0x08,   10 /* Private */,
      14,    1,   89,    2, 0x08,   12 /* Private */,
      15,    1,   92,    2, 0x08,   14 /* Private */,
      17,    1,   95,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 13,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,

       0        // eod
};

Q_CONSTINIT const QMetaObject loginwindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSloginwindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSloginwindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSloginwindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<loginwindow, std::true_type>,
        // method 'loginStatus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<userInfo, std::false_type>,
        // method 'on_pushButtonlogin_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_close_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_minimize_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'mousePressEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'mouseMoveEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'keyPressEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QKeyEvent *, std::false_type>,
        // method 'mouseReleaseEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'on_lineEditUsername_textEdited'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_lineEdit_Password_textEdited'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void loginwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<loginwindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loginStatus((*reinterpret_cast< std::add_pointer_t<userInfo>>(_a[1]))); break;
        case 1: _t->on_pushButtonlogin_clicked(); break;
        case 2: _t->on_close_clicked(); break;
        case 3: _t->on_minimize_clicked(); break;
        case 4: _t->mousePressEvent((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 5: _t->mouseMoveEvent((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 6: _t->keyPressEvent((*reinterpret_cast< std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        case 7: _t->mouseReleaseEvent((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 8: _t->on_lineEditUsername_textEdited((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->on_lineEdit_Password_textEdited((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (loginwindow::*)(userInfo );
            if (_t _q_method = &loginwindow::loginStatus; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *loginwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *loginwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSloginwindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int loginwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void loginwindow::loginStatus(userInfo _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
