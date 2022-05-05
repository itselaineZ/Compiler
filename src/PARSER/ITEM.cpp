#include "../../include/PARSER/ITEM.h"
#include "../../include/PARSER/PARSER.h"

bool ITEM::operator == (const ITEM & item) const {
    return this->left == item.left && this->right == item.right && this->pos == item.pos && this->expect == item.expect;
}

ITEM::ITEM(){}

ITEM::ITEM(pair<string, int> x, TERM right) {
    this->left = x.first;
    this->right = right;
    this->pos = x.second;
}
string ITEM::GetLeft(){
    return this->left;
}
set<string> ITEM::FindStringFirst(PARSER *g) {
    set<string> first;
    int p = this->pos+1;
    if (p < this->right.num) {
        if(g->Grammar[this->right.term[p]].size()==0){
            first.insert(this->right.term[p]);
            return first;
        }
        bool varepsilon = false;
        for (; p < this->right.num; p ++) {
            varepsilon = false;
            if(this->right.term[p][0]<'A'||this->right.term[p][0]>'Z'){
                first.insert(this->right.term[p]);
                return first;
            }
            for (set<string>::iterator it = g->FIRST[this->right.term[p]].begin(); it != g->FIRST[this->right.term[p]].end(); ++ it) {
                if (*it != VAREPSILON)
                    first.insert(*it);
                else
                    varepsilon = true;
            }
            if (varepsilon == false)
                break;
        }
        if (varepsilon == false)
            return first;
    }
    for (set<string>::iterator it = this->expect.begin(); it != this->expect.end(); ++ it)
            first.insert(*it);
    return first;
}// 求当前ITEM和pos对应的后缀（B#）的FIRST集，循环依次找FIRST集直到无空或到达末尾（末尾一定是终结符）

pair<string, int> ITEM::FindNextTerm() {
    if (this->IsEnd())  return make_pair("", this->pos);
    return make_pair(this->right.term[this->pos], this->pos+1);
}// 求当前dot后的第一个文字

bool ITEM::IsSameProduction(ITEM item) {
    return this->left == item.left && this->right == item.right && this->pos == item.pos;
}

void ITEM::InsertExpectSet(set<string> e) {
    this->expect.insert(e.begin(), e.end());
}

ITEM ITEM::GetNext() {
    ITEM rt;
    rt.left = this->left;
    rt.right = this->right;
    rt.pos = this->pos+1;
    rt.expect.insert(this->expect.begin(), this->expect.end());
    return rt;
}

bool ITEM::IsEnd() {
    return this->pos == this->right.num;
}

void ITEM::Print() {
    cout << this->left << " -> ";
    this->right.Print();
    cout << " ,";
    for(set<string>::iterator it = this->expect.begin(); it != this->expect.end(); ++ it)
        cout << " " <<  *it;
    cout << " " << this->pos << endl;
}
