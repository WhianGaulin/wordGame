#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "mythread.h"
#include "datactrl.h"
#include "play1.h"

class myTcpserver:public QTcpServer
{
    Q_OBJECT
public:
    myTcpserver();
    void sendUsrList();
    void handle_rgst();
    void handle_log();
    void handle_board();
    void handle_break();//闯关者提交答案
    void handle_build();//出题者提交答案
    void handle_breaker_ask();
    void handle_builder_ask();
    void Handle_Data();  //用于真正的处理
    void handle_word_ask();
    void handle_breaker_update();
    void handle_builder_update();
    void handle_builder_board();
    void handle_breaker_board();
    void handle_delete_usr();
    void handle_search();
    void handle_invite();
    void send_word_to_two();
    void handle_online_exit();
    void transfer_invite_result();
    void tail_handle(int i);
    void search_function(int builder_OR_breaker,QString attribute,int condition);
private:
    void incomingConnection(qintptr socketDes);
    QMap<int,QString> m_socketList;
    Datactrl *m_sql;
    QSqlQuery query;
    QSqlDatabase db_Tcpserver;
    //int read_flag;
    int read_pointer;
    int read_len;
    int read_socketDes;
    QStringList list;
    QString last_send_word;//刚刚送出的单词
    play1 *p1;
private slots:
    void  cilentLeaveSLOT(int socketDes);
    void Recv_Data(int socketDes,QString data);//triggered by mythread->transfer_recvData()   仅用于接收
signals:
    void SendData(int socketDes,QString data);
    //void SendVariant(int socketDes,QVariant data,int type);//type=1->breaker   type=2->builder
    //void SendData()
    void Someone_arrive(QString);
    void Someone_leave(QString);


};

#endif // MYTCPSERVER_H
