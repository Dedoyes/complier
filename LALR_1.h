#ifndef _LALR_1_H
#define _LALR_1_H
#include<iostream>
#include"Scanner.h"
using namespace std;

/*
项目
*/
struct LALR_1_Item {
    int pos;    //记录项目中点的位置
    stmt st;    //项目语句
    LALR_1_Item() {}
    LALR_1_Item(int _pos, stmt _st) {
        pos = _pos;
        st = _st;
    }
    //重载部分
    bool operator<(const LALR_1_Item &it) const {
        if (st != it.st)
        {
            return st < it.st;
        }
        return pos < it.pos;
    }
    bool operator==(const LALR_1_Item &it) const {
        if (st == it.st && pos == it.pos)
            return true;
        return false;
    }
    bool operator!=(const LALR_1_Item &it) const {
        return !(*this == it);
    }
    inline void print() {
        cout << st.Vn << "->";
        for (int i = 0; i < pos; i++)
            cout << st.Vt[i] << " ";
        cout << "[*]";
        for (int i = pos; i <= st.Vt.size() - 1; i++)
        {
            cout << st.Vt[i] << " ";
        }
        cout << endl;
    }
};

/*
项目集闭包
*/
struct closure {
    set<LALR_1_Item> close;     //建立close(set容器) 用以存放 LALR_1_Item 类型数据
    closure() {}
    closure(vector<LALR_1_Item> &clo) {
        for (auto x : clo)
        {
            close.insert(x);    //向close插入 LALR_1_Item 类型数据
        }
    }
    //重载部分
    bool operator<(const closure &clo) const {
        if (close.size() != clo.close.size()) {
            return close.size() < clo.close.size();
        }
        auto it1 = close.begin();
        auto it2 = clo.close.begin();
        for (; it1 != close.end() && it2 != clo.close.end(); it1++, it2++) {
            if (*it1 == *it2) {
                continue;
            }
            return *it1 < *it2;
        }
        return true;
    }
    bool operator==(const closure &clo) const {
        if (close.size() != clo.close.size())
            return false;
        auto it1 = close.begin();
        auto it2 = clo.close.begin();
        for (; it1 != close.end() && it2 != clo.close.end(); it1++, it2++) {
            if (*it1 == *it2) {
                continue;
            }
            return false;
        }
        return true;
    }
    bool operator!=(const closure &clo) const {
        return !(*this == clo);
    }
    inline void print() {
        cout << "{" << endl;
        for (auto x : close) {
            x.print();
        }
        cout << "}" << endl;
    }
};

const int CLOSURE_MAX_CNT = 1e5 + 5;

map<closure, bool> dfsVis;
map <closure, int> closureF;                    // 将项目集与其对应标号对应： I0 -> 0
map <string, int> closureG[CLOSURE_MAX_CNT];    // 项目集闭包转换

//dfs创建项目集闭包
void dfs_closure (closure &u, closure &ret) {
    dfsVis[u] = true;
    for (auto v : u.close) {
        int pos = v.pos;
        if (pos == v.st.Vt.size ()) continue;
        if (v.st.Vt[pos] == "empty") continue;
        string nextS = v.st.Vt[pos];
        auto vecStmt = law.queryVn (nextS);
        closure temp;
        for (auto x : vecStmt) {
            LALR_1_Item newitm = LALR_1_Item (0, x);
            ret.close.insert (newitm);
            temp.close.insert (newitm);
        }
        if (!dfsVis[temp])
            dfs_closure(temp, ret);
    }
}
void dfs(closure &u) {
    dfsVis[u] = true;
    for(auto v : u.close) {
        int pos = v.pos;
        if (pos == v.st.Vt.size ()) continue;
        if (v.st.Vt[pos] == "empty") continue;
        string nextVn = v.st.Vt[pos];
        //cout << "下一个Vn为：" << nextVn<<endl;
        // for (vector<stmt>::iterator it = law.st.begin(); it != law.st.end(); it++) {
        //     if(it->Vn == nextVn) {
        //         LALR_1_Item additem;
        //         additem.st = stmt(it->Vn, it->Vt);
        //         additem.pos = 0;
        //         u.close.insert(additem);
        //     }
        // }
        vector<string> nextVns;
        nextVns.push_back(nextVn);
        for(auto x : nextVns) {
            cout << "当前容器大小为：" << nextVns.size() << endl;
            for (vector<stmt>::iterator it = law.st.begin(); it != law.st.end(); it++) {
                if(x == it->Vn) {
                    cout << "即将加入的元素为:" << it->Vt[0]<<endl;
                    nextVns.push_back(it->Vt[0]);
                    cout << "当前nextVns大小为:" << nextVns.size() << endl;
                }
            }
        }
        cout << "nextVns中元素为:" << endl;
        for(auto x : nextVns) {
            cout << x << " ";
        }
    }
}

void bfs() {

}


//获取规约项目展望符
void get_lookahead () {
    
}

//消除同心项目集
void cut_core_item() {

}

void LALR_1() {
    closure I0;     //建立I0项目集闭包
    LALR_1_Item I0_core_item;     //I0中核心项目 __s -> _s
    vector<string> vec;     //stmt结构体中： 终结符 用 vector<string>存储
    vec.push_back("_s");
    I0_core_item.st = stmt("__s", vec);   //将起始项目初始化为： __s -> _s
    I0_core_item.pos = 0;
    I0.close.insert(I0_core_item);    //在I0中插入起始项目
    dfs(I0);
}




#endif
