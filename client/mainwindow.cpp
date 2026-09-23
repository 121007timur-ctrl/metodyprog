#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "authregform.h"
#include "clientapi.h"
#include "functionclient.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>

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

void MainWindow::on_task4Button_clicked()
{
    ClientAPI* api = ClientAPI::getInstance();
    QString task = stripServerMenu(api->sendCommand("TASK4"));
    addInfo(task);

    bool ok;
    QString answer = QInputDialog::getText(this, "Задание 4: метод Ньютона",
                                           task + "\n\nВведите x1:", QLineEdit::Normal, "", &ok);

    if (ok && !answer.isEmpty()) {
        QString result = stripServerMenu(api->sendCommand(answer));
        addInfo(result);
        QMessageBox::information(this, "Результат", result);
    } else {
        // Сервер ждёт ответ на задание — отправляем нечисловой ответ, чтобы сбросить ожидание
        api->sendCommand("-");
        addInfo("Задание 4 отменено");
    }
}

void MainWindow::on_newtonButton_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Вариант 4: метод Ньютона");

    QFormLayout* form = new QFormLayout(&dialog);
    form->addRow(new QLabel("f(x) = c3·x³ + c2·x² + c1·x + c0\n"
                            "x1 = x0 − f(x0) / f'(x0)"));

    auto makeSpin = [&dialog](double value) {
        QDoubleSpinBox* spin = new QDoubleSpinBox(&dialog);
        spin->setRange(-1e6, 1e6);
        spin->setDecimals(4);
        spin->setValue(value);
        return spin;
    };

    // По умолчанию — классический пример f(x) = x³ − 2x − 5, x0 = 2
    QDoubleSpinBox* c3 = makeSpin(1);
    QDoubleSpinBox* c2 = makeSpin(0);
    QDoubleSpinBox* c1 = makeSpin(-2);
    QDoubleSpinBox* c0 = makeSpin(-5);
    QDoubleSpinBox* x0 = makeSpin(2);
    form->addRow("c3:", c3);
    form->addRow("c2:", c2);
    form->addRow("c1:", c1);
    form->addRow("c0:", c0);
    form->addRow("x0:", x0);

    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    form->addRow(buttons);

    if (dialog.exec() != QDialog::Accepted) return;

    QString response = ClientAPI::getInstance()->newtonStep(
        c3->value(), c2->value(), c1->value(), c0->value(), x0->value());
    addInfo(response);

    double x1;
    if (parseNewtonResponse(response, x1)) {
        QMessageBox::information(this, "Метод Ньютона", response);
    } else {
        QMessageBox::warning(this, "Метод Ньютона", response);
    }
}
