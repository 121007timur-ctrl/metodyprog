#include "test_newton.h"
#include "newton.h"
#include <QtTest>
#include <cmath>

void TestNewton::polynomialValue_data()
{
    QTest::addColumn<double>("c3");
    QTest::addColumn<double>("c2");
    QTest::addColumn<double>("c1");
    QTest::addColumn<double>("c0");
    QTest::addColumn<double>("x");
    QTest::addColumn<double>("expected");

    QTest::newRow("x^3-2x-5 at 2")  << 1.0 << 0.0 << -2.0 << -5.0 << 2.0 << -1.0;
    QTest::newRow("x^2-4 at 3")     << 0.0 << 1.0 << 0.0 << -4.0 << 3.0 << 5.0;
    QTest::newRow("constant")       << 0.0 << 0.0 << 0.0 << 7.0 << 100.0 << 7.0;
    QTest::newRow("negative x")     << 2.0 << -1.0 << 3.0 << 1.0 << -1.0 << -5.0;
}

void TestNewton::polynomialValue()
{
    QFETCH(double, c3);
    QFETCH(double, c2);
    QFETCH(double, c1);
    QFETCH(double, c0);
    QFETCH(double, x);
    QFETCH(double, expected);

    Polynomial f{c3, c2, c1, c0};
    QCOMPARE(f.value(x), expected);
}

void TestNewton::polynomialDerivative()
{
    Polynomial f{1, 0, -2, -5};          // f'(x) = 3x^2 - 2
    QCOMPARE(f.derivative(2), 10.0);
    QCOMPARE(f.derivative(0), -2.0);

    Polynomial g{2, -3, 4, 1};           // g'(x) = 6x^2 - 6x + 4
    QCOMPARE(g.derivative(1), 4.0);
    QCOMPARE(g.derivative(-1), 16.0);
}

void TestNewton::polynomialToString_data()
{
    QTest::addColumn<double>("c3");
    QTest::addColumn<double>("c2");
    QTest::addColumn<double>("c1");
    QTest::addColumn<double>("c0");
    QTest::addColumn<QString>("f");
    QTest::addColumn<QString>("df");

    QTest::newRow("cubic")    << 1.0 << 0.0 << -2.0 << -5.0 << "x^3 - 2x - 5" << "3x^2 - 2";
    QTest::newRow("square")   << 0.0 << 1.0 << 5.0 << -9.0 << "x^2 + 5x - 9" << "2x + 5";
    QTest::newRow("leading-") << -1.0 << 2.0 << 0.0 << 0.0 << "-x^3 + 2x^2" << "-3x^2 + 4x";
    QTest::newRow("zero")     << 0.0 << 0.0 << 0.0 << 0.0 << "0" << "0";
    QTest::newRow("fraction") << 0.0 << 0.5 << 0.0 << 1.5 << "0.5x^2 + 1.5" << "x";
}

void TestNewton::polynomialToString()
{
    QFETCH(double, c3);
    QFETCH(double, c2);
    QFETCH(double, c1);
    QFETCH(double, c0);
    QFETCH(QString, f);
    QFETCH(QString, df);

    Polynomial p{c3, c2, c1, c0};
    QCOMPARE(p.toString(), f);
    QCOMPARE(p.derivativeToString(), df);
}

void TestNewton::firstStep_data()
{
    QTest::addColumn<double>("c3");
    QTest::addColumn<double>("c2");
    QTest::addColumn<double>("c1");
    QTest::addColumn<double>("c0");
    QTest::addColumn<double>("x0");
    QTest::addColumn<double>("x1");

    // Классический пример Ньютона: x^3 - 2x - 5, x0 = 2 -> x1 = 2 - (-1)/10 = 2.1
    QTest::newRow("x^3-2x-5")  << 1.0 << 0.0 << -2.0 << -5.0 << 2.0 << 2.1;
    // sqrt(2): x^2 - 2, x0 = 1 -> x1 = 1.5
    QTest::newRow("sqrt2")     << 0.0 << 1.0 << 0.0 << -2.0 << 1.0 << 1.5;
    // Линейная функция: корень находится за один шаг
    QTest::newRow("linear")    << 0.0 << 0.0 << 2.0 << -8.0 << 100.0 << 4.0;
    // Отрицательное x0
    QTest::newRow("neg x0")    << 0.0 << 1.0 << 0.0 << -4.0 << -1.0 << -2.5;
}

