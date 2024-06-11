#ifndef TEAM_COMPLIER_PROGRAMHASH_H
#define TEAM_COMPLIER_PROGRAMHASH_H

#include <bits/stdc++.h>
#include "Scanner.h"

inline void initBaseExp () {                         // 初始化 base 数组
    baseExp[0][0] = baseExp[0][1] = 1;
    for (int i = 1; i < MAXN_PROGRAM_LEN; i++) {
        for (int j = 0; j <= 1; j++) {
            baseExp[i][j] = baseExp[i - 1][j] * hashBase mod programP[j];
        }
    }
}

struct doubleHash {                                  // 双哈希结构体, 支持快速查找字符串位置
    int programLen;
    LL hs[MAXN_PROGRAM_LEN][2];
    inline void initHash (string &s) {               // 初始化特定双哈希
        programLen = s.size ();
        int len = s.size () - 1;
        for (int i = 0; i <= len; i++) {
            for (int j = 0; j <= 1; j++) {
                if (i == 0) {
                    hs[i][j] = s[i] mod programP[j];
                } else {
                    hs[i][j] = (hs[i - 1][j] * hashBase + s[i]) mod programP[j];
                }
            }
        }
    }
    inline LL getHashVal (int L, int R, int type) {  // 得到 [L,R]的哈希区间值
        if (L == 0)
            return hs[R][type];
        LL rightVal = hs[R][type];
        LL leftVal = hs[L - 1][type] * baseExp[R - L + 1][type] mod programP[type];
        LL ret = (rightVal - leftVal + programP[type]) mod programP[type];
        return ret;
    }
    inline int findPos (string &s, int pos) { // 得到 s 的匹配串的起点位置, 没找到返回 -1
        LL findVal[2] = {0, 0};
        for (int i = 0; i < s.size (); i++) {
            for (int j = 0; j <= 1; j++) {
                findVal[j] = (findVal[j] * hashBase + s[i]) mod programP[j];
            }
        }
        int len = s.size ();
        for (int i = pos; i + len - 1 < programLen; i++) {
            LL val[2] = {0, 0};
            val[0] = this->getHashVal (i, i + len - 1, 0);
            val[1] = this->getHashVal (i, i + len - 1, 1);
            if (val[0] == findVal[0] && val[1] == findVal[1]) {
                return i;
            }
        }
        return -1;
    }
};

#endif //TEAM_COMPLIER_PROGRAMHASH_H


