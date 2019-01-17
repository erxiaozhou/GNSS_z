#-------------------------------------------------
#
# Project created by QtCreator 2018-11-23T12:51:05
#
#-------------------------------------------------

QT       += core gui
QT       +=charts
QT       +=axcontainer
QT       +=sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ch_9_4
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

CONFIG += c++11

CONFIG += qaxcontainer

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qwchartview.cpp \
    save_xls.cpp \
    fac_toshow.cpp \
    BOC_Fac.cpp \
    BPSK_Fac.cpp \
    integration.cpp \
    NELP.cpp \
    Gboc.cpp \
    Multipath.cpp \
    Kls.cpp \
    slots.cpp \
    save_screen.cpp \
    tbdialog.cpp

HEADERS += \
        mainwindow.h \
    qwchartview.h \
    save_xls.h \
    ../build-ch_9_4-Desktop_Qt_5_9_7_MinGW_32bit-Debug/ui_mainwindow.h \
    BOC_Fac.h \
    BPSK_Fac.h \
    integration.h \
    NELP.h \
    Gboc.h \
    Multipath.h \
    Kls.h \
    tbdialog.h

FORMS += \
        mainwindow.ui \
    tbdialog.ui \
    tbdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
