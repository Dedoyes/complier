//我还没写完
#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<string>
#include<cstdio>
#include<map>
#include<set>
using namespace std;

struct token {
    string TokenA;
    int TokenB;
};

struct stmt {
    string Vn;
    vector <string> Vt;
    stmt() {}
    stmt(string _Vn, vector <string>& _Vt) {
        Vn = _Vn;
        for (auto x : _Vt) {
            Vt.push_back(x);
        }
    }
};

struct Law {
    vector <stmt> st;
    Law() {}
    Law(vector <stmt> _st) {
        for (auto x : _st) {
            st.push_back(x);
        }
    }
};


//词法结构
struct Lexical {

    map <string,int> K;//关键字
    map <string,int> P;//界符
    map <string,int> I;//标识符
    int NUMI = 0;
    map <string,int> C1;//常整数
    int NUMC1 = 0;
    map <string,int> C2;//浮点数
    int NUMC2 = 0;
    map <string,int> CT;
    int NUMCT = 0;
    map <string,int> ST;
    int NUMST = 0;

    //存Token序列
    vector <token> Token;

    vector <string> now;
    
    //初始化关键字表和界符表
    void iniK() {
        K["int"] = 1;
        K["float"] = 2;
        K["void"] = 3;
        K["double"] = 4;
        K["bool"] = 5;
        K["for"] = 6;
        K["while"] = 7;
        K["if"] = 8;
        K["else"] = 9;
        K["do"] = 10;
        K["break"] = 11;
        K["main"] = 12;
        K["return"] = 13;
        K["string"] = 14;
        K["char"] = 15;
        K["struct"] = 16;
        K["continue"] = 17;
    }

    void iniP() {
        P["+"] = 1;
        P["-"] = 2;
        P["*"] = 3;
        P["/"] = 4;
        P[","] = 5;
        P[":"] = 6;
        P[";"] = 7;
        P["("] = 8;
        P[")"] = 9;
        P["{"] = 10;
        P["}"] = 11;
        P["["] = 12;
        P["]"] = 13;
        P["+="] = 14;
        P["-="] = 15;
        P["++"] = 16;
        P["--"] = 17;
        P["!"] = 18;
        P["&"] = 19;
        P["&&"] = 20;
        P["||"] = 21;
        P["%"] = 22;
        P["~"] = 23;
        P["="] = 24;
        P["!="] = 25;
        P[">"] = 26;
        P["<"] = 27;
        P[">="] = 28;
        P["<="] = 29;
        P["|"] = 30;
        P["^"] = 31;
        P["\'"] = 32;
        P["\""] = 33;
    }
    
    bool isK(string S) {
        if(K.count(S)) {
            token Var;
            Var.TokenA = "K";
            Var.TokenB = K[S];
            Token.push_back(Var);
            return true;
        }
        else {
            return false;
        }
    }

    bool isP(string S) {
        if(P.count(S)) {
            token Var;
            Var.TokenA = "P";
            Var.TokenB = P[S];
            Token.push_back(Var);
            return true;
        }
        else {
            return false;
        }
    }

    void isI(string S) {
        if(I.count(S)) {
            token Var;
            Var.TokenA = "I";
            Var.TokenB = I[S];
            Token.push_back(Var);
        }
        else if(!I.count(S)) {
            I[S] = ++NUMI;
            token Var;
            Var.TokenA = "I";
            Var.TokenB = I[S];
            Token.push_back(Var);
        }
    }

    void isC1(string S) {
        if(C1.count(S)) {
            token Var;
            Var.TokenA = "C1";
            Var.TokenB = C1[S];
            Token.push_back(Var);
        }
        else if(!C1.count(S)) {
            C1[S] = ++NUMC1;
            token Var;
            Var.TokenA = "C1";
            Var.TokenB = C1[S];
            Token.push_back(Var);
        }
    }

    void isC2(string S) {
        if(C2.count(S)) {
            token Var;
            Var.TokenA = "C2";
            Var.TokenB = C2[S];
            Token.push_back(Var);
        }
        else if(!C2.count(S)) {
            C2[S] = ++NUMC2;
            token Var;
            Var.TokenA = "C2";
            Var.TokenB = C2[S];
            Token.push_back(Var);
        }
    }

