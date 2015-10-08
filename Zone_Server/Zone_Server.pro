#-------------------------------------------------
#
# Project created by QtCreator 2015-09-30T05:59:34
#
#-------------------------------------------------

QT       += core network sql

QT       -= gui

TARGET = Zone_Server
CONFIG   += console
CONFIG   -= app_bundle
CONFIG +=c++11

TEMPLATE = app


SOURCES += main.cpp \
    userhandler.cpp \
    datahandler.cpp \
    myhttpserver.cpp \
    myrequesthandler.cpp \
    dbhandler.cpp \
    zoneapphandler.cpp \
    abstractoperationshandler.cpp \
    abstractappshander.cpp \
    configuration.cpp


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

INCLUDEPATH += $$_PRO_FILE_PWD_/../tufao/include
INCLUDEPATH += $$_PRO_FILE_PWD_/../tufao/src

INCLUDEPATH += $$_PRO_FILE_PWD_/../AppDepotUtilities


LIBS+= -L$${DESTDIR}/ -lTufao
LIBS+= -L$${DESTDIR}/ -lAppDepotUtilities

HEADERS += \
    userhandler.h \
    datahandler.h \
    myhttpserver.h \
    myrequesthandler.h \
    dbhandler.h \
    zoneapphandler.h \
    abstractoperationshandler.h \
    abstractappshander.h \
    configuration.h

RESOURCES += \
    resources.qrc
DEFINES += HTTPS_SERVER
