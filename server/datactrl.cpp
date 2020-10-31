#include "datactrl.h"
int Datactrl::get_wordlist_count(){
    QSqlQuery query("select count(*) from wordlist",db);
    query.next();
    int count=query.value(0).toInt();
    return  count;
}

QString Datactrl::get_Random_word(){//总感觉还不够随机。。。
    int buffer=get_wordlist_count();
    qDebug()<<"the srand..."<<QTime(0,0,0).secsTo(QTime::currentTime());
    qDebug()<<"total words:"<<buffer;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int rand=qrand()%buffer;
    qDebug()<<"the rand:"<<rand;
    QSqlQuery query(db);
    query.exec("select * from wordlist");
    query.seek(rand);
    QString word=query.value(0).toString();
    return word;
}

void Datactrl::delete_usr(QString usr){
    QSqlQuery query(db);
    query.exec(QString("delete from userinfo where usr='%1'").arg(usr));
    query.exec(QString("delete from breakers where usr='%1'").arg(usr));
    query.exec(QString("delete from builders where usr='%1'").arg(usr));
}

int Datactrl::search_usr(QString usr){
    QSqlQuery query(db);
    query.exec(QString("select * from userinfo where usr='%1'").arg(usr));
    int b=query.next();
    return b;
}
