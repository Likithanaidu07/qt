/****************************************************************************
** Meta object code from reading C++ file 'watch_cards.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Cards/watch_cards.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'watch_cards.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSWatch_cardsENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSWatch_cardsENDCLASS = QtMocHelpers::stringData(
    "Watch_cards",
    "add_remove_watch_card_signal",
    "",
    "Indices_Data_Struct",
    "indicesDataRecv_Slot",
    "data",
    "on_lineEditWatchSearch_textChanged",
    "arg1",
    "on_listWidgetWatch_itemClicked",
    "QListWidgetItem*",
    "item",
    "on_listWidgetWatch_itemDoubleClicked",
    "on_listWidgetWatch_itemSelectionChanged",
    "keyPressEvent",
    "QKeyEvent*",
    "event"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWatch_cardsENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[12];
    char stringdata1[29];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[21];
    char stringdata5[5];
    char stringdata6[35];
    char stringdata7[5];
    char stringdata8[31];
    char stringdata9[17];
    char stringdata10[5];
    char stringdata11[37];
    char stringdata12[40];
    char stringdata13[14];
    char stringdata14[11];
    char stringdata15[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWatch_cardsENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWatch_cardsENDCLASS_t qt_meta_stringdata_CLASSWatch_cardsENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "Watch_cards"
        QT_MOC_LITERAL(12, 28),  // "add_remove_watch_card_signal"
        QT_MOC_LITERAL(41, 0),  // ""
        QT_MOC_LITERAL(42, 19),  // "Indices_Data_Struct"
        QT_MOC_LITERAL(62, 20),  // "indicesDataRecv_Slot"
        QT_MOC_LITERAL(83, 4),  // "data"
        QT_MOC_LITERAL(88, 34),  // "on_lineEditWatchSearch_textCh..."
        QT_MOC_LITERAL(123, 4),  // "arg1"
        QT_MOC_LITERAL(128, 30),  // "on_listWidgetWatch_itemClicked"
        QT_MOC_LITERAL(159, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(176, 4),  // "item"
        QT_MOC_LITERAL(181, 36),  // "on_listWidgetWatch_itemDouble..."
        QT_MOC_LITERAL(218, 39),  // "on_listWidgetWatch_itemSelect..."
        QT_MOC_LITERAL(258, 13),  // "keyPressEvent"
        QT_MOC_LITERAL(272, 10),  // "QKeyEvent*"
        QT_MOC_LITERAL(283, 5)   // "event"
    },
    "Watch_cards",
    "add_remove_watch_card_signal",
    "",
    "Indices_Data_Struct",
    "indicesDataRecv_Slot",
    "data",
    "on_lineEditWatchSearch_textChanged",
    "arg1",
    "on_listWidgetWatch_itemClicked",
    "QListWidgetItem*",
    "item",
    "on_listWidgetWatch_itemDoubleClicked",
    "on_listWidgetWatch_itemSelectionChanged",
    "keyPressEvent",
    "QKeyEvent*",
    "event"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWatch_cardsENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   61,    2, 0x0a,    4 /* Public */,
       6,    1,   64,    2, 0x08,    6 /* Private */,
       8,    1,   67,    2, 0x08,    8 /* Private */,
      11,    1,   70,    2, 0x08,   10 /* Private */,
      12,    0,   73,    2, 0x08,   12 /* Private */,
      13,    1,   74,    2, 0x08,   13 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 3,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject Watch_cards::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSWatch_cardsENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWatch_cardsENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWatch_cardsENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Watch_cards, std::true_type>,
        // method 'add_remove_watch_card_signal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<Indices_Data_Struct, std::false_type>,
        // method 'indicesDataRecv_Slot'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Indices_Data_Struct, std::false_type>,
        // method 'on_lineEditWatchSearch_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_listWidgetWatch_itemClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_listWidgetWatch_itemDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'on_listWidgetWatch_itemSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'keyPressEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QKeyEvent *, std::false_type>
    >,
    nullptr
} };

void Watch_cards::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Watch_cards *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->add_remove_watch_card_signal((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Indices_Data_Struct>>(_a[2]))); break;
        case 1: _t->indicesDataRecv_Slot((*reinterpret_cast< std::add_pointer_t<Indices_Data_Struct>>(_a[1]))); break;
        case 2: _t->on_lineEditWatchSearch_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->on_listWidgetWatch_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 4: _t->on_listWidgetWatch_itemDoubleClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 5: _t->on_listWidgetWatch_itemSelectionChanged(); break;
        case 6: _t->keyPressEvent((*reinterpret_cast< std::add_pointer_t<QKeyEvent*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Watch_cards::*)(bool , Indices_Data_Struct );
            if (_t _q_method = &Watch_cards::add_remove_watch_card_signal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Watch_cards::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Watch_cards::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWatch_cardsENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Watch_cards::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void Watch_cards::add_remove_watch_card_signal(bool _t1, Indices_Data_Struct _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
