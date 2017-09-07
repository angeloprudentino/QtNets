QT -= gui
QT += widgets

TARGET = QtNetsDraw
TEMPLATE = lib

DEFINES += QTNETSDRAW_LIBRARY

INCLUDEPATH += ../../../../Shared/QN \
    ../../../../Shared/QN/QNDraw \
    ../../../../Shared/QN/QNData \
    ../../../../Shared/QN/QNStyle \
    ../../../../Shared/Utils

SOURCES += ../../../../Shared/QN/QNDraw/qnnetworkscene.cpp \
    ../../../../Shared/QN/QNDraw/qnnodeitem.cpp \
    ../../../../Shared/QN/QNDraw/qncommonitem.cpp \
    ../../../../Shared/QN/QNDraw/qnmodelitem.cpp \
    ../../../../Shared/QN/QNDraw/qntextboxitem.cpp \
    ../../../../Shared/QN/QNDraw/qnedgeitem.cpp \
    ../../../../Shared/QN/QNDraw/qnnetworkview.cpp

HEADERS += ../../../../Shared/QN/QNDraw/qnnetworkscene.h \
    ../../../../Shared/QN/QNDraw/qndrawtypes.h \
    ../../../../Shared/QN/QNDraw/qnnodeitem.h \
    ../../../../Shared/QN/QNDraw/qncommonitem.h \
    ../../../../Shared/QN/QNDraw/qndrawconst.h \
    ../../../../Shared/QN/QNDraw/qnmodelitem.h \
    ../../../../Shared/QN/QNDraw/qntextboxitem.h \
    ../../../../Shared/QN/QNDraw/qnedgeitem.h \
    ../../../../Shared/QN/QNDraw/qnnetworkview.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
