#-------------------------------------------------
#
# Project created by QtCreator 2015-09-14T16:37:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphwidget.cpp \
    scene.cpp

HEADERS  += mainwindow.h \
    graphwidget.h \
    scene.h

FORMS    += mainwindow.ui
