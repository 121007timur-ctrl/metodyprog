/**
 * @file mytcpserver.h
 * @brief TCP-сервер: принимает подключения и передаёт запросы обработчикам.
 */
#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include <QMap>

/**
 * @brief Многоклиентский TCP-сервер на порту 33333.
 *
 * Каждое сообщение клиента разбирается в slotServerRead(): во время
 * регистрации/входа оно передаётся в handleAuthSteps(), иначе —
 * в handleCommands() (см. server_functions.h).
 */
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Создаёт сервер и начинает слушать порт 33333.
     * @param parent Родительский объект Qt.
     */
    explicit MyTcpServer(QObject *parent = nullptr);

    /// Закрывает серверный сокет.
    ~MyTcpServer();

private slots:
    /// Новое подключение: регистрирует клиента и отправляет меню входа.
    void slotNewConnection();
    /// Входящие данные от клиента: разбор и ответ.
    void slotServerRead();
    /// Отключение клиента: очистка его состояния.
    void slotClientDisconnected();
    /// Истёк таймер ожидания во время регистрации/входа.
    void slotAuthTimeout();

private:
    QTcpServer *mTcpServer;                    ///< Слушающий сокет
    QMap<QTcpSocket*, QTimer*> m_authTimers;   ///< Таймеры ожидания по клиентам
};

#endif // MYTCPSERVER_H
