#ifndef _STACKSTORE_H_
#define _STACKSTORE_H_
#include "../base.h"

class STACKSTORE{
    vector<int> stat;//状态
    int pos;
public:
    vector<string> symbol;//符号
    STACKSTORE();
    void Add(int st,string sym, int p);
    void Pop();
    string Print();
    int GetPos();
};

#endif
