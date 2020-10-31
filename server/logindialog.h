#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include<QMainWindow>
#include <QDebug>
namespace Ui {
class LoginDialog;
}

class LoginDialog:public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    int modeCheck();

private slots:
    void on_LoginButton_clicked();

    void on_ExitButton_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
