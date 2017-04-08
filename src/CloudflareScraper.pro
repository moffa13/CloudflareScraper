#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T19:40:02
#
#-------------------------------------------------

QT       -= gui
QT		 += network

TARGET = CloudflareScraper


DEFINES += NO_DLL DLL_EXPORT
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
