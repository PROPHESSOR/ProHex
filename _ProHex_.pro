#-------------------------------------------------
#
# Project created by QtCreator 2018-10-11T00:34:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# CONFIG += static
QMAKE_CXXFLAGS_WARN_OFF = -Wunused-parameter -Wmissing-noreturn

TARGET = _ProHex_
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        datastorage.cpp \
        qhexview.cpp \
        converter.cpp \
        config.cpp \
        asciitable.cpp \
        preferences.cpp

HEADERS += \
        mainwindow.h \
        datastorage.h \
        qhexview.h \
        constants.h \
        converter.h \
        config.h \
        asciitable.h \
        preferences.h

FORMS += \
        mainwindow.ui \
        converter.ui \
        asciitable.ui \
        preferences.ui
