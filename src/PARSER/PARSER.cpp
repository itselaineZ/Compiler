#include "../../include/ourclass.h"

void PARSER::Begin(string addr)
{   
    this->sematics=new SEMANTICS();
    this->ReadGrammar(addr);
    //cout << "rrrrr\n";
    this->GetFirstSet();
    //this->PrintFirst();
    //cout << "gggg\n";
    this->GetFollowSet();
    //cout << "zzzz\n";
    //this->PrintFollow();
    //cout << "1111\n";
    this->GenerateItemSetFamily();
    //cout << "2222\n";
    /*for(int i=0;i<I.size();i++){
        I[i].Print();
        cout<<"/////////////\n";
    }*/
    //this->PrintLR1Table();
    //cout << "3333\n";
    //GenerateTree();

    this->sematics->Print();
}

//功能：生成first集
//nonTerminal：非终结符
//所要做的就是求nonTerminal的FIRST集
//该函数返回值：若为true,FIRST集含有ε
/*
First集算法描述:
1.若X->a…，则将终结符a放入First（X）中
2.若X->ε，则将ε放入First（X）中
3.若有X->Y1Y2Y3…Yk，则
（1）把First（Y1）去掉 ε后加入First（X）
（2）如果First（Y1）包含ε，则把First（Y2）也加入到First（X）中，以此类推，
        直到某一个非终结符Yn的First集中不包含ε
（3）如果First（Yk）中包含ε，即Y1~Yn都有ε产生式，就把ε加入到First（X）中
*/
bool PARSER::IsFindFirstNull(string nonTerminal) {
    bool flag = false;//初始化，first集不含空串
    //length为当前非终结符为产生式左部时，产生式的总个数
    //例：S->AB|Ba  length=2
    int length = Grammar[nonTerminal].size();
    //遍历产生式
    for (int i = 0; i < length; i++) {
        string s = Grammar[nonTerminal][i].term[0];//s为当前产生式的右部的第一个字符
        //cout<<nonTerminal<<": "<<s<<endl;
        //若为X->ε（s为空串）
        if (s == VAREPSILON) {
            FIRST[nonTerminal].insert(s);
            flag = true;
        }
        //若为X->a... （s为终结符）
        else if (Grammar.count(s) == 0) {
            FIRST[nonTerminal].insert(s);
        }
        //若为X->Y1Y2Y3…Yk（s为非终结符）
        else {
            if(Grammar[nonTerminal][i].term[0]==nonTerminal)
                continue;//若为左递归，则直接跳过
            for (int j = 0; j < Grammar[nonTerminal][i].num; j++) {
                 s = Grammar[nonTerminal][i].term[j]; //s为当前产生式的右部的第j个字符
                //s的fisrt集不包含终结符,将s的first集加入nonTerminal的first集，并退出当前循环。
                if (IsFindFirstNull(s)==false) {
                    for (set<string>::iterator it = FIRST[s].begin(); it != FIRST[s].end(); it++) {
                        FIRST[nonTerminal].insert(*it);
                    }
                    break;
                }
                //s的fisrt集包含终结符,将s的first集-{ε}加入nonTerminal的first集，并看s的下一个字符
                else {
                    for (set<string>::iterator it = FIRST[s].begin(); it != FIRST[s].end(); it++) {
                        if (*it != VAREPSILON) {
                            FIRST[nonTerminal].insert(*it);
                        }                        
                    }
                    //若s是当前产生式的右部的最后一个字符，将ε加入nonTerminal的first集
                    if (j == Grammar[nonTerminal][i].num - 1) {
                        FIRST[nonTerminal].insert(VAREPSILON);
                        flag = true;
                    }                   
                }
            }
        }
    }
    //此处返回的时候，说明nonTerminal的first集已经求解完毕
    return flag;
}

void PARSER::GetFirstSet()
{
    map<string, vector<TERM> >::iterator iter;
    iter = Grammar.begin();
    while (iter != Grammar.end()) {
        /*cout<<iter->first<<" -> ";
        for(int n=0;n<iter->second.size();n++){
            iter->second[n].Print();
            cout<<"| ";
        }
        cout<<endl;*/
        //如果当前非终结符的first集为空，就调用函数求解
        if (FIRST[iter->first].empty()) {
            IsFindFirstNull(iter->first);
        }
        iter++;
    }
}

