/**
 * @file newton.h
 * @brief Вариант 4: первое приближение корня методом Ньютона (касательных).
 * @author Далаков Аслан, 251-952
 *
 * Функция задаётся многочленом третьей степени
 * f(x) = c3*x^3 + c2*x^2 + c1*x + c0, её производная
 * f'(x) = 3*c3*x^2 + 2*c2*x + c1 вычисляется аналитически.
 *
 * Первое приближение считается по формуле
 * @f[ x_1 = x_0 - \frac{f(x_0)}{f'(x_0)} @f]
 */
#ifndef NEWTON_H
#define NEWTON_H

#include <QString>
#include <QStringList>

/**
 * @brief Многочлен не выше третьей степени: c3*x^3 + c2*x^2 + c1*x + c0.
 */
struct Polynomial {
    double c3 = 0; ///< Коэффициент при x^3
    double c2 = 0; ///< Коэффициент при x^2
    double c1 = 0; ///< Коэффициент при x
    double c0 = 0; ///< Свободный член

    /**
     * @brief Значение многочлена в точке.
     * @param x Точка.
     * @return f(x).
     */
    double value(double x) const;

    /**
     * @brief Значение производной многочлена в точке.
     * @param x Точка.
     * @return f'(x).
     */
    double derivative(double x) const;

    /**
     * @brief Текстовая запись многочлена, например "x^3 - 2x - 5".
     * @return Строка с многочленом.
     */
    QString toString() const;

    /**
     * @brief Текстовая запись производной, например "3x^2 - 2".
     * @return Строка с производной.
     */
    QString derivativeToString() const;
};

/**
 * @brief Результат одного шага метода Ньютона.
 */
struct NewtonStep {
    bool ok = false;  ///< true, если шаг выполнен
    QString error;    ///< Текст ошибки, если ok == false
    double x0 = 0;    ///< Начальное приближение
    double fx0 = 0;   ///< f(x0)
    double dfx0 = 0;  ///< f'(x0)
    double x1 = 0;    ///< Первое приближение x1
    double fx1 = 0;   ///< f(x1) — невязка после шага
};

/**
 * @brief Выполняет один шаг метода Ньютона.
 * @param f  Многочлен.
 * @param x0 Начальное приближение.
 * @return Результат шага. Если f'(x0) = 0 (касательная параллельна оси OX),
 *         возвращается ok == false и описание ошибки.
 */
NewtonStep newtonFirstStep(const Polynomial& f, double x0);

/**
 * @brief Шаг метода Ньютона с заданным значением производной.
 *
 * Соответствует условию варианта 4 «дана функция и её производная»:
 * f'(x0) берётся из входных данных, а не вычисляется.
 * @param f    Многочлен.
 * @param x0   Начальное приближение.
 * @param dfx0 Значение производной в точке x0.
 * @return Результат шага; ok == false, если dfx0 = 0.
 */
NewtonStep newtonFirstStep(const Polynomial& f, double x0, double dfx0);

/**
 * @brief Разбирает аргументы команды NEWTON.
 *
 * Формат: `c3 c2 c1 c0 x0 [f'(x0)]` — пять или шесть чисел через пробел,
 * десятичный разделитель — точка или запятая. Шестое число — заданное
 * значение производной в x0; если его нет, производная вычисляется по формуле.
 *
 * @param args          Аргументы команды (без слова NEWTON).
 * @param f             [out] Многочлен.
 * @param x0            [out] Начальное приближение.
 * @param error         [out] Текст ошибки разбора.
 * @param hasDerivative [out, необязательно] true, если f'(x0) задано явно.
 * @param dfx0          [out, необязательно] f'(x0): заданное или вычисленное.
 * @return true, если аргументы прочитаны.
 */
bool parseNewtonArgs(const QStringList& args, Polynomial& f, double& x0, QString& error,
                     bool* hasDerivative = nullptr, double* dfx0 = nullptr);

/**
 * @brief Обработчик серверной команды `NEWTON c3 c2 c1 c0 x0 [f'(x0)]`.
 *
 * Если f'(x0) задано и не совпадает с вычисленным по формуле, в ответ
 * добавляется предупреждение, но шаг считается по заданному значению.
 * @param args Аргументы команды.
 * @return Текстовый ответ сервера с f(x0), f'(x0), x1 и f(x1).
 */
QString handleNewtonCommand(const QStringList& args);

/**
 * @brief Сгенерировать случайное задание варианта 4 (для команды TASK4).
 * @param f  [out] Многочлен задания.
 * @param x0 [out] Начальное приближение.
 *
 * Коэффициенты и x0 целые, f'(x0) != 0, ответ x1 получается конечной
 * десятичной дробью.
 */
void generateNewtonTask(Polynomial& f, int& x0);

#endif // NEWTON_H
