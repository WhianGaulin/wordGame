#include "listen.h"
#include "ui_listen.h"

listen::listen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::listen)
{
    ui->setupUi(this);
    m_Tcpserver=new myTcpserver();
    connect(m_Tcpserver,SIGNAL(Someone_arrive(QString)),this,SLOT(insert_ListItem(QString)));
    connect(m_Tcpserver,SIGNAL(Someone_leave(QString)),this,SLOT(remove_ListItem(QString)));
}

listen::~listen()
{
    qDebug()<<"~listen";
    delete ui;
}

void listen::on_startBtn_clicked()
{
    ui->startBtn->setEnabled(false);
    if(!m_Tcpserver->listen(QHostAddress::Any,8001)){
        qDebug()<<m_Tcpserver->errorString();
        close();//close what???
        return;
    }
    qDebug()<<"listening...";
}

void listen::insert_ListItem(QString usr){
    qDebug()<<"insert ListItem:"<<usr;
    ui->listWidget->addItem(usr);
}

void listen::remove_ListItem(QString usr){//好像依然有内存泄露的问题
    qDebug()<<"remove ListItem:"<<usr;
    QList<QListWidgetItem*> list;
    list=ui->listWidget->findItems(usr,Qt::MatchCaseSensitive);
    if(list.isEmpty())  return;
    QListWidgetItem *sel=list[0];
    int r=ui->listWidget->row(sel);
    QListWidgetItem *item=ui->listWidget->takeItem(r);
    ui->listWidget->removeItemWidget(item);
    delete item;
}
