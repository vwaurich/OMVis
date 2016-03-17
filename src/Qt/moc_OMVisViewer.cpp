/****************************************************************************
** Meta object code from reading C++ file 'OMVisViewer.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/Qt/OMVisViewer.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OMVisViewer.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OMVisViewer_t {
    QByteArrayData data[22];
    char stringdata0[316];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OMVisViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OMVisViewer_t qt_meta_stringdata_OMVisViewer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "OMVisViewer"
QT_MOC_LITERAL(1, 12, 20), // "updateTimingElements"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "playSlotFunction"
QT_MOC_LITERAL(4, 51, 17), // "pauseSlotFunction"
QT_MOC_LITERAL(5, 69, 16), // "initSlotFunction"
QT_MOC_LITERAL(6, 86, 18), // "coffeeSlotFunction"
QT_MOC_LITERAL(7, 105, 11), // "updateScene"
QT_MOC_LITERAL(8, 117, 22), // "setVisTimeSlotFunction"
QT_MOC_LITERAL(9, 140, 3), // "val"
QT_MOC_LITERAL(10, 144, 9), // "loadModel"
QT_MOC_LITERAL(11, 154, 12), // "loadModelCow"
QT_MOC_LITERAL(12, 167, 15), // "loadModelCessna"
QT_MOC_LITERAL(13, 183, 11), // "unloadModel"
QT_MOC_LITERAL(14, 195, 11), // "exportVideo"
QT_MOC_LITERAL(15, 207, 21), // "openDialogInputMapper"
QT_MOC_LITERAL(16, 229, 18), // "openDialogSettings"
QT_MOC_LITERAL(17, 248, 25), // "changeBGColourOfSceneView"
QT_MOC_LITERAL(18, 274, 8), // "colorIdx"
QT_MOC_LITERAL(19, 283, 17), // "updateKeyMapValue"
QT_MOC_LITERAL(20, 301, 3), // "key"
QT_MOC_LITERAL(21, 305, 10) // "aboutOMVis"

    },
    "OMVisViewer\0updateTimingElements\0\0"
    "playSlotFunction\0pauseSlotFunction\0"
    "initSlotFunction\0coffeeSlotFunction\0"
    "updateScene\0setVisTimeSlotFunction\0"
    "val\0loadModel\0loadModelCow\0loadModelCessna\0"
    "unloadModel\0exportVideo\0openDialogInputMapper\0"
    "openDialogSettings\0changeBGColourOfSceneView\0"
    "colorIdx\0updateKeyMapValue\0key\0"
    "aboutOMVis"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OMVisViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x0a /* Public */,
       3,    0,  100,    2, 0x0a /* Public */,
       4,    0,  101,    2, 0x0a /* Public */,
       5,    0,  102,    2, 0x0a /* Public */,
       6,    0,  103,    2, 0x0a /* Public */,
       7,    0,  104,    2, 0x0a /* Public */,
       8,    1,  105,    2, 0x0a /* Public */,
      10,    0,  108,    2, 0x0a /* Public */,
      11,    0,  109,    2, 0x0a /* Public */,
      12,    0,  110,    2, 0x0a /* Public */,
      13,    0,  111,    2, 0x0a /* Public */,
      14,    0,  112,    2, 0x0a /* Public */,
      15,    0,  113,    2, 0x0a /* Public */,
      16,    0,  114,    2, 0x0a /* Public */,
      17,    1,  115,    2, 0x0a /* Public */,
      19,    1,  118,    2, 0x0a /* Public */,
      21,    0,  121,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void,

       0        // eod
};

void OMVisViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OMVisViewer *_t = static_cast<OMVisViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateTimingElements(); break;
        case 1: _t->playSlotFunction(); break;
        case 2: _t->pauseSlotFunction(); break;
        case 3: _t->initSlotFunction(); break;
        case 4: _t->coffeeSlotFunction(); break;
        case 5: _t->updateScene(); break;
        case 6: _t->setVisTimeSlotFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->loadModel(); break;
        case 8: _t->loadModelCow(); break;
        case 9: _t->loadModelCessna(); break;
        case 10: _t->unloadModel(); break;
        case 11: _t->exportVideo(); break;
        case 12: _t->openDialogInputMapper(); break;
        case 13: _t->openDialogSettings(); break;
        case 14: _t->changeBGColourOfSceneView((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 15: _t->updateKeyMapValue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->aboutOMVis(); break;
        default: ;
        }
    }
}

const QMetaObject OMVisViewer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_OMVisViewer.data,
      qt_meta_data_OMVisViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OMVisViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OMVisViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OMVisViewer.stringdata0))
        return static_cast<void*>(const_cast< OMVisViewer*>(this));
    if (!strcmp(_clname, "osgViewer::CompositeViewer"))
        return static_cast< osgViewer::CompositeViewer*>(const_cast< OMVisViewer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int OMVisViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
