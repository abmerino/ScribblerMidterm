/****************************************************************************
** Meta object code from reading C++ file 'scribbler.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../scribbler.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scribbler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.1. It"
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
struct qt_meta_tag_ZN9ScribblerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN9ScribblerE = QtMocHelpers::stringData(
    "Scribbler",
    "captureEnded",
    "",
<<<<<<< HEAD
    "QList<MouseEvent>",
=======
    "QList<MouseEvent>&",
>>>>>>> backupBranch
    "events",
    "reset",
    "onSaveTriggered",
    "onLoadTriggered"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN9ScribblerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   41,    2, 0x0a,    3 /* Public */,
       6,    0,   42,    2, 0x0a,    4 /* Public */,
       7,    0,   43,    2, 0x0a,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Scribbler::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_ZN9ScribblerE.offsetsAndSizes,
    qt_meta_data_ZN9ScribblerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN9ScribblerE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Scribbler, std::true_type>,
        // method 'captureEnded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
<<<<<<< HEAD
        QtPrivate::TypeAndForceComplete<const QList<MouseEvent> &, std::false_type>,
=======
        QtPrivate::TypeAndForceComplete<QList<MouseEvent> &, std::false_type>,
>>>>>>> backupBranch
        // method 'reset'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSaveTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onLoadTriggered'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Scribbler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Scribbler *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
<<<<<<< HEAD
        case 0: _t->captureEnded((*reinterpret_cast< std::add_pointer_t<QList<MouseEvent>>>(_a[1]))); break;
=======
        case 0: _t->captureEnded((*reinterpret_cast< std::add_pointer_t<QList<MouseEvent>&>>(_a[1]))); break;
>>>>>>> backupBranch
        case 1: _t->reset(); break;
        case 2: _t->onSaveTriggered(); break;
        case 3: _t->onLoadTriggered(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
<<<<<<< HEAD
            using _q_method_type = void (Scribbler::*)(const QList<MouseEvent> & );
=======
            using _q_method_type = void (Scribbler::*)(QList<MouseEvent> & );
>>>>>>> backupBranch
            if (_q_method_type _q_method = &Scribbler::captureEnded; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *Scribbler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Scribbler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN9ScribblerE.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int Scribbler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
<<<<<<< HEAD
void Scribbler::captureEnded(const QList<MouseEvent> & _t1)
=======
void Scribbler::captureEnded(QList<MouseEvent> & _t1)
>>>>>>> backupBranch
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
