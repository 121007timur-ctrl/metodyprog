#DEFINES += RUN_TESTS

QT += core gui network widgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

# Базовые файлы (всегда)
SOURCES += \
    authregform.cpp \
    mainwindow.cpp \
    clientapi.cpp \
    functionclient.cpp \
    #test/test_clientapi_logic.cpp \
    #test/test_validation.cpp \
    #test/test_function.cpp

HEADERS += \
    authregform.h \
    mainwindow.h \
    clientapi.h \
    functionclient.h

FORMS += \
    authregform.ui \
    mainwindow.ui

# Включаем тесты или программу
contains(DEFINES, RUN_TESTS) {
    # Режим ТЕСТОВ
    message("Building TESTS mode")
    SOURCES += #test/test_function.cpp
    # Убираем main.cpp (если он есть)
    SOURCES -= main.cpp
} else {
    # Режим ПРОГРАММЫ
    message("Building APPLICATION mode")
    SOURCES += main.cpp
}

