/****************************************************************************
** Meta object code from reading C++ file 'RMainNeighborhoodModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Models/Buildings/RMainNeighborhoodModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RMainNeighborhoodModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RMainNeighborhoodModel_t {
    QByteArrayData data[12];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RMainNeighborhoodModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RMainNeighborhoodModel_t qt_meta_stringdata_RMainNeighborhoodModel = {
    {
QT_MOC_LITERAL(0, 0, 22), // "RMainNeighborhoodModel"
QT_MOC_LITERAL(1, 23, 21), // "PreparednessEstimator"
QT_MOC_LITERAL(2, 45, 21), // "DisplacementEstimator"
QT_MOC_LITERAL(3, 67, 18), // "MorbidityEstimator"
QT_MOC_LITERAL(4, 86, 17), // "PowerSupplierList"
QT_MOC_LITERAL(5, 104, 17), // "WaterSupplierList"
QT_MOC_LITERAL(6, 122, 21), // "BuildingInspectorList"
QT_MOC_LITERAL(7, 144, 14), // "ContractorList"
QT_MOC_LITERAL(8, 159, 19), // "EngineeringFirmList"
QT_MOC_LITERAL(9, 179, 23), // "PermittingAuthorityList"
QT_MOC_LITERAL(10, 203, 20), // "MaterialSupplierList"
QT_MOC_LITERAL(11, 224, 30) // "FinancialResourcesSupplierList"

    },
    "RMainNeighborhoodModel\0PreparednessEstimator\0"
    "DisplacementEstimator\0MorbidityEstimator\0"
    "PowerSupplierList\0WaterSupplierList\0"
    "BuildingInspectorList\0ContractorList\0"
    "EngineeringFirmList\0PermittingAuthorityList\0"
    "MaterialSupplierList\0"
    "FinancialResourcesSupplierList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RMainNeighborhoodModel[] = {

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
       1, QMetaType::QObjectStar, 0x00095103,
       2, QMetaType::QObjectStar, 0x00095103,
       3, QMetaType::QObjectStar, 0x00095103,
       4, QMetaType::QString, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       6, QMetaType::QString, 0x00095103,
       7, QMetaType::QString, 0x00095103,
       8, QMetaType::QString, 0x00095103,
       9, QMetaType::QString, 0x00095103,
      10, QMetaType::QString, 0x00095103,
      11, QMetaType::QString, 0x00095103,

       0        // eod
};

void RMainNeighborhoodModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RMainNeighborhoodModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QObject**>(_v) = _t->getPreparednessEstimator(); break;
        case 1: *reinterpret_cast< QObject**>(_v) = _t->getDisplacementEstimator(); break;
        case 2: *reinterpret_cast< QObject**>(_v) = _t->getMorbidityEstimator(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getPowerSupplierList(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getWaterSupplierList(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getBuildingInspectorList(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getContractorList(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->getEngineeringFirmList(); break;
        case 8: *reinterpret_cast< QString*>(_v) = _t->getPermittingAuthorityList(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->getMaterialSupplierList(); break;
        case 10: *reinterpret_cast< QString*>(_v) = _t->getFinancialResourcesSupplierList(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RMainNeighborhoodModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPreparednessEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 1: _t->setDisplacementEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 2: _t->setMorbidityEstimator(*reinterpret_cast< QObject**>(_v)); break;
        case 3: _t->setPowerSupplierList(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setWaterSupplierList(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setBuildingInspectorList(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setContractorList(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setEngineeringFirmList(*reinterpret_cast< QString*>(_v)); break;
        case 8: _t->setPermittingAuthorityList(*reinterpret_cast< QString*>(_v)); break;
        case 9: _t->setMaterialSupplierList(*reinterpret_cast< QString*>(_v)); break;
        case 10: _t->setFinancialResourcesSupplierList(*reinterpret_cast< QString*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RMainNeighborhoodModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RNeighborhoodModel::staticMetaObject>(),
    qt_meta_stringdata_RMainNeighborhoodModel.data,
    qt_meta_data_RMainNeighborhoodModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RMainNeighborhoodModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RMainNeighborhoodModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RMainNeighborhoodModel.stringdata0))
        return static_cast<void*>(this);
    return RNeighborhoodModel::qt_metacast(_clname);
}

int RMainNeighborhoodModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RNeighborhoodModel::qt_metacall(_c, _id, _a);
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
