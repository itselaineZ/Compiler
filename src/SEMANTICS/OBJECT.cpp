#include "../../include/SEMANTICS/OBJECT.h"
#include "../../include/SEMANTICS/NAMEITEM.h"
#include "../../include/SEMANTICS/PROCITEM.h"
#include "../../include/SEMANTICS/LAYERITEM.h"
#include "../../include/SEMANTICS/ARRAYITEM.h"
#include "../../include/SEMANTICS/SYMBOL.h"
#include "../../include/SEMANTICS/QUATERNION.h"
#include "../../include/SEMANTICS/WTITEM.h"
#include "../../include/SEMANTICS/CODE.h"

void OBJECT::DivFunc() {
    int len = this->symbol->nametab.size();
    for (int i = 0; i < len; ++ i) if (this->symbol->nametab[i].GetKind() == "func") {
        int proc = this->symbol->nametab[i].GetRef();
        int j;
        for (j = this->symbol->proctab[proc].GetEndPos()-1; j >= 0 &&
             (this->symbol->code[j].GetAddr() != -1 || this->symbol->code[j].GetOp()[0] != 'j'); -- j)
            ;
        ++ j;
        this->funcname[j] = this->symbol->nametab[i].GetName();
        if (this->symbol->nametab[i].GetName() == "main")
            this->mainad = j;
    }
}

void OBJECT::DivSeg() {
    int len = this->symbol->code.size();
    for (int i = 0; i < len; ++ i)  this->segflag.push_back(0);
    this->segflag[0] = 1;
    int jump = 0;
    for (int i = 0; i < len; ++ i) {
        int addr = this->symbol->code[i].JumpClause();
        if (addr != -2) {
            if (addr != -1) {
                this->segflag[addr] = 1;
                //  标号
                this->label[addr] = "J" + to_string(jump++);
            }
            this->segflag[i+1] = 1;
        }
    }
    this->label[len] = "END";
}

void OBJECT::DecideActiveVar() {
    int len = this->symbol->code.size();
    for (int i = 0; i < len; ++ i) this->activevar.push_back(0);
    for (int i = 0; i < len; ) {
        int j;
        for (j = i + 1; j < len && !this->segflag[j]; ++ j);
        for ( ; i < j; ++ i) if (this->symbol->code[i].JumpClause() == -2){ //不是跳转语句
            for (int k = j; k < len; ++ k) {
                if (this->symbol->code[k].GetOpdl() == this->symbol->code[i].GetResult())
                    this->activevar[i] = 1;
                if (this->symbol->code[k].GetOpdr() == this->symbol->code[i].GetResult())
                    this->activevar[i] = 1;
                if (this->symbol->code[k].GetOp() == "PAR" &&
                        this->symbol->code[k].GetResult() == this->symbol->code[i].GetResult())
                    this->activevar[i] = 1;
            }
        }
    }
}

void OBJECT::GenWaitActiveTab() {
    map<string, pair<int, bool>> mp;
    mp["-"] = make_pair(-1, false);
    int len = this->symbol->code.size();
    for (int i = 0; i < len; ++ i)  this->wttab.push_back(WTITEM());
    for (int i = 0; i < len; ) {
        int beg = i;
        for(++ i; i < len && !this->segflag[i]; ++ i);
        for (int j = beg; j < i; ++ j)
            mp[this->symbol->code[j].GetResult()] = make_pair(-1, this->activevar[j]);
        for (int end = i-1; end >= beg; -- end) {
            this->wttab[end] = WTITEM(mp[this->symbol->code[end].GetResult()],
                    mp[this->symbol->code[end].GetOpdl()],
                    mp[this->symbol->code[end].GetOpdr()]);
            mp[this->symbol->code[end].GetOpdl()] = make_pair(end, 1);
            mp[this->symbol->code[end].GetOpdr()] = make_pair(end, 1);
            if (this->symbol->code[end].GetOp() == "PAR")
                mp[this->symbol->code[end].GetResult()] = make_pair(end, 1);
        }
    }
}

