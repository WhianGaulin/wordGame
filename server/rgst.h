#ifndef RGST_H
#define RGST_H

#include <QMainWindow>
#include "datactrl.h"
#include <QMessageBox>
namespace Ui {
class rgst;
}

class rgst : public QMainWindow
{
    Q_OBJECT

public:
    explicit rgst(QWidget *parent = nullptr);
    ~rgst();

private slots:
    void on_returnBtn_clicked();

    void on_CfnBtn_clicked();

private:
    Ui::rgst *ui;
    Datactrl r_datactrl;//会自动初始化
};

#endif // RGST_H
