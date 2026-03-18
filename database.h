#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QMap>

class Database
{
public:
    static Database& instance();


    bool registerUser(const QString& email, const QString& login, const QString& password);
    bool loginUser(const QString& loginOrEmail, const QString& password);
    bool isLoggedIn(const QString& loginOrEmail);


    QString getUserLogin(const QString& email);
    bool userExists(const QString& loginOrEmail);


    int getTotalUsers();
    QVector<QString> getAllLogins();

private:
    Database() = default;

    struct UserData {
        QString email;
        QString login;
        QString password;
        bool isOnline;
    };

    QMap<QString, UserData> users;
    QMap<QString, QString> emailToLogin;
};

#endif // DATABASE_H
