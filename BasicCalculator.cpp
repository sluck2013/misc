#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

using namespace std;

class Calculator {
    class ExpressionElement {
    public:
        ExpressionElement(int number) : elemNumber(number), elemOperator(0), elemType(NUMBER) {}
        ExpressionElement(char elemOperator) : elemNumber(0), elemOperator(elemOperator), elemType(OPERATOR) {}
        enum ElementType {OPERATOR, NUMBER};
        ElementType elemType;
        char elemOperator;
        int elemNumber;
        friend ostream& operator<<(ostream& os, const ExpressionElement &e) {
            if (e.elemType == OPERATOR) {
                os << e.elemOperator;
            } else {
                os << e.elemNumber;
            }
            return os;
        }
    };
public:
    int calculate(string s) {
        vector<ExpressionElement> expressionStack;
        parseExpression_(expressionStack, s);
        return calcFromExpressionStack_(expressionStack);
    }
private:
    unordered_map<char, int> operatorPriorities {{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}};
    void parseExpression_(vector<ExpressionElement>& expressionStack, 
            const string & expression) {
        vector<char> operatorStack;
        int currentNumber = 0;
        bool hasPendingNumber = false;
        for (size_t i = 0; i <= expression.length(); ++i) {
            if (i == expression.length()) {
                if (hasPendingNumber) {
                    expressionStack.push_back(ExpressionElement(currentNumber));
                }
                moveOperators_(operatorStack, expressionStack, 0);
            } else if (isdigit(expression[i])) {
                currentNumber = appendDigitToNumber_(currentNumber, expression[i]);
                hasPendingNumber = true;
            } else if (isOperator_(expression[i])) {
                if (hasPendingNumber) {
                    expressionStack.push_back(ExpressionElement(currentNumber));
                }
                hasPendingNumber = false;
                currentNumber = 0;
                moveOperators_(operatorStack, expressionStack, 
                        operatorPriorities[expression[i]]);
                operatorStack.push_back(expression[i]);
            } else if (expression[i] == '(') {
                operatorStack.push_back(expression[i]);
            } else if (expression[i] == ')') {
                if (hasPendingNumber) {
                    expressionStack.push_back(ExpressionElement(currentNumber));
                }
                hasPendingNumber = false;
                currentNumber = 0;
                moveOperators_(operatorStack, expressionStack, 0);
            } else if (isspace(expression[i])) {
                //do nothing
            } else {
                //do nothing
            }
        }
    }
    
    inline bool isOperator_(char c) {
        for (const auto &x : operatorPriorities) {
            if (x.first == c) {
                return true;
            }
        }
        return false;
    }
    
    inline int appendDigitToNumber_(int originalNumber, char digit) {
        return 10 * originalNumber + (digit - '0');
    }
    
    void moveOperators_(vector<char> &operatorStack, 
            vector<ExpressionElement> &expressionStack, int priorityUpperLimit) {
        while (!operatorStack.empty()) {
            char top = operatorStack.back();
            if (top == '(') {
                if (priorityUpperLimit < 1) {
                    operatorStack.pop_back();
                }
                break;
            } else {
                if (operatorPriorities[top] >= priorityUpperLimit) {
                    expressionStack.push_back(ExpressionElement(top));
                    operatorStack.pop_back();
                } else {
                    break;
                }
            }
        }
    }
    
    int calcFromExpressionStack_(vector<ExpressionElement>& expressionStack) {
        vector<int> intermediate;
        unordered_map<char, function<int(int, int)> > binOperate;
        initBinOperate_(binOperate);
        for (const auto &e : expressionStack) {
            switch (e.elemType) {
                case ExpressionElement::OPERATOR: {
                        int lastNumber = intermediate.back();
                        intermediate.pop_back();
                        int firstNumber = intermediate.back();
                        intermediate.pop_back();
                        int r = binOperate[e.elemOperator](firstNumber, lastNumber);
                        intermediate.push_back(r);
                    }
                    break;
                case ExpressionElement::NUMBER:
                    intermediate.push_back(e.elemNumber);
                    break;
                default:;
            }
        }
        return intermediate[0];
    }
    
    void initBinOperate_(unordered_map<char, function<int(int, int)>> &binOperate) {
        binOperate['+'] = [](int a, int b){return a + b;};
        binOperate['-'] = [](int a, int b){return a - b;};
        binOperate['*'] = [](int a, int b){return a * b;};
        binOperate['/'] = [](int a, int b){return a / b;};
    }
};

int main()
{
   Calculator calc;
   string s;
   getline(cin, s);
   cout << calc.calculate(s) << endl;
   
   return 0;
}

