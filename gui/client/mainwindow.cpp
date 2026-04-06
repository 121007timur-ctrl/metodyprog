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

// Задание 1: Метод половинного деления
void MainWindow::on_task1Button_clicked()
{
    addInfo("Задание 1: Метод половинного деления");

    bool ok;
    double a = QInputDialog::getDouble(this, "Метод половинного деления",
                                       "Левая граница a:", 1.0, -1000, 1000, 2, &ok);
    if (!ok) return;

    double b = QInputDialog::getDouble(this, "Метод половинного деления",
                                       "Правая граница b:", 3.0, -1000, 1000, 2, &ok);
    if (!ok) return;

    QString result = bisectionMethod(a, b);
    addInfo(result);
    QMessageBox::information(this, "Результат", result);
}

// Задание 2: Метод левых прямоугольников
void MainWindow::on_task2Button_clicked()
{
    addInfo("Задание 2: Метод левых прямоугольников");

    bool ok;
    double a = QInputDialog::getDouble(this, "Метод прямоугольников",
                                       "Нижний предел a:", 0, -1000, 1000, 2, &ok);
    if (!ok) return;

    double b = QInputDialog::getDouble(this, "Метод прямоугольников",
                                       "Верхний предел b:", 2, -1000, 1000, 2, &ok);
    if (!ok) return;

    int n = QInputDialog::getInt(this, "Метод прямоугольников",
                                 "Количество отрезков n:", 10, 1, 1000, 1, &ok);
    if (!ok) return;

    QString result = leftRectangleMethod(a, b, n);
    addInfo(result);
    QMessageBox::information(this, "Результат", result);
}

// Статистика
void MainWindow::on_statsButton_clicked()
{
    addInfo("Запрос статистики...");
    QString response = ClientAPI::getInstance()->getServerStats();
    addInfo("Статистика: " + response);
    QMessageBox::information(this, "Статистика", response);
}

// Меню
void MainWindow::on_menuButton_clicked()
{
    QStringList items = {"О программе", "Помощь", "Настройки", "Контакты"};
    bool ok;
    QString choice = QInputDialog::getItem(this, "Меню", "Выберите:", items, 0, false, &ok);

    if (ok && choice == "О программе") {
        QMessageBox::about(this, "О программе", "Клиент для работы с сервером\nВарианты 1 и 16");
    } else if (ok && choice == "Помощь") {
        QMessageBox::information(this, "Помощь", "Задание 1 - метод половинного деления\nЗадание 2 - метод левых прямоугольников");
    }
}

// Выход
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
