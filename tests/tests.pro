# Юнит-тесты проекта (Qt Test).
# Сборка:  qmake tests.pro && make   (Windows + MinGW: mingw32-make)
# Запуск:  ./unit_tests  — код возврата 0, если все тесты прошли.

QT += testlib network
QT -= gui
CONFIG += c++17 console testcase
CONFIG -= app_bundle
TARGET = unit_tests

INCLUDEPATH += ../server ../client

SOURCES += \
    test_main.cpp \
    test_newton.cpp \
    test_database.cpp \
    test_functionclient.cpp \
    ../server/newton.cpp \
    ../server/database.cpp \
    ../client/functionclient.cpp

HEADERS += \
    test_newton.h \
    test_database.h \
    test_functionclient.h
