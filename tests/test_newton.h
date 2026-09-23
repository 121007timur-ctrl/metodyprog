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
    void polynomialValue_data();
    void polynomialValue();
    void polynomialDerivative();
    void polynomialToString_data();
    void polynomialToString();

    void firstStep_data();
    void firstStep();
    void firstStepZeroDerivative();
    void firstStepRootStaysRoot();
    void stepsConvergeToRoot();

    void parseArgsValid();
    void parseArgsCommaDecimal();
    void parseArgsWrongCount();
    void parseArgsNotNumber();

    void commandOk();
    void commandError();

    void generatedTaskIsSolvable();
};

#endif // TEST_NEWTON_H
