QT       += widgets core xmlpatterns
QT       -= gui

TARGET = BaseStylePlugin
TEMPLATE = lib

DEFINES += BASESTYLEPLUGIN_LIBRARY

SOURCES += basestyleplugin.cpp \
    ../../../../Shared/Style/StyleXML/basestylexmlparser.cpp \
    ../../../../Shared/Style/StyleXML/basestylexmlwriter.cpp \
    ../../../../Shared/Utils/logger.cpp

INCLUDEPATH += ../../../../Shared/Utils \
    ../../../../Shared/QN \
    ../../../../Shared/QN/QNData \
    ../../../../Shared/QN/QNDraw \
    ../../../../Shared/QN/QNStyle \
    ../../../../Shared/Style \
    ../../../../Shared/Style/StyleXML

HEADERS += basestyleplugin.h\
    basestyleplugin_global.h \
    ../../../../Shared/Style/StyleXML/basestylexmlparser.h \
    ../../../../Shared/Style/StyleXML/basestylexmlwriter.h \
    ../../../../Shared/Style/basestyletypes.h \
    ../../../../Shared/Utils/logger.h \
    ../../../../Shared/Style/basestyleconst.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
