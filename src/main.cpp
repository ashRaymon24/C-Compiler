#include "lexer.h"
#include "parser.h"
#include "astnodes.h"

#include <iostream>
#include <string>

static std::string describeExpression(const Expression* expr) {
    if (const auto* literal = dynamic_cast<const IntegerLiteral*>(expr)) {
        return std::to_string(literal->value);
    }

    if (const auto* variable = dynamic_cast<const VariableExpression*>(expr)) {
        return variable->name;
    }

    if (const auto* binary = dynamic_cast<const BinaryExpression*>(expr)) {
        std::string op;
        if (binary->op.lexeme == "+") op = "+";
        else if (binary->op.lexeme == "-") op = "-";
        else if (binary->op.lexeme == "*") op = "*";
        else if (binary->op.lexeme == "/") op = "/";
        else op = binary->op.lexeme;
        return "(" + describeExpression(binary->left) + " " + op + " " + describeExpression(binary->right) + ")";
    }

    return "?";
}

static std::string describeStatement(const Statement* stmt) {
    if (const auto* decl = dynamic_cast<const VariableDeclaration*>(stmt)) {
        return "int " + decl->name + " = " + describeExpression(decl->initializer) + ";";
    }

    if (const auto* ret = dynamic_cast<const ReturnStatement*>(stmt)) {
        return "return " + describeExpression(ret->value) + ";";
    }
	if (const auto* ifStmt = dynamic_cast<const IfStatement*>(stmt)) {
		return "if (" + describeExpression(ifStmt->condition) + ") { ... }";
	}

    return "statement";
}

int main() {
    const std::string source = R"(
int main() {
    int x = 5;

    if (x > 3) {
        return x;
    }

    return 0;
}
)";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    Program program = parser.parse();

    std::cout << "Parsed functions: " << program.functions.size() << "\n";

    for (const Function* function : program.functions) {
        std::cout << "Function: " << function->name << "\n";
        for (const Statement* statement : function->body) {
            std::cout << "  " << describeStatement(statement) << "\n";
        }
    }

    return 0;
}
