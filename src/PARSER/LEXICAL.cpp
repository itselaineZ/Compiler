#include "../../include/PARSER/LEXICAL.h"

//保留字
const char* reserved[] = {
    "string","void","float","int","char","double","long","break","const","return","case","for","continue","default","switch","if","while","do","else" };//保留字
const int RESERVED_NUM = 19;//保留字的个数

string LEXICAL::ToString(double number){//待解决问题：在词法分析过程中判断常量类型再进行字符串转换
    char buffer[50];
    if(this->numtype=="int")
        sprintf(buffer,"%d",(int)number);//目前是直接转换为整型
    else if(this->numtype=="double")
        sprintf(buffer,"%f",number);
    string str=buffer;
    return str;
}
void LEXICAL::ReadData(string addr){//把读入的字符串去掉注释行存入data
    fstream f;
    f.open(addr.c_str());
    if(!f){
        cout<<"open error!"<<endl;
        exit(0);
    }
    string temp;
	data="";//初始化空data
    while (!f.eof()) {//不忽略空格和换行符读入字符串
		getline(f, temp);
        data=data+temp+"\n";
	}
	f.close();
    int pos=-1;
    temp="";
    int len=data.length();
    while (++pos<len) {
        char ch=data[pos];
		if (ch == '/') {
			ch=data[++pos];
			if (ch == '*') {//"/**/"注释的情况
				ch=data[++pos];
				while(!(ch=='*'&&data[pos+1]=='/')){
					ch=data[++pos];
				}
				continue;
			}
			else if (ch == '/') {// //注释的情况
				while (ch != '\n') {
					ch=data[++pos];
				}
			}
		}
        temp+=ch;
	}
    data=temp;
}
void LEXICAL::Scanner(int & state){
    char ch;
    while (data[nowPos] == ' '||data[nowPos]=='\t') {
		nowPos++;
	}
	ch = data[nowPos];
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')||ch=='_') {//开头是字母或下划线，保留字或标识符
		token = ch;
		ch = data[++nowPos];
		while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_') {
			token += ch;
			ch = data[++nowPos];
		}
		species = 19;
		state = 1;
		if (token[0] == '_') {//确认是标识符
			return;
		}
		string temp;
		for (int i = 0; i < RESERVED_NUM; i++) {
			temp = "";
			temp += reserved[i];
			if (temp==token) {
                species = i;
				break;
			}
		}
	}
	else if (ch >= '0' && ch <= '9') {//接受实常数
        numtype="int";
		number = ch - '0';
		ch = data[++nowPos];
		while (ch >= '0' && ch <= '9') {
			number = number * 10 + ch - '0';
			ch = data[++nowPos];
		}
		if (ch == '.') {
            numtype="double";
			double i = 0;
			ch = data[++nowPos];
			int j = 1;
			while (ch >= '0' && ch <= '9') {
				i = i + (double)(ch - '0') / pow(10.0, j);
				j++;
				ch = data[++nowPos];
			}
			number += i;
		}
		if (ch == 'e' || ch == 'E') {
			ch = data[++nowPos];
			int n = 0;
			char signal = ch;
			if (ch == '-' || ch == '+') {
				ch = data[++nowPos];
				if (!(ch >= '0' && ch <= '9')) {//e、E的后面没有数字，视为出错
					state = -1;
					return;
				}
			}
			else if (!(ch >= '0' && ch <= '9')) {//e、E的后面没有数字，视为出错
				state = -1;
				return;
			}
			while (ch >= '0' && ch <= '9') {
				n = n * 10 + ch - '0';
				ch = data[++nowPos];
			}
			number = number * pow(10.0, n);
			if (signal == '-') {
				number = 0 - number;
			}
		}
		species = 20;
		state = 2;
	}
	else {
		state = 1;
		token = "";
		switch (ch) {
		case '\n':
			state = 3;
			break;
		case '<':
			token = ch;
			if (data[++nowPos] == '=') {
				species = 21;
				token += data[nowPos];
			}
			else {
				species = 22;
				nowPos--;
			}
			break;
		case '>':
			token = ch;
			if (data[++nowPos] == '=') {
				species = 23;
				token += data[nowPos];
			}
			else {
				species = 24;
				nowPos--;
			}
			break;
		case '=':
			token = ch;
			if (data[++nowPos] == '=') {
				species = 25;
				token += data[nowPos];
			}
			else {
				species = 26;
				nowPos--;
			}
			break;
		case '!':
			token = ch;
			if (data[++nowPos] == '=') {
				species = 27;
				token += data[nowPos];
			}
			else {
				species = 28;
				nowPos--;
			}
			break;
		case '&':
			token = ch;
			if (data[++nowPos] == '&') {
				species = 29;
				token += data[nowPos];
			}
			else {
				species = 30;
				nowPos--;
			}
			break;
		case '|':
			token = ch;
			if (data[++nowPos] == '|') {
				species = 31;
				token += data[nowPos];
			}
			else {
				species = 32;
				nowPos--;
			}
			break;
		case '*':species = 33; token += ch; break;
		case '/':species = 34; token += ch; break;
		case '+':species = 35; token += ch; break;
		case '-':species = 36; token += ch; break;
		case ';':species = 37; token += ch; break;
		case '(':species = 38; token += ch; break;
		case ')':species = 39; token += ch; break;
		case '{':species = 40; token += ch; break;
		case '}':species = 41; token += ch; break;
        case '[':species = 42; token += ch; break;
        case ']':species = 43; token += ch; break;
        case ',':species = 44; token += ch; break;
		default:state = -1; break;
		}
		nowPos++;
	}
}
TERM LEXICAL::GetData(){
	return this->finalData;
}
void LEXICAL::Begin(string addr){
    ReadData(addr);
    nowPos=0;
	int len=data.length();
	row=0;
    while(nowPos<len){
        int state = 0;
		Scanner(state);//每当做完一次解析任务后就会返回，state是解析的状态
		if (state == 1) {//成功解析出来一个标志符或一个运算符号
			//cout << "(" << species << "," << token << ")" << endl;
            string symbol;
            if(species<=18){
                //symbol="type";
                symbol=reserved[species];//保留字
            }
            /*else if(species==16){
                symbol="while";
            }
            else if(species==15){
                symbol="if";
            }
            else if(species==18){
                symbol="else";
            }
            else if(species==9){
                symbol="return";
            }*/
            else if(species==19){
                symbol="id"+token;//标识符
            }
            else if(species>=21&&species<=27&&species!=26){
                symbol="relop"+token;//运算符
            }
            else if((species>=28&&species<=44&&species!=29&&species!=31)||species==26){
                symbol=token;//分隔符等等
            }
            else if(species==29){
                symbol="and";//&&
            }
            else if(species==31){
                symbol="or";//||
            }
            else{
                symbol="notuse";//运算符
            }
            LIST l=LIST(species,symbol);
            WORDLIST.insert(make_pair(token,l));
            finalData.Insert(symbol);//给term加入新的终结符串
            //cout<<symbol<<endl;
		}
		else if (state == 2) {//解析出来一个常数
			//cout << "(" << species << "," << number << ")" << endl;
            string symbol=ToString(number);
            //cout<<number<<" "<<symbol<<" "<<numtype<<endl;
            LIST l=LIST(species,symbol);
			WORDLIST.insert(make_pair(ToString(number),l));
            finalData.Insert("num"+symbol);//给term加入新的终结符串
            //cout<<symbol<<endl;
		}
		else if (state == 3) {
			row++;
		}
		else {//出现错误，直接退出
			cout << "error in line " << row + 1 << endl;
			exit(0);
		}
    }
	finalData.Insert(_END);
	finalData.Print();
	cout<<"\n";
}

void LEXICAL::TestBegin(string addr){
    fstream f;
    f.open(addr.c_str());
    if(!f){
        cout<<"open error!"<<endl;
        exit(0);
    }
    string temp;
    getline(f, data);
    f.close();
    int i=0;
    int len=data.length();
    while(i<len){
        string term="";
        while(i<len&&data[i]!=' '){
            term+=data[i++];
        }
        finalData.Insert(term);
        i++;
    }
    finalData.Insert(_END);
    //finalData.Print();
    cout<<"\n";
}
