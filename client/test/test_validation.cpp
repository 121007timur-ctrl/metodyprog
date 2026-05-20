#include <QtTest>
#include <QString>

// ========== ФУНКЦИИ ВАЛИДАЦИИ ==========

bool isValidLogin(const QString& login)
{
    if (login.isEmpty()) return false;
    if (login.length() < 3) return false;
    if (login.contains(' ')) return false;
    return true;
}

bool isValidEmail(const QString& email)
{
    if (email.isEmpty()) return false;
    if (!email.contains('@')) return false;
    int atPos = email.indexOf('@');
    if (atPos == 0) return false;
    if (atPos == email.length() - 1) return false;
    if (!email.contains('.', Qt::CaseSensitive)) return false;
    return true;
}

bool isValidPassword(const QString& password)
{
    if (password.isEmpty()) return false;
    if (password.length() < 6) return false;
    return true;
}

QString validateRegistration(const QString& login, const QString& email, const QString& password)
{
    if (!isValidLogin(login)) return "Ошибка: логин должен быть не менее 3 символов";
    if (!isValidEmail(email)) return "Ошибка: неверный формат email";
    if (!isValidPassword(password)) return "Ошибка: пароль должен быть не менее 6 символов";
    return "Успех";
}

// ========== ТЕСТЫ ==========

class TestValidation : public QObject
{
    Q_OBJECT

private slots:
    // Тесты для логина
    void testLoginValid()
    {
        QVERIFY(isValidLogin("user"));
        QVERIFY(isValidLogin("abc"));
        QVERIFY(isValidLogin("user123"));
        QVERIFY(isValidLogin("a_b_c"));
    }

    void testLoginInvalid()
    {
        QVERIFY(!isValidLogin(""));
        QVERIFY(!isValidLogin("ab"));
        QVERIFY(!isValidLogin("a"));
        QVERIFY(!isValidLogin("us er"));
        QVERIFY(!isValidLogin("  "));
    }

    // Тесты для email
    void testEmailValid()
    {
        QVERIFY(isValidEmail("user@example.com"));
        QVERIFY(isValidEmail("name@domain.ru"));
        QVERIFY(isValidEmail("test123@mail.co.uk"));
        QVERIFY(isValidEmail("a@b.c"));
    }

    void testEmailInvalid()
    {
        QVERIFY(!isValidEmail(""));
        QVERIFY(!isValidEmail("user@"));
        QVERIFY(!isValidEmail("user@domain"));
        QVERIFY(!isValidEmail("user"));
        QVERIFY(!isValidEmail("@domain.com"));
        QVERIFY(!isValidEmail("user@.com"));
    }

    // Тесты для пароля
    void testPasswordValid()
    {
        QVERIFY(isValidPassword("123456"));
        QVERIFY(isValidPassword("password"));
        QVERIFY(isValidPassword("qwerty123"));
    }

    void testPasswordInvalid()
    {
        QVERIFY(!isValidPassword(""));
        QVERIFY(!isValidPassword("12345"));
        QVERIFY(!isValidPassword("a"));
    }

    // Тесты регистрации
    void testRegistrationValidation()
    {
        QCOMPARE(validateRegistration("user", "user@mail.com", "123456"), QString("Успех"));
        QVERIFY(validateRegistration("", "user@mail.com", "123456").contains("логин"));
        QVERIFY(validateRegistration("user", "invalid", "123456").contains("email"));
        QVERIFY(validateRegistration("user", "user@mail.com", "123").contains("пароль"));
    }
};

QTEST_MAIN(TestValidation)
