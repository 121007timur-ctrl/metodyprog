/**
 * @file functionclient.h
 * @brief Функции вариантов на стороне клиента (численные методы).
 */
#ifndef FUNCTIONCLIENT_H
#define FUNCTIONCLIENT_H

#include <QString>

/**
 * @brief Вариант 1: одна итерация метода половинного деления для f(x) = x^2 - 4.
 * @param a Левая граница отрезка.
 * @param b Правая граница отрезка.
 * @return Новые границы отрезка или сообщение об ошибке, если f не меняет знак.
 */
QString bisectionMethod(double a, double b);

/**
 * @brief Вариант 16: интеграл x^2 методом левых прямоугольников.
 * @param a Нижний предел.
 * @param b Верхний предел.
 * @param n Число отрезков разбиения (n > 0).
 * @return Значение интеграла или сообщение об ошибке.
 */
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

/**
 * @brief Функция для варианта 1: f(x) = x^2 - 4.
 * @param x Аргумент.
 * @return f(x).
 */
double fBisection(double x);

/**
 * @brief Подынтегральная функция для варианта 16: f(x) = x^2.
 * @param x Аргумент.
 * @return f(x).
 */
double fRectangle(double x);

/**
 * @name Заглушки для вариантов, которые никто не выбрал
 * Каждая возвращает строку "[ЗАГЛУШКА] Вариант N не выбран".
 * @{
 */
/** @brief Заглушка варианта 2. @return "[ЗАГЛУШКА] Вариант 2 не выбран". */
QString variant2();
/** @brief Заглушка варианта 3. @return "[ЗАГЛУШКА] Вариант 3 не выбран". */
QString variant3();
/** @brief Заглушка варианта 5. @return "[ЗАГЛУШКА] Вариант 5 не выбран". */
QString variant5();
/** @brief Заглушка варианта 6. @return "[ЗАГЛУШКА] Вариант 6 не выбран". */
QString variant6();
/** @brief Заглушка варианта 7. @return "[ЗАГЛУШКА] Вариант 7 не выбран". */
QString variant7();
/** @brief Заглушка варианта 8. @return "[ЗАГЛУШКА] Вариант 8 не выбран". */
QString variant8();
/** @brief Заглушка варианта 9. @return "[ЗАГЛУШКА] Вариант 9 не выбран". */
QString variant9();
/** @brief Заглушка варианта 10. @return "[ЗАГЛУШКА] Вариант 10 не выбран". */
QString variant10();
/** @} */

#endif
