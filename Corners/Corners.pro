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
    mygamefieldview.cpp

HEADERS  += cornersgame.h \
    mygamefieldview.h

FORMS    += cornersgame.ui

DISTFILES += \
    resources/fieldTexture.jpg

RESOURCES += \
    textures.qrc
