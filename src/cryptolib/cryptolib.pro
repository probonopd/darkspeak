#-------------------------------------------------
#
# Project created by QtCreator 2018-03-01T10:30:29
#
#-------------------------------------------------

QT       -= gui sql
INCLUDEPATH += $$PWD/../../dependencies/logfault/include/
TARGET = cryptolib
TEMPLATE = lib
CONFIG += staticlib c++17
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
    src/crypto.cpp \
    #src/rsacertimpl.cpp \
    src/certimpl.cpp \
    src/base58.cpp \
    src/base32.cpp

HEADERS += \
    include/ds/crypto.h \
    include/ds/dscert.h \
    #include/ds/rsacertimpl.h \
    include/ds/cvar.h \
    include/ds/certimpl.h \
    include/ds/base58.h \
    include/ds/base32.h \
    include/ds/safememory.h \
    include/ds/memoryview.h

INCLUDEPATH += $$PWD/include
