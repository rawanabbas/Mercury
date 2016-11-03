#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T11:57:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mercury
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thread.cpp \
    socket.cpp \
    client.cpp \
    server.cpp \
    message.cpp

HEADERS  += mainwindow.h \
    thread.h \
    socket.h \
    client.h \
    server.h \
    message.h

FORMS    += mainwindow.ui
