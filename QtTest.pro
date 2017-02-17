#-------------------------------------------------
#
# Project created by QtCreator 2017-02-12T17:55:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filedatasource.cpp \
    qtdrawer.cpp \
    drawables.cpp

HEADERS  += mainwindow.h \
    interfaces.h \
    filedatasource.h \
    qtdrawer.h \
    drawables.h

FORMS    += mainwindow.ui
