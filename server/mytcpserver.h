#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include <QMap>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

private slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
    void slotAuthTimeout();

private:
    QTcpServer *mTcpServer;
    QMap<QTcpSocket*, QTimer*> m_authTimers;
};

#endif // MYTCPSERVER_H