//功能：生成follow集
//nonTerminal：非终结符
//所要做的就是求nonTerminal的follow集
/*
1.对于文法开始符号S'，把#加入到Follow（S'）中
2.若有A->aBC，则将First（C）中除了ε之外的元素加入到Follow（B）（此处a可以为空）
3.若有A->aB或者A->aBC且ε属于first（C），则将Follow(A)加入到follow（B）中（此处a可以为空）
4.若有A->Bc,则直接将c加入follow（B）中
follow集中不含有空串ε
*/
/*
可能含有s->BAcBad这种情况 求follow(B)
*/
void PARSER::FindFollow(string nonTerminal) { 
    if (nonTerminal == _S) {
        FOLLOW[_S].insert(_END);
    }
    int length = RevGrammar[nonTerminal].size();
    for (int i = 0; i < length; i++) {
        //leftTerminal为可以产生nonTerminal的左部的非终结符
        string leftTerminal = RevGrammar[nonTerminal][i];
        int leftT_length = Grammar[leftTerminal].size();
        //遍历以leftTerminal为左部的所有产生式
        for (int j = 0; j < leftT_length; j++) {
            //有问题
            //TERM rightPart = Grammar[leftTerminal][j];//rightPart为以leftTerminal为左部的某一个产生式的右部           
            //遍历rightPart看其中是否含有nonTerminal
            for (int z = 0; z < Grammar[leftTerminal][j].num;) {
                //找到了含有nonTerminal的产生式
                if (Grammar[leftTerminal][j].term[z] == nonTerminal) {
                    //nonTerminal后面没有字符了
                    if (z == Grammar[leftTerminal][j].num - 1) {
                        //将leftTerminal的follow集加上；
                        if (leftTerminal != nonTerminal) {
                            if (FOLLOW[leftTerminal].empty())
                                FindFollow(leftTerminal);
                            for (set<string>::iterator it = FOLLOW[leftTerminal].begin(); it != FOLLOW[leftTerminal].end(); it++) {
                                FOLLOW[nonTerminal].insert(*it);
                            }
                        } 
                        z++;
                    }
                    else {
                        //遍历nonTerminal后面的字符
                        for (z++; z < Grammar[leftTerminal][j].num; z++) {
                            //nonTerminal后面接的是终结符
                            if (Grammar.count(Grammar[leftTerminal][j].term[z]) == 0) {
                                FOLLOW[nonTerminal].insert(Grammar[leftTerminal][j].term[z]);
                                break;
                            }
                            //nonTerminal后面接的是nonTerminal
                            /*else if(Grammar[leftTerminal][j].term[z]==nonTerminal){
                                break;
                            }*/
                            //nonTerminal后面接的是非终结符VN
                            else {
                                string VN = Grammar[leftTerminal][j].term[z];
                                int vn_flag = 0;
                                //将VN的first集-空串加入nonTerminal的follow集
                                for (set<string>::iterator it = FIRST[VN].begin(); it != FIRST[VN].end(); it++) {
                                    if (*it != VAREPSILON) {
                                        FOLLOW[nonTerminal].insert(*it);
                                    }
                                    else {
                                        vn_flag = 1;
                                    }
                                }
                                //如果VN不含空串,退出当前循环
                                if (vn_flag == 0) {
                                    break;
                                }
                                else {
                                    //若VN含有空串，则看VN的下一个字符                                    
                                    //VN含有空串且VN为当前产生式的最后一个字符
                                    if (z == Grammar[leftTerminal][j].num - 1) {
                                        //将leftTerminal的follow集加上；
                                        if (leftTerminal != nonTerminal) {
                                            if (FOLLOW[leftTerminal].empty())
                                                FindFollow(leftTerminal);
                                            for (set<string>::iterator it = FOLLOW[leftTerminal].begin(); it != FOLLOW[leftTerminal].end(); it++) {
                                                FOLLOW[nonTerminal].insert(*it);
                                            }
                                        }
                                        z++;
                                    }                                   
                                }                               
                            }
                        }
                    }
                }
                else {
                    z++;
                }
            }
        }
    }
}

