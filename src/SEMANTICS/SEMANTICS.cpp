#include "../../include/SEMANTICS/SEMANTICS.h"
#include "../../include/SEMANTICS/SYMBOL.h"
#include "../../include/SEMANTICS/QUATERNION.h"
#include "../../include/SEMANTICS/NAMEITEM.h"
#include "../../include/SEMANTICS/PROCITEM.h"
#include "../../include/SEMANTICS/ARRAYITEM.h"
#include "../../include/SEMANTICS/ATTRIBUTE.h"
#include "../../include/PARSER/TERM.h"

SEMANTICS::SEMANTICS(){
    this->nextquad=0;
    this->tempNum=0;
    this->symbol=new SYMBOL();
}
string SEMANTICS::NewTemp(){
    string t;
    char c[10];
    sprintf(c, "%d", this->tempNum++);
    t=c;
    t="T"+t;
    return t;
}
string SEMANTICS::Print(){
    return this->symbol->PrintCode();
}
string SEMANTICS::PrintError(){
    string err=this->symbol->GetError();
    if(err.length()>9)
        err=err.substr(9);
    return err;
}
void SEMANTICS::AllCtrl(string left,TERM right,vector<string> stream){
    if(left=="FZYJ"){//赋值语句
        if(right.num==4){
            this->Assign(stream[0].substr(2));
        }
        else{
            this->AssignArray(stream[0].substr(2));
        }
    }
    else if(left=="FHYJ"){//返回语句
        if(right.num==2){
            this->Return(false);
        }
        else{
            this->Return(true);
        }
    }
    else if(left=="SCLB"){//实参归结语句
        if(right.num==1){
            this->termtab["SCLB"].push_back(ATTRIBUTE(1));
        }
        else{
            int scNum=this->termtab["SCLB"].back().GetQuad();
            this->termtab["SCLB"].back().SetQuad(scNum+1);
        }
        this->ToPara();
    }
    else if(left=="SMLX"){//声明类型归结语句
        if(right.num==1)
            this->ToSM();
        else
            this->ToSM(stream[1].substr(3));
    }
    else if(left=="HS"||left=="QJBL"||left=="CS"||left=="NBBLSM"){//各种声明
        string type=stream[0];
        string name=stream[1].substr(2);
        string kind,place="null";
        if(left=="HS"){
            kind="func";
            int lastpar=this->termtab["HSSM"].back().GetLastPara();
            int last=this->termtab["HSSM"].back().GetLastVar();
            this->symbol->AddProcTab(PROCITEM(lastpar,last,this->symbol->GetQtNum()-1));
            this->termtab["HSSM"].pop_back();
        }
        else if(left=="QJBL"||left=="NBBLSM"){
            if(right.term[0]!="const"){
                place=this->termtab["SMLX"].back().GetPlace();
                this->termtab["SMLX"].pop_back();
                kind=(place=="var")?place:"array";
            }
            else{
                //set name表里back的kind值
                this->symbol->SetNameTabKind();
                return;
            }
        }
        else if(left=="CS"){//参数归结
            kind="para";

        }
        this->Claim(name,kind,type,left);
        if(place!="null"&&place!="var"){
            //set name表back的num值
            int num=std::stoi(place);
            this->symbol->AddArrayTab(ARRAYITEM("int",type,0,num,num));
        }
    }
    else if(left=="GXBDS"){//关系表达式
        if(right.term[0]=="JFBDS")
            this->Relation();
        else if(right.term[0]=="("||right.term[0]=="!")
            this->Relation(right.term[0],true);
        else if(right.term[0]=="GXBDS")
            this->Relation(stream[1].substr(5),false);
    }
    else if(left=="BDS"||left=="YBDS"){//表达式和与表达式
        string op;
        bool single;
        if(left=="BDS")
            op="or";
        else
            op="and";
        if(right.num==1)
            single=true;
        else
            single=false;
        this->Bool(op,single);
    }
    else if(left=="M"||left=="N"){
        this->EmptyAction(left);
    }
    else if(left=="YZ"){//因子
        string place;
        if(right.num==2){
            if(right.term[1]=="CALL"){
                this->Call(stream[0].substr(2));
            }
            else{
                this->Array(stream[0].substr(2));
            }
        }
        else if(right.num==4){
            this->Array(stream[0].substr(2));
        }
        else{
            if(right.term[0]=="num"){
                place=stream[0].substr(3);
            }
            else if(right.term[0]=="("){
                place=this->termtab["BDS"].back().GetPlace();
                this->termtab["BDS"].pop_back();
            }
            else
                place=stream[0].substr(2);
            this->EleToFactor(place);
        }
    }
    else if(left=="X"){//项
        if(right.num==3)
            this->FactorToItem(right.term[1]);
        else
            this->FactorToItem();
    }
    else if(left=="JFBDS"){//加法表达式
        if(right.num==3)
            this->ItemToExp(right.term[1]);
        else
            this->ItemToExp();
    }
    else if(left=="TJYJ"){//条件语句
        if(right.num>6)
            this->Condition(false);
        else
            this->Condition(true);
    }
    else if(left=="XHYJ"){//循环语句
        this->Circulate();
    }
    else if(left=="YJ"){//语句
        //if(right.term[0]=="TJYJ"||right.term[0]=="XHYJ")
        this->ToState(right.term[0]);
    }
    else if(left=="YJC"){//语句串
        if(right.num==3)
            this->ToStateStream(false);
        else
            this->ToStateStream(true);
    }
    else if(left=="YJK"){//语句块
        this->ToStateBlock();
        if(right.num>3){
            this->termtab["YJK"].back().SetLastVar(this->termtab["NBSM"].back().GetQuad());
            //从最后一个变量顺着link链往上查找是否有重定义出现
            int link=this->termtab["YJK"].back().GetLastVar();
            this->symbol->isNameTabValid(link);
        }
        else{
            this->termtab["YJK"].back().SetLastVar(-1);
        }
    }
    else if(left=="HSSM"){
        this->termtab["HSSM"].push_back(ATTRIBUTE(0));
        this->termtab["HSSM"].back().SetLastVar(this->termtab["YJK"].back().GetLastVar());
        this->termtab["HSSM"].back().SetLastPara(this->termtab["XC"].back().GetLastPara());
        this->termtab["YJK"].pop_back();
        this->termtab["XC"].pop_back();
    }
    else if(left=="SMC"){
        if(right.num==1){
            this->termtab["SMC"].push_back(ATTRIBUTE(this->termtab["SM"].back().GetQuad()));
            this->symbol->SetNameTabLink(this->termtab["SM"].back().GetQuad(),-1);
            this->termtab["SM"].pop_back();
        }
        else{//填link
            int link=this->termtab["SMC"].back().GetQuad();
            this->termtab["SMC"].pop_back();
            this->termtab["SMC"].push_back(ATTRIBUTE(this->termtab["SM"].back().GetQuad()));
            this->symbol->SetNameTabLink(this->termtab["SM"].back().GetQuad(),link);
            this->termtab["SM"].pop_back();
        }
    }
    else if(left=="CSLB"){
        if(right.num==1){
            this->termtab["CSLB"].push_back(ATTRIBUTE(this->termtab["CS"].back().GetQuad()));
            this->symbol->SetNameTabLink(this->termtab["CS"].back().GetQuad(),-1);
            this->termtab["CS"].pop_back();
        }
        else{//填link
            int link=this->termtab["CSLB"].back().GetQuad();
            this->termtab["CSLB"].pop_back();
            this->termtab["CSLB"].push_back(ATTRIBUTE(this->termtab["CS"].back().GetQuad()));
            this->symbol->SetNameTabLink(this->termtab["CS"].back().GetQuad(),link);
            this->termtab["CS"].pop_back();
        }
    }
    else if(left=="XC"){
        if(right.term[0]=="void"){
            this->termtab["XC"].push_back(ATTRIBUTE(0));
            this->termtab["XC"].back().SetLastPara(-1);
        }
        else{
            this->termtab["XC"].push_back(ATTRIBUTE(0));
            this->termtab["XC"].back().SetLastPara(this->termtab["CSLB"].back().GetQuad());
            //从最后一个形参顺着link链往上查找是否有重定义出现
            int link=this->termtab["XC"].back().GetLastPara();
            this->symbol->isNameTabValid(link);
        }
    }
    else if(left=="NBSM"){
        if(right.num==1){
            this->termtab["NBSM"].push_back(ATTRIBUTE(this->termtab["NBBLSM"].back().GetQuad()));
            this->symbol->SetNameTabLink(this->termtab["NBBLSM"].back().GetQuad(),-1);
            this->termtab["NBBLSM"].pop_back();
        }
        else{//填link
            int link=this->termtab["NBSM"].back().GetQuad();
            this->termtab["NBSM"].pop_back();
            this->termtab["NBSM"].push_back(ATTRIBUTE(this->termtab["NBBLSM"].back().GetQuad()));
            this->symbol->SetNameTabLink(this->termtab["NBBLSM"].back().GetQuad(),link);
            this->termtab["NBBLSM"].pop_back();
        }
    }
    else if(left=="SM"){
        this->termtab["SM"].push_back(ATTRIBUTE(this->termtab[right.term[0]].back().GetQuad()));
        this->termtab[right.term[0]].pop_back();
    }
    else if(left=="Program"){
        this->termtab["Program"].push_back(ATTRIBUTE(0));
        this->termtab["Program"].back().SetLastVar(this->termtab["SMC"].back().GetQuad());
        //从最后一个全局变量顺着link链往上查找是否有重定义出现
        int link=this->termtab["Program"].back().GetLastVar();
        this->symbol->isNameTabGlobalValid(link);
    }
    else if(left=="CALL"){
        if(right.num==2){
            this->termtab["CALL"].push_back(ATTRIBUTE(0));
        }
        else{
            int paraNum=this->termtab["SCLB"].back().GetQuad();
            this->termtab["SCLB"].pop_back();
            this->termtab["CALL"].push_back(ATTRIBUTE(paraNum));
        }
    }
}
//对应 "赋值语句 -> id = 表达式"归结时的动作
void SEMANTICS::Assign(string result){
    //if(this->symbol->LookUpNametab(result)){
    string place=this->termtab["BDS"].back().GetPlace();
    this->termtab["BDS"].pop_back();
    this->symbol->AddQuaternion(QUATERNION(place,"=",result));//产生一个赋值语句的四元式
    this->nextquad++;
    vector<int> t=vector<int>();
    this->termtab["FZYJ"].push_back(ATTRIBUTE(t));
    //}
    //else{
    //报错，被赋值的id不存在
    //}
}
//对应 "赋值语句 -> id [ 表达式 ] = 表达式"归结时的动作
void SEMANTICS::AssignArray(string id){
    //if(this->symbol->LookUpNametab(result)){
    string place1=this->termtab["BDS"].back().GetPlace();
    this->termtab["BDS"].pop_back();
    string place2=this->termtab["BDS"].back().GetPlace();
    this->termtab["BDS"].pop_back();
    this->symbol->AddQuaternion(QUATERNION(place1,"-","[]=",id+"["+place2+"]"));//产生一个赋值语句的四元式
    this->nextquad++;
    vector<int> t=vector<int>();
    this->termtab["FZYJ"].push_back(ATTRIBUTE(t));
    //}
    //else{
    //报错，被赋值的id不存在
    //}
}
//对应 "返回语句 -> return ; | return BDS ;"归结时的动作
void SEMANTICS::Return(bool ret){
    if(ret){
        string place=this->termtab["BDS"].back().GetPlace();
        this->termtab["BDS"].pop_back();
        this->symbol->AddQuaternion(QUATERNION("j",-1,place));//产生一个返回语句的四元式
    }
    else{
        this->symbol->AddQuaternion(QUATERNION("j",-1));//产生一个返回语句的四元式
    }
    this->nextquad++;
    vector<int> t=vector<int>();
    this->termtab["FHYJ"].push_back(ATTRIBUTE(t));
}
/*对应声明语句的归结，如下
1.函数 -> int id 函数声明 | double id 函数声明 | float id 函数声明 | char id 函数声明 | string id 函数声明 | void id 函数声明
2.全局变量 -> int id 声明类型 | double id 声明类型 | float id 声明类型 | char id 声明类型 | string id 声明类型 | const 全局变量
3.参数 -> int id | double id | string id | char id | float id
4.内部变量声明 -> int id | double id | char id | float id | string id
*/
void SEMANTICS::Claim(string name, string kind, string type,string left){//kind的传入需要先根据归结的产生式进行判断，可能是var（变量），const（常量），func（函数）和class（类名）的其中之一
    int size=kind!="func"?((type=="int"||type=="float")?4:(type=="double"?8:(type=="char"?1:-1))):0;
    int link=0;//需要改动
    int normal=kind=="func"?1:0;
    this->termtab[left].push_back(ATTRIBUTE(this->symbol->GetNtNum()));//将待填link的序号赋给非终结符属性
    if(kind=="func"){
        this->symbol->AddNameTab(NAMEITEM(name,kind,type,size,link,normal,this->symbol->GetPtNum()-1));
    }
    else if(kind=="array"){
        this->symbol->AddNameTab(NAMEITEM(name,kind,type,size,link,normal,this->symbol->GetAtNum()));
    }
    else{
        this->symbol->AddNameTab(NAMEITEM(name,kind,type,size,link,normal));
    }
}

