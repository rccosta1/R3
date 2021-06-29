/****************************************************************************
** Meta object code from reading C++ file 'RBuildingPortfolio.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Repositories/RBuildingPortfolio.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RBuildingPortfolio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RBuildingPortfolio_t {
    QByteArrayData data[12];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RBuildingPortfolio_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RBuildingPortfolio_t qt_meta_stringdata_RBuildingPortfolio = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RBuildingPortfolio"
QT_MOC_LITERAL(1, 19, 12), // "Municipality"
QT_MOC_LITERAL(2, 32, 21), // "MedianHouseholdIncome"
QT_MOC_LITERAL(3, 54, 13), // "BuildingTypes"
QT_MOC_LITERAL(4, 68, 24), // "BuildingOccupancyClasses"
QT_MOC_LITERAL(5, 93, 21), // "BuildingNumberPerType"
QT_MOC_LITERAL(6, 115, 17), // "BuildingCodeLevel"
QT_MOC_LITERAL(7, 133, 13), // "BuildingAreas"
QT_MOC_LITERAL(8, 147, 14), // "BuildingValues"
QT_MOC_LITERAL(9, 162, 15), // "BuildingDensity"
QT_MOC_LITERAL(10, 178, 13), // "RetrofitLevel"
QT_MOC_LITERAL(11, 192, 17) // "RoomsDistribution"

    },
    "RBuildingPortfolio\0Municipality\0"
    "MedianHouseholdIncome\0BuildingTypes\0"
    "BuildingOccupancyClasses\0BuildingNumberPerType\0"
    "BuildingCodeLevel\0BuildingAreas\0"
    "BuildingValues\0BuildingDensity\0"
    "RetrofitLevel\0RoomsDistribution"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RBuildingPortfolio[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      11,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::QString, 0x00095103,
       4, QMetaType::QString, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::Double, 0x00095103,
       9, QMetaType::QString, 0x00095103,
      10, QMetaType::Double, 0x00095103,
      11, QMetaType::QString, 0x00095103,

       0        // eod
};

void RBuildingPortfolio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RBuildingPortfolio *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getMunicipality(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->getMedianHouseholdIncome(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getBuildingTypes(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getBuildingOccupancyClasses(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getBuildingNumberPerType(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getBuildingCodeLevel(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getBuildingAreas(); break;
        case 7: *reinterpret_cast< double*>(_v) = _t->getBuildingValues(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->getBuildingDensity(); break;
        case 9: *reinterpret_cast< double*>(_v) = _t->getRetrofitLevel(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->getRoomsDistribution(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RBuildingPortfolio *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMunicipality(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setMedianHouseholdIncome(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setBuildingTypes(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setBuildingOccupancyClasses(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setBuildingNumberPerType(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setBuildingCodeLevel(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setBuildingAreas(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setBuildingValues(*reinterpret_cast< double*>(_v)); break;
        case 8: _t->setBuildingDensity(*reinterpret_cast< QString*>(_v)); break;
        case 9: _t->setRetrofitLevel(*reinterpret_cast< double*>(_v)); break;
        case 10: _t->setRoomsDistribution(*reinterpret_cast< QString*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RBuildingPortfolio::staticMetaObject = { {
    QMetaObject::SuperData::link<RObject::staticMetaObject>(),
    qt_meta_stringdata_RBuildingPortfolio.data,
    qt_meta_data_RBuildingPortfolio,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RBuildingPortfolio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RBuildingPortfolio::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RBuildingPortfolio.stringdata0))
        return static_cast<void*>(this);
    return RObject::qt_metacast(_clname);
}

int RBuildingPortfolio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
