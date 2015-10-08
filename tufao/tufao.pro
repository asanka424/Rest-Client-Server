#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T23:26:09
#
#-------------------------------------------------

QT       -= gui



# greaterThan(QT_MAJOR_VERSION, 4):
# QT += widgets

TARGET = Tufao

TEMPLATE = lib
QT += core network

CONFIG += dll
CONFIG +=c++11
# CONFIG += -stdlib=libc++
# QMAKE_CXXFLAGS += -std=c++11
# LIBS += -stdlib=libc++

#QMAKE_CXXFLAGS += -stdlib=libc++
# QMAKE_CXXFLAGS += -std=c++11
# QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
# QMAKE_LFLAGS += -mmacosx-version-min=10.7

DEFINES += QT_DLL TUFAO_LIBRARY

#CONFIG(debug, debug|release){
#    DESTDIR = $$_PRO_FILE_PWD_/../../../bin/Linux-Debug
#}else{
#    DESTDIR = $$_PRO_FILE_PWD_/../../../bin/Linux-Release
#}

INCLUDEPATH += $$_PRO_FILE_PWD_/include/tufao



HEADERS +=  $$_PRO_FILE_PWD_/include/*
HEADERS += $$_PRO_FILE_PWD_/src/*.h
HEADERS += $$_PRO_FILE_PWD_/src/priv/*.h

SOURCES +=  $$_PRO_FILE_PWD_/src/*.cpp
SOURCES +=  $$_PRO_FILE_PWD_/src/priv/*.cpp
SOURCES +=  $$_PRO_FILE_PWD_/src/priv/http_parser.c

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