void TestNewton::firstStep()
{
    QFETCH(double, c3);
    QFETCH(double, c2);
    QFETCH(double, c1);
    QFETCH(double, c0);
    QFETCH(double, x0);
    QFETCH(double, x1);

    const NewtonStep step = newtonFirstStep(Polynomial{c3, c2, c1, c0}, x0);
    QVERIFY(step.ok);
    QVERIFY(step.error.isEmpty());
    QCOMPARE(step.x0, x0);
    QVERIFY(qAbs(step.x1 - x1) < 1e-12);
}

void TestNewton::firstStepZeroDerivative()
{
    // f(x) = x^2 - 4, f'(0) = 0 — касательная горизонтальна
    const NewtonStep step = newtonFirstStep(Polynomial{0, 1, 0, -4}, 0);
    QVERIFY(!step.ok);
    QVERIFY(step.error.contains("f'(x0) = 0"));
}

void TestNewton::firstStepRootStaysRoot()
{
    // Если x0 уже корень, x1 == x0
    const NewtonStep step = newtonFirstStep(Polynomial{0, 1, 0, -4}, 2);
    QVERIFY(step.ok);
    QCOMPARE(step.fx0, 0.0);
    QCOMPARE(step.x1, 2.0);
}

void TestNewton::stepsConvergeToRoot()
{
    // Несколько шагов подряд должны сходиться к sqrt(2)
    const Polynomial f{0, 1, 0, -2};
    double x = 1;
    for (int i = 0; i < 6; ++i) {
        const NewtonStep step = newtonFirstStep(f, x);
        QVERIFY(step.ok);
        // Невязка каждого шага не растёт
        QVERIFY(std::fabs(step.fx1) <= std::fabs(step.fx0) + 1e-15);
        x = step.x1;
    }
    QVERIFY(qAbs(x - std::sqrt(2.0)) < 1e-12);
}

void TestNewton::parseArgsValid()
{
    Polynomial f;
    double x0 = 0;
    QString error;
    QVERIFY(parseNewtonArgs({"1", "0", "-2", "-5", "2"}, f, x0, error));
    QCOMPARE(f.c3, 1.0);
    QCOMPARE(f.c2, 0.0);
    QCOMPARE(f.c1, -2.0);
    QCOMPARE(f.c0, -5.0);
    QCOMPARE(x0, 2.0);
    QVERIFY(error.isEmpty());
}

void TestNewton::parseArgsCommaDecimal()
{
    Polynomial f;
    double x0 = 0;
    QString error;
    QVERIFY(parseNewtonArgs({"0", "0,5", "0", "-1.5", "1,25"}, f, x0, error));
    QCOMPARE(f.c2, 0.5);
    QCOMPARE(f.c0, -1.5);
    QCOMPARE(x0, 1.25);
}

void TestNewton::parseArgsWrongCount()
{
    Polynomial f;
    double x0 = 0;
    QString error;
    QVERIFY(!parseNewtonArgs({"1", "2", "3"}, f, x0, error));
    QVERIFY(error.contains("5 или 6 чисел"));
    QVERIFY(!parseNewtonArgs({}, f, x0, error));
    QVERIFY(!parseNewtonArgs({"1", "2", "3", "4", "5", "6", "7"}, f, x0, error));
}

void TestNewton::parseArgsNotNumber()
{
    Polynomial f;
    double x0 = 0;
    QString error;
    QVERIFY(!parseNewtonArgs({"1", "abc", "3", "4", "5"}, f, x0, error));
    QVERIFY(error.contains("abc"));
    QVERIFY(!parseNewtonArgs({"1", "2", "3", "4", "inf"}, f, x0, error));
}

