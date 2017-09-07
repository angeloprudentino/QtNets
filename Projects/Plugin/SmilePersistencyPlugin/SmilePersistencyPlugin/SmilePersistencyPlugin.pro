QT -= gui
QT += core xmlpatterns

TARGET = SmilePersistencyPlugin
TEMPLATE = lib

DEFINES += SMILEPERSISTENCYPLUGIN_LIBRARY

INCLUDEPATH += ../../../../Shared/QN \
    ../../../../Shared/QN/QNData \
    ../../../../Shared/QN/QNPersistence \
    ../../../../Shared/Utils \
    ../../../../Shared/Smile \
    ../../../../Shared/Smile/SmileXML \
    ../../../../Shared/Smile/SmileData

SOURCES += smilepersistencyplugin.cpp \
    ../../../../Shared/Smile/SmileData/smilealgparams.cpp \
    ../../../../Shared/Smile/SmileData/smilegenieext.cpp \
    ../../../../Shared/Smile/SmileData/smilenode.cpp \
    ../../../../Shared/Smile/SmileData/smilenodegenieext.cpp \
    ../../../../Shared/Smile/SmileData/smileobservationcost.cpp \
    ../../../../Shared/Smile/SmileXML/smilexmlparser.cpp \
    ../../../../Shared/Smile/SmileXML/smilexmlwriter.cpp \
    ../../../../Shared/Utils/logger.cpp

HEADERS += smilepersistencyplugin.h\
    smilepersistencyplugin_global.h \
    ../../../../Shared/Smile/SmileData/smilealgparams.h \
    ../../../../Shared/Smile/SmileData/smilegenieext.h \
    ../../../../Shared/Smile/SmileData/smilenode.h \
    ../../../../Shared/Smile/SmileData/smilenodegenieext.h \
    ../../../../Shared/Smile/SmileData/smileobservationcost.h \
    ../../../../Shared/Smile/SmileXML/smilexmlconst.h \
    ../../../../Shared/Smile/SmileXML/smilexmlparser.h \
    ../../../../Shared/Smile/SmileXML/smilexmlwriter.h \
    ../../../../Shared/Utils/logger.h \
    ../../../../Shared/Utils/printable.h \
    ../../../../Shared/Smile/smiletypes.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
