QT       -= gui

TARGET = QtNetsPersistence
TEMPLATE = lib

DEFINES += QTNETSPERSISTENCE_LIBRARY

INCLUDEPATH += ../../../../Shared/QN \
    ../../../../Shared/QN/QNPersistence \
    ../../../../Shared/QN/QNData

SOURCES += ../../../../Shared/QN/QNPersistence/qnpersistencemanager.cpp

HEADERS += ../../../../Shared/QN/QNPersistence/qnpersistencemanager.h \
    ../../../../Shared/QN/QNPersistence/qnpersistenceinterface.h \
    ../../../../Shared/QN/QNPersistence/qnpersistencetypes.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
