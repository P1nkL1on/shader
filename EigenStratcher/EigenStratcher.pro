#-------------------------------------------------
#
# Project created by QtCreator 2019-02-05T12:52:18
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EigenStratcher
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


include(../../QT-serious/modules/Eigen/Eigen.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    esmodel.cpp \
    esdrawer.cpp \
    escalculator.cpp \
    escalculatortests.cpp

HEADERS += \
        mainwindow.h \
    esmodel.h \
    esfuncs.h \
    esdrawer.h \
    escalculator.h \
    escalculatortests.h

FORMS += \
        mainwindow.ui