#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T19:49:31
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = webcam
TEMPLATE = app


SOURCES += main.cpp\
        webcam.cpp \
    qtwin.cpp \
    cameraframegrabber.cpp

HEADERS  += webcam.h \
    qtwin.h \
    cameraframegrabber.h

FORMS    += webcam.ui \
    webcamselect.ui

QMAKE_CXXFLAGS += -std=gnu++11
