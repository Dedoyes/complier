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
        };
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

int closureCnt;                                 // 内核项闭包时间戳
set <closure> lawI;                              // 内核项闭包集合
map <closure, int> closureF;                    // 内核项闭包离散化函数
map <string, int> closureG[CLOSURE_MAX_CNT];    // 内核项闭包图

closure borderClousre (closure &clo) {          // 求出内核项的全闭包
    closure ret = clo;
    auto it = ret.close.begin ();
    while (true) {
        bool flag = false;
        for (; it != ret.close.end (); it++) {
            auto itm = *it;
            int pos = itm.pos;
            if (pos == itm.st.Vt.size ())
                continue;
            string nextVn = itm.st.Vt[pos];
            if (nature[nextVn] == 1) {
                for (auto x : law.queryVn (nextVn)) {
                    item newitm = item (0, x);
                    ret.close.insert (newitm);
                    flag = true;
                }
            }
        }
        if (!flag) {
            break;
        }
    }
    return ret;
}

void bfs (closure &clo) {
    map <closure, bool> cloVis;
    cout << "bfs ()" << endl;
    queue <closure> q;
    q.push (clo);
    closureF[clo] = ++closureCnt;
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
                    if (!closureF.count (v))
                        closureF[v] = ++closureCnt;
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
            if (!closureF.count (v))
                closureF[v] = ++closureCnt;
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

bool LR0 () {
    cout << "no problem" << endl;
    closure start;
    item It;
    It.pos = 0;
    vector <string> vec;
    vec.push_back ("_s");
    It.st = stmt ("__s", vec);
    start.close.insert (It);
    bfs (start);
    return true;
}

#endif //TEAM_COMPLIER_LR0_H