int OBJECT::GetReg() {
    if (!this->reglist.empty()){
        int rt = this->reglist.top();
        this->reglist.pop();
        return rt;
    }
    cout << "NO REG CAN USE.\n";
    exit(0);
    return -1;
}

void OBJECT::ReturnReg(int reg) {
    this->reglist.push(reg);
}

string OBJECT::GetOp(string op) {
    if (op == "+")
        return "add";
    else if (op == "-")
        return "sub";
    else if (op == "*")
        return "mul";
    else if (op == "/")
        return "div";
    return "ERROR";
}

void OBJECT::AsignClause(QUATERNION q, int i) {
    string opdr = q.GetOpdl(), opdl = "$0";
    string aim = q.GetResult();
    int reg;
    if (this->avalue.find(aim) == this->avalue.end() || this->avalue[aim].empty()) {
        reg = this->GetReg();
        aim = "$" + to_string(reg);
        this->avalue[q.GetResult()].push(reg);
        this->rvalue[reg].push(q.GetResult());
    }
    else {
        reg = this->avalue[q.GetResult()].top();
        aim = "$" + to_string(reg);
    }
    if (opdr[0] >= '0' && opdr[0] <= '9') {
        opdl = "$0";
        this->objectcode.push_back(CODE("addi", aim, opdl, opdr));
        if (this->wttab[i].GetResult().second == false) {
            this->ReturnReg(reg);
            this->avalue[q.GetResult()].pop();
            this->rvalue[reg].pop();
        }
    }
    else {
        opdr = "$" + to_string(this->avalue[q.GetOpdl()].top());
        this->objectcode.push_back(CODE("add", aim, opdl, opdr));
        if (this->wttab[i].GetOpdr().second == false) {
            this->ReturnReg(this->avalue[q.GetOpdl()].top());
            this->rvalue[this->avalue[q.GetOpdl()].top()].pop();
            this->avalue[q.GetOpdl()].pop();
        }
        if (this->wttab[i].GetResult().second == false) {
            this->ReturnReg(reg);
            this->avalue[q.GetResult()].pop();
            this->rvalue[reg].pop();
        }
    }
}

bool OBJECT::CalClause(QUATERNION q, int i) {
    string opdl = q.GetOpdl();
    string opdr = q.GetOpdr();
    string aim;
    int reg, regl, regr;
    if (opdl[0] >= '0' && opdl[0] <= '9' && opdr[0] >= '0' && opdr[0] <= '9') { //  两个都是常数，计算出结果转赋值语句
        int num = 0;
        if (q.GetOp() == "+")
            num = stoi(opdl) + stoi(opdr);
        else if (q.GetOp() == "-")
            num = stoi(opdl) - stoi(opdr);
        else if (q.GetOp() == "*")
            num = stoi(opdl) * stoi(opdr);
        else if (q.GetOp() == "/")
            num = stoi(opdl) / stoi(opdr);
        this->AsignClause(QUATERNION(to_string(num), "=", q.GetResult()), i);
        return true;
    }
    if (this->avalue.find(q.GetResult()) == this->avalue.end() || this->avalue[q.GetResult()].empty()) {
        reg = this->GetReg();
        aim = "$" + to_string(reg);
        this->avalue[q.GetResult()].push(reg);
        this->rvalue[reg].push(q.GetResult());
    }
    else {
        reg = this->avalue[q.GetResult()].top();
        aim = "$" + to_string(reg);
    }
    if ((opdl[0] >= '0' && opdl[0] <= '9') || (opdr[0] >= '0' && opdr[0] <= '9')) { //  有一个是常数
        bool rev = 0;
        if (opdl[0] >= '0' && opdl[0] <= '9') {
            rev = 1;
            opdr = opdl;
            opdl = q.GetOpdr();
        }
        if (this->avalue.find(opdl) == this->avalue.end() || this->avalue[opdl].empty()) {
            cout << "ERROR: " << opdl << " not exist!\n";
            this->err += "ERROR: " + opdl + " not exist!\n";
            return false;
        }
        else
            regl = this->avalue[opdl].top();
        opdl = "$" + to_string(regl);
        if (q.GetOp() == "+")
            this->objectcode.push_back(CODE("addi", aim, opdl, opdr));
        else {
            regr = this->GetReg();
            this->objectcode.push_back(CODE("addi", "$"+to_string(regr), "$0", opdr));
            this->objectcode.push_back(CODE(this->GetOp(q.GetOp()), aim, opdl, "$"+to_string(regr)));
            this->ReturnReg(regr);
        }
        if (!rev && this->wttab[i].GetOpdl().second == false) {
            this->ReturnReg(regl);
            this->avalue[q.GetOpdl()].pop();
            this->rvalue[regl].pop();
        }
        else if (rev && this->wttab[i].GetOpdr().second == false) {
            this->ReturnReg(regl);
            this->avalue[q.GetOpdr()].pop();
            this->rvalue[regl].pop();
        }
        return true;
    }
    //  非常数
    if (this->avalue.find(opdl) == this->avalue.end() || this->avalue[opdl].empty()) {
        cout << "ERROR: " << opdl << " not exist!\n";
        this->err += "ERROR: " + opdl + " not exist!\n";
        return false;
    }
    else
        regl = this->avalue[opdl].top();
    opdl = "$" + to_string(regl);
    if (this->avalue.find(opdr) == this->avalue.end() || this->avalue[opdr].empty()) {
        cout << "ERROR: " << opdr << " not exist!\n";
        this->err += "ERROR: " + opdr + " not exist!\n";
        return false;
    }
    else
        regr = this->avalue[opdr].top();
    opdr = "$" + to_string(regr);
    this->objectcode.push_back(CODE(this->GetOp(q.GetOp()), aim, opdl, opdr));
    if (this->wttab[i].GetResult().second == false) {
        this->ReturnReg(reg);
        this->avalue[q.GetResult()].pop();
        this->rvalue[reg].pop();
    }
    return true;
}

