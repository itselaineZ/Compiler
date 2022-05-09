#ifndef PROCITEM_H
#define PROCITEM_H

#include "../base.h"

class PROCITEM{
    int lastpar, last, psize, vsize, endpos;
public:
    PROCITEM(){}
    PROCITEM(int lastpar, int last){
        this->lastpar = lastpar;
        this->last = last;
        this->psize = 0;
        this->vsize = 0;
    }
    PROCITEM(int lastpar, int last, int endpos){
        this->lastpar = lastpar;
        this->last = last;
        this->psize = 0;
        this->vsize = 0;
        this->endpos = endpos;
    }
    void ChangeLast(int last);
    int GetLastPara();
    int GetEndPos();
};

#endif // PROCITEM_H
