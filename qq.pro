#-------------------------------------------------
#
# Project created by QtCreator 2017-06-10T18:08:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qq
TEMPLATE = app


SOURCES += main.cpp \
    login.cpp \
    registerpage.cpp \
    chatpage.cpp \
    showresultpage.cpp \
    addfriendpage.cpp

HEADERS  += \
    login.h \
    registerpage.h \
    chatpage.h \
    showresultpage.h \
    addfriendpage.h \
    protocol.h

FORMS    += \
    login.ui \
    registerpage.ui \
    chatpage.ui \
    showresultpage.ui \
    addfriendpage.ui

LIBS+=-lsqlite3

