/**
 * @file test_database.h
 * @brief Тесты синглтона Database (server/database.h): регистрация, вход, статистика.
 */
#ifndef TEST_DATABASE_H
#define TEST_DATABASE_H

#include <QObject>

/// Набор тестов для Database. Синглтон общий, поэтому каждый тест использует свои логины.
class TestDatabase : public QObject
{
    Q_OBJECT

private slots:
    void singletonIsSameInstance(); ///< instance() всегда возвращает один объект
    void registerNewUser(); ///< Регистрация нового пользователя
    void registerDuplicateLogin(); ///< Повторный логин отклоняется
    void registerDuplicateEmail(); ///< Повторный email отклоняется
    void loginByLogin(); ///< Вход по логину
    void loginByEmail(); ///< Вход по email
    void loginWrongPassword(); ///< Неверный пароль отклоняется
    void loginUnknownUser(); ///< Неизвестный пользователь не входит
    void getUserLoginByEmail(); ///< Поиск логина по email
    void gameStatsUpdate(); ///< Накопление игровой статистики
    void gameStatsReset(); ///< Сброс игровой статистики
    void topPlayersSortedAndLimited(); ///< Топ игроков отсортирован и ограничен
};

#endif // TEST_DATABASE_H
