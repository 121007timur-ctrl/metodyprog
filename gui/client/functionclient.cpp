#include "functionclient.h"
#include <cmath>

// Тестовые функции
double fBisection(double x) { return x * x - 4; }
double fRectangle(double x) { return x * x; }

// ========== ВАРИАНТ 1 ==========
QString bisectionMethod(double a, double b)
{
    double fa = fBisection(a);
    double fb = fBisection(b);

    if (fa * fb >= 0) {
        return "Ошибка: Функция не меняет знак на отрезке";
    }

    double c = (a + b) / 2;
    double fc = fBisection(c);

    double newA = a, newB = b;
    if (fa * fc < 0) newB = c;
    else newA = c;

    return QString(
               "МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ (Вариант 1)\n"
               "Отрезок: [%1, %2] -> [%3, %4]\n"
               "Середина: %5, f(c)=%6\n"
               "Длина нового отрезка: %7"
               ).arg(a).arg(b).arg(newA).arg(newB).arg(c).arg(fc).arg(newB - newA);
}

// ========== ВАРИАНТ 16 ==========
QString leftRectangleMethod(double a, double b, int n)
{
    if (n <= 0) return "Ошибка: n > 0";

    double h = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        sum += fRectangle(a + i * h);
    }

    double integral = sum * h;

    return QString(
               "МЕТОД ЛЕВЫХ ПРЯМОУГОЛЬНИКОВ (Вариант 16)\n"
               "Интеграл ∫[%1, %2] x^2 dx ≈ %3\n"
               "n=%4, h=%5"
               ).arg(a).arg(b).arg(integral).arg(n).arg(h);
}

// ========== ЗАГЛУШКИ ==========
QString variant2() { return "[ЗАГЛУШКА] Вариант 2 не выбран"; }
QString variant3() { return "[ЗАГЛУШКА] Вариант 3 не выбран"; }
QString variant4() { return "[ЗАГЛУШКА] Вариант 4 не выбран"; }
QString variant5() { return "[ЗАГЛУШКА] Вариант 5 не выбран"; }
QString variant6() { return "[ЗАГЛУШКА] Вариант 6 не выбран"; }
QString variant7() { return "[ЗАГЛУШКА] Вариант 7 не выбран"; }
QString variant8() { return "[ЗАГЛУШКА] Вариант 8 не выбран"; }
QString variant9() { return "[ЗАГЛУШКА] Вариант 9 не выбран"; }
QString variant10() { return "[ЗАГЛУШКА] Вариант 10 не выбран"; }
