#include "../../include/SEMANTICS/QUATERNION.h"

bool QUATERNION::IsAsgClause() {
    if (this->op[0] == '=' || this->op[1] == '=')
        return true;
    return false;
}
int QUATERNION::JumpClause() {
    if (this->op[0] == 'j')
        return addr;
    return -2;
}
int QUATERNION::GetAddr(){
    return this->addr;
}
string QUATERNION::GetOpdl(){
    return this->opdl;
}
string QUATERNION::GetOpdr(){
    return this->opdr;
}
string QUATERNION::GetResult(){
    return this->result;
}
string QUATERNION::GetOp() {
    return this->op;
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
