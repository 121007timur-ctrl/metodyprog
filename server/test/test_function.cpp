#include <QtTest>
#include "../functionclient.h"

// ТОЛЬКО ТЕСТЫ, БЕЗ РЕАЛИЗАЦИИ ФУНКЦИЙ 

class TestMathFunctions : public QObject
{
    Q_OBJECT

private slots:
    // Тест для метода половинного деления
    void testBisectionMethod()
    {
        QString result1 = bisectionMethod(0, 3);
        QVERIFY(!result1.contains("Ошибка"));
        QVERIFY(result1.contains("МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ"));

        QString result2 = bisectionMethod(0, 1);
        QVERIFY(result2.contains("Ошибка"));

        QString result3 = bisectionMethod(1, 3);
        QVERIFY(result3.contains("Середина: 2"));
    }

    // Тест для метода левых прямоугольников
    void testLeftRectangleMethod()
    {
        QString result1 = leftRectangleMethod(0, 2, 100);
        QVERIFY(!result1.contains("Ошибка"));
        QVERIFY(result1.contains("МЕТОД ЛЕВЫХ ПРЯМОУГОЛЬНИКОВ"));

        QString result2 = leftRectangleMethod(0, 2, 0);
        QVERIFY(result2.contains("Ошибка"));
    }

    // Тест для заглушек
    void testStubs()
    {
        QCOMPARE(variant2(), QString("[ЗАГЛУШКА] Вариант 2 не выбран"));
        QCOMPARE(variant3(), QString("[ЗАГЛУШКА] Вариант 3 не выбран"));
        QCOMPARE(variant4(), QString("[ЗАГЛУШКА] Вариант 4 не выбран"));
        QCOMPARE(variant5(), QString("[ЗАГЛУШКА] Вариант 5 не выбран"));
        QCOMPARE(variant6(), QString("[ЗАГЛУШКА] Вариант 6 не выбран"));
        QCOMPARE(variant7(), QString("[ЗАГЛУШКА] Вариант 7 не выбран"));
        QCOMPARE(variant8(), QString("[ЗАГЛУШКА] Вариант 8 не выбран"));
        QCOMPARE(variant9(), QString("[ЗАГЛУШКА] Вариант 9 не выбран"));
        QCOMPARE(variant10(), QString("[ЗАГЛУШКА] Вариант 10 не выбран"));
    }

    void testBisectionOutputFormat()
    {
        QString result = bisectionMethod(1, 3);
        QVERIFY(result.contains("МЕТОД ПОЛОВИННОГО ДЕЛЕНИЯ"));
        QVERIFY(result.contains("Отрезок:"));
    }

    void testRectangleMethodConvergence()
    {
        QString result10 = leftRectangleMethod(0, 2, 10);
        QString result100 = leftRectangleMethod(0, 2, 100);

        QVERIFY(!result10.contains("Ошибка"));
        QVERIFY(!result100.contains("Ошибка"));
    }
};

// Запуск тестов
QTEST_MAIN(TestMathFunctions)
#include "test_function.moc"
