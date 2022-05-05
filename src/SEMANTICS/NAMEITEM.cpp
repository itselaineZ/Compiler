#include "../../include/SEMANTICS/NAMEITEM.h"
int NAMEITEM::GetRef(){
    return this->ref;
}
int NAMEITEM::GetLink(){
    return this->link;
}
void NAMEITEM::SetLink(int link){
    this->link=link;
}
string NAMEITEM::GetKind(){
    return this->kind;
}
string NAMEITEM::GetName(){
    return this->name;
}
void NAMEITEM::ChangeVal(double doubleval){
    this->doubleval = doubleval;
}

void NAMEITEM::ChangeVal(int intval){
    this->intval = intval;
}

void NAMEITEM::ChangeVal(float floatval){
    this->floatval = floatval;
}

void NAMEITEM::ChangeVal(char charval){
    this->charval = charval;
}

void NAMEITEM::ChangeVal(string stringval){
    this->stringval = stringval;
}
void NAMEITEM::SetKind(){
    this->kind="const"+this->kind;
}
