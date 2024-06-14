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
        // 检查变量是否已经在寄存器中
        for (const auto& entry : regMap) {
            if (entry.second == var) {
                return entry.first;
            }
        }

        // 优先使用空闲寄存器
        for (string reg : registers) {
            if (regMap[reg] == "") {
                regMap[reg] = var;
                return reg;
            }
        }

        // 使用存放不活跃变量的寄存器
        for (string reg : registers) {
            if (varLiveness[regMap[reg]] < currentIndex) {
                freeRegister(reg);
                regMap[reg] = var;
                return reg;
            }
        }

        // 按优先顺序释放寄存器
        for (string reg : registers) {
            freeRegister(reg);
            regMap[reg] = var;
            return reg;
        }

        return ""; // 不应到达这里
    }

    void freeRegister(string reg) {
        if (regMap[reg] != "" && regMap[reg] != "AX" && regMap[reg] != "BX") {
            cout << "MOV " << regMap[reg] << ", " << reg << endl;
            regMap[reg] = "";
        }
    }

    void allocateToAX(string var, unordered_map<string, int>& varLiveness, int currentIndex) {
        if (regMap["AX"] == var) {
            return; // 如果AX已经包含变量，直接返回
        }
        if (regMap["AX"] != "") {
            freeRegister("AX");
        }
        regMap["AX"] = var;
    }

    void allocateToBX(string var, unordered_map<string, int>& varLiveness, int currentIndex) {
        if (regMap["BX"] == var) {
            return; // 如果BX已经包含变量，直接返回
        }
        if (regMap["BX"] != "") {
            freeRegister("BX");
        }
        regMap["BX"] = var;
    }

    string getRegister(string var) {
        for (const auto& entry : regMap) {
            if (entry.second == var) {
                return entry.first;
            }
        }
        return "";
    }

private:
    vector<string> registers;
    unordered_map<string, string> regMap;
};

// 获取变量活跃信息
unordered_map<string, int> getLivenessInfo(const vector<Quaternion>& quaternions) {
    unordered_map<string, int> varLiveness;
    for (int i = 0; i < quaternions.size(); ++i) {
        if (quaternions[i].opt1 != "" && !isdigit(quaternions[i].opt1[0]) && quaternions[i].opt1[0] != '\'') {
            varLiveness[quaternions[i].opt1] = i;
        }
        if (quaternions[i].opt2 != "" && !isdigit(quaternions[i].opt2[0]) && quaternions[i].opt2[0] != '\'') {
            varLiveness[quaternions[i].opt2] = i;
        }
        if (quaternions[i].result != "" && !isdigit(quaternions[i].result[0]) && quaternions[i].result[0] != '\'') {
            varLiveness[quaternions[i].result] = i;
        }
    }
    return varLiveness;
}

// 检查是否是立即数或字符常量
bool isImmediateOrConstant(const string& operand) {
    return isdigit(operand[0]) || operand[0] == '\'';
}

// 检查是否是无效标识符
bool isInvalidIdentifier(const string& operand) {
    static unordered_set<string> invalidIdentifiers = { "main", "int", "char", "FUNC", "function_begin", "function_end", "param", "call", "return", "JUMP_FALSE", "LABEL", "JUMP", "struct_begin", "struct_end" };
    return invalidIdentifiers.find(operand) != invalidIdentifiers.end();
}

// 检查是否是数组变量
bool isArrayVariable(const unordered_map<string, string>& arrayTypes, const string& operand) {
    return arrayTypes.find(operand) != arrayTypes.end();
}

