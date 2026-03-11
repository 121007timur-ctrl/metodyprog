#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>

MyTcpServer::~MyTcpServer()
{

    mTcpServer->close();
    //server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    socket->write("Hello, World!!! I am echo server!\r\n");
    socket->write("=== Echo Server ===\r\n");
    socket->write("Commands:\r\n");
    socket->write("REGISTER:login:pass\r\n");
    socket->write("LOGIN:login:pass\r\n");
    socket->write("STATS\r\n");
    socket->write("==================\r\n");
    socket->write("> ");

    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    mTcpSockets.append(socket);
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    QString msg = QString::fromUtf8(data).trimmed();

    qDebug() << "Received:" << msg;

    if (msg.startsWith("REGISTER:")) {
        socket->write("[✅] Registration successful (stub)\r\n> ");
    }
    else if (msg.startsWith("LOGIN:")) {
        socket->write("[✅] Login successful (stub)\r\n> ");
    }
    else if (msg == "STATS") {
        QString resp = "[📊] Online clients: " + QString::number(mTcpSockets.size()) + " (stub)\r\n> ";
        socket->write(resp.toUtf8());
    }
    else {
        socket->write("[❌] Unknown command\r\n> ");
    }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        mTcpSockets.removeOne(socket);
        socket->deleteLater();
    }
}