//对应产生式：关系表达式 -> 加法表达式
void SEMANTICS::Relation(){
    string place=this->termtab["JFBDS"].back().GetPlace();
    this->termtab["GXBDS"].push_back(ATTRIBUTE(place));
    this->termtab["JFBDS"].pop_back();//每当一个非终结符进行归结后就无用了，pop出过期属性
}

//对应产生式：关系表达式1 -> 关系表达式2 relop 加法表达式 以及 关系表达式1 -> ! 关系表达式2 以及 关系表达式1 -> ( 关系表达式2 )
void SEMANTICS::Relation(string op,bool single){
    if(single){
        vector<int> truelist=this->termtab["GXBDS"].back().GetTrueList();
        vector<int> falselist=this->termtab["GXBDS"].back().GetFalseList();
        this->termtab["GXBDS"].pop_back();
        if(op=="!"){
            this->termtab["GXBDS"].push_back(ATTRIBUTE(falselist,truelist));
        }
        else if(op=="("){
            this->termtab["GXBDS"].push_back(ATTRIBUTE(truelist,falselist));
        }
    }
    else{
        string place1=this->termtab["GXBDS"].back().GetPlace();
        string place2=this->termtab["JFBDS"].back().GetPlace();
        this->termtab["GXBDS"].pop_back();
        this->termtab["JFBDS"].pop_back();
        this->termtab["GXBDS"].push_back(ATTRIBUTE(this->nextquad,this->nextquad+1));
        this->symbol->AddQuaternion(QUATERNION("j"+op,place1,place2,-1));//产生关系表达式的四元式，真出口
        this->symbol->AddQuaternion(QUATERNION("j",-1));//产生关系表达式的四元式，假出口
        this->nextquad+=2;
    }
}

