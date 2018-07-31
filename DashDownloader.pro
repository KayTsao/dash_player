#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T09:42:09
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DashDownloader
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    MPD/src/MPD.cpp

HEADERS += \
    MPD/src/MPD.h \
    MPD/src/MPD_AdaptationSet.h \
    MPD/src/MPD_Period.h \
    MPD/src/MPD_Representation.h \
    MPD/src/MPD_Segment.h
