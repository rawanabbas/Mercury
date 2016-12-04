#-------------------------------------------------
#
# Project created by QtCreator 2016-11-26T14:36:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Distributed
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../../Desktop/Mercury-master/modules/sources/app.cpp \
    ../../../Desktop/Mercury-master/modules/sources/client_main.cpp \
    ../../../Desktop/Mercury-master/modules/sources/client.cpp \
    ../../../Desktop/Mercury-master/modules/sources/connection_manager.cpp \
    ../../../Desktop/Mercury-master/modules/sources/database.cpp \
    ../../../Desktop/Mercury-master/modules/sources/decoder.cpp \
    ../../../Desktop/Mercury-master/modules/sources/file.cpp \
    ../../../Desktop/Mercury-master/modules/sources/heartbeat.cpp \
    ../../../Desktop/Mercury-master/modules/sources/job.cpp \
    ../../../Desktop/Mercury-master/modules/sources/logger.cpp \
    ../../../Desktop/Mercury-master/modules/sources/message.cpp \
    ../../../Desktop/Mercury-master/modules/sources/peer_server_main.cpp \
    ../../../Desktop/Mercury-master/modules/sources/peer_server.cpp \
    ../../../Desktop/Mercury-master/modules/sources/peer.cpp \
    ../../../Desktop/Mercury-master/modules/sources/server_main.cpp \
    ../../../Desktop/Mercury-master/modules/sources/server.cpp \
    ../../../Desktop/Mercury-master/modules/sources/socket_exception.cpp \
    ../../../Desktop/Mercury-master/modules/sources/steganography.cpp \
    ../../../Desktop/Mercury-master/modules/sources/thread.cpp \
    ../../../Desktop/Mercury-master/modules/sources/tracker.cpp \
    ../../../Desktop/Mercury-master/modules/sources/udp_socket.cpp \
    ../../../Desktop/Mercury-master/modules/sources/user.cpp

HEADERS  += mainwindow.h \
    ../../../Desktop/Mercury-master/modules/include/peerServer.h \
    ../../../Desktop/Mercury-master/modules/include/stdfax.h \
    ../../../Desktop/Mercury-master/modules/include/client.hpp \
    ../../../Desktop/Mercury-master/modules/include/connection_manager.hpp \
    ../../../Desktop/Mercury-master/modules/include/database.hpp \
    ../../../Desktop/Mercury-master/modules/include/decoder.hpp \
    ../../../Desktop/Mercury-master/modules/include/file.hpp \
    ../../../Desktop/Mercury-master/modules/include/heartbeat.hpp \
    ../../../Desktop/Mercury-master/modules/include/job.hpp \
    ../../../Desktop/Mercury-master/modules/include/logger.hpp \
    ../../../Desktop/Mercury-master/modules/include/message.hpp \
    ../../../Desktop/Mercury-master/modules/include/peer_server.hpp \
    ../../../Desktop/Mercury-master/modules/include/peer.hpp \
    ../../../Desktop/Mercury-master/modules/include/server.hpp \
    ../../../Desktop/Mercury-master/modules/include/socket_exception.hpp \
    ../../../Desktop/Mercury-master/modules/include/stdafx.h \
    ../../../Desktop/Mercury-master/modules/include/steganography.hpp \
    ../../../Desktop/Mercury-master/modules/include/thread.hpp \
    ../../../Desktop/Mercury-master/modules/include/tracker.hpp \
    ../../../Desktop/Mercury-master/modules/include/udp_socket.hpp \
    ../../../Desktop/Mercury-master/modules/include/user.hpp

FORMS    += mainwindow.ui
