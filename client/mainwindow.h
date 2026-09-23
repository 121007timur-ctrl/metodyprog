#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, const QString& username = "");
    ~MainWindow();

private slots:
    void on_task1Button_clicked();
    void on_task2Button_clicked();
    void on_statsButton_clicked();
    void on_menuButton_clicked();
    void on_logoutButton_clicked();
    void on_task4Button_clicked();   ///< Вариант 4: задание на метод Ньютона (TASK4)
    void on_newtonButton_clicked();  ///< Вариант 4: калькулятор шага Ньютона (NEWTON)

private:
    Ui::MainWindow *ui;
    QString currentUser;

    void addInfo(const QString& text);
};

#endif // MAINWINDOW_H
