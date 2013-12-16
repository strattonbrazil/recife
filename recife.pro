#-------------------------------------------------
#
# Project created by QtCreator 2013-12-02T06:28:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = recife
TEMPLATE = app

LIBS += -lopencv_core -lopencv_highgui -lopencv_ocl -lopencv_imgproc

SOURCES += main.cpp\
        mainwindow.cpp \
    layermodel.cpp \
    source.cpp \
    compositorpane.cpp \
    renderview.cpp \
    utils.cpp \
    layerspane.cpp \
    effectspane.cpp \
    effectsmodel.cpp \
    effect.cpp \
    attributespane.cpp

HEADERS  += mainwindow.h \
    layermodel.h \
    source.h \
    compositorpane.h \
    renderview.h \
    utils.h \
    layerspane.h \
    effectspane.h \
    effectsmodel.h \
    effect.h \
    attributespane.h

FORMS    += mainwindow.ui \
    compositorpane.ui \
    layerspane.ui \
    effectspane.ui \
    attributespane.ui
