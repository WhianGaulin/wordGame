#ifndef PLAY2_H
#define PLAY2_H

#include <QMainWindow>
#include <QMessageBox>
#include "datactrl.h"
#include <QKeyEvent>
#include "builder.h"
namespace Ui {
class play2;
}

class play2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit play2(QWidget *parent = nullptr);
    ~play2();
    void set_player(QString);
    void keyPressEvent(QKeyEvent *);
private slots:
    void on_returnButton_clicked();

    void on_exitButton_clicked();

    void on_submitBtn_clicked();

private:
    Ui::play2 *ui;
    QString usr;//从game获取
    Datactrl p2_ctrl;
    builder *my_builder;
};

#endif // PLAY2_H
