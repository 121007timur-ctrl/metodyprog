QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    mytcpserver.cpp \
    database.cpp \
    server_functions.cpp

HEADERS += \
    mytcpserver.h \
    database.h \
    server_functions.h
