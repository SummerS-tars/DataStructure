#include <iostream>
#include <stack>
#include <string>

using namespace std;

// 判断运算符优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 计算两个数的运算结果
int calculate(int a, int b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

int evaluateExpression(string expression) {
    stack<int> numbers;
    stack<char> operators;
    
    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];
        
        // 跳过空格
        if (c == ' ') continue;
        
        // 如果是数字，读取完整的数字
        if (isdigit(c)) {
            int num = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            i--; // 因为for循环会再次递增i
            numbers.push(num);
        }
        // 如果是左括号，直接入栈
        else if (c == '(') {
            operators.push(c);
        }
        // 如果是右括号，计算到左括号为止
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                int b = numbers.top(); numbers.pop();
                int a = numbers.top(); numbers.pop();
                char op = operators.top(); operators.pop();
                numbers.push(calculate(a, b, op));
            }
            operators.pop(); // 弹出左括号
        }
        // 如果是运算符
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            // 如果当前运算符优先级低于或等于栈顶运算符，先计算栈顶
            while (!operators.empty() && 
                   operators.top() != '(' && 
                   precedence(operators.top()) >= precedence(c)) {
                int b = numbers.top(); numbers.pop();
                int a = numbers.top(); numbers.pop();
                char op = operators.top(); operators.pop();
                numbers.push(calculate(a, b, op));
            }
            operators.push(c);
        }
    }
    
    // 计算剩余的运算
    while (!operators.empty()) {
        int b = numbers.top(); numbers.pop();
        int a = numbers.top(); numbers.pop();
        char op = operators.top(); operators.pop();
        numbers.push(calculate(a, b, op));
    }
    
    return numbers.top();
}

int main() {
    string expression;
    getline(cin, expression);
    
    int result = evaluateExpression(expression);
    cout << result << endl;
    
    return 0;
}