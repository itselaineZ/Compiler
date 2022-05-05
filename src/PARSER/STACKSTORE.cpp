#include "../../include/PARSER/STACKSTORE.h"

//初始化，做分析的准备工作
STACKSTORE::STACKSTORE(){
    this->stat.push_back(0);
    this->symbol.push_back(_END);
    pos=0;//分析到的输入串的位置
}
void STACKSTORE::Add(int st,string sym, int p){
    this->stat.push_back(st);
    this->symbol.push_back(sym);
    pos=p;
}
void STACKSTORE::Pop(){
    this->stat.pop_back();
    this->symbol.pop_back();
}
int STACKSTORE::GetPos(){
    return this->pos;
}
string STACKSTORE::Print(){
    string oo;
    for(int i=0;i<(int)this->stat.size();i++){
        char a[20];
        sprintf(a,"%d",stat[i]);
        oo+=a;
        oo+=" ";
        //cout<<stat[i]<<" ";
    }
    oo+="\t\t";
    //cout<<"\t\t";
    for(int i=0;i<(int)this->symbol.size();i++){
        oo+=symbol[i]+" ";
        //cout<<symbol[i]<<" ";
    }
    oo+="\t\t\t\t";
    //cout<<"\t\t\t\t";
    return oo;
}
