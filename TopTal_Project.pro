TARGET = .
TEMPLATE = subdirs
CONFIG += ordered
CONFIG +=c++11

DEFINES += HTTPS_SERVER
SUBDIRS = tufao \
            Zone_Client \
    Zone_Server \
    AppDepotUtilities



################################
# Output configuration
################################
CONFIG(debug, debug|release){
    DESTDIR = $$_PRO_FILE_PWD_/../../../bin/Debug
}else{
    DESTDIR = $$_PRO_FILE_PWD_/../../../bin/Release
}
#Debug: DESTDIR = $$_PRO_FILE_PWD_/../../bin/Linux-Debug
#Release: DESTDIR = $$_PRO_FILE_PWD_/../../bin/Linux-Release

MOC_DIR = $$_PRO_FILE_PWD_/GeneratedFiles
UI_DIR = $$_PRO_FILE_PWD_/GeneratedFiles
RCC_DIR = $$_PRO_FILE_PWD_/GeneratedFiles


OBJECTS_DIR = $$_PRO_FILE_PWD_/../../bin-temp/$${TARGET}