    void isCT(string S) {
        if(CT.count(S)) {
            token Var;
            Var.TokenA = "CT";
            Var.TokenB = CT[S];
            Token.push_back(Var);
        }
        else if(!CT.count(S)) {
            CT[S] = ++NUMCT;
            token Var;
            Var.TokenA = "CT";
            Var.TokenB = CT[S];
            Token.push_back(Var);
        }
    }

    void isST(string S) {
        if(ST.count(S)) {
            token Var;
            Var.TokenA = "ST";
            Var.TokenB = ST[S];
            Token.push_back(Var);
        }
        else if(!ST.count(S)) {
            ST[S] = ++NUMST;
            token Var;
            Var.TokenA = "ST";
            Var.TokenB = ST[S];
            Token.push_back(Var);
        }
    }

    //判断是不是数字
    bool isfigure(char S) {
        if(S >= '0' && S <= '9') {
            return true;
        }
        return false;
    }
    //字母
    bool isletter(char S) {
        if((S >= 'A' && S <= 'Z') || (S >= 'a' && S <= 'z') || (S == '_')) {
            return true;
        }
        return false;
    }
    //符号
    bool issymbol(char S) {
        if(!isfigure(S) && !isletter(S) && !isspace(S)) {
            return true;
        }
        return false;
    }
    //空格
    bool isspace(char S) {
        if(S == ' '){
            return true;
        }
        return false;
    }
    //拆分代码
    bool Disassemble(string Coden, int m) {
        string tem;
        int Cleng = Coden.length();
        int i = 0;
        tem += Coden[i];
        for(;i < Cleng - 1;) {
            if(isletter(tem[0])) {
                if(isletter(Coden[i + 1])) {
                    tem += Coden[++i];
                }
                else if(isfigure(Coden[i + 1])) {
                    tem += Coden[++i];
                }
                else if(isspace(Coden[i + 1])) {
                    if(isK(tem)) {
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else{
                        isI(tem);
                        tem.clear();
                        tem += Coden[++i];
                    }
                }
                else if(issymbol(Coden[i + 1])) {
                    if(Coden[i + 1] == '.'){
                        cout << m << "行第" << i << "处字符附近出现错误" << endl;
                        return false;
                    }

                    if(isK(tem)) {
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else{
                        isI(tem);
                        tem.clear();
                        tem += Coden[++i];
                    }
                }
            }
            else if(isfigure(tem[0])) {
                if(isletter(Coden[i + 1])) {
                    if(Coden[i + 1] == 'e') {
                        tem += Coden[++i];
                        if(Coden[i + 1] == '+' || Coden[i + 1] == '-') {
                            tem += Coden[++i];
                        }
                        else if(isfigure(Coden[i + 1])) {
                            tem += Coden[++i];
                        }
                        else {
                            cout << m << "行第" << i << "处字符附近出现错误" << endl;
                            return false;
                        }
                    }
                    else {
                        cout << m << "行第" << i << "处字符附近出现错误" << endl;
                        return false;
                    }
                }
                else if(isfigure(Coden[i + 1])) {
                    tem += Coden[++i];
                }
                else if(isspace(Coden[i + 1])) {
                    int F = 0;
                    for(int have = 0;have <= tem.length();have++) {
                        if(tem[have] == 'e' || tem[have] == '.') {
                            F = 1;
                        }
                    }
                    if(F == 0) {
                        isC1(tem);
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else if(F == 1) {
                        isC2(tem);
                        tem.clear();
                        tem += Coden[++i];
                    }
                }
                else if(issymbol(Coden[i + 1])) {
                    if(Coden[i + 1] == '.') {
                        tem += Coden[++i];
                    }
                    else {
                        int F = 0;
                        for(int have = 0;have <= tem.length();have++) {
                            if(tem[have] == 'e' || tem[have] == '.') {
                                F = 1;
                            }
                        }
                        if(F == 0) {
                            isC1(tem);
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else if(F == 1) {
                            isC2(tem);
                            tem.clear();
                            tem += Coden[++i];
                        }
                    }
                }
            }
            else if(isspace(tem[0])) {
                tem.clear();
                tem += Coden[++i];
            }
            else if(issymbol(tem[0])) {
                
            }
        }
    }

};

int main() {

    //初始化
    struct Lexical Word;
    Word.iniK();
    Word.iniP();


    int n; //假设有n行代码
    cin >> n;
    getchar();
    int clock = 1;
    while (n--) {
        string temp;  // 一行一行代码来  进行词法分析
        getline(cin, temp);
        bool truth = Word.Disassemble(temp,clock++);





    }
    return 0;
}