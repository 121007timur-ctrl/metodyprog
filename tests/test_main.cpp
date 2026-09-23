/**
 * @file test_main.cpp
 * @brief Точка входа юнит-тестов: запускает все наборы Qt Test подряд.
 *
 * Если задана переменная окружения TEST_LOG_DIR, отчёт каждого набора
 * дополнительно пишется в файл <TEST_LOG_DIR>/<ИмяНабора>.txt.
 */
#include <QCoreApplication>
#include <QDir>
#include <QtTest>

#include "test_newton.h"
#include "test_database.h"
#include "test_functionclient.h"

/// Запускает один набор тестов, возвращает число упавших тестов.
static int run(QObject* suite, const QStringList& baseArgs)
{
    QStringList args = baseArgs;
    const QString logDir = qEnvironmentVariable("TEST_LOG_DIR");
    if (!logDir.isEmpty()) {
        QDir().mkpath(logDir);
        const QString file = logDir + "/" + suite->metaObject()->className() + ".txt";
        args << "-o" << file + ",txt" << "-o" << "-,txt";
    }
    return QTest::qExec(suite, args);
}

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    const QStringList args = app.arguments();

    TestNewton newton;
    TestDatabase database;
    TestFunctionClient functionClient;

    int failed = 0;
    failed += run(&newton, args);
    failed += run(&database, args);
    failed += run(&functionClient, args);

    return failed == 0 ? 0 : 1;
}
