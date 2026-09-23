/**
 * @file mainwindow.h
 * @brief Главное окно клиента после входа.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Главное окно: задания, статистика, вариант 4, выход.
 *
 * Все действия выполняются через ClientAPI; ответы сервера пишутся в журнал окна.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Создаёт окно для вошедшего пользователя.
     * @param parent   Родительский виджет.
     * @param username Логин пользователя.
     */
    MainWindow(QWidget *parent = nullptr, const QString& username = "");

    /// Удаляет форму.
    ~MainWindow();

private slots:
    void on_task1Button_clicked();   ///< Задание 1: уравнение (TASK1)
    void on_task2Button_clicked();   ///< Задание 2: интеграл (TASK2)
    void on_statsButton_clicked();   ///< Статистика сервера (STATS)
    void on_menuButton_clicked();    ///< Меню «О программе» / «Помощь»
    void on_logoutButton_clicked();  ///< Выход и возврат к окну входа
    void on_task4Button_clicked();   ///< Вариант 4: задание на метод Ньютона (TASK4)
    void on_newtonButton_clicked();  ///< Вариант 4: калькулятор шага Ньютона (NEWTON)

private:
    Ui::MainWindow *ui;  ///< Сгенерированная форма
    QString currentUser; ///< Логин пользователя

    /**
     * @brief Добавить строку с меткой времени в журнал окна.
     * @param text Текст.
     */
    void addInfo(const QString& text);
};

#endif // MAINWINDOW_H