//对应产生式：表达式 -> 表达式 || M 与表达式 | 与表达式 以及 与表达式 -> 与表达式 && M 关系表达式 | 关系表达式
void SEMANTICS::Bool(string op,bool single){
    if(op=="or"){
        vector<int>truelist2=this->termtab["YBDS"].back().GetTrueList(),
                falselist2=this->termtab["YBDS"].back().GetFalseList();
        string place=this->termtab["YBDS"].back().GetPlace();
        this->termtab["YBDS"].pop_back();
        if(!single){
            vector<int> truelist1=this->termtab["BDS"].back().GetTrueList(),
                    falselist1=this->termtab["BDS"].back().GetFalseList();
            this->termtab["BDS"].pop_back();
            int quad=this->termtab["M"].back().GetQuad();
            this->termtab["M"].pop_back();
            this->symbol->backpatch(falselist1,quad);
            this->termtab["BDS"].push_back(ATTRIBUTE(this->symbol->MergeList(truelist1,truelist2),falselist2));
        }
        else{
            this->termtab["BDS"].push_back(ATTRIBUTE(truelist2,falselist2));
            this->termtab["BDS"].back().SetPlace(place);
        }
    }
    else if(op=="and"){
        vector<int>truelist2=this->termtab["GXBDS"].back().GetTrueList(),
                falselist2=this->termtab["GXBDS"].back().GetFalseList();
        string place=this->termtab["GXBDS"].back().GetPlace();
        this->termtab["GXBDS"].pop_back();
        if(!single){
            vector<int> truelist1=this->termtab["YBDS"].back().GetTrueList(),
                    falselist1=this->termtab["YBDS"].back().GetFalseList();
            this->termtab["YBDS"].pop_back();
            int quad=this->termtab["M"].back().GetQuad();
            this->termtab["M"].pop_back();
            this->symbol->backpatch(truelist1,quad);
            this->termtab["YBDS"].push_back(ATTRIBUTE(truelist2,this->symbol->MergeList(falselist1,falselist2)));
        }
        else{
            this->termtab["YBDS"].push_back(ATTRIBUTE(truelist2,falselist2));
            this->termtab["YBDS"].back().SetPlace(place);
        }
    }
}