void PARSER::GetFollowSet()
{
    map<string, vector<TERM> >::iterator iter;
    iter = Grammar.begin();
    while (iter != Grammar.end()) {  
        /*cout<<iter->first<<" -> ";
        for(int n=0;n<iter->second.size();n++){
            iter->second[n].Print();
            cout<<"| ";
        }
        cout<<endl;*/
        //如果当前非终结符的FOLLOW集为空，就调用函数求解
        if (FOLLOW[iter->first].empty()) {
            FindFollow(iter->first);
        }
        iter++;
    }
}

//输出非终结符的FIRST集
string PARSER::PrintFirst() {
    string out;
    map<string, vector<TERM> >::iterator iter;
    iter = Grammar.begin();
    while (iter != Grammar.end()) {
        string s = iter->first;
        set<string>::iterator it = FIRST[s].begin();
        if(it == FIRST[s].end()){
            iter++;
            continue;
        }
        out+="FIRST(" + s + ") = ";
        //cout << "FIRST(" << s << ") = ";
        for (; it != FIRST[s].end(); it++) {
            out+=*it+" ";
            //cout << *it << ' ';
        }
        out+="\n";
        //cout << endl;
        iter++;
    }
    return out;
}
//输出非终结符的FOLLOW集
string PARSER::PrintFollow() {
    string out;
    map<string, vector<TERM> >::iterator iter;
    iter = Grammar.begin();
    while (iter != Grammar.end()) {
        string s = iter->first;
        set<string>::iterator it = FOLLOW[s].begin();
        if(it == FOLLOW[s].end()){
            iter++;
            continue;
        }
        out+="FOLLOW(" + s + ") = ";
        //cout << "FOLLOW(" << s << ") = ";
        for (set<string>::iterator it = FOLLOW[s].begin(); it != FOLLOW[s].end(); it++) {
            out+=*it+" ";
            //cout << *it << ' ';
        }
        out+="\n";
        //cout << endl;
        iter++;
    }
    return out;
}

//输入格式：S -> a b B | a B
void PARSER::ReadGrammar(string addr){
    fstream f;
    f.open(addr.c_str());
    if(!f){
        cout<<addr<<endl;
        cout<<"open error!"<<endl;
        exit(0);
    }
    string temp,gram;
    bool isGrammar=false;
    bool isStart=true;
    while (!f.eof()) {//不忽略空格和换行符读入字符串
		getline(f, temp);
        if(!isGrammar){//第一行输入所有的终结符串和非终结符串
            int pos=0,len=temp.length();
            string word;
            while(pos<len){
                word=temp[pos];
                while(++pos<len&&temp[pos]!=' '){
                    word=word+temp[pos];
                }
                Word.push_back(word);
                pos++;
            }
            isGrammar=true;
        }
        else if(isStart){//第二行开始是第一条产生式，它的左部是整个文法的开始符
            int p=0;
            string start="";
            while(temp[p]!=' '){
                start=start+temp[p++];//得到非终结符
            }
            string str="START -> "+start;
            this->InsertToGrammar(str);//加入START -> S
            RevGrammar[start].push_back(_S);//加入初始逆文法
            isStart=false;
            this->InsertToGrammar(temp);
            //cout<<temp<<endl;
        }
        else{
            this->InsertToGrammar(temp);
            //cout<<temp<<endl;
        }
	}
    f.close();
}
//输入格式：S -> a b B | a B
void PARSER::InsertToGrammar(string g){
    //map<string, vector<TERM> >Grammar;
    //map<string, vector<string> >RevGrammar;
    int pos=0,len=g.length();
    string leftTerm="";
    string tempRight;
    while(g[pos]!=' '){
        leftTerm=leftTerm+g[pos++];//得到非终结符
    }
    while(g[++pos]!=' '){//跳过"->"
    }
    while(pos<len){
        TERM oneGen;//存储一个产生式右部
        while(++pos<len&&g[pos]!='|'){
            string oneTerm="";
            oneTerm=oneTerm+g[pos];
            while(++pos<len&&g[pos]!=' '){
                oneTerm=oneTerm+g[pos];
            }
            oneGen.Insert(oneTerm);
            if(oneTerm[0]>='A'&&oneTerm[0]<='Z'){//遇到非终结符在产生式右部
                RevGrammar[oneTerm].push_back(leftTerm);//构造逆文法表
            }
        }
        Grammar[leftTerm].push_back(oneGen);
        pos++;
    }
}

