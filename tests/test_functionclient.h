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
    void buildNewtonCommand(); ///< Формирование команды NEWTON
    void parseNewtonResponseOk(); ///< Разбор успешного ответа
    void parseNewtonResponseError(); ///< Разбор ответа с ошибкой
    void stripServerMenu(); ///< Удаление главного меню из ответа
    void clientServerRoundTrip(); ///< Цикл клиент → сервер → клиент

    void bisectionVariant1(); ///< Вариант 1 (дихотомия)
    void leftRectangleVariant16(); ///< Вариант 16 (левые прямоугольники)
};

#endif // TEST_FUNCTIONCLIENT_H
