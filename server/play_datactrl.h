#ifndef PLAY_DATACTRL_H
#define PLAY_DATACTRL_H
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
//无用!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
class play_datactrl:public QObject{
public:
    play_datactrl(){
        if(QSqlDatabase::contains("play_connection")){
            qDebug()<<"already exists connection:play_connection";
            db=QSqlDatabase::database("play_connection");
        }
        else {
            db=QSqlDatabase::addDatabase("QSQLITE","play_connection");
            db.setDatabaseName("play.db");
            if(db.open())   qDebug()<<"db open play_connection";
            QSqlQuery("create table if not exists wordlist(word varchar primary key,length int)",db);
        }
        traverse();
    }
    ~play_datactrl(){
        db.commit();
        db.close();
    }
    void traverse(){
        QSqlQuery query("select * from wordlist",db);
        while(query.next()){
            QString s=query.value(0).toString();
            int l=query.value(1).toInt();
            qDebug()<<s<<" "<<l;
        }
    }

private:
    QSqlDatabase db;
};

#endif // PLAY_DATACTRL_H
