/****************************************************************************
** Meta object code from reading C++ file 'RFlowchartWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../R3_Source/Core/FlowchartWidget/RFlowchartWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RFlowchartWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RFlowchartWidget_t {
    QByteArrayData data[12];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RFlowchartWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RFlowchartWidget_t qt_meta_stringdata_RFlowchartWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "RFlowchartWidget"
QT_MOC_LITERAL(1, 17, 9), // "showPopup"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "position"
QT_MOC_LITERAL(4, 37, 23), // "saveDiagramToVectorFile"
QT_MOC_LITERAL(5, 61, 22), // "saveDiagramToPixelFile"
QT_MOC_LITERAL(6, 84, 12), // "printDiagram"
QT_MOC_LITERAL(7, 97, 8), // "setElbow"
QT_MOC_LITERAL(8, 106, 7), // "checked"
QT_MOC_LITERAL(9, 114, 6), // "zoomIn"
QT_MOC_LITERAL(10, 121, 7), // "zoomOut"
QT_MOC_LITERAL(11, 129, 9) // "resetZoom"

    },
    "RFlowchartWidget\0showPopup\0\0position\0"
    "saveDiagramToVectorFile\0saveDiagramToPixelFile\0"
    "printDiagram\0setElbow\0checked\0zoomIn\0"
    "zoomOut\0resetZoom"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RFlowchartWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       4,    0,   57,    2, 0x08 /* Private */,
       5,    0,   58,    2, 0x08 /* Private */,
       6,    0,   59,    2, 0x08 /* Private */,
       7,    1,   60,    2, 0x08 /* Private */,
       9,    0,   63,    2, 0x08 /* Private */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    0,   65,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RFlowchartWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RFlowchartWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showPopup((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: _t->saveDiagramToVectorFile(); break;
        case 2: _t->saveDiagramToPixelFile(); break;
        case 3: _t->printDiagram(); break;
        case 4: _t->setElbow((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->zoomIn(); break;
        case 6: _t->zoomOut(); break;
        case 7: _t->resetZoom(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RFlowchartWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_RFlowchartWidget.data,
    qt_meta_data_RFlowchartWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RFlowchartWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RFlowchartWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RFlowchartWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Ui::RFlowchartWidgetUi"))
        return static_cast< Ui::RFlowchartWidgetUi*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int RFlowchartWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
