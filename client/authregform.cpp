#include "authregform.h"
#include "ui_authregform.h"
#include "mainwindow.h"
#include "clientapi.h"
#include <QMessageBox>

AUTHregForm::AUTHregForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AUTHregForm)
{
    ui->setupUi(this);
    setWindowTitle("Авторизация");
    updateStatus("Подключение к серверу...");

    ClientAPI* api = ClientAPI::getInstance();
    if (api->connectToServer("127.0.0.1", 33333)) {
        updateStatus("Подключено к серверу");
    } else {
        updateStatus("Ошибка подключения к серверу");
    }
}

AUTHregForm::~AUTHregForm()
{
    delete ui;
}

void AUTHregForm::updateStatus(const QString& status)
{
    ui->statusLabel->setText("Статус: " + status);
}

void AUTHregForm::on_registerButton_clicked()
{
    QString email = ui->loginEdit->text();     // Email
    QString login = ui->emailEdit->text();     // Логин
    QString password = ui->passwordEdit->text();

    if (login.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля!");
        return;
    }

    updateStatus("Выполняется регистрация...");

    ClientAPI* api = ClientAPI::getInstance();
    QString response = api->registerUser(login, email, password);

    if (response.contains("успешна") || response.contains("uspeshna")) {
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно!\nТеперь войдите в систему.");
        updateStatus("Регистрация выполнена");
        ui->loginEdit->clear();
        ui->emailEdit->clear();
        ui->passwordEdit->clear();
    } else {
        QMessageBox::critical(this, "Ошибка", "Регистрация не удалась:\n" + response);
        updateStatus("Ошибка регистрации");
    }
}

void AUTHregForm::on_loginButton_clicked()
{
    QString login = ui->emailEdit->text();     // Логин
    QString password = ui->passwordEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль!");
        return;
    }

    updateStatus("Выполняется вход...");

    ClientAPI* api = ClientAPI::getInstance();
    QString response = api->loginUser(login, password);

    if (response.contains("успешно") || response.contains("uspeshno") ||
        response.contains("Добро пожаловать") || response.contains("Dobro pozhalovat")) {
        QMessageBox::information(this, "Успех", "Вход выполнен успешно!");
        updateStatus("Вход выполнен");

        MainWindow *mainWindow = new MainWindow(nullptr, login);
        mainWindow->show();
        this->close();
    } else {
        QMessageBox::critical(this, "Ошибка", "Неверный логин или пароль:\n" + response);
        updateStatus("Ошибка входа");
    }
}
