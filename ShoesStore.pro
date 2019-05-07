#-------------------------------------------------
#
# Project created by QtCreator 2019-03-15T15:46:42
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShoesStore
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
    storemainwindow.cpp \
    addmodeldialog.cpp \
    addgoodsdialog.cpp \
    database.cpp \
    saledialog.cpp \
    reportdialog.cpp \
    dictionarydialog.cpp \
    returngoodsdialog.cpp

HEADERS  +=    storemainwindow.h \
    general.h \
    addmodeldialog.h \
    addgoodsdialog.h \
    database.h \
    saledialog.h \
    reportdialog.h \
    dictionarydialog.h \
    returngoodsdialog.h

FORMS    +=    storemainwindow.ui \
    addmodeldialog.ui \
    addgoodsdialog.ui \
    saledialog.ui \
    reportdialog.ui \
    dictionarydialog.ui \
    returngoodsdialog.ui

RESOURCES += \
    pics.qrc

RC_ICONS = main_icon.ico