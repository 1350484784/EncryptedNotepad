#-------------------------------------------------
#
# Project created by QtCreator 2018-06-16T13:32:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project10
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    replacedialog.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    replacedialog.h

FORMS    += mainwindow.ui \
    finddialog.ui \
    replacedialog.ui

RESOURCES += \
    images.qrc
