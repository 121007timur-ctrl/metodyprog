/**
 * @file test_newton.h
 * @brief Тесты варианта 4 — метод Ньютона (server/newton.h).
 */
#ifndef TEST_NEWTON_H
#define TEST_NEWTON_H

#include <QObject>

/// Набор тестов для Polynomial, newtonFirstStep, parseNewtonArgs и handleNewtonCommand.
class TestNewton : public QObject
{
    Q_OBJECT

private slots:
    void polynomialValue_data(); ///< Данные: коэффициенты, точка, ожидаемое значение
    void polynomialValue(); ///< f(x) для разных многочленов
    void polynomialDerivative(); ///< f'(x) считается верно
    void polynomialToString_data(); ///< Данные: многочлен и его запись
    void polynomialToString(); ///< Текстовая запись f и f'

    void firstStep_data(); ///< Данные: многочлен, x0 и ожидаемое x1
    void firstStep(); ///< x1 = x0 - f(x0)/f'(x0) на известных примерах
    void firstStepZeroDerivative(); ///< Ошибка при f'(x0) = 0
    void firstStepRootStaysRoot(); ///< Если x0 — корень, то x1 = x0
    void stepsConvergeToRoot(); ///< Повторные шаги сходятся к sqrt(2)

    void parseArgsValid(); ///< Разбор пяти чисел
    void parseArgsCommaDecimal(); ///< Запятая как десятичный разделитель
    void parseArgsWrongCount(); ///< Ошибка при неверном числе аргументов
    void parseArgsNotNumber(); ///< Ошибка при нечисловом аргументе

    void commandOk(); ///< Ответ сервера на корректную команду NEWTON
    void commandError(); ///< Ответ сервера на ошибочную команду NEWTON

    void generatedTaskIsSolvable(); ///< Задание TASK4 всегда корректно (200 генераций)

    void parseArgsWithDerivative();  ///< Шестой аргумент — заданная f'(x0)
    void firstStepGivenDerivative(); ///< Шаг по заданной производной
    void commandWithDerivative();    ///< Ответ NEWTON с заданной f'(x0), предупреждение о несовпадении
};

#endif // TEST_NEWTON_H
