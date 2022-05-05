#ifndef _ITEM_H_
#define _ITEM_H_

#include "../base.h"
#include "TERM.h"

class REDUCT;
class PARSER;

class ITEM {
    string left;

    friend class REDUCT;
public:
    int pos; // 表示当前dot在第pos个文字之前，从0开始
    TERM right;
    set<string>expect;
    bool operator == (const ITEM & item) const;
    ITEM();
    ITEM(pair<string, int> x, TERM right);
    set<string> FindStringFirst(PARSER *g);// 求当前ITEM和pos对应的后缀（B#）的FIRST集，循环依次找FIRST集直到无空或到达末尾（末尾一定是终结符）
    pair<string, int> FindNextTerm();// 求当前dot后的第一个文字
    bool IsSameProduction(ITEM item);
    void InsertExpectSet(set<string> e);
    ITEM GetNext();
    bool IsEnd();
    void Print();
    string GetLeft();
};

#endif
