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
    vector <stmt> queryVn (string &Vn) {     // 文法有序时 O(logn + |ans|) 得到 Vn->...的派生语句集合
        vector <stmt> ret;
        int leftPos = -1;
        int L = 0, R = st.size () - 1;
        while (L <= R) {
            int mid = (L + R) >> 1;
            stmt tempSt = st[mid];
            if (tempSt.Vn < Vn) {
                L = mid + 1;
            } else if (tempSt.Vn > Vn) {
                R = mid - 1;
            } else {
                if (leftPos == -1) {
                    leftPos = mid;
                } else {
                    leftPos = min (leftPos, mid);
                }
                R = mid - 1;
            }
        }
        if (leftPos == -1) {
            return ret;
        }
        int rightPos = -1;
        L = 0, R = st.size () - 1;
        while (L <= R) {
            int mid = (L + R) >> 1;
            stmt tempSt = st[mid];
            if (tempSt.Vn < Vn) {
                L = mid + 1;
            } else if (tempSt.Vn > Vn) {
                R = mid - 1;
            } else {
                rightPos = max (rightPos, mid);
                L = mid + 1;
            }
        }
        for (int i = leftPos; i <= rightPos; i++) {
            ret.push_back (st[i]);
        }
        return ret;
    }
};

void Border_Law (Law &law) {            // 增广文法
    vector <string> tempSt;
    tempSt.push_back ("_s");
    law.st.push_back (stmt ("__s", tempSt));
    sort (law.st.begin (), law.st.end ());
}

#endif //TEAM_COMPLIER_LAW_H
