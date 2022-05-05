#ifndef _LEXICAL_H_
#define _LEXICAL_H_

#include "../base.h"
#include "TERM.h"
class LIST{
    int id;
    string symbol;
public:
    LIST(int i,string s){
        id=i;
        symbol=s;
    }
};
class LEXICAL{
private:
    int species;//当前判断出的单词种别
    string token;//当前判断出的标识符/运算符
    double number;//当前计算出的常数值
    string numtype;
    int row;//当前处理到代码的哪一行
    int nowPos;//当前处理到代码的哪个位置
    string data;//读入的字符串
    TERM finalData;//最终的字符串存储
    map<string,LIST> WORDLIST;//构造出来的单词表

    string ToString(double number);
    void ReadData(string addr);//从文件中读入字符串到data中，并做初步处理
    void Scanner(int &);//对初步处理好的字符串做扫描，构造单词表，转化为新字符串
public:
    TERM GetData();
    void Begin(string addr);
    void TestBegin(string addr);
};

#endif
