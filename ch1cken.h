#ifndef TEAM_COMPLIER_QUA2_H
#define TEAM_COMPLIER_QUA2_H

vector<quaternion> Quaternion; // 全局变量，用来存储生成的四元式序列
map<string, int> OP; // 操作符优先级

void iniOP() {
    OP["+"] = 1;
    OP["-"] = 1;
    OP["*"] = 2;
    OP["/"] = 2;
    OP["+="] = 1;
    OP["-="] = 1;
    OP["*="] = 2;
    OP["/="] = 2;
    OP["("] = 0;
}

void addToken(const string& TokenA, int TokenB) {
    WordFinal.Token.push_back({TokenA, TokenB});
}

void addQuaternion(const string& op, const string& opt1, const string& opt2, const string& result) {
    Quaternion.push_back({op, opt1, opt2, result});
}

vector<string> infixToPostfix(const vector<token>& expr) {
    stack<string> opStack;
    vector<string> postfix;
    for (const auto& t : expr) {
        if (t.TokenB == 0) {
            postfix.push_back(t.TokenA);
        } else if (t.TokenA == "(") {
            opStack.push(t.TokenA);
        } else if (t.TokenA == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.pop(); // pop "("
        } else {
            while (!opStack.empty() && OP[opStack.top()] >= OP[t.TokenA]) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(t.TokenA);
        }
    }
    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }
    return postfix;
}

string evaluatePostfix(vector<string>& postfix) {
    stack<string> evalStack;
    int tempCount = 1;
    for (const auto& token : postfix) {
        if (OP.find(token) == OP.end()) {
            evalStack.push(token);
        } else {
            string op2 = evalStack.top(); evalStack.pop();
            string op1 = evalStack.top(); evalStack.pop();
            string tempVar = "t" + to_string(tempCount++);
            addQuaternion(token, op1, op2, tempVar);
            evalStack.push(tempVar);
        }
    }
    return evalStack.top();
}

void parseExpression(size_t& i, int& labelCount);
void parseExpression_for(size_t& i, int& labelCount);
void parseBlock(size_t& i, int& labelCount);
void parseVariableDeclaration(size_t& i);
void parseIfStatement(size_t& i, int& labelCount);
void parseWhileStatement(size_t& i, int& labelCount);
void parseForStatement(size_t& i, int& labelCount);

