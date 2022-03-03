/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[28];
    char stringdata0[376];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 4), // "open"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 4), // "save"
QT_MOC_LITERAL(4, 22, 13), // "saveSelection"
QT_MOC_LITERAL(5, 36, 13), // "readSelection"
QT_MOC_LITERAL(6, 50, 16), // "signalNavigation"
QT_MOC_LITERAL(7, 67, 4), // "flag"
QT_MOC_LITERAL(8, 72, 13), // "shiftToOrigin"
QT_MOC_LITERAL(9, 86, 10), // "updateTree"
QT_MOC_LITERAL(10, 97, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(11, 112, 12), // "QMouseEvent*"
QT_MOC_LITERAL(12, 125, 5), // "event"
QT_MOC_LITERAL(13, 131, 30), // "on_pushButton_clearAll_clicked"
QT_MOC_LITERAL(14, 162, 19), // "on_treeView_clicked"
QT_MOC_LITERAL(15, 182, 11), // "QModelIndex"
QT_MOC_LITERAL(16, 194, 5), // "index"
QT_MOC_LITERAL(17, 200, 21), // "runToolpathGeneration"
QT_MOC_LITERAL(18, 222, 25), // "toolpath_computeParameter"
QT_MOC_LITERAL(19, 248, 19), // "toolPathGeneration*"
QT_MOC_LITERAL(20, 268, 17), // "layerToolPathComp"
QT_MOC_LITERAL(21, 286, 4), // "int&"
QT_MOC_LITERAL(22, 291, 11), // "boundaryNum"
QT_MOC_LITERAL(23, 303, 14), // "zigzagTPathNum"
QT_MOC_LITERAL(24, 318, 16), // "boundaryTPathNum"
QT_MOC_LITERAL(25, 335, 7), // "double&"
QT_MOC_LITERAL(26, 343, 12), // "shrinkOffset"
QT_MOC_LITERAL(27, 356, 19) // "boundaryTPathOffset"

    },
    "MainWindow\0open\0\0save\0saveSelection\0"
    "readSelection\0signalNavigation\0flag\0"
    "shiftToOrigin\0updateTree\0mouseMoveEvent\0"
    "QMouseEvent*\0event\0on_pushButton_clearAll_clicked\0"
    "on_treeView_clicked\0QModelIndex\0index\0"
    "runToolpathGeneration\0toolpath_computeParameter\0"
    "toolPathGeneration*\0layerToolPathComp\0"
    "int&\0boundaryNum\0zigzagTPathNum\0"
    "boundaryTPathNum\0double&\0shrinkOffset\0"
    "boundaryTPathOffset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    1,   78,    2, 0x08 /* Private */,
       8,    0,   81,    2, 0x08 /* Private */,
       9,    0,   82,    2, 0x08 /* Private */,
      10,    1,   83,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    1,   87,    2, 0x08 /* Private */,
      17,    0,   90,    2, 0x08 /* Private */,
      18,    6,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19, 0x80000000 | 21, 0x80000000 | 21, 0x80000000 | 21, 0x80000000 | 25, 0x80000000 | 25,   20,   22,   23,   24,   26,   27,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->open(); break;
        case 1: _t->save(); break;
        case 2: _t->saveSelection(); break;
        case 3: _t->readSelection(); break;
        case 4: _t->signalNavigation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->shiftToOrigin(); break;
        case 6: _t->updateTree(); break;
        case 7: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 8: _t->on_pushButton_clearAll_clicked(); break;
        case 9: _t->on_treeView_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: _t->runToolpathGeneration(); break;
        case 11: _t->toolpath_computeParameter((*reinterpret_cast< toolPathGeneration*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
