QT       += widgets

QT       -= gui

TARGET = QtNetsStyle
TEMPLATE = lib

DEFINES += QTNETSSTYLE_LIBRARY

SOURCES += ../../../../Shared/QN/QNStyle/qnstyleclass.cpp \
    ../../../../Shared/QN/QNStyle/qnstylemanager.cpp \
    ../../../../Shared/QN/QNStyle/qnstyle.cpp

INCLUDEPATH += ../../../../Shared/QN \
    ../../../../Shared/QN/QNData \
    ../../../../Shared/QN/QNDraw \
    ../../../../Shared/QN/QNStyle

HEADERS += ../../../../Shared/QN/QNStyle/qnstyleclass.h \
    ../../../../Shared/QN/QNStyle/qnstylemanager.h \
    ../../../../Shared/QN/QNStyle/qnstyletypes.h \
    ../../../../Shared/QN/QNStyle/qnstyleinterface.h \
    ../../../../Shared/QN/QNStyle/qnstyle.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
