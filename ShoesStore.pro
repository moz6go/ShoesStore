#-------------------------------------------------
#
# Project created by QtCreator 2019-03-15T15:46:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShoesStore
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
    storemainwindow.cpp \
    addmodeldialog.cpp

HEADERS  +=    storemainwindow.h \
    general.h \
    addmodeldialog.h

FORMS    +=    storemainwindow.ui \
    addmodeldialog.ui

RESOURCES += \
    pics.qrc
