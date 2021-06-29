/****************************************************************************
** Meta object code from reading C++ file 'RNeighborhoodModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Models/Buildings/RNeighborhoodModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RNeighborhoodModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RNeighborhoodModel_t {
    QByteArrayData data[7];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RNeighborhoodModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RNeighborhoodModel_t qt_meta_stringdata_RNeighborhoodModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "RNeighborhoodModel"
QT_MOC_LITERAL(1, 19, 18), // "HouseholdPortfolio"
QT_MOC_LITERAL(2, 38, 19), // "BuildingDamageModel"
QT_MOC_LITERAL(3, 58, 21), // "DisplacementEstimator"
QT_MOC_LITERAL(4, 80, 22), // "FinancingTimeEstimator"
QT_MOC_LITERAL(5, 103, 14), // "ContractorList"
QT_MOC_LITERAL(6, 118, 21) // "SamplingStartingPoint"

    },
    "RNeighborhoodModel\0HouseholdPortfolio\0"
    "BuildingDamageModel\0DisplacementEstimator\0"
    "FinancingTimeEstimator\0ContractorList\0"
    "SamplingStartingPoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RNeighborhoodModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QObjectStar, 0x00095103,
       2, QMetaType::QObjectStar, 0x00095103,
       3, QMetaType::QObjectStar, 0x00095103,
       4, QMetaType::QObjectStar, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       6, QMetaType::Int, 0x00095103,

       0        // eod
};

void RNeighborhoodModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RNeighborhoodModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QObject**>(_v) = _t->getHouseholdPortfolio(); break;
        case 1: *reinterpret_cast< QObject**>(_v) = _t->getBuildingDamageModel(); break;
        case 2: *reinterpret_cast< QObject**>(_v) = _t->getDisplacementEstimator(); break;
        case 3: *reinterpret_cast< QObject**>(_v) = _t->getFinancingTimeEstimator(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getContractorList(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->getSamplingStartingPoint(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RNeighborhoodModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHouseholdPortfolio(*reinterpret_cast< QObject**>(_v)); break;
        case 1: _t->setBuildingDamageModel(*reinterpret_cast< QObject**>(_v)); break;
        case 2: _t->setDisplacementEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 3: _t->setFinancingTimeEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 4: _t->setContractorList(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setSamplingStartingPoint(*reinterpret_cast< int*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RNeighborhoodModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RNetworkModel::staticMetaObject>(),
    qt_meta_stringdata_RNeighborhoodModel.data,
    qt_meta_data_RNeighborhoodModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RNeighborhoodModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RNeighborhoodModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RNeighborhoodModel.stringdata0))
        return static_cast<void*>(this);
    return RNetworkModel::qt_metacast(_clname);
}

int RNeighborhoodModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RNetworkModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
