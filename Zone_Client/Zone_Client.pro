#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T21:06:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Zone_Client
TEMPLATE = app
CONFIG +=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    signupdialog.cpp \
    signindialog.cpp \
    mydatamodel.cpp \
    dropmenudelegate.cpp \
    configuration.cpp \
    controller.cpp \
    restapihandler.cpp \
    usermod.cpp


INCLUDEPATH += $$_PRO_FILE_PWD_/../AppDepotUtilities


HEADERS  += mainwindow.h \
    signupdialog.h \
    signindialog.h \
    mydatamodel.h \
    dropmenudelegate.h \
    configuration.h \
    controller.h \
    restapihandler.h \
    usermod.h

FORMS    += mainwindow.ui \
    signupdialog.ui \
    signindialog.ui \
    usermod.ui

CONFIG(debug, debug|release){
    DESTDIR = $$_PRO_FILE_PWD_/../bin/Debug
}else{
    DESTDIR = $$_PRO_FILE_PWD_/../bin/Release
}
#Debug: DESTDIR = $$_PRO_FILE_PWD_/../../bin/Linux-Debug
#Release: DESTDIR = $$_PRO_FILE_PWD_/../../bin/Linux-Release

MOC_DIR = $$_PRO_FILE_PWD_/../GeneratedFiles
UI_DIR = $$_PRO_FILE_PWD_/../GeneratedFiles
RCC_DIR = $$_PRO_FILE_PWD_/../GeneratedFiles


OBJECTS_DIR = $$_PRO_FILE_PWD_/../bin-temp/$${TARGET}

RESOURCES += \
    client_resources.qrc

LIBS+= -L$${DESTDIR}/ -lAppDepotUtilities
DEFINES += HTTPS_SERVER
