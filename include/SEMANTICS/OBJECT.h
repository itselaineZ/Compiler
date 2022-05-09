#ifndef OBJECT_H
#define OBJECT_H

#include "../base.h"

class SYMBOL;
class WTITEM;
class CODE;
class QUATERNION;

class OBJECT{
private:
    vector<bool>segflag;
    vector<bool>activevar;
    vector<WTITEM>wttab;
    map<string, stack<int> >avalue;
    map<int, string>label;
    map<int, string>funcname;
    set<string>usefunc;
    vector<stack<string> >rvalue;
    stack<int>reglist;
    vector<CODE>objectcode;
    SYMBOL *symbol;
    string err;
    int mainad;
    void DivFunc();
    void DivSeg();
    void DecideActiveVar();
    void GenWaitActiveTab();
    int GetReg();
    void ReturnReg(int reg);
    void AsignClause(QUATERNION q, int i);
    bool CalClause(QUATERNION q, int i);
    bool JumpSub(QUATERNION q, int i);
    bool FuncClause(int &i, int end);
    string GetOp(string op);
public:
    OBJECT(){
        this->symbol = NULL;
    }
    OBJECT(SYMBOL *symbol){
        this->symbol = symbol;
        stack<string>aa;
        for (int i = 31; i > 0; -- i) { //  31返回地址 30返回值
            this->reglist.push(i);
            this->rvalue.push_back(aa);
        }
    }
    void GenObjectCode();
    void Print();
    string PrintCode();
    string PrintERROR();
};

#endif // OBJECT_H
