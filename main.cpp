#include <bits/stdc++.h>
const int MAXN_PROGRAM_LEN = 1e6 + 5;
#define LL long long
#define mod %
const LL hashBase = 256;
LL baseExp[MAXN_PROGRAM_LEN][2];                                // 双模数哈希 base 幂
const LL programP[] = {(LL)1e9 + 7, (LL)1e9 + 9} ;       // 双模数哈希的俩个质数
#include "Scanner.h"
#include "programHash.h"
#include "Token.h"

using namespace std;

int main () {
    cout << "ok" << endl;
    initBaseExp ();
    getPrograme ();
    initLaw ();
    initword ();
    initVnVt ();
    //printVn ();
    //printVt ();
    //law.print ();
    getFirst ();
    //printFirst ();
    getFollow ();
    printFollow ();
    cout << "ok" << endl;
    return 0;
}
