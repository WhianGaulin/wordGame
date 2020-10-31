#include "breaker.h"

breaker::breaker()
{

}

breaker::breaker(QString usr){
    this->set_database();
    this->set_query();
    this->set_usrName(usr);
    this->set_nickName();
    if(!get_record()){
        create_record();
        get_record();
    }
    traverse();
}

void breaker::create_record(){
    query_mine.exec(QString("insert into breakers values('%1','%2',%3,%4,%5)").arg(this->get_usrName()).arg(this->get_nickName()).arg(0).
                                                                     arg(0).arg(0));
}

void breaker::update_record(){
    query_mine.exec(QString("replace into breakers values('%1','%2',%3,%4,%5)").arg(this->get_usrName()).arg(this->get_nickName())
                                                                               .arg(this->break_rounds).arg(this->exp).arg(this->level));
}

int breaker::get_record(){
    query_mine.exec(QString("select * from breakers where usr='%1'").arg(this->get_usrName()));
    int b=query_mine.next();
    if(!b)  return 0;
    else {
        this->set_break_rounds(query_mine.value(2).toInt());
        this->set_exp(query_mine.value(3).toInt());
        this->set_level(query_mine.value(4).toInt());
        return 1;
    }
}

void breaker::traverse(){
    query_mine.exec("select * from breakers");
    while (query_mine.next()) {
        qDebug()<<query_mine.value(0).toString()<<query_mine.value(1).toString()<<query_mine.value(2).toInt()<<query_mine.value(3).toInt()
               <<query_mine.value(4).toInt();
    }
}

