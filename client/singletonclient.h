#ifndef SINGLETONCLIENT_H
#define SINGLETONCLIENT_H

#include <QString>
#include <QDebug>
#include <QTcpSocket>

class SingletonClient;

class SingletonClientDestroyer
{
private:
    SingletonClient* p_instance;
public:
    ~SingletonClientDestroyer() { delete p_instance; }
    void initialize(SingletonClient* p) { p_instance = p; }
};

class SingletonClient
{
private:
    static SingletonClient* p_instance;
    static SingletonClientDestroyer destroyer;

    SingletonClient() { qDebug() << "SingletonClient СОЗДАН"; }
    SingletonClient(const SingletonClient&) = delete;
    SingletonClient& operator=(const SingletonClient&) = delete;
    ~SingletonClient() { qDebug() << "SingletonClient УНИЧТОЖЕН"; }
    friend class SingletonClientDestroyer;

    //решить какие функции реально нужны или же удалить
        QString m_userLogin;
        bool m_isLoggedIn;
    QTcpSocket* m_socket;
        bool m_connected;

public:
    static SingletonClient* getInstance();
            void setUserLogin(const QString& login);
            QString getUserLogin() const;
            bool isLoggedIn() const;
            void setLoggedIn(bool status);
            void showStatus() const;
            void login(const QString& login);
            void logout();
    bool connectToServer(const QString& host, int port);
    void disconnectFromServer();
    bool sendCommand(const QString& command);
    QString waitForResponse();
            void processUserInput();
            bool isConnected() const { return m_connected; }
};

#endif
