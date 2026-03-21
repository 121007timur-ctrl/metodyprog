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
