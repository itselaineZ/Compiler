#ifndef _ITEMSET_H_
#define _ITEMSET_H_

#include"../base.h"
class ITEM;
class PARSER;

class ITEMSET {
	int num;
    vector<ITEM>group;
    friend class PARSER;
    ITEMSET(){num = 0;}
    void Insert(ITEM x);
    void Clear();
    void GenerateItemSet(PARSER *g);
    bool operator == (const ITEMSET & x)const;
    void Print();
};

#endif
