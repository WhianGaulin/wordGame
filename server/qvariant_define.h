#ifndef QVARIANT_DEFINE_H
#define QVARIANT_DEFINE_H
#include <QVariant>
typedef struct tag_Breaker_Info{
    QString usrname;
    QString nickname;
    int break_rounds;
    int exp;
    int level;
}Breaker_Info;

typedef struct tag_Builder_Info{
    QString usrname;
    QString nickname;
    int build_words;
    int exp;
    int level;
}Builder_Info;

Q_DECLARE_METATYPE(Breaker_Info)
Q_DECLARE_METATYPE(Builder_Info)


#endif // QVARIANT_DEFINE_H
