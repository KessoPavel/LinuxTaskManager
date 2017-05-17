#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07T18:02:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cube
TEMPLATE = app


SOURCES += main.cpp\
    process.cpp \
    logic.cpp \
    mainwindow.cpp \
    proctablemodel.cpp \
    rehreshthread.cpp

HEADERS  += \
    process.h \
    logic.h \
    mainwindow.h \
    proctablemodel.h \
    rehreshthread.h

FORMS    += \
    mainwindow.ui

DISTFILES +=