bool OBJECT::JumpSub(QUATERNION q, int i) {
    int len = this->wttab.size();
    this->wttab.push_back(WTITEM(make_pair(i, true), this->wttab[i].GetOpdl(), this->wttab[i].GetOpdr()));
    int rt = this->CalClause(QUATERNION(q.GetOpdl(), q.GetOpdr(), "-", "NOWTEMP"), len);
    this->wttab.pop_back();
    return rt;
}

bool OBJECT::FuncClause(int &i, int end) {
    stack<int>reglst;
    for (; i < end && this->symbol->code[i].GetOp() == "PAR"; ++ i) {
        for (; i < end && this->symbol->code[i].GetOp() == "PAR"; ++ i) {
            int reg = this->GetReg();
            reglst.push(reg);
            QUATERNION q = this->symbol->code[i];
            if (q.GetResult()[0] >= '0' && q.GetResult()[0] <= '9')
                this->objectcode.push_back(CODE("addi", "$"+to_string(reg), "$0", q.GetResult()));
            else {
                if (this->avalue.find(q.GetResult()) == this->avalue.end() || this->avalue[q.GetResult()].empty()) {
                    cout << "ERROR: " << q.GetResult() << " not exist!\n";
                    this->err += "ERROR: " + q.GetResult() + " not exist!\n";
                    return 0;
                }
                this->objectcode.push_back(CODE("add", "$"+to_string(reg), "$0", "$" + to_string(this->avalue[q.GetResult()].top())));
            }
        }
        int x = this->symbol->FindName(this->symbol->code[i].GetOpdl());
        x = this->symbol->proctab[this->symbol->nametab[x].GetRef()].GetLastPara();
        for (; x != -1 && !reglst.empty(); x = this->symbol->nametab[x].GetLink()) {
            this->avalue[this->symbol->nametab[x].GetName()].push(reglst.top());
            this->rvalue[reglst.top()].push(this->symbol->nametab[x].GetName());
            reglst.pop();
        }
        this->objectcode.push_back(CODE("jal", this->symbol->code[i].GetOpdl()));
        this->avalue[this->symbol->code[i].GetResult()].push(30);
        this->rvalue[30].push(this->symbol->code[i].GetResult());
        this->usefunc.insert(this->symbol->code[i].GetOpdl());
    }
    return 1;
}

