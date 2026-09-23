/**
 * @file database.h
 * @brief Хранилище пользователей сервера (синглтон).
 *
 * Данные хранятся в памяти процесса сервера и теряются при его перезапуске.
 */
#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QMap>
#include <QVector>

/**
 * @brief Игровая статистика пользователя (игра «Угадай число»).
 */
struct GameStatsData {
    int totalAttempts;    ///< Сумма попыток по всем партиям
    int totalCorrect;     ///< Количество угаданных чисел
    int gamesPlayed;      ///< Сыграно партий
    int bestGameAttempts; ///< Лучший результат (меньше — лучше); 999999 — нет данных
};

/**
 * @brief База данных пользователей. Реализована как синглтон Мейерса.
 *
 * Пользователь идентифицируется логином; email дополнительно
 * отображается на логин, поэтому войти можно по любому из них.
 */
class Database
{
public:
    /**
     * @brief Единственный экземпляр базы.
     * @return Ссылка на экземпляр.
     */
    static Database& instance();

    /**
     * @brief Зарегистрировать пользователя.
     * @param email    Email (уникальный).
     * @param login    Логин (уникальный).
     * @param password Пароль.
     * @return false, если логин или email уже заняты.
     */
    bool registerUser(const QString& email, const QString& login, const QString& password);

    /**
     * @brief Вход в систему.
     * @param loginOrEmail Логин или email (распознаётся по символу '@').
     * @param password     Пароль.
     * @return true при верной паре логин/пароль; пользователь помечается как online.
     */
    bool loginUser(const QString& loginOrEmail, const QString& password);

    /**
     * @brief Проверить, выполнен ли вход.
     * @param loginOrEmail Логин или email.
     * @return true, если пользователь существует и online.
     */
    bool isLoggedIn(const QString& loginOrEmail);

    /**
     * @brief Получить логин по email.
     * @param email Email.
     * @return Логин или пустая строка, если email не найден.
     */
    QString getUserLogin(const QString& email);

    /**
     * @brief Существует ли пользователь.
     * @param loginOrEmail Логин или email.
     * @return true, если найден.
     */
    bool userExists(const QString& loginOrEmail);

    /**
     * @brief Количество зарегистрированных пользователей.
     * @return Число пользователей.
     */
    int getTotalUsers();

    /**
     * @brief Список всех логинов.
     * @return Логины в алфавитном порядке.
     */
    QVector<QString> getAllLogins();

    /**
     * @brief Учесть результат партии игры.
     * @param login    Логин игрока.
     * @param attempts Попыток в партии.
     * @param success  Угадано ли число.
     */
    void updateGameStats(const QString& login, int attempts, bool success);

    /**
     * @brief Игровая статистика пользователя.
     * @param login Логин.
     * @return Статистика; нули, если пользователь не найден.
     */
    GameStatsData getUserGameStats(const QString& login);

    /**
     * @brief Обнулить игровую статистику.
     * @param login Логин.
     */
    void resetUserGameStats(const QString& login);

    /**
     * @brief Лучшие игроки по числу угаданных чисел.
     * @param limit Максимальная длина списка.
     * @return Пары (логин, угадано), по убыванию.
     */
    QVector<QPair<QString, int>> getTopPlayers(int limit = 10);

private:
    Database() = default; ///< Закрытый конструктор синглтона.

    /// Запись о пользователе.
    struct UserData {
        QString email;          ///< Email
        QString login;          ///< Логин
        QString password;       ///< Пароль (хранится открытым текстом)
        bool isOnline;          ///< Выполнен ли вход
        GameStatsData gameStats;///< Игровая статистика
    };

    QMap<QString, UserData> users;       ///< Пользователи по логину
    QMap<QString, QString> emailToLogin; ///< Соответствие email → логин
};

#endif // DATABASE_H