// 生成8086汇编代码
void generateAssembly(const vector<Quaternion>& quaternions) {
    unordered_map<string, int> varLiveness = getLivenessInfo(quaternions);
    RegisterAllocator regAlloc;
    int labelCount = 0; // 用于生成唯一的标签

    // 初始化段定义
    cout << "SSEG SEGMENT STACK" << endl;
    cout << "STK DB 20 DUP(0)" << endl;
    cout << "SSEG ENDS" << endl;
    cout << "DSEG SEGMENT" << endl;

    // 声明变量
    unordered_set<string> declaredVars;
    unordered_set<string> functionNames;
    unordered_set<string> operators = { "+", "-", "*", "/", "==", "!=", "<", "<=", ">", ">=" };
    unordered_map<string, string> arrayTypes; // 记录数组类型
    unordered_map<string, vector<pair<string, string>>> structs; // 记录结构体定义

    // 首先记录所有的函数名和结构体定义
    string currentStruct;
    for (const auto& quaternion : quaternions) {
        if (quaternion.op == "FUNC" || quaternion.op == "function_begin") {
            functionNames.insert(quaternion.result);
        }
        if (quaternion.op == "struct_begin") {
            currentStruct = quaternion.result;
            structs[currentStruct] = {};
        }
        if (quaternion.op == "struct_end") {
            currentStruct = "";
        }
        if (currentStruct != "" && quaternion.op != "struct_begin" && quaternion.op != "struct_end") {
            structs[currentStruct].push_back({ quaternion.op, quaternion.opt1 });
        }
    }

    // 声明变量，排除函数名和操作符，并排除标签和立即数
    for (const auto& quaternion : quaternions) {
        if (quaternion.op == "ARRAY") {
            string arrayType = quaternion.opt2;
            arrayTypes[quaternion.result] = arrayType;
            if (arrayType == "int") {
                cout << quaternion.result << " DW " << quaternion.opt1 << " DUP(0)" << endl;
            }
            else if (arrayType == "char") {
                cout << quaternion.result << " DB " << quaternion.opt1 << " DUP(0)" << endl;
            }
            declaredVars.insert(quaternion.result);
        }
        else if (quaternion.op != "LABEL" && quaternion.op != "JUMP_FALSE" && quaternion.op != "JUMP" &&
            quaternion.op != "function_begin" && quaternion.op != "function_end" &&
            quaternion.op != "param" && quaternion.op != "call" && quaternion.op != "return" &&
            quaternion.op != "struct_begin" && quaternion.op != "struct_end") {
            if (quaternion.opt1 != "" && quaternion.opt1 != "_" && !isImmediateOrConstant(quaternion.opt1) &&
                !isInvalidIdentifier(quaternion.opt1) &&
                declaredVars.find(quaternion.opt1) == declaredVars.end() &&
                functionNames.find(quaternion.opt1) == functionNames.end() &&
                operators.find(quaternion.opt1) == operators.end()) {
                cout << quaternion.opt1 << " DW 0" << endl;
                declaredVars.insert(quaternion.opt1);
            }
            if (quaternion.opt2 != "" && quaternion.opt2 != "_" && !isImmediateOrConstant(quaternion.opt2) &&
                !isInvalidIdentifier(quaternion.opt2) &&
                declaredVars.find(quaternion.opt2) == declaredVars.end() &&
                functionNames.find(quaternion.opt2) == functionNames.end() &&
                operators.find(quaternion.opt2) == operators.end()) {
                cout << quaternion.opt2 << " DW 0" << endl;
                declaredVars.insert(quaternion.opt2);
            }
            if (quaternion.result != "" && quaternion.result != "_" && !isImmediateOrConstant(quaternion.result) &&
                !isInvalidIdentifier(quaternion.result) &&
                declaredVars.find(quaternion.result) == declaredVars.end() &&
                functionNames.find(quaternion.result) == functionNames.end() &&
                operators.find(quaternion.result) == operators.end()) {
                cout << quaternion.result << " DW 0" << endl;
                declaredVars.insert(quaternion.result);
            }
        }
        else if (quaternion.op == "param" && declaredVars.find(quaternion.result) == declaredVars.end()) {
            cout << quaternion.result << " DW 0" << endl;
            declaredVars.insert(quaternion.result);
        }
    }

    // 声明结构体
    for (const auto& structDef : structs) {
        cout << structDef.first << " STRUC" << endl;
        for (const auto& member : structDef.second) {
            if (member.first == "int") {
                cout << "  " << member.second << " DW 0" << endl;
            }
            else if (member.first == "char") {
                cout << "  " << member.second << " DB 0" << endl;
            }
        }
        cout << structDef.first << " ENDS" << endl;
    }

    cout << "DSEG ENDS" << endl;

    // 生成代码段
    cout << "CSEG SEGMENT" << endl;
    cout << "ASSUME CS:CSEG, DS:DSEG" << endl;

    cout << "START:" << endl;
    cout << "MOV AX, DSEG" << endl;
    cout << "MOV DS, AX" << endl;
    cout << "MOV AX, SSEG" << endl;
    cout << "MOV SS, AX" << endl;
    cout << "MOV SP, SIZE STK" << endl;

    // 生成主函数的起始和结束标签
    cout << "main_start:" << endl;

    // 生成四元式对应的指令
    for (int i = 0; i < quaternions.size(); ++i) {
        const auto& quaternion = quaternions[i];
        if (quaternion.op == "ARRAY_ASSIGN") {
            // 数组赋值操作
            cout << "MOV CX, " << quaternion.opt2 << endl; // 索引
            cout << "MOV BX, " << quaternion.opt1 << endl; // 基地址
            cout << "SHL CX, 1" << endl; // 乘以2，因为是int数组
            cout << "ADD BX, CX" << endl; // 将偏移量加到基地址上
            cout << "MOV DX, " << quaternion.result << endl; // 将值加载到DX寄存器
            cout << "MOV [BX], DX" << endl; // 使用计算后的地址和中间寄存器进行存储
        }
        else if (quaternion.op == "ARRAY_ACCESS") {
            // 数组访问操作
            cout << "MOV CX, " << quaternion.opt2 << endl; // 索引
            cout << "MOV BX, " << quaternion.opt1 << endl; // 基地址
            cout << "SHL CX, 1" << endl; // 乘以2，因为是int数组
            cout << "ADD BX, CX" << endl; // 将偏移量加到基地址上
            cout << "MOV DX, [BX]" << endl; // 使用计算后的地址加载数据到DX寄存器
            cout << "MOV " << quaternion.result << ", DX" << endl; // 将数据从DX寄存器加载到目标变量
        }
        else if (quaternion.op == "+" || quaternion.op == "-" || quaternion.op == "*" || quaternion.op == "/") {
            if (quaternion.op == "*") {
                // 乘法操作，目标操作数为 AX
                regAlloc.allocateToAX(quaternion.opt1, varLiveness, i);
                if (!isImmediateOrConstant(quaternion.opt1)) {
                    cout << "MOV AX, " << quaternion.opt1 << endl;
                }
                else {
                    cout << "MOV AX, " << quaternion.opt1 << endl;
                }
                string reg2;
                if (!isImmediateOrConstant(quaternion.opt2)) {
                    reg2 = regAlloc.allocate(quaternion.opt2, varLiveness, i);
                    cout << "MOV " << reg2 << ", " << quaternion.opt2 << endl;
                }
                else {
                    // 使用中间寄存器存储立即数
                    reg2 = regAlloc.allocate("temp", varLiveness, i);
                    cout << "MOV " << reg2 << ", " << quaternion.opt2 << endl;
                }
                cout << "IMUL " << reg2 << endl;
                cout << "MOV " << quaternion.result << ", AX" << endl;
            }
            else {
                string reg1, reg2;
                if (!isImmediateOrConstant(quaternion.opt1)) {
                    reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
                    cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
                }
                else {
                    reg1 = quaternion.opt1;
                }
                if (!isImmediateOrConstant(quaternion.opt2)) {
                    reg2 = regAlloc.allocate(quaternion.opt2, varLiveness, i);
                    cout << "MOV " << reg2 << ", " << quaternion.opt2 << endl;
                }
                else {
                    reg2 = quaternion.opt2;
                }
                if (quaternion.op == "+") cout << "ADD " << reg1 << ", " << reg2 << endl;
                if (quaternion.op == "-") cout << "SUB " << reg1 << ", " << reg2 << endl;
                if (quaternion.op == "/") {
                    cout << "XOR DX, DX" << endl;
                    cout << "IDIV " << reg2 << endl;
                }
                cout << "MOV " << quaternion.result << ", " << reg1 << endl;
            }
        }
        else if (quaternion.op == "=") {
            if (isArrayVariable(arrayTypes, quaternion.result) || isArrayVariable(arrayTypes, quaternion.opt1)) {
                // 对于数组赋值的处理逻辑
                string baseReg = regAlloc.allocate(quaternion.result, varLiveness, i);
                string indexReg = regAlloc.allocate(quaternion.opt1, varLiveness, i);
                cout << "MOV " << indexReg << ", " << quaternion.opt1 << endl;
                cout << "MOV [" << baseReg << "+" << indexReg << "*2], " << quaternion.opt2 << endl; // int 类型定义为 DW，所以 +indexReg*2
            }
            else if (!isImmediateOrConstant(quaternion.opt1) &&
                operators.find(quaternion.opt1) == operators.end() && functionNames.find(quaternion.opt1) == functionNames.end()) {
                string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
                cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
                cout << "MOV " << quaternion.result << ", " << reg1 << endl;
            }
            else {
                cout << "MOV " << quaternion.result << ", " << quaternion.opt1 << endl;  // 将立即数或字符常量直接赋值给结果
            }
        }
        else if (quaternion.op == "<=" || quaternion.op == ">" || quaternion.op == "<" || quaternion.op == ">=" || quaternion.op == "==" || quaternion.op == "!=") {
            string reg1, reg2;
            if (!isImmediateOrConstant(quaternion.opt1)) {
                reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
                cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            }
            else {
                reg1 = quaternion.opt1;
            }
            if (!isImmediateOrConstant(quaternion.opt2)) {
                reg2 = regAlloc.allocate(quaternion.opt2, varLiveness, i);
                cout << "MOV " << reg2 << ", " << quaternion.opt2 << endl;
            }
            else {
                reg2 = quaternion.opt2;
            }

            // 避免生成 CMP reg, reg 的指令
            if (reg1 != reg2) {
                cout << "CMP " << reg1 << ", " << reg2 << endl;
            }
            else {
                // 如果reg1 == reg2，说明两个操作数是同一个变量
                // 此时直接根据比较操作的类型生成结果
                if (quaternion.op == "==") {
                    cout << "MOV " << quaternion.result << ", 1" << endl;
                    continue;
                }
                else {
                    cout << "MOV " << quaternion.result << ", 0" << endl;
                    continue;
                }
            }

            // 生成自动分配的标签
            string label = "LABEL_" + to_string(labelCount++);
            string labelEnd = "LABEL_END_" + to_string(labelCount++);

            if (quaternion.op == "<=") {
                cout << "JG " << label << endl;
                cout << "MOV " << quaternion.result << ", 1" << endl;
                cout << "JMP " << labelEnd << endl;
                cout << label << ":" << endl;
                cout << "MOV " << quaternion.result << ", 0" << endl;
                cout << labelEnd << ":" << endl;
            }
            if (quaternion.op == "<") {
                cout << "JGE " << label << endl;
                cout << "MOV " << quaternion.result << ", 1" << endl;
                cout << "JMP " << labelEnd << endl;
                cout << label << ":" << endl;
                cout << "MOV " << quaternion.result << ", 0" << endl;
                cout << labelEnd << ":" << endl;
            }
            if (quaternion.op == ">") {
                cout << "JLE " << label << endl;
                cout << "MOV " << quaternion.result << ", 1" << endl;
                cout << "JMP " << labelEnd << endl;
                cout << label << ":" << endl;
                cout << "MOV " << quaternion.result << ", 0" << endl;
                cout << labelEnd << ":" << endl;
            }
            if (quaternion.op == ">=") {
                cout << "JL " << label << endl;
                cout << "MOV " << quaternion.result << ", 1" << endl;
                cout << "JMP " << labelEnd << endl;
                cout << label << ":" << endl;
                cout << "MOV " << quaternion.result << ", 0" << endl;
                cout << labelEnd << ":" << endl;
            }
            if (quaternion.op == "==") {
                cout << "JNE " << label << endl;
                cout << "MOV " << quaternion.result << ", 1" << endl;
                cout << "JMP " << labelEnd << endl;
                cout << label << ":" << endl;
                cout << "MOV " << quaternion.result << ", 0" << endl;
                cout << labelEnd << ":" << endl;
            }
            if (quaternion.op == "!=") {
                cout << "JE " << label << endl;
                cout << "MOV " << quaternion.result << ", 1" << endl;
                cout << "JMP " << labelEnd << endl;
                cout << label << ":" << endl;
                cout << "MOV " << quaternion.result << ", 0" << endl;
                cout << labelEnd << ":" << endl;
            }
        }
        else if (quaternion.op == "JUMP_FALSE") {
            string reg1 = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg1 << ", " << quaternion.opt1 << endl;
            cout << "CMP " << reg1 << ", 0" << endl;
            cout << "JE " << quaternion.result << endl;
        }
        else if (quaternion.op == "LABEL") {
            cout << quaternion.result << ":" << endl;
        }
        else if (quaternion.op == "JUMP") {
            cout << "JMP " << quaternion.result << endl;
        }
        else if (quaternion.op == "function_begin") {
            cout << quaternion.result << " PROC" << endl;
        }
        else if (quaternion.op == "function_end") {
            cout << quaternion.result << " ENDP" << endl;
        }
        else if (quaternion.op == "param") {
            // 删除 param 处理逻辑
        }
        else if (quaternion.op == "call") {
            // 函数调用逻辑
            cout << "CALL " << quaternion.opt1 << endl;
        }
        else if (quaternion.op == "return" && quaternion.result != "function") {
            // 普通返回值处理逻辑
            cout << "MOV AX, " << quaternion.opt1 << endl;
        }
        else if (quaternion.op == "return" && quaternion.result == "function") {
            // 递归调用函数处理逻辑
            cout << "CALL " << quaternion.opt1 << endl;
        }
        else if (quaternion.op == "arg") {
            // 调用参数处理逻辑
            string reg = regAlloc.allocate(quaternion.opt1, varLiveness, i);
            cout << "MOV " << reg << ", " << quaternion.opt1 << endl;
            cout << "MOV " << quaternion.result << ", " << reg << endl;
        }
    }

    // 主函数结束标签
    cout << "main_end:" << endl;

    // 结束代码段和程序
    cout << "MOV AX, 4C00H" << endl;
    cout << "INT 21H" << endl;
    cout << "CSEG ENDS" << endl;
    cout << "END START" << endl;
}

