/****************************************************************************
** Meta object code from reading C++ file 'RContinuousRandomVariable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/RContinuousRandomVariable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RContinuousRandomVariable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RContinuousRandomVariable_t {
    QByteArrayData data[18];
    char stringdata0[239];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RContinuousRandomVariable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RContinuousRandomVariable_t qt_meta_stringdata_RContinuousRandomVariable = {
    {
QT_MOC_LITERAL(0, 0, 25), // "RContinuousRandomVariable"
QT_MOC_LITERAL(1, 26, 7), // "plotPDF"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 7), // "plotCDF"
QT_MOC_LITERAL(4, 43, 14), // "plotInverseCDF"
QT_MOC_LITERAL(5, 58, 6), // "getPDF"
QT_MOC_LITERAL(6, 65, 6), // "getCDF"
QT_MOC_LITERAL(7, 72, 13), // "getInverseCDF"
QT_MOC_LITERAL(8, 86, 16), // "DistributionType"
QT_MOC_LITERAL(9, 103, 4), // "Mean"
QT_MOC_LITERAL(10, 108, 17), // "StandardDeviation"
QT_MOC_LITERAL(11, 126, 22), // "CoefficientOfVariation"
QT_MOC_LITERAL(12, 149, 10), // "Parameter1"
QT_MOC_LITERAL(13, 160, 10), // "Parameter2"
QT_MOC_LITERAL(14, 171, 10), // "Parameter3"
QT_MOC_LITERAL(15, 182, 10), // "Parameter4"
QT_MOC_LITERAL(16, 193, 22), // "AlphaImportanceMeasure"
QT_MOC_LITERAL(17, 216, 22) // "GammaImportanceMeasure"

    },
    "RContinuousRandomVariable\0plotPDF\0\0"
    "plotCDF\0plotInverseCDF\0getPDF\0getCDF\0"
    "getInverseCDF\0DistributionType\0Mean\0"
    "StandardDeviation\0CoefficientOfVariation\0"
    "Parameter1\0Parameter2\0Parameter3\0"
    "Parameter4\0AlphaImportanceMeasure\0"
    "GammaImportanceMeasure"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RContinuousRandomVariable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
      10,   50, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       8, QMetaType::QString, 0x00095103,
       9, QMetaType::QString, 0x00095103,
      10, QMetaType::QString, 0x00095103,
      11, QMetaType::QString, 0x00095103,
      12, QMetaType::QString, 0x00095103,
      13, QMetaType::QString, 0x00095103,
      14, QMetaType::QString, 0x00095103,
      15, QMetaType::QString, 0x00095103,
      16, QMetaType::Double, 0x00095001,
      17, QMetaType::Double, 0x00095001,

       0        // eod
};

void RContinuousRandomVariable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RContinuousRandomVariable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->plotPDF(); break;
        case 1: _t->plotCDF(); break;
        case 2: _t->plotInverseCDF(); break;
        case 3: _t->getPDF(); break;
        case 4: _t->getCDF(); break;
        case 5: _t->getInverseCDF(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RContinuousRandomVariable *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->getDistributionType(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->getMean(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getStandardDeviation(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->getCoefficientOfVariation(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->getParameter1(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->getParameter2(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->getParameter3(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->getParameter4(); break;
        case 8: *reinterpret_cast< double*>(_v) = _t->getAlphaImportanceMeasure(); break;
        case 9: *reinterpret_cast< double*>(_v) = _t->getGammaImportanceMeasure(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RContinuousRandomVariable *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDistributionType(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setMean(*reinterpret_cast< QString*>(_v)); break;
        case 2: _t->setStandardDeviation(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setCoefficientOfVariation(*reinterpret_cast< QString*>(_v)); break;
        case 4: _t->setParameter1(*reinterpret_cast< QString*>(_v)); break;
        case 5: _t->setParameter2(*reinterpret_cast< QString*>(_v)); break;
        case 6: _t->setParameter3(*reinterpret_cast< QString*>(_v)); break;
        case 7: _t->setParameter4(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RContinuousRandomVariable::staticMetaObject = { {
    QMetaObject::SuperData::link<RRandomVariable::staticMetaObject>(),
    qt_meta_stringdata_RContinuousRandomVariable.data,
    qt_meta_data_RContinuousRandomVariable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RContinuousRandomVariable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RContinuousRandomVariable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RContinuousRandomVariable.stringdata0))
        return static_cast<void*>(this);
    return RRandomVariable::qt_metacast(_clname);
}

int RContinuousRandomVariable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RRandomVariable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 10;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
