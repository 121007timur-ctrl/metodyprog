#include "server_functions.h"
#include "database.h"
#include <QDebug>
#include <QRandomGenerator>
#include <QMap>
#include <cmath>

QMap<QTcpSocket*, ClientInfo> g_clients;
QMap<QString, QTcpSocket*> g_logins;
QMap<QTcpSocket*, GameSession> g_gameSessions;
QMap<QTcpSocket*, TaskStats> g_taskStats;  // Статистика заданий
int g_clientCounter = 1;

// Для отслеживания выполненных заданий
static QMap<QTcpSocket*, bool> выполненныеЗадания;
// Для хранения текущего задания
static QMap<QTcpSocket*, QPair<double, double>> текущееЗадание; // (правильный ответ, тип задания)

QString generateClientId()
{
    return "Клиент_" + QString::number(g_clientCounter++);
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

    if (выполненныеЗадания.contains(socket) && выполненныеЗадания[socket]) {
        выполненныеЗадания[socket] = false;
        return;
    }

    ClientInfo &info = g_clients[socket];

    TaskStats stats = g_taskStats.value(socket, {0, 0});
    double percent = 0;
    if (stats.totalAttempts > 0) {
        percent = (double)stats.correctAnswers / stats.totalAttempts * 100;
    }

    QString menu = "\r\n=== ГЛАВНОЕ МЕНЮ ===\r\n";
    menu += "Здравствуйте, " + info.login + "!\r\n";
    menu += "Ваша статистика: решено " + QString::number(stats.totalAttempts) +
            ", правильно " + QString::number(stats.correctAnswers) +
            " (" + QString::number(percent, 'f', 1) + "%)\r\n";
    menu += "Доступные команды:\r\n";
    menu += "STATS     - статистика сервера\r\n";
    menu += "TASK1     - Случайное уравнение\r\n";
    menu += "TASK2     - Случайный интеграл\r\n";
    menu += "MENU      - Показать меню\r\n";
    menu += "LOGOUT    - Выйти из системы\r\n";
    menu += "> ";

    socket->write(menu.toUtf8());
}

QString getStats()
{
    int totalConnected = g_clients.size();
    int authenticated = 0;
    int registered = Database::instance().getTotalUsers();

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
            socket->write("\r\nРегистрация успешна! Теперь войдите в систему.\r\n");
        } else {
            socket->write("\r\nОшибка регистрации! Email или логин уже существуют.\r\n");
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

            socket->write("\r\nВход выполнен успешно! Добро пожаловать, " + login.toUtf8() + "!\r\n");
            sendMainMenu(socket);
        } else {
            socket->write("\r\nОшибка входа! Неверный логин/email или пароль.\r\n");
            info.authStep = 0;
            sendAuthMenu(socket);
        }
        info.tempLogin = "";
        return "DONE";
    }

    return "CONTINUE";
}

// Генерация случайного уравнения
QString generateRandomEquation()
{
    int type = QRandomGenerator::global()->bounded(1, 5);

    switch(type) {
    case 1: return "x + 5 = 10";
    case 2: return "2x = 12";
    case 3: return "x - 7 = 3";
    case 4: return "3x + 2 = 11";
    default: return "x + 5 = 10";
    }
}

double getEquationAnswer(const QString& equation)
{
    if (equation == "x + 5 = 10") return 5;
    if (equation == "2x = 12") return 6;
    if (equation == "x - 7 = 3") return 10;
    if (equation == "3x + 2 = 11") return 3;
    return 0;
}

// Генерация случайного интеграла
QString generateRandomIntegral()
{
    int type = QRandomGenerator::global()->bounded(1, 5);
    int a = QRandomGenerator::global()->bounded(0, 5);
    int b = QRandomGenerator::global()->bounded(a + 1, a + 6);

    switch(type) {
    case 1: return "∫ x dx от " + QString::number(a) + " до " + QString::number(b);
    case 2: return "∫ x^2 dx от " + QString::number(a) + " до " + QString::number(b);
    case 3: return "∫ 2x dx от " + QString::number(a) + " до " + QString::number(b);
    case 4: return "∫ 3x^2 dx от " + QString::number(a) + " до " + QString::number(b);
    default: return "∫ x dx от 0 до 2";
    }
}

