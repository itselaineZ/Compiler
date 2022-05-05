#ifndef _TERM_H_
#define _TERM_H_

#include"../base.h"

class TERM{
public:
    int num;
    vector<string>term;
    TERM(){this->num = 0;}
    void Insert(string x)
    {
        this->term.push_back(x);
        this->num++;
    }
    bool operator == (const TERM & x)const
    {
        return this->num == x.num && this->term == x.term;
    }
    string Print()
    {
        string oo="";
        for (int i = 0; i < this->num; ++i){
            oo+=this->term[i];
            oo+=" ";
            //cout << this->term[i] << " ";//需要被注释掉
        }
        return oo;
    }
};

#endif
