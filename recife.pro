#-------------------------------------------------
#
# Project created by QtCreator 2013-12-02T06:28:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = recife
TEMPLATE = app

LIBS += -lopencv_core -lopencv_highgui -lopencv_ocl

SOURCES += main.cpp\
        mainwindow.cpp \
    layermodel.cpp \
    source.cpp \
    compositorpane.cpp \
    renderview.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    layermodel.h \
    source.h \
    compositorpane.h \
    renderview.h \
    utils.h

FORMS    += mainwindow.ui \
    compositorpane.ui
