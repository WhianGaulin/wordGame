#ifndef GAME_H
#define GAME_H
#include "play1.h"
#include "play2.h"
#include <QMainWindow>
#include <QMessageBox>
#include "datactrl.h"
#include "board.h"
namespace Ui {
class game;
}

class game : public QMainWindow
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    ~game();
    play1 *p1;
    play2 *p2;
    board *heroes;
    QString usr;//从界面得到的用户名
private slots:
    void on_play1Btn_clicked();

    void on_play2Btn_clicked();

    void on_exitBtn_clicked();

    void on_returnBtn_clicked();

    void on_deleteBtn_clicked();
//signals:
//    void ask_delete(QString);
    void on_HeroBtn_clicked();

private:
    Ui::game *ui;
    Datactrl g_ctrl;
};

#endif // GAME_H
