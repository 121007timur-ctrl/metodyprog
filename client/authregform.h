/**
 * @file authregform.h
 * @brief Окно регистрации и входа.
 */
#ifndef AUTHREGFORM_H
#define AUTHREGFORM_H

#include <QWidget>

namespace Ui {
class AUTHregForm;
}

/**
 * @brief Стартовое окно клиента: подключение к серверу, регистрация и вход.
 *
 * После успешного входа открывает MainWindow и закрывается.
 */
class AUTHregForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Создаёт окно и подключается к серверу 127.0.0.1:33333.
     * @param parent Родительский виджет.
     */
    explicit AUTHregForm(QWidget *parent = nullptr);

    /// Удаляет форму.
    ~AUTHregForm();

private slots:
    /// Кнопка «Регистрация».
    void on_registerButton_clicked();
    /// Кнопка «Вход».
    void on_loginButton_clicked();

private:
    Ui::AUTHregForm *ui; ///< Сгенерированная форма

    /**
     * @brief Показать статус в нижней строке окна.
     * @param status Текст статуса.
     */
    void updateStatus(const QString& status);
};

#endif // AUTHREGFORM_H
