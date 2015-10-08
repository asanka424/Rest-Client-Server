#-------------------------------------------------
#
# Project created by QtCreator 2015-10-03T20:41:24
#
#-------------------------------------------------

QT       -= gui

TARGET = AppDepotUtilities
TEMPLATE = lib

DEFINES += APPDEPOTUTILITIES_LIBRARY

SOURCES += appdepotutilities.cpp \
    jsonhandler.cpp

HEADERS += appdepotutilities.h\
        appdepotutilities_global.h

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





