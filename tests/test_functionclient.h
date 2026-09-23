/**
 * @file test_functionclient.h
 * @brief Тесты клиентских функций (client/functionclient.h).
 */
#ifndef TEST_FUNCTIONCLIENT_H
#define TEST_FUNCTIONCLIENT_H

#include <QObject>

/// Набор тестов для функций вариантов 1, 4, 16 на стороне клиента.
class TestFunctionClient : public QObject
{
    Q_OBJECT

private slots:
    void buildNewtonCommand();
    void parseNewtonResponseOk();
    void parseNewtonResponseError();
    void stripServerMenu();
    void clientServerRoundTrip();

    void bisectionVariant1();
    void leftRectangleVariant16();
};

#endif // TEST_FUNCTIONCLIENT_H
