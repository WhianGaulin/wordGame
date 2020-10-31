#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
LoginDialog::LoginDialog(QWidget *parent) :
    QMainWindow (parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_LoginButton_clicked()
{
    modeCheck();
    if(ui->UsernameEdit->text()==tr("1")&&ui->passwordEdit->text()==tr("1")){//仅账号密码正确
        if(this->modeCheck()==0)    QMessageBox::warning(this,tr("warning"),tr("please choose one mode only!"),QMessageBox::Yes);
        else if(this->modeCheck()==3) QMessageBox::warning(this,tr("warning"),tr("please choose one and only one mode"),QMessageBox::Yes);
        else    accept();
    }
    else  QMessageBox::warning(this,tr("warning"),tr("username or password error"),QMessageBox::Yes);
}


int LoginDialog::modeCheck(){
    if(ui->play1check->isChecked()&&ui->play2check->isChecked())
        return 0;
    else if(ui->play1check->isChecked())
        return 1;
    else if(ui->play2check->isChecked())
        return 2;
    else
        return 3;
}

void LoginDialog::on_ExitButton_clicked()
{
    close();
}
