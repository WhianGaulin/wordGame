#ifndef BUILDER_H
#define BUILDER_H
#include "usr.h"
class builder:public usr
{
public:
    builder();
    builder(QString);
    ~builder(){
        //this->close_database();
        //qDebug()<<"db(used by builder) close...";
    }
    int get_record();
    void create_record();
    void update_record();
    void traverse();
    void set_bldwords(int n){
        this->build_words=n;
    }
    void set_exp(int n){
        this->exp=n;
    }
    void set_level(int n){
        this->level=n;
    }
    int get_bldwords(){
        return build_words;
    }
    int get_exp(){
        return exp;
    }
    int get_level(){
        return level;
    }
    void inc_bldwords(){
        build_words++;
    }
    void inc_exp(){
        exp++;
        set_level();
    }
    void set_level(){
        level=exp/5;
    }
private:
    int build_words;
    int exp;
    int required_exp;
    int level;
};

#endif // BUILDER_H
