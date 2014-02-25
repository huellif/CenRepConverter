#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T17:56:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CenRepConverter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -fpermissive

win32 {
RC_FILE = CenRepConverter.rc
}
