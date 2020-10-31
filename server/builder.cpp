#include "builder.h"

builder::builder()
{

}

builder::builder(QString usrName){
    this->set_database();
    this->set_query();
    this->set_usrName(usrName);
    this->set_nickName();
    if(!get_record()){
        create_record();
        get_record();
    }
    traverse();
}

void builder::create_record(){
    query_mine.exec(QString("insert into builders values('%1','%2',%3,%4,%5)").arg(this->get_usrName()).arg(this->get_nickName()).
                                                                                                            arg(0).arg(0).arg(0));
}

void builder::update_record(){
    query_mine.exec(QString("replace into builders values('%1','%2',%3,%4,%5)").arg(this->get_usrName()).arg(this->get_nickName()).
                                                                               arg(this->build_words).arg(this->exp).arg(this->level));
}

int builder::get_record(){
    query_mine.exec(QString("select * from builders where usr='%1'").arg(this->get_usrName()));
    int b=query_mine.next();
    if(!b)  return 0;
    else {
        this->set_bldwords(query_mine.value(2).toInt());
        this->set_exp(query_mine.value(3).toInt());
        this->set_level(query_mine.value(4).toInt());
        return 1;
    }
}

void builder::traverse(){
    query_mine.exec("select * from builders");
    while (query_mine.next()) {
        qDebug()<<query_mine.value(0).toString()<<query_mine.value(1).toString()<<query_mine.value(2).toInt()<<query_mine.value(3).toInt()
                <<query_mine.value(4).toInt();
    }
}
