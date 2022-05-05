#ifndef _REDUCT_H_
#define _REDUCT_H_

#include "../base.h"
#include "TERM.h"
class ITEM;

class REDUCT{
    int s;//非负表示通过s转移到的状态，-2表示接受
    string left;//s为-1时有效，存储规约式左部
    TERM right;//s为-1时有效，存储规约式右部
    friend class PARSER;
public:
    REDUCT();
    REDUCT(int s); //  -2代表接受
    //REDUCT(ITEM *item);
    void Add(ITEM *item);
    string Print();
};

#endif
