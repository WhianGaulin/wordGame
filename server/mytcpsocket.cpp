#include "mytcpsocket.h"
#include <QtNetwork>
myTcpsocket::myTcpsocket(int socketDes,QObject *parent):
    QTcpSocket (parent),
    m_socketDesc(socketDes)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(RecvData()));

}

void myTcpsocket::sendData(int socketDes,QString data){
    if(socketDes==m_socketDesc)
        write(data.toUtf8());
}

void myTcpsocket::sendData(QString data){
    qDebug()<<"sendData:"<<data<<" socket num:"<<m_socketDesc;
    write(data.toUtf8());
}

//type==1, Variant为Breaker_Info
//type==2, Variant为Builder_Info
//..
//void myTcpsocket::sendVariant(QVariant data,int type){
//    QByteArray block;
//    QDataStream out(&block,QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_12);
//    if(type==1){
//        out<<"f";
//        Breaker_Info b=data.value<Breaker_Info>();
//        out<<b.usrname;
//        out<<b.nickname;
//        out<<b.break_rounds;
//        out<<b.exp;
//        out<<b.level;
//        qDebug()<<"send Variant:"<<"f "<<b.usrname<<" "<<b.nickname<<" "<<b.break_rounds<<" "<<b.exp<<" "<<b.level;
//    }
//    else if(type==2){
//        out<<"h";
//        Builder_Info b=data.value<Builder_Info>();
//        out<<b.usrname;
//        out<<b.nickname;
//        out<<b.build_words;
//        out<<b.exp;
//        out<<b.level;
//        qDebug()<<"send Variant:"<<"h "<<b.usrname<<" "<<b.nickname<<" "<<b.build_words<<" "<<b.exp<<" "<<b.level;
//    }
//    write(block);

//}

//rgst -a
//log  -b
//board -c
//break-submit -d
//build-submit -e
//break-ask -f
//build-ask -g

void myTcpsocket::RecvData(){
    QString data=readAll();
    emit transfer_RecvData(m_socketDesc,data);
}
