/****************************************************************************
** Meta object code from reading C++ file 'RPublicMoneyLenderModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Models/Businesses/RPublicMoneyLenderModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RPublicMoneyLenderModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RPublicMoneyLenderModel_t {
    QByteArrayData data[7];
    char stringdata0[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RPublicMoneyLenderModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RPublicMoneyLenderModel_t qt_meta_stringdata_RPublicMoneyLenderModel = {
    {
QT_MOC_LITERAL(0, 0, 23), // "RPublicMoneyLenderModel"
QT_MOC_LITERAL(1, 24, 22), // "MedianDelayForPayments"
QT_MOC_LITERAL(2, 47, 28), // "DispersionOfDelayForPayments"
QT_MOC_LITERAL(3, 76, 31), // "DelayMultiplierForSmallRequests"
QT_MOC_LITERAL(4, 108, 19), // "SmallClaimThreshold"
QT_MOC_LITERAL(5, 128, 30), // "DelayForOwnerOccupiedBuildings"
QT_MOC_LITERAL(6, 159, 31) // "DelayForRenterOccupiedBuildings"

    },
    "RPublicMoneyLenderModel\0MedianDelayForPayments\0"
    "DispersionOfDelayForPayments\0"
    "DelayMultiplierForSmallRequests\0"
    "SmallClaimThreshold\0DelayForOwnerOccupiedBuildings\0"
    "DelayForRenterOccupiedBuildings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RPublicMoneyLenderModel[] = {

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
       1, QMetaType::Double, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::Double, 0x00095103,
       4, QMetaType::Double, 0x00095103,
       5, QMetaType::Double, 0x00095103,
       6, QMetaType::Double, 0x00095103,

       0        // eod
};

void RPublicMoneyLenderModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RPublicMoneyLenderModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->getMedianDelayForPayments(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->getDispersionOfDelayForPayments(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->getDelayMultiplierForSmallRequests(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->getSmallClaimThreshold(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->getDelayForOwnerOccupiedBuildings(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->getDelayForRenterOccupiedBuildings(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RPublicMoneyLenderModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMedianDelayForPayments(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setDispersionOfDelayForPayments(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setDelayMultiplierForSmallRequests(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setSmallClaimThreshold(*reinterpret_cast< double*>(_v)); break;
        case 4: _t->setDelayForOwnerOccupiedBuildings(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setDelayForRenterOccupiedBuildings(*reinterpret_cast< double*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RPublicMoneyLenderModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RNetworkModel::staticMetaObject>(),
    qt_meta_stringdata_RPublicMoneyLenderModel.data,
    qt_meta_data_RPublicMoneyLenderModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RPublicMoneyLenderModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RPublicMoneyLenderModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RPublicMoneyLenderModel.stringdata0))
        return static_cast<void*>(this);
    return RNetworkModel::qt_metacast(_clname);
}

int RPublicMoneyLenderModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
