#ifndef PROCITEM_H
#define PROCITEM_H

#include "../base.h"

class PROCITEM{
    int lastpar, last, psize, vsize;
public:
    PROCITEM(){}
    PROCITEM(int lastpar, int last){
        this->lastpar = lastpar;
        this->last = last;
        this->psize = 0;
        this->vsize = 0;
    }
    void ChangeLast(int last);
    int GetLastPara();
};

#endif // PROCITEM_H
