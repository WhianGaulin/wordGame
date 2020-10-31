#-------------------------------------------------
#
# Project created by QtCreator 2019-05-02T09:32:20
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wg_server
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        play1.cpp \
    play2.cpp \
    mainwindow.cpp \
    rgst.cpp \
    game.cpp \
    breaker.cpp \
    builder.cpp \
    datactrl.cpp \
    board.cpp \
    mytcpserver.cpp \
    mytcpsocket.cpp \
    mythread.cpp \
    listen.cpp

HEADERS += \
        play1.h \
    play2.h \
    mainwindow.h \
    rgst.h \
    game.h \
    datactrl.h \
    usr.h \
    breaker.h \
    builder.h \
    board.h \
    mytcpserver.h \
    mytcpsocket.h \
    mythread.h \
    listen.h \
    qvariant_define.h

FORMS += \
        play1.ui \
    play2.ui \
    mainwindow.ui \
    rgst.ui \
    game.ui \
    board.ui \
    listen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
