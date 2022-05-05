#include "../../include/SEMANTICS/ATTRIBUTE.h"

string ATTRIBUTE::GetPlace(){
    return this->place;
}
vector<int> ATTRIBUTE::GetTrueList(){
    return this->truelist;
}
vector<int> ATTRIBUTE::GetFalseList(){
    return this->falselist;
}
vector<int> ATTRIBUTE::GetNextList(){
    return this->nextlist;
}
int ATTRIBUTE::GetQuad(){
    return this->quad;
}
void ATTRIBUTE::SetQuad(int quad){
    this->quad=quad;
}
void ATTRIBUTE::SetPlace(string place){
    this->place=place;
}
void ATTRIBUTE::SetLastVar(int lastVar){
    this->lastVar=lastVar;
}
void ATTRIBUTE::SetLastPara(int lastPara){
    this->lastPara=lastPara;
}
int ATTRIBUTE::GetLastVar(){
    return this->lastVar;
}
int ATTRIBUTE::GetLastPara(){
    return this->lastPara;
}