void parseIfStatement(size_t& i, int& labelCount) {
    i++;
    if (WordFinal.Token[i].TokenA == "(") {
        i++;
        vector<token> expr;
        while (WordFinal.Token[i].TokenA != ")") {
            expr.push_back(WordFinal.Token[i]);
            i++;
        }
        vector<string> postfix = infixToPostfix(expr);
        string result = evaluatePostfix(postfix);
        string tempVar = "t" + to_string(labelCount++);
        addQuaternion(postfix.back(), postfix[postfix.size() - 3], postfix[postfix.size() - 2], tempVar);
        string falseLabel = "L" + to_string(labelCount++);
        addQuaternion("JUMP_FALSE", tempVar, "_", falseLabel);
        i++; // 跳过")"
        parseBlock(i, labelCount);
        addQuaternion("LABEL", "_", "_", falseLabel);
    }
}
void parseExpression(size_t& i, int& labelCount) {
    string varName = WordFinal.Token[i].TokenA;
    i++;
    if (WordFinal.Token[i].TokenA == "=" || WordFinal.Token[i].TokenA == "+=" || WordFinal.Token[i].TokenA == "-=" || WordFinal.Token[i].TokenA == "*=" || WordFinal.Token[i].TokenA == "/=") {
        string op = WordFinal.Token[i].TokenA;
        i++;
        vector<token> expr;
        while (WordFinal.Token[i].TokenA != ";") {
            expr.push_back(WordFinal.Token[i]);
            i++;
        }
        vector<string> postfix = infixToPostfix(expr);
        string result = evaluatePostfix(postfix);
        if (op == "=") {
            addQuaternion("=", result, "_", varName);
        } else {
            string basicOp = op.substr(0, 1);
            addQuaternion(basicOp, varName, result, varName);
        }
        i++; // 跳过分号
    } else if (WordFinal.Token[i].TokenA == "++") { // 处理自增操作符
        addQuaternion("+", varName, "1", varName);
        i++; // 跳过++
        if (WordFinal.Token[i].TokenA == ";") {
            i++; // 跳过分号
        }
    } else if (WordFinal.Token[i].TokenA == "--") { // 处理自减操作符
        addQuaternion("-", varName, "1", varName);
        i++; // 跳过--
        if (WordFinal.Token[i].TokenA == ";") {
            i++; // 跳过分号
        }
    } else if (WordFinal.Token[i].TokenA == "[") { // 数组访问或赋值
        i++; // 跳过"["
        string index = WordFinal.Token[i].TokenA;
        i++; // 跳过索引
        if (WordFinal.Token[i].TokenA == "]") {
            i++; // 跳过"]"
            if (WordFinal.Token[i].TokenA == "=") {
                i++; // 跳过"="
                string value = WordFinal.Token[i].TokenA;
                addQuaternion("ARRAY_ASSIGN", varName, index, value);
                i++; // 跳过值
                i++; // 跳过";"
            } else {
                addQuaternion("ARRAY_ACCESS", varName, index, "t" + to_string(labelCount++));
                i++; // 跳过分号
            }
        }
    } else if (WordFinal.Token[i].TokenA == "(") {
        i++; // 跳过"("
        vector<string> args;
        while (WordFinal.Token[i].TokenA != ")") {
            if (WordFinal.Token[i].TokenB == 0) {
                args.push_back(WordFinal.Token[i].TokenA);
                i++;
                if (WordFinal.Token[i].TokenA == ",") i++; // 跳过逗号
            }
        }
        for (const auto& arg : args) {
            addQuaternion("arg", arg, "_", "_");
        }
        addQuaternion("call", varName, to_string(args.size()), "_");
        i++; // 跳过")"
        i++; // 跳过";"
    }
}void parseExpression_for(size_t& i, int& labelCount) {
    string varName = WordFinal.Token[i].TokenA;
    i++;
    if (WordFinal.Token[i].TokenA == "=" || WordFinal.Token[i].TokenA == "+=" || WordFinal.Token[i].TokenA == "-=" || WordFinal.Token[i].TokenA == "*=" || WordFinal.Token[i].TokenA == "/=") {
        string op = WordFinal.Token[i].TokenA;
        i++;
        vector<token> expr;
        while (WordFinal.Token[i].TokenA != ")") {
            expr.push_back(WordFinal.Token[i]);
            i++;
        }
        vector<string> postfix = infixToPostfix(expr);
        string result = evaluatePostfix(postfix);
        if (op == "=") {
            addQuaternion("=", result, "_", varName);
        } else {
            string basicOp = op.substr(0, 1);
            addQuaternion(basicOp, varName, result, varName);
        }
        i++; // 跳过")"


    }
}

void parseBlock(size_t& i, int& labelCount) {
    i++; // 跳过"{"
    while (WordFinal.Token[i].TokenA != "}") {
        if (WordFinal.Token[i].TokenA == "int") {
            parseVariableDeclaration(i);
        } else if (WordFinal.Token[i].TokenA == "if") {
            parseIfStatement(i, labelCount);
        } else if (WordFinal.Token[i].TokenA == "while") {
            parseWhileStatement(i, labelCount);
        } else if (WordFinal.Token[i].TokenA == "for") {
            parseForStatement(i, labelCount);
        } else {
            parseExpression(i, labelCount);
        }
    }
    i++; // 跳过"}"
}

