#include <QMainWindow>
#include "mainwindow.h"
#include "listen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    listen w;
    w.show();
    return  a.exec();
}
