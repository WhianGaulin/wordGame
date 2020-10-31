#include "board.h"
#include "ui_board.h"

board::board(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::board)
{
    ui->setupUi(this);
    db_board=QSqlDatabase::database("my_connection");
    db_board.open();
}

board::~board()
{
//    db_board.close();
//    qDebug()<<"db_board close...";
    delete ui;
}

void board::InitTableView_builders(){
    ui->tableView->setSortingEnabled(true);

    dataModel=new QStandardItemModel();

    ui->tableView->setModel(dataModel);

    dataModel->setColumnCount(4);
    dataModel->setHeaderData(0,Qt::Horizontal,"昵称");
    dataModel->setHeaderData(1,Qt::Horizontal,"出题数");
    dataModel->setHeaderData(2,Qt::Horizontal,"经验");
    dataModel->setHeaderData(3,Qt::Horizontal,"等级");
}

void board::InitTableView_breakers(){
    ui->tableView->setSortingEnabled(true);

    dataModel=new QStandardItemModel();

    ui->tableView->setModel(dataModel);

    dataModel->setColumnCount(4);
    dataModel->setHeaderData(0,Qt::Horizontal,"昵称");
    dataModel->setHeaderData(1,Qt::Horizontal,"闯关数");
    dataModel->setHeaderData(2,Qt::Horizontal,"经验");
    dataModel->setHeaderData(3,Qt::Horizontal,"等级");
}

void board::on_return_to_game_Btn_clicked()
{
    parentWidget()->show();
    this->close();
}

void board::on_Builder_Btn_clicked()
{
    //dataModel->clear();//这会异常结束
    InitTableView_builders();
    QSqlQuery query(db_board);
    query.exec("select * from builders order by build_words desc");//排序取出
    int i=0;
    QStandardItem *_nick_name;
    QStandardItem *_build_words;
    QStandardItem *_exp;
    QStandardItem *_level;
    while (query.next()) {//问题：内存是否需要释放？？  clear()?? clearItem()
        _nick_name=new QStandardItem;//建立新的内存？？
        _build_words=new QStandardItem;
        _exp=new QStandardItem;
        _level=new QStandardItem;

        qDebug()<<"address:"<<_nick_name<<_build_words<<_exp<<_level;

        _nick_name->setData(query.value(1).toString(),0);
        _build_words->setData(query.value(2).toInt(),0);
        _exp->setData(query.value(3).toInt(),0);
        _level->setData(query.value(4).toInt(),0);

        dataModel->setItem(i,0,_nick_name);//把地址放了进去，内存级别？？
        dataModel->setItem(i,1,_build_words);
        dataModel->setItem(i,2,_exp);
        dataModel->setItem(i,3,_level);

        i++;
    }
    qDebug()<<"--------------------------------------------------";
}

void board::on_breaker_Btn_clicked()
{
    //dataModel->clear();//这会异常结束
    InitTableView_breakers();
    QSqlQuery query(db_board);
    query.exec("select * from breakers order by break_rounds desc");
    int i=0;
    QStandardItem *_nick_name;
    QStandardItem *_break_rounds;
    QStandardItem *_exp;
    QStandardItem *_level;
    while (query.next()) {
        _nick_name=new QStandardItem;//建立新的内存？？
        _break_rounds=new QStandardItem;
        _exp=new QStandardItem;
        _level=new QStandardItem;

        //qDebug()<<"address:"<<_nick_name<<_break_rounds<<_exp<<_level;

        _nick_name->setData(query.value(1).toString(),0);
        _break_rounds->setData(query.value(2).toInt(),0);
        _exp->setData(query.value(3).toInt(),0);
        _level->setData(query.value(4).toInt(),0);

        dataModel->setItem(i,0,_nick_name);//把地址放了进去，内存级别？？
        dataModel->setItem(i,1,_break_rounds);
        dataModel->setItem(i,2,_exp);
        dataModel->setItem(i,3,_level);

        i++;
    }
    qDebug()<<"-------------------------------------------";
}
