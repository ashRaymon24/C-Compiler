#include "semanticAnalyser.h"

#include <stdexcept>

void SemanticAnalyser::analyse(Program& program) {
    for (Function* function : program.functions) {
        declaredVariables.clear(); // Clear declared variables for each function
        for (Statement* statement : function->body) {
            analyseStatement(statement);
        }
    }
};
void SemanticAnalyser::analyseStatement(Statement* stmt) {
    if (auto* varDecl = dynamic_cast<VariableDeclaration*>(stmt)) {
        // Check if the variable has already been declared
        if (declaredVariables.find(varDecl->name) != declaredVariables.end()) {
            throw std::runtime_error("Variable '" + varDecl->name + "' is already declared.");
        }
        analyseExpression(varDecl->initializer);
        declaredVariables.insert(varDecl->name);
    } else if (auto* assignStmt = dynamic_cast<AssignmentStatement*>(stmt)) {
        // Check if the variable has been declared before assignment
        if (declaredVariables.find(assignStmt->name) == declaredVariables.end()) {
            throw std::runtime_error("Variable '" + assignStmt->name + "' is not declared.");
        }
        analyseExpression(assignStmt->value);
    } else if (auto* retStmt = dynamic_cast<ReturnStatement*>(stmt)) {
        analyseExpression(retStmt->value);
    } else if (auto* ifStmt = dynamic_cast<IfStatement*>(stmt)) {
        analyseExpression(ifStmt->condition);
        for (Statement* bodyStmt : ifStmt->body) {
            analyseStatement(bodyStmt);
        }
    } else if (auto* whileStmt = dynamic_cast<WhileStatement*>(stmt)) {
        analyseExpression(whileStmt->condition);
        for (Statement* bodyStmt : whileStmt->body) {
            analyseStatement(bodyStmt);
        }
    }
}
void SemanticAnalyser::analyseExpression(Expression* expr) {
    if (auto* varExpr = dynamic_cast<VariableExpression*>(expr)) {
        // Check if the variable has been declared
        if (declaredVariables.find(varExpr->name) == declaredVariables.end()) {
            throw std::runtime_error("Variable '" + varExpr->name + "' is not declared.");
        }
    } else if (auto* binaryExpr = dynamic_cast<BinaryExpression*>(expr)) {
        analyseExpression(binaryExpr->left);
        analyseExpression(binaryExpr->right);
    }
    else if (auto* intLiteral = dynamic_cast<IntegerLiteral*>(expr)) {
        // Integer literals are always valid, no action needed
    }
};