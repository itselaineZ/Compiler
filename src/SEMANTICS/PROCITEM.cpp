#include "../../include/SEMANTICS/PROCITEM.h"

void PROCITEM::ChangeLast(int last){
    this->last = last;
}
int PROCITEM::GetLastPara(){
    return this->lastpar;
}
int PROCITEM::GetEndPos(){
    return this->endpos;
}
