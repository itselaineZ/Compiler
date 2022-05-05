#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "../base.h"

class SYMBOL;
class ATTRIBUTE;
class TERM;

class SEMANTICS{
    int nextquad;//跳转地址不明确时设为-1，初始化为0，每生成一个四元式就自动加一
    int tempNum;//临时变量的数目，用于产生新临时变量
    SYMBOL *symbol;
    map<string,vector<ATTRIBUTE> >termtab;
public:
    SEMANTICS();
    string NewTemp();
    void AllCtrl(string left,TERM right,vector<string> stream);//用来做总控，控制哪条产生式对应哪个动作
    void Assign(string result);
    void AssignArray(string id);
    void Return();
    void Claim(string name, string kind, string type,string left);
    void Relation();
    void Relation(string op,bool single);
    void Bool(string op,bool single);
    void EmptyAction(string term);
    void EleToFactor(string place);
    void FactorToItem(string op="null");
    void ItemToExp(string op="null");
    void Condition(bool single);
    void Circulate();
    void ToState(string right);
    void ToStateStream(bool single);
    void ToStateBlock();
    void ToPara();
    string Print();
    void Call(string id);
    void Array(string id);
    void ToSM(string num="null");
    void FillProc();
    string PrintError();
};

#endif // SEMANTICS_H
