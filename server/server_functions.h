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


extern QMap<QTcpSocket*, ClientInfo> g_clients;
extern QMap<QString, QTcpSocket*> g_logins;
extern int g_clientCounter;


QString generateClientId();
void sendAuthMenu(QTcpSocket* socket);
void sendMainMenu(QTcpSocket* socket);
QString handleAuthSteps(QTcpSocket* socket, ClientInfo& info, const QString& request);
QString handleCommands(QTcpSocket* socket, ClientInfo& info, const QString& command);
void cleanupClient(QTcpSocket* socket);


QString getStats();


QString task1Handler();
QString task2Handler();

#endif // SERVER_FUNCTIONS_H
