#include "../../include/SEMANTICS/QUATERNION.h"

int QUATERNION::GetAddr(){
    return this->addr;
}
void QUATERNION::SetAddr(int addr){
    this->addr=addr;
}
string QUATERNION::Print(){
    string out;
    if(result=="."){
        out=this->op+" "+this->opdl+" "+this->opdr+" "+to_string(this->addr)+"\n";
        //cout<<this->op<<" "<<this->opdl<<" "<<this->opdr<<" "<<this->addr<<endl;
    }
    else{
        out=this->op+" "+this->opdl+" "+this->opdr+" "+this->result+"\n";
        //cout<<this->op<<" "<<this->opdl<<" "<<this->opdr<<" "<<this->result<<endl;
    }
    return out;
}
