/****************************************************************************
** Meta object code from reading C++ file 'RObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/RObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RObject_t {
    QByteArrayData data[24];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RObject_t qt_meta_stringdata_RObject = {
    {
QT_MOC_LITERAL(0, 0, 7), // "RObject"
QT_MOC_LITERAL(1, 8, 4), // "help"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 16), // "ROutputLevelType"
QT_MOC_LITERAL(4, 31, 7), // "Minimum"
QT_MOC_LITERAL(5, 39, 6), // "Medium"
QT_MOC_LITERAL(6, 46, 7), // "Maximum"
QT_MOC_LITERAL(7, 54, 14), // "RParameterType"
QT_MOC_LITERAL(8, 69, 14), // "RandomVariable"
QT_MOC_LITERAL(9, 84, 16), // "DecisionVariable"
QT_MOC_LITERAL(10, 101, 8), // "Constant"
QT_MOC_LITERAL(11, 110, 8), // "Response"
QT_MOC_LITERAL(12, 119, 3), // "ALL"
QT_MOC_LITERAL(13, 123, 10), // "RPointType"
QT_MOC_LITERAL(14, 134, 6), // "Origin"
QT_MOC_LITERAL(15, 141, 13), // "CurrentValues"
QT_MOC_LITERAL(16, 155, 13), // "RGradientType"
QT_MOC_LITERAL(17, 169, 4), // "NONE"
QT_MOC_LITERAL(18, 174, 3), // "FDM"
QT_MOC_LITERAL(19, 178, 4), // "F2DM"
QT_MOC_LITERAL(20, 183, 4), // "F3DM"
QT_MOC_LITERAL(21, 188, 3), // "DDM"
QT_MOC_LITERAL(22, 192, 4), // "D2DM"
QT_MOC_LITERAL(23, 197, 4) // "D3DM"

    },
    "RObject\0help\0\0ROutputLevelType\0Minimum\0"
    "Medium\0Maximum\0RParameterType\0"
    "RandomVariable\0DecisionVariable\0"
    "Constant\0Response\0ALL\0RPointType\0"
    "Origin\0CurrentValues\0RGradientType\0"
    "NONE\0FDM\0F2DM\0F3DM\0DDM\0D2DM\0D3DM"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RObject[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       4,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       3,    3, 0x0,    3,   40,
       7,    7, 0x0,    5,   46,
      13,   13, 0x0,    2,   56,
      16,   16, 0x0,    7,   60,

 // enum data: key, value
       4, uint(RObject::Minimum),
       5, uint(RObject::Medium),
       6, uint(RObject::Maximum),
       8, uint(RObject::RandomVariable),
       9, uint(RObject::DecisionVariable),
      10, uint(RObject::Constant),
      11, uint(RObject::Response),
      12, uint(RObject::ALL),
      14, uint(RObject::Origin),
      15, uint(RObject::CurrentValues),
      17, uint(RObject::NONE),
      18, uint(RObject::FDM),
      19, uint(RObject::F2DM),
      20, uint(RObject::F3DM),
      21, uint(RObject::DDM),
      22, uint(RObject::D2DM),
      23, uint(RObject::D3DM),

       0        // eod
};

void RObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->help(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RObject::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RObject.data,
    qt_meta_data_RObject,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RObject.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
