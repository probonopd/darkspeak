#-------------------------------------------------
#
# Project created by QtCreator 2018-02-27T16:25:05
#
#-------------------------------------------------

QT       += core network
INCLUDEPATH += $$PWD/../../dependencies/logfault/include/
TARGET = protlib
TEMPLATE = lib
CONFIG += staticlib
DEFINES += LOGFAULT_ENABLE_LOCATION=1

macx {
    INCLUDEPATH += /usr/local/Cellar/libsodium/1.0.17/include
}

unix {
    CONFIG += c++14
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/torprotocolmanager.cpp \
    src/torserviceinterface.cpp \
    src/connectionsocket.cpp \
    src/dsclient.cpp \
    src/torsocketlistener.cpp \
    src/peer.cpp \
    src/dsserver.cpp \
     src/imageutil.cpp

HEADERS += \
    include/ds/torprotocolmanager.h \
    include/ds/torserviceinterface.h \
    include/ds/connectionsocket.h \
    include/ds/dsclient.h \
    include/ds/torsocketlistener.h \
    include/ds/peer.h \
    include/ds/dsserver.h \
    include/ds/imageutil.h


INCLUDEPATH += $$PWD/include \
    $$PWD/../torlib/include \
    $$PWD/../corelib/include \
    $$PWD/../cryptolib/include

