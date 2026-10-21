#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <set>
#include <iomanip>

// Structure to hold TAC Instruction details
struct Instruction {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

class TACGenerator {
private:
    std::string expression;
    std::vector<Instruction> quadruples;
    std::set<std::string> symbolTable;
    int tempCount = 1;

    // Operator Precedence Helper
    int getPrecedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    // Check if character is valid operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

public:
    TACGenerator(const std::string& expr) : expression(expr) {}

    // 1. Semantic Validation
    bool validateExpression() {
        int parenBalance = 0;
        bool lastWasOp = true; // Expect operand or '(' at start

        for (size_t i = 0; i < expression.length(); ++i) {
            char c = expression[i];
            if (std::isspace(c)) continue;

            if (c == '(') {
                parenBalance++;
                lastWasOp = true;
            } else if (c == ')') {
                parenBalance--;
                if (parenBalance < 0) return false;
                lastWasOp = false;
            } else if (isOperator(c)) {
                // Catch invalid consecutive operators like "a + * b"
                if (lastWasOp) return false; 
                lastWasOp = true;
            } else if (std::isalnum(c)) {
                lastWasOp = false;
            } else {
                return false; // Unknown token
            }
        }
        return parenBalance == 0 && !lastWasOp;
    }

    // 2. Parser & TAC Generator (Shunting-Yard Infix-to-TAC conversion)
    bool process() {
        if (!validateExpression()) {
            std::cout << "\n[Error] Invalid arithmetic expression! Semantic validation failed.\n";
            return false;
        }

        std::stack<std::string> operands;
        std::stack<char> operators;

        auto applyOp = [&]() {
            if (operands.size() < 2 || operators.empty()) return;
            
            std::string op2 = operands.top(); operands.pop();
            std::string op1 = operands.top(); operands.pop();
            char op = operators.top(); operators.pop();

            std::string temp = "t" + std::to_string(tempCount++);
            
            Instruction inst;
            inst.op = std::string(1, op);
            inst.arg1 = op1;
            inst.arg2 = op2;
            inst.result = temp;
            
            quadruples.push_back(inst);
            operands.push(temp);
        };

        for (size_t i = 0; i < expression.length(); ++i) {
            char c = expression[i];
            if (std::isspace(c)) continue;

            if (std::isalnum(c)) {
                std::string var(1, c);
                // Extract full multi-char identifier or number
                while (i + 1 < expression.length() && std::isalnum(expression[i + 1])) {
                    var += expression[++i];
                }
                operands.push(var);
                symbolTable.insert(var);
            } else if (c == '(') {
                operators.push(c);
            } else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    applyOp();
                }
                if (!operators.empty()) operators.pop(); // Pop '('
            } else if (isOperator(c)) {
                while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(c)) {
                    applyOp();
                }
                operators.push(c);
            }
        }

        while (!operators.empty()) {
            applyOp();
        }

        return true;
    }

    // 3. Output Display Modules
    void displaySymbolTable() {
        std::cout << "\n=========================================\n";
        std::cout << "               SYMBOL TABLE              \n";
        std::cout << "=========================================\n";
        std::cout << std::left << std::setw(15) << "Identifier" << "Type\n";
        std::cout << "-----------------------------------------\n";
        for (const auto& sym : symbolTable) {
            std::string type = std::isdigit(sym[0]) ? "Constant" : "Variable";
            std::cout << std::left << std::setw(15) << sym << type << "\n";
        }
    }

    void displayTAC() {
        std::cout << "\n=========================================\n";
        std::cout << "        THREE ADDRESS CODE (TAC)         \n";
        std::cout << "=========================================\n";
        for (const auto& q : quadruples) {
            std::cout << q.result << " = " << q.arg1 << " " << q.op << " " << q.arg2 << "\n";
        }
    }

    void displayQuadruples() {
        std::cout << "\n=========================================\n";
        std::cout << "              QUADRUPLES                 \n";
        std::cout << "=========================================\n";
        std::cout << std::left << std::setw(8) << "Index" 
                  << std::setw(10) << "Operator" 
                  << std::setw(10) << "Arg1" 
                  << std::setw(10) << "Arg2" 
                  << std::setw(10) << "Result" << "\n";
        std::cout << "-----------------------------------------\n";
        for (size_t i = 0; i < quadruples.size(); ++i) {
            std::cout << std::left << std::setw(8) << ("(" + std::to_string(i) + ")")
                      << std::setw(10) << quadruples[i].op
                      << std::setw(10) << quadruples[i].arg1
                      << std::setw(10) << quadruples[i].arg2
                      << std::setw(10) << quadruples[i].result << "\n";
        }
    }

    void displayTriples() {
        std::cout << "\n=========================================\n";
        std::cout << "                TRIPLES                  \n";
        std::cout << "=========================================\n";
        std::cout << std::left << std::setw(8) << "Index" 
                  << std::setw(10) << "Operator" 
                  << std::setw(12) << "Arg1" 
                  << std::setw(12) << "Arg2" << "\n";
        std::cout << "-----------------------------------------\n";

        for (size_t i = 0; i < quadruples.size(); ++i) {
            std::string a1 = quadruples[i].arg1;
            std::string a2 = quadruples[i].arg2;

            // Replace temporary variable references with pointer indices (x)
            for (size_t j = 0; j < i; ++j) {
                if (a1 == quadruples[j].result) a1 = "(" + std::to_string(j) + ")";
                if (a2 == quadruples[j].result) a2 = "(" + std::to_string(j) + ")";
            }

            std::cout << std::left << std::setw(8) << ("(" + std::to_string(i) + ")")
                      << std::setw(10) << quadruples[i].op
                      << std::setw(12) << a1
                      << std::setw(12) << a2 << "\n";
        }
    }

    void displayIndirectTriples() {
        std::cout << "\n=========================================\n";
        std::cout << "            INDIRECT TRIPLES             \n";
        std::cout << "=========================================\n";
        std::cout << std::left << std::setw(15) << "Pointer Index" << "Triple Index\n";
        std::cout << "-----------------------------------------\n";
        for (size_t i = 0; i < quadruples.size(); ++i) {
            std::cout << std::left << std::setw(15) << ("p[" + std::to_string(i) + "]")
                      << ("(" + std::to_string(i) + ")") << "\n";
        }
    }
};

int main() {
    std::string expr;
    std::cout << "=========================================\n";
    std::cout << " THREE ADDRESS CODE GENERATOR TOOL (TAC) \n";
    std::cout << "=========================================\n";
    std::cout << "Enter an arithmetic expression (e.g., a+b*c or (a+b)*(c-d)): ";
    std::getline(std::cin, expr);

    TACGenerator generator(expr);

    if (generator.process()) {
        generator.displaySymbolTable();
        generator.displayTAC();
        generator.displayQuadruples();
        generator.displayTriples();
        generator.displayIndirectTriples();
    }

    return 0;
}