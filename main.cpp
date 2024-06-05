#include <bits/stdc++.h>
using namespace std;
const int MAXN_PROGRAM_LEN = 1e6 + 5;
#define LL long long
#define mod %
const LL hashBase = 256;
LL baseExp[MAXN_PROGRAM_LEN][2];                                // 双模数哈希 base 幂
const LL programP[] = {(LL)1e9 + 7, (LL)1e9 + 9} ;       // 双模数哈希的俩个质数
#include "Scanner.h"
#include "programHash.h"
#include "Token.h"
doubleHash OriginProgamF;                                       // 程序的双哈希表
#include "completeLaw.h"

int main () {
    cout << "ok" << endl;
    initBaseExp ();
    initLaw ();
    initword ();
    initVnVt ();
    getPrograme ();
    law.print ();
    OriginProgamF.initHash (programe);
    int pos = 0;
    while (pos < programe.size ()) {
        string temps = "struct";
        pos = OriginProgamF.findPos (temps, pos) + 1;
        if (pos == 0) break;
        cout << "pos = " << pos << endl;
    }
    cout << "ok" << endl;
    return 0;
}
