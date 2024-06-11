#ifndef __SCANNER_H
#define __SCANNER_H

#include <bits/stdc++.h>
#include "Law.h"

using namespace std;

inline string ChartoStr (char c) {   // char 类型转 string 类型函数
    string res = "";
    res += c;
    return res;
}

map <string, int> nature;                     // 属性，为 1 是 Vn, 为 2 是 Vt
map <string, set <string> > first, follow;    // first, follow 集合
set <char> word;                              // 字集合， U - {$}
Law law;                                      // 文法表
string programe, lawstr;                      // 源程序, 文法字符串
set <string> Vt, Vn;                          // 终结符集合Vt, 非终结符集合 Vn
const char* pathLaw =                         // 本地目录名称
    "C:\\Users\\lumia\\CLionProjects\\cf1969\\Team_complier\\Law.in";
const char* pathPrograme =
    "C:\\Users\\lumia\\CLionProjects\\cf1969\\Team_complier\\Program.in";
map <string, vector <string> > lawG;          // 文法拓扑图
map <string, int> lawindeg, lawoutdeg;        // 文法出入度

inline void initLaw () {                      // 用 law.in 初始化文法
    freopen (pathLaw, "r", stdin);
    string temp;
    while (getline (cin, temp)) {
        lawstr += temp;
        lawstr += '\n';
    }
    fclose (stdin);
    int len = lawstr.size () - 1;
    for (int i = 0; i <= len; i++) {
        if (lawstr[i] == '-' && lawstr[i + 1] == '>') {
            int r = i - 1;
            int l = i - 1;
            for (; l >= 0; l--) {
                if (lawstr[l] == '\n') {
                    l++; break;
                }
            }
            string tempVn;
            for (int j = l; j <= r; j++) {
                if (lawstr[j] == ' ') continue;
                tempVn += lawstr[j];
            }
            l = i + 2;
            int endpos = l;
            for (; endpos <= len; endpos++) {
                if (lawstr[endpos] == '\n') {
                    break;
                }
            }
            for (r = l; l <= endpos && r <= endpos; l = r + 1) {
                r = l;
                for (; r <= endpos; r++) {
                    if (lawstr[r] == '|' || lawstr[r] == '\n') {
                        vector <string> tempVt;
                        r--;
                        string tempVtStr;
                        for (int j = l; j <= r; j++) {
                            if (lawstr[j] == ' ') {
                                if (tempVtStr.size ()) {
                                    tempVt.push_back (tempVtStr);
                                    //cout << "tempVtStr = " << "[" << tempVtStr << "]" << endl;
                                }
                                tempVtStr = "";
                            } else {
                                tempVtStr += lawstr[j];
                            }
                        }
                        if (tempVtStr.size ())
                            tempVt.push_back (tempVtStr);
                        string finalSt;
                        if (tempVt.size ())
                            law.st.push_back (stmt (tempVn, tempVt));
                        //stmt (tempVn, tempVt).print ();
                        r++;
                        break;
                    }
                }
            }
        }
    }
    string tempVn = "opt2";
    string tempStr = "|";
    vector <string> tempVt;
    tempVt.push_back (tempStr);
    law.st.push_back (stmt (tempVn, tempVt));
    tempVt[0] = "||";
    law.st.push_back (stmt (tempVn, tempVt));
    tempVn = "spcletter";
    tempVt[0] = "|";
    law.st.push_back (stmt (tempVn, tempVt));
    sort (law.st.begin (), law.st.end ());
    cin.clear ();
}

inline void initword () {                               // 初始化字表
    for (char c = '0'; c <= '9'; c++)
        word.insert (c);
    for (char c = 'a'; c <= 'z'; c++)
        word.insert (c);
    for (char c = 'A'; c <= 'Z'; c++)
        word.insert (c);
    string spcletter = "!@#%^&*()_+=[]{}|;',<>./?\"";
    for (auto c : spcletter) {
        word.insert (c);
    }
}

inline void initVnVt () {                               // 初始化 Vn, Vt
    for (auto stmt : law.st) {
        string s = stmt.Vn;
        Vn.insert (s);
        for (auto x : stmt.Vt) {
            Vt.insert (x);
        }
    }
    for (auto x : Vn) {
        auto it = Vt.lower_bound (x);
        if (it == Vt.end ()) continue;
        if (*it == x) {
            Vt.erase (x);
        }
    }
    for (auto x : Vn) {
        nature[x] = 1;
    }
    for (auto x : Vt) {
        nature[x] = 2;
    }
}

