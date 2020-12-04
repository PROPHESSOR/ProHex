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

TRANSLATIONS += \
        locale/locale_ru.ts \
        locale/locale_uk.ts

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
    Assembler/assembler.cpp \
        main.cpp \
        MainWindow/mainwindow.cpp \
        DataStorage/datastorage.cpp \
        QHexView/qhexview.cpp \
        Converter/converter.cpp \
        Config/config.cpp \
        AsciiTable/asciitable.cpp \
        Preferences/preferences.cpp \
        Strings/strings.cpp \
        Finder/finder.cpp \
        UndoCommands/undodelete.cpp \
        UndoCommands/undoinsert.cpp \
        UndoCommands/undoreplace.cpp \
    UndoCommands/undoxor.cpp

HEADERS += \
    Assembler/assembler.h \
        constants.h \
        MainWindow/mainwindow.h \
        DataStorage/datastorage.h \
        QHexView/qhexview.h \
        Converter/converter.h \
        Config/config.h \
        AsciiTable/asciitable.h \
        Preferences/preferences.h \
        Strings/strings.h \
        Finder/finder.h \
        UndoCommands/undodelete.h \
        UndoCommands/undocommands.h \
        UndoCommands/undoinsert.h \
        UndoCommands/undoreplace.h \
    UndoCommands/undoxor.h

FORMS += \
        Assembler/assembler.ui \
        MainWindow/mainwindow.ui \
        Converter/converter.ui \
        AsciiTable/asciitable.ui \
        Preferences/preferences.ui \
        Strings/strings.ui \
        Finder/finder.ui

STATECHARTS +=

DISTFILES +=

RESOURCES += \
    resources.qrc
