QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CenRepConverter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h


FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -unicode -fpermissive

win32 {
RC_FILE = CenRepConverter.rc
}

OTHER_FILES += \
    CenRepConverter.rc \
    info.h
