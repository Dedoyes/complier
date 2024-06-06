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
        if (st != it.st) {
            return st < it.st;
        }
        return pos < it.pos;
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
        if (close.size () != clo.close.size ()) {
            return close.size () < clo.close.size ();
        }
        auto it1 = close.begin ();
        auto it2 = clo.close.begin ();
        for (; it1 != close.end () && it2 != clo.close.end (); it1++, it2++) {
            if (*it1 == *it2) {
                continue;
            }
            return *it1 < *it2;
        }
        return true;
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

int closureCnt;
set <closure> lawI;
map <closure, int> closureF;

closure borderClousre (closure &clo) {          // 求出内核项的全闭包
    map <string, bool> isBorder;
    closure ret = clo;
    auto it = ret.close.begin ();
    while (true) {
        bool flag = false;
        for (; it != ret.close.end (); it++) {
            auto itm = *it;
            int pos = itm.pos;
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
    queue <closure> q;
    q.push (clo);
    while (!q.empty ()) {
        auto u = q.front ();
        q.pop ();

    }
}

bool LR0 () {
    Border_Law (law);
    closure start;
    item It;
    It.pos = 0;
    vector <string> vec;
    vec.push_back ("_s");
    It.st = stmt ("__s", vec);
    start.close.insert (It);
    lawI.insert (start);
}

#endif //TEAM_COMPLIER_LR0_H
