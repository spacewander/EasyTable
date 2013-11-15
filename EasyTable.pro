#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T21:49:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyTable
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    gotocelldialog.cpp \
    cell.cpp \
    easytable.cpp \
    sortdialog.cpp \
    mainwidget.cpp \
    easytablecompare.cpp \
    find.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    gotocelldialog.h \
    cell.h \
    easytable.h \
    sortdialog.h \
    mainwidget.h \
    easytablecompare.h \
    find.h

FORMS += \
    sortdialog.ui
