#ifndef TEAM_COMPLIER_PREWORK_H
#define TEAM_COMPLIER_PREWORK_H

inline void programPreWork () {     // 替换 += 等运算符
    string copyProgram;
    int len = programe.size () - 1;
    map <char, int> isSpcOpt, isStop;
    string tempOpt = "+-*/&|^";
    string stopChar = ";, \n(){}[]<>.?:";
    for (auto x : tempOpt) {
        isSpcOpt[x] = true;
    }
    for (auto x : stopChar) {
        isStop[x] = true;
    }
    for (int i = 0; i + 1 <= len; i++) {
        if (isSpcOpt[programe[i]] && programe[i + 1] == '=') {
            copyProgram += "= ";
            int j = i;
            for (j = i - 1; j >= 0; j--) {
                if (programe[j] == ' ') continue;
                break;
            }
            int rightPos = j;
            for (; j >= 0; j--) {
                if (isStop[programe[j]]) {
                    j++; break;
                }
            }
            int leftPos = j;
            for (j = leftPos; j <= rightPos; j++) {
                copyProgram += programe[j];
            }
            copyProgram += " ";
            copyProgram += programe[i];
            i++;
        } else {
            copyProgram += programe[i];
        }
    }
    programe = copyProgram;
}

#endif //TEAM_COMPLIER_PREWORK_H