void TestNewton::commandOk()
{
    const QString r = handleNewtonCommand({"1", "0", "-2", "-5", "2"});
    QVERIFY(r.contains("f(x)  = x^3 - 2x - 5"));
    QVERIFY(r.contains("f'(x) = 3x^2 - 2"));
    QVERIFY(r.contains("f'(x0) = 10"));
    QVERIFY(r.contains("NEWTON OK x1=2.1"));
}

void TestNewton::commandError()
{
    QVERIFY(handleNewtonCommand({"1", "2"}).contains("NEWTON ERROR"));
    QVERIFY(handleNewtonCommand({"0", "1", "0", "-4", "0"}).contains("NEWTON ERROR"));
    QVERIFY(!handleNewtonCommand({"0", "1", "0", "-4", "0"}).contains("NEWTON OK"));
}

void TestNewton::generatedTaskIsSolvable()
{
    for (int i = 0; i < 200; ++i) {
        Polynomial f;
        int x0 = 0;
        generateNewtonTask(f, x0);

        QVERIFY(f.c3 != 0 || f.c2 != 0);       // степень не ниже второй
        QVERIFY(x0 >= -4 && x0 <= 4);
        QVERIFY(std::fabs(f.derivative(x0)) >= 1);
        QVERIFY(f.value(x0) != 0);             // x0 не является корнем

        const NewtonStep step = newtonFirstStep(f, x0);
        QVERIFY(step.ok);
        QVERIFY(std::isfinite(step.x1));
    }
}

void TestNewton::parseArgsWithDerivative()
{
    Polynomial f;
    double x0 = 0, dfx0 = 0;
    bool given = false;
    QString error;

    QVERIFY(parseNewtonArgs({"1", "0", "-2", "-5", "2", "10"}, f, x0, error, &given, &dfx0));
    QVERIFY(given);
    QCOMPARE(dfx0, 10.0);

    // Без шестого числа производная считается по формуле
    QVERIFY(parseNewtonArgs({"1", "0", "-2", "-5", "2"}, f, x0, error, &given, &dfx0));
    QVERIFY(!given);
    QCOMPARE(dfx0, 10.0);

    QVERIFY(!parseNewtonArgs({"1", "0", "-2", "-5", "2", "abc"}, f, x0, error, &given, &dfx0));
}

void TestNewton::firstStepGivenDerivative()
{
    // Условие варианта: дана f, f'(x0) и x0. x1 = 2 - (-1)/10 = 2.1
    const NewtonStep step = newtonFirstStep(Polynomial{1, 0, -2, -5}, 2, 10);
    QVERIFY(step.ok);
    QCOMPARE(step.dfx0, 10.0);
    QVERIFY(qAbs(step.x1 - 2.1) < 1e-12);

    // Шаг считается по заданному значению, даже если оно отличается от формулы
    const NewtonStep other = newtonFirstStep(Polynomial{1, 0, -2, -5}, 2, 5);
    QVERIFY(qAbs(other.x1 - 2.2) < 1e-12);

    QVERIFY(!newtonFirstStep(Polynomial{1, 0, -2, -5}, 2, 0).ok);
}

void TestNewton::commandWithDerivative()
{
    const QString ok = handleNewtonCommand({"1", "0", "-2", "-5", "2", "10"});
    QVERIFY(ok.contains("f'(x0) = 10 (задано)"));
    QVERIFY(!ok.contains("ВНИМАНИЕ"));
    QVERIFY(ok.contains("NEWTON OK x1=2.1"));

    const QString warn = handleNewtonCommand({"1", "0", "-2", "-5", "2", "5"});
    QVERIFY(warn.contains("ВНИМАНИЕ: по формуле f'(x0) = 10"));
    QVERIFY(warn.contains("NEWTON OK x1=2.2"));

    QVERIFY(handleNewtonCommand({"1", "0", "-2", "-5", "2", "0"}).contains("NEWTON ERROR"));
}
