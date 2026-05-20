#include "clientapi.h"
#include <QDebug>

ClientAPI* ClientAPI::m_instance = nullptr;

ClientAPI::ClientAPI(QObject* parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_connected(false)
    , m_isLoggedIn(false)
{
    connect(m_socket, &QTcpSocket::connected, this, &ClientAPI::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientAPI::onDisconnected);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &ClientAPI::onError);
}

ClientAPI::~ClientAPI()
{
    disconnectFromServer();
}

ClientAPI* ClientAPI::getInstance()
{
    if (!m_instance) m_instance = new ClientAPI();
    return m_instance;
}

bool ClientAPI::connectToServer(const QString& host, int port)
{
    if (m_connected) return true;
    m_socket->connectToHost(host, port);
    m_connected = m_socket->waitForConnected(3000);
    return m_connected;
}

void ClientAPI::disconnectFromServer()
{
    if (m_connected) {
        m_socket->disconnectFromHost();
        m_connected = false;
    }
}

QString ClientAPI::sendAndReceive(const QString& message, int timeoutMs)
{
    if (!m_connected) return "ERROR: Not connected";

    QByteArray data = message.toUtf8() + "\r\n";
    m_socket->write(data);
    if (!m_socket->waitForBytesWritten(timeoutMs)) return "ERROR: Send failed";
    if (!m_socket->waitForReadyRead(timeoutMs)) return "ERROR: No response";

    return QString::fromUtf8(m_socket->readAll());
}

QString ClientAPI::registerUser(const QString& login, const QString& email, const QString& password)
{
    if (!m_connected) return "ERROR: Server not available";

    // Шаг 1: REGISTER
    QString response = sendAndReceive("REGISTER");
    if (response.isEmpty()) return "ERROR: No response";

    // Шаг 2: EMAIL (НЕ ЛОГИН!)
    response = sendAndReceive(email);
    if (response.isEmpty()) return "ERROR: No response";

    // Шаг 3: LOGIN (НЕ EMAIL!)
    response = sendAndReceive(login);
    if (response.isEmpty()) return "ERROR: No response";

    // Шаг 4: PASSWORD
    response = sendAndReceive(password);
    if (response.isEmpty()) return "ERROR: No response";

    return response;
}

QString ClientAPI::loginUser(const QString& login, const QString& password)
{
    if (!m_connected) return "ERROR: Server not available";

    QString response = sendAndReceive("LOGIN");
    if (response.isEmpty()) return "ERROR: No response";

    response = sendAndReceive(login);
    if (response.isEmpty()) return "ERROR: No response";

    response = sendAndReceive(password);
    if (response.isEmpty()) return "ERROR: No response";

    if (response.contains("uspeshno") || response.contains("успешно") ||
        response.contains("Dobro pozhalovat") || response.contains("Добро пожаловать")) {
        m_userLogin = login;
        m_isLoggedIn = true;
    }

    return response;
}

void ClientAPI::logout()
{
    if (m_connected && m_isLoggedIn) {
        sendAndReceive("LOGOUT");
        m_isLoggedIn = false;
        m_userLogin = "";
    }
}

QString ClientAPI::sendCommand(const QString& command)
{
    if (!m_connected || !m_isLoggedIn) return "ERROR: Not logged in";
    return sendAndReceive(command);
}

QString ClientAPI::getServerStats()
{
    if (!m_connected) return "ERROR: Server not available";
    return sendAndReceive("STATS");
}

QString ClientAPI::startGame()
{
    if (!m_connected || !m_isLoggedIn) return "ERROR: Not logged in";
    return sendAndReceive("STARTGAME");
}

QString ClientAPI::makeGuess(int number)
{
    if (!m_connected || !m_isLoggedIn) return "ERROR: Not logged in";
    return sendAndReceive("GUESS " + QString::number(number));
}

QString ClientAPI::getGameStats()
{
    if (!m_connected || !m_isLoggedIn) return "ERROR: Not logged in";
    return sendAndReceive("GAMESTATS");
}

QString ClientAPI::getHint()
{
    if (!m_connected || !m_isLoggedIn) return "ERROR: Not logged in";
    return sendAndReceive("HINT");
}

void ClientAPI::onConnected()
{
    m_connected = true;
}

void ClientAPI::onDisconnected()
{
    m_connected = false;
}

void ClientAPI::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error:" << error;
    m_connected = false;
}
