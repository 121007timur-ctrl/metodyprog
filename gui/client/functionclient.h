#ifndef FUNCTIONCLIENT_H
#define FUNCTIONCLIENT_H

#include <QString>

// Вариант 1: Метод половинного деления
QString bisectionMethod(double a, double b);

// Вариант 16: Метод левых прямоугольников
QString leftRectangleMethod(double a, double b, int n);

// Тестовые функции
double fBisection(double x);
double fRectangle(double x);

// ========== ЗАГЛУШКИ ДЛЯ ДРУГИХ ВАРИАНТОВ ==========
// Выводят сообщение, что вариант не выбран

QString variant2();
QString variant3();
QString variant4();
QString variant5();
QString variant6();
QString variant7();
QString variant8();
QString variant9();
QString variant10();

#endif