//对应的表达式：M -> @ 以及 N -> @
void SEMANTICS::EmptyAction(string term){
    if(term=="M"){
        this->termtab["M"].push_back(ATTRIBUTE(this->nextquad));
    }
    else if(term=="N"){
        this->termtab["N"].push_back(ATTRIBUTE(this->nextquad,"N"));
        this->symbol->AddQuaternion(QUATERNION("j",-1));//跳转出条件语句
        this->nextquad++;
    }
}

//对应的产生式：因子 -> num | ( 表达式 ) | id,四条产生式本质都是为因子赋一个place
void SEMANTICS::EleToFactor(string place){
    this->termtab["YZ"].push_back(ATTRIBUTE(place));
}

//对应的产生式：因子 -> id CALL
void SEMANTICS::Call(string id){
    string temp=this->NewTemp();
    this->termtab["YZ"].push_back(ATTRIBUTE(temp));
    this->symbol->AddQuaternion(QUATERNION(id,"-","CALL",temp));
    this->nextquad++;
    int paraNum=this->termtab["CALL"].back().GetQuad();
    this->symbol->isProcCallValid(id,paraNum);
}

//对应的产生式：因子 -> id [ BDS ]
void SEMANTICS::Array(string id){
    string temp=this->NewTemp();
    this->termtab["YZ"].push_back(ATTRIBUTE(temp));
    string place=this->termtab["BDS"].back().GetPlace();
    this->termtab["BDS"].pop_back();
    this->symbol->AddQuaternion(QUATERNION(id+"["+place+"]","-","=",temp));
    this->nextquad++;
}