void OBJECT::GenObjectCode() {
    this->DivFunc();
    this->DivSeg();
    this->DecideActiveVar();
    this->GenWaitActiveTab();
    int len = this->symbol->code.size();
    int up = len;
    string funcnow;
    this->usefunc.insert("main");
    for (int i = this->mainad; i < up; ) {
        int end;
        for (end = i + 1; end < len && !this->segflag[end]; ++ end);
        for (; i < end; ++ i) {
            QUATERNION q = this->symbol->code[i];
            if (this->funcname.find(i) != this->funcname.end()) {
                if (this->usefunc.find(this->funcname[i]) == this->usefunc.end()) {
                    i = end;
                    break;
                }
                funcnow = this->funcname[i];
                this->objectcode.push_back(CODE(this->funcname[i]));
            }
            if (this->label.find(i) != this->label.end())
                this->objectcode.push_back(CODE(this->label[i]));
            if (q.IsAsgClause()) {  //  赋值表达式
                this->AsignClause(q, i);
            }
            else if (q.GetOp() == "PAR") {
                bool rt = this->FuncClause(i, end);
                if (!rt)
                    return;
            }
            else if (q.JumpClause() == -2) {    //  计算表达式
                bool rt = this->CalClause(q, i);
                if (!rt)
                    return;
            }
            else {
                string op = q.GetOp();
                if (op == "j") {
                    if (q.GetAddr() == -1 && q.GetOpdl() == "-")
                        this->objectcode.push_back(CODE("j", this->label[len]));
                    else if (q.GetAddr() == -1) {
                        if (funcnow != "main") {
                            if (this->avalue.find(q.GetOpdl()) == this->avalue.end() || this->avalue[q.GetOpdl()].empty()) {
                                cout << "ERROR: " << q.GetOpdl() << " not exist!\n";
                                this->err += "ERROR: " + q.GetOpdl() + " not exist!\n";
                                return;
                            }
                            int reg = this->avalue[q.GetOpdl()].top();
                            this->avalue[q.GetOpdl()].pop();
                            this->rvalue[reg].pop();
                            this->objectcode.push_back(CODE("add", "$30", "$0", "$" + to_string(reg)));
                            this->objectcode.push_back(CODE("jr", "$31"));
                        }
                        else
                            this->objectcode.push_back(CODE("j", "END"));
                    }
                    else
                        this->objectcode.push_back(CODE("j", this->label[q.GetAddr()]));
                }
                else {
                    int rt = this->JumpSub(q, i);
                    if (!rt)
                        return;
                    string aim = "$" + to_string(this->avalue["NOWTEMP"].top());
                    if (op == "j<")
                        this->objectcode.push_back(CODE("bltz", aim, this->label[q.GetAddr()]));
                    else if (op == "j<=")
                        this->objectcode.push_back(CODE("blez", aim, this->label[q.GetAddr()]));
                    else if (op == "j=")
                        this->objectcode.push_back(CODE("beq", aim, "$0", this->label[q.GetAddr()]));
                    else if (op == "j>")
                        this->objectcode.push_back(CODE("bgtz", aim, this->label[q.GetAddr()]));
                    else if (op == "j>=")
                        this->objectcode.push_back(CODE("bgez", aim, this->label[q.GetAddr()]));
                }
            }
        }
        if (i == up && up == len)
            i = 0, up = this->mainad;
    }
    this->objectcode.push_back(CODE("END"));
}

void OBJECT::Print() {
    int len = this->objectcode.size();
    for (int i = 0 ; i < len; ++ i) {
        this->objectcode[i].Print();
    }
}

string OBJECT::PrintCode() {
    string rt;
    int len = this->objectcode.size();
    for (int i = 0; i < len; ++ i) {
        rt += this->objectcode[i].PrintCode() + "\n";
    }
    return rt;
}

string OBJECT::PrintERROR() {
    if (this->err.empty())
        this->err = "ObjectCode:\nNo error!\n";
    return this->err;
}
