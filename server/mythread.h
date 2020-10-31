#ifndef MYTHREAD_H
#define MYTHREAD_H
#include "mytcpsocket.h"
#include <QThread>
#include <QMap>

class myThread:public QThread
{
    Q_OBJECT
public:
    myThread(int socketDes,QObject *parent=nullptr);
    ~myThread();//将m_socket关闭
private:
    void run();
    myTcpsocket *m_socket;
    int m_socketDesc;
private slots:
    void transfer_sendDataSLOT(int,QString);//trigger by server
    //void transfer_sendVariantSLOT(int,QVariant,int);
    void transfer_disconnectSLOT();//trigger by socket
    void transfer_recvDataSLOT(int socketDes,QString data);//trigger by socket
signals:
    void transfer_sendData(int,QString);//emit by tranfer_sendDataSLOT
    //void transfer_sendVariant(QVariant,int);
    void transfer_sendData(QString);
    void transfer_disconnect(int);
    void transfer_recvData(int socketDes,QString data);
};

#endif // MYTHREAD_H
