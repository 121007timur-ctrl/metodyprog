#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QString>
#include <QTcpSocket>

// Singleton для работы с сервером
class ClientAPI : public QObject
{
    Q_OBJECT

private:
    static ClientAPI* m_instance;
    QTcpSocket* m_socket;
    bool m_connected;
    QString m_userLogin;
    bool m_isLoggedIn;

    ClientAPI(QObject* parent = nullptr);
    ~ClientAPI();

public:
    static ClientAPI* getInstance();

    // Сеть
    bool connectToServer(const QString& host = "127.0.0.1", int port = 33333);
    void disconnectFromServer();
    bool isConnected() const { return m_connected; }
    QString sendAndReceive(const QString& message, int timeoutMs = 5000);

    // Авторизация
    QString registerUser(const QString& login, const QString& email, const QString& password);
    QString loginUser(const QString& login, const QString& password);
    void logout();
    bool isLoggedIn() const { return m_isLoggedIn; }
    QString getCurrentUser() const { return m_userLogin; }

    // Команды
    QString sendCommand(const QString& command);
    QString getServerStats();

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
};

#endif
