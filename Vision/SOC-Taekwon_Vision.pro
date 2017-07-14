#-------------------------------------------------
#
# Project created by QtCreator 2017-07-14T15:59:19
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SOC-Taekwon_Vision
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    imageglview.cpp \
    capturethread.cpp

HEADERS  += widget.h \
    imageglview.h \
    capturethread.h

FORMS    += widget.ui