bool PARSER::GenerateItemSetFamily()
{
    // 用于创建LR1的空map
    map<string, REDUCT> nullmp;

    ITEMSET itemset;
    if (this->Grammar.count(_S) == 0)
        return false;
    //  取出_S("START")的产生式，将dot置于最前面，展望值为#
    ITEM item = ITEM(make_pair(_S, 0), this->Grammar[_S][0]);
    set<string> expect;
    expect.insert(_END); // _END为"#"
    item.InsertExpectSet(expect);
    itemset.Insert(item);

    //item.Print();

    //  放入完整的I0
    itemset.GenerateItemSet(this);
    this->I.push_back(itemset);
    //cout << "00\n";
    for (int i = 0; i < (int)this->I.size(); ++i)
    { //  不能用迭代器，因为需要对I[i]元素进行修改
        //  创建LR1[i]号空表
        this->LR1.push_back(nullmp);
        //cout<<"Number "<<i<<": "<<endl;
        //this->I[i].Print();
        //cout << "--------\n";
        /*cout << this->Word.size() << endl;
        for (vector<string>::iterator t = this->Word.begin(); t != this->Word.end(); ++t)
            cout << *t << endl;*/

        // 找出dot后是文字*t的产生式
        for (vector<string>::iterator t = this->Word.begin(); t != this->Word.end(); ++t)
        {
            itemset.Clear();
            //cout << (*t) << "\n";
            for (vector<ITEM>::iterator it = this->I[i].group.begin(); it != this->I[i].group.end(); ++it)
                if (!it->IsEnd()){
                    string term = it->FindNextTerm().first;
                    if (term == *t){
                        ITEM xx = it->GetNext();
                        //xx.Print();
                        itemset.Insert(it->GetNext());
                    }
                }
            if(itemset.num) {
                itemset.GenerateItemSet(this); //   产生完整的项目集
                //itemset.Print();
                //cout << "+++++++\n";
                int id = -1;
                for (int j = 0; j < (int)this->I.size() && id == -1; ++j)
                    if (itemset == this->I[j])
                        id = j;

                if (id == -1)
                {
                    id = this->I.size();
                    this->I.push_back(itemset);
                }
                //cout << "id:" << id << endl;
                this->LR1[i][*t] = REDUCT(id);
            }
        }

        //cout << "*******\n";
        //  找出当前项目集I[i]是否可能归约
        for (vector<ITEM>::iterator it = this->I[i].group.begin(); it != this->I[i].group.end(); ++it)
        {
            if (it->IsEnd())
            {
                //it->Print();
                for (set<string>::iterator e = it->expect.begin(); e != it->expect.end(); ++e)
                {
                    if (it->GetLeft()!=_S){
                        this->LR1[i][*e] = REDUCT(),
                        this->LR1[i][*e].Add(&(*it));
                    }
                    else
                        this->LR1[i][*e] = REDUCT(-2);
                }
            }
        }
    }
    return true;
}

