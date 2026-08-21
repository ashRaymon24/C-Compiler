#include "codeGenerator.h" 

void CodeGenerator::emit(const std::string& line) {
    output << line << std::endl;
}

void CodeGenerator::generate(Program* program) {
    output.open("output.s");
    emit(".intel_syntax noprefix");
    emit(".global main");
    emit("");
    for (Function* function : program->functions) {
        
        generateFunction(function);
    }
    output.close();
}
void CodeGenerator::generateFunction(Function* function) {
    emit(function->name + ":");
    for (Statement* stmt : function->body) {
        generateStatement(stmt);
    }
}
void CodeGenerator::generateStatement(Statement* stmt) {
    if (auto* retStmt = dynamic_cast<ReturnStatement*>(stmt)) {
        generateExpression(retStmt->value);
        emit("ret");
    }
}
void CodeGenerator::generateExpression(Expression* expr) {
    if (auto* intLiteral = dynamic_cast<IntegerLiteral*>(expr)) {
        emit("mov rax, " + std::to_string(intLiteral->value));
    } 
    else if (auto* binaryExpr = dynamic_cast<BinaryExpression*>(expr)) {
        generateExpression(binaryExpr->left);
        emit("push rax");
        generateExpression(binaryExpr->right);
        emit("pop rbx");

        if (binaryExpr->op.type == TokenType::PLUS) {
            emit("add rax, rbx");
        } else if (binaryExpr->op.type == TokenType::MINUS) {
            emit("sub rbx, rax");
            emit("mov rax, rbx");
        } else if (binaryExpr->op.type == TokenType::STAR) {
            emit("imul rax, rbx");
        } else if (binaryExpr->op.type == TokenType::SLASH) {
            emit("mov rcx, rax"); // Move dividend to rcx
            emit("mov rax, rbx"); // Move divisor to rax
            emit("cqo");          // Sign-extend rax into rdx:rax
            emit("idiv rcx");     // Divide rcx by rax, result in rax
        }
    }
}