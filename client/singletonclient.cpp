#include "singletonclient.h"

SingletonClient* SingletonClient::p_instance = nullptr;
SingletonClientDestroyer SingletonClient::destroyer;

SingletonClient* SingletonClient::getInstance()
{
    if (!p_instance) {
        p_instance = new SingletonClient();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

void SingletonClient::setUserLogin(const QString& login) { m_userLogin = login; }
QString SingletonClient::getUserLogin() const { return m_userLogin; }
bool SingletonClient::isLoggedIn() const { return m_isLoggedIn; }
void SingletonClient::setLoggedIn(bool status) { m_isLoggedIn = status; }

void SingletonClient::showStatus() const
{
    qDebug() << "=== Статус клиента ===";
    qDebug() << "Логин:" << (m_userLogin.isEmpty() ? "не задан" : m_userLogin);
    qDebug() << "Статус:" << (m_isLoggedIn ? "В СИСТЕМЕ" : "НЕ в системе");
}

void SingletonClient::login(const QString& login)
{
    m_userLogin = login;
    m_isLoggedIn = true;
    qDebug() << "Клиент" << login << "вошёл в систему";
}

void SingletonClient::logout()
{
    m_userLogin = "";
    m_isLoggedIn = false;
    qDebug() << "Клиент вышел из системы";
}

bool SingletonClient::connectToServer(const QString& host, int port)
{
    m_socket = new QTcpSocket();
    m_socket->connectToHost(host, port);
    m_connected = m_socket->waitForConnected(3000);
    return m_connected;
}

void SingletonClient::disconnectFromServer()
{
    if (m_connected) {
        m_socket->disconnectFromHost();
        m_connected = false;
    }
}

bool SingletonClient::sendCommand(const QString& command)
{
    if (!m_connected) return false;
    m_socket->write(command.toUtf8() + "\r\n");
    return m_socket->waitForBytesWritten();
}

QString SingletonClient::waitForResponse()
{
    if (!m_connected) return "";
    m_socket->waitForReadyRead(3000);
    return QString::fromUtf8(m_socket->readAll());
}

void SingletonClient::processUserInput()
{
    QTextStream stdinStream(stdin);
    QString input = stdinStream.readLine();
    sendCommand(input);
    qDebug() << "Ответ:" << waitForResponse();
}