/****************************************************************************
** Meta object code from reading C++ file 'RIntensityModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Hazards/IntensityModels/RIntensityModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RIntensityModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RIntensityModel_t {
    QByteArrayData data[6];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RIntensityModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RIntensityModel_t qt_meta_stringdata_RIntensityModel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RIntensityModel"
QT_MOC_LITERAL(1, 16, 13), // "RResponseType"
QT_MOC_LITERAL(2, 30, 3), // "PGA"
QT_MOC_LITERAL(3, 34, 3), // "PGD"
QT_MOC_LITERAL(4, 38, 3), // "PGV"
QT_MOC_LITERAL(5, 42, 2) // "Sa"

    },
    "RIntensityModel\0RResponseType\0PGA\0PGD\0"
    "PGV\0Sa"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RIntensityModel[] = {

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
       1,    1, 0x0,    4,   19,

 // enum data: key, value
       2, uint(RIntensityModel::PGA),
       3, uint(RIntensityModel::PGD),
       4, uint(RIntensityModel::PGV),
       5, uint(RIntensityModel::Sa),

       0        // eod
};

void RIntensityModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RIntensityModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RModel::staticMetaObject>(),
    qt_meta_stringdata_RIntensityModel.data,
    qt_meta_data_RIntensityModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RIntensityModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RIntensityModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RIntensityModel.stringdata0))
        return static_cast<void*>(this);
    return RModel::qt_metacast(_clname);
}

int RIntensityModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
