#-------------------------------------------------
#
# Project created by QtCreator 2018-04-27T18:21:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PJC
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


SOURCES += \
        main.cpp \
    cmap.cpp \
    cprogram.cpp \
    cobject.cpp \
    cmovable.cpp \
    cnonmovable.cpp \
    cobstacle.cpp \
    crobot.cpp \
    cdirt.cpp \
    ctreasure.cpp \
    cmine.cpp \
    ccleaningrobot.cpp \
    csearchingrobot.cpp \
    cfightingrobot.cpp \
    cgobject.cpp \
    cgobstacle.cpp \
    cgdirt.cpp \
    cgcleaningrobot.cpp \
    cgmine.cpp \
    cgtreasure.cpp \
    cgsearchingrobot.cpp \
    cgfightingrobot.cpp

HEADERS += \
    cmap.h \
    cprogram.h \
    cuser.h \
    cobject.h \
    cmovable.h \
    cnonmovable.h \
    cobstacle.h \
    crobot.h \
    cdirt.h \
    ctreasure.h \
    cmine.h \
    ccleaningrobot.h \
    csearchingrobot.h \
    cfightingrobot.h \
    cgobject.h \
    cgobstacle.h \
    cgdirt.h \
    cgcleaningrobot.h \
    cgmine.h \
    cgtreasure.h \
    cgsearchingrobot.h \
    cgfightingrobot.h

FORMS +=