/****************************************************************************
** Meta object code from reading C++ file 'RWorkforceStudyNeighborhoodModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Models/Buildings/RWorkforceStudyNeighborhoodModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RWorkforceStudyNeighborhoodModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RWorkforceStudyNeighborhoodModel_t {
    QByteArrayData data[10];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RWorkforceStudyNeighborhoodModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RWorkforceStudyNeighborhoodModel_t qt_meta_stringdata_RWorkforceStudyNeighborhoodModel = {
    {
QT_MOC_LITERAL(0, 0, 32), // "RWorkforceStudyNeighborhoodModel"
QT_MOC_LITERAL(1, 33, 18), // "HouseholdPortfolio"
QT_MOC_LITERAL(2, 52, 19), // "BuildingDamageModel"
QT_MOC_LITERAL(3, 72, 21), // "PreparednessEstimator"
QT_MOC_LITERAL(4, 94, 21), // "DisplacementEstimator"
QT_MOC_LITERAL(5, 116, 18), // "MorbidityEstimator"
QT_MOC_LITERAL(6, 135, 14), // "ContractorList"
QT_MOC_LITERAL(7, 150, 30), // "FinancialResourcesSupplierList"
QT_MOC_LITERAL(8, 181, 25), // "ProbabilityOfLiquefaction"
QT_MOC_LITERAL(9, 207, 21) // "SamplingStartingPoint"

    },
    "RWorkforceStudyNeighborhoodModel\0"
    "HouseholdPortfolio\0BuildingDamageModel\0"
    "PreparednessEstimator\0DisplacementEstimator\0"
    "MorbidityEstimator\0ContractorList\0"
    "FinancialResourcesSupplierList\0"
    "ProbabilityOfLiquefaction\0"
    "SamplingStartingPoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RWorkforceStudyNeighborhoodModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       9,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QObjectStar, 0x00095103,
       2, QMetaType::QObjectStar, 0x00095103,
       3, QMetaType::QObjectStar, 0x00095103,
       4, QMetaType::QObjectStar, 0x00095103,
       5, QMetaType::QObjectStar, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::Double, 0x00095103,
       9, QMetaType::Int, 0x00095103,

       0        // eod
};

void RWorkforceStudyNeighborhoodModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RWorkforceStudyNeighborhoodModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QObject**>(_v) = _t->getHouseholdPortfolio(); break;
        case 1: *reinterpret_cast< QObject**>(_v) = _t->getBuildingDamageModel(); break;
        case 2: *reinterpret_cast< QObject**>(_v) = _t->getPreparednessEstimator(); break;
        case 3: *reinterpret_cast< QObject**>(_v) = _t->getDisplacementEstimator(); break;
        case 4: *reinterpret_cast< QObject**>(_v) = _t->getMorbidityEstimator(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getContractorList(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getFinancialResourcesSupplierList(); break;
        case 7: *reinterpret_cast< double*>(_v) = _t->getProbabilityOfLiquefaction(); break;
        case 8: *reinterpret_cast< int*>(_v) = _t->getSamplingStartingPoint(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RWorkforceStudyNeighborhoodModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHouseholdPortfolio(*reinterpret_cast< QObject**>(_v)); break;
        case 1: _t->setBuildingDamageModel(*reinterpret_cast< QObject**>(_v)); break;
        case 2: _t->setPreparednessEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 3: _t->setDisplacementEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 4: _t->setMorbidityEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 5: _t->setContractorList(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setFinancialResourcesSupplierList(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setProbabilityOfLiquefaction(*reinterpret_cast< double*>(_v)); break;
        case 8: _t->setSamplingStartingPoint(*reinterpret_cast< int*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RWorkforceStudyNeighborhoodModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RNeighborhoodModel::staticMetaObject>(),
    qt_meta_stringdata_RWorkforceStudyNeighborhoodModel.data,
    qt_meta_data_RWorkforceStudyNeighborhoodModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RWorkforceStudyNeighborhoodModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RWorkforceStudyNeighborhoodModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RWorkforceStudyNeighborhoodModel.stringdata0))
        return static_cast<void*>(this);
    return RNeighborhoodModel::qt_metacast(_clname);
}

int RWorkforceStudyNeighborhoodModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RNeighborhoodModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 9;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 9;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
