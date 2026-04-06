#ifndef AUTHREGFORM_H
#define AUTHREGFORM_H

#include <QWidget>

namespace Ui {
class AUTHregForm;
}

class AUTHregForm : public QWidget
{
    Q_OBJECT

public:
    explicit AUTHregForm(QWidget *parent = nullptr);
    ~AUTHregForm();

private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();

private:
    Ui::AUTHregForm *ui;
    void updateStatus(const QString& status);
};

#endif // AUTHREGFORM_H
