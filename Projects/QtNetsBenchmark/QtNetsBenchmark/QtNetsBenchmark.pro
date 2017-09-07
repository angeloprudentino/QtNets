QT += core widgets
QT -= gui

CONFIG += c++11

TARGET = QtNetsBenchmark
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += .\
    ../../../Shared/QN \
    ../../../Shared/QN/QNData \
    ../../../Shared/QN/QNDraw \
    ../../../Shared/QN/QNPersistence \
    ../../../Shared/QN/QNStyle \
    ../../../Shared/Utils \
    ../../../Shared/Style \
    ../../../Shared/Smile \
    ../../../Shared/Smile/SmileXML \
    ../../../Shared/Smile/SmileData

DEPENDPATH += /../../../Shared/QN/QNData \
    ../../../Shared/QN/QNPersistence \
    ../../../Shared/QN/QNStyle

HEADERS += \
    ../../../Shared/Utils/utility.h \
    benchmarkclass.h

SOURCES += main.cpp \
    ../../../Shared/Utils/utility.cpp \
    benchmarkclass.cpp

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsData/Release/ -lQtNetsData
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsData/Debug/ -lQtNetsData

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsPersistence/Release/ -lQtNetsPersistence
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsPersistence/Debug/ -lQtNetsPersistence

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsStyle/Release/ -lQtNetsStyle
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsStyle/Debug/ -lQtNetsStyle

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Plugin/SmilePersistencyPlugin/Release/ -lSmilePersistencyPlugin
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Plugin/SmilePersistencyPlugin/Debug/ -lSmilePersistencyPlugin
