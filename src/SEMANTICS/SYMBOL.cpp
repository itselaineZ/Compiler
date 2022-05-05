#include "../../include/SEMANTICS/SYMBOL.h"
#include "../../include/SEMANTICS/QUATERNION.h"
#include "../../include/SEMANTICS/NAMEITEM.h"
#include "../../include/SEMANTICS/PROCITEM.h"
#include "../../include/SEMANTICS/LAYERITEM.h"
#include "../../include/SEMANTICS/ARRAYITEM.h"
void SYMBOL::SetNameTabKind(){
    this->nametab.back().SetKind();
}
void SYMBOL::SetNameTabLink(int order,int link){
    this->nametab[order].SetLink(link);
}
string SYMBOL::GetError(){
    return this->err;
}
void SYMBOL::isNameTabValid(int link){
    while (link!=-1) {
        int pre=this->nametab[link].GetLink();
        string name=this->nametab[link].GetName();
        while(pre!=-1){
            if(name==this->nametab[pre].GetName()){
                this->err+="The variable "+name+" is definied several times.\n";
            }
            pre=this->nametab[pre].GetLink();
        }
        link=this->nametab[link].GetLink();
    }
}
void SYMBOL::isNameTabGlobalValid(int link){
    while (link!=-1) {
        int pre=this->nametab[link].GetLink();
        string name=this->nametab[link].GetName();
        string kind=this->nametab[link].GetKind();
        if(kind=="func"){
            link=this->nametab[link].GetLink();
            continue;
        }
        while(pre!=-1){
            if((name==this->nametab[pre].GetName())&&(this->nametab[pre].GetKind()!="func")){
                this->err+="The global variable "+name+" is definied several times.\n";
            }
            pre=this->nametab[pre].GetLink();
        }
        link=this->nametab[link].GetLink();
    }
}
void SYMBOL::isProcCallValid(string id,int paraNum){
    for(int i=0;i<this->GetNtNum();i++){
        string kind=this->nametab[i].GetKind();
        string name=this->nametab[i].GetName();
        if(kind=="func"&&name==id){
            int parnum=0;
            int link=this->proctab[this->nametab[i].GetRef()].GetLastPara();
            while(link!=-1){
                parnum++;
                link=this->nametab[link].GetLink();
            }
            if(parnum!=paraNum){
                this->err+="The function "+id+" is called with mismatched arguments.\n";
            }
            return;
        }
    }
    this->err+="The function "+id+" does not exist.\n";
}
void SYMBOL::backpatch(vector<int> des,int addr){
    for(int i=0;i<(int)des.size();i++){
        int k=des[i];
        while(k!=-1){
            int t=this->code[k].GetAddr();
            this->code[k].SetAddr(addr);
            k=t;
        }
    }
}
vector<int> SYMBOL::MergeList(vector<int> list1,vector<int> list2){
    vector<int> list;
    sort(list1.begin(),list1.end());
    sort(list2.begin(),list2.end());
    list.resize(list1.size()+list2.size());
    merge(list1.begin(),list1.end(),list2.begin(),list2.end(),list.begin());
    return list;
}
void SYMBOL::AddQuaternion(QUATERNION qt){
    this->code.push_back(qt);
}
void SYMBOL::AddNameTab(NAMEITEM nt){
    this->nametab.push_back(nt);
}
void SYMBOL::AddProcTab(PROCITEM pt){
    this->proctab.push_back(pt);
}
void SYMBOL::AddLayerTab(LAYERITEM lt){
    this->layertab.push_back(lt);
}
void SYMBOL::AddArrayTab(ARRAYITEM at){
    this->arraytab.push_back(at);
}
int SYMBOL::GetQtNum(){
    return this->code.size();
}
int SYMBOL::GetNtNum(){
    return this->nametab.size();
}
int SYMBOL::GetPtNum(){
    return this->proctab.size();
}
int SYMBOL::GetLtNum(){
    return this->layertab.size();
}
int SYMBOL::GetAtNum(){
    return this->arraytab.size();
}
bool SYMBOL::LookUpNametab(string id){
    if((count(this->code.begin(),this->code.end(),id))!=0){
        return true;
    }
    return false;
}
string SYMBOL::PrintCode(){
    string out="";
    int len=(int)this->code.size();
    for(int i=0;i<len;i++){
        out+=to_string(i)+": ";
        //cout<<i<<": ";
        out+=this->code[i].Print();
    }
    return out;
}
