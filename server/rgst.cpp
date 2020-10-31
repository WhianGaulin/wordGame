#include "rgst.h"
#include "ui_rgst.h"

rgst::rgst(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::rgst)
{
    ui->setupUi(this);
}

rgst::~rgst()
{
    delete ui;
}

void rgst::on_returnBtn_clicked()
{
    this->close();
    parentWidget()->show();
}

void rgst::on_CfnBtn_clicked()
{
    int b=r_datactrl.reg(ui->UserlineEdit->text(),ui->NickNameEdit->text(),ui->pwdlineEdit->text(),ui->pwdCfnEdit->text());
    if(b==1)    QMessageBox::information(this,"tips",QString("该账号已经被注册!"));
    else if(b==2)   QMessageBox::information(this,"tips",QString("两次输入密码不一致!"));
    else if(b==4)   QMessageBox::information(this,"tips",QString("昵称已经被注册了"));
    else if(b==3)   QMessageBox::information(this,"tips",QString("注册成功"));
    else            QMessageBox::information(this,"tips",QString("注册失败，原因未知"));
}
