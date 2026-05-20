#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <QString>
#include <QTcpSocket>
#include <QMap>

struct ClientInfo {
    QString clientId;
    QString login;
    bool isAuthenticated;
    int authStep;
    QString tempEmail;
    QString tempLogin;
};

struct GameSession {
    int secretNumber;
    int attempts;
    int minRange;
    int maxRange;
    bool gameActive;
    int currentNumber;
};

// Структура для статистики заданий
struct TaskStats {
    int totalAttempts;      // Всего решено заданий
    int correctAnswers;     // Правильных ответов
};

extern QMap<QTcpSocket*, ClientInfo> g_clients;
extern QMap<QString, QTcpSocket*> g_logins;
extern QMap<QTcpSocket*, GameSession> g_gameSessions;
extern QMap<QTcpSocket*, TaskStats> g_taskStats;  // Статистика заданий
extern int g_clientCounter;

QString generateClientId();
void sendAuthMenu(QTcpSocket* socket);
void sendMainMenu(QTcpSocket* socket);
QString handleAuthSteps(QTcpSocket* socket, ClientInfo& info, const QString& request);
QString handleCommands(QTcpSocket* socket, ClientInfo& info, const QString& command);
void cleanupClient(QTcpSocket* socket);

QString getStats();
QString task1Handler(QTcpSocket* socket, const QString& answer = "");
QString task2Handler(QTcpSocket* socket, const QString& answer = "");

QString handleStartGame(QTcpSocket* socket);
QString handleGuess(QTcpSocket* socket, const QString& guess, const QString& login);
QString handleGameStats(QTcpSocket* socket, const QString& login);
QString handleHint(QTcpSocket* socket);

#endif // SERVER_FUNCTIONS_H
