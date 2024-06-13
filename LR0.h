#ifndef TEAM_COMPLIER_LR0_H
#define TEAM_COMPLIER_LR0_H

struct item {                               // 项目集
    int pos;
    stmt st;
    item () {}
    item (int _pos, stmt _st) {
        pos = _pos; st = _st;
    }
    bool operator < (const item &it) const {
        if (this->st != it.st) {
            return this->st < it.st;
        }
        return this->pos < it.pos;
    }
    bool operator == (const item &it) const {
        if (st == it.st && pos == it.pos)
            return true;
        return false;
    }
    bool operator != (const item &it) const {
        return !(*this == it);
    }
    inline void print () {
        cout << st.Vn << "->";
        for (int i = 0; i <= pos - 1; i++)
            cout << st.Vt[i] << " ";
        cout << "[*]";
        for (int i = pos; i <= st.Vt.size() - 1; i++) {
            cout << st.Vt[i] << " ";
        }
        cout << endl;
    }
};

struct closure {                        // 项目集闭包
    set <item> close;
    closure () {}
    closure (vector <item> &clo) {
        for (auto x : clo) {
            close.insert (x);
        }
    }
    bool operator < (const closure &clo) const {
        if (this->close.size () != clo.close.size ()) {
            return this->close.size () < clo.close.size ();
        }
        auto it1 = close.begin ();
        auto it2 = clo.close.begin ();
        for (; it1 != close.end () && it2 != clo.close.end (); it1++, it2++) {
            if (*it1 == *it2) {
                continue;
            }
            return *it1 < *it2;
        }
        return false;
    }
    bool operator == (const closure &clo) const {
        if (close.size () != clo.close.size ())
            return false;
        auto it1 = close.begin ();
        auto it2 = clo.close.begin ();
        for (; it1 != close.end () && it2 != clo.close.end (); it1++, it2++) {
            if (*it1 == *it2) {
                continue;
            }
            return false;
        }
        return true;
    }
    bool operator != (const closure &clo) const {
        return !(*this == clo);
    }
    inline void print () {
        cout << "{";
        for (auto x : close) {
            x.print ();
        }
        cout << "}" << endl;
    }
};

const int CLOSURE_MAX_CNT = 1e5 + 5;

int closureCnt;                                  // 内核项闭包时间戳
set <closure> lawI;                              // 内核项闭包集合
map <closure, int> closureF;                     // 内核项闭包离散化函数
map <string, int> closureG[CLOSURE_MAX_CNT];     // 内核项闭包图
closure vecClo[CLOSURE_MAX_CNT];
map <closure, bool> dfsVis;

void closureDfs (closure &u, closure &ret) {
    dfsVis[u] = true;
    for (auto v : u.close) {
        int pos = v.pos;
        if (pos == v.st.Vt.size ()) continue;
        if (v.st.Vt[pos] == "empty") continue;
        string nextS = v.st.Vt[pos];
        auto vecStmt = law.queryVn (nextS);
        closure temp;
        for (auto x : vecStmt) {
            item newitm = item (0, x);
            ret.close.insert (newitm);
            temp.close.insert (newitm);
        }
        if (!dfsVis[temp])
            closureDfs(temp, ret);
    }
    //dfsVis[u] = false;
}

closure borderClousre (closure &clo) {          // 求出内核项的全闭包
    closure ret = clo;
    auto it = ret.close.begin ();
    closureDfs (clo, ret);
    dfsVis.clear ();
    return ret;
}