//对应的产生式：项 -> 项 * 因子 | 项 / 因子 | 因子
void SEMANTICS::FactorToItem(string op){
    string place2=this->termtab["YZ"].back().GetPlace();
    this->termtab["YZ"].pop_back();
    if(op!="null"){
        string place1=this->termtab["X"].back().GetPlace();
        this->termtab["X"].pop_back();
        string temp=this->NewTemp();
        this->termtab["X"].push_back((ATTRIBUTE(temp)));
        this->symbol->AddQuaternion(QUATERNION(place1,place2,op,temp));
        this->nextquad++;
    }
    else{
        this->termtab["X"].push_back((ATTRIBUTE(place2)));
    }
}

//对应的产生式：加法表达式 -> 加法表达式 + 项 | 加法表达式 - 项 | 项
void SEMANTICS::ItemToExp(string op){
    string place2=this->termtab["X"].back().GetPlace();
    this->termtab["X"].pop_back();
    if(op!="null"){
        string place1=this->termtab["JFBDS"].back().GetPlace();
        this->termtab["JFBDS"].pop_back();
        string temp=this->NewTemp();
        this->termtab["JFBDS"].push_back((ATTRIBUTE(temp)));
        this->symbol->AddQuaternion(QUATERNION(place1,place2,op,temp));
        this->nextquad++;
    }
    else{
        this->termtab["JFBDS"].push_back((ATTRIBUTE(place2)));
    }
}

