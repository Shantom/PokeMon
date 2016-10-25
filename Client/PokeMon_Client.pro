#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T22:39:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PokeMon_Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pokemon.cpp \
    pmstrength.cpp \
    pmagility.cpp \
    pmshield.cpp \
    pmdefense.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    pokemon.h \
    pmstrength.h \
    pmagility.h \
    pmshield.h \
    pmdefense.h \
    logindialog.h

FORMS    += mainwindow.ui \
    logindialog.ui