void bfs (closure &clo) {
    law.print ();
    map <closure, bool> cloVis;
    cout << "bfs ()" << endl;
    queue <closure> q;
    q.push (clo);
    closureF[clo] = ++closureCnt;
    vecClo[closureCnt] = clo;
    cloVis[clo] = true;
    lawI.insert (clo);
    while (!q.empty ()) {
        closure u = q.front ();
        q.pop ();
        closure fullClo = borderClousre (u);
        auto it = fullClo.close.begin ();
        cout << "fullClo = "; fullClo.print ();
        fullClo.print ();
        string last = "";
        closure v;
        for (; it != fullClo.close.end (); it++) {
            auto itm = *it;
            if (itm.pos == itm.st.Vt.size ())
                continue;
            if (itm.st.Vt[itm.pos] == "empty") continue;
            string thsVt = itm.st.Vt[itm.pos];
            if (last == "" || last == thsVt) {
                item vItm = itm;
                vItm.pos++;
                v.close.insert (vItm);
                last = thsVt;
            } else {
                if (v.close.size ()) {
                    lawI.insert (v);
                    if (!closureF.count (v)) {
                        closureF[v] = ++closureCnt;
                        vecClo[closureCnt] = v;
                    }
                    cout << "case1" << endl;
                    cout << "closureCnt = " << closureCnt << endl;
                    if (!cloVis[v]) {
                        q.push(v);
                        cloVis[v] = true;
                    }
                    //cout << "u = ";
                    //fullClo.print ();
                    //cout << "v = ";
                    //v.print ();
                    cout << "f[" << closureF[u] << "][" << last << "] = " << closureF[v] << endl;
                    closureG[closureF[u]][last] = closureF[v];
                    v.close.clear ();
                }
                if (itm.pos == itm.st.Vt.size ())
                    continue;
                if (itm.st.Vt[itm.pos] == "empty") continue;
                item vItm = itm;
                vItm.pos++;
                v.close.insert (vItm);
                last = thsVt;
            }
        }
        if (v.close.size ()) {
            cout << "case2" << endl;
            lawI.insert (v);
            if (!closureF.count (v)) {
                closureF[v] = ++closureCnt;
                vecClo[closureCnt] = v;
            }
            if (!cloVis[v]) {
                q.push (v);
                cloVis[v] = true;
            }
            //cout << "u = ";
            //fullClo.print ();
            //cout << "v = ";
            //v.print ();
            cout << "f[" << closureF[u] << "][" << last << "] = " << closureF[v] << endl;
            closureG[closureF[u]][last] = closureF[v];
        }
    }
}

struct Action {
    string type;
    int pos;
    Action () {}
    Action (string _type, int _pos) {
        type = _type; pos = _pos;
    }
};

map <string, Action> act[CLOSURE_MAX_CNT];

void buildTable () {
    for (int i = 1; i <= closureCnt; i++) {
        for (auto itm : vecClo[i].close) {
            int pos = itm.pos;
            if (pos == itm.st.Vt.size ()) {
                string tempVn = itm.st.Vn;
                for (auto x : follow[tempVn]) {
                    act[i][x].pos = law.idx[itm.st];
                    act[i][x].type = "protocal";
                }
                continue;
            }
            string tempVt = itm.st.Vt[pos];
            if (nature[tempVt] == 2) {
                act[i][tempVt].pos = closureG[i][tempVt];
                act[i][tempVt].type = "move";
            }
        }
    }
    act[2]["$"].type = "accept";
}

bool syntaxCacu () {
    stack <int> stk;
    stack <string> charStk;
    stk.push (1);
    charStk.push ("$");
    int state = 1;
    buildTable ();
    for (auto x : WordFinal.Token) {
        string type = x.TokenA;
        int pos = x.TokenB;
        string inStack;
        if (type == "K") {
            inStack = WordFinal.revFind (x);
            state = closureG[state][inStack];
        } else if (type == "P") {
            inStack = WordFinal.revFind (x);
            state = closureG[state][inStack];
        } else if (type == "I") {
            inStack = "var";
            state = closureG[state][inStack];
        } else if (type == "C1") {
            inStack = "integrate";
            state = closureG[state][inStack];
        } else if (type == "C2") {
            inStack = "floatnum";
            state = closureG[state][inStack];
        } else if (type == "CT") {
            inStack = "chartype";
            state = closureG[state][inStack];
        } else {
            inStack = "stringtype";
            state = closureG[state][inStack];
        }
        while (true) {

        }
    }
}

bool LR0 () {
    cout << "LR0 () : " << endl;
    closure start;
    item It;
    It.pos = 0;
    vector <string> vec;
    vec.push_back ("_s");
    It.st = stmt ("__s", vec);
    start.close.insert (It);
    bfs (start);
    cout << "cnt = " << closureCnt << endl;
    for (int i = 1; i <= closureCnt; i++) {
        cout << "clo[" << i << "] = ";
        vecClo[i].print ();
    }
    closureG[2]["$"] = ++closureCnt;
    return syntaxCacu ();
}

#endif //TEAM_COMPLIER_LR0_H
