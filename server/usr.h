#ifndef USR_H
#define USR_H
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
class usr:public QObject{
public:
    usr(){

    }
    ~usr(){

    }
    void set_usrName(QString usr){
        this->usrName=usr;
    }
    void set_nickName(){//必须在set_usrName之后使用
        query_mine.exec(QString("select * from userinfo where usr='%1'").arg(this->usrName));
        query_mine.next();
        this->NickName=query_mine.value(1).toString();
    }
    QString get_usrName(){
        return this->usrName;
    }
    QString get_nickName(){
        return this->NickName;
    }
    void set_database(){
        db_mine=QSqlDatabase::database("my_connection");
    }
    void close_database(){
        db_mine.close();
    }
    void set_query(){
        query_mine=QSqlQuery(db_mine);
    }
    QSqlDatabase db_mine;
    QSqlQuery query_mine;
private:
    QString usrName;
    QString NickName;
};

#endif // USR_H
