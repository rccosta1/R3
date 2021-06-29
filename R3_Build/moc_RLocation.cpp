/****************************************************************************
** Meta object code from reading C++ file 'RLocation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/RLocation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RLocation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RLocation_t {
    QByteArrayData data[8];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RLocation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RLocation_t qt_meta_stringdata_RLocation = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RLocation"
QT_MOC_LITERAL(1, 10, 8), // "Latitude"
QT_MOC_LITERAL(2, 19, 9), // "Longitude"
QT_MOC_LITERAL(3, 29, 8), // "Altitude"
QT_MOC_LITERAL(4, 38, 12), // "PGAIntensity"
QT_MOC_LITERAL(5, 51, 12), // "Sa1Intensity"
QT_MOC_LITERAL(6, 64, 14), // "Sa0_3Intensity"
QT_MOC_LITERAL(7, 79, 17) // "ShearWaveVelocity"

    },
    "RLocation\0Latitude\0Longitude\0Altitude\0"
    "PGAIntensity\0Sa1Intensity\0Sa0_3Intensity\0"
    "ShearWaveVelocity"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RLocation[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       7,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::Double, 0x00095103,
       4, QMetaType::QString, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,

       0        // eod
};

void RLocation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RLocation *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->getLatitude(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->getLongitude(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->getAltitude(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getPGAIntensity(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getSa1Intensity(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getSa0_3Intensity(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getShearWaveVelocity(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RLocation *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setLatitude(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setLongitude(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setAltitude(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setPGAIntensity(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setSa1Intensity(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setSa0_3Intensity(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setShearWaveVelocity(*reinterpret_cast< QString*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RLocation::staticMetaObject = { {
    QMetaObject::SuperData::link<RParameter::staticMetaObject>(),
    qt_meta_stringdata_RLocation.data,
    qt_meta_data_RLocation,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RLocation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RLocation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RLocation.stringdata0))
        return static_cast<void*>(this);
    return RParameter::qt_metacast(_clname);
}

int RLocation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RParameter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
