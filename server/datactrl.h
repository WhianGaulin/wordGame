#ifndef DATACTRL_H
#define DATACTRL_H
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QTime>
class Datactrl:public QObject{
public:
    Datactrl(){
        if(QSqlDatabase::contains("my_connection")){
            qDebug()<<"already exist connection:my_connection";
            db=QSqlDatabase::database("my_connection");
        }
        else{
            db=QSqlDatabase::addDatabase("QSQLITE","my_connection");
            db.setDatabaseName("my.db");
            if(db.open())   qDebug()<<"db open my_connection";
            QSqlQuery("create table if not exists userinfo(usr varchar primary key,nickname varchar,pwd varchar)",db);
            QSqlQuery("create table if not exists wordlist(word varchar primary key,length int,builder_usr varchar,nickname varchar)",db);
            QSqlQuery("create table if not exists breakers(usr varchar primary key,nickname varchar,break_rounds int,exp int,level int)",db);
            QSqlQuery("create table if not exists builders(usr varchar primary key,nickname varchar,build_words int,exp int,level int)",db);
        }
        traverse_usr();
    }
    ~Datactrl(){
        db.commit();
        db.close();
        qDebug()<<"db close...";
    }
    void traverse_usr(){           //用于调试时遍历输出
        QSqlQuery query("select * from userinfo",db);
        while(query.next()){
            QString s1=query.value(0).toString();
            QString s2=query.value(1).toString();
            QString s3=query.value(2).toString();
            qDebug()<<s1<<" "<<s2<<" "<<s3;
        }
    }
    void traverse_word(){
        qDebug()<<"traverse_word";
        QSqlQuery query("select * from wordlist",db);
        while(query.next()){
            QString s=query.value(0).toString();
            int l=query.value(1).toInt();
            qDebug()<<s<<" "<<l<<" "<<query.value(2).toString();
        }
    }
    bool verifyusr(const QString &strName,const QString &strpwd){
        QSqlQuery query(QString("select * from userinfo where usr='%1' and pwd='%2'").arg(strName,strpwd),db);
        return query.next();
    }
    bool usrNameExist(const QString &strName){
        QSqlQuery query(QString("select * from userinfo where usr='%1'").arg(strName),db);
        return  query.next();
    }
    bool NickNameExist(const QString &nickName){
        QSqlQuery query(QString("select * from userinfo where nickname='%1'").arg(nickName),db);
        return query.next();
    }
    int reg(const QString &strName,const QString &NickName,const QString &strpwd,const QString &strpwdcfn){
        if(usrNameExist(strName))   return 1;
        if(strpwd!=strpwdcfn)       return 2;
        if(NickNameExist(NickName)) return 4;
        QSqlQuery query(db);
        bool b=query.exec(QString("insert into userinfo values('%1','%2','%3')").arg(strName).arg(NickName).arg(strpwd));
        if(b)   return 3;
        else    return 0;
    }
    int wordExist(const QString &word){
        QSqlQuery query(QString("select * from wordlist where word='%1'").arg(word),db);
        return query.next();
    }
    int add_word(const QString &word,const QString &builder,const QString &nickname){
        if(wordExist(word)) return 1;
        QSqlQuery query(db);
        bool b=query.exec(QString("insert into wordlist values('%1',%2,'%3','%4')").arg(word).arg(word.length()).arg(builder).arg(nickname));
        if(b)   return 2;
        else {
            qDebug()<<query.lastError();
            return 3;
        }
    }
    QString get_Random_word();
    int get_wordlist_count();
    void delete_usr(QString);
    void set_question();
    int search_usr(QString);
private:
    QSqlDatabase db;
};

#endif // DATACTRL_H
