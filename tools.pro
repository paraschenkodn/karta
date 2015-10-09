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
    scene.cpp \
    triangle.cpp \
    pointsofsphere.cpp \
    shphere.cpp

HEADERS  += mainwindow.h \
    scene.h \
    triangle.h \
    pointsofsphere.h \
    shphere.h

FORMS    += mainwindow.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    pointsofsphere.qrc