void parseVariableDeclaration(size_t& i) {
    string type = WordFinal.Token[i].TokenA;
    i++;
    cout<< WordFinal.Token[i].TokenA<<"  "<<i<<endl;
    if (WordFinal.Token[i + 1].TokenA == "[") { // 检查是否为数组定义
        string varName = WordFinal.Token[i].TokenA;
        i += 2; // 跳过数组名和"["
        string arraySize = WordFinal.Token[i].TokenA;
        addQuaternion("ARRAY", arraySize, type, varName);
        i += 2; // 跳过数组大小和"]"
        i++; // 跳过分号
    } else if (WordFinal.Token[i + 1].TokenA == "(") { // 检查是否为函数定义
        string funcName = WordFinal.Token[i].TokenA;
        addQuaternion("function_begin", type, "_", funcName);
        i += 2; // 跳过函数名和"("
        while (WordFinal.Token[i].TokenA != ")") {
            if (WordFinal.Token[i].TokenA == "int" || WordFinal.Token[i].TokenA == "char") {
                i++; // 跳过参数类型
                string paramName = WordFinal.Token[i].TokenA;
                addQuaternion("param", "_", "_", paramName);
                i++; // 跳过参数名
                if (WordFinal.Token[i].TokenA == ",") i++; // 跳过逗号
            }
        }
        i++; // 跳过")"
        if (WordFinal.Token[i].TokenA == "{") {
            i++; // 跳过"{"
            while (WordFinal.Token[i].TokenA != "}") {
                if (WordFinal.Token[i].TokenA == "int" || WordFinal.Token[i].TokenA == "char") {
                    i++;
                    string varName = WordFinal.Token[i].TokenA;
                    addQuaternion(type,  varName, "_", "_");
                    i++;
                    if (WordFinal.Token[i].TokenA == "=") {
                        i++;
                        vector<token> expr;
                        while (WordFinal.Token[i].TokenA != ";") {
                            expr.push_back(WordFinal.Token[i]);
                            i++;
                        }
                        vector<string> postfix = infixToPostfix(expr);
                        string result = evaluatePostfix(postfix);
                        addQuaternion("=", result, "_", varName);
                        i++; // 跳过";"
                    }
                } else if (WordFinal.Token[i].TokenA == "return") {
                    i++;
                    string returnVar = WordFinal.Token[i].TokenA;
                    addQuaternion("return", returnVar, "_", "_");
                    i++; // 跳过返回变量
                    i++; // 跳过";"
                }
            }
            addQuaternion("function_end", "_", "_",funcName);
        }
        i++; // 跳过大括号
    } else {
        // 变量定义和表达式处理
        while (i < WordFinal.Token.size() && WordFinal.Token[i].TokenA != ";") {
            if (WordFinal.Token[i].TokenB == 0) {
                string varName = WordFinal.Token[i].TokenA;
                addQuaternion(type, varName, "_", "_" );
                i++;
                if (WordFinal.Token[i].TokenA == "=" || WordFinal.Token[i].TokenA == "+=" || WordFinal.Token[i].TokenA == "-=" || WordFinal.Token[i].TokenA == "*=" || WordFinal.Token[i].TokenA == "/=") {
                    string op = WordFinal.Token[i].TokenA;
                    i++;
                    vector<token> expr;
                    while (WordFinal.Token[i].TokenA != "," && WordFinal.Token[i].TokenA != ";") {
                        expr.push_back(WordFinal.Token[i]);
                        i++;
                    }
                    vector<string> postfix = infixToPostfix(expr);
                    string result = evaluatePostfix(postfix);
                    if (op == "=") {
                        addQuaternion("=", result, "_", varName);
                    } else {
                        string basicOp = op.substr(0, 1);
                        addQuaternion(basicOp, varName, result, varName);
                    }
                }
                if (WordFinal.Token[i].TokenA == ",") {
                    i++;
                }
            }
        }
        i++; // 跳过分号
    }
}
void parseWhileStatement(size_t& i, int& labelCount) {
    i++;
    if (WordFinal.Token[i].TokenA == "(") {
        i++;
        vector<token> expr;
        while (WordFinal.Token[i].TokenA != ")") {
            expr.push_back(WordFinal.Token[i]);
            i++;
        }
        string startLabel = "L" + to_string(labelCount++);
        addQuaternion("LABEL", "_", "_", startLabel);
        vector<string> postfix = infixToPostfix(expr);
        string result = evaluatePostfix(postfix);
        string tempVar = "t" + to_string(labelCount++);
        addQuaternion(postfix.back(), postfix[postfix.size() - 3], postfix[postfix.size() - 2], tempVar);
        string falseLabel = "L" + to_string(labelCount++);
        addQuaternion("JUMP_FALSE", tempVar, "_", falseLabel);
        i++; // 跳过")"
        parseBlock(i, labelCount);
        addQuaternion("JUMP", "_","_", startLabel);
        addQuaternion("LABEL", "_", "_", falseLabel);
    }
}

