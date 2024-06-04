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

map <string, set <string> > first, follow;    // first, follow 集合
set <char> word;                              // 字集合， U - {$}
Law law;                                      // 文法表
string programe, lawstr;                      // 源程序, 文法字符串
vector <string> Vt, Vn;                       // 终结符集合Vt, 非终结符集合 Vn
const char* pathLaw =                         // 本地目录名称
    "C:\\Users\\lumia\\CLionProjects\\cf1969\\Team_complier\\Law.in";
const char* pathPrograme =
    "C:\\Users\\lumia\\CLionProjects\\cf1969\\Team_complier\\Program.in";

inline void initLaw () {
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
}

#endif
