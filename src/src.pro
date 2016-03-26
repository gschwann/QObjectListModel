QT  += qml

TARGET = QObjectListModel
TEMPLATE = lib

DEFINES += QOBJECTLISTMODEL_LIBRARY

SOURCES += qobjectlistmodel.cpp

HEADERS += qobjectlistmodel.h\
    qobjectlistmodelt.h\
    qobjectlistmodel_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
