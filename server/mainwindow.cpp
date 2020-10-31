#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<"create Mainwindow";
    ui->setupUi(this);
    g=new game(this);
    r=new rgst(this);
    //connect(g,SIGNAL(ask_delete(QString)),this,SLOT(receive_delete(QString)));
}

MainWindow::~MainWindow()
{
    delete g;
    delete r;
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    if(m_datactrl.verifyusr(ui->UserlineEdit->text(),ui->pwdlineEdit->text())){
        g->usr=ui->UserlineEdit->text();
        g->show();this->hide();
    }
    else
        QMessageBox::information(this,"tips",QString("登陆失败!账号或密码错误"));
}

void MainWindow::on_registerBtn_clicked()
{
    r->show();this->hide();
}

void MainWindow::on_ExitBtn_clicked()
{
    close();
}

