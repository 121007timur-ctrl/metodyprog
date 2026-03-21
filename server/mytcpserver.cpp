#include "mytcpserver.h"
#include "server_functions.h"
#include "database.h"
#include <QDebug>

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "Server is not started";
    } else {
        qDebug() << "Server is started on port 33333";
        qDebug() << "Waiting for connections...";
    }

    Database::instance();
}

void MyTcpServer::slotNewConnection()
{
    QTcpSocket* socket = mTcpServer->nextPendingConnection();

    ClientInfo info;
    info.clientId = generateClientId();
    info.login = "";
    info.isAuthenticated = false;
    info.authStep = 0;

    g_clients[socket] = info;

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MyTcpServer::slotAuthTimeout);
    m_authTimers[socket] = timer;

    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    qDebug() << "New client:" << info.clientId << "Total:" << g_clients.size();

    sendAuthMenu(socket);
}

void MyTcpServer::slotServerRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !g_clients.contains(socket)) return;

    QByteArray data = socket->readAll();
    QString request = QString::fromUtf8(data).trimmed();

    ClientInfo &info = g_clients[socket];
    qDebug() << "From" << info.clientId << ":" << request;


    if (m_authTimers.contains(socket)) {
        m_authTimers[socket]->stop();
        m_authTimers[socket]->start(60000);
    }


    if (info.authStep > 0) {
        QString result = handleAuthSteps(socket, info, request);
        if (result == "DONE") return;
        if (!result.isEmpty()) {
            socket->write(result.toUtf8());
        }
        return;
    }


    QString command = request.toUpper();
    QString response = handleCommands(socket, info, command);

    if (!response.isEmpty()) {
        socket->write(response.toUtf8());
        if (info.isAuthenticated) {
            sendMainMenu(socket);
        }
    }
}

void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        ClientInfo info = g_clients[socket];
        cleanupClient(socket);

        if (m_authTimers.contains(socket)) {
            m_authTimers[socket]->stop();
            m_authTimers[socket]->deleteLater();
            m_authTimers.remove(socket);
        }

        qDebug() << "Client disconnected:" << info.clientId << "Total:" << g_clients.size();
        socket->deleteLater();
    }
}

void MyTcpServer::slotAuthTimeout()
{
    QTimer* timer = qobject_cast<QTimer*>(sender());
    if (!timer) return;


    for (auto it = m_authTimers.begin(); it != m_authTimers.end(); ++it) {
        if (it.value() == timer) {
            QTcpSocket* socket = it.key();
            if (socket && g_clients.contains(socket)) {
                ClientInfo &info = g_clients[socket];
                info.authStep = 0;
                info.tempEmail = "";
                info.tempLogin = "";
                socket->write("\r\nВремя ожидания истекло. Начните заново.\r\n");
                sendAuthMenu(socket);
            }
            break;
        }
    }
}
