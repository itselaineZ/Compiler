#ifndef SYMBOL_H
#define SYMBOL_H

#include "../base.h"

class QUATERNION;
class NAMEITEM;
class PROCITEM;
class LAYERITEM;
class ARRAYITEM;

class SYMBOL{
    vector<QUATERNION>code;
    vector<NAMEITEM>nametab;
    vector<PROCITEM>proctab;
    vector<LAYERITEM>layertab;
    vector<ARRAYITEM>arraytab;
    string err;
public:
    SYMBOL(){
       this->err="no error!";
    }
    void AddQuaternion(QUATERNION qt);
    void AddNameTab(NAMEITEM nt);
    void AddProcTab(PROCITEM pt);
    void AddLayerTab(LAYERITEM lt);
    void AddArrayTab(ARRAYITEM at);

    void backpatch(vector<int> des,int addr);
    vector<int> MergeList(vector<int> list1,vector<int> list2);

    int GetQtNum();//获取各个表的大小
    int GetNtNum();
    int GetPtNum();
    int GetLtNum();
    int GetAtNum();

    bool LookUpNametab(string id);
    string PrintCode();
    void SetNameTabKind();
    void SetNameTabLink(int order,int link);
    void isNameTabValid(int link);
    void isNameTabGlobalValid(int link);
    void isProcCallValid(string id,int paraNum);
    string GetError();
};

#endif // SYMBOL_H
