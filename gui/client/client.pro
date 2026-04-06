QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    authregform.cpp \
    mainwindow.cpp \
    clientapi.cpp \
    functionclient.cpp

HEADERS += \
    authregform.h \
    mainwindow.h \
    clientapi.h \
    functionclient.h

FORMS += \
    authregform.ui \
    mainwindow.ui
