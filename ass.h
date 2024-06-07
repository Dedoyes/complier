#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

// 四元式结构体
struct Quaternion {
    string op;   // 操作符
    string opt1; // 第一个操作数
    string opt2; // 第二个操作数
    string result; // 结果
};

// 寄存器分配类
class RegisterAllocator {
public:
    RegisterAllocator() : registers({ "AX", "BX", "CX", "DX" }) {}

    string allocate(string var, unordered_map<string, int>& varLiveness, int currentIndex) {
        for (string reg : registers) {
            if (regMap[reg] == "" || varLiveness[regMap[reg]] < currentIndex) {
                if (regMap[reg] != "") {
                    freeRegister(reg);
                }
                regMap[reg] = var;
                return reg;
            }
        }
        // 没有空闲寄存器，选择一个最不活跃的变量存入内存
        string toFree = registers[0];
        for (string reg : registers) {
            if (varLiveness[regMap[reg]] < varLiveness[regMap[toFree]]) {
                toFree = reg;
            }
        }
        freeRegister(toFree);
        regMap[toFree] = var;
        return toFree;
    }

    void freeRegister(string reg) {
        if (regMap[reg] != "") {
            cout << "MOV " << regMap[reg] << ", " << reg << endl;
            regMap[reg] = "";
        }
    }

private:
    vector<string> registers;
    unordered_map<string, string> regMap;
};

// 获取变量活跃信息
unordered_map<string, int> getLivenessInfo(const vector<Quaternion>& quaternions) {
    unordered_map<string, int> varLiveness;
    for (int i = 0; i < quaternions.size(); ++i) {
        if (quaternions[i].opt1 != "" && !isdigit(quaternions[i].opt1[0])) {
            varLiveness[quaternions[i].opt1] = i;
        }
        if (quaternions[i].opt2 != "" && !isdigit(quaternions[i].opt2[0])) {
            varLiveness[quaternions[i].opt2] = i;
        }
        if (quaternions[i].result != "" && !isdigit(quaternions[i].result[0])) {
            varLiveness[quaternions[i].result] = i;
        }
    }
    return varLiveness;
}

// 生成8086汇编代码
void generateAssembly(const vector<Quaternion>& quaternions) {
    unordered_map<string, int> varLiveness = getLivenessInfo(quaternions);
    RegisterAllocator regAlloc;

    cout << "DATA SEGMENT" << endl;
    unordered_set<string> declaredVars;
    for (const auto& quaternion : quaternions) {
        if (quaternion.opt1 != "" && !isdigit(quaternion.opt1[0]) && declaredVars.find(quaternion.opt1) == declaredVars.end()) {
            cout << quaternion.opt1 << " DW 0" << endl;
            declaredVars.insert(quaternion.opt1);
        }
        if (quaternion.opt2 != "" && !isdigit(quaternion.opt2[0]) && declaredVars.find(quaternion.opt2) == declaredVars.end()) {
            cout << quaternion.opt2 << " DW 0" << endl;
            declaredVars.insert(quaternion.opt2);
        }
        if (quaternion.result != "" && !isdigit(quaternion.result[0]) && declaredVars.find(quaternion.result) == declaredVars.end()) {
            cout << quaternion.result << " DW 0" << endl;
            declaredVars.insert(quaternion.result);
        }
    }
    cout << "DATA ENDS" << endl;

    cout << "CODE SEGMENT" << endl;
    cout << "ASSUME CS:CODE, DS:DATA" << endl;
    cout << "START:" << endl;
    cout << "MOV AX, DATA" << endl;
    cout << "MOV DS, AX" << endl;

    for (int i = 0; i < quaternions.size(); ++i) {
        const auto& quaternion = quaternions[i];
        if (quaternion.op == "+" || quaternion.op == "-" || quaternion.op == "*" || quaternion.op == "/") {
            string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            string reg2 = regAlloc.allocate(quaternion.opt2, varLiveness, i);
            cout << "MOV " << reg2 << ", " << quaternion.opt2 << endl;
            if (quaternion.op == "+") cout << "ADD " << reg1 << ", " << reg2 << endl;
            if (quaternion.op == "-") cout << "SUB " << reg1 << ", " << reg2 << endl;
            if (quaternion.op == "*") cout << "IMUL " << reg1 << ", " << reg2 << endl;
            if (quaternion.op == "/") {
                cout << "XOR DX, DX" << endl;
                cout << "IDIV " << reg2 << endl;
            }
            cout << "MOV " << quaternion.result << ", " << reg1 << endl;
        }
        else if (quaternion.op == "=") {
            string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            cout << "MOV " << quaternion.result << ", " << reg1 << endl;
        }
        else if (quaternion.op == "if") {
            string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            cout << "CMP " << reg1 << ", " << quaternion.opt2 << endl;
            cout << "JE " << quaternion.result << endl;
        }
        else if (quaternion.op == "while") {
            string label = "L" + to_string(i);
            cout << label << ":" << endl;
            string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            cout << "CMP " << reg1 << ", " << quaternion.opt2 << endl;
            cout << "JNE " << quaternion.result << endl;
        }
        else if (quaternion.op == "for") {
            string label = "L" + to_string(i);
            cout << label << ":" << endl;
            string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            cout << "CMP " << reg1 << ", " << quaternion.opt2 << endl;
            cout << "JE " << quaternion.result << endl;
        }
        else if (quaternion.op == "DECL") {
            cout << quaternion.opt1 << " DW 0" << endl;
        }
        else if (quaternion.op == "FUNC") {
            cout << quaternion.result << " PROC" << endl;
            cout << "MOV AX, DATA" << endl;
            cout << "MOV DS, AX" << endl;
        }
        else if (quaternion.op == "CALL") {
            cout << "CALL " << quaternion.opt1 << endl;
        }
    }

    cout << "MOV AX, 4C00H" << endl;
    cout << "INT 21H" << endl;
    cout << "CODE ENDS" << endl;
    cout << "END START" << endl;
}

int main() {
    vector<Quaternion> quaternions = {
        {"DECL", "a", "", ""},
        {"DECL", "b", "", ""},
        {"DECL", "c", "", ""},
        {"+", "a", "b", "t1"},
        {"*", "t1", "c", "t2"},
        {"-", "t2", "d", "t3"},
        {"if", "t3", "0", "L1"},
        {"while", "t3", "10", "L2"},
        {"for", "i", "0", "L3"},
        {"CALL", "func", "", ""}
    };//正常是根据jjh传

    generateAssembly(quaternions);
    return 0;
}
