QT -= gui
QT += network

TARGET = CloudflareScraper

DEFINES += NO_DLL
DEFINES += QT_DEPRECATED_WARNINGS

contains(DEFINES, NO_DLL){
    TEMPLATE = app
}else{
    TEMPLATE = lib
}

CONFIG += c++11

DESTDIR = $$PWD/bin/

SOURCES += $$PWD/CloudflareScraper.cpp \
        $$PWD/Cookies.cpp \
        $$PWD/Logger.cpp \
        $$PWD/main.cpp

HEADERS += $$PWD/CloudflareScraper.h \
        $$PWD/Cookies.h \
        $$PWD/CloudFlareException.h \
        $$PWD/CloudflareScraper_export.h \
        $$PWD/Logger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
