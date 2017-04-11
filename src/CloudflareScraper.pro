QT -= gui
QT += network

TARGET = CloudflareScraper


DEFINES += DLL_EXPORT NO_DLL
DEFINES += QT_DEPRECATED_WARNINGS

contains(DEFINES, NO_DLL){
    TEMPLATE = app
	DESTDIR = bin
}else{
    TEMPLATE = lib
	DESTDIR = bin/lib
}

CONFIG += c++11

OBJECTS_DIR = build
MOC_DIR = build

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
