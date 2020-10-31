#include "mythread.h"

myThread::myThread(int socketDes,QObject *parent):
    QThread (parent),
    m_socketDesc(socketDes){

}

myThread::~myThread(){
    m_socket->close();
}

void myThread::run(){
    m_socket=new myTcpsocket(m_socketDesc);

    if(!m_socket->setSocketDescriptor(m_socketDesc)){
        qDebug()<<"my thread run()出错了";
        return;
    }

    connect(this,SIGNAL(transfer_sendData(QString)),m_socket,SLOT(sendData(QString)));
    connect(m_socket,SIGNAL(transfer_RecvData(int,QString)),this,SLOT(transfer_recvDataSLOT(int,QString)));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(transfer_disconnectSLOT()));

    this->exec();
}


void myThread::transfer_recvDataSLOT(int socketDes,QString data){
    emit transfer_recvData(socketDes,data);
}

void myThread::transfer_sendDataSLOT(int socketDes,QString data){
    qDebug()<<"transfer_sendDataSLOT"<<" Destination:socketDes:"<<socketDes<<" data:"<<data<<" and my socketDes: "<<m_socketDesc;
    if(socketDes==-520)//如果是-520则无条件转发
        emit transfer_sendData(data);
    else if(socketDes==m_socketDesc)//筛选
        emit transfer_sendData(data);
}

//void myThread::transfer_sendVariantSLOT(int socketDes,QVariant data){
//    if(socketDes==-520)//如果是-520则无条件转发
//        emit transfer_sendVariant(data);
//    else if(socketDes==m_socketDesc)//筛选
//        emit transfer_sendVariant(data);
//}

void myThread::transfer_disconnectSLOT(){
    emit transfer_disconnect(m_socketDesc);
    m_socket->disconnectFromHost();
}
