#include "play1.h"
#include "ui_play1.h"
#include "logindialog.h"
/*
    int time_used;//花费时间
    int TimeBuffer=2;//单词显示时间
    int round_count;//闯关数
    int wordlist_sum;//单词总数
    int round_count_wordsum;//关卡的单词数量
    int breaked_word_count;//该关卡已经通过的单词数量
    int len_min;//单词长度
    */
play1::play1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::play1)
{
    qDebug()<<"construct a play1";
    ui->setupUi(this);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    db_p1=QSqlDatabase::database("my_connection");db_p1.open();
    my_builder=nullptr;//初始化令其为空指针，否则不知道指向哪里，也许导致程序异常
    query=QSqlQuery(db_p1);
    breaked_word_count=0;
}

play1::~play1()
{
    delete ui;
}

void play1::set_player(QString usr){//调用在play1的构造函数之后
    this->usr=usr;
    my_breaker=new breaker(usr);
    ui->NickNameBrowser->setText(my_breaker->get_nickName());
    ui->RoundBrowser->setText(QString("%1").arg(my_breaker->get_breaked_rounds()));
    ui->levelBrowser->setText(QString("%1").arg(my_breaker->get_level()));
    ui->expBrowser->setText(QString("%1").arg(my_breaker->get_exp()));
    round_count=my_breaker->get_breaked_rounds();//读取之前闯关数
    current_word="";
    UI_display();

}

void play1::on_returnButton_clicked()
{
    my_breaker->update_record();//离开之前更新（保存）变更后的数据
    if(id1){
        killTimer(id1);//离开之前保证kill所有无用的timer
        id1=0;//置零表示失效
    }
    if(id2){
        killTimer(id2);//离开之前保证kill所有无用的timer
        id2=0;//置零表示失效
    }
    this->close();
    parentWidget()->show();
}

void play1::on_exitButton_clicked()
{
    my_breaker->update_record();//离开之前更新（保存）变更后的数据
    close();
}

void play1::on_startBtn_clicked()
{
    set_round();
}

void play1::UI_display(){//不控制time_LCD
    ui->round_LCD->display(round_count+1);//关卡号=闯关数+1
    ui->RoundBrowser->setText(QString("%1").arg(my_breaker->get_breaked_rounds()));
    ui->levelBrowser->setText(QString("%1").arg(my_breaker->get_level()));
    ui->left_word_LCD->display(round_count_wordsum-breaked_word_count);
    ui->round_word_LCD->display(round_count_wordsum);
    ui->expBrowser->setText(QString("%1").arg(my_breaker->get_exp()));
    ui->wordBrowser->setText(current_word);
    ui->builder_label->setText(QString("来自:'%1'").arg(builder_nickname));
}

void play1::on_submitBtn_clicked()
{
    QString submit_word=ui->lineEdit->text();
    ui->lineEdit->clear();
    if(submit_word==current_word){
        breaked_word_count++;
        if(breaked_word_count==round_count_wordsum){
            round_count++;//关卡数增加
            UI_display();
            //my_breaker->inc_exp();
            my_breaker->inc_exp(round_count,time_used);
            my_breaker->set_level();
            my_breaker->set_rounds(round_count);
            set_round();//set new round
        }
        else {//单词正确但是这关还没有闯完
            set_question();
            UI_display();
        }
    }
    else {
        QMessageBox::information(this,"tips",QString("提交单词错误!!"));
        set_round();//set new round
    }
}

void play1::improve(){

}

void play1::set_word(){
    current_word=p1_ctrl.get_Random_word();
}

void play1::set_current_word(){//设置单词
    qDebug()<<"set_current_word...";
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int rand=qrand()%wordlist_sum;

    query.exec(QString("select * from wordlist where length=%1").arg(len_min));
    //qDebug()<<wordlist_sum<<"  "<<rand;
    query.seek(rand);
    current_word=query.value(0).toString();
    builder_usr=query.value(2).toString();
    about_word_builder();
}

void play1::set_current_word(int round){
    len_min=3+round/4;
    get_wordlist_count(len_min);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int rand=qrand()%wordlist_sum;
    query.exec(QString("select * from wordlist where length=%1").arg(len_min));
    query.seek(rand);
    current_word=query.value(0).toString();
    builder_usr=query.value(2).toString();//给出题者经验
    about_word_builder();//给出题者经验，函数里面设置了builder_nickname
}

void play1::timerEvent(QTimerEvent *event){
    if(event->timerId()==id1){//单词显示时间到
        ui->wordBrowser->setText("");
        killTimer(id1);id1=0;
    }
    else if(event->timerId()==id2){//一秒过去了
        time_count--;
        time_used++;
        ui->time_LCD->display(time_count);
        if(time_count==0){
            killTimer(id2);id2=0;
        }
    }
}

void play1::get_wordlist_count(int len){
    query.exec(QString("select count(*) from wordlist where length=%1").arg(len));
    query.next();
    wordlist_sum=query.value(0).toInt();
    qDebug()<<"wordlist_sum:"<<wordlist_sum;
}


void play1::set_round(){
    breaked_word_count=0;//重置
    int s=5-round_count/3;//s为单词显示时间
    TimeBuffer=s>1?s:1;
    qDebug()<<"TimeBuffer:"<<TimeBuffer;
    len_min=3+round_count/4;
    get_wordlist_count(len_min);
    round_count_wordsum=1+round_count/4;
    qDebug()<<"this round has word sum:"<<round_count_wordsum;
    qDebug()<<"this round word len:"<<len_min;
    qDebug()<<"total word:"<<wordlist_sum;
    set_question();
}


void play1::set_question(){
    //set_word();
    set_current_word();
    time_count=TimeBuffer;//重置
    if(id1)    killTimer(id1);//startTimer之前保证kill所有无用的timer
    if(id2)    killTimer(id2);//startTimer之前保证kill所有无用的timer
    ui->time_LCD->display(time_count);//先显示
    UI_display();
    id1=startTimer(TimeBuffer*1000);//单词显示时间
    id2=startTimer(1000);//表示倒计时
    //qDebug()<<"id1:"<<id1<<"id2:"<<id2;
}

void play1::keyPressEvent(QKeyEvent *e){
    if(e->key()==Qt::Key_Return){
        on_submitBtn_clicked();
    }
}


void play1::about_word_builder(){
    if(!query.exec(QString("select * from wordlist where word='%1'").arg(current_word)))//???
        qDebug()<<query.lastError();
    query.next();
    builder_nickname=query.value(3).toString();
    my_builder=new builder(builder_usr);
    my_builder->inc_exp();
    my_builder->update_record();
    delete my_builder;my_builder=nullptr;//删除指针所指，并令指针指向空
}











