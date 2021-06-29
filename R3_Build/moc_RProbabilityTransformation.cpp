/****************************************************************************
** Meta object code from reading C++ file 'RProbabilityTransformation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/RProbabilityTransformation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RProbabilityTransformation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RProbabilityTransformation_t {
    QByteArrayData data[4];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RProbabilityTransformation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RProbabilityTransformation_t qt_meta_stringdata_RProbabilityTransformation = {
    {
QT_MOC_LITERAL(0, 0, 26), // "RProbabilityTransformation"
QT_MOC_LITERAL(1, 27, 13), // "RJacobianType"
QT_MOC_LITERAL(2, 41, 3), // "u_x"
QT_MOC_LITERAL(3, 45, 3) // "x_u"

    },
    "RProbabilityTransformation\0RJacobianType\0"
    "u_x\0x_u"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RProbabilityTransformation[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    2,   19,

 // enum data: key, value
       2, uint(RProbabilityTransformation::u_x),
       3, uint(RProbabilityTransformation::x_u),

       0        // eod
};

void RProbabilityTransformation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RProbabilityTransformation::staticMetaObject = { {
    QMetaObject::SuperData::link<RMethod::staticMetaObject>(),
    qt_meta_stringdata_RProbabilityTransformation.data,
    qt_meta_data_RProbabilityTransformation,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RProbabilityTransformation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RProbabilityTransformation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RProbabilityTransformation.stringdata0))
        return static_cast<void*>(this);
    return RMethod::qt_metacast(_clname);
}

int RProbabilityTransformation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RMethod::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
