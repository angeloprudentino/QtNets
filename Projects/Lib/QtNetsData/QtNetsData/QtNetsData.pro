QT       -= gui

TARGET = QtNetsData
TEMPLATE = lib

DEFINES += QTNETSDATA_LIBRARY

INCLUDEPATH += ../../../../Shared/QN \
    ../../../../Shared/QN/QNData \
    ../../../../Shared/Utils

SOURCES += ../../../../Shared/QN/QNData/qnedge.cpp \
    ../../../../Shared/QN/QNData/qnedgestyle.cpp \
    ../../../../Shared/QN/QNData/qnfont.cpp \
    ../../../../Shared/QN/QNData/qnicon.cpp \
    ../../../../Shared/QN/QNData/qnmodel.cpp \
    ../../../../Shared/QN/QNData/qnnetwork.cpp \
    ../../../../Shared/QN/QNData/qnnetworkmodel.cpp \
    ../../../../Shared/QN/QNData/qnnode.cpp \
    ../../../../Shared/QN/QNData/qnnodeicon.cpp \
    ../../../../Shared/QN/QNData/qnposition.cpp \
    ../../../../Shared/QN/QNData/qntextbox.cpp \
    ../../../../Shared/Utils/optionalbool.cpp \
    ../../../../Shared/QN/qnerror.cpp \
    ../../../../Shared/Utils/utility.cpp \
    ../../../../Shared/QN/QNData/qnstylable.cpp \
    ../../../../Shared/QN/QNData/qncommonicon.cpp \
    ../../../../Shared/QN/QNData/qnproperties.cpp


HEADERS += ../../../../Shared/QN/qntypes.h \
    ../../../../Shared/QN/QNData/qnedge.h \
    ../../../../Shared/QN/QNData/qnedgestyle.h \
    ../../../../Shared/QN/QNData/qnfont.h \
    ../../../../Shared/QN/QNData/qngraphicconst.h \
    ../../../../Shared/QN/QNData/qnicon.h \
    ../../../../Shared/QN/QNData/qnmodel.h \
    ../../../../Shared/QN/QNData/qnnetwork.h \
    ../../../../Shared/QN/QNData/qnnetworkmodel.h \
    ../../../../Shared/QN/QNData/qnnode.h \
    ../../../../Shared/QN/QNData/qnnodeicon.h \
    ../../../../Shared/QN/QNData/qnposition.h \
    ../../../../Shared/QN/QNData/qntextbox.h \
    ../../../../Shared/Utils/optionalbool.h \
    ../../../../Shared/QN/qnglobalexport.h \
    ../../../../Shared/QN/qnerror.h \
    ../../../../Shared/Utils/utility.h \
    ../../../../Shared/QN/QNData/qnstylable.h \
    ../../../../Shared/QN/QNData/qncommonicon.h \
    ../../../../Shared/QN/QNData/qnproperties.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
