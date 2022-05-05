#include "../../include/PARSER/ITEMSET.h"
#include "../../include/PARSER/ITEM.h"
#include "../../include/PARSER/PARSER.h"

void ITEMSET::Insert(ITEM x)
{
    this->group.push_back(x);
    this->num++;
}

void ITEMSET::Clear()
{
    this->num = 0;
    this->group.clear();
}

void ITEMSET::GenerateItemSet(PARSER *g)
{
    for (int i = 0; i < this->num; ++i)
    {
        ITEM now = this->group[i];
        if (now.IsEnd())
            continue;
        pair<string, int> nxt = now.FindNextTerm();
        if (g->Grammar.count(nxt.first) == 0) // dot后面是终结符
            continue;

        set<string> expect = now.FindStringFirst(g); // 找到当前dot后后方的FIRST集

        //  dot后面是非终结符，查Grammar把所有的产生式扔进去
        for (vector<TERM>::iterator it = g->Grammar[nxt.first].begin(); it != g->Grammar[nxt.first].end(); ++it)
        {
            ITEM item = ITEM(make_pair(nxt.first, 0), *it); //  新产生式item的dot位于产生式开头
            bool uniq = true;
            for (int j = 0; j < this->num; ++j)
                if (this->group[j].IsSameProduction(item) == true)
                {
                    this->group[j].InsertExpectSet(expect);
                    uniq = false;
                    break;
                }
            if (uniq)
            {
                item.InsertExpectSet(expect);
                this->Insert(item);
            }
        }
    }
}
bool ITEMSET::operator==(const ITEMSET &x) const
{
    return this->num && x.num && this->group == x.group;
}

void ITEMSET::Print() {
    for(vector<ITEM>::iterator it = this->group.begin(); it != this->group.end(); ++ it) {
        it->Print();
    }
}
