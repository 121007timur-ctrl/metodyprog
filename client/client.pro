QT -= gui
QT += core network

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    client_main.cpp \
    singletonclient.cpp

HEADERS += \
    singletonclient.h

DESTDIR = $$PWD/../bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
TARGET = client