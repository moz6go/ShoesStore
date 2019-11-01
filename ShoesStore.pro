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
    addmodeldialog.cpp \
    addgoodsdialog.cpp \
    database.cpp \
    mainwindow.cpp \
    saledialog.cpp \
    reportdialog.cpp \
    dictionarydialog.cpp \
    returngoodsdialog.cpp \
    myproxymodel.cpp \
    sqlqueries.cpp \
    copyrestoredbdialog.cpp \
    archivedialog.cpp

HEADERS  +=    \
    general.h \
    addmodeldialog.h \
    addgoodsdialog.h \
    database.h \
    mainwindow.h \
    saledialog.h \
    reportdialog.h \
    dictionarydialog.h \
    returngoodsdialog.h \
    myproxymodel.h \
    sqlqueries.h \
    copyrestoredbdialog.h \
    archivedialog.h

FORMS    +=    \
    addmodeldialog.ui \
    addgoodsdialog.ui \
    mainwindow.ui \
    saledialog.ui \
    reportdialog.ui \
    dictionarydialog.ui \
    returngoodsdialog.ui \
    copyrestoredbdialog.ui \
    archivedialog.ui

RESOURCES += \
    pics.qrc

RC_ICONS = main_icon.ico