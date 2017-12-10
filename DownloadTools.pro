#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T20:50:19
#
#-------------------------------------------------

QT += core gui

LIBS += ./lib/libcurld.lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DownloadTools
TEMPLATE = app

SOURCES += main.cpp \
    downloadtools.cpp \
    download.cpp

HEADERS  += \
    downloadtools.h \
    download.h

FORMS    += \
    downloadtools.ui

