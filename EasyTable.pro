#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T21:49:01
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EasyTable
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    finddialog.cpp \
    gotocelldialog.cpp \
    cell.cpp \
    easytable.cpp \
    sortdialog.cpp \
    mainwidget.cpp \
    easytablecompare.cpp \
    find.cpp \
    helpsearchdialog.cpp

HEADERS  += mainwindow.h \
    finddialog.h \
    gotocelldialog.h \
    cell.h \
    easytable.h \
    sortdialog.h \
    mainwidget.h \
    easytablecompare.h \
    find.h \
    helpsearchdialog.h

FORMS += \
    sortdialog.ui
