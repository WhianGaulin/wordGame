#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMessageBox>
#include "game.h"
#include "rgst.h"
#include "datactrl.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    game *g;
    rgst *r;

private slots:
    void on_loginBtn_clicked();

    void on_registerBtn_clicked();

    void on_ExitBtn_clicked();

    //void receive_delete(QString);
private:
    Ui::MainWindow *ui;
    Datactrl m_datactrl;//自动初始化
};

#endif // MAINWINDOW_H
