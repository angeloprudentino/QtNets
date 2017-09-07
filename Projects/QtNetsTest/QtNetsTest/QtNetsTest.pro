QT       += widgets testlib

TARGET = QtNetsTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += ../../../Shared/QN \
    ../../../Shared/QN/QNData \
    ../../../Shared/QN/QNDraw \
    ../../../Shared/QN/QNPersistence \
    ../../../Shared/QN/QNStyle \
    ../../../Shared/Style \
    ../../../Shared/Utils \
    ../../../Shared/Smile \
    ../../../Shared/Smile/SmileXML \
    ../../../Shared/Smile/SmileData

SOURCES += tst_qtnetstest.cpp \
    ../../../Shared/Utils/utility.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../../Shared/Utils/utility.h

DEPENDPATH += /../../../Shared/QN/QNData \
    ../../../Shared/QN/QNDraw \
    ../../../Shared/QN/QNPersistence \
    ../../../Shared/QN/QNStyle

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsData/Release/ -lQtNetsData
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsData/Debug/ -lQtNetsData

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsDraw/Release/ -lQtNetsDraw
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsDraw/Debug/ -lQtNetsDraw

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsPersistence/Release/ -lQtNetsPersistence
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsPersistence/Debug/ -lQtNetsPersistence

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsStyle/Release/ -lQtNetsStyle
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsStyle/Debug/ -lQtNetsStyle

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Plugin/SmilePersistencyPlugin/Release/ -lSmilePersistencyPlugin
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Plugin/SmilePersistencyPlugin/Debug/ -lSmilePersistencyPlugin
