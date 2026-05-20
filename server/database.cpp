#include "database.h"
#include <QDebug>

Database& Database::instance()
{
    static Database db;
    return db;
}

bool Database::registerUser(const QString& email, const QString& login, const QString& password)
{
    if (users.contains(login)) {
        qDebug() << "Register failed: login" << login << "already exists";
        return false;
    }

    if (emailToLogin.contains(email)) {
        qDebug() << "Register failed: email" << email << "already registered";
        return false;
    }

    UserData newUser;
    newUser.email = email;
    newUser.login = login;
    newUser.password = password;
    newUser.isOnline = false;
    newUser.gameStats.totalAttempts = 0;
    newUser.gameStats.totalCorrect = 0;
    newUser.gameStats.gamesPlayed = 0;
    newUser.gameStats.bestGameAttempts = 999999;

    users[login] = newUser;
    emailToLogin[email] = login;

    qDebug() << "User registered:" << login << "with email:" << email;
    return true;
}

bool Database::loginUser(const QString& loginOrEmail, const QString& password)
{
    QString login;

    if (loginOrEmail.contains('@')) {
        if (!emailToLogin.contains(loginOrEmail)) {
            qDebug() << "Login failed: email" << loginOrEmail << "not found";
            return false;
        }
        login = emailToLogin[loginOrEmail];
    } else {
        login = loginOrEmail;
    }

    if (!users.contains(login)) {
        qDebug() << "Login failed: login" << login << "not found";
        return false;
    }

    if (users[login].password != password) {
        qDebug() << "Login failed: wrong password for" << login;
        return false;
    }

    users[login].isOnline = true;
    qDebug() << "User logged in:" << login;
    return true;
}

bool Database::isLoggedIn(const QString& loginOrEmail)
{
    QString login;

    if (loginOrEmail.contains('@')) {
        if (!emailToLogin.contains(loginOrEmail)) return false;
        login = emailToLogin[loginOrEmail];
    } else {
        login = loginOrEmail;
    }

    return users.contains(login) && users[login].isOnline;
}

QString Database::getUserLogin(const QString& email)
{
    return emailToLogin.value(email, "");
}

bool Database::userExists(const QString& loginOrEmail)
{
    if (loginOrEmail.contains('@')) {
        return emailToLogin.contains(loginOrEmail);
    } else {
        return users.contains(loginOrEmail);
    }
}

int Database::getTotalUsers()
{
    return users.size();
}

QVector<QString> Database::getAllLogins()
{
    return users.keys();
}

void Database::updateGameStats(const QString& login, int attempts, bool success)
{
    if (!users.contains(login)) return;

    UserData& user = users[login];
    user.gameStats.totalAttempts += attempts;

    if (success) {
        user.gameStats.totalCorrect++;
        if (attempts < user.gameStats.bestGameAttempts) {
            user.gameStats.bestGameAttempts = attempts;
        }
    }

    user.gameStats.gamesPlayed++;
    qDebug() << "Game stats updated for:" << login << "attempts:" << attempts << "success:" << success;
}

GameStatsData Database::getUserGameStats(const QString& login)
{
    GameStatsData empty = {0, 0, 0, 0};
    if (!users.contains(login)) return empty;
    return users[login].gameStats;
}

void Database::resetUserGameStats(const QString& login)
{
    if (!users.contains(login)) return;

    users[login].gameStats.totalAttempts = 0;
    users[login].gameStats.totalCorrect = 0;
    users[login].gameStats.gamesPlayed = 0;
    users[login].gameStats.bestGameAttempts = 999999;

    qDebug() << "Game stats reset for:" << login;
}

QVector<QPair<QString, int>> Database::getTopPlayers(int limit)
{
    QVector<QPair<QString, int>> topPlayers;

    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it.value().gameStats.totalCorrect > 0) {
            topPlayers.append(qMakePair(it.key(), it.value().gameStats.totalCorrect));
        }
    }

    std::sort(topPlayers.begin(), topPlayers.end(),
              [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                  return a.second > b.second;
              });

    if (topPlayers.size() > limit) {
        topPlayers.resize(limit);
    }

    return topPlayers;
}
