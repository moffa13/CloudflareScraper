QT -= gui
QT += network

TARGET = CloudflareScraper
CONFIG -= debug_and_release


DEFINES += DLL_EXPORT
DEFINES += QT_DEPRECATED_WARNINGS

contains(DEFINES, NO_DLL){
    TEMPLATE = app
    DESTDIR = $$PWD/../bin
}else{
    TEMPLATE = lib
    DESTDIR = $$PWD/../bin/lib
}

CONFIG += c++11 console

OBJECTS_DIR = $$PWD/../build
MOC_DIR = $$PWD/../build

SOURCES += $$PWD/CloudflareScraper.cpp \
        $$PWD/Cookies.cpp \
        $$PWD/Logger.cpp \
        $$PWD/main.cpp

HEADERS += $$PWD/CloudflareScraper.h \
        $$PWD/Cookies.h \
        $$PWD/CloudflareException.h \
        $$PWD/CloudflareScraper_export.h \
        $$PWD/Logger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
