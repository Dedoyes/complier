#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio> // for sprintf
#include<stdlib.h>
using namespace std;

// 生成四元式并存入全局变量Quaternion
void generateQuaternion(const vector<token>& tokens) {
    int tempVarCount = 0;

    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].TokenA == "=") {//如果遇到等于号
            // 赋值操作
            quaternion q;
            q.op = "=";
            q.opt1 = tokens[i+1].TokenA;
            q.opt2 = "";
            q.result = tokens[i-1].TokenA;
            Quaternion.push_back(q);
        }
        else if (K.find(tokens[i].TokenA) != K.end() && K[tokens[i].TokenA] == 1) {//遇到int
            // 处理变量声明（如 int a = 5）
            if (tokens[i+1].TokenA != "(") {
                quaternion q;
                q.op = "DECL";
                q.opt1 = tokens[i].TokenA;
                q.opt2 = "";
                q.result = tokens[i+1].TokenA;
                Quaternion.push_back(q);

                // 处理赋值
                if (tokens[i+2].TokenA == "=") {
                    quaternion q2;
                    q2.op = "=";
                    q2.opt1 = tokens[i+3].TokenA;
                    q2.opt2 = "";
                    q2.result = tokens[i+1].TokenA;
                    Quaternion.push_back(q2);
                    i += 3; // 跳过赋值部分
                } else {
                    i++; // 跳过变量名
                }
            }
            // 处理函数定义
            else {
                quaternion q;
                q.op = "FUNC";
                q.opt1 = tokens[i].TokenA;
                q.opt2 = "";
                q.result = tokens[i+1].TokenA;
                Quaternion.push_back(q);

                // 处理参数列表
                i += 2; // 跳过函数名和左括号
                while (tokens[i].TokenA != ")") {
                    if (K.find(tokens[i].TokenA) != K.end()) {
                        quaternion param;
                        param.op = "PARAM";
                        param.opt1 = tokens[i].TokenA;
                        param.opt2 = "";
                        param.result = tokens[i+1].TokenA;
                        Quaternion.push_back(param);
                        i += 2;
                    }
                    if (tokens[i].TokenA == ",") {
                        i++;
                    }
                }
                // 跳过右括号和左大括号
                i += 2;
            }
        }
        else if (tokens[i].TokenA == "main" && tokens[i+1].TokenA == "(") {//遇到main
            // 处理主函数定义
            quaternion q;
            q.op = "MAIN";
            q.opt1 = tokens[i-1].TokenA;
            q.opt2 = "";
            q.result = "main";
            Quaternion.push_back(q);

            // 跳过右括号和左大括号
            i += 3;
        }
        else if (P.find(tokens[i].TokenA) != P.end()) {//遇到加减乘除
            // 处理算术运算
            if (tokens[i].TokenA == "+" || tokens[i].TokenA == "-" ||//遇到加减乘除
                tokens[i].TokenA == "*" || tokens[i].TokenA == "/") {
                quaternion q;
                q.op = tokens[i].TokenA;
                q.opt1 = tokens[i-1].TokenA;
                q.opt2 = tokens[i+1].TokenA;

                char tempVar[10];
                sprintf(tempVar, "t%d", tempVarCount++);
                q.result = string(tempVar);

                Quaternion.push_back(q);

                // 生成赋值四元式
                if (tokens.size() > i + 2 && tokens[i+2].TokenA == "=") {
                    quaternion q2;
                    q2.op = "=";
                    q2.opt1 = q.result;
                    q2.opt2 = "";
                    q2.result = tokens[i+3].TokenA;
                    Quaternion.push_back(q2);
                    i += 3; // 跳过赋值部分
                }
            }
            // 处理比较运算
            else if (tokens[i].TokenA == ">" || tokens[i].TokenA == "<" ||
                     tokens[i].TokenA == ">=" || tokens[i].TokenA == "<=" ||
                     tokens[i].TokenA == "==" || tokens[i].TokenA == "!=") {
                quaternion q;
                q.op = tokens[i].TokenA;
                q.opt1 = tokens[i-1].TokenA;
                q.opt2 = tokens[i+1].TokenA;

                char tempVar[10];
                sprintf(tempVar, "t%d", tempVarCount++);
                q.result = string(tempVar);

                Quaternion.push_back(q);
            }
        }
        else if (tokens[i].TokenA == "if") {//遇到if
            // 处理 if 语句
            quaternion q;
            q.op = "IF";
            q.opt1 = tokens[i+2].TokenA; // 条件
            q.opt2 = tokens[i+4].TokenA; // 比较对象
            char tempVar[10];
            sprintf(tempVar, "t%d", tempVarCount++);
            q.result = string(tempVar);

            Quaternion.push_back(q);
            i += 4; // 跳过 if 条件部分
        }
        else if (tokens[i].TokenA == "while") {//遇到while
            // 处理 while 语句
            quaternion q;
            q.op = "WHILE";
            q.opt1 = tokens[i+2].TokenA; // 条件
            q.opt2 = tokens[i+4].TokenA; // 比较对象
            char tempVar[10];
            sprintf(tempVar, "t%d", tempVarCount++);
            q.result = string(tempVar);

            Quaternion.push_back(q);
            i += 4; // 跳过 while 条件部分
        }
        else if (tokens[i].TokenA == "for") {//遇到for
            // 处理 for 语句
            quaternion q1, q2, q3, q4;
            q1.op = "FOR";
            q1.opt1 = tokens[i+2].TokenA; // 初始化
            q1.opt2 = tokens[i+3].TokenA;
            q1.result = tokens[i+5].TokenA;

            q2.op = "IF";
            q2.opt1 = tokens[i+7].TokenA; // 条件
            q2.opt2 = tokens[i+9].TokenA;
            char tempVar[10];
            sprintf(tempVar, "t%d", tempVarCount++);
            q2.result = string(tempVar);

            q3.op = tokens[i+11].TokenA; // 更新
            q3.opt1 = tokens[i+10].TokenA;
            q3.opt2 = "";
            q3.result = "";

            q4.op = "END_FOR";
            q4.opt1 = "";
            q4.opt2 = "";
            q4.result = "";

            Quaternion.push_back(q1);
            Quaternion.push_back(q2);
            Quaternion.push_back(q3);
            Quaternion.push_back(q4);
            i += 11; // 跳过 for 语句部分
        }
        else if (tokens[i].TokenA == "struct") {//遇到struct
            // 处理 struct 定义
            quaternion q;
            q.op = "STRUCT";
            q.opt1 = "";
            q.opt2 = "";
            q.result = tokens[i+1].TokenA;
            Quaternion.push_back(q);

            // 处理 struct 内部变量声明
            i += 3; // 跳过 struct 名称和左大括号
            while (tokens[i].TokenA != "}") {
                if (K.find(tokens[i].TokenA) != K.end()) {
                    quaternion varDecl;
                    varDecl.op = "DECL";
                    varDecl.opt1 = tokens[i].TokenA;
                    varDecl.opt2 = "";
                    varDecl.result = tokens[i+1].TokenA;
                    Quaternion.push_back(varDecl);
                    i += 2;
                }
            }
        }
    }
}
