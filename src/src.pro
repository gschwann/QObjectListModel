QT  += qml

TARGET = QObjectListModel
TEMPLATE = lib

DEFINES += QOBJECTLISTMODEL_LIBRARY

SOURCES += qobjectlistmodel.cpp\
    qobjectlistmodeladapter.cpp\
    qobjectlistmodelindex.cpp

HEADERS += qobjectlistmodel.h\
    qobjectlistmodeladapter.h\
    qobjectlistmodelattachment.h\
    qobjectlistmodelindex.h\
    qobjectlistmodelt.h\
    qobjectlistmodel_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