//对应的产生式：条件语句 -> if ( 表达式 ) M 语句块 | if ( 表达式 ) M 语句块 N else M 语句块
void SEMANTICS::Condition(bool single){
    int quad2=this->termtab["M"].back().GetQuad();
    this->termtab["M"].pop_back();

    vector<int> nextlist2=this->termtab["YJK"].back().GetNextList();
    this->termtab["YJK"].pop_back();

    vector<int> truelist=this->termtab["BDS"].back().GetTrueList();
    vector<int> falselist=this->termtab["BDS"].back().GetFalseList();
    this->termtab["BDS"].pop_back();

    if(single){
        this->symbol->backpatch(truelist,quad2);
        this->termtab["TJYJ"].push_back(ATTRIBUTE(this->symbol->MergeList(falselist,nextlist2)));
    }
    else{
        int quad1=this->termtab["M"].back().GetQuad();
        this->termtab["M"].pop_back();
        vector<int> n_nextlist=this->termtab["N"].back().GetNextList();
        this->termtab["N"].pop_back();
        vector<int> nextlist1=this->termtab["YJK"].back().GetNextList();
        this->termtab["YJK"].pop_back();

        this->symbol->backpatch(truelist,quad1);
        this->symbol->backpatch(falselist,quad2);
        this->termtab["TJYJ"].push_back(ATTRIBUTE(this->symbol->MergeList(this->symbol->MergeList(nextlist1,nextlist2),n_nextlist)));
    }
}

//对应的产生式：循环语句 -> while M1 ( 表达式 ) M2 语句块
void SEMANTICS::Circulate(){
    int quad2=this->termtab["M"].back().GetQuad();
    this->termtab["M"].pop_back();
    int quad1=this->termtab["M"].back().GetQuad();
    this->termtab["M"].pop_back();

    vector<int> nextlist=this->termtab["YJK"].back().GetNextList();
    this->termtab["YJK"].pop_back();

    vector<int> truelist=this->termtab["BDS"].back().GetTrueList();
    vector<int> falselist=this->termtab["BDS"].back().GetFalseList();
    this->termtab["BDS"].pop_back();

    this->symbol->backpatch(nextlist,quad1);
    this->symbol->backpatch(truelist,quad2);
    this->termtab["XHYJ"].push_back(ATTRIBUTE(falselist));
    this->symbol->AddQuaternion(QUATERNION("j",quad1));
    this->nextquad++;
}

//对应产生式：语句 -> 条件语句 | 循环语句 | 返回语句 | 赋值语句
void SEMANTICS::ToState(string right){
    vector<int> nextlist=this->termtab[right].back().GetNextList();
    this->termtab[right].pop_back();
    this->termtab["YJ"].push_back(ATTRIBUTE(nextlist));
}

//对应产生式：语句串 -> 语句串 M 语句 | 语句
void SEMANTICS::ToStateStream(bool single){
    vector<int> nextlist=this->termtab["YJ"].back().GetNextList();
    this->termtab["YJ"].pop_back();
    if(single){
        this->termtab["YJC"].push_back(ATTRIBUTE(nextlist));
    }
    else{
        vector<int> nextlist1=this->termtab["YJC"].back().GetNextList();
        this->termtab["YJC"].pop_back();
        int quad=this->termtab["M"].back().GetQuad();
        this->termtab["M"].pop_back();
        this->symbol->backpatch(nextlist1,quad);
        this->termtab["YJC"].push_back(ATTRIBUTE(nextlist));
    }
}

//对应产生式：语句块 -> { 内部声明 , 语句串 } | { 语句串 }
void SEMANTICS::ToStateBlock(){
    vector<int> nextlist=this->termtab["YJC"].back().GetNextList();
    this->termtab["YJC"].pop_back();
    this->termtab["YJK"].push_back(ATTRIBUTE(nextlist));
}

//对应产生式：实参列表 -> 实参列表 , 表达式 | 表达式
void SEMANTICS::ToPara(){
    string place=this->termtab["BDS"].back().GetPlace();
    this->termtab["BDS"].pop_back();
    this->symbol->AddQuaternion(QUATERNION("-","-","PAR",place));
    this->nextquad++;
}

//对应产生式：声明类型 -> ; | [ num ] ;
void SEMANTICS::ToSM(string num){
    if(num=="null")
        this->termtab["SMLX"].push_back(ATTRIBUTE("var"));
    else{
        this->termtab["SMLX"].push_back(ATTRIBUTE(num));
    }
}

SYMBOL* SEMANTICS::GetSymbol() {
    return this->symbol;
}
