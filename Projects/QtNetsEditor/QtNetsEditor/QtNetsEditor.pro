QT       += core gui
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtNetsEditor
TEMPLATE = app

INCLUDEPATH += ../../../Shared/QN \
    ../../../Shared/QN/QNData \
    ../../../Shared/QN/QNDraw \
    ../../../Shared/QN/QNPersistence \
    ../../../Shared/QN/QNStyle \
    ../../../Shared/Utils \
    ../../../Shared/Style

DEPENDPATH += /../../../Shared/QN/QNData \
    ../../../Shared/QN/QNDraw \
    ../../../Shared/QN/QNPersistence \
    ../../../Shared/QN/QNStyle

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsData/Release/ -lQtNetsData
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsData/Debug/ -lQtNetsData

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsDraw/Release/ -lQtNetsDraw
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsDraw/Debug/ -lQtNetsDraw

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsPersistence/Release/ -lQtNetsPersistence
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsPersistence/Debug/ -lQtNetsPersistence

unix|win32:CONFIG(release, debug|release): LIBS += -L../../Lib/QtNetsStyle/Release/ -lQtNetsStyle
else:unix|win32:CONFIG(debug, debug|release): LIBS += -L../../Lib/QtNetsStyle/Debug/ -lQtNetsStyle
