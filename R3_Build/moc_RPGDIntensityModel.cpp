/****************************************************************************
** Meta object code from reading C++ file 'RPGDIntensityModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Hazards/IntensityModels/RPGDIntensityModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RPGDIntensityModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RPGDIntensityModel_t {
    QByteArrayData data[4];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RPGDIntensityModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RPGDIntensityModel_t qt_meta_stringdata_RPGDIntensityModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RPGDIntensityModel"
QT_MOC_LITERAL(1, 19, 13), // "MagnitudeList"
QT_MOC_LITERAL(2, 33, 14), // "HypocentreList"
QT_MOC_LITERAL(3, 48, 22) // "PeakGroundAcceleration"

    },
    "RPGDIntensityModel\0MagnitudeList\0"
    "HypocentreList\0PeakGroundAcceleration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RPGDIntensityModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       3,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::QObjectStar, 0x00095103,

       0        // eod
};

void RPGDIntensityModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RPGDIntensityModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getMagnitudeList(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getHypocentreList(); break;
        case 2: *reinterpret_cast< QObject**>(_v) = _t->getPeakGroundAcceleration(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RPGDIntensityModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMagnitudeList(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setHypocentreList(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setPeakGroundAcceleration(*reinterpret_cast< QObject**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RPGDIntensityModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RIntensityModel::staticMetaObject>(),
    qt_meta_stringdata_RPGDIntensityModel.data,
    qt_meta_data_RPGDIntensityModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RPGDIntensityModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RPGDIntensityModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RPGDIntensityModel.stringdata0))
        return static_cast<void*>(this);
    return RIntensityModel::qt_metacast(_clname);
}

int RPGDIntensityModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RIntensityModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
