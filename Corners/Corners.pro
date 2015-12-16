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
    settingsdialog.cpp \
    rulesdialog.cpp \
    solutiontree.cpp

HEADERS  += cornersgame.h \
    mygamefieldview.h \
    checker.h \
    newgamedialog.h \
    exitdialog.h \
    settingsdialog.h \
    rulesdialog.h \
    solutiontree.h

FORMS    += cornersgame.ui \
    newgamedialog.ui \
    exitdialog.ui \
    settingsdialog.ui \
    rulesdialog.ui

DISTFILES +=

RESOURCES += \
    textures.qrc

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

RC_ICONS = icon.ico
