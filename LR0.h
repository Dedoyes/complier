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
        set <string> edge;
        for (; it != fullClo.close.end (); it++) {
            auto itm = *it;
            if (itm.pos == itm.st.Vt.size ())
                continue;
            if (itm.st.Vt[itm.pos] == "empty") continue;
            string thsVt = itm.st.Vt[itm.pos];
            edge.insert (thsVt);
        }
        for (auto x : edge) {
            v.close.clear ();
            for (it = fullClo.close.begin (); it != fullClo.close.end (); it++) {
                auto itm = *it;
                if (itm.pos == itm.st.Vt.size ())
                    continue;
                if (itm.st.Vt[itm.pos] == "empty") continue;
                string thsVt = itm.st.Vt[itm.pos];
                if (thsVt == x) {
                    item newItm = item (it->pos, it->st);
                    newItm.pos++;
                    v.close.insert (newItm);
                }
            }
            if (v.close.size ()) {
                lawI.insert (v);
                if (!closureF.count (v)) {
                    closureF[v] = ++closureCnt;
                    vecClo[closureCnt] = v;
                }
                if (!cloVis[v]) {
                    q.push (v);
                    cloVis[v] = true;
                }
                cout << "f[" << closureF[u] << "][" << x << "] = " << closureF[v] << endl;
                closureG[closureF[u]][x] = closureF[v];
            }
        }
        //if (v.close.size ()) {
        //    cout << "case2" << endl;
        //    lawI.insert (v);
        //    if (!closureF.count (v)) {
        //        closureF[v] = ++closureCnt;
        //        vecClo[closureCnt] = v;
        //    }
        //    if (!cloVis[v]) {
        //        q.push (v);
        //        cloVis[v] = true;
        //    }
        //    cout << "f[" << closureF[u] << "][" << last << "] = " << closureF[v] << endl;
        //    closureG[closureF[u]][last] = closureF[v];
        //}
    }
}

struct Action {
    string type;
    int pos;
    Action () {}
    Action (string _type, int _pos) {
        type = _type; pos = _pos;
    }
    inline void print () {
        cout << "(" << type << ", " << pos << ")" << endl;
    }
};

map <string, Action> act[CLOSURE_MAX_CNT];

void buildTable () {
    for (int i = 1; i <= closureCnt; i++) {
        cout << "i = " << i << endl;
        for (auto itm : borderClousre(vecClo[i]).close) {
            int pos = itm.pos;
            if (pos == itm.st.Vt.size ()) {
                string tempVn = itm.st.Vn;
                if (tempVn == "__s") continue;
                for (auto x : follow[tempVn]) {
                    act[i][x].pos = law.idx[itm.st];
                    act[i][x].type = "protocal";
                    cout << "act[" << i << "][" << x << "] = ";
                    act[i][x].print ();
                }
                continue;
            }
            for (int j = pos; j < itm.st.Vt.size (); j++) {
                string tempVt = itm.st.Vt[pos];
                if (nature[tempVt] == 2 || nature[tempVt] == 0) {
                    act[i][tempVt].pos = closureG[i][tempVt];
                    act[i][tempVt].type = "move";
                    cout << "act[" << i << "][" << tempVt << "] = ";
                    act[i][tempVt].print ();
                    break;
                } else {
                    bool flag = false;
                    auto it = first[tempVt].lower_bound ("empty");
                    if (it == first[tempVt].end () || (*it) != "empty") {
                        flag = true;
                    }
                    for (auto x : first[tempVt]) {
                        if (x == "empty") continue;
                        if (nature[x] == 1) continue;
                        act[i][x].pos = closureG[i][x];
                        act[i][x].type = "move";
                        cout << "act[" << i << "][" << x << "] = ";
                        act[i][x].print ();
                        break;
                    }
                    if (!flag) break;
                }
            }
        }
    }
    act[2]["$"].type = "accept";
    //act[5]["main"] = Action ("move", 45);
    //act[2]["$"].print ();
}

bool syntaxCacu () {
    for (auto x : act[1]) {
        cout << "(" << x.first << ", "; x.second.print();
        cout << ")" << endl;
    }
    cout << "syntaxCacu" << endl;
    stack <int> stk;
    stack <string> charStk;
    stk.push (1);
    charStk.push ("$");
    int state = 1;
    buildTable ();
    int tokenCnt = 0;
    token x = WordFinal.Token[tokenCnt];
    while (true) {
        cout << endl;
        x = WordFinal.Token[tokenCnt];
        string type = x.TokenA;
        int pos = x.TokenB;
        string inStack;
        if (type == "K") {
            inStack = WordFinal.revFind (x);
            if (inStack == "main") {
                inStack = "var";
            }
        } else if (type == "P") {
            inStack = WordFinal.revFind (x);
        } else if (type == "I") {
            inStack = "var";
        } else if (type == "C1") {
            inStack = "integrate";
        } else if (type == "C2") {
            inStack = "floatnum";
        } else if (type == "CT") {
            inStack = "chartype";
        } else {
            inStack = "stringtype";
        }
        cout << "inStack = " << inStack << endl;
        int nowS = stk.top ();
        cout << "nowS = " << nowS << endl;
        if (act[nowS][inStack].type == "move") {
            cout << "case move" << endl;
            stk.push (act[nowS][inStack].pos);
            cout << act[nowS][inStack].pos << "was pushed" << endl;
            charStk.push (inStack);
            tokenCnt++;
        } else if (act[nowS][inStack].type == "protocal") {
            cout << "case protocal" << endl;
            int lawPos = act[nowS][inStack].pos;
            stmt st = law.st[lawPos];
            string tempVn = st.Vn;
            int num = st.Vt.size ();
            for (int i = 1; i <= num; i++) {
                charStk.pop();
                stk.pop ();
                cout << "pop" << endl;
            }
            int stktop = stk.top ();
            cout << "stktop = " << stktop << endl;
            cout << "Vn = " << tempVn << endl;
            int nextState = closureG[stktop][tempVn];
            cout << "next State = " << nextState << endl;
            charStk.push (tempVn);
            stk.push (nextState);
            cout << nextState << "was pushed" << endl;
            st.print ();
        } else if (act[nowS][inStack].type == "accept") {
            return true;
        } else {
            return false;
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
        borderClousre(vecClo[i]).print ();
    }
    closureG[2]["$"] = ++closureCnt;
    buildTable ();
    return syntaxCacu ();
}

#endif //TEAM_COMPLIER_LR0_H
