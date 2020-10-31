#ifndef BREAKER_H
#define BREAKER_H
#include "usr.h"
class breaker:public usr
{
public:
    breaker();
    breaker(QString);
    ~breaker(){
//        this->close_database();
//        qDebug()<<"db(used by breaker) close..";
    }
    void set_break_rounds(int n){
        this->break_rounds=n;
    }
    void set_exp(int n){
        this->exp=n;
    }
    void set_level(int n){
        this->level=n;
    }
    int get_breaked_rounds(){
        return this->break_rounds;
    }
    int get_exp(){
        return this->exp;
    }
    int get_level(){
        return this->level;
    }
    void set_rounds(int r){//有问题
        if(this->break_rounds<r)
            this->break_rounds=r;
    }
//    void inc_rounds(){
//        this->break_rounds++;
//    }
    void inc_exp(){
        this->exp++;
    }
    void inc_exp(int n,int t){
        if(t>3) t=0;
        this->exp+=(n+3-t);
    }
    void set_level(){
        this->level=exp/30;
    }
    void create_record();
    void update_record();
    void traverse();//Debug用
    int get_record();
private:
    int break_rounds;
    int exp;
    int required_exp;//升级所需经验
    int level;
};

#endif // BREAKER_H
