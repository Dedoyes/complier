#include <bits/stdc++.h>
#include "quaternion.h"
using namespace std;
const int MAXN_PROGRAM_LEN = 1e6 + 5;
#define LL long long
#define mod %
const LL hashBase = 256;
LL baseExp[MAXN_PROGRAM_LEN][2];                                // 双模数哈希 base 幂
const LL programP[] = {(LL)1e9 + 7, (LL)1e9 + 9} ;       // 双模数哈希的俩个质数
#include "Scanner.h"
#include "programHash.h"
doubleHash OriginProgamF;               // 程序的双哈希表
#include "preWork.h"
#include "Token.h"
#include "completeLaw.h"
#include "Lexical_Analysis.h"
#include "getSize.h"
#include "quaternion.h"
#include "LR0.h"
#include "cacuAdress.h"
#include "assamble_generate.h"


int main () {
    cout << "ok" << endl;
    initBaseExp ();
    initLaw ();
    initword ();
    initVnVt ();
    getPrograme ();
    programPreWork ();
    //cout << programe << endl;
    //law.print ();
    OriginProgamF.initHash (programe);
    //int pos = 0;  这段注释指出了双哈希的用法
    //while (pos < programe.size ()) {
    //    string temps = "struct";
    //    pos = OriginProgamF.findPos (temps, pos) + 1;
    //    if (pos == 0) break;
    //    cout << "pos = " << pos << endl;
    //}
    makeUpLaw ();
    getFirst ();
    getFollow ();
    //Lexical_Analysis ();
    Border_Law (law);
    LR0 ();
    //for (auto x : lawI) {
    //    x.print ();
    //}
    cout << "ok" << endl;
    return 0;
}


