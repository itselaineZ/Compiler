#ifndef WTITEM_H
#define WTITEM_H

#include "../base.h"

class WTITEM{
    pair<int, bool>result, opdl, opdr;
public:
    WTITEM(){
        result = opdl = opdr = make_pair(-1, false);
    }
    WTITEM(pair<int, bool> a, pair<int, bool> b, pair<int, bool> c){
        result = a;
        opdl = b;
        opdr = c;
    }

    pair<int, bool> GetResult();
    pair<int, bool> GetOpdl();
    pair<int, bool> GetOpdr();
    int GetAddr();
};

#endif // WTITEM_H
