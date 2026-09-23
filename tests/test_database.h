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
    void singletonIsSameInstance();
    void registerNewUser();
    void registerDuplicateLogin();
    void registerDuplicateEmail();
    void loginByLogin();
    void loginByEmail();
    void loginWrongPassword();
    void loginUnknownUser();
    void getUserLoginByEmail();
    void gameStatsUpdate();
    void gameStatsReset();
    void topPlayersSortedAndLimited();
};

#endif // TEST_DATABASE_H
