/****************************************************************************
** Meta object code from reading C++ file 'RNetworkSamplingModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Models/Orchestrating/RNetworkSamplingModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RNetworkSamplingModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RNetworkSamplingModel_t {
    QByteArrayData data[13];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RNetworkSamplingModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RNetworkSamplingModel_t qt_meta_stringdata_RNetworkSamplingModel = {
    {
QT_MOC_LITERAL(0, 0, 21), // "RNetworkSamplingModel"
QT_MOC_LITERAL(1, 22, 18), // "InputParameterList"
QT_MOC_LITERAL(2, 41, 18), // "VariableOfInterest"
QT_MOC_LITERAL(3, 60, 10), // "MaxSamples"
QT_MOC_LITERAL(4, 71, 12), // "PlotInterval"
QT_MOC_LITERAL(5, 84, 14), // "SamplingCentre"
QT_MOC_LITERAL(6, 99, 21), // "RandomNumberGenerator"
QT_MOC_LITERAL(7, 121, 25), // "ProbabilityTransformation"
QT_MOC_LITERAL(8, 147, 26), // "SamplingMultiplierFunction"
QT_MOC_LITERAL(9, 174, 27), // "RSamplingMultiplierFunction"
QT_MOC_LITERAL(10, 202, 8), // "Constant"
QT_MOC_LITERAL(11, 211, 6), // "Linear"
QT_MOC_LITERAL(12, 218, 4) // "None"

    },
    "RNetworkSamplingModel\0InputParameterList\0"
    "VariableOfInterest\0MaxSamples\0"
    "PlotInterval\0SamplingCentre\0"
    "RandomNumberGenerator\0ProbabilityTransformation\0"
    "SamplingMultiplierFunction\0"
    "RSamplingMultiplierFunction\0Constant\0"
    "Linear\0None"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RNetworkSamplingModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       8,   14, // properties
       1,   38, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095103,
       2, QMetaType::QString, 0x00095103,
       3, QMetaType::Int, 0x00095103,
       4, QMetaType::Int, 0x00095103,
       5, QMetaType::QString, 0x00095103,
       6, QMetaType::QObjectStar, 0x00095103,
       7, QMetaType::QObjectStar, 0x00095103,
       8, 0x80000000 | 9, 0x0009510b,

 // enums: name, alias, flags, count, data
       9,    9, 0x0,    3,   43,

 // enum data: key, value
      10, uint(RNetworkSamplingModel::Constant),
      11, uint(RNetworkSamplingModel::Linear),
      12, uint(RNetworkSamplingModel::None),

       0        // eod
};

void RNetworkSamplingModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RNetworkSamplingModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getInputParameterList(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getVariableOfInterest(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->getMaxSamples(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->getPlotInterval(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getSamplingCentre(); break;
        case 5: *reinterpret_cast< QObject**>(_v) = _t->getRandomNumberGenerator(); break;
        case 6: *reinterpret_cast< QObject**>(_v) = _t->getProbabilityTransformation(); break;
        case 7: *reinterpret_cast< RSamplingMultiplierFunction*>(_v) = _t->getSamplingMultiplierFunction(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RNetworkSamplingModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setInputParameterList(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setVariableOfInterest(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setMaxSamples(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setPlotInterval(*reinterpret_cast< int*>(_v)); break;
        case 4: _t->setSamplingCentre(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setRandomNumberGenerator(*reinterpret_cast< QObject**>(_v)); break;
        case 6: _t->setProbabilityTransformation(*reinterpret_cast< QObject**>(_v)); break;
        case 7: _t->setSamplingMultiplierFunction(*reinterpret_cast< RSamplingMultiplierFunction*>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RNetworkSamplingModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RModel::staticMetaObject>(),
    qt_meta_stringdata_RNetworkSamplingModel.data,
    qt_meta_data_RNetworkSamplingModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RNetworkSamplingModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RNetworkSamplingModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RNetworkSamplingModel.stringdata0))
        return static_cast<void*>(this);
    return RModel::qt_metacast(_clname);
}

int RNetworkSamplingModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
