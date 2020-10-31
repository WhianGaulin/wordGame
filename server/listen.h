#ifndef LISTEN_H
#define LISTEN_H

#include <QMainWindow>
#include "mytcpserver.h"
namespace Ui {
class listen;
}

class listen : public QMainWindow
{
    Q_OBJECT

public:
    explicit listen(QWidget *parent = nullptr);
    ~listen();

private slots:
    void on_startBtn_clicked();
    void insert_ListItem(QString);
    void remove_ListItem(QString);
private:
    Ui::listen *ui;
    myTcpserver *m_Tcpserver;
};

#endif // LISTEN_H