int main() {
    vector<Quaternion> quaternions = {
        {"struct_begin", "", "", "myStruct"},
        {"int", "i", "", ""},
        {"int", "j", "", ""},
        {"char", "c", "", ""},
        {"struct_end", "", "", "myStruct"},
        {"function_begin", "int", "_", "func"},
        {"==", "a", "b", "t1"},
        {"JUMP_FALSE", "t1", "_", "L2"},
        {"+", "a", "1", "t1"},
        {"=", "t1", "_", "a"},
        {"JUMP", "_", "_", "L3"},
        {"LABEL", "_", "_", "L2"},
        {"==", "a", "b", "t4"},
        {"JUMP_FALSE", "t4", "_", "L5"},
        {"!=", "a", "b", "t6"},
        {"JUMP_FALSE", "t6", "_", "L7"},
        {"+", "a", "1", "t1"},
        {"=", "t1", "_", "a"},
        {"JUMP", "_", "_", "L8"},
        {"LABEL", "_", "_", "L7"},
        {"LABEL", "_", "_", "L8"},
        {"+", "b", "1", "t1"},
        {"=", "t1", "_", "b"},
        {"JUMP", "_", "_", "L3"},
        {"LABEL", "_", "_", "L5"},
        {"LABEL", "_", "_", "L3"},
        {"return", "func", "_", "function"},
        {"function_end", "_", "_", "func"}
    };

    generateAssembly(quaternions);
    return 0;
}