double getIntegralAnswer(const QString& integral)
{
    if (integral.contains("∫ x dx")) {
        // ∫ x dx = (b^2 - a^2)/2
        int a = integral.split("до ")[0].split(" ").last().toInt();
        int b = integral.split("до ")[1].toInt();
        return (double)(b*b - a*a) / 2.0;
    }
    else if (integral.contains("∫ x^2 dx")) {
        // ∫ x^2 dx = (b^3 - a^3)/3
        int a = integral.split("до ")[0].split(" ").last().toInt();
        int b = integral.split("до ")[1].toInt();
        return (double)(b*b*b - a*a*a) / 3.0;
    }
    else if (integral.contains("∫ 2x dx")) {
        // ∫ 2x dx = b^2 - a^2
        int a = integral.split("до ")[0].split(" ").last().toInt();
        int b = integral.split("до ")[1].toInt();
        return (double)(b*b - a*a);
    }
    else if (integral.contains("∫ 3x^2 dx")) {
        // ∫ 3x^2 dx = b^3 - a^3
        int a = integral.split("до ")[0].split(" ").last().toInt();
        int b = integral.split("до ")[1].toInt();
        return (double)(b*b*b - a*a*a);
    }
    return 0;
}

QString task1Handler(QTcpSocket* socket, const QString& answer)
{
    static QMap<QTcpSocket*, bool> ожиданиеОтвета;
    static QMap<QTcpSocket*, QString> текущееУравнение;

    if (!ожиданиеОтвета.contains(socket) || !ожиданиеОтвета[socket]) {
        ожиданиеОтвета[socket] = true;
        текущееУравнение[socket] = generateRandomEquation();
        double correctAnswer = getEquationAnswer(текущееУравнение[socket]);
        текущееЗадание[socket] = qMakePair(correctAnswer, 1.0);

        return "\r\n=== ЗАДАНИЕ 1 ===\r\n"
               "Решите уравнение: " + текущееУравнение[socket] + "\r\n"
                                            "Введите ответ (число): ";
    } else {
        ожиданиеОтвета[socket] = false;
        bool ok;
        double userAnswer = answer.toDouble(&ok);

        if (!ok) {
            return "\r\nОшибка: введите число!\r\n";
        }

        double correctAnswer = текущееЗадание[socket].first;

        // Обновляем статистику
        TaskStats& stats = g_taskStats[socket];
        stats.totalAttempts++;

        if (qAbs(userAnswer - correctAnswer) < 0.001) {
            stats.correctAnswers++;
            выполненныеЗадания[socket] = true;
            return "\r\n✅ ПРАВИЛЬНО! Ответ: " + QString::number(correctAnswer) + "\r\n"
                                                                                  "Ваша статистика: " + QString::number(stats.correctAnswers) +
                   " правильных из " + QString::number(stats.totalAttempts) + "\r\n";
        } else {
            выполненныеЗадания[socket] = true;
            return "\r\n❌ НЕПРАВИЛЬНО! Правильный ответ: " + QString::number(correctAnswer) + "\r\n"
                                                                                               "Ваша статистика: " + QString::number(stats.correctAnswers) +
                   " правильных из " + QString::number(stats.totalAttempts) + "\r\n";
        }
    }
}