string PARSER::PrintLR1Table()
{
    string out;
    int id = 0;
    char a[20];
    for (vector<map<string, REDUCT> >::iterator it = this->LR1.begin(); it != this->LR1.end(); it++)
    {
        sprintf(a, "%d", id);
        out += a;
        out += ": ";
        for (map<string, REDUCT>::iterator i = it->begin(); i != it->end(); i++)
        {
            if(i->second.s==-3){
                continue;
            }
            out+=i->first + " : ";
            //cout << i->first << " : ";
            out+=i->second.Print()+"\t";
            //cout << "\t";
        }
        out+= "\n";
        //cout << "\n";
        id++;
    }
    return out;
}
string PARSER::PrintStack(TERM T){
    string out;
    for(int i=0;i<(int)this->SSE.size();i++){
        out+=SSE[i].Print();
        int pos=SSE[i].GetPos();
        /*for(int j=pos;j<T.num;j++){
            out+=T.term[j]+" ";
            cout<<T.term[j]<<" ";
        }*/
        out+=T.term[pos];
        out+="\n";
        //cout<<'\n';
    }
    return out;
}
string PARSER::PrintSemantics(){
    return this->sematics->Print();
}
bool PARSER::Analyse(TERM T){
    stack<int>stackstat;//状态栈
    stack<string>stacksymbol;//符号栈
    stackstat.push(0);
    stacksymbol.push(_END);
    int pos=0;
    STACKSTORE store;//初始状态
    this->SSE.push_back(store);

    while(LR1[stackstat.top()][T.term[pos]].s!=-2){//不是acc态
        int stattop=stackstat.top();//状态栈栈顶元素
        string symboltop=stacksymbol.top();//符号栈栈顶元素
        string firstterm=T.term[pos];//输入串串首终结符字符串
        //cout<<stattop<<"       "<<symboltop<<"         "<<firstterm<<endl;
        if(symboltop==Grammar[_S][0].term[0]){//符号栈已经是开始状态了，但是仍然不能接受，视为此语句不能被接受
            return false;
        }
        int stat;
        if(firstterm.length()>=2&&firstterm[0]=='i'&&firstterm[1]=='d'){
            stat=LR1[stattop]["id"].s;
        }
        else if(firstterm.length()>=3&&firstterm[0]=='n'&&firstterm[1]=='u'&&firstterm[2]=='m'){
            stat=LR1[stattop]["num"].s;
        }
        else if(firstterm.length()>=3&&firstterm[0]=='r'&&firstterm[1]=='e'&&firstterm[2]=='l'){
            stat=LR1[stattop]["relop"].s;
        }
        else
            stat=LR1[stattop][firstterm].s;
        if(stat>0){//说明要进行移进的操作
            stackstat.push(stat);
            stacksymbol.push(firstterm);
            pos++;
            store.Add(stat,firstterm,pos);
            SSE.push_back(store);
            NODE n;
            n.content=stacksymbol.top();//把一个新的终结符放进数组中
            /*cout<<n.content<<":";
            for(int i=0;i<(int)n.children.size();i++){
                cout<<n.children[i]<<" ";
            }
            cout<<endl;*/
            tree.push_back(n);
        }
        else if(stat==-1){//用产生式进行归结
            string left;
            TERM right;
            if(firstterm.length()>=2&&firstterm[0]=='i'&&firstterm[1]=='d'){
                left=LR1[stattop]["id"].left;
                right=LR1[stattop]["id"].right;
            }
            else if(firstterm.length()>=3&&firstterm[0]=='n'&&firstterm[1]=='u'&&firstterm[2]=='m'){
                left=LR1[stattop]["num"].left;
                right=LR1[stattop]["num"].right;
            }
            else if(firstterm.length()>=3&&firstterm[0]=='r'&&firstterm[1]=='e'&&firstterm[2]=='l'){
                left=LR1[stattop]["relop"].left;
                right=LR1[stattop]["relop"].right;
            }
            else{
                left=LR1[stattop][firstterm].left;
                right=LR1[stattop][firstterm].right;
            }
            NODE n;
            vector<string> stream;
            for(int i=right.num-1,j=1;i>=0;i--,j++){
                if(symboltop==right.term[i]||(symboltop.length()>=2&&symboltop[0]=='i'&&symboltop[1]=='d'&&right.term[i]=="id")||(symboltop.length()>=3&&symboltop[0]=='n'&&symboltop[1]=='u'&&symboltop[2]=='m'&&right.term[i]=="num")||(symboltop.length()>=3&&symboltop[0]=='r'&&symboltop[1]=='e'&&symboltop[2]=='l'&&right.term[i]=="relop")){
                    while(tree[tree.size()-j].isUsed){
                        j++;
                    }
                    n.children.push_back(tree.size()-j);
                    tree[tree.size()-j].isUsed=true;
                    stacksymbol.pop();
                    stream.insert(stream.begin(),symboltop);//将要传入语义分析器
                    symboltop=stacksymbol.top();//新的top
                    store.Pop();
                }
                else{//栈顶元素和要规约的产生式不匹配，认为错误
                    return false;
                }
            }
            stacksymbol.push(left);//把产生式左部的非终结符放进符号栈中
            symboltop=stacksymbol.top();
            n.content=symboltop;
            tree.push_back(n);

            /*cout<<n.content<<":";
            for(int i=0;i<(int)n.children.size();i++){
                cout<<n.children[i]<<" ";
            }
            cout<<endl;*/

            for(int i=0;i<right.num;i++){//把状态栈也弹射出相应数量的状态
                stackstat.pop();
            }
            stattop=stackstat.top();
            stackstat.push(LR1[stattop][symboltop].s);
            store.Add(stackstat.top(),symboltop,pos);
            SSE.push_back(store);

            this->sematics->AllCtrl(left,right,stream);
            //cout<<stackstat.top()<<"       "<<symboltop<<"         "<<firstterm<<endl;
        }
        else if(stat==-3){
            stat=LR1[stattop]["@"].s;
            if(stat>0){
                stackstat.push(stat);
                stacksymbol.push("@");
                store.Add(stat,"@",pos);
                SSE.push_back(store);
                NODE n;
                n.content=stacksymbol.top();//把一个新的终结符放进数组中
                tree.push_back(n);
            }
            else{
                return false;
            }
        }
    }
    //cout<<LR1[stackstat.top()][T.term[pos]].s<<endl;
    //PrintStack(T);
    //cout<<"AAAA:"<<tree.size()<<endl;
    this->ReverseSequence();
    cout<<this->sematics->Print();

    cout<<this->sematics->PrintError();
    return true;
}

