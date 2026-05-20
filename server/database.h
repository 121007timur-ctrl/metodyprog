#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QMap>
#include <QVector>

struct GameStatsData {
    int totalAttempts;
    int totalCorrect;
    int gamesPlayed;
    int bestGameAttempts;
};

class Database
{
public:
    static Database& instance();

    // Регистрация и авторизация
    bool registerUser(const QString& email, const QString& login, const QString& password);
    bool loginUser(const QString& loginOrEmail, const QString& password);
    bool isLoggedIn(const QString& loginOrEmail);
    QString getUserLogin(const QString& email);
    bool userExists(const QString& loginOrEmail);

    // Статистика сервера
    int getTotalUsers();
    QVector<QString> getAllLogins();

    // Игровая статистика
    void updateGameStats(const QString& login, int attempts, bool success);
    GameStatsData getUserGameStats(const QString& login);
    void resetUserGameStats(const QString& login);
    QVector<QPair<QString, int>> getTopPlayers(int limit = 10);

private:
    Database() = default;

    struct UserData {
        QString email;
        QString login;
        QString password;
        bool isOnline;
        GameStatsData gameStats;
    };

    QMap<QString, UserData> users;
    QMap<QString, QString> emailToLogin;
};

#endif // DATABASE_H
