/****************************************************************************
** Meta object code from reading C++ file 'RInspectorModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Networks/Models/Businesses/RInspectorModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RInspectorModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RInspectorModel_t {
    QByteArrayData data[5];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RInspectorModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RInspectorModel_t qt_meta_stringdata_RInspectorModel = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RInspectorModel"
QT_MOC_LITERAL(1, 16, 26), // "BaselineNumberOfInspectors"
QT_MOC_LITERAL(2, 43, 22), // "NumberOfDailyWorkHours"
QT_MOC_LITERAL(3, 66, 18), // "MeanInspectionTime"
QT_MOC_LITERAL(4, 85, 17) // "EpicenterLocation"

    },
    "RInspectorModel\0BaselineNumberOfInspectors\0"
    "NumberOfDailyWorkHours\0MeanInspectionTime\0"
    "EpicenterLocation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RInspectorModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Int, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::Double, 0x00095103,
       4, QMetaType::QObjectStar, 0x00095103,

       0        // eod
};

void RInspectorModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RInspectorModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< int*>(_v) = _t->getBaselineNumberOfInspectors(); break;
        case 1: *reinterpret_cast< double*>(_v) = _t->getNumberOfDailyWorkHours(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->getMeanInspectionTime(); break;
        case 3: *reinterpret_cast< QObject**>(_v) = _t->getEpicenterLocation(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RInspectorModel *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBaselineNumberOfInspectors(*reinterpret_cast< int*>(_v)); break;
        case 1: _t->setNumberOfDailyWorkHours(*reinterpret_cast< double*>(_v)); break;
        case 2: _t->setMeanInspectionTime(*reinterpret_cast< double*>(_v)); break;
        case 3: _t->setEpicenterLocation(*reinterpret_cast< QObject**>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RInspectorModel::staticMetaObject = { {
    QMetaObject::SuperData::link<RNetworkModel::staticMetaObject>(),
    qt_meta_stringdata_RInspectorModel.data,
    qt_meta_data_RInspectorModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RInspectorModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RInspectorModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RInspectorModel.stringdata0))
        return static_cast<void*>(this);
    return RNetworkModel::qt_metacast(_clname);
}

int RInspectorModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RNetworkModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
