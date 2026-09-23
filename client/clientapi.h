/**
 * @file clientapi.h
 * @brief Сетевой API клиента (синглтон): соединение с сервером и команды протокола.
 */
#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QString>
#include <QTcpSocket>

/**
 * @brief Единая точка связи клиента с сервером. Реализована как синглтон.
 *
 * Все запросы синхронные: sendAndReceive() отправляет строку и ждёт ответ.
 */
class ClientAPI : public QObject
{
    Q_OBJECT

private:
    static ClientAPI* m_instance; ///< Единственный экземпляр
    QTcpSocket* m_socket;         ///< Сокет соединения с сервером
    bool m_connected;             ///< Есть ли соединение
    QString m_userLogin;          ///< Логин вошедшего пользователя
    bool m_isLoggedIn;            ///< Выполнен ли вход

    /**
     * @brief Закрытый конструктор синглтона.
     * @param parent Родительский объект Qt.
     */
    ClientAPI(QObject* parent = nullptr);

    /// Разрывает соединение.
    ~ClientAPI();

public:
    /**
     * @brief Получить экземпляр (создаётся при первом вызове).
     * @return Указатель на экземпляр.
     */
    static ClientAPI* getInstance();

    /**
     * @brief Подключиться к серверу.
     * @param host Адрес сервера.
     * @param port Порт сервера.
     * @return true, если соединение установлено за 3 секунды.
     */
    bool connectToServer(const QString& host = "127.0.0.1", int port = 33333);

    /// Разорвать соединение.
    void disconnectFromServer();

    /**
     * @brief Есть ли соединение с сервером.
     * @return true, если подключены.
     */
    bool isConnected() const { return m_connected; }

    /**
     * @brief Отправить строку и дождаться ответа.
     * @param message   Строка запроса (перевод строки добавляется автоматически).
     * @param timeoutMs Таймаут ожидания, мс.
     * @return Ответ сервера или строка "ERROR: ...".
     */
    QString sendAndReceive(const QString& message, int timeoutMs = 5000);

    /**
     * @brief Регистрация: REGISTER, затем email, логин, пароль.
     * @param login    Логин.
     * @param email    Email.
     * @param password Пароль.
     * @return Последний ответ сервера.
     */
    QString registerUser(const QString& login, const QString& email, const QString& password);

    /**
     * @brief Вход: LOGIN, затем логин (или email) и пароль.
     * @param login    Логин или email.
     * @param password Пароль.
     * @return Ответ сервера; при успехе isLoggedIn() становится true.
     */
    QString loginUser(const QString& login, const QString& password);

    /// Выход из системы (LOGOUT).
    void logout();

    /**
     * @brief Выполнен ли вход.
     * @return true после успешного loginUser().
     */
    bool isLoggedIn() const { return m_isLoggedIn; }

    /**
     * @brief Логин текущего пользователя.
     * @return Логин или пустая строка.
     */
    QString getCurrentUser() const { return m_userLogin; }

    /**
     * @brief Отправить произвольную команду (только после входа).
     * @param command Команда.
     * @return Ответ сервера.
     */
    QString sendCommand(const QString& command);

    /**
     * @brief Статистика сервера (STATS).
     * @return Ответ сервера.
     */
    QString getServerStats();

    /**
     * @brief Начать игру «Угадай число» (STARTGAME).
     * @return Ответ сервера.
     */
    QString startGame();

    /**
     * @brief Сделать попытку (GUESS n).
     * @param number Число.
     * @return Ответ сервера.
     */
    QString makeGuess(int number);

    /**
     * @brief Игровая статистика (GAMESTATS).
     * @return Ответ сервера.
     */
    QString getGameStats();

    /**
     * @brief Подсказка в игре (HINT).
     * @return Ответ сервера.
     */
    QString getHint();

    /**
     * @brief Вариант 4: шаг метода Ньютона на сервере (команда NEWTON).
     *
     * f(x) = c3*x^3 + c2*x^2 + c1*x + c0.
     * @param c3 Коэффициент при x^3.
     * @param c2 Коэффициент при x^2.
     * @param c1 Коэффициент при x.
     * @param c0 Свободный член.
     * @param x0 Начальное приближение.
     * @return Ответ сервера без главного меню.
     */
    QString newtonStep(double c3, double c2, double c1, double c0, double x0);

private slots:
    /// Соединение установлено.
    void onConnected();
    /// Соединение разорвано.
    void onDisconnected();
    /**
     * @brief Ошибка сокета.
     * @param error Код ошибки.
     */
    void onError(QAbstractSocket::SocketError error);
};

#endif // CLIENTAPI_H
