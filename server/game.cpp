#include "game.h"
#include "ui_game.h"

game::game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::game)
{
    p1=new play1(this);
    p2=new play2(this);
    heroes=new board(this);
    ui->setupUi(this);
}

game::~game()
{
    qDebug()<<"delete game...";
    delete p1;
    delete p2;
    delete ui;
}

void game::on_play1Btn_clicked()
{
    p1->set_player(this->usr);
    this->hide();
    p1->show();
}

void game::on_play2Btn_clicked()
{
    p2->set_player(this->usr);
    this->hide();
    p2->show();
}

void game::on_exitBtn_clicked()
{
    close();
}

void game::on_returnBtn_clicked()
{
    parentWidget()->show();
    this->close();
}

void game::on_deleteBtn_clicked()
{
    //emit ask_delete(this->usr);
    g_ctrl.delete_usr(this->usr);
    if(!g_ctrl.search_usr(this->usr)){
        QMessageBox::information(this,"tips",QString("注销成功!"));
        on_returnBtn_clicked();
    }
    else  QMessageBox::information(this,"tips",QString("注销失败!"));
}

void game::on_HeroBtn_clicked()
{
    this->hide();
    heroes->show();
}
