#-------------------------------------------------
#
# Project created by QtCreator 2015-09-08T16:11:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimerEE
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    AboutDialog.cpp

HEADERS  += MainWindow.h \
    AboutDialog.h

FORMS    += MainWindow.ui \
    AboutDialog.ui

RC_FILE = timerEE.rc
