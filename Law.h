#ifndef TEAM_COMPLIER_LAW_H
#define TEAM_COMPLIER_LAW_H

#include <bits/stdc++.h>

using namespace std;

struct stmt {                                     // 派生语句
    string Vn;                                    // 终结符
    vector <string> Vt;                           // 非终结符
    stmt () {}
    stmt (string _Vn, vector <string> &_Vt) {
        Vn = _Vn;
        for (auto x : _Vt) {
            Vt.push_back (x);
        }
    }
    bool operator < (const stmt &st) const {
        if (Vn != st.Vn)
            return Vn < st.Vn;
        string thisStr, stStr;
        for (auto s : Vt) {
            thisStr += s;
        }
        for (auto s : st.Vt) {
            stStr += s;
        }
        return thisStr < stStr;
    }
    bool operator == (const stmt &st) const {
        if (Vn != st.Vn)
            return false;
        string thisStr, stStr;
        for (auto s : Vt) {
            thisStr += s;
        }
        for (auto s : st.Vt) {
            stStr += s;
        }
        return thisStr == stStr;
    }
    bool operator != (const stmt &st) const {
        return !(*this == st);
    }
    inline void print () {
        cout << Vn << " -> ";
        for (auto x : Vt) {
            cout << x << " ";
        }
        cout << endl;
    }
};

struct Law {                                    // 文法
    vector <stmt> st;                           // 文法语句表
    Law () {}
    Law (vector <stmt> _st) {
        for (auto x : _st) {
            st.push_back (x);
        }
    }
    inline void print () {
        for (auto x : st) {
            x.print ();
        }
    }
};

void Border_Law (Law &law) {            // 增广文法
    vector <string> tempSt;
    tempSt.push_back ("_s");
    law.st.push_back (stmt ("__s", tempSt));
}

#endif //TEAM_COMPLIER_LAW_H
