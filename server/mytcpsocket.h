#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QTcpSocket>
#include <QHostAddress>
#include <QMap>
#include "qvariant_define.h"
class myTcpsocket:public QTcpSocket
{
    Q_OBJECT
public:
    myTcpsocket();
    myTcpsocket(int SocketDesc,QObject *parent=nullptr);
private:
    int m_socketDesc;
signals:
    void transfer_RecvData(int socketDes,QString data);//emitted by this->RecvData()
private slots:
    void sendData(int,QString);
    void sendData(QString);
    //void sendVariant(QVariant,int);
    void RecvData();
};

#endif // MYTCPSOCKET_H
