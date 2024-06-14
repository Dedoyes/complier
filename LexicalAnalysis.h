#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H

#include<iostream>
#include<vector>
#include<algorithm>
#include<math.h>
#include<string>
#include<cstdio>
#include<map>
#include<set>
#include"Token.h"

using namespace std;

string BreakDown[5070];
int NUMBreak = 0;

//词法结构
struct Lexical {

    map <string, int> K;//关键字
    map <string, int> P;//界符
    map <string, int> I;//标识符
    int NUMI = 0;
    map <string, int> C1;//常整数
    int NUMC1 = 0;
    map <string, int> C2;//浮点数
    int NUMC2 = 0;
    map <string, int> CT;
    int NUMCT = 0;
    map <string, int> ST;
    int NUMST = 0;


    int NUMK = 19;
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
        K["false"] = 18;
        K["true"] = 19;
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
        P["*="] = 33;
        P["/="] = 33;
        P["=="] = 34;
        P["$"] = 35;
    }

    bool isK(string S) {
        if (K.count(S)) {
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
        if (P.count(S)) {
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
        if (I.count(S)) {
            token Var;
            Var.TokenA = "I";
            Var.TokenB = I[S];
            Token.push_back(Var);
        }
        else if (!I.count(S)) {
            I[S] = ++NUMI;
            token Var;
            Var.TokenA = "I";
            Var.TokenB = I[S];
            Token.push_back(Var);
        }
    }

    void isC1(string S) {
        if (C1.count(S)) {
            token Var;
            Var.TokenA = "C1";
            Var.TokenB = C1[S];
            Token.push_back(Var);
        }
        else if (!C1.count(S)) {
            C1[S] = ++NUMC1;
            token Var;
            Var.TokenA = "C1";
            Var.TokenB = C1[S];
            Token.push_back(Var);
        }
    }

    void isC2(string S) {
        if (C2.count(S)) {
            token Var;
            Var.TokenA = "C2";
            Var.TokenB = C2[S];
            Token.push_back(Var);
        }
        else if (!C2.count(S)) {
            C2[S] = ++NUMC2;
            token Var;
            Var.TokenA = "C2";
            Var.TokenB = C2[S];
            Token.push_back(Var);
        }
    }

    void isCT(string S) {
        if (CT.count(S)) {
            token Var;
            Var.TokenA = "CT";
            Var.TokenB = CT[S];
            Token.push_back(Var);
        }
        else if (!CT.count(S)) {
            CT[S] = ++NUMCT;
            token Var;
            Var.TokenA = "CT";
            Var.TokenB = CT[S];
            Token.push_back(Var);
        }
    }

    void isST(string S) {
        if (ST.count(S)) {
            token Var;
            Var.TokenA = "ST";
            Var.TokenB = ST[S];
            Token.push_back(Var);
        }
        else if (!ST.count(S)) {
            ST[S] = ++NUMST;
            token Var;
            Var.TokenA = "ST";
            Var.TokenB = ST[S];
            Token.push_back(Var);
        }
    }

    //判断是不是数字
    bool isfigure(char S) {
        if (S >= '0' && S <= '9') {
            return true;
        }
        return false;
    }
    //字母
    bool isletter(char S) {
        if ((S >= 'A' && S <= 'Z') || (S >= 'a' && S <= 'z') || (S == '_')) {
            return true;
        }
        return false;
    }
    //符号
    bool issymbol(char S) {
        if (!isfigure(S) && !isletter(S) && !isspace(S)) {
            return true;
        }
        return false;
    }
    //空格
    bool isspace(char S) {
        if (S == ' ' || S =='	') {
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
        for (; i < Cleng - 1;) {
            if (isletter(tem[0])) {
                if (isletter(Coden[i + 1])) {
                    tem += Coden[++i];
                }
                else if (isfigure(Coden[i + 1])) {
                    tem += Coden[++i];
                }
                else if (isspace(Coden[i + 1])) {
                    if (isK(tem)) {
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else {
                        token tstr;
                        if (Token.empty()) {
                            isI(tem);
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {
                            tstr = Token.back();
                            if (tstr.TokenB == 16) {
                                K[tem] = ++NUMK;
                                token Va;
                                Va.TokenA = "K";
                                Va.TokenB = K[tem];
                                Token.push_back(Va);
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                            else {
                                isI(tem);
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                    }
                }
                else if (issymbol(Coden[i + 1])) {
                    if (Coden[i + 1] == '.') {
                        cout << m << "行第" << i << "处字符附近出现错误" << endl;
                        return false;
                    }

                    if (isK(tem)) {
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else {
                        token tstr;
                        if (Token.empty()) {
                            isI(tem);
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {
                            tstr = Token.back();
                            if (tstr.TokenB == 16) {
                                K[tem] = ++NUMK;
                                token Va;
                                Va.TokenA = "K";
                                Va.TokenB = K[tem];
                                Token.push_back(Va);
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                            else {
                                isI(tem);
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                    }
                }
            }
            else if (isfigure(tem[0])) {
                if (isletter(Coden[i + 1])) {
                    if (Coden[i + 1] == 'e') {
                        tem += Coden[++i];
                        if (Coden[i + 1] == '+' || Coden[i + 1] == '-') {
                            tem += Coden[++i];
                        }
                        else if (isfigure(Coden[i + 1])) {
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
                else if (isfigure(Coden[i + 1])) {
                    tem += Coden[++i];
                }
                else if (isspace(Coden[i + 1])) {
                    int F = 0;
                    for (int have = 0; have <= tem.length(); have++) {
                        if (tem[have] == 'e' || tem[have] == '.') {
                            F = 1;
                        }
                    }
                    if (F == 0) {
                        isC1(tem);
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else if (F == 1) {
                        isC2(tem);
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                    }
                }
                else if (issymbol(Coden[i + 1])) {
                    if (Coden[i + 1] == '.') {
                        tem += Coden[++i];
                    }
                    else {
                        int F = 0;
                        for (int have = 0; have <= tem.length(); have++) {
                            if (tem[have] == 'e' || tem[have] == '.') {
                                F = 1;
                            }
                        }
                        if (F == 0) {
                            isC1(tem);
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else if (F == 1) {
                            isC2(tem);
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                    }
                }
            }
            else if (isspace(tem[0])) {
                tem.clear();
                tem += Coden[++i];
            }
            else if (issymbol(tem[0])) {
                if (tem[0] == '\'') {
                    if (isP(tem)) {
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                        if (tem[0] == '\\') {
                            tem.clear();
                            tem += Coden[++i];
                        }

                        if (Coden[i + 1] != '\'') {
                            cout << "第" << m << "行出现错误，字符定义错误" << endl;
                            return false;
                        }
                        else {
                            isCT(tem);
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                            if (isP(tem)) {
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                    }
                }
                else if (tem[0] == '\"') {
                    if (isP(tem)) {
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                        if (tem[0] == '\"') {
                            string impor = "";
                            isST(impor);
                            if (isP(tem)) {
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                        else {
                            while (Coden[i + 1] != '\"' || (Coden[i] == '\\' && Coden[i + 1] == '\"')) {
                                tem += Coden[++i];
                            }
                            isST(tem);
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                            if (isP(tem)) {
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                    }
                }
                else if (isletter(Coden[i + 1])) {
                    if (isP(tem)) {
                        BreakDown[++NUMBreak] = tem;
                        tem.clear();
                        tem += Coden[++i];
                    }
                    else {
                        cout << m << "行第" << i << "处字符附近出现错误" << endl;
                        cout << tem << "不存在于界符表里" << endl;
                        return false;
                    }
                }
                else if (isspace(Coden[i + 1])) {
                    if (tem[0] == '-') {
                        token Now = Token.back();
                        if (Now.TokenA == "P") {
                            i++;
                        }
                        else{
                            if(isP(tem)){
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                    }
                    else {
                        if (isP(tem)) {
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {
                            cout << m << "行第" << i << "处字符附近出现错误" << endl;
                            cout << tem << "不存在于界符表里" << endl;
                            return false;
                        }
                    }
                }
                else if (issymbol(Coden[i + 1])) {

                    if (Coden[i + 1] == '\"' || Coden[i + 1] == '\'') {
                        if (isP(tem)) {
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                    }
                    else if (tem[0] != '+' && tem[0] != '-') {
                         tem += Coden[++i];
                         if (isP(tem)) {
                             BreakDown[++NUMBreak] = tem;
                             tem.clear();
                             tem += Coden[++i];
                         }
                         else {
                             string out1, out2;
                             out1 += tem[0];
                             out2 += tem[1];
                             if (isP(out1)) {
                                 if (isP(out2)) {
                                     BreakDown[++NUMBreak] = tem;
                                     tem.clear();;
                                     tem += Coden[++i];
                                 }
                                 else {
                                     cout << m << "行第" << i << "处字符附近出现错误" << endl;
                                     cout << tem << "不存在于界符表里" << endl;
                                     return false;
                                 }
                             }
                             else {
                                 cout << m << "行第" << i << "处字符附近出现错误" << endl;
                                 cout << tem << "不存在于界符表里" << endl;
                                 return false;
                             }
                         }
                    }
                    else if (tem[0] == '+' && Coden[i + 1] == '=') {
                        tem += Coden[++i];
                        if (isP(tem)) {
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {}
                    }
                    else if (tem[0] == '-' && Coden[i + 1] == '=') {
                        tem += Coden[++i];
                        if (isP(tem)) {
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {}
                    }
                    else {
                        int Addnum = 0;
                        int Subnum = 0;

                        if (tem[0] == '+') {
                            Addnum++;
                        }
                        else if (tem[0] == '-') {
                            Subnum++;
                        }
                        while (Coden[i + 1] == '+' || Coden[i + 1] == '-') {
                            if (Coden[i + 1] == '+') {
                                Addnum++;
                            }
                            else if (Coden[i + 1] == '-') {
                                Subnum++;
                            }

                            tem += Coden[++i];
                        }

                        if (Addnum == 2 || Subnum == 2) {
                            if (isP(tem)) {
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                        }
                        else if (Subnum != 0 && Addnum == 0) {
                            if (Subnum % 2 == 0) {
                                for (int j = 0; j < tem.length(); ) {
                                    string spc;
                                    spc += tem[j++];
                                    if (isP(spc)) {}
                                }
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                            else {
                                cout << m << "行" << "出现错误" << endl;
                                return false;
                            }
                        }
                        else if (Addnum != 0 && Subnum == 0) {
                            if (Addnum % 2 == 0) {
                                for (int j = 0; j < tem.length(); ) {
                                    string spc;
                                    spc += tem[j++];
                                    if (isP(spc)) {}
                                }
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                            else {
                                cout << m << "行" << "出现错误" << endl;
                                return false;
                            }
                        }
                    }

                }
                else if (isfigure(Coden[i + 1])) {
                    if (tem[0] == '-') {
                        token Now = Token.back();
                        if (Now.TokenA == "P") {
                            tem += Coden[++i];
                            isC1(tem);
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {
                            if (isP(tem)) {
                                BreakDown[++NUMBreak] = tem;
                                tem.clear();
                                tem += Coden[++i];
                            }
                            else {
                                cout << m << "行" << "出现错误" << endl;
                                return false;
                            }
                        }
                    }
                    else {
                        if (isP(tem)) {
                            BreakDown[++NUMBreak] = tem;
                            tem.clear();
                            tem += Coden[++i];
                        }
                        else {
                            cout << m << "行" << "出现错误" << endl;
                            return false;
                        }
                    }
                }
            }
        }
        if (isletter(tem[0])) {
            if (isK(tem)) {
                BreakDown[++NUMBreak] = tem;
            }
            else {
                isI(tem);
                BreakDown[++NUMBreak] = tem;
            }
        }
        else if (isfigure(tem[0])) {
            int tems = 0;
            for (int sp = 0; sp < tem.length(); sp++) {
                if (tem[sp] == 'e' || tem[sp] == '.') {
                    tems = 1;
                }
            }
            if (tems == 0) {
                isC1(tem);
                BreakDown[++NUMBreak] = tem;
            }
            else {
                isC2(tem);
                BreakDown[++NUMBreak] = tem;
            }
        }
        else if (issymbol(tem[0])) {
            if (isP(tem)) {
                BreakDown[++NUMBreak] = tem;
            }
        }
        return true;
    }

    string revFind(token x) {
        string ret;
        if (x.TokenA == "K") {
            for (map <string, int>::iterator it = K.begin(); it != K.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        else if (x.TokenA == "P") {
            for (map <string, int>::iterator it = P.begin(); it != P.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        else if (x.TokenA == "I") {
            for (map <string, int>::iterator it = I.begin(); it != I.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        else if (x.TokenA == "C1") {
            for (map <string, int>::iterator it = C1.begin(); it != C1.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        else if (x.TokenA == "C2") {
            for (map <string, int>::iterator it = C2.begin(); it != C2.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        else if (x.TokenA == "CT") {
            for (map <string, int>::iterator it = CT.begin(); it != CT.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        else if (x.TokenA == "ST") {
            for (map <string, int>::iterator it = ST.begin(); it != ST.end(); it++)
            {
                if (it->second == x.TokenB)
                    ret = it->first;
            }
        }
        return ret;
    }
};

struct Lexical WordFinal;//词法分析的全局变量 Word





void Lexical_Analysis() {
    WordFinal.iniK();
    WordFinal.iniP();

    //文件输入
    freopen("Program.in", "r", stdin);

    int ClockA = 1;
    string temp; 
    while (getline(cin,temp)) {
        bool truth = WordFinal.Disassemble(temp, ClockA++);
        if (truth) {
        }
        else {
            cout << "词法分析失败" << endl;
        }
    }

    cout << "词法分析成功" << endl;
    for (auto C : WordFinal.Token) {
        cout << "(" << C.TokenA << "," << C.TokenB << ")" << endl;
    }

    fclose(stdin);
    cin.clear();
}

#endif

