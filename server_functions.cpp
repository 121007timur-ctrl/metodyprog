#include "server_functions.h"
#include "database.h"
#include <QDebug>
#include <QRandomGenerator>


QMap<QTcpSocket*, ClientInfo> g_clients;
QMap<QString, QTcpSocket*> g_logins;
int g_clientCounter = 1;

QString generateClientId()
{
    return "Client_" + QString::number(g_clientCounter++);
}

void sendAuthMenu(QTcpSocket* socket)
{
    if (!g_clients.contains(socket)) return;

    ClientInfo &info = g_clients[socket];

    QString menu = "\r\n=== ДОБРО ПОЖАЛОВАТЬ " + info.clientId.toUpper() + " ===\r\n";
    menu += "Вы не авторизованы\r\n";
    menu += "Доступные команды:\r\n";
    menu += "REGISTER - зарегистрироваться\r\n";
    menu += "LOGIN - войти в систему\r\n";
    menu += "STATS - статистика сервера\r\n";
    menu += "> ";

    socket->write(menu.toUtf8());
}

void sendMainMenu(QTcpSocket* socket)
{
    if (!g_clients.contains(socket)) return;

    ClientInfo &info = g_clients[socket];

    QString menu = "\r\n=== ГЛАВНОЕ МЕНЮ ===\r\n";
    menu += "Здравствуйте, " + info.login + "!\r\n";
    menu += "Доступные команды:\r\n";
    menu += "STATS - статистика сервера\r\n";
    menu += "TASK1 - Задание 1\r\n";
    menu += "TASK2 - Задание 2\r\n";
    menu += "MENU  - Показать меню\r\n";
    menu += "LOGOUT - Выйти из системы\r\n";
    menu += "> ";

    socket->write(menu.toUtf8());
}

QString getStats()
{
    int totalConnected = g_clients.size();
    int authenticated = 0;
    int registered = Database::instance().getTotalUsers();

    // Считаем авторизованных
    QString onlineList;
    for (auto it = g_clients.begin(); it != g_clients.end(); ++it) {
        if (it.value().isAuthenticated) {
            authenticated++;
            onlineList += "  - " + it.value().login + " (" + it.value().clientId + ")\r\n";
        }
    }

    QString stats = "\r\n=== СТАТИСТИКА СЕРВЕРА ===\r\n";
    stats += "Подключено клиентов: " + QString::number(totalConnected) + "\r\n";
    stats += "Авторизовано: " + QString::number(authenticated) + "\r\n";
    stats += "Зарегистрировано в БД: " + QString::number(registered) + "\r\n";

    if (authenticated > 0) {
        stats += "Сейчас в сети:\r\n" + onlineList;
    }

    stats += "==========================\r\n";

    return stats;
}

QString handleAuthSteps(QTcpSocket* socket, ClientInfo& info, const QString& request)
{
    if (info.authStep == 1) {
        info.tempEmail = request;
        info.authStep = 2;
        socket->write("\r\nВведите логин: ");
        return "";
    }
    else if (info.authStep == 2) {
        info.tempLogin = request;
        info.authStep = 3;
        socket->write("\r\nВведите пароль: ");
        return "";
    }
    else if (info.authStep == 3) {
        QString password = request;

        if (Database::instance().registerUser(info.tempEmail, info.tempLogin, password)) {
            socket->write("\r\n Регистрация успешна! Теперь войдите в систему.\r\n");
        } else {
            socket->write("\r\n Ошибка регистрации! Email или логин уже существуют.\r\n");
        }

        info.authStep = 0;
        info.tempEmail = "";
        info.tempLogin = "";
        sendAuthMenu(socket);
        return "DONE";
    }
    else if (info.authStep == 4) {
        info.tempLogin = request;
        info.authStep = 5;
        socket->write("\r\nВведите пароль: ");
        return "";
    }
    else if (info.authStep == 5) {
        QString password = request;

        if (Database::instance().loginUser(info.tempLogin, password)) {
            QString login;
            if (info.tempLogin.contains('@')) {
                login = Database::instance().getUserLogin(info.tempLogin);
            } else {
                login = info.tempLogin;
            }

            info.login = login;
            info.isAuthenticated = true;
            info.authStep = 0;

            if (g_logins.contains(login)) {
                g_logins.remove(login);
            }
            g_logins[login] = socket;

            socket->write("\r\n Вход выполнен успешно! Добро пожаловать, " + login.toUtf8() + "!\r\n");
            sendMainMenu(socket);
        } else {
            socket->write("\r\n Ошибка входа! Неверный логин/email или пароль.\r\n");
            info.authStep = 0;
            sendAuthMenu(socket);
        }
        info.tempLogin = "";
        return "DONE";
    }

    return "CONTINUE";
}

QString handleCommands(QTcpSocket* socket, ClientInfo& info, const QString& command)
{
    if (!info.isAuthenticated) {
        if (command == "REGISTER") {
            info.authStep = 1;
            socket->write("\r\nВведите ваш email: ");
            return "";
        }
        else if (command == "LOGIN") {
            info.authStep = 4;
            socket->write("\r\nВведите логин или email: ");
            return "";
        }
        else if (command == "STATS") {
            return getStats();
        }
        else {
            socket->write("\r\n Неизвестная команда. Используйте REGISTER, LOGIN или STATS.\r\n");
            sendAuthMenu(socket);
            return "";
        }
    } else {
        if (command == "STATS") {
            return getStats();
        }
        else if (command == "TASK1") {
            return task1Handler();
        }
        else if (command == "TASK2") {
            return task2Handler();
        }
        else if (command == "MENU") {
            sendMainMenu(socket);
            return "";
        }
        else if (command == "LOGOUT") {
            g_logins.remove(info.login);
            info.login = "";
            info.isAuthenticated = false;
            socket->write("\r\n Вы вышли из системы.\r\n");
            sendAuthMenu(socket);
            return "";
        }
        else {
            socket->write("\r\n Неизвестная команда. Доступно: STATS, TASK1, TASK2, MENU, LOGOUT\r\n");
            sendMainMenu(socket);
            return "";
        }
    }
}

QString task1Handler()
{
    QString result = "\r\n=== ЗАДАНИЕ 1 ===\r\n";
    result += " Вы выбрали задание 1\r\n";
    result += " Описание: реализовать функцию подсчета чисел\r\n";
    result += " Статус: в разработке\r\n";
    result += "==================\r\n";
    return result;
}

QString task2Handler()
{
    QString result = "\r\n=== ЗАДАНИЕ 2 ===\r\n";
    result += " Вы выбрали задание 2\r\n";
    result += " Описание: реализовать функцию обработки чисел\r\n";
    result += " Статус: ожидает выполнения\r\n";
    result += "==================\r\n";
    return result;
}

void cleanupClient(QTcpSocket* socket)
{
    if (g_clients.contains(socket)) {
        ClientInfo info = g_clients[socket];

        if (!info.login.isEmpty()) {
            g_logins.remove(info.login);
        }

        g_clients.remove(socket);
    }
}