void parseForStatement(size_t& i, int& labelCount) {
    i++;
    if (WordFinal.Token[i].TokenA == "(") {
        i++;
        if (WordFinal.Token[i].TokenA == "int") {
            parseVariableDeclaration(i);
        } else {
            parseExpression(i, labelCount);
        }

        string labelStart = "L" + to_string(labelCount++);
        string labelEnd = "L" + to_string(labelCount++);
        addQuaternion("LABEL", "_", "_", labelStart);

        vector<token> condExpr;
        while (WordFinal.Token[i].TokenA != ";") {
            condExpr.push_back(WordFinal.Token[i]);
            i++;
        }
        vector<string> condPostfix = infixToPostfix(condExpr);
        string condResult = evaluatePostfix(condPostfix);
        string tempVar = "t" + to_string(labelCount++);
        addQuaternion(condPostfix.back(), condPostfix[condPostfix.size() - 3], condPostfix[condPostfix.size() - 2], tempVar);
        addQuaternion("JUMP_FALSE", tempVar, "_", labelEnd);
        i++; // 跳过";"

        size_t incStart = i;
        vector<token> incExpr;
        while (WordFinal.Token[i].TokenA != ")") {
            incExpr.push_back(WordFinal.Token[i]);
            i++;
        }
        i++; // 跳过")"

        parseBlock(i, labelCount);
        int tmp = i;

        incExpr.clear();
        i = incStart;
        parseExpression_for(i, labelCount);

        addQuaternion("JUMP", "_", "_", labelStart);
        addQuaternion("LABEL", "_", "_", labelEnd);
        i = tmp;
    }
}
void parseMain(size_t& i, int& labelCount) {
    addQuaternion("main_start",  "main", "_","_");
    i++; // 跳过"main"
    i++; // 跳过"("
    i++; // 跳过")"
    i++;
    //cout<<Token[i].TokenA <<endl;
    if (WordFinal.Token[i].TokenA == "{") {
        parseBlock(i, labelCount);
    }
    addQuaternion("main_end", "main", "_", "_");
}

void generateQuaternions() {
    size_t i = 0;
    int labelCount = 1;
    while (i < WordFinal.Token.size()) {
        cout << "i = " << i << endl;
        cout << "WordFinal.Token[" << i << "].TokenA" << WordFinal.Token[i].TokenA << endl;
        if (WordFinal.Token[i].TokenA == "int" && WordFinal.Token[i + 1].TokenA == "main") {
            parseMain(i, labelCount);
        }
        else if ((WordFinal.Token[i].TokenA == "int"&&WordFinal.Token[i].TokenB == 1)|| WordFinal.Token[i].TokenA == "char") {
            parseVariableDeclaration(i);
        } else if (WordFinal.Token[i].TokenA == "if") {
            parseIfStatement(i, labelCount);
        } else if (WordFinal.Token[i].TokenA == "while") {
            parseWhileStatement(i, labelCount);
        } else if (WordFinal.Token[i].TokenA == "for") {
            parseForStatement(i, labelCount);
        } else {
            parseExpression(i, labelCount);
        }
    }
}

void printQuaternions() {
    cout << "printQuaternions ()" << endl;
    for (const auto& q : Quaternion) {
        cout << "{" << "\"" << q.op << "\"" << ", " << "\"" << q.opt1 << "\"" << ", " << "\"" << q.opt2 << "\"" << ", " << "\"" << q.result << "\"" << "}," << endl;
    }
}

void QUA2 () {
    Lexical temp;
    temp = WordFinal;
    cout << "printQuaternions ()" << endl;
    iniOP();
    string code = "int a = 5, b = 10;    b -= a + 2;    int arr[20]; arr[5] = b;";
    generateQuaternions();
    printQuaternions();
}

#endif //TEAM_COMPLIER_QUA2_H
