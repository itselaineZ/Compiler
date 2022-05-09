#ifndef QUATERNION_H
#define QUATERNION_H

#include "../base.h"

class QUATERNION{
    string op, opdl, opdr, result;
    int addr;
public:
    bool operator==(basic_string<char>){
        return true;
    }
    QUATERNION(){
        this->addr = -1;
        this->result=".";
    }
    QUATERNION(string op,int addr,string opdl){
        this->opdl = opdl;
        this->opdr = "-";
        this->op = op;
        this->result = ".";
        this->addr = addr;
    }
    QUATERNION(string opd, string op, string result){
        this->opdl = opd;
        this->opdr = "-";
        this->op = op;
        this->result = result;
        this->addr = -1;
    }  // Assign
    QUATERNION(string op, int addr){
        this->op = op;
        this->addr = addr;
        this->opdl="-";
        this->opdr="-";
        this->result=".";
    }    // Jump：无条件
    QUATERNION(string op, string opdl, string opdr, int addr){
        this->op = op;
        this->opdl = opdl;
        this->opdr = opdr;
        this->addr = addr;
        this->result=".";
    }  // Jump：带关系运算符
    QUATERNION(string op, string opd, int addr){
        this->op = op;
        this->opdl = opd;
        this->opdr="-";
        this->addr = addr;
        this->result=".";
    }    // Jump：为真时跳转
    QUATERNION(string opdl, string opdr, string op, string result){
        this->opdl = opdl;
        this->opdr = opdr;
        this->op = op;
        this->result = result;
        this->addr = -1;
    }  // Calc
    bool IsAsgClause();
    int JumpClause();
    int GetAddr();
    string GetOpdl();
    string GetOpdr();
    string GetOp();
    string GetResult();
    void SetAddr(int addr);
    string Print();
};

#endif // QUATERNION_H
