#ifndef CODE_H
#define CODE_H

#include "../base.h"

class CODE{
    string op, opdl, opdr, aim, label;
public:
    CODE(const string a, const string b, const string c, const string d) {
        op = a;
        aim = b;
        opdl = c;
        opdr = d;
        label = "";
    }
    CODE(const string a, const string b, const string c) {
        op = a;
        aim = b;
        label = c;
    }
    CODE(const string a, const string b) {
        aim = "";
        op = a;
        label = b;
    }
    CODE(const string a) {
        aim = "";
        op = "";
        label = a;
    }
    void Print() {
        if (this->op == "")
            cout << this->label << ":" << endl;
        else if (this->aim == "")
            cout << this->op << " " << this->label << endl;
        else if (this->label == "")
            cout << this->op << " " << this->aim << " " << this->opdl << " " << this->opdr << endl;
        else
            cout << this->op << " " << this->aim << " " << this->label << endl;
    }
    string PrintCode() {
        if (this->op == "")
            return this->label + ":";
        else if (this->aim == "")
            return this->op + " " + this->label;
        else if (this->label == "")
            return this->op + " " + this->aim + " " + this->opdl + " " + this->opdr;
        else
            return this->op + " " + this->aim + " " + this->label;
    }
};

#endif // CODE_H
