#include "../../include/PARSER/REDUCT.h"
#include "../../include/PARSER/ITEM.h"

REDUCT::REDUCT() {
    this->s=-3;//代表不进行任何操作
}
REDUCT::REDUCT(int s) { 
    this->s = s; 
    this->right = TERM();
} //  -2代表接受
void REDUCT::Add(ITEM *item) {
    this->s = -1;
    this->left = item->left;
    this->right = item->right;
}
/*
REDUCT::REDUCT(ITEM *item)
{
    this->s = -1;
    this->left = item->left;
    this->right = item->right; //此处vector的拷贝要查资料确定是否使用循环
}*/
string REDUCT::Print()
{
    string oo;
    if (s == -2){
        //cout << "ACCEPT";
        oo+="ACCEPT";
    }
    else if (s == -1)
    {
        oo+="["+this->left + " -> ";
        //cout << this->left << " -> ";
        oo+=this->right.Print()+"]";
    }
    else{
        char a[20];
        sprintf(a, "%d", this->s);
        oo+="s";
        oo+=a;
        //cout << "s" << this->s;
    }
    return oo;
}
