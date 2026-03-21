QT -= gui
QT += network sql

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    mytcpserver.cpp \
    server_functions.cpp \
    database.cpp

HEADERS += \
    mytcpserver.h \
    server_functions.h \
    database.h

DESTDIR = $$PWD/../bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
TARGET = echoServer