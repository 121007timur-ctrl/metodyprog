#include "test_database.h"
#include "database.h"
#include <QtTest>

void TestDatabase::singletonIsSameInstance()
{
    QCOMPARE(&Database::instance(), &Database::instance());
}

void TestDatabase::registerNewUser()
{
    Database& db = Database::instance();
    const int before = db.getTotalUsers();

    QVERIFY(db.registerUser("reg@test.ru", "reg_user", "secret"));
    QCOMPARE(db.getTotalUsers(), before + 1);
    QVERIFY(db.userExists("reg_user"));
    QVERIFY(db.userExists("reg@test.ru"));
    QVERIFY(db.getAllLogins().contains("reg_user"));
}

void TestDatabase::registerDuplicateLogin()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("dup1@test.ru", "dup_login", "p"));
    QVERIFY(!db.registerUser("dup2@test.ru", "dup_login", "p"));
}

void TestDatabase::registerDuplicateEmail()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("same@test.ru", "email_user1", "p"));
    QVERIFY(!db.registerUser("same@test.ru", "email_user2", "p"));
    QVERIFY(!db.userExists("email_user2"));
}

void TestDatabase::loginByLogin()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("l1@test.ru", "login_user", "pass123"));
    QVERIFY(!db.isLoggedIn("login_user"));
    QVERIFY(db.loginUser("login_user", "pass123"));
    QVERIFY(db.isLoggedIn("login_user"));
}

void TestDatabase::loginByEmail()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("mail@test.ru", "mail_user", "pass123"));
    QVERIFY(db.loginUser("mail@test.ru", "pass123"));
    QVERIFY(db.isLoggedIn("mail@test.ru"));
}

void TestDatabase::loginWrongPassword()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("wp@test.ru", "wp_user", "right"));
    QVERIFY(!db.loginUser("wp_user", "wrong"));
    QVERIFY(!db.loginUser("wp_user", ""));
    QVERIFY(!db.isLoggedIn("wp_user"));
}

void TestDatabase::loginUnknownUser()
{
    Database& db = Database::instance();
    QVERIFY(!db.loginUser("nobody", "x"));
    QVERIFY(!db.loginUser("nobody@test.ru", "x"));
    QVERIFY(!db.userExists("nobody"));
}

void TestDatabase::getUserLoginByEmail()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("find@test.ru", "find_user", "p"));
    QCOMPARE(db.getUserLogin("find@test.ru"), QString("find_user"));
    QCOMPARE(db.getUserLogin("missing@test.ru"), QString());
}

void TestDatabase::gameStatsUpdate()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("g@test.ru", "gamer", "p"));

    db.updateGameStats("gamer", 7, true);
    db.updateGameStats("gamer", 3, true);
    db.updateGameStats("gamer", 10, false);

    const GameStatsData s = db.getUserGameStats("gamer");
    QCOMPARE(s.totalAttempts, 20);
    QCOMPARE(s.totalCorrect, 2);
    QCOMPARE(s.gamesPlayed, 3);
    QCOMPARE(s.bestGameAttempts, 3);

    // Для неизвестного пользователя — нули
    const GameStatsData none = db.getUserGameStats("ghost");
    QCOMPARE(none.totalAttempts, 0);
    QCOMPARE(none.gamesPlayed, 0);
}

void TestDatabase::gameStatsReset()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("r@test.ru", "resetter", "p"));
    db.updateGameStats("resetter", 5, true);
    db.resetUserGameStats("resetter");

    const GameStatsData s = db.getUserGameStats("resetter");
    QCOMPARE(s.totalAttempts, 0);
    QCOMPARE(s.totalCorrect, 0);
    QCOMPARE(s.gamesPlayed, 0);
}

void TestDatabase::topPlayersSortedAndLimited()
{
    Database& db = Database::instance();
    QVERIFY(db.registerUser("t1@test.ru", "top_a", "p"));
    QVERIFY(db.registerUser("t2@test.ru", "top_b", "p"));
    for (int i = 0; i < 5; ++i) db.updateGameStats("top_a", 1, true);
    for (int i = 0; i < 9; ++i) db.updateGameStats("top_b", 1, true);

    const auto top = db.getTopPlayers(2);
    QCOMPARE(top.size(), 2);
    QCOMPARE(top[0].first, QString("top_b"));
    QCOMPARE(top[0].second, 9);
    QVERIFY(top[0].second >= top[1].second);
}
