/**
 * @file server_functions.h
 * @brief Протокол сервера: состояние клиентов, меню и обработчики команд.
 *
 * Команды (текст, строка на запрос):
 * | Команда | Доступ | Действие |
 * |---------|--------|----------|
 * | REGISTER | гость | регистрация: далее email, логин, пароль |
 * | LOGIN | гость | вход: далее логин или email, пароль |
 * | STATS | все | статистика сервера |
 * | TASK1 / TASK2 | пользователь | задание: уравнение / интеграл |
 * | TASK4 | пользователь | вариант 4: найти x1 методом Ньютона |
 * | NEWTON c3 c2 c1 c0 x0 | пользователь | вариант 4: расчёт шага Ньютона |
 * | STARTGAME, GUESS n, HINT, GAMESTATS | пользователь | игра «Угадай число» |
 * | MENU, LOGOUT | пользователь | меню, выход |
 */
#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <QString>
#include <QTcpSocket>
#include <QMap>

/**
 * @brief Состояние подключённого клиента.
 */
struct ClientInfo {
    QString clientId;     ///< Идентификатор вида "Клиент_N"
    QString login;        ///< Логин после входа
    bool isAuthenticated; ///< Выполнен ли вход
    int authStep;         ///< Шаг диалога: 0 — нет, 1–3 — регистрация, 4–5 — вход
    QString tempEmail;    ///< Email, введённый при регистрации
    QString tempLogin;    ///< Логин, введённый при регистрации/входе
};

/**
 * @brief Партия игры «Угадай число».
 */
struct GameSession {
    int secretNumber;  ///< Загаданное число
    int attempts;      ///< Сделано попыток
    int minRange;      ///< Нижняя граница диапазона
    int maxRange;      ///< Верхняя граница диапазона
    bool gameActive;   ///< Идёт ли партия
    int currentNumber; ///< Копия загаданного числа
};

/**
 * @brief Статистика решения заданий TASK1/TASK2/TASK4 за сессию.
 */
struct TaskStats {
    int totalAttempts;  ///< Всего решено заданий
    int correctAnswers; ///< Правильных ответов
};

extern QMap<QTcpSocket*, ClientInfo> g_clients;       ///< Все подключённые клиенты
extern QMap<QString, QTcpSocket*> g_logins;           ///< Вошедшие пользователи: логин → сокет
extern QMap<QTcpSocket*, GameSession> g_gameSessions; ///< Партии игры по клиентам
extern QMap<QTcpSocket*, TaskStats> g_taskStats;      ///< Статистика заданий по клиентам
extern int g_clientCounter;                           ///< Счётчик для номеров клиентов

/**
 * @brief Сгенерировать идентификатор нового клиента.
 * @return Строка "Клиент_N".
 */
QString generateClientId();

/**
 * @brief Отправить меню для неавторизованного клиента.
 * @param socket Сокет клиента.
 */
void sendAuthMenu(QTcpSocket* socket);

/**
 * @brief Отправить главное меню со статистикой пользователя.
 * @param socket Сокет клиента.
 */
void sendMainMenu(QTcpSocket* socket);

/**
 * @brief Обработать очередной шаг регистрации или входа.
 * @param socket  Сокет клиента.
 * @param info    Состояние клиента (изменяется).
 * @param request Введённая строка.
 * @return "DONE", если диалог завершён; пустая строка, если ждём следующий ввод.
 */
QString handleAuthSteps(QTcpSocket* socket, ClientInfo& info, const QString& request);

/**
 * @brief Выполнить команду клиента.
 * @param socket  Сокет клиента.
 * @param info    Состояние клиента.
 * @param command Команда в верхнем регистре.
 * @return Текст ответа (может быть пустым, если ответ уже отправлен).
 */
QString handleCommands(QTcpSocket* socket, ClientInfo& info, const QString& command);

/**
 * @brief Удалить всё состояние отключившегося клиента.
 * @param socket Сокет клиента.
 */
void cleanupClient(QTcpSocket* socket);

/**
 * @brief Статистика сервера: подключено, авторизовано, зарегистрировано.
 * @return Текст статистики.
 */
QString getStats();

/**
 * @brief Задание 1: случайное линейное уравнение.
 * @param socket Сокет клиента.
 * @param answer Ответ пользователя; пусто — выдать новое задание.
 * @return Текст задания или результат проверки.
 */
QString task1Handler(QTcpSocket* socket, const QString& answer = "");

/**
 * @brief Задание 2: случайный определённый интеграл.
 * @param socket Сокет клиента.
 * @param answer Ответ пользователя; пусто — выдать новое задание.
 * @return Текст задания или результат проверки.
 */
QString task2Handler(QTcpSocket* socket, const QString& answer = "");

/**
 * @brief Задание 4 (вариант 4): найти первое приближение x1 методом Ньютона.
 *
 * Ответ засчитывается при отклонении меньше 0.01; разделитель — точка или запятая.
 * @param socket Сокет клиента.
 * @param answer Ответ пользователя; пусто — выдать новое задание.
 * @return Текст задания или результат проверки.
 */
QString task4Handler(QTcpSocket* socket, const QString& answer = "");

/**
 * @brief Начать партию «Угадай число» (1–100).
 * @param socket Сокет клиента.
 * @return Приветствие игры.
 */
QString handleStartGame(QTcpSocket* socket);

/**
 * @brief Проверить догадку игрока.
 * @param socket Сокет клиента.
 * @param guess  Число в виде строки.
 * @param login  Логин игрока.
 * @return «больше», «меньше» или поздравление.
 */
QString handleGuess(QTcpSocket* socket, const QString& guess, const QString& login);

/**
 * @brief Игровая статистика пользователя из БД.
 * @param socket Сокет клиента.
 * @param login  Логин.
 * @return Текст статистики.
 */
QString handleGameStats(QTcpSocket* socket, const QString& login);

/**
 * @brief Подсказка: в какой трети диапазона число (стоит одну попытку).
 * @param socket Сокет клиента.
 * @return Текст подсказки.
 */
QString handleHint(QTcpSocket* socket);

#endif // SERVER_FUNCTIONS_H