QString task2Handler(QTcpSocket* socket, const QString& answer)
{
    static QMap<QTcpSocket*, bool> ожиданиеОтвета;
    static QMap<QTcpSocket*, QString> текущийИнтеграл;

    if (!ожиданиеОтвета.contains(socket) || !ожиданиеОтвета[socket]) {
        ожиданиеОтвета[socket] = true;
        текущийИнтеграл[socket] = generateRandomIntegral();
        double correctAnswer = getIntegralAnswer(текущийИнтеграл[socket]);
        текущееЗадание[socket] = qMakePair(correctAnswer, 2.0);

        return "\r\n=== ЗАДАНИЕ 2 ===\r\n"
               "Вычислите интеграл: " + текущийИнтеграл[socket] + "\r\n"
                                           "Введите ответ (число): ";
    } else {
        ожиданиеОтвета[socket] = false;
        bool ok;
        double userAnswer = answer.toDouble(&ok);

        if (!ok) {
            return "\r\nОшибка: введите число!\r\n";
        }

        double correctAnswer = текущееЗадание[socket].first;

        // Обновляем статистику
        TaskStats& stats = g_taskStats[socket];
        stats.totalAttempts++;

        if (qAbs(userAnswer - correctAnswer) < 0.001) {
            stats.correctAnswers++;
            выполненныеЗадания[socket] = true;
            return "\r\n✅ ПРАВИЛЬНО! Интеграл = " + QString::number(correctAnswer, 'f', 4) + "\r\n"
                                                                                              "Ваша статистика: " + QString::number(stats.correctAnswers) +
                   " правильных из " + QString::number(stats.totalAttempts) + "\r\n";
        } else {
            выполненныеЗадания[socket] = true;
            return "\r\n❌ НЕПРАВИЛЬНО! Правильный ответ: " + QString::number(correctAnswer, 'f', 4) + "\r\n"
                                                                                                       "Ваша статистика: " + QString::number(stats.correctAnswers) +
                   " правильных из " + QString::number(stats.totalAttempts) + "\r\n";
        }
    }
}

QString handleStartGame(QTcpSocket* socket)
{
    if (!g_clients.contains(socket)) return "\r\nОшибка: вы не авторизованы\r\n";

    GameSession session;
    session.secretNumber = QRandomGenerator::global()->bounded(1, 101);
    session.attempts = 0;
    session.minRange = 1;
    session.maxRange = 100;
    session.gameActive = true;
    session.currentNumber = session.secretNumber;

    g_gameSessions[socket] = session;

    QString result = "\r\n=== ИГРА УГАДАЙ ЧИСЛО ===\r\n";
    result += "Число загадано! От " + QString::number(session.minRange) +
              " до " + QString::number(session.maxRange) + "\r\n";
    result += "Введите GUESS <число> чтобы угадать\r\n";
    result += "Или HINT для подсказки\r\n";
    result += "==========================\r\n";
    return result;
}

QString handleGuess(QTcpSocket* socket, const QString& guess, const QString& login)
{
    if (!g_gameSessions.contains(socket)) {
        return "\r\nИгра не начата! Введите STARTGAME для начала игры\r\n";
    }

    GameSession& session = g_gameSessions[socket];

    if (!session.gameActive) {
        return "\r\nИгра окончена! Введите STARTGAME для новой игры\r\n";
    }

    bool ok;
    int number = guess.toInt(&ok);
    if (!ok) {
        return "\r\nОшибка! Введите число. Пример: GUESS 50\r\n";
    }

    session.attempts++;

    if (number == session.secretNumber) {
        session.gameActive = false;

        QString result = "\r\nПОЗДРАВЛЯЮ! Вы угадали число " + QString::number(session.secretNumber) + "!\r\n";
        result += "Потрачено попыток: " + QString::number(session.attempts) + "\r\n";
        result += "Введите STARTGAME чтобы сыграть ещё\r\n";
        return result;

    } else if (number < session.secretNumber) {
        return "\r\nНе угадал! Загаданное число БОЛЬШЕ чем " + QString::number(number) + "\r\n";
    } else {
        return "\r\nНе угадал! Загаданное число МЕНЬШЕ чем " + QString::number(number) + "\r\n";
    }
}

