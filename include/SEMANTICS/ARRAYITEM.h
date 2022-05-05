#ifndef ARRAYITEM_H
#define ARRAYITEM_H

#include "../base.h"

class ARRAYITEM{
    string inxtyp, eltyp;
    int elref, low, high, elsize, size;
public:
    ARRAYITEM(){}
    ARRAYITEM(string inxtyp, string eltyp, int low, int high, int size){
        this->inxtyp = inxtyp;
        this->eltyp = eltyp;
        this->low = low;
        this->high = high;
        this->size = size;
    }
};

#endif // ARRAYITEM_H
