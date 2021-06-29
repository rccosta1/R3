/****************************************************************************
** Meta object code from reading C++ file 'RObjectWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/RObjectWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RObjectWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RObjectWidget_t {
    QByteArrayData data[25];
    char stringdata0[337];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RObjectWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RObjectWidget_t qt_meta_stringdata_RObjectWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RObjectWidget"
QT_MOC_LITERAL(1, 14, 20), // "currentObjectChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 8), // "QString&"
QT_MOC_LITERAL(4, 45, 4), // "name"
QT_MOC_LITERAL(5, 50, 10), // "updateView"
QT_MOC_LITERAL(6, 61, 10), // "selectItem"
QT_MOC_LITERAL(7, 72, 8), // "itemName"
QT_MOC_LITERAL(8, 81, 9), // "addObject"
QT_MOC_LITERAL(9, 91, 15), // "removeAllObject"
QT_MOC_LITERAL(10, 107, 12), // "removeObject"
QT_MOC_LITERAL(11, 120, 13), // "changeCurrent"
QT_MOC_LITERAL(12, 134, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(13, 151, 7), // "current"
QT_MOC_LITERAL(14, 159, 8), // "previous"
QT_MOC_LITERAL(15, 168, 14), // "currentClicked"
QT_MOC_LITERAL(16, 183, 4), // "item"
QT_MOC_LITERAL(17, 188, 9), // "showPopup"
QT_MOC_LITERAL(18, 198, 8), // "position"
QT_MOC_LITERAL(19, 207, 9), // "runAction"
QT_MOC_LITERAL(20, 217, 25), // "rankRandomVariablesByName"
QT_MOC_LITERAL(21, 243, 25), // "getCoordinatesfromAddress"
QT_MOC_LITERAL(22, 269, 21), // "viewAllLocationsOnMap"
QT_MOC_LITERAL(23, 291, 25), // "removeAllLocationsFromMap"
QT_MOC_LITERAL(24, 317, 19) // "viewAllSectorsOnMap"

    },
    "RObjectWidget\0currentObjectChanged\0\0"
    "QString&\0name\0updateView\0selectItem\0"
    "itemName\0addObject\0removeAllObject\0"
    "removeObject\0changeCurrent\0QTreeWidgetItem*\0"
    "current\0previous\0currentClicked\0item\0"
    "showPopup\0position\0runAction\0"
    "rankRandomVariablesByName\0"
    "getCoordinatesfromAddress\0"
    "viewAllLocationsOnMap\0removeAllLocationsFromMap\0"
    "viewAllSectorsOnMap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RObjectWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   92,    2, 0x0a /* Public */,
       6,    1,   93,    2, 0x0a /* Public */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    0,   98,    2, 0x08 /* Private */,
      11,    2,   99,    2, 0x08 /* Private */,
      15,    1,  104,    2, 0x08 /* Private */,
      17,    1,  107,    2, 0x08 /* Private */,
      19,    0,  110,    2, 0x08 /* Private */,
      20,    0,  111,    2, 0x08 /* Private */,
      21,    0,  112,    2, 0x08 /* Private */,
      22,    0,  113,    2, 0x08 /* Private */,
      23,    0,  114,    2, 0x08 /* Private */,
      24,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 12,   13,   14,
    QMetaType::Void, 0x80000000 | 12,   16,
    QMetaType::Void, QMetaType::QPoint,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RObjectWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RObjectWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->currentObjectChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->updateView(); break;
        case 2: _t->selectItem((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->addObject(); break;
        case 4: _t->removeAllObject(); break;
        case 5: _t->removeObject(); break;
        case 6: _t->changeCurrent((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 7: _t->currentClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->showPopup((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: _t->runAction(); break;
        case 10: _t->rankRandomVariablesByName(); break;
        case 11: _t->getCoordinatesfromAddress(); break;
        case 12: _t->viewAllLocationsOnMap(); break;
        case 13: _t->removeAllLocationsFromMap(); break;
        case 14: _t->viewAllSectorsOnMap(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RObjectWidget::*)(QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RObjectWidget::currentObjectChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RObjectWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_RObjectWidget.data,
    qt_meta_data_RObjectWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RObjectWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RObjectWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RObjectWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::RObjectWidgetUi"))
        return static_cast< Ui::RObjectWidgetUi*>(this);
    return QWidget::qt_metacast(_clname);
}

int RObjectWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void RObjectWidget::currentObjectChanged(QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
