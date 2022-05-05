#ifndef NAMEITEM_H
#define NAMEITEM_H

#include "../base.h"

class NAMEITEM{
    string name, kind, lev, typ;
    bool normal;
    int ref, addr, size, link;
    double doubleval;
    int intval;
    float floatval;
    char charval;
    string stringval;
public:
    //填name表是在各种声明语句进行归结时，对于一个函数来说，顺序是先填形参，再填内部变量，最后填函数名
    NAMEITEM(){}
    NAMEITEM(string name, string kind, string typ, int size, int link, bool normal){
        this->name = name;
        this->kind = kind;
        this->typ = typ;
        this->size = size;
        this->link = link;
        this->normal = normal;
    }
    NAMEITEM(string name, string kind, string typ, int size, int link, bool normal, int ref){
        this->name = name;
        this->kind = kind;
        this->typ = typ;
        this->size = size;
        this->link = link;
        this->normal = normal;
        this->ref = ref;
    }
    void SetLink(int link);
    void SetKind();
    string GetKind();
    int GetLink();
    int GetRef();
    string GetName();
    void ChangeVal(double doubleval);
    void ChangeVal(int intval);
    void ChangeVal(float floatval);
    void ChangeVal(char charval);
    void ChangeVal(string stringval);
};

#endif // NAMEITEM_H
