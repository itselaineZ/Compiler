#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "../base.h"

class ATTRIBUTE{
    string place;
    vector<int> truelist;
    vector<int> falselist;
    vector<int> nextlist;
    int quad;
    int lastVar;
    int lastPara;
public:
    ATTRIBUTE(string place){
        this->place=place;
    }
    ATTRIBUTE(int quad){
        this->quad=quad;
    }
    ATTRIBUTE(int truelist,int falselist){
        this->truelist.push_back(truelist);
        this->falselist.push_back(falselist);
    }
    ATTRIBUTE(vector<int> truelist,vector<int> falselist){
        this->truelist=truelist;
        this->falselist=falselist;
    }
    ATTRIBUTE(int nextlist,string identifiy){
        this->nextlist.push_back(nextlist);
        identifiy="N";
    }
    ATTRIBUTE(vector<int> nextlist){
        this->nextlist=nextlist;
    }
    string GetPlace();
    vector<int> GetTrueList();
    vector<int> GetFalseList();
    vector<int> GetNextList();
    int GetQuad();
    int GetLastVar();
    int GetLastPara();
    void SetPlace(string place);
    void SetLastVar(int lastVar);
    void SetLastPara(int lastPara);
    void SetQuad(int quad);
};

#endif // ATTRIBUTE_H
