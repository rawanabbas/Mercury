#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T11:57:25
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lpthread -lcrypto++
INCLUDEPATH += modules/include
TARGET = Mercury
TEMPLATE = app

PRECOMPILED_HEADER = modules/include/stdafx.h

SOURCES += mainwindow.cpp \
#    main.cpp\
    modules/sources/app.cpp \
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
#    modules/sources/client_main.cpp \
#    modules/sources/logger.cpp \
#    modules/sources/server_main.cpp \
#    modules/sources/socket_exception.cpp \
#    modules/sources/peer_server.cpp \
#    modules/sources/user.cpp \
#    modules/sources/database.cpp \
#    modules/sources/tracker.cpp \
#    modules/sources/steganography.cpp

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
#    modules/include/logger.hpp \
#    modules/include/peer_server.hpp \
#    modules/include/socket_exception.hpp \
#    modules/include/user.hpp \
#    modules/include/database.hpp \
#    modules/include/tracker.hpp \
#    modules/include/steganography.hpp \
#    modules/include/stdafx.h

#APP_OBJS = \
#	$(OBJ_DIR)/udp_socket.o \
#	$(OBJ_DIR)/job.o \
#	$(OBJ_DIR)/thread.o \
#	$(OBJ_DIR)/server.o \
#	$(OBJ_DIR)/client.o \
#	$(OBJ_DIR)/message.o \
#	$(OBJ_DIR)/file.o \
#	$(OBJ_DIR)/decoder.o \
#	$(OBJ_DIR)/peer.o \
#	$(OBJ_DIR)/heartbeat.o \
#	$(OBJ_DIR)/connection_manager.o \
#	$(OBJ_DIR)/app.o


RC_FILE = mercury.rc


FORMS    += mainwindow.ui
