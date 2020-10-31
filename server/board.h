#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QStandardItem>
#include <QStandardItemModel>
namespace Ui {
class board;
}

class board : public QMainWindow
{
    Q_OBJECT

public:
    explicit board(QWidget *parent = nullptr);
    ~board();
    void InitTableView_breakers();
    void InitTableView_builders();
    QSqlDatabase db_board;
    QStandardItemModel *dataModel;

private slots:
    void on_return_to_game_Btn_clicked();

    void on_Builder_Btn_clicked();

    void on_breaker_Btn_clicked();

private:
    Ui::board *ui;
};

#endif // BOARD_H
