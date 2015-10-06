#-------------------------------------------------
#
# Project created by QtCreator 2015-09-14T16:37:33
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    lenswindow.cpp \
    openglwindow.cpp \
    oglw.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    scene.h \
    lenswindow.h \
    openglwindow.h \
    oglw.h \
    glwidget.h

FORMS    += mainwindow.ui
