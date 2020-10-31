#ifndef PLAY1_H
#define PLAY1_H

#include <QMainWindow>
#include <QMessageBox>
#include "datactrl.h"
#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include "breaker.h"
#include "builder.h"
namespace Ui {
class play1;
}

class play1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit play1(QWidget *parent = nullptr);
    ~play1();
    void set_word();
    void set_question();
    void set_player(QString);//在game中被调用
    void keyPressEvent(QKeyEvent *);
    void timerEvent(QTimerEvent *);
    void improve();
    void set_round();

    void inc_exp();

    void get_time_used();

    void get_wordlist_count(int len);

    void set_current_word();

    void set_current_word(int round);

    void UI_display();

    void about_word_builder();
    QString current_word;
    QString builder_nickname;
private slots:
    void on_returnButton_clicked();

    void on_exitButton_clicked();

    void on_startBtn_clicked();

    void on_submitBtn_clicked();
private:
    Ui::play1 *ui;
    Datactrl p1_ctrl;
    QSqlDatabase db_p1;
    QSqlQuery query;
    //QString current_word;
    QString usr;
    int id1=0;
    int id2=0;
    int time_count;//计数器
    int time_used;//花费时间
    int TimeBuffer;//单词显示时间
    int round_count;//闯关数
    int wordlist_sum;//单词总数
    int round_count_wordsum;//关卡的单词数量
    int breaked_word_count;//该关卡已经通过的单词数量
    int len_min;//单词长度
    QString builder_usr;
    //QString builder_nickname;
    breaker *my_breaker;//所有的闯关者的信息
    builder *my_builder;
};

#endif // PLAY1_H
