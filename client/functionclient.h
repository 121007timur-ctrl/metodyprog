#ifndef FUNCTIONCLIENT_H
#define FUNCTIONCLIENT_H

#include <QString>

// Вариант 1: Метод половинного деления
QString bisectionMethod(double a, double b);

// Вариант 16: Метод левых прямоугольников
QString leftRectangleMethod(double a, double b, int n);

/**
 * @name Вариант 4: метод Ньютона (Далаков Аслан, 251-952)
 * Вычисление выполняет сервер (команда NEWTON), клиент формирует
 * запрос и разбирает ответ.
 * @{
 */

/**
 * @brief Сформировать команду для сервера.
 *
 * Функция задаётся многочленом f(x) = c3*x^3 + c2*x^2 + c1*x + c0.
 * @param c3 Коэффициент при x^3.
 * @param c2 Коэффициент при x^2.
 * @param c1 Коэффициент при x.
 * @param c0 Свободный член.
 * @param x0 Начальное приближение.
 * @return Строка вида "NEWTON 1 0 -2 -5 2".
 */
QString buildNewtonCommand(double c3, double c2, double c1, double c0, double x0);

/**
 * @brief Достать x1 из ответа сервера на команду NEWTON.
 * @param response Ответ сервера.
 * @param x1 [out] Первое приближение.
 * @return true, если в ответе есть строка "NEWTON OK x1=...".
 */
bool parseNewtonResponse(const QString& response, double& x1);

/**
 * @brief Убрать из ответа сервера главное меню, которое сервер
 *        дописывает после каждой команды.
 * @param response Ответ сервера.
 * @return Ответ без блока "=== ГЛАВНОЕ МЕНЮ ===".
 */
QString stripServerMenu(const QString& response);

/** @} */

// Тестовые функции
double fBisection(double x);
double fRectangle(double x);

// ========== ЗАГЛУШКИ ДЛЯ ДРУГИХ ВАРИАНТОВ ==========
// Выводят сообщение, что вариант не выбран

QString variant2();
QString variant3();
QString variant5();
QString variant6();
QString variant7();
QString variant8();
QString variant9();
QString variant10();

#endif
