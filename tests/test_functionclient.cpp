#include "test_functionclient.h"
#include "functionclient.h"
#include "newton.h"
#include <QtTest>

void TestFunctionClient::buildNewtonCommand()
{
    QCOMPARE(::buildNewtonCommand(1, 0, -2, -5, 2), QString("NEWTON 1 0 -2 -5 2"));
    QCOMPARE(::buildNewtonCommand(0, 0.5, 0, -1.25, 1.5), QString("NEWTON 0 0.5 0 -1.25 1.5"));
}

void TestFunctionClient::parseNewtonResponseOk()
{
    double x1 = 0;
    QVERIFY(parseNewtonResponse("...\r\nNEWTON OK x1=2.1\r\n", x1));
    QCOMPARE(x1, 2.1);
    QVERIFY(parseNewtonResponse("NEWTON OK x1=-0.5", x1));
    QCOMPARE(x1, -0.5);
}

void TestFunctionClient::parseNewtonResponseError()
{
    double x1 = 42;
    QVERIFY(!parseNewtonResponse("NEWTON ERROR: f'(x0) = 0", x1));
    QVERIFY(!parseNewtonResponse("", x1));
    QVERIFY(!parseNewtonResponse("NEWTON OK x1=abc", x1));
}

void TestFunctionClient::stripServerMenu()
{
    QCOMPARE(::stripServerMenu("\r\nОтвет\r\n\r\n=== ГЛАВНОЕ МЕНЮ ===\r\nкоманды\r\n> "),
             QString("Ответ"));
    QCOMPARE(::stripServerMenu("  без меню  "), QString("без меню"));
}

void TestFunctionClient::clientServerRoundTrip()
{
    // Команда клиента -> разбор на сервере -> ответ сервера -> разбор на клиенте
    const QString command = ::buildNewtonCommand(0, 1, 0, -2, 1);
    const QStringList args = command.mid(6).split(' ', Qt::SkipEmptyParts);
    const QString response = handleNewtonCommand(args);

    double x1 = 0;
    QVERIFY(parseNewtonResponse(response, x1));
    QCOMPARE(x1, 1.5);
}

void TestFunctionClient::bisectionVariant1()
{
    QVERIFY(bisectionMethod(1, 3).contains("Середина: 2"));
    QVERIFY(bisectionMethod(0, 1).contains("Ошибка"));
}

void TestFunctionClient::leftRectangleVariant16()
{
    QVERIFY(leftRectangleMethod(0, 2, 0).contains("Ошибка"));
    QVERIFY(leftRectangleMethod(0, 2, 100).contains("n=100"));
}