void PARSER::ReverseSequence(){
    for(int i=0;i<(int)tree.size();i++){
        reverse(tree[i].children.begin(),tree[i].children.end());
    }
}
void PARSER::PrintTreeArray(){
    NODE n;
    n.content=" ";//放入一个空结点
    if(tree.size()==0){
        tree.push_back(n);
    }
    queue<NODE> Q;
    Q.push(tree[tree.size()-1]);//把起始非终结符放入
    int layer=0;
    POINT point;

    while(!Q.empty()){
        QPoint p;
        QLine l;
        int rowpos=0;
        int len=Q.size();
        NODE n;
        int nextlayernum=0;
        for(int i=0;i<len;i++){
            n=Q.front();
            for(int j=0;j<(int)n.children.size();j++){
                Q.push(tree[n.children[j]]);
                l.setP1(QPoint(rowpos,layer));
                l.setP2(QPoint(nextlayernum,layer+1));
                treeLines.push_back(l);
                nextlayernum++;
            }
            p.setX(rowpos++);
            p.setY(layer);
            point.pos=p;
            point.content=n.content;
            treePoints.push_back(point);
            Q.pop();
        }
        layer++;
    }
    /*for(int i=0;i<(int)treePoints.size();i++){
        cout<<treePoints[i].content<<" "<<treePoints[i].pos.x()<<" "<<treePoints[i].pos.y()<<endl;
    }
    for(int i=0;i<(int)treeLines.size();i++){
        cout<<treeLines[i].p1().x()<<","<<treeLines[i].p1().y()<<" "<<treeLines[i].p2().x()<<","<<treeLines[i].p2().y()<<endl;
    }
    //cout<<"len:"<<tree.size()<<endl;
    for(int i=0;i<len;i++){
        cout<<tree[i].content<<":";
        //cout<<tree[i].children.size();
        for(int j=0;j<(int)tree[i].children.size();j++){
            cout<<tree[i].children[j]<<" ";
        }
        cout<<"\n";
    }*/
}
vector<POINT> PARSER::PrintTreePoints(){
    /*
    POINT p;

    QPoint pp(20,20);
    p.pos=pp;
    p.content="AAAA";

    treePoints.push_back(p);*/
    return this->treePoints;
}

//vector<QLine>treeLines;
vector<QLine> PARSER::PrintTreeLines(){
    return this->treeLines;
}
string PARSER::PrintError(){
    return this->sematics->PrintError();
}
