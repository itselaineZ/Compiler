#ifndef _PARSER_H_
#define _PARSER_H_

#include "../base.h"
#include<QPoint>
#include<QLine>
class TERM;
class ITEM;
class ITEMSET;
class REDUCT;
class STACKSTORE;
class SEMANTICS;
struct POINT{
    string content;
    QPoint pos;
};
class PARSER{
private:
    struct NODE{
      bool isUsed=false;
      string content;
      vector<int> children;
    };
    vector<NODE> tree;
    //vector<map<string,vector<string> > >tree;
    vector<string>Word;
    map<string, vector<TERM> >Grammar;
    map<string, vector<string> >RevGrammar;
    map<string, set<string> >FIRST;
    map<string, set<string> >FOLLOW;
    vector<ITEMSET>I;
    vector<map<string, REDUCT> >LR1;
    vector<STACKSTORE> SSE;

    SEMANTICS *sematics;
    vector<POINT>treePoints;
    vector<QLine>treeLines;

    void InsertToGrammar(string g);
    void ReadGrammar(string addr);
    bool IsFindFirstNull(string nonTerminal);
    void GetFirstSet();

    void ReverseSequence();

    //生成follow集
    void FindFollow(string nonTerminal);
    void GetFollowSet();
    //void GenerateTree();

    
    bool GenerateItemSetFamily();

    friend class ITEM;
    friend class ITEMSET;

public:
    void Begin(string addr);
    bool Analyse(TERM T);
    //输出follow集
    string PrintFollow();
    //输出first集
    string PrintFirst();
    //输出LR1table
    string PrintLR1Table();
    //从、输出栈的过程
    string PrintStack(TERM T);
    void PrintTreeArray();
    string PrintSemantics();
    string PrintError();
    vector<POINT> PrintTreePoints();
    vector<QLine> PrintTreeLines();
};

#endif
