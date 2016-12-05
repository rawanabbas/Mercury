#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T11:57:25
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lpthread -lcrypto++ -lsqlite3
INCLUDEPATH += modules/include
TARGET = Mercury
TEMPLATE = app

PRECOMPILED_HEADER = modules/include/stdafx.h

SOURCES += mainwindow.cpp \
    main.cpp\
    modules/sources/udp_socket.cpp \
    modules/sources/job.cpp \
    modules/sources/thread.cpp \
    modules/sources/server.cpp \
    modules/sources/client.cpp \
    modules/sources/message.cpp \
    modules/sources/file.cpp \
    modules/sources/decoder.cpp \
    modules/sources/peer.cpp \
    modules/sources/heartbeat.cpp \
    modules/sources/connection_manager.cpp \
    modules/sources/steganography.cpp \
    login.cpp \
    upload.cpp
    login.cpp

HEADERS  += mainwindow.h \
    modules/include/udp_socket.hpp \
    modules/include/job.hpp \
    modules/include/thread.hpp \
    modules/include/server.hpp \
    modules/include/client.hpp \
    modules/include/message.hpp \
    modules/include/file.hpp \
    modules/include/decoder.hpp \
    modules/include/peer.hpp \
    modules/include/heartbeat.hpp \
    modules/include/connection_manager.hpp \
    modules/include/steganography.hpp \
    modules/include/stdafx.h \
    login.hpp \
    upload.hpp
    login.hpp


RC_FILE += mercury.rc


FORMS    += mainwindow.ui \
    login.ui \
    upload.ui

RESOURCES += \
    images.qrc
