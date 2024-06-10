#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <cstdio> // for sprintf
#include <cstdlib> // for system("pause")

using namespace std;

struct token {
    string TokenA;
    int TokenB;
};

struct quaternion {
    string op;      // 操作符
    string opt1;    // 操作数1
    string opt2;    // 操作数2
    string result;  // 结果
};

vector<token> Token;        // 全局变量，用来存储分析好的token序列
vector<quaternion> Quaternion; // 全局变量，用来存储生成的四元式序列

map<string, int> K;  // 关键字
map<string, int> P;  // 界符

// 初始化关键字表
void iniK() {
    K["int"] = 1;
    K["float"] = 2;
    K["void"] = 3;
    K["double"] = 4;
    K["bool"] = 5;
    K["for"] = 6;
    K["while"] = 7;
    K["if"] = 8;
    K["else"] = 9;
    K["do"] = 10;
    K["break"] = 11;
    K["main"] = 12;
    K["return"] = 13;
    K["string"] = 14;
    K["char"] = 15;
    K["struct"] = 16;
    K["continue"] = 17;
}

// 初始化界符表
void iniP() {
    P["+"] = 1;
    P["-"] = 2;
    P["*"] = 3;
    P["/"] = 4;
    P[","] = 5;
    P[":"] = 6;
    P[";"] = 7;
    P["("] = 8;
    P[")"] = 9;
    P["{"] = 10;
    P["}"] = 11;
    P["["] = 12;
    P["]"] = 13;
    P["+="] = 14;
    P["-="] = 15;
    P["++"] = 16;
    P["--"] = 17;
    P["!"] = 18;
    P["&"] = 19;
    P["&&"] = 20;
    P["||"] = 21;
    P["%"] = 22;
    P["~"] = 23;
    P["="] = 24;
    P["!="] = 25;
    P[">"] = 26;
    P["<"] = 27;
    P[">="] = 28;
    P["<="] = 29;
    P["|"] = 30;
    P["^"] = 31;
    P["'"] = 32;
    P["\""] = 33;
}

// 生成临时变量
string generateTempVar(int& tempVarCount) {
    char tempVar[10];
    sprintf(tempVar, "t%d", tempVarCount++);
    return string(tempVar);
}

// 处理表达式，生成四元式
string processExpression(const vector<token>& tokens, size_t& i, int& tempVarCount) {
    stack<string> ops;   // 操作符栈
    stack<string> vals;  // 操作数栈

    auto applyOp = [&]() {
        string op = ops.top();
        ops.pop();

        string val2 = vals.top();
        vals.pop();

        string val1 = vals.top();
        vals.pop();

        string tempVar = generateTempVar(tempVarCount);

        quaternion q;
        q.op = op;
        q.opt1 = val1;
        q.opt2 = val2;
        q.result = tempVar;
        Quaternion.push_back(q);

        vals.push(tempVar);
    };

    auto precedence = [](const string& op) -> int {
        if (op == "*" || op == "/") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;
    };

    while (i < tokens.size() && tokens[i].TokenA != ";" && tokens[i].TokenA != ")") {
        if (tokens[i].TokenA == "(") {
            // 递归处理括号内的表达式
            i++;
            vals.push(processExpression(tokens, i, tempVarCount));
        } else if (tokens[i].TokenA == ")") {
            i++;
            break;
        } else if (P.find(tokens[i].TokenA) != P.end()) {
            // 处理操作符
            while (!ops.empty() && precedence(ops.top()) >= precedence(tokens[i].TokenA)) {
                applyOp();
            }
            ops.push(tokens[i].TokenA);
        } else {
            // 处理操作数
            vals.push(tokens[i].TokenA);
        }
        i++;
    }

    while (!ops.empty()) {
        applyOp();
    }

    return vals.top();
}

// 生成四元式并存入全局变量Quaternion
void generateQuaternion(const vector<token>& tokens) {
    int tempVarCount = 0;

    for (size_t i = 0; i < tokens.size(); i++) {
        cout<<"i:"<<i<<endl;
        if (tokens[i].TokenA == "=") {
            // 赋值操作
            string leftVar = tokens[i - 1].TokenA;
            string rightExpr = processExpression(tokens, ++i, tempVarCount);
            quaternion q;
            q.op = "=";
            q.opt1 = rightExpr; 
            q.opt2 = "";
            q.result = leftVar;
            Quaternion.push_back(q);
        } 
        else if (K.find(tokens[i].TokenA) != K.end() && K[tokens[i].TokenA] == 1) {
            // 处理int关键字
            if (tokens[i + 2].TokenA == "("&&tokens[i+1].TokenA!="main") {
                // 函数定义
                quaternion q;
                q.op = "FUNC";
                q.opt1 = tokens[i].TokenA;
                q.opt2 = "";
                q.result = tokens[i + 1].TokenA;
                Quaternion.push_back(q);

                // 处理参数列表
                i += 3; // 跳过函数名和左括号
                while (tokens[i].TokenA != ")") {
                    if (K.find(tokens[i].TokenA) != K.end()) {
                        quaternion param;
                        param.op = "PARAM";
                        param.opt1 = tokens[i].TokenA;
                        param.opt2 = "";
                        param.result = tokens[i + 1].TokenA;
                        Quaternion.push_back(param);
                        i += 2;
                    }
                    if (tokens[i].TokenA == ",") {
                        i++;
                    }
                }
                // 跳过右括号和左大括号
                i += 2;
            } else if (tokens[i + 2].TokenA == "," || tokens[i + 2].TokenA == ";") {
                // 多变量声明
                size_t j = i + 1;
                while (tokens[j].TokenA != ";") {
                    if (tokens[j].TokenA == ",") {
                        j++;
                        continue;
                    }
                    quaternion q;
                    q.op = "DECL";
                    q.opt1 = tokens[i].TokenA;
                    q.opt2 = "";
                    q.result = tokens[j].TokenA;
                    Quaternion.push_back(q);
                    j++;
                }
                i = j;
            } else if (tokens[i + 2].TokenA == "=") {
                // 变量声明并赋值
                quaternion q;
                q.op = "DECL";
                q.opt1 = tokens[i].TokenA;
                q.opt2 = "";
                q.result = tokens[i + 1].TokenA;
                Quaternion.push_back(q);

                quaternion q2;
                q2.op = "=";
                q2.opt1 = processExpression(tokens, i += 3, tempVarCount); // 处理右侧表达式
                q2.opt2 = "";
                q2.result = tokens[i - 1].TokenA;
                Quaternion.push_back(q2);
                i += 2;
            }
        } else if (tokens[i].TokenA == "main" && tokens[i + 1].TokenA == "(") {
            // 处理主函数定义
            quaternion q;
            q.op = "FUNC";
            q.opt1 = "int";
            q.opt2 = "";
            q.result = "main";
            Quaternion.push_back(q);

            // 跳过右括号和左大括号
            i += 3;
        } else if (tokens[i].TokenA == "return") {
            // 处理return语句
            quaternion q;
            q.op = "RETURN";
            q.opt1 = "";
            q.opt2 = "";
            q.result = tokens[i + 1].TokenA;
            Quaternion.push_back(q);
            i += 1; // 跳过return值
        } else if (tokens[i].TokenA == "}") {
            // 处理函数结束
            quaternion q;
            q.op = "END_FUNC";
            q.opt1 = "";
            q.opt2 = "";
            q.result = "main";
            Quaternion.push_back(q);
        }
    }
}
