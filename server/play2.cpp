#include "play2.h"
#include "ui_play2.h"
#include "logindialog.h"
play2::play2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::play2)
{
    ui->setupUi(this);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
}

play2::~play2()
{
    delete ui;
}

void play2::set_player(QString usr){
    this->usr=usr;
    my_builder=new builder(usr);
    ui->NickNameBrowser->setText(my_builder->get_nickName());
    ui->bldwords_Browser->setText(QString("%1").arg(my_builder->get_bldwords()));
    ui->exp_Browser->setText(QString("%1").arg(my_builder->get_exp()));
    ui->level_Browser->setText(QString("%1").arg(my_builder->get_level()));
}

void play2::on_returnButton_clicked()
{
    my_builder->update_record();
    this->close();
    parentWidget()->show();
}

void play2::on_exitButton_clicked()
{
    my_builder->update_record();
    close();
}

void play2::keyPressEvent(QKeyEvent *e){
    if(e->key()==Qt::Key_Return)
        on_submitBtn_clicked();
}

void play2::on_submitBtn_clicked()
{
    QString word=ui->lineEdit->text();
    int b=p2_ctrl.add_word(word,usr,my_builder->get_nickName());
    if(b==1)    QMessageBox::information(this,"tips",QString("该单词已经存在!"));
    else if(b==2) {
        QMessageBox::information(this,"tips",QString("出题成功!"));
        my_builder->inc_bldwords();
        my_builder->inc_exp();
        ui->bldwords_Browser->setText(QString("%1").arg(my_builder->get_bldwords()));
        ui->exp_Browser->setText(QString("%1").arg(my_builder->get_exp()));
        ui->level_Browser->setText(QString("%1").arg(my_builder->get_level()));
    }
    else QMessageBox::information(this,"tips",QString("出题失败，未知错误"));
    ui->lineEdit->clear();
    p2_ctrl.traverse_word();
}
