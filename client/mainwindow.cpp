#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authregform.h"
#include "clientapi.h"
#include "functionclient.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent, const QString& username)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(username)
{
    ui->setupUi(this);
    setWindowTitle("Главное окно - " + currentUser);
    ui->userLabel->setText("Пользователь: " + currentUser);
    addInfo("Добро пожаловать, " + currentUser + "!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addInfo(const QString& text)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->infoTextEdit->append("[" + timestamp + "] " + text);
}

void MainWindow::on_task1Button_clicked()
{
    addInfo("Задание 1: Решите уравнение x^2 - 4 = 0");

    ClientAPI* api = ClientAPI::getInstance();
    QString response = api->sendCommand("TASK1");
    addInfo(response);

    bool ok;
    QString answer = QInputDialog::getText(this, "Задание 1",
                                           response + "\n\nВведите ваш ответ:", QLineEdit::Normal, "", &ok);

    if (ok && !answer.isEmpty()) {
        QString result = api->sendCommand(answer);
        addInfo(result);
        QMessageBox::information(this, "Результат", result);
    } else {
        addInfo("Задание 1 отменено");
    }
}

void MainWindow::on_task2Button_clicked()
{
    addInfo("Задание 2: Вычислите интеграл ∫ x^2 dx от 0 до 2");

    ClientAPI* api = ClientAPI::getInstance();
    QString response = api->sendCommand("TASK2");
    addInfo(response);

    bool ok;
    QString answer = QInputDialog::getText(this, "Задание 2",
                                           response + "\n\nВведите ваш ответ:", QLineEdit::Normal, "", &ok);

    if (ok && !answer.isEmpty()) {
        QString result = api->sendCommand(answer);
        addInfo(result);
        QMessageBox::information(this, "Результат", result);
    } else {
        addInfo("Задание 2 отменено");
    }
}

void MainWindow::on_statsButton_clicked()
{
    addInfo("Запрос статистики...");
    QString response = ClientAPI::getInstance()->getServerStats();
    addInfo("Статистика: " + response);
    QMessageBox::information(this, "Статистика", response);
}

void MainWindow::on_menuButton_clicked()
{
    QStringList items = {"О программе", "Помощь", "Настройки", "Контакты"};
    bool ok;
    QString choice = QInputDialog::getItem(this, "Меню", "Выберите:", items, 0, false, &ok);

    if (ok && choice == "О программе") {
        QMessageBox::about(this, "О программе", "Клиент для работы с сервером\nВарианты 1 и 16");
    } else if (ok && choice == "Помощь") {
        QMessageBox::information(this, "Помощь", "Задание 1 - решить уравнение x^2 - 4 = 0\nЗадание 2 - вычислить интеграл ∫ x^2 dx от 0 до 2");
    }
}

void MainWindow::on_logoutButton_clicked()
{
    int result = QMessageBox::question(this, "Выход", "Выйти из системы?",
                                       QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        ClientAPI::getInstance()->logout();
        AUTHregForm *authForm = new AUTHregForm();
        authForm->show();
        this->close();
    }
}
