#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T22:31:08
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FactoryAlert-NodeUnit
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../common/common.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig

PKGCONFIG += QtGLib-2.0 QtGStreamer-0.10 QtGStreamerUi-0.10

target.path = /root
target.files = FactoryAlert-NodeUnit config.ini
INSTALLS += target
