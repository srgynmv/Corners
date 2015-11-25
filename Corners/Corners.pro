#-------------------------------------------------
#
# Project created by QtCreator 2015-11-22T12:03:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Corners
TEMPLATE = app


SOURCES += main.cpp\
        cornersgame.cpp \
    mygamefieldview.cpp \
    checker.cpp \
    newgamedialog.cpp \
    exitdialog.cpp \
    settingsdialog.cpp

HEADERS  += cornersgame.h \
    mygamefieldview.h \
    checker.h \
    newgamedialog.h \
    exitdialog.h \
    settingsdialog.h

FORMS    += cornersgame.ui \
    newgamedialog.ui \
    exitdialog.ui \
    settingsdialog.ui

DISTFILES +=

RESOURCES += \
    textures.qrc
