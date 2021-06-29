/****************************************************************************
** Meta object code from reading C++ file 'RResponse.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/RResponse.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RResponse.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RResponse_t {
    QByteArrayData data[11];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RResponse_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RResponse_t qt_meta_stringdata_RResponse = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RResponse"
QT_MOC_LITERAL(1, 10, 12), // "CurrentValue"
QT_MOC_LITERAL(2, 23, 5), // "Model"
QT_MOC_LITERAL(3, 29, 22), // "TotalStandardDeviation"
QT_MOC_LITERAL(4, 52, 40), // "BetaSensitivityWrtTotalStanda..."
QT_MOC_LITERAL(5, 93, 31), // "TotalEpistemicStandardDeviation"
QT_MOC_LITERAL(6, 125, 49), // "BetaSensitivityWrtTotalEpiste..."
QT_MOC_LITERAL(7, 175, 22), // "ModelStandardDeviation"
QT_MOC_LITERAL(8, 198, 40), // "BetaSensitivityWrtModelStanda..."
QT_MOC_LITERAL(9, 239, 31), // "ModelEpistemicStandardDeviation"
QT_MOC_LITERAL(10, 271, 49) // "BetaSensitivityWrtModelEpiste..."

    },
    "RResponse\0CurrentValue\0Model\0"
    "TotalStandardDeviation\0"
    "BetaSensitivityWrtTotalStandardDeviation\0"
    "TotalEpistemicStandardDeviation\0"
    "BetaSensitivityWrtTotalEpistemicStandardDeviation\0"
    "ModelStandardDeviation\0"
    "BetaSensitivityWrtModelStandardDeviation\0"
    "ModelEpistemicStandardDeviation\0"
    "BetaSensitivityWrtModelEpistemicStandardDeviation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RResponse[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      10,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00095103,
       2, QMetaType::QObjectStar, 0x00095103,
       3, QMetaType::Double, 0x00095001,
       4, QMetaType::Double, 0x00095001,
       5, QMetaType::Double, 0x00095001,
       6, QMetaType::Double, 0x00095001,
       7, QMetaType::Double, 0x00095001,
       8, QMetaType::Double, 0x00095001,
       9, QMetaType::Double, 0x00095001,
      10, QMetaType::Double, 0x00095001,

       0        // eod
};

void RResponse::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{

#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<RResponse *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->getCurrentValue(); break;
        case 1: *reinterpret_cast< QObject**>(_v) = _t->getModel(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->getTotalStandardDeviation(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->getBetaSensitivityWrtTotalStandardDeviation(); break;
        case 4: *reinterpret_cast< double*>(_v) = _t->getTotalEpistemicStandardDeviation(); break;
        case 5: *reinterpret_cast< double*>(_v) = _t->getBetaSensitivityWrtTotalEpistemicStandardDeviation(); break;
        case 6: *reinterpret_cast< double*>(_v) = _t->getModelStandardDeviation(); break;
        case 7: *reinterpret_cast< double*>(_v) = _t->getBetaSensitivityWrtModelStandardDeviation(); break;
        case 8: *reinterpret_cast< double*>(_v) = _t->getModelEpistemicStandardDeviation(); break;
        case 9: *reinterpret_cast< double*>(_v) = _t->getBetaSensitivityWrtModelEpistemicStandardDeviation(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<RResponse *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setCurrentValue(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setModel(*reinterpret_cast< QObject**>(_v)); break;
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

QT_INIT_METAOBJECT const QMetaObject RResponse::staticMetaObject = { {
    QMetaObject::SuperData::link<RParameter::staticMetaObject>(),
    qt_meta_stringdata_RResponse.data,
    qt_meta_data_RResponse,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RResponse::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RResponse::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RResponse.stringdata0))
        return static_cast<void*>(this);
    return RParameter::qt_metacast(_clname);
}

int RResponse::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RParameter::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
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