inline void printVn () {                        // 输出 Vn
    cout << "Vn : " << endl;
    for (auto x : Vn) {
        cout << x << endl;
    }
}

inline void printVt () {                        // 输出 Vt
    cout << "Vt : " << endl;
    for (auto x : Vt) {
        cout << x << endl;
    }
}

inline void printFirst () {                  // 输出 first 集合
    for (auto x : Vn) {
        cout << "first[" << x << "] = {";
        for (auto y : first[x]) {
            cout << y << " ";
        }
        cout << "}" << endl;
    }
}

inline void printFollow () {                // 输出 follow 集合
    for (auto x : Vn) {
        cout << "follow[" << x << "] = {";
        for (auto y : follow[x]) {
            cout << y << " ";
        }
        cout << "}" << endl;
    }
}

void getFirst () {                             // 求 first 集合
    for (auto x : Vt) {
        first[x].insert (x);
    }
    int siz = Vn.size ();
    for (int i = 1; i <= siz; i++) {
        for (int j = 0; j < law.st.size (); j++) {
            string tempVn = law.st[j].Vn;
            vector <string> tempVt;
            for (auto x : law.st[j].Vt) {
                tempVt.push_back (x);
            }
            bool flag = true;
            for (auto x : tempVt) {
                auto it = first[x].lower_bound ("empty");
                if (it != first[x].end () && (*it == "first")) {
                    for (auto y : first[x]) {
                        if (y == "empty") continue;
                        first[tempVn].insert (y);
                    }
                } else {
                    for (auto y : first[x]) {
                        first[tempVn].insert (y);
                    }
                    flag = false;
                    break;
                }
            }
            if (flag) {
                first[tempVn].insert ("empty");
            }
        }
    }
}

void getFollow () {                     // 求 follow 集合
    follow["_s"].insert ("$");
    for (auto st : law.st) {        // 用终结符初始化部分 follow 集合
        for (int i = 0; i < st.Vt.size () - 1; i++) {
            string thisVt = st.Vt[i];
            string nextVt = st.Vt[i + 1];
            if (nature[thisVt] == 1) {
                for (auto x : first[nextVt]) {
                    if (x == "empty") continue;
                    follow[thisVt].insert (x);
                }
            }
        }
    }
    for (auto st : law.st) {            // 建立文法拓扑图
        string thisVn = st.Vn;
        for (int i = 0; i < st.Vt.size (); i++) {
            string thisVt = st.Vt[i];
            if (thisVn == thisVt) continue;
            if (nature[thisVt] == 1) {
                bool flag = true;
                for (int j = i + 1; j < st.Vt.size (); j++) {
                    string tempVt = st.Vt[j];
                    if (nature[tempVt] == 2) {
                        flag = false;
                        break;
                    } else {
                        auto it = first[tempVt].lower_bound ("empty");
                        if (it == first[tempVt].end () || (*it != "empty")) {
                            flag = false;
                            break;
                        }
                    }
                }
                if (flag) {
                    //cout << "(" << thisVn << ", " << thisVt << ")" << endl;
                    lawG[thisVn].push_back (thisVt);
                    lawindeg[thisVt]++;
                    lawoutdeg[thisVn]++;
                }
            }
        }
    }
    queue <string> q;                       // 文法拓扑排序求 follow 集合
    for (auto x : Vn) {
        if (lawindeg[x] == 0) {
            q.push (x);
        }
    }
    while (!q.empty ()) {
        string u = q.front ();
        q.pop ();
        for (auto v : lawG[u]) {
            for (auto x : follow[u]) {
                follow[v].insert (x);
            }
            lawindeg[v]--;
            if (!lawindeg[v]) {
                q.push (v);
            }
        }
    }
}

inline void getPrograme () {                    // 读入程序
    freopen (pathPrograme, "r", stdin);
    string temps;
    while (getline (cin, temps)) {
        programe += temps;
        programe += '\n';
    }
    fclose (stdin);
    cin.clear ();
}

#endif