QString handleGameStats(QTcpSocket* socket, const QString& login)
{
    Q_UNUSED(socket);
    GameStatsData stats = Database::instance().getUserGameStats(login);

    QString result = "\r\n=== СТАТИСТИКА ИГРЫ ДЛЯ " + login.toUpper() + " ===\r\n";
    result += "Всего попыток: " + QString::number(stats.totalAttempts) + "\r\n";
    result += "Всего угадано: " + QString::number(stats.totalCorrect) + "\r\n";
    result += "Сыграно партий: " + QString::number(stats.gamesPlayed) + "\r\n";

    if (stats.bestGameAttempts != 999999) {
        result += "Лучший результат: " + QString::number(stats.bestGameAttempts) + " попыток\r\n";
    } else {
        result += "Лучший результат: нет данных\r\n";
    }

    if (stats.totalAttempts > 0) {
        double percent = (double)stats.totalCorrect / stats.totalAttempts * 100;
        result += "Процент угадывания: " + QString::number(percent, 'f', 1) + "%\r\n";
    }

    result += "========================\r\n";
    return result;
}

QString handleHint(QTcpSocket* socket)
{
    Q_UNUSED(socket);
    if (!g_gameSessions.contains(socket)) {
        return "\r\nИгра не начата! Введите STARTGAME для начала игры\r\n";
    }

    GameSession& session = g_gameSessions[socket];

    if (!session.gameActive) {
        return "\r\nИгра окончена! Введите STARTGAME для новой игры\r\n";
    }

    int secret = session.secretNumber;
    QString hint;

    if (secret <= 30) {
        hint = "Число в диапазоне 1-30";
    } else if (secret <= 60) {
        hint = "Число в диапазоне 31-60";
    } else {
        hint = "Число в диапазоне 61-100";
    }

    session.attempts++;

    return "\r\nПОДСКАЗКА: " + hint + "\r\n";
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
            socket->write("\r\nНеизвестная команда. Используйте REGISTER, LOGIN или STATS.\r\n");
            sendAuthMenu(socket);
            return "";
        }
    }
    else
    {
        static QMap<QTcpSocket*, QString> ожиданиеЗадания;

        if (ожиданиеЗадания.contains(socket) && !ожиданиеЗадания[socket].isEmpty()) {
            QString taskType = ожиданиеЗадания[socket];
            ожиданиеЗадания[socket] = "";

            if (taskType == "TASK1") {
                return task1Handler(socket, command);
            } else if (taskType == "TASK2") {
                return task2Handler(socket, command);
            }
        }

        if (command == "STARTGAME") {
            return handleStartGame(socket);
        }
        else if (command.startsWith("GUESS ")) {
            QString guess = command.mid(6);
            return handleGuess(socket, guess, info.login);
        }
        else if (command == "GAMESTATS") {
            return handleGameStats(socket, info.login);
        }
        else if (command == "HINT") {
            return handleHint(socket);
        }
        else if (command == "STATS") {
            return getStats();
        }
        else if (command == "TASK1") {
            ожиданиеЗадания[socket] = "TASK1";
            return task1Handler(socket);
        }
        else if (command == "TASK2") {
            ожиданиеЗадания[socket] = "TASK2";
            return task2Handler(socket);
        }
        else if (command == "MENU") {
            sendMainMenu(socket);
            return "";
        }
        else if (command == "LOGOUT") {
            ожиданиеЗадания.remove(socket);
            выполненныеЗадания.remove(socket);
            текущееЗадание.remove(socket);
            if (g_gameSessions.contains(socket)) {
                g_gameSessions.remove(socket);
            }
            g_logins.remove(info.login);
            info.login = "";
            info.isAuthenticated = false;
            socket->write("\r\nВы вышли из системы.\r\n");
            sendAuthMenu(socket);
            return "";
        }
        else {
            socket->write("\r\nНеизвестная команда.\r\n");
            sendMainMenu(socket);
            return "";
        }
    }
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

    if (g_gameSessions.contains(socket)) {
        g_gameSessions.remove(socket);
    }

    g_taskStats.remove(socket);
    выполненныеЗадания.remove(socket);
    текущееЗадание.remove(socket);
}
